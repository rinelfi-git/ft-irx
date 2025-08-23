#ifndef USERINFO_HPP
# define USERINFO_HPP

# include <string>
# include <iostream>

class UserInfo
{
	private:
		std::string	_nick;
		std::string	_uname;
		std::string	_realName;
		std::string	_host;
		std::string	_server;
	public:
		UserInfo();
		~UserInfo();
		UserInfo& operator=(const UserInfo& another);
		void	setNick(std::string nick);
		void	setUname(std::string uname);
		void	setRealName(std::string realName);
		void	setHost(std::string host);
		void	setServer(std::string server);
		const std::string&	getNick() const;
		const std::string&	getUname() const;
		const std::string&	getRealName() const;
		const std::string&	getHost() const;
		const std::string&	getServer() const;
};

#endif
