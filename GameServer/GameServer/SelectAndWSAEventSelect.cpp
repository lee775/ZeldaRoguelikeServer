#include "pch.h"
//#include "SelectAndWSAEventSelect.h"
//#include "ThreadManager.h"
//
//// ����
//// 1) ���ο� ���� ���� (socket)
//// 2) ���Ͽ� �ּ�/��Ʈ ��ȣ ���� (bind)
//// 3) ���� �� ��Ű�� (listen)
//// 4) �մ� ���� (accept)
//// 5) Ŭ��� ���
//
//// UDP
//// 1) ���ο� ���� ���� (socket)
//// 2) ���Ͽ� �ּ�/��Ʈ ��ȣ ���� (bind)
//// 5) Ŭ��� ���
//
//// Select �� = (select �Լ�)
//
//// socket set
//// 1) �б�[] ����[] ����[]
//// 2) select(readSet, writeSet, exceptSet); -> ���� ����
//// 3) ��� �ϳ��� �����̻� �غ�Ǿ�߸� ���� -> �غ� �ȵǸ� ����
//// 4) �����ִ� ������ üũ�ؼ� ����
//
//// WSAEventSelect = WSAEventSelect�� �ٽ��� �Ǵ�
//// ���ϰ� ���õ� ��Ʈ��ũ �̺�Ʈ�� [�̺�Ʈ ��ü]�� ���� ����
//// ���� : WSACreateEvent ( ���� ���� + Manual-Reset + Non-Signaled ���·� ����)
//// ���� : WSACloseEvent
//// ��ȣ ���� ���� : WSAWaitForMultipleEvents
//// ��ü���� ��Ʈ��ũ �̺�Ʈ �˾Ƴ��� : WSAEnumNetworkEvents
//
//const int32 BUF_SIZE = 1000;
//
//struct Session
//{
//	// ������ ù��°�� �־���
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
//// WSAOVERLAPPED ����ü�� �� �Ը���� Ŀ����
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	// �߰����� ����
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
//		// ť�� �Ϸ�� ������ ���� ���� �Լ�
//		bool ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, (ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		// ���� ���̽��ϱ� ���� �����ִ� ó�� ���� �ϸ� ��
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
////// 1) ���� ����
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
////// ���� �ɼ�
////// - 1) level (SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP)
////// - 2) Option Name ( optname )
////// - 3) Option Value ( optval )
//
////// SO_KEEPALIVE
////bool enable = true;
////::setsockopt(listen, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
//
////// SO_LINGER = �����ϴ�
//
////// SO_REUSEADDR
////
////// 2) �ּ�/��Ʈ ��ȣ ���� (bind)
////SOCKADDR_IN serverAddr;
////::memset(&serverAddr, 0, sizeof(serverAddr));
////serverAddr.sin_family = AF_INET;
////serverAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
////serverAddr.sin_port = ::htons(7777); // 80 : HTTP
////if (::bind(listen, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
////	return 0;
//
////// 3) ���� ���� (listen)
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
////		// ��Ŷ
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
//	// ����Ʈ
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
//		// ����Ʈ
//		//// ���� �� �ʱ�ȭ
//		//FD_ZERO(&reads);
//
//		//// listen ���
//		//FD_SET(listen, &reads);
//
//		//// ���� ���
//		//for (Session& s : sessions)
//		//	FD_SET(s.socket, &reads);
//
//		//// [�ɼ�] ������ timeout ���� ���� ����
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
//		//// ������ ���ϵ� read
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
//		// Client Session ���� üũ
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
//// Overlapped IO ( �񵿱� + ����ŷ)
//
//// Reactor Pattern - select�� ���� ������ ���� Ȯ�� �� �۾��� �ϴ� ��
//// Proactor Pattern - overlapped�� ���� �۾��� ���� �ɾ�� ��, �Ϸ�Ǿ����� �̺�Ʈ �Ǵ� �ݹ��Լ��� Ž�� �� �� �ִ� ������ ������ �۾��ϴ°�
//int SelectAndWSAEventSelect::Overlapped(SOCKET listen)
//{
//	// - Overlapped �Լ��� �Ǵ� (WSARecv, WSASend)
//	// - Overlapped �Լ��� �����ߴ��� Ȯ��
//	// -> ���������� ��� �� ó��
//	// -> ���������� ������ Ȯ��
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
//			// ������ �ִ� ��Ȳ
//			return 0;
//		}
//
//		// Overlapped Event ���
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
//					// �̺�Ʈ�� �Ϸ�ɶ����� ��ٸ�
//					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
//					// �̺�Ʈ�� �Ϸ� �Ǿ����� ���� �޾� ��
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
//			// ������ �ִ� ��Ȳ
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
//// ��û�� ��Ʈ��ũ����� ����� �ϳ��� ť�� ����
//// �Ϸ�� ��û�� ó���ϱ� ���� ����ϴ� ��Ŀ�����尡 ������ - �����带 ���� ��ġ�ؼ� �������� �Ϸ�Ǹ� ó�� �� �� �ְԸ���
//int SelectAndWSAEventSelect::IOCP(SOCKET listen)
//{
//	vector<Session2*> sessionManager;
//
//	// �̰� IOCP�� ������, ť�� ����� �۾���
//	// CompletionPort - CP
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	// WorkerThreads ����
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
//		// IOCP�� �ٽ� �ݹ��Լ� �κп� null�� �־��ָ� ��
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//
//		// ���Ͱ��� �ϰ��� ����س��� ���¿��� ������ ������ �����ϸ� ��۸� �����Ͱ� ��
//		// �̸� �ذ��ϱ� ���� ����Ʈ �����ͳ� ref�� �����ִ� ���𰡸� ����������
//	}
//
//	return 0;
//}
