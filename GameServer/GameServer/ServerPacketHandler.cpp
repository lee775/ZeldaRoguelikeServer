#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameRoom.h"

void ServerPacketHandler::HandlePacket(GameSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	case C_Move:
		Handle_C_Move(session, buffer, len);
	default:
		break;
	}
}

void ServerPacketHandler::Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	uint16 size = header->size;

	Protocol::C_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	// TODO
	GameRoomRef gameRoom = session->gameRoom.lock();
	if (gameRoom)
		gameRoom->Handle_C_Move(pkt);
}

//SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs)
//{
//	SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
//
//	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->Capacity());
//
//	PacketHeader* header = bw.Reserve<PacketHeader>();
//	// id(uint64), 체력(uint32), 공격력(uint16)
//	bw << id << hp << attack;
//
//	// 가변 데이터
//	bw << (uint16)buffs.size();
//	for (BuffData& buff : buffs)
//	{
//		bw << buff.buffId << buff.remainTime;
//	}
//
//	header->size = bw.WriteSize();
//	header->id = S_TEST; // 1 : Test Msg
//
//	sendBuffer->Close(bw.WriteSize());
//
//	return sendBuffer;
//}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs)
{
	Protocol::S_TEST pkt;

	pkt.set_id(10);
	pkt.set_hp(100);
	pkt.set_attack(10);

	{
		Protocol::BuffData* data = pkt.add_buffs();
		data->set_buffid(100);
		data->set_remaintime(1.2f);
		{
			data->add_victims(10);
		}
	}
	{
		Protocol::BuffData* data = pkt.add_buffs();
		data->set_buffid(200);
		data->set_remaintime(2.2f);
		{
			data->add_victims(20);
		}
	}
	//cout << "wawaww ID : " << pkt.id() << endl;
	//cout << "wawaww hp : " << pkt.hp() << endl;
	//cout << "wawaww att000 : " << pkt.attack() << endl;
	return MakeSendBuffer(pkt, S_TEST);
}

SendBufferRef ServerPacketHandler::Make_S_EnterGame()
{
	Protocol::S_EnterGame pkt;

	pkt.set_success(true);
	pkt.set_accountid(0);

	return MakeSendBuffer(pkt, S_EnterGame);
}

SendBufferRef ServerPacketHandler::Make_S_MyPlayer(const Protocol::ObjectInfo& info)
{
	Protocol::S_MyPlayer pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	return MakeSendBuffer(pkt, S_MyPlayer);
}

SendBufferRef ServerPacketHandler::Make_S_AddObject(const Protocol::S_AddObject& pkt)
{
	return MakeSendBuffer(pkt, S_AddObject);
}

SendBufferRef ServerPacketHandler::Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt)
{
	return MakeSendBuffer(pkt, S_RemoveObject);
}

SendBufferRef ServerPacketHandler::Make_S_Move(const Protocol::ObjectInfo& info)
{
	Protocol::S_Move pkt;

	Protocol::ObjectInfo* objectInfo = pkt.mutable_info();
	*objectInfo = info;

	return MakeSendBuffer(pkt, S_Move);
}

SendBufferRef ServerPacketHandler::Make_S_Attack(const Protocol::ObjectInfo& targetInfo, const Protocol::ObjectInfo& attackerInfo)
{
	Protocol::S_Attack pkt;

	Protocol::ObjectInfo* tInfo = pkt.mutable_targetinfo();
	Protocol::ObjectInfo* aInfo = pkt.mutable_attackerinfo();

	*tInfo = targetInfo;
	*aInfo = attackerInfo;

	return MakeSendBuffer(pkt, S_Attack);
}
