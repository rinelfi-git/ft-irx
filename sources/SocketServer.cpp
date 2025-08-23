#include "SocketServer.hpp"
#include "ASocketServerObserver.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <poll.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <iostream>

SocketServer::SocketServer(int port):
	_fd(socket(AF_INET, SOCK_STREAM, 0)),
	_running(false),
	_polls(),
	_observers()
{
	struct sockaddr_in address;
	int sockopt(1);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (_fd == -1
			|| setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1
			|| fcntl(_fd, F_SETFL, O_NONBLOCK) == -1
			|| bind(_fd, (const struct sockaddr *)&address, sizeof(address)) == -1
			|| listen(_fd, 42) == -1)
	{
		std::stringstream builder;
		builder << "Socket initiation error : " << std::strerror(errno) << '.';
		std::string error;
		std::getline(builder, error);
		throw std::runtime_error(error);
	}
	struct pollfd serverPoll;
	serverPoll.fd = _fd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	_polls.push_back(serverPoll);
}

SocketServer::~SocketServer()
{
	stop();
}

void	SocketServer::attachObserver(ASocketServerObserver* observer)
{
	std::vector<ASocketServerObserver*>::iterator	foundPtr(std::find(_observers.begin(), _observers.end(), observer));
	if (foundPtr != _observers.end())
		return ;
	observer->bind(this);
	_observers.push_back(observer);
}

void	SocketServer::detachObserver(ASocketServerObserver* observer)
{
	std::vector<ASocketServerObserver*>::iterator	foundPtr(std::find(_observers.begin(), _observers.end(), observer));
	if (foundPtr != _observers.end())
		_observers.erase(foundPtr);
}

void	SocketServer::start(void)
{
	_running = true;
	while (_running)
	{
		const int	in(poll(&_polls[0], _polls.size(), -1));
		if (in == -1 && errno != EINTR)
			throw std::runtime_error("Polling error : " + std::string(std::strerror(errno)) + ".");
		for (size_t i(0); i < _polls.size(); i++)
		{
			const struct pollfd currentPoll = _polls[i];
			if (currentPoll.fd == _fd && (currentPoll.revents & POLLIN))
				_acceptClient();
			else if (currentPoll.revents & POLLIN)
			{
				const bool	handled(_handleClient(i));
				if (!handled)
					i--;
			}
		}
	}
}

void	SocketServer::stop(void)
{
	if (!_running)
		return ;
	_running = false;
	std::vector<ASocketServerObserver*>::iterator	observersPtr(_observers.begin());
	std::vector<struct pollfd>::iterator	pollsPtr(_polls.begin());
	while (observersPtr != _observers.end())
	{
		ASocketServerObserver* observer(*observersPtr++);
		observer->onDisconnect(_fd);
	}
	while (pollsPtr != _polls.end())
		::close((pollsPtr++)->fd);
}

void	SocketServer::_acceptClient(void)
{
	const int	fd(accept(_fd, NULL, NULL));
	if (fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
		throw (std::runtime_error("Socket listen error : " + std::string(std::strerror(errno)) + "."));
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Client non-blocking mode error : " + std::string(std::strerror(errno)) + ".");
	struct pollfd	client;
	client.fd = fd;
	client.events = POLLIN;
	client.revents = 0;
	for (size_t j(0); j < _observers.size(); j++)
		_observers[j]->onConnect(fd);
	std::cout << "NEW CONNECTION << " << fd << std::endl;
	_polls.push_back(client);
}

bool	SocketServer::_handleClient(int index)
{
	char		buffer[SOCKET_READ_BUFFER];
	const int	fd(_polls[index].fd);
	const int	read(recv(fd, buffer, SOCKET_READ_BUFFER, 0));	

	if (read > 0)
	{
		buffer[read] = 0;
		for(size_t j(0); j < _observers.size(); j++)
			_observers[j]->onData(fd, std::string(buffer));
	}
	else if (read == 0)
	{
		for(size_t j(0); j < _observers.size(); j++)
			_observers[j]->onDisconnect(fd);
		::close(fd);
		_polls.erase(_polls.begin() + index);
		std::cout << "DISCONNECTION >> " << fd << std::endl;
	}
	else
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (true);
		for(size_t j(0); j < _observers.size(); j++)
			_observers[j]->onDisconnect(fd);
		::close(fd);
		_polls.erase(_polls.begin() + index);
		std::cout << "DISCONNECTION >> " << fd << std::endl;
	}
	return (read);
}