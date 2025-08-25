#ifndef USER_HPP
# define USER_HPP
# include "UserInfo.hpp"
# include "Authenticated.hpp"

class	Pending;
class	Message;

class User
{
private:
	UserInfo 		_info;
	Authenticated	_socket;
public:
	User(const UserInfo& info, const Pending& pending);
	~User();
	std::string	networkld(void) const;
	void	message(const Message& message);
	const UserInfo& info(void) const;
	const Authenticated& socket(void) const;
};

#endif