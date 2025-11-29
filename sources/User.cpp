#include "User.hpp"
#include "Message.hpp"
#include "Authenticated.hpp"
#include "UserInfo.hpp"
#include "utils.hpp"
#include <cctype>

User::User(const UserInfo& info, Authenticated* socket):
	_id(strToLower(info.nick())),
	_info(info),
	_socket(socket)
{
	socket->user(this);
}

User::User(const User& other):
	_id(other._id),
	_info(other._info),
	_socket(other._socket)
{}

User::~User()
{}

void	User::message(const Message& message)
{
	(void) message;
}

std::string	User::networkId(void) const
{
	return (_info.nick() + "!" + _info.uname() + "@" + _info.host());
}

const UserInfo&	User::info(void) const
{
	return (_info);
}

const Authenticated&	User::socket(void) const
{
	return (*_socket);
}

bool	User::isNickName(const std::string& str)
{
	if (str.empty() || str.length() > 9)
		return (false);
	std::string::const_iterator	itStr(str.begin());
	if (!isAlpha(*itStr))
		return (false);
	++itStr;
	for (; itStr != str.end(); itStr++)
	{
		char	c(*itStr);
		if (!std::isalnum(c) && std::string("-[]\\`^{}").find(c) == std::string::npos)
			return (false);
	}
	return (true);
}

const std::string&	User::id(void) const
{
	return (_id);
}