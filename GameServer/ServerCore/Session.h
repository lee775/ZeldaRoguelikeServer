#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;

/*--------------
	Session
---------------*/

class Session : public IocpObject
{
	friend class Listener;
	friend class BoostListener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session(ServerLibMode mode);
	virtual ~Session();

public:
	/* 외부에서 사용 */
	void				Send(SendBufferRef sendBuffer);
	bool				Connect();
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 */
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	// Boost 함수들
	void				BoostProcessConnect();
	void				BoostRegisterRecv();
	void				BoostRegisterSend();
	void				BoostRegisterDisconnect();

	// Cliend만 사용
	bool				BoostRegisterConnect();

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 재정의 */
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

public:

	// Circular Buffer [             ]
	//char _sendBuffer[1000];
	//int32 _sendLen = 0;
	void InitBoostSocket(boost::asio::io_context& io);
	boost::asio::ip::tcp::socket& GetBoostSocket();

private:
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	std::optional<boost::asio::ip::tcp::socket> _boostSocket;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;
	ServerLibMode		_serverMode;

private:
	USE_LOCK;

	/* 수신 관련 */
	RecvBuffer			_recvBuffer;

	/* 송신 관련 */
	queue<SendBufferRef> _sendQueue;
	atomic<bool>		_sendResitered = false;

private:
	/* IocpEvent 재사용 */
	IocpEvent		_connectEvent;
	IocpEvent		_disconnectEvent;
	IocpEvent		_recvEvent;
	IocpEvent		_sendEvent;
};


/*-----------------
	PacketSession
------------------*/

struct PacketHeader
{
	uint16 size;
	uint16 id; // 프로토콜ID (ex. 1=로그인, 2=이동요청)
};

class PacketSession : public Session
{
public:
	PacketSession(ServerLibMode mode);
	virtual ~PacketSession();

	PacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed;
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) abstract;
};