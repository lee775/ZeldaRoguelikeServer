#include "pch.h"
#include "Player.h"
#include "GameRoom.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Attack()
{
	GameObjectRef gameObject = room->GetGameObjectAt(GetFrontCellPos());
	if (gameObject && gameObject->info.objecttype() == Protocol::OBJECT_TYPE_MONSTER)
	{
		gameObject->info.set_hp(gameObject->info.hp() - info.attack());
		BroadcastAttack(gameObject->info, info);

		if (gameObject->info.hp() <= 0)
			room->RemoveObject(gameObject->info.objectid());
	}
}
