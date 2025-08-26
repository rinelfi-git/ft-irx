#include "User.hpp"
#include "Message.hpp"
#include "Authenticated.hpp"
#include "UserInfo.hpp"

User::User(const UserInfo& info, Authenticated* socket):
	_info(info),
	_socket(socket)
{
	socket->user(this);
}

User::~User()
{}

void	User::message(const Message& message)
{
	(void) message;
}

std::string	User::networkld(void) const
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