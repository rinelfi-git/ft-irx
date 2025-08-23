#include <ASocketServerObserver.hpp>
#include <SocketServer.hpp>
#include <string>

ASocketServerObserver::ASocketServerObserver(SocketServer& observable): _observable(observable)
{}

ASocketServerObserver::~ASocketServerObserver()
{}

SocketServer&	ASocketServerObserver::observable(void) const
{
	return (_observable);
}