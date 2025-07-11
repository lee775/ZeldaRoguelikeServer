#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

/*--------------
	Session
---------------*/

Session::Session(ServerLibMode mode) : _connectEvent(EventType::Connect), _disconnectEvent(EventType::Disconnect), _recvEvent(EventType::Recv), _recvBuffer(BUFFER_SIZE), _sendEvent(EventType::Send), _serverMode(mode)
{
	if (_serverMode == ServerLibMode::iocp)
		_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(SendBufferRef sendBuffer)
{
	//// 생각할 문제
	//// 1) 버퍼 관리?
	//// 2) sendEvent 관리? 단일? 여러개? WSASend 중첩?

	//// TEMP
	//IocpEvent* sendEvent = new IocpEvent(EventType::Send);
	//sendEvent->owner = shared_from_this(); // ADD_REF
	//sendEvent->buffer.resize(len);
	//::memcpy(sendEvent->buffer.data(), buffer, len);
	bool registerSend = false;
	{
		WRITE_LOCK;
		_sendQueue.push(sendBuffer);
		registerSend = _sendResitered.exchange(true) == false;
	}

	if (registerSend)
	{
		if (_serverMode == ServerLibMode::iocp)
			RegisterSend();
		else if (_serverMode == ServerLibMode::boost)
			BoostRegisterSend();

	}
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnect : " << cause << endl;

	OnDisconnected(); // 컨텐츠 코드에서 재정의
	GetService()->ReleaseSession(GetSessionRef());

	if (_serverMode == ServerLibMode::iocp)
		RegisterDisconnect();
	else if (_serverMode == ServerLibMode::boost)
		BoostRegisterDisconnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->type)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(_socket, 0/*남는거*/) == false)
		return false;

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this(); // ADD_REF

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_connectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisconnect()
{
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this(); // ADD_REF

	if (false == SocketUtils::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_disconnectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); // ADD_REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this(); // ADD_REF
	_sendEvent.sendBuffers.clear();

	// 보낼 데이터를 sendEvent에 등록
	{
		WRITE_LOCK;

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO : 예외 체크

			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	// Scatter-Gather (흩어져 있는 데이터를 모아서 한 방에 보낸다)
	vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_sendEvent.owner = nullptr; // RELEASE_REF
			_sendEvent.sendBuffers.clear(); // RELESE_REF
			_sendResitered.store(false);
		}
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; // RELEASE_REF

	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 재정의
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	_disconnectEvent.owner = nullptr; // RELEASE_REF
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr; // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"OnWirte Overflow");
	}

	int32 dataSize = _recvBuffer.DataSize();
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize); // 컨텐츠 코드에서 재정의
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	// 커서 정리
	_recvBuffer.Clean();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	_sendEvent.owner = nullptr; // RELEASE_REF
	_sendEvent.sendBuffers.clear(); // RELESE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// 컨텐츠 코드에서 재정의
	OnSend(numOfBytes);

	//WRITE_LOCK;
	//if (_sendQueue.empty())
	//	_sendResitered.store(false);
	//else
	//	RegisterSend();

	bool registerSend = false;

	{
		WRITE_LOCK;
		if (_sendQueue.empty())
			_sendResitered.store(false);
		else
			registerSend = true;
	}

	if (registerSend)
		RegisterSend();
}

void Session::BoostProcessConnect()
{
	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 재정의
	OnConnected();

	// 수신 등록
	BoostRegisterRecv();
}

void Session::BoostRegisterRecv()
{
	if (IsConnected() == false)
		return;


	_boostSocket.value().async_read_some(boost::asio::buffer(_recvBuffer.WritePos(), _recvBuffer.FreeSize()), [this](boost::system::error_code ec, std::size_t len) {
		if (len == 0)
		{
			Disconnect(L"Recv 0");
			return;
		}

		if (_recvBuffer.OnWrite(len) == false)
		{
			Disconnect(L"OnWirte Overflow");
		}

		int32 dataSize = _recvBuffer.DataSize();
		int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize); // 컨텐츠 코드에서 재정의
		if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
		{
			Disconnect(L"OnRead Overflow");
			return;
		}

		// 커서 정리
		_recvBuffer.Clean();

		// 수신 등록
		BoostRegisterRecv();
		});
}

void Session::BoostRegisterSend()
{
	if (IsConnected() == false)
		return;

	// 보낼 데이터를 sendEvent에 등록
	{
		WRITE_LOCK;

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO : 예외 체크

			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	// Scatter-Gather (흩어져 있는 데이터를 모아서 한 방에 보낸다)
	vector<boost::asio::const_buffer> buffers;
	buffers.reserve(_sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		buffers.push_back(boost::asio::buffer(sendBuffer->Buffer(), sendBuffer->WriteSize()));
	}

	boost::asio::async_write(_boostSocket.value(), buffers, [this](boost::system::error_code ec, size_t len) {
		_sendEvent.owner = nullptr; // RELEASE_REF
		_sendEvent.sendBuffers.clear(); // RELESE_REF

		if (len == 0)
		{
			Disconnect(L"Send 0");
			return;
		}

		// 컨텐츠 코드에서 재정의
		OnSend(len);

		bool registerSend = false;

		{
			WRITE_LOCK;
			if (_sendQueue.empty())
				_sendResitered.store(false);
			else
				registerSend = true;
		}

		if (registerSend)
			BoostRegisterSend();
		});
}

void Session::BoostRegisterDisconnect()
{
	boost::system::error_code ec;

	_boostSocket.value().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	if (ec) cerr << "shutdown 실패 : " << ec.message() << endl;

	_boostSocket.value().close(ec);
	if (ec) cerr << "close 실패 : " << ec.message() << endl;
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}

void Session::InitBoostSocket(boost::asio::io_context& io)
{
	_boostSocket.emplace(io);
}

boost::asio::ip::tcp::socket& Session::GetBoostSocket()
{
	assert(_boostSocket.has_value()); // 생성되었는지 확인
	return _boostSocket.value();
}


/*-----------------
	PacketSession
------------------*/

PacketSession::PacketSession(ServerLibMode mode) : Session(mode)
{
}

PacketSession::~PacketSession()
{
}

// [size(2)][id(2)][data....][size(2)][id(2)][data....]
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processLen = 0;

	while (true)
	{
		int32 dataSize = len - processLen;
		// 최소한 헤더는 파싱할 수 있어야 한다
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		// 헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다
		if (dataSize < header.size)
			break;

		// 패킷 조립 성공
		OnRecvPacket(&buffer[processLen], header.size);

		processLen += header.size;
	}

	return processLen;
}
