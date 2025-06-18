#pragma once
#include "Tilemap.h"

struct PQNode
{
	PQNode(int32 cost, VectorInt pos) : cost(cost), pos(pos) {}

	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	VectorInt pos;
};

class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	USE_LOCK
	GameRoom();

	void Init();
	void Update();

	void EnterRoom(GameSessionRef session);
	void LeaveRoom(GameSessionRef session);
	GameObjectRef FindObject(uint64 id);

	GameRoomRef GetRoomRef() { return shared_from_this(); }

public:
	// PacketHandler
	void Handle_C_Move(Protocol::C_Move& pkt);


public:
	void AddObject(GameObjectRef gameObject);
	void RemoveObject(uint64 id);
	void Broadcast(SendBufferRef& sendBuffer);

public:
	PlayerRef FindClosestPlayer(VectorInt cellPos);
	bool FindPath(VectorInt src, VectorInt dest, vector<VectorInt>& path, int32 maxDepth = 10);
	bool CanGo(VectorInt cellPos);
	VectorInt GetRandomEmptyCellPos();
	GameObjectRef GetGameObjectAt(VectorInt cellPos);

private:
	map<uint64, PlayerRef> _players;
	map<uint64, MonsterRef> _monsters;

	TileMap _tilemap;
};

extern GameRoomRef GRoom;