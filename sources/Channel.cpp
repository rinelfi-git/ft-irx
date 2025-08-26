#include "Channel.hpp"
#include "Mode.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Authenticated.hpp"
#include <string>
#include <map>

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

void	Channel::setTopic(const User& setter, const std::string& set)
{
	std::map<std::string, User*>::const_iterator	memberPtr(_members.begin());

	_topic = set;
	while (memberPtr != _members.end())
	{
		User	member(*(memberPtr++)->second);
		member.socket().send("332 " + setter.info().nick() + " " + _name + " :" + _topic);
	}
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
	
	return (_operators.find(user.info().nick()) != _operators.end());
}

bool	Channel::isOperator(const std::string& user) const
{
	return (_operators.find(user) != _operators.end());
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
	std::string	nick( user->info().nick());
	_members[nick] = user;
	std::map<std::string, User*>::const_iterator	memberPtr(_members.begin());

	while (memberPtr != _members.end())
	{
		User	member(*(memberPtr++)->second);
		member.socket().send(":" + user->networkld() + " JOIN " + _name);
	}
	user->socket().send("353 " + nick + " = " + _name + " :" + getUsers());
	user->socket().send("366 " + nick + " " + _name + " :End of /NAMES list.");
	if (!_topic.empty())
		user->socket().send("332 " + nick + " " + _name + " :" + _topic);
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