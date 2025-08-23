#include <ASocketObserver.hpp>
#include <SocketServer.hpp>
#include <string>

ASocketObserver::ASocketObserver(SocketServer& observable): _observable(observable)
{}

ASocketObserver::~ASocketObserver()
{}