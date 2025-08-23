#include "User.hpp"
#include "Message.hpp"

User		*User::message(Message message)
{
	return (this);
}

std::string	User::networkld(void)
{
	return ("std::string	User::networkld(void)");
}

User		*User::connect(ASocketClient *socket)
{
	return (this);
}

User		*User::disconnect(void)
{
	return (this);
}

User::User(const UserInfo& userInfo)
{
	this->_userInfo = userInfo;
}

User::~User()
{}
