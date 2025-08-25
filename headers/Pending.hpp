#ifndef PENDING_HPP
# define PENDING_HPP
# include "ASocketClient.hpp"
# include "UserInfo.hpp"
# include <string>
# include <map>

class	Pending: public ASocketClient
{
private:
	UserInfo	_userInfo;
	std::string	_password;
	void	_parseNick(const std::string& in);
	void	_parsePass(const std::string& in);
	void	_parseUser(const std::string& in);
	void	_parseCap(const std::string& in);
public:
	Pending(int fd);
	~Pending();
	bool	auth(void);
	virtual void	parse(const std::map<std::string, std::string>& cmds);
	const std::string&	password(void) const;
	const UserInfo&	userInfo(void) const;
};
#endif