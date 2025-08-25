#include "User.hpp"
#include "Message.hpp"
#include "Authenticated.hpp"
#include "Pending.hpp"
#include "UserInfo.hpp"

User::User(const UserInfo& info, const Pending& pending):
	_info(info),
	_socket(pending)
{}

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
	return (_socket);
}