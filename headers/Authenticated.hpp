#ifndef AUTHENTICATED_HPP
# define AUTHENTICATED_HPP
# include "ASocketClient.hpp"
# include <string>
# include <map>

class	Pending;
class	User;
class Channel;

class Authenticated: public ASocketClient
{
private:
	User*	_user;
	void	_parseMode(const std::string& arg);
	void	_parsePrivMsg(const std::string& arg);
	void	_parsePing(const std::string& arg);
	void	_parseJoin(const std::string& arg);
	void	_parseInvite(const std::string& arg);
	void	_parseTopic(const std::string& arg);
	void	_parseQuit(const std::string& arg);
	void	_parseKick(const std::string& arg);
	void	_parsePart(const std::string& arg);
	void	_tMode(const std::string& name, char action);
	void	_iMode(const std::string& name, char action);
	void	_kMode(const std::string& name, char action, const std::string& password);
	void	_oMode(const std::string& name, char action, const std::string& user);
	void	_lMode(const std::string& name, char action, const std::string& limit);
	void	_lMode(const std::string& name, char action);
	void	_parseRegister(const std::string& arg);
protected:
	virtual void	parse(const std::map<std::string, std::string>& cmds);
public:
	Authenticated(int fd);
	Authenticated(const Pending& pending);
	~Authenticated();
	User*	user(void) const;
	void	user(User* set);
};

#endif
