#ifndef USER_HPP
# define USER_HPP
# include "UserInfo.hpp"

class	ASocketClient;

class User
{
private:
	UserInfo _info;
public:
	User(const UserInfo& info);
	~User();
	std::string	networkld(void) const;
	void	message(const Message& message);
	void	connect(ASocketClient* socket);
	void	disconnect(void);
	const UserInfo& info(void) const;
};

#endif