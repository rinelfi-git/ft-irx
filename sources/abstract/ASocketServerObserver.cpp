#include "ASocketServerObserver.hpp"
#include "SocketServer.hpp"
#include <string>
#include <stdexcept>

ASocketServerObserver::ASocketServerObserver(): _observable()
{}

ASocketServerObserver::~ASocketServerObserver()
{}

void	ASocketServerObserver::bind(SocketServer& observable)
{
	_observable = &observable;
	observable.attachObserver(this);
}

void	ASocketServerObserver::unbind(SocketServer& observable)
{
	observable.detachObserver(this);
	_observable = NULL;
}

SocketServer&	ASocketServerObserver::observable(void) const
{
	if (!_observable)
		throw (std::logic_error("Socket server instance cannot be NULL."));
	return (*_observable);
}