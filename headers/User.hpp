#ifndef USER_HPP
# define USER_HPP
# include "UserInfo.hpp"
# include <string>

class	Pending;
class	Message;
class	Authenticated;

class User
{
private:
	std::string		_id;
	UserInfo 		_info;
	Authenticated*	_socket;
public:
	User(const UserInfo& info, Authenticated* socket);
	User(const User& other);
	~User();
	std::string	networkId(void) const;
	void	message(const Message& message);
	const UserInfo& info(void) const;
	const Authenticated& socket(void) const;
	static bool	isNickName(const std::string& str);
	const std::string& id(void) const;
};

#endif