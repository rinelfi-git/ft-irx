#include "Authenticated.hpp"
#include "ASocketClient.hpp"
#include "Pending.hpp"

Authenticated::Authenticated(int fd):
	ASocketClient(fd)
{}

Authenticated::Authenticated(const Pending& pending):
	ASocketClient(pending)
{}

Authenticated::~Authenticated()
{}

void	Authenticated::_parseMode(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parsePrivMsg(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parsePing(const std::string& arg)
{
	send("pong " + arg);
}

void	Authenticated::_parseJoin(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_parseInvite(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::_iMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;
}

void	Authenticated::_tMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;


}

void	Authenticated::_kMode(bool enable, const std::string& name, const std::string& password)
{
	(void)enable;
	(void)name;
	(void)password;

}

void	Authenticated::_oMode(bool enable, const std::string& name, const std::string& user)
{
	(void)enable;
	(void)name;
	(void)user;

}

void	Authenticated::_lMode(bool enable, const std::string& name, const std::string& limit)
{
	(void)enable;
	(void)name;
	(void)limit;

}

void	Authenticated::parse(const std::map<std::string, std::string>& cmds)
{
	std::map<std::string, void (Authenticated::*)(const std::string&)>	actions;

	actions["mode"] = &Authenticated::_parseMode;
	actions["privmsg"] = &Authenticated::_parsePrivMsg;
	actions["ping"] = &Authenticated::_parsePing;
	actions["join"] = &Authenticated::_parseJoin;
	actions["invite"] = &Authenticated::_parseInvite;

	std::map<std::string, void (Authenticated::*)(const std::string&)>::iterator	actionPtr(actions.begin());
	while (actionPtr != actions.end())
	{
		std::map<std::string, std::string>::const_iterator	cmdPtr(cmds.find(actionPtr->first));
		if (cmdPtr != cmds.end())
			(this->*actions.at(cmdPtr->first))(cmdPtr->second);
		actionPtr++;
	}

	// other commands to implement here
	// kick, topic, part, notice, whois, away, quit, list, names, oper, userhost, motd, lusers, version, time, admin, info
	// and all the channel modes +i +k +l +o +t
}