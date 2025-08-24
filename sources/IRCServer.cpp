#include "IRCServer.hpp"
#include "ASocketServerObserver.hpp"
#include "SocketServer.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "UserInfo.hpp"
#include "Pending.hpp"
#include <string>
#include <stdexcept>
#include <iostream>

IRCServer* IRCServer::_instance = NULL;

IRCServer::IRCServer(const std::string& password):
	ASocketServerObserver(),
	_password(password),
	_pendings(),
	_users(),
	_channels()
{
	if (IRCServer::_instance)
		throw std::logic_error("Violation of the singleton principle.");
	IRCServer::_instance = this;
}

IRCServer::~IRCServer()
{}

IRCServer&	IRCServer::getInstance(void)
{
	if (!IRCServer::_instance)
		throw std::logic_error("Instance error.");
	return (*IRCServer::_instance);
}

void	IRCServer::joinChannel(const User& user, const std::string& name)
{
	(void)user;
	(void)name;
}

void	IRCServer::auth(const UserInfo& user, const std::string& password)
{
	(void)user;
	(void)password;
}

User*	IRCServer::user(const std::string& nick) const
{
	std::map<std::string, User*>::const_iterator	userPtr(_users.find(nick));
	if (userPtr != _users.end())
		return (userPtr->second);
	return (NULL);
}

Channel*	IRCServer::channel(const std::string& name) const
{
	std::map<std::string, Channel*>::const_iterator	channelPtr(_channels.find(name));
	if (channelPtr != _channels.end())
		return (channelPtr->second);
	return (NULL);
}

void	IRCServer::onConnect(int fd)
{
	ASocketClient*	socket(new Pending(fd));
	_pendings[fd] = socket;
}

void	IRCServer::onData(int fd, const std::string& data)
{
	std::map<int, ASocketClient*>::iterator	pendingPtr(_pendings.find(fd));

	if (pendingPtr != _pendings.end())
	{
		Pending*	pending(dynamic_cast<Pending*>(pendingPtr->second));
		pending->input(data);
	}
}

void	IRCServer::onDisconnect(int fd)
{
	(void)fd;
	std::cout << "Disconnect event" << std::endl;
}