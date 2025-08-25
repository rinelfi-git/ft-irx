#ifndef AUTHENTICATED_HPP
# define AUTHENTICATED_HPP
# include "ASocketClient.hpp"
# include <string>
# include <map>

class	Pending;

class Authenticated: public ASocketClient
{
private:
	void	_parseMode(const std::string& arg);
	void	_parsePrivMsg(const std::string& arg);
	void	_parsePing(const std::string& arg);
	void	_parseJoin(const std::string& arg);
	void	_parseInvite(const std::string& arg);
	void	_iMode(bool enable, const std::string& name);
	void	_tMode(bool enable, const std::string& name);
	void	_kMode(bool enable, const std::string& name, const std::string& password);
	void	_oMode(bool enable, const std::string& name, const std::string& user);
	void	_lMode(bool enable, const std::string& name, const std::string& limit);
protected:
	virtual void	parse(const std::map<std::string, std::string>& cmds);
public:
	Authenticated(int fd);
	Authenticated(const Pending& pending);
	~Authenticated();
};

#endif
