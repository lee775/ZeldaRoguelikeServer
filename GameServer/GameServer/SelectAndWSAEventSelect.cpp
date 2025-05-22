#include "pch.h"
//#include "SelectAndWSAEventSelect.h"
//#include "ThreadManager.h"
//
//// 서버
//// 1) 새로운 소켓 생성 (socket)
//// 2) 소켓에 주소/포트 번호 설정 (bind)
//// 3) 소켓 일 시키기 (listen)
//// 4) 손님 접속 (accept)
//// 5) 클라와 통신
//
//// UDP
//// 1) 새로운 소켓 생성 (socket)
//// 2) 소켓에 주소/포트 번호 설정 (bind)
//// 5) 클라와 통신
//
//// Select 모델 = (select 함수)
//
//// socket set
//// 1) 읽기[] 쓰기[] 예외[]
//// 2) select(readSet, writeSet, exceptSet); -> 관찰 시작
//// 3) 적어도 하나의 소켓이상 준비되어야만 리턴 -> 준비 안되면 제거
//// 4) 남아있는 소켓을 체크해서 진행
//
//// WSAEventSelect = WSAEventSelect가 핵심이 되는
//// 소켓과 관련된 네트워크 이벤트를 [이벤트 객체]를 통해 감지
//// 생성 : WSACreateEvent ( 수동 리셋 + Manual-Reset + Non-Signaled 상태로 시작)
//// 삭제 : WSACloseEvent
//// 신호 상태 감지 : WSAWaitForMultipleEvents
//// 구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents
//
//const int32 BUF_SIZE = 1000;
//
//struct Session
//{
//	// 무조건 첫번째에 둬야함
//	WSAOVERLAPPED overlapped = {};
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUF_SIZE] = {};
//	int32 recvBytes = 0;
//};
//
//enum IO_TYPE
//{
//	READ,
//	WRITE,
//	ACCEPT,
//	CONNECT
//};
//
//// WSAOVERLAPPED 구조체를 내 입맛대로 커스텀
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	// 추가적인 정보
//	int32 type = 0;
//	// TODO
//};
//
//void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
//{
//	cout << "=============================" << endl;
//	cout << "Error = " << error << endl;
//	cout << "Data Recv Len Callback = " << recvLen << endl;
//	cout << "overlapped = " << overlapped << endl;
//	cout << "flags = " << flags << endl;
//	cout << "=============================" << endl;
//
//	// TODO
//}
//
//struct Session2
//{
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUF_SIZE] = {};
//	int32 recvBytes = 0;
//};
//
//void WorkerThreadMain(HANDLE iocpHandle)
//{
//	while (true)
//	{
//		// TODO
//
//		DWORD bytesTransferred = 0;
//		Session2* session = nullptr;
//		OverlappedEx* overlappedEx = nullptr;
//		// GQCS
//		// 큐에 완료된 정보를 가져 오는 함수
//		bool ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, (ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		// 예외 케이스니까 연결 끊어주는 처리 등을 하면 됨
//		if (ret == false || bytesTransferred == 0)
//			continue;
//
//		switch (overlappedEx->type)
//		{
//		case IO_TYPE::READ:
//			cout << "Recv Data Len = " << bytesTransferred << endl;
//			cout << "Recv Data IOCP = " << session->recvBuffer << endl;
//			break;
//		}
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUF_SIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//}
//
//int SelectAndWSAEventSelect::run(SOCKET listen)
//{
//	//WSADATA wsaData;
////if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
////	return 0;
//
////// 1) 소켓 생성
////// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
////// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
////// protocol : 0
////// return : descriptor
////// int32 errCode = ::WSAGetLastError();
////SOCKET listen = ::socket(AF_INET, SOCK_STREAM, 0);
//////SOCKET listen = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
////if (listen == INVALID_SOCKET)
////	return 0;
//
//
////// 소켓 옵션
////// - 1) level (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP)
////// - 2) Option Name ( optname )
////// - 3) Option Value ( optval )
//
////// SO_KEEPALIVE
////bool enable = true;
////::setsockopt(listen, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
//
////// SO_LINGER = 지연하다
//
////// SO_REUSEADDR
////
////// 2) 주소/포트 번호 설정 (bind)
////SOCKADDR_IN serverAddr;
////::memset(&serverAddr, 0, sizeof(serverAddr));
////serverAddr.sin_family = AF_INET;
////serverAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
////serverAddr.sin_port = ::htons(7777); // 80 : HTTP
////if (::bind(listen, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
////	return 0;
//
////// 3) 업무 개시 (listen)
////if (::listen(listen, SOMAXCONN) == SOCKET_ERROR)
////	return 0;
//
////// 4)
////while (true)
////{
////	SOCKADDR_IN clientAddr;
////	::memset(&serverAddr, 0, sizeof(serverAddr));
////	int32 addrLen = sizeof(clientAddr);
//
////	SOCKET clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
////	if (clientSocket == INVALID_SOCKET)
////		return 0;
//
////	//WCHAR ip[16];
////	//::InetNtop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip) / sizeof(WCHAR));
////	//wcout << "Client Connected! IP = " << ip << endl;
//
////	char ip[16];
////	::InetNtopA(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
////	wcout << "Client Connected! IP = " << ip << endl;
//
////	// TODO
////	while(true)
////	{
////		// 패킷
////		char recvBuffer[100];
////		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
////		if (recvLen <= 0)
////			return 0;
//
////		//char recvBuffer[100];
////		//int32 recvLen = ::recvfrom(listen, recvBuffer, sizeof(recvBuffer), 0, (sockaddr*)&clientAddr, &addrLen);
////		//if (recvLen <= 0)
////		//	return 0;
//
////		cout << "Recv Data : " << recvBuffer << endl;
////		cout << "Recv Len : " << recvLen << endl;
//
////		//int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
////		//if (resultCode == SOCKET_ERROR)
////		//	return 0;
//
////		this_thread::sleep_for(1s);
////	}
////}
//
////::closesocket(listen);
////::WSACleanup();
//
//	vector<WSAEVENT> wsaEvents;
//
//	vector<Session> sessions;
//	sessions.reserve(100);
//
//	// 셀렉트
//	//fd_set reads;
//	//fd_set writes;
//
//	//WSAEventSelect
//	WSAEVENT listenEvent = ::WSACreateEvent();
//	wsaEvents.push_back(listenEvent);
//	sessions.push_back(Session{ listen });
//
//	if (::WSAEventSelect(listen, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
//		return 0;
//
//	while (true)
//	{
//		// 셀렉트
//		//// 소켓 셋 초기화
//		//FD_ZERO(&reads);
//
//		//// listen 등록
//		//FD_SET(listen, &reads);
//
//		//// 소켓 등록
//		//for (Session& s : sessions)
//		//	FD_SET(s.socket, &reads);
//
//		//// [옵션] 마지막 timeout 인자 설정 가능
//		//int32 retVal = ::select(0, &reads, nullptr, nullptr, nullptr);
//		//if (retVal == SOCKET_ERROR)
//		//	break;
//
//		//if (FD_ISSET(listen, &reads))
//		//{
//		//	SOCKADDR_IN clientAddr;
//		//	int32 addrLen = sizeof(clientAddr);
//		//	SOCKET clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
//
//		//	if (clientSocket != INVALID_SOCKET)
//		//	{
//		//		if (::WSAGetLastError() == WSAEWOULDBLOCK)
//		//			continue;
//
//		//		cout << "Client Connected" << endl;
//		//		sessions.push_back(Session{ clientSocket });
//		//	}
//		//}
//
//		//// 나머지 소켓도 read
//		//for (Session& s : sessions)
//		//{
//		//	if (FD_ISSET(s.socket, &reads))
//		//	{
//		//		s.recvBytes = ::recv(s.socket, s.recvBuffer, BUF_SIZE, 0);
//		//		if (s.recvBytes <= 0)
//		//		{
//		//			continue;
//		//		}
//
//		//		cout << "RecvData = " << s.recvBuffer << endl;
//		//		cout << "RecvLen = " << s.recvBytes << endl;
//		//	}
//		//}
//
//
//		// WSAEventSelect
//		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
//		if (index == WSA_WAIT_FAILED)
//			continue;
//
//		index -= WSA_WAIT_EVENT_0;
//
//		WSANETWORKEVENTS networkEvents;
//		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
//			continue;
//
//		if (networkEvents.lNetworkEvents & FD_ACCEPT)
//		{
//			// Error-Check
//			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
//				continue;
//
//			SOCKADDR_IN clientAddr;
//			int32 addrLen = sizeof(clientAddr);
//
//			SOCKET clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//			{
//				cout << "Client Connected" << endl;
//
//				WSAEVENT clientEvent = ::WSACreateEvent();
//				wsaEvents.push_back(clientEvent);
//				sessions.push_back(Session{ clientSocket });
//
//				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
//					return 0;
//			}
//		}
//
//		// Client Session 소켓 체크
//		if (networkEvents.lNetworkEvents & FD_READ)
//		{
//			if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
//				continue;
//
//			Session& s = sessions[index];
//
//			// Read
//			s.recvBytes = ::recv(s.socket, s.recvBuffer, BUF_SIZE, 0);
//			if (s.recvBytes == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
//			{
//				if (s.recvBytes <= 0)
//					continue;
//			}
//
//			cout << "Recv Data = " << s.recvBuffer << endl;
//			cout << "Recv Len = " << s.recvBytes << endl;
//		}
//	}
//}
//
//// Overlapped IO ( 비동기 + 논블로킹)
//
//// Reactor Pattern - select와 같이 소켓을 먼저 확인 후 작업을 하는 것
//// Proactor Pattern - overlapped와 같이 작업을 먼저 걸어둔 뒤, 완료되었을때 이벤트 또는 콜백함수로 탐지 할 수 있는 수단을 마련해 작업하는것
//int SelectAndWSAEventSelect::Overlapped(SOCKET listen)
//{
//	// - Overlapped 함수를 건다 (WSARecv, WSASend)
//	// - Overlapped 함수가 성공했는지 확인
//	// -> 성공했으면 결과 얻어서 처리
//	// -> 실패했으면 사유를 확인
//
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket;
//		while (true)
//		{
//			clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//				break;
//
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//				continue;
//
//			// 문제가 있는 상황
//			return 0;
//		}
//
//		// Overlapped Event 방식
//		Session session = Session{ clientSocket };
//		WSAEVENT wsaEvent = ::WSACreateEvent();
//		session.overlapped.hEvent = wsaEvent;
//
//		cout << "Clien Connected" << endl;
//
//		while (true)
//		{
//			WSABUF wsaBuf;
//			wsaBuf.buf = session.recvBuffer;
//			wsaBuf.len = BUF_SIZE;
//
//			DWORD recvLen = 0;
//			DWORD flags = 0;
//			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
//			{
//				if (::WSAGetLastError() == WSA_IO_PENDING)
//				{
//					// TODO
//					// 이벤트가 완료될때까지 기다림
//					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
//					// 이벤트가 완료 되었으면 값을 받아 옴
//					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
//				}
//				else
//				{
//					// TODO
//					break;
//				}
//			}
//			cout << "Data Recv = " << session.recvBuffer << endl;
//		}
//		::WSACloseEvent(wsaEvent);
//	}
//	return 0;
//}
//
//int SelectAndWSAEventSelect::OverlappedCallback(SOCKET listen)
//{
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket;
//		while (true)
//		{
//			clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//				break;
//
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//				continue;
//
//			// 문제가 있는 상황
//			return 0;
//		}
//
//		Session session = Session{ clientSocket };
//
//		while (true)
//		{
//			WSABUF wsaBuf;
//			wsaBuf.buf = session.recvBuffer;
//			wsaBuf.len = BUF_SIZE;
//
//			DWORD recvLen = 0;
//			DWORD flags = 0;
//			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
//			{
//				if (::WSAGetLastError() == WSA_IO_PENDING)
//				{
//
//					::SleepEx(INFINITE, TRUE);
//				}
//				else
//				{
//					// TODO
//					break;
//				}
//			}
//			cout << "Data Recv = " << session.recvBuffer << endl;
//		}
//	}
//	return 0;
//}
//
//
//// 요청한 네트워크통신의 결과를 하나의 큐로 받음
//// 완료된 요청을 처리하기 위해 대기하는 워커스레드가 존재함 - 스레드를 따로 배치해서 언제든지 완료되면 처리 할 수 있게만듬
//int SelectAndWSAEventSelect::IOCP(SOCKET listen)
//{
//	vector<Session2*> sessionManager;
//
//	// 이게 IOCP의 약자임, 큐를 만드는 작업임
//	// CompletionPort - CP
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	// WorkerThreads 생성
//	for (int32 i = 0; i < 5; i++)
//	{
//		GThreadManager->Launch([=]() {WorkerThreadMain(iocpHandle); });
//	}
//
//	// Accept
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listen, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket == INVALID_SOCKET)
//			return 0;
//
//		Session2* session = new Session2();
//		session->socket = clientSocket;
//
//		sessionManager.push_back(session);
//
//		cout << "Client Connected" << endl;
//
//		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, /*Key*/(ULONG_PTR)session, 0);
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUF_SIZE;
//
//		OverlappedEx* overlappedEx = new OverlappedEx();
//		overlappedEx->type = IO_TYPE::READ;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		// IOCP는 다시 콜백함수 부분에 null을 넣어주면 됨
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//
//		// 위와같이 일감을 등록해놓은 상태에서 유저가 세션을 종료하면 댕글링 포인터가 됨
//		// 이를 해결하기 위해 스마트 포인터나 ref를 세어주는 무언가를 만들어줘야함
//	}
//
//	return 0;
//}
