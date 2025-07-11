#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class ServerService;

/*--------------
	Listener
---------------*/

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* �ܺο��� ��� */
	bool StartAccept(ServerServiceRef serverService);
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(IocpEvent* acceptEvent);
	void ProcessAccept(IocpEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<IocpEvent*> _acceptEvents;
	ServerServiceRef _service;
};

class BoostListener : public IocpObject
{
public:
	BoostListener() = default;
	~BoostListener();

public:
	/* �ܺο��� ��� */
	bool StartAccept(BoostServiceRef serverService);
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() sealed;
	boost::asio::ip::tcp::acceptor& GetBoostHandle();
	virtual void Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept();

	// ����� ����
	void AcceptorInit();

protected:
	optional<boost::asio::ip::tcp::acceptor> _acceptor;
	BoostServiceRef _service;
};

