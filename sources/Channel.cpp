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
}

Channel::~Channel()
{}

void	Channel::message(const Message& msg) const
{}

void	Channel::invite(const User& host, const User& guest)
{}

void	Channel::setTopic(const std::string& set)
{}

void	Channel::getTopic(const User& requester) const
{}

bool	Channel::isMember(const User& user) const
{}

bool	Channel::isMember(const std::string& user) const
{}

bool	Channel::isOperator(const User& user) const
{}

bool	Channel::isOperator(const std::string& user) const
{}

bool	Channel::isInvited(const User& user) const
{}

bool	Channel::isInvited(const std::string& user) const
{}

const std::string&	Channel::name(void) const
{}

const Mode&	Channel::mode(void) const
{}