#pragma once

class Player;
class Monster;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update();

	static PlayerRef CreatePlayer();
	static MonsterRef CreateMonster();

	void SetState(ObjectState state, bool broadcast = false);
	void SetDir(Dir dir, bool broadcast = false);

	bool HasReachedDest();
	bool CanGo(VectorInt cellPos);
	Dir GetLookAtDir(VectorInt cellPos);

	void SetCellPos(VectorInt cellPos, bool broadcast = false);
	VectorInt GetFrontCellPos();
	VectorInt GetCellPos();
	int64 GetObjectId() { return info.objectid(); }
	void SetObjectId(int64 id) { info.set_objectid(id); }

	void BroadcastMove();

public:
	Protocol::ObjectInfo info;
	GameRoomRef room;

private:
	static atomic<uint64> s_idGenerator;
};

