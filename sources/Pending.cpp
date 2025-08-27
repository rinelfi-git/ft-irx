#include "Pending.hpp"
#include "ASocketClient.hpp"
#include "UserInfo.hpp"
#include "IRCServer.hpp"
#include "utils.hpp"
#include "User.hpp"
#include <string>
#include <map>
#include <sstream>

Pending::Pending(int fd):
	ASocketClient(fd),
	_id(),
	_userInfo(),
	_password()
{}

Pending::~Pending()
{}

bool	Pending::auth(void)
{
	if (!_userInfo.complete())
		return (false);
	if (User::isNickName(_userInfo.nick()) == false)
	{
		send("432 * " + _userInfo.nick() + " :Erroneous nickname");
		return (false);
	}
	return IRCServer::getInstance().auth(*this);
}

void	Pending::parse(const std::map<std::string, std::string>& cmds)
{
	std::map<std::string, void (Pending::*)(const std::string&)>	actions;
	actions["user"] = &Pending::_parseUser;
	actions["nick"] = &Pending::_parseNick;
	actions["cap"] = &Pending::_parseCap;
	actions["pass"] = &Pending::_parsePass;
	
	std::map<std::string, void (Pending::*)(const std::string&)>::iterator	actionPtr(actions.begin());
	while (actionPtr != actions.end())
	{
		std::map<std::string, std::string>::const_iterator	cmdPtr(cmds.find(actionPtr->first));
		if (cmdPtr != cmds.end())
			(this->*actions.at(cmdPtr->first))(cmdPtr->second);
		actionPtr++;
	}
}

void	Pending::_parseNick(const std::string& in)
{
	_userInfo.nick(in);
	_id = strToLower(in);
}

void	Pending::_parsePass(const std::string& in)
{
	_password = in;
}

void	Pending::_parseUser(const std::string& in)
{
	std::stringstream	builder(in);
	std::string			uname;
	std::string			host;
	std::string			server;
	std::string			realname;
	char				ddot;

	builder >> uname;
	builder >> host;
	builder >> server;
	builder >> ddot;
	std::getline(builder, realname);

	_userInfo.uname(uname)
		.host(host)
		.server(server)
		.realname(realname);
}

void	Pending::_parseCap(const std::string& in)
{
	std::stringstream	builder(in);
	std::string			cmd;
	std::string			arg;

	builder >> cmd;
	builder >> arg;

	if (cmd == "LS")
		send("CAP * LS :");
}

const std::string&	Pending::password() const
{
	return (_password);
}

const UserInfo&	Pending::userInfo() const
{
	return (_userInfo);
}

const std::string&	Pending::id(void) const
{
	return (_id);
}