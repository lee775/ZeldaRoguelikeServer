#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "GameRoom.h"
#include "ServerPacketHandler.h"

atomic<uint64> GameObject::s_idGenerator = 1;

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

PlayerRef GameObject::CreatePlayer()
{
	PlayerRef player = make_shared<Player>();
	player->info.set_objectid(s_idGenerator++);
	player->info.set_objecttype(Protocol::OBJECT_TYPE_PLAYER);

	//TEMP
	player->info.set_posx(5);
	player->info.set_posy(5);

	return player;
}

MonsterRef GameObject::CreateMonster()
{
	MonsterRef monster = make_shared<Monster>();
	monster->info.set_objectid(s_idGenerator++);
	monster->info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);

	return monster;
}

void GameObject::SetState(ObjectState state, bool broadcast)
{
	if (info.state() == state)
		return;

	info.set_state(state);

	if (broadcast)
		BroadcastMove();
}

void GameObject::SetDir(Dir dir, bool broadcast)
{
	info.set_dir(dir);

	if (broadcast)
		BroadcastMove();
}

bool GameObject::HasReachedDest()
{
	return false;
}

bool GameObject::CanGo(VectorInt cellPos)
{
	if (room == nullptr)
		return false;

	return room->CanGo(cellPos);
}

Dir GameObject::GetLookAtDir(VectorInt cellPos)
{
	VectorInt dir = cellPos - GetCellPos();
	if (dir.x > 0)
		return DIR_RIGHT;
	else if (dir.x < 0)
		return DIR_LEFT;
	else if (dir.y > 0)
		return DIR_DOWN;
	else
		return DIR_UP;
}

void GameObject::SetCellPos(VectorInt cellPos, bool broadcast)
{
	info.set_posx(cellPos.x);
	info.set_posy(cellPos.y);

	if (broadcast)
		BroadcastMove();
}

VectorInt GameObject::GetFrontCellPos()
{
	VectorInt pos = GetCellPos();

	switch (info.dir())
	{
	case DIR_UP:
		return GetCellPos() + VectorInt{ 0,-1 };
	case DIR_DOWN:
		return GetCellPos() + VectorInt{ 0,1 };
	case DIR_LEFT:
		return GetCellPos() + VectorInt{ -1,0 };
	case DIR_RIGHT:
		return GetCellPos() + VectorInt{ 1,0 };
	}

	return pos;
}

VectorInt GameObject::GetCellPos()
{
	return VectorInt{ info.posx(), info.posy() };
}

void GameObject::BroadcastMove()
{
	if (room)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Move(info);
		room->Broadcast(sendBuffer);
	}
}
