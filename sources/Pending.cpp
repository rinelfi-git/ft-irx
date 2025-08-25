#include "Pending.hpp"
#include "ASocketClient.hpp"
#include "UserInfo.hpp"
#include <string>
#include <map>
#include <sstream>

Pending::Pending(int fd):
	ASocketClient(fd),
	_userInfo(),
	_password()
{}

Pending::~Pending()
{}

bool	Pending::auth(void)
{return (false);}


// en plus du parsing, a la fin on execute l'authentification
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

	// code de l'authentification ici: a discuter
}

void	Pending::_parseNick(const std::string& in)
{(void)in;}

void	Pending::_parsePass(const std::string& in)
{(void)in;}

void	Pending::_parseUser(const std::string& in)
{(void)in;}

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