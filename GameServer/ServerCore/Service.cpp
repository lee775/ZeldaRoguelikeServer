#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

/*-------------
	Service
--------------*/

Service::Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddress(address), _iocpCore(core), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{

}

Service::~Service()
{
}

void Service::CloseService()
{
	// TODO
}

SessionRef Service::CreateSession()
{
	SessionRef session = _sessionFactory();
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(SessionRef session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);
}

void Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	assert(_sessions.erase(session) != 0);
	_sessionCount--;
}

/*-----------------
	ClientService
------------------*/

ClientService::ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		SessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

/*-----------------
	ServerService
------------------*/

ServerService::ServerService(NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	_listener = make_shared<Listener>();
	if (_listener == nullptr)
		return false;

	ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;

	return true;
}

void ServerService::CloseService()
{
	// TODO

	Service::CloseService();
}

BoostService::BoostService(NetAddress address, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, nullptr, factory, maxSessionCount)
{
}

bool BoostService::BoostInit()
{
	if (CanStart() == false)
		return false;

	_listener = make_shared<BoostListener>();
	if (_listener == nullptr)
		return false;

	BoostServiceRef service = static_pointer_cast<BoostService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;
}

bool BoostService::Start()
{
	io.run();

	return true;
}

void BoostService::CloseService()
{
}

SessionRef BoostService::CreateSession()
{
	SessionRef session = _sessionFactory();
	session->SetService(shared_from_this());

	session->InitBoostSocket(io);

	return session;
}

BoostClientService::BoostClientService(NetAddress targetAddress, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, nullptr, factory, maxSessionCount)
{
}

SessionRef BoostClientService::CreateSession()
{
	SessionRef session = _sessionFactory();
	session->SetService(shared_from_this());

	session->InitBoostSocket(io);

	return session;
}

bool BoostClientService::BoostInit()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		SessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

bool BoostClientService::Start()
{
	io.run();

	return true;
}
