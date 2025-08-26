#include <string>
#include <map>
#include "Channel.hpp"
#include "Mode.hpp"
#include "User.hpp"
#include "Message.hpp"

Channel::Channel(const std::string& name, User* first):
	_name(name),
	_password(),
	_topic(),
	_mode(),
	_members(),
	_operators(),
	_inviteds()
{
	_members[first->info().nick()] = first;
	_operators[first->info().nick()] = first;
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

void	Channel::setTopic(const std::string& set)
{
	(void)set;
}

void	Channel::getTopic(const User& requester) const
{
	(void)requester;
}

bool	Channel::isMember(const User& user) const
{
	(void)user;
	return (true);
}

bool	Channel::isMember(const std::string& user) const
{
	(void)user;
	return (true);
}

bool	Channel::isOperator(const User& user) const
{
	(void)user;
	return (true);
}

bool	Channel::isOperator(const std::string& user) const
{
	(void)user;
	return (true);
}

bool	Channel::isInvited(const User& user) const
{
	(void)user;
	return (true);
}

bool	Channel::isInvited(const std::string& user) const
{
	(void)user;
	return (true);
}

const std::string&	Channel::name(void) const
{
	return (_name);
}

const Mode&	Channel::mode(void) const
{
	return (_mode);
}

void	Channel::join(User* user)
{
	(void)user;
}