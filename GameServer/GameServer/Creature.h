#pragma once

#include "GameObject.h"

class Creature : public GameObject
{
public:
	Creature();
	virtual ~Creature();

	void BroadcastAttack(const Protocol::ObjectInfo& targetInfo, const Protocol::ObjectInfo& attackerInfo);
};

