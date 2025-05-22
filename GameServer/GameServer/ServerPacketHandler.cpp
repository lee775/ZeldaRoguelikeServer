#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	default:
		break;
	}
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
	cout << "wawaww ID : " << pkt.id() << endl;
	cout << "wawaww hp : " << pkt.hp() << endl;
	cout << "wawaww att000 : " << pkt.attack() << endl;
	return MakeSendBuffer(pkt, S_TEST);
}
