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
}

void GameRoom::Update()
{
}

void GameRoom::EnterRoom(GameSessionRef session)
{
	PlayerRef player = GameObject::CreatePlayer();

	// ������ ���縦 ����
	session->gameRoom = GetRoomRef();
	session->player = player;
	player->session = session;

	// ������ Ŭ�󿡰� ������ ���������
	{
		SendBufferRef sendBuf = ServerPacketHandler::Make_S_MyPlayer(player->info);
		session->Send(sendBuf);
	}
	// ��� ������Ʈ ���� ����
	{
		Protocol::S_AddObject pkt;

		for (auto& item : _players)
		{
			Protocol::ObjectInfo* info = pkt.add_object();
			*info = item.second->info;
		}
		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* info = pkt.add_object();
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

	// TODO �ű� ������Ʈ ������ ��ε� ĳ����
	{
		Protocol::S_AddObject pkt;

		Protocol::ObjectInfo* info = pkt.add_object();
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

	// TODO �ű� ������Ʈ ���� ����
	{
		Protocol::S_RemoveObject pkt;

		pkt.add_id(id);

		SendBufferRef sendBuf = ServerPacketHandler::Make_S_RemoveObject(pkt);
		Broadcast(sendBuf);

	}
}

void GameRoom::Broadcast(SendBufferRef& sendBuffer)
{
	for (auto& item : _players)
	{
		item.second->session->Send(sendBuffer);
	}
}
