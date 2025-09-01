#include "Channel.hpp"
#include "Mode.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Authenticated.hpp"
#include <string>
#include <map>
#include <sstream>

Channel::Channel(const std::string& name, User* first):
	_name(name),
	_password(),
	_topic(),
	_mode(),
	_members(),
	_operators(),
	_inviteds()
{
	_members[first->id()] = first;
	_operators[first->id()] = first;
}

void	Channel::message(const Message& msg) const
{
	(void)msg;
}

void	Channel::invite(const User& host, const User& guest)
{
	(void)host;
	(void)guest;
}

void	Channel::setTopic(const User& setter, const std::string& set)
{
	if (_mode.isTopicRestricted() && !isOperator(setter))
	{
		setter.socket().send("482 " + setter.info().nick() + " " + _name + " :You're not channel operator");
		return ;
	}
	_topic = set;
	broadcast("332 " + setter.info().nick() + " " + _name + " :" + _topic);
}

void	Channel::getTopic(const User& requester) const
{
	requester.socket().send("332 " + requester.info().nick() + " " + _name + " :" + _topic);
}

bool	Channel::isMember(const User& user) const
{
	return (_members.find(user.id()) != _members.end());
}

bool	Channel::isMember(const std::string& id) const
{
	return (_members.find(id) != _members.end());
}

bool	Channel::isOperator(const User& user) const
{
	
	return (_operators.find(user.id()) != _operators.end());
}

bool	Channel::isOperator(const std::string& id) const
{
	return (_operators.find(id) != _operators.end());
}

bool	Channel::isInvited(const User& user) const
{
	return (_inviteds.find(user.id()) != _inviteds.end());
}

bool	Channel::isInvited(const std::string& id) const
{
	return (_inviteds.find(id) != _inviteds.end());
}

const std::string&	Channel::name(void) const
{
	return (_name);
}

Mode&	Channel::mode(void)
{
	return (_mode);
}

void	Channel::join(User* user)
{
	if (isMember(*user))
		return;
	std::string	id(user->id());
	_members[id] = user;
	broadcast(":" + user->networkId() + " JOIN " + _name);
	user->socket().send("353 " + id + " = " + _name + " :" + getUsers());
	user->socket().send("366 " + id + " " + _name + " :End of /NAMES list.");
	if (!_topic.empty())
		user->socket().send("332 " + id + " " + _name + " :" + _topic);
	else
		user->socket().send("331 " + id + " " + _name + " :No topic is set");
}

std::string Channel::getUsers() const
{
	std::map<std::string, User*>::const_iterator	itOperator(_operators.begin());
	std::map<std::string, User*>::const_iterator	itMember(_members.begin());
	std::string out;

	while (itOperator != _operators.end())
		out += " @" + (itOperator++)->second->info().nick();

	while (itMember != _members.end())
	{
		User	member(*(itMember++)->second);
		if (!isOperator(member))
			out += " " + member.info().nick();
	}
	return (out.substr(1));
}

std::string	Channel::modeResume(void) const
{
	std::string	modes;
	std::string	args;
	std::map<char, bool>		modeFlags;
	std::map<char, std::string>	modeArgs;

	std::stringstream	builder;
	std::string			limit;
	builder << _mode.memberLimit();
	builder >> limit;

	modeFlags['i'] = _mode.isInviteOnly();
	modeFlags['t'] = _mode.isTopicRestricted();
	modeFlags['l'] = _mode.memberLimit() > 0;
	modeFlags['k'] = !_password.empty();
	modeArgs['k'] = _password;
	modeArgs['l'] = limit;
	std::map<char, bool>::const_iterator	itModeFlag(modeFlags.begin());
	while (itModeFlag != modeFlags.end())
	{
		if (itModeFlag->second)
			modes += itModeFlag->first;
		if (modeArgs.find(itModeFlag->first) != modeArgs.end() && itModeFlag->second)
			args += " " + modeArgs.at(itModeFlag->first);
		++itModeFlag;
	}
	if (!modes.empty())
		return ("+" + modes + args);
	return ("");
}

std::string	Channel::modeResume(const std::string& modes) const
{
	std::string	modeOutputs;
	std::string	args;
	std::map<char, bool>		modeFlags;
	std::map<char, std::string>	modeArgs;

	std::stringstream	builder;
	std::string			limit;
	builder << _mode.memberLimit();
	builder >> limit;

	modeFlags['i'] = _mode.isInviteOnly();
	modeFlags['t'] = _mode.isTopicRestricted();
	modeFlags['l'] = _mode.memberLimit() > 0;
	modeFlags['k'] = !_password.empty();
	modeArgs['k'] = _password;
	modeArgs['l'] = limit;
	std::string::const_iterator				itMode(modes.begin());
	while (itMode != modes.end())
	{
		if (modeFlags.find(*itMode) != modeFlags.end() && modeFlags.at(*itMode))
			modeOutputs += *itMode;
		if (modeArgs.find(*itMode) != modeArgs.end() && modeFlags.at(*itMode))
			args += " " + modeArgs.at(*itMode);
		++itMode;
	}
	if (!modeOutputs.empty())
		return ("+" + modeOutputs + args);
	return ("");
}
bool	Channel::auth(User* user, const std::string& password)
{
	if (isInvited(*user))
		return (true);
	if (_mode.isInviteOnly() && !isInvited(*user))
	{
		user->socket().send("473 " + user->info().nick() + " " + _name + " :Cannot join channel (+i)");
		return (false);
	}
	if (!_password.empty() && _password != password)
	{
		user->socket().send("475 " + user->info().nick() + " " + _name + " :Cannot join channel (+k)");
		return (false);
	}
	if (_mode.memberLimit() > 0 && _members.size() >= _mode.memberLimit())
	{
		user->socket().send("471 " + user->info().nick() + " " + _name + " :Cannot join channel (+l)");
		return (false);
	}
	return (true);
}

void	Channel::setPassword(const User& setter, const std::string& password)
{
	if (!isOperator(setter))
	{
		setter.socket().send("482 " + setter.info().nick() + " " + _name + " :You're not channel operator");
		return;
	}
	_password = password;
	if (!password.empty())
		broadcast(":" + setter.networkId() + " MODE " + _name + " +k " + password);
	else
		broadcast(":" + setter.networkId() + " MODE " + _name + " -k");
}

void	Channel::setTopicMode(const User& setter, bool operatorOnly)
{
	if (!isOperator(setter))
	{
		setter.socket().send("482 " + setter.info().nick() + " " + _name + " :You're not channel operator");
		return ;
	}
	_mode.topicRestricted(operatorOnly);
	broadcast(":" + setter.networkId() + " MODE " + _name + (operatorOnly ? " +t" : " -t"));
}

void	Channel::quit(const User& user, const std::string& msg)
{
	std::string	id(user.id());
	_members.erase(id);
	_operators.erase(id);
	_inviteds.erase(id);
	broadcast(":" + user.networkId() + " QUIT " + msg);
}

bool	Channel::isChannelName(const std::string& str)
{
	if (str.empty() || str.length() > 200)
		return (false);
	if (str.at(0) != '#')
		return (false);
	for (std::string::const_iterator	itStr(str.begin() + 1); itStr != str.end(); itStr++)
	{
		const char	c(*itStr);
		if (!std::isalnum(c) && c != '#' && c != '-' && c != '_')
			return (false);
	}
	return (true);
}

void	Channel::broadcast(const std::string& msg) const
{
	std::map<std::string, User*>::const_iterator	itMember(_members.begin());

	while (itMember != _members.end())
		(itMember++)->second->socket().send(msg);
}

void	Channel::kick(const User& op, const User& member , const std::string& message)
{
	if (!isOperator(op))
	{
		op.socket().send("482 " + op.info().nick() + " " + _name + " :You're not channel operator");
		return ;
	}
	if (!isMember(member))
	{
		op.socket().send("441 " + op.info().nick() + " " + _name +" " + member.info().nick() + " :They aren't on that channel");
		return ;
	}
	broadcast(":" + op.networkId() + " KICK " + _name +" " + member.info().nick() + " " + message);
	_members.erase(member.id());
}
