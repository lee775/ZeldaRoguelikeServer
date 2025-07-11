#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "Listener.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

int main()
{
	SocketUtils::Init();
	GRoom->Init();

	// 기존 iocp 서버 코어 부분

	//ServerServiceRef service = make_shared<ServerService>(
	//	NetAddress(L"127.0.0.1", 7777),
	//	make_shared<IocpCore>(),
	//	[]() {
	//		ServerLibMode mode = ServerLibMode::iocp;
	//		return make_shared<GameSession>(mode);
	//	},
	//	100
	//);

	//assert(service->Start());

	//for (int32 i = 0; i < 5; i++)
	//{
	//	GThreadManager->Launch([=]()
	//		{
	//			while (true)
	//			{
	//				service->GetIocpCore()->Dispatch(0);
	//			}
	//		});
	//}
	//GThreadManager->Launch([=]()
	//	{
	//		while (true)
	//		{
	//			GRoom->Update();
	//		}
	//	});

	// boost 서버 코어 부분


	BoostServiceRef service = make_shared<BoostService>(
		NetAddress(L"127.0.0.1", 7777),
		[]() {
			ServerLibMode mode = ServerLibMode::boost;
			return make_shared<GameSession>(mode);
		},
		100
	);

	assert(service->BoostInit());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->Start();
				}
			});
	}
	GThreadManager->Launch([=]()
		{
			while (true)
			{
				GRoom->Update();
			}
		});

	GThreadManager->Join();

	// 윈속 종료
	SocketUtils::Clear();
}