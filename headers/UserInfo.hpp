#ifndef USERINFO_HPP
# define USERINFO_HPP

# include <string>
# include <iostream>

class UserInfo
{
	private:
		std::string	_nick;
		std::string	_uname;
		std::string	_realname;
		std::string	_host;
		std::string	_server;
	public:
		UserInfo();
		~UserInfo();
		UserInfo&	operator=(const UserInfo& another);
		UserInfo&	nick(const std::string& set);
		UserInfo&	uname(const std::string& set);
		UserInfo&	realname(const std::string& set);
		UserInfo&	host(const std::string& set);
		UserInfo&	server(const std::string& set);
		const std::string&	nick(void) const;
		const std::string&	uname(void) const;
		const std::string&	realname(void) const;
		const std::string&	host(void) const;
		const std::string&	server(void) const;
};

#endif
