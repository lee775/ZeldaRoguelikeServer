#include "pch.h"
#include "Creature.h"
#include "GameRoom.h"

Creature::Creature()
{

}

Creature::~Creature()
{

}

void Creature::BroadcastAttack(const Protocol::ObjectInfo& targetInfo, const Protocol::ObjectInfo& attackerInfo)
{
	if (room)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Attack(targetInfo, attackerInfo);
		room->Broadcast(sendBuffer);
	}
}
