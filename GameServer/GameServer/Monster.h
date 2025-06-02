#pragma once
#include "Creature.h"

class Monster : public Creature
{
	using Super = Creature;
public:
	Monster();
	virtual ~Monster() override;

	virtual void Update();
private:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();

protected:
	uint64 _waitMoveUntil = 0;
	uint64 _waitAttackUntil = 0;
	weak_ptr<Player> _target;
};

