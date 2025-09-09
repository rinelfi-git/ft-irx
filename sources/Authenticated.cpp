#include "Authenticated.hpp"
#include "ASocketClient.hpp"
#include "Pending.hpp"
#include "User.hpp"
#include "utils.hpp"
#include "Message.hpp"
#include "IRCServer.hpp"
#include "Channel.hpp"
#include <sstream>
#include <string>
#include <stdexcept>

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
	withoutParameter["+i"] = &Authenticated::_iMode;
	withoutParameter["-i"] = &Authenticated::_iMode; 

	withParameter["-k"] = &Authenticated::_kMode;
	withParameter["+k"] = &Authenticated::_kMode;
	withParameter["-o"] = &Authenticated::_oMode;
	withParameter["+o"] = &Authenticated::_oMode;
	//withParameter["-i"] = &Authenticated::_iMode;
	//withParameter["+i"] = &Authenticated::_iMode;
	withParameter["+l"] = &Authenticated::_lMode;

	Channel*	channel(IRCServer::getInstance().channel(name));
	if (!channel)
		return;
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

/* void Authenticated::_parsePrivMsg(const std::string& arg)
{
    std::stringstream ss(arg);
    std::string send_to, content;
    
    ss >> send_to;
    getline(ss, content);
    if (!content.empty() && content[0] == ' ')
        content = content.substr(1);
    if (!content.empty() && content[0] == ':')
        content = content.substr(1);
    if (send_to.empty())
    {
        send("411 " + _user->info().nick() + " :No recipient given (PRIVMSG)");
        return;
    }
    if (content.empty())
    {
        send("412 " + _user->info().nick() + " :No text to send");
        return;
    }
    if (!send_to.empty() && send_to[0] == '#')
    {
        Channel *channel = IRCServer::getInstance().channel(send_to);
        if (!channel)
        {
            send("403 " + _user->info().nick() + " " + send_to + " :No such channel");
            return;
        }
        if (!channel->isMember(*_user))
        {
            send("404 " + _user->info().nick() + " " + send_to + " :Cannot send to channel");
            return;
        }
        channel->broadcast(":" + _user->networkId() + " PRIVMSG " + send_to + " :" + content);
    }
    else
    {
        User* target = IRCServer::getInstance().user(send_to);
        if (!target)
        {
            send("401 " + _user->info().nick() + " " + send_to + " :No such nick/channel");
            return;
        }
        target->socket().send(":" + _user->networkId() + " PRIVMSG " + send_to + " :" + content);
    }
}
 */

 void Authenticated::_parsePrivMsg(const std::string& arg)
{
    std::stringstream ss(arg);
    std::string send_to, content;
    
    ss >> send_to;
    getline(ss, content);
    if (!content.empty() && content[0] == ' ')
        content = content.substr(1);
    if (!content.empty() && content[0] == ':')
        content = content.substr(1);
    
    if (send_to.empty())
    {
        send("411 " + _user->info().nick() + " :No recipient given (PRIVMSG)");
        return;
    }
    if (content.empty())
    {
        send("412 " + _user->info().nick() + " :No text to send");
        return;
    }
    
    if (!send_to.empty() && send_to[0] == '#')
    {
        Channel *channel = IRCServer::getInstance().channel(send_to);
        if (!channel)
        {
            send("403 " + _user->info().nick() + " " + send_to + " :No such channel");
            return;
        }
        if (!channel->isMember(*_user))
        {
            send("404 " + _user->info().nick() + " " + send_to + " :Cannot send to channel");
            return;
        }
        channel->broadcast(":" + _user->networkId() + " PRIVMSG " + send_to + " :" + content);
    }
    else
    {
        User* target = IRCServer::getInstance().user(strToLower(send_to));
        if (!target)
        {
            send("401 " + _user->info().nick() + " " + send_to + " :No such nick/channel");
            return;
        }
        target->socket().send(":" + _user->networkId() + " PRIVMSG " + send_to + " :" + content);
    }
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

void Authenticated::_iMode(const std::string& name, char action)
{
    Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
	{
        send("403 " + _user->info().nick() + " " + name + " :No such channel");
        return;
    }
    if (!channel->isOperator(*_user))
	{
        send("482 " + _user->info().nick() + " " + name + " :You're not channel operator");
        return;
    }
    
    if (action == '+')
	{
        channel->mode().inviteOnly(true);
        channel->broadcast(":" + _user->networkId() + " MODE " + name + " +i");
    } else if (action == '-')
	{
        channel->mode().inviteOnly(false);
        channel->broadcast(":" + _user->networkId() + " MODE " + name + " -i");
    }
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
	 Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
	{
        send("403 " + _user->info().nick() + " " + name + " :No such channel");
        return;
    }
    if (!channel->isOperator(*_user))
	{
        send("482 " + _user->info().nick() + " " + name + " :You're not channel operator");
        return;
    }

	if (action == '+')
	{
		channel->addOperator(*_user, user);
	}
	else
		channel->removeOperator(*_user, user);
}

void	Authenticated::_lMode(const std::string& name, char action, const std::string& limit)
{
	 Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
	{
        send("403 " + _user->info().nick() + " " + name + " :No such channel");
        return;
    }
    if (!channel->isOperator(*_user))
	{
        send("482 " + _user->info().nick() + " " + name + " :You're not channel operator");
        return;
    }

	if (action == '+')
	{
		if (limit.empty())
        {
            send("461 " + _user->info().nick() + " MODE :Not enough parameters");
            return;
        }
		int limitValue = 0;
        std::stringstream ss(limit);
        if (!(ss >> limitValue) || limitValue <= 0)
        {
            send("696 " + _user->info().nick() + " " + name + " l :Invalid limit");
            return;
        }
		channel->mode().memberLimit(limitValue);
		channel->broadcast(":" + _user->networkId() + " MODE " + name + " +l " + limit);
	}
	
}

void	Authenticated::_lMode(const std::string& name, char action)
{
	 Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
	{
        send("403 " + _user->info().nick() + " " + name + " :No such channel");
        return;
    }
    if (!channel->isOperator(*_user))
	{
        send("482 " + _user->info().nick() + " " + name + " :You're not channel operator");
        return;
    }
	if (action == '-')
	{
		channel->mode().memberLimit(0);
		channel->broadcast(":" + _user->networkId() + " MODE " + name + " -l");
	}
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
