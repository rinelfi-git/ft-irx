#ifndef AUTHENTICATED_HPP
# define AUTHENTICATED_HPP
# include "ASocketClient.hpp"
# include <string>
# include <map>

class	Pending;

class Authenticated: public ASocketClient
{
public:
	Authenticated(int fd);
	Authenticated(const Pending& pending);
	~Authenticated();
	void	parseMode(const std::string& arg);
	void	parsePrivMsg(const std::string& arg);
	void	parsePing(const std::string& arg);
	void	parseJoin(const std::string& arg);
	void	parseInvite(const std::string& arg);
	void	iMode(bool enable, const std::string& name);
	void	tMode(bool enable, const std::string& name);
	void	kMode(bool enable, const std::string& name, const std::string& password);
	void	oMode(bool enable, const std::string& name, const std::string& user);
	void	lMode(bool enable, const std::string& name, const std::string& limit);
	virtual void	parse(const std::map<std::string, std::string>& cmds);
};

#endif
