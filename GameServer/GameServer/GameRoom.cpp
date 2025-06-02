#include "pch.h"
#include "GameRoom.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"

GameRoomRef GRoom = make_shared<GameRoom>();

GameRoom::GameRoom()
{

}

void GameRoom::Init()
{
	MonsterRef monster = GameObject::CreateMonster();
	monster->info.set_posx(8);
	monster->info.set_posy(8);
	AddObject(monster);

	_tilemap.LoadFile(L"C:\\WookChae\\GameServer\\GameServer\\GameCoding\\GameCoding\\Resources\\Tilemap\\Tilemap01.txt");
}

void GameRoom::Update()
{
	for (auto& item : _players)
	{
		item.second->Update();
	}

	for (auto& item : _monsters)
	{
		item.second->Update();
	}
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = GameObject::CreatePlayer();

	// 서로의 존재를 연결
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	// 입장한 클라에게 정보를 보내줘야함
	{
		SendBufferRef sendBuf = ServerPacketHandler::Make_S_MyPlayer(player->info);
		session->Send(sendBuf);
	}
	// 모든 오브젝트 정보 전송
	{
		Protocol::S_AddObject pkt;

		for (auto& item : _players)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}
		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* info = pkt.add_objects();
			*info = item.second->info;
		}

		SendBufferRef sendBuf = ServerPacketHandler::Make_S_AddObject(pkt);
		session->Send(sendBuf);
	}

	AddObject(player);
}

void GameRoom::LeaveRoom(GameSessionRef session)
{
	if (session == nullptr)
		return;

	if (session->player.lock() == nullptr)
		return;

	uint64 id = session->player.lock()->info.objectid();
	RemoveObject(id);
}

GameObjectRef GameRoom::FindObject(uint64 id)
{
	{
		auto findIt = _players.find(id);
		if (findIt != _players.end())
			return findIt->second;
	}
	{
		auto findIt = _monsters.find(id);
		if (findIt != _monsters.end())
			return findIt->second;
	}
	return nullptr;
}

void GameRoom::Handle_C_Move(Protocol::C_Move& pkt)
{
	uint64 id = pkt.info().objectid();
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	// TOOD : Validation
	gameObject->info.set_state(pkt.info().state());
	gameObject->info.set_dir(pkt.info().dir());
	gameObject->info.set_posx(pkt.info().posx());
	gameObject->info.set_posy(pkt.info().posy());
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(pkt.info());
		Broadcast(sendBuffer);
	}
	if (gameObject->info.state() == SKILL)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(gameObject);
		if (player)
			player->Attack();
	}
}

void GameRoom::AddObject(GameObjectRef gameObject)
{
	uint64 id = gameObject->info.objectid();
	auto objectType = gameObject->info.objecttype();

	switch (objectType)
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		_players[id] = static_pointer_cast<Player>(gameObject);
		break;
	case Protocol::OBJECT_TYPE_MONSTER:
		_monsters[id] = static_pointer_cast<Monster>(gameObject);
		break;
	default:
		return;
	}

	gameObject->room = GetRoomRef();

	// TODO 신규 오브젝트 정보를 브로드 캐스팅
	{
		Protocol::S_AddObject pkt;

		Protocol::ObjectInfo* info = pkt.add_objects();
		*info = gameObject->info;

		SendBufferRef sendBuf = ServerPacketHandler::Make_S_AddObject(pkt);
		Broadcast(sendBuf);

	}
}

void GameRoom::RemoveObject(uint64 id)
{
	GameObjectRef gameObject = FindObject(id);
	if (gameObject == nullptr)
		return;

	{
		Protocol::S_RemoveObject pkt;

		pkt.add_id(id);

		SendBufferRef sendBuf = ServerPacketHandler::Make_S_RemoveObject(pkt);
		Broadcast(sendBuf);

	}

	switch (gameObject->info.objecttype())
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		_players.erase(id);
		break;
	case Protocol::OBJECT_TYPE_MONSTER:
		_monsters.erase(id);
		break;
	default:
		return;
	}

	gameObject->room = nullptr;
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
	}
}

PlayerRef GameRoom::FindClosestPlayer(VectorInt cellPos)
{
	float best = FLT_MAX;
	PlayerRef ret = nullptr;

	for (auto& item : _players)
	{

		PlayerRef player = item.second;
		if (player)
		{
			VectorInt dir = cellPos - player->GetCellPos();
			float dist = dir.LengthSquared();
			if (dist < best)
			{
				best = dist;
				ret = player;
			}

		}
	}
	return ret;
}

bool GameRoom::FindPath(VectorInt src, VectorInt dest, vector<VectorInt>& path, int32 maxDepth)
{
	int32 depth = abs(dest.y - src.y) + abs(dest.x - src.x);
	if (depth >= maxDepth)
		return false;

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	map<VectorInt, int32> best;
	map<VectorInt, VectorInt> parent;

	bool found = false;

	// 초기값
	{
		int32 cost = abs(dest.y - src.y) + abs(dest.x - src.x);

		pq.push(PQNode(cost, src));
		best[src] = cost;
		parent[src] = src;
	}

	VectorInt front[4] =
	{
		{0, -1},
		{0, 1},
		{-1, 0},
		{1, 0},
	};

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다.
		PQNode node = pq.top();
		pq.pop();

		// 더 짧은 경로를 이미 찾았다면 스킵
		if (best[node.pos] < node.cost)
			continue;

		// 목적지에 도착했으면 바로 종료
		if (node.pos == dest)
		{
			found = true;
			break;
		}

		// 방문
		for (int32 dir = 0; dir < 4; dir++)
		{
			VectorInt nextPos = node.pos + front[dir];

			if (CanGo(nextPos) == false)
				continue;

			int32 depth = abs(src.y - nextPos.y) + abs(src.x - nextPos.x);
			if (depth >= maxDepth)
				continue;

			int32 cost = abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x);
			int32 bestCost = best[nextPos];
			if (bestCost != 0)
			{
				// 다른 경로에서 더 빠른 길을 찾았으면 스킵
				if (bestCost <= cost)
					continue;
			}

			// 예약 진행
			best[nextPos] = cost;
			pq.push(PQNode(cost, nextPos));
			parent[nextPos] = node.pos;
		}
	}

	if (!found)
	{
		// 목적지 까지의 경로를 찾기 못하면 찾은 곳 까지라도 이동하도록 설정
		float bestScore = FLT_MAX;

		for (auto& item : best)
		{
			VectorInt pos = item.first;
			int32 score = item.second;

			// 동점이라면 최초 위치에서 가장 덜 이동하는 쪽으로
			if (bestScore == score)
			{
				int32 dist1 = abs(dest.x - src.x) + abs(dest.y - src.y);
				int32 dist2 = abs(pos.x - src.x) + abs(pos.y - src.y);
				if (dist1 > dist2)
					dest = pos;
			}
			else if (bestScore > score)
			{
				dest = pos;
				bestScore = score;
			}
		}
	}

	path.clear();
	VectorInt pos = dest;

	while (true)
	{
		path.push_back(pos);

		// 시작점
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(path.begin(), path.end());

	return true;
}

bool GameRoom::CanGo(VectorInt cellPos)
{
	Tile* tile = _tilemap.GetTileAt(cellPos);
	if (tile == nullptr)
		return false;

	// 몬스터 충돌?
	if (GetGameObjectAt(cellPos) != nullptr)
		return false;

	return tile->value != 1;
}

VectorInt GameRoom::GetRandomEmptyCellPos()
{
	VectorInt ret = { -1,-1 };

	VectorInt size = _tilemap.GetMapSize();

	// 몇번이나 시도 할지는 정책을 정해야함 (매우 오래 걸릴 수 있음)
	while (true)
	{
		int32 x = rand() % size.x;
		int32 y = rand() % size.y;
		VectorInt cellPos{ x,y };

		if (CanGo(cellPos))
			return cellPos;
	}


	return ret;
}

GameObjectRef GameRoom::GetGameObjectAt(VectorInt cellPos)
{
	for (auto& item : _players)
	{
		if (item.second->GetCellPos() == cellPos)
			return item.second;
	}

	for (auto& item : _monsters)
	{
		if (item.second->GetCellPos() == cellPos)
			return item.second;
	}

	return nullptr;
}