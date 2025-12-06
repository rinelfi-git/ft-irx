#ifndef PENDING_HPP
# define PENDING_HPP
# include "ASocketClient.hpp"
# include "UserInfo.hpp"
# include <string>
# include <map>
# include <vector>
# include <exception>

class	Pending: public ASocketClient
{
private:
	std::string	_id;
	UserInfo	_userInfo;
	std::string	_password;
	int		_currentStep;
	void	_parseNick(const std::string& in);
	void	_parsePass(const std::string& in);
	void	_parseUser(const std::string& in);
	void	_parseCap(const std::string& in);
protected:
	virtual void	parse(const std::map<std::string, std::string>& cmds);
public:
	Pending(int fd);
	~Pending();
	bool	auth(void);
	class	PassMismatchException: public std::exception
	{
	public:
		virtual const char* what(void) const throw();
	};
	const std::string&	password(void) const;
	const UserInfo&	userInfo(void) const;
	const std::string&	id(void) const;
};
#endif