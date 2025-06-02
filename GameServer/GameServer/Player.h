#pragma once
#include "Creature.h"

class Player : public Creature
{
	using Super = Creature;
public:
	Player();
	virtual ~Player();

	void Attack();

public:
	GameSessionRef session;
};

