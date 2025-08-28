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
	std::stringstream	builder(arg);
	std::string			name;
	std::string			modes;
	std::string			args;
	builder >> name;
	builder >> modes;
	std::map<std::string, void (Authenticated::*)(const std::string&, char, const std::string&)>	withParameter;
	std::map<std::string, void (Authenticated::*)(const std::string&, char)>	withoutParameter;
	withoutParameter["-l"] = &Authenticated::_lMode;
	withoutParameter["-t"] = &Authenticated::_tMode;
	withoutParameter["+t"] = &Authenticated::_tMode;
	withParameter["-k"] = &Authenticated::_kMode;
	withParameter["+k"] = &Authenticated::_kMode;
	withParameter["-o"] = &Authenticated::_oMode;
	withParameter["+o"] = &Authenticated::_oMode;
	withParameter["-i"] = &Authenticated::_iMode;
	withParameter["+i"] = &Authenticated::_iMode;
	withParameter["+l"] = &Authenticated::_lMode;

	Channel*	channel(IRCServer::getInstance().channel(name));
	if (Channel::isChannelName(name) && !channel)
		return	send("403 " + _user->info().nick() + " " + name + " :No such channel");
	if (modes.empty())
		send("324 " + _user->info().nick() + " " + name + " :" + channel->modeResume());
	else if (modes.at(0) != '+' && modes.at(0) != '-')
		send("324 " + _user->info().nick() + " " + name + " :" + channel->modeResume(modes));
	else
	{
		char	action = modes.at(0);
		bool	hasNextParams(!builder.eof());
		builder >> args;
		std::string::const_iterator	itMode(modes.begin());
		while (itMode != modes.end())
		{
			char	c(*itMode++);
			if (c == '+' || c == '-')
			{
				action = c;
				continue ;
			}
			std::string	mode(1, action);
			mode += std::string(1, c);
			if (withoutParameter.find(mode) != withoutParameter.end())
				(this->*withoutParameter.at(mode))(name, action);
			else if (withParameter.find(mode) != withParameter.end())
			{
				if (hasNextParams)
				{
					(this->*withParameter.at(mode))(name, action, args);
					hasNextParams = !builder.eof();
					builder >> args;
				}
				else
					send("461 " + _user->info().nick() + " MODE :Not enough parameters");
			}
		}
	}
}

void	Authenticated::_parsePrivMsg(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parsePing(const std::string& arg)
{
	send("PONG " + arg);
}

void	Authenticated::_parseJoin(const std::string& arg)
{
	std::stringstream	builder(arg);
	std::string			name;
	std::string			password;

	builder >> name;
	builder >> password;
	Channel *channel = IRCServer::getInstance().channel(name);
	if (!channel)
		IRCServer::getInstance().createChannel(name, _user);
	else if (channel->auth(_user, password))
		channel->join(_user);
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
	Channel*			channel;

	builder >> name;
	std::getline(builder, topic);
	channel = IRCServer::getInstance().channel(name);
	channel->setTopic(*_user, topic.substr(topic.find(':') + 1));
}

void	Authenticated::_parseKick(const std::string& arg)
{
	std::stringstream ss(arg);
	std::string channelName;
	std::string nickUser;
	std::string message;
	Channel*	channel;
	User*		member;

	ss >> channelName;
	ss >> nickUser;
	std::getline(ss, message);
	channel = IRCServer::getInstance().channel(channelName);
	member = IRCServer::getInstance().user(nickUser);
	if (!member)
		return send("401 " + _user->info().nick() + " " + nickUser + " :No such nick");
	channel->kick(*_user, *member, message.substr(message.find(':')));
}

void	Authenticated::_parseQuit(const std::string& arg)
{
	IRCServer::getInstance().quit(*_user, arg);
}

void	Authenticated::_iMode(const std::string& name, char action, const std::string& user)
{
	if (action == '+')
		std::cout << "Invite " << user << " in " << name << std::endl;
	else
		std::cout << "Remove invite " << user << " in " << name << std::endl;
}

void	Authenticated::_tMode(const std::string& name, char action)
{
	Channel*	channel(IRCServer::getInstance().channel(name));

	if (!channel)
		return	send("403 " + _user->info().nick() + " " + name + " :No such channel");
	channel->setTopicMode(*_user, action == '+');
}

void	Authenticated::_kMode(const std::string& name, char action, const std::string& password)
{
	Channel*	channel(IRCServer::getInstance().channel(name));

	if (!channel)
		return send("403 " + _user->info().nick() + " " + name + " :No such channel");
	if (action == '+')
		channel->setPassword(*_user, password);
	else
		channel->setPassword(*_user, "");
}

void	Authenticated::_oMode(const std::string& name, char action, const std::string& user)
{
	if (action == '+')
		std::cout << "invite operator " << user << " in " << name << std::endl;
	else
		std::cout << "remove operator " << user << " from " << name << std::endl;
}

void	Authenticated::_lMode(const std::string& name, char action, const std::string& limit)
{
	(void)action;
	std::cout << "set limit in " << name << " to " << limit << std::endl;
}

void	Authenticated::_lMode(const std::string& name, char action)
{
	(void)action;
	std::cout << "delete limit in " << name << std::endl;
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
	actions["kick"] = &Authenticated::_parseKick;
	actions["quit"] = &Authenticated::_parseQuit;

	std::map<std::string, void (Authenticated::*)(const std::string&)>::iterator	actionPtr(actions.begin());
	while (actionPtr != actions.end())
	{
		std::map<std::string, std::string>::const_iterator	cmdPtr(cmds.find((actionPtr++)->first));
		if (cmdPtr != cmds.end())
			(this->*actions.at(cmdPtr->first))(cmdPtr->second);
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
