#include "Authenticated.hpp"
#include "ASocketClient.hpp"
#include "Pending.hpp"
#include "User.hpp"
#include "utils.hpp"
#include "Message.hpp"
#include "IRCServer.hpp"
#include "Channel.hpp"
#include "Response.hpp"
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
	withParameter["+l"] = &Authenticated::_lMode;

	Channel*	channel(IRCServer::getInstance().channel(name));
	if (User::isNickName(name))
		return ;
	if (!channel)
		return;
	if (Channel::isChannelName(name) && !channel)
		return	Response(*this).errNoSuchChannel(_user->info().nick(), name);
	if (modes.empty())
		return Response(*this).rplChannelModeIs(_user->info().nick(), *channel);
	else if (modes.at(0) != '+' && modes.at(0) != '-')
		return Response(*this).rplChannelModeIs(_user->info().nick(), *channel, modes);
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
					return Response(*this).errNeedMoreParams(_user->info().nick(), "MODE");
			}
		}
	}
}

void Authenticated::_parsePrivMsg(const std::string& arg)
{
    std::stringstream ss(arg);
    std::string send_to, content;
    
    ss >> send_to;
    getline(ss, content);
    content = content.substr(1);
    
    if (send_to.empty())
		return Response(*this).errEmptyRecipient(_user->info().nick());
    if (content.empty())
		return Response(*this).errEmptyContent(_user->info().nick());
    
    if (Channel::isChannelName(send_to))
    {
        Channel *channel = IRCServer::getInstance().channel(send_to);
        if (!channel)
			return Response(*this).errNoSuchChannel(_user->info().nick(), send_to);
        if (!channel->isMember(*_user))
			return Response(*this).errCannotSendToChannel(_user->info().nick(), send_to);
        channel->broadcast(":" + _user->networkId() + " PRIVMSG " + send_to + " :" + content);
    }
    else
    {
        User* target = IRCServer::getInstance().user(strToLower(send_to));
        if (!target)
			return Response(*this).errNoSuchNick(_user->info().nick(), send_to);
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

void    Authenticated::_parsePart(const std::string& arg)
{
    std::istringstream iss(arg);
    std::string channels;
    std::string reason;
    
    if (!std::getline(iss, channels, ' ') || channels.empty())
		return Response(*this).errNeedMoreParams(_user->info().nick(), "PART");
    std::string temp;
    if (std::getline(iss, temp))
    {
        if (!temp.empty() && temp[0] == ':')
            reason = " " + temp;
        else
            reason = " :" + temp;
    }
    if (reason.empty())
        reason = " : good bye!";
    std::istringstream channelStream(channels);
    std::string channelName;
    
    while (std::getline(channelStream, channelName, ','))
    {
        size_t start = channelName.find_first_not_of(" \t");
        size_t end = channelName.find_last_not_of(" \t");
        
        if (start == std::string::npos)
            continue;
        channelName = channelName.substr(start, end - start + 1);
        if (!Channel::isChannelName(channelName))
        {
			Response(*this).errNoSuchChannel(_user->info().nick(), channelName);
            continue;
        }
        Channel* channel = IRCServer::getInstance().channel(channelName);
        if (!channel)
        {
            Response(*this).errNoSuchChannel(_user->info().nick(), channelName);
            continue;
        }
        if (!channel->isMember(*_user))
        {
            Response(*this).errNotOnThatChannel(_user->info().nick(), channelName);
            continue;
        }
        std::string msg = channelName + reason;
        channel->part(*_user, msg);
    }
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
		return Response(*this).errNoSuchNick(_user->info().nick(), nickUser);
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
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
    if (!channel->isOperator(*_user))
		return Response(*this).errNotOperator(_user->info().nick(), name);
	channel->setInviteOnly(_user->networkId(), action == '+');
}


void	Authenticated::_tMode(const std::string& name, char action)
{
	Channel*	channel(IRCServer::getInstance().channel(name));

	if (!channel)
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
	channel->setTopicMode(*_user, action == '+');
}

void	Authenticated::_kMode(const std::string& name, char action, const std::string& password)
{
	Channel*	channel(IRCServer::getInstance().channel(name));

	if (!channel)
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
	if (action == '+')
		channel->setPassword(*_user, password);
	else
		channel->setPassword(*_user, "");
}

void	Authenticated::_oMode(const std::string& name, char action, const std::string& user)
{
	 Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
    if (!channel->isOperator(*_user))
		return Response(*this).errNotOperator(_user->info().nick(), name);

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
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
    if (!channel->isOperator(*_user))
		return Response(*this).errNotOperator(_user->info().nick(), name);

	if (action == '+')
	{
		if (limit.empty())
			return Response(*this).errNeedMoreParams(_user->info().nick(), "MODE");
		int limitValue = 0;
        std::stringstream ss(limit);
        if (!(ss >> limitValue) || limitValue <= 0)
			return Response(*this).errInvalidLimit(_user->info().nick(), "MODE");
		channel->mode().memberLimit(limitValue);
		channel->broadcast(":" + _user->networkId() + " MODE " + name + " +l " + limit);
	}
}

void	Authenticated::_lMode(const std::string& name, char action)
{
	 Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
		return Response(*this).errNoSuchChannel(_user->info().nick(), name);
    if (!channel->isOperator(*_user))
		return Response(*this).errNotOperator(_user->info().nick(), name);
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
	actions["part"] = &Authenticated::_parsePart;

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
