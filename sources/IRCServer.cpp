#include "IRCServer.hpp"
#include "ASocketServerObserver.hpp"
#include "SocketServer.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "UserInfo.hpp"
#include "Pending.hpp"
#include "Authenticated.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <csignal>

IRCServer* IRCServer::_instance = NULL;

IRCServer::IRCServer(const std::string& password):
	ASocketServerObserver(),
	_password(password),
	_socketClients(),
	_users(),
	_channels()
{
	if (IRCServer::_instance)
		throw std::logic_error("Violation of the singleton principle.");
	IRCServer::_instance = this;
	signal(SIGINT, IRCServer::_sigint);
}

IRCServer::~IRCServer()
{
	std::map<int, ASocketClient*>::iterator	socketClientPtr(_socketClients.begin());
	std::map<std::string, User*>::iterator	userPtr(_users.begin());
	std::map<std::string, Channel*>::iterator	channelPtr(_channels.begin());
	while (socketClientPtr != _socketClients.end())
		delete (socketClientPtr++)->second;
	while (userPtr != _users.end())
		delete (userPtr++)->second;
	while (channelPtr != _channels.end())
		delete (channelPtr++)->second;
}

void	IRCServer::_sigint(int num)
{
	_instance->observable().stop();
	std::cout << std::endl << "Stopping server..." << std::endl;
	signal(num, SIG_DFL);
}

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

bool	IRCServer::auth(const Pending& pending)
{
	if (user(pending.userInfo().nick()) != NULL)
	{
		pending.send("433 * " + pending.userInfo().nick() + " :Nickname is already in use");
		return (false);
	}
	if (_password.empty() || pending.password() == _password)
	{
		User	created(_createUser(pending.userInfo(), pending));
		created.socket().send("001 " + created.info().nick() + " :Welcome to the Internet Relay Network " + created.networkId());
		return true;
	}
	pending.send("464 * :Password incorrect");
	return false;
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
	_socketClients[fd] = socket;
}

void	IRCServer::onData(int fd, const std::string& data)
{
	std::map<int, ASocketClient*>::iterator	socketClientPtr(_socketClients.find(fd));

	if (dynamic_cast<Pending*>(socketClientPtr->second))
	{
		Pending*	pending(dynamic_cast<Pending*>(socketClientPtr->second));
		pending->input(data);
		pending->auth();
	}
	else if (dynamic_cast<Authenticated*>(socketClientPtr->second))
	{
		Authenticated*	authenticated(dynamic_cast<Authenticated*>(socketClientPtr->second));
		authenticated->input(data);
	}
}

void	IRCServer::onDisconnect(int fd)
{
	ASocketClient*	socket(_socketClients.at(fd));
	Authenticated*	auth(dynamic_cast<Authenticated*> (socket));
	if (auth)
	{
		// disconnect user from all channels then
		User*	user(auth->user());
		std::string	nick(user->info().nick());
		delete user;
		_users.erase(nick);
	}
	delete	socket;
	_socketClients.erase(fd);
}

User&	IRCServer::_createUser(const UserInfo& info, const Pending& pending)
{
	ASocketClient*	socket(new Authenticated(pending));
	User*			user(new User(info, dynamic_cast<Authenticated*>(socket)));
	const int		fd(pending.fd());
	_users[info.nick()] = user;
	delete _socketClients.at(fd);
	_socketClients[fd] = socket;
	return *user;
}

void	IRCServer::createChannel(const std::string& name, User* first)
 {
	Channel *channel = new Channel(name, first);
	_channels[name] = channel;
	first->socket().send(":" + first->networkId() + " JOIN " + name);
	first->socket().send("353 " + first->info().nick() + " = " + name + " :" + channel->getUsers());
	first->socket().send("366 " + first->info().nick() + " " + name + " :End of /NAMES list.");
 }
