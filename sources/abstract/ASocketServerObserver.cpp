#include <ASocketObserver.hpp>
#include <SocketServer.hpp>
#include <string>

ASocketObserver::ASocketObserver(SocketServer& observable): _observable(observable)
{}

ASocketObserver::~ASocketObserver()
{}

SocketServer	ASocketObserver::observable(void) const
{
	return (_observable);
}