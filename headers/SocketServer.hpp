
#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <vector>
#include "ASocketServerObserver.hpp"

class SocketServer
{
private:
	int _fd;
	bool _running;
	std::vector<struct pollfd> _polls;
public:
	SocketServer(int portIn);
	~SocketServer();
	ASocketServerObserver *attachObserver(ASocketServerObserver *observerIn);
	ASocketServerObserver *detachObserver(ASocketServerObserver *observerIn);
	ASocketServerObserver *start();
	ASocketServerObserver *stop();
};

#endif
