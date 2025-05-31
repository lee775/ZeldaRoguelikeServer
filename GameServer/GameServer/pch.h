#pragma once

#include "CorePch.h"

using GameSessionRef = shared_ptr<class GameSession>;
using GameRoomRef = shared_ptr<class GameRoom>;

using GameObjectRef = shared_ptr<class GameObject>;
using CreatureRef = shared_ptr<class Creature>;
using PlayerRef = shared_ptr<class Player>;
using MonsterRef = shared_ptr<class Monster>;

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#endif

#include "ServerPacketHandler.h"
