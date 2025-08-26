#include "Authenticated.hpp"
#include "ASocketClient.hpp"
#include "Pending.hpp"
#include "User.hpp"
#include "IRCServer.hpp"
#include "Channel.hpp"
#include <sstream>
#include <string>

Authenticated::Authenticated(int fd):
	ASocketClient(fd),
	_user(NULL)
{}

Authenticated::Authenticated(const Pending& pending):
	ASocketClient(pending),
	_user(NULL)
{}

Authenticated::~Authenticated()
{}

void	Authenticated::_parseMode(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parsePrivMsg(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parsePing(const std::string& arg)
{
	send("pong " + arg);
}

void	Authenticated::_parseJoin(const std::string& arg)
{
	Channel *channel = IRCServer::getInstance().channel(arg);
	if (!channel)
	{
		IRCServer::getInstance().createChannel(arg, _user);
	}
	else
	{
		channel->join(_user);
	}
}

void	Authenticated::_parseInvite(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parseTopic(const std::string& arg)
{
	std::stringstream	builder(arg);
	std::string			name;
	std::string			topic;
	char				ddot;

	builder >> name;
	builder >> ddot;
	std::getline(builder, topic);
	Channel*	channel(IRCServer::getInstance().channel(name));
	channel->setTopic(*_user, topic);
}

void	Authenticated::_iMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;
}

void	Authenticated::_tMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;


}

void	Authenticated::_kMode(bool enable, const std::string& name, const std::string& password)
{
	(void)enable;
	(void)name;
	(void)password;

}

void	Authenticated::_oMode(bool enable, const std::string& name, const std::string& user)
{
	(void)enable;
	(void)name;
	(void)user;

}

void	Authenticated::_lMode(bool enable, const std::string& name, const std::string& limit)
{
	(void)enable;
	(void)name;
	(void)limit;

}

void	Authenticated::parse(const std::map<std::string, std::string>& cmds)
{
	std::map<std::string, void (Authenticated::*)(const std::string&)>	actions;

	actions["mode"] = &Authenticated::_parseMode;
	actions["privmsg"] = &Authenticated::_parsePrivMsg;
	actions["ping"] = &Authenticated::_parsePing;
	actions["join"] = &Authenticated::_parseJoin;
	actions["invite"] = &Authenticated::_parseInvite;
	actions["topic"] = &Authenticated::_parseTopic;

	std::map<std::string, void (Authenticated::*)(const std::string&)>::iterator	actionPtr(actions.begin());
	while (actionPtr != actions.end())
	{
		std::map<std::string, std::string>::const_iterator	cmdPtr(cmds.find(actionPtr->first));
		if (cmdPtr != cmds.end())
			(this->*actions.at(cmdPtr->first))(cmdPtr->second);
		actionPtr++;
	}
}

User*	Authenticated::user(void) const
{
	return (_user);
}

void	Authenticated::user(User* set)
{
	_user = set;
}