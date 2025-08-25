#include "User.hpp"
#include "Message.hpp"

User::User(const UserInfo& info): _info(info)
{}

void	User::message(const Message& message)
{
	(void) message;
}

std::string	User::networkld(void) const
{
	return ("std::string	User::networkld(void)");
}

void	User::connect(ASocketClient *socket)
{
	(void)socket;
}

void	User::disconnect(void)
{}

User::~User()
{}

const UserInfo&	User::info(void) const
{
	return (_info);
}
