#include "ASocketServerObserver.hpp"
#include "SocketServer.hpp"
#include <string>

ASocketServerObserver::ASocketServerObserver(): _observable()
{}

ASocketServerObserver::~ASocketServerObserver()
{}

void	ASocketServerObserver::bind(SocketServer* observable)
{
	_observable = observable;
}

SocketServer&	ASocketServerObserver::observable(void) const
{
	return (*_observable);
}