#ifndef USER_HPP
#define USER_HPP

#include "UserInfo.hpp"

class User
{
	private:
		UserInfo _userInfo;
	public:
		User(const UserInfo& userInfo);
		~User();
		std::string	networkld(void);
		User	*message(Message message);
		User	*connect(AsocketClient *socket);
		User	*disconnect(void);
};

#endif