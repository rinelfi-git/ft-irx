#include "Pending.hpp"
#include "ASocketClient.hpp"
#include "UserInfo.hpp"
#include "IRCServer.hpp"
#include "utils.hpp"
#include "User.hpp"
#include "Response.hpp"
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <iostream>

Pending::Pending(int fd):
	ASocketClient(fd),
	_id(),
	_userInfo(),
	_password(),
	_currentStep(0)
{}

Pending::~Pending()
{}

bool	Pending::auth(void)
{
	if (!_userInfo.complete())
		return (false);
	bool	auth(IRCServer::getInstance().auth(*this));
	if (!auth)
		_currentStep = 0;
	return auth;
}

void	Pending::parse(const std::map<std::string, std::string>& cmds)
{
	std::vector<std::pair<std::string, void (Pending::*)(const std::string&)> > actions;
	actions.push_back(std::make_pair("cap", &Pending::_parseCap));
	actions.push_back(std::make_pair("pass", &Pending::_parsePass));
	actions.push_back(std::make_pair("nick", &Pending::_parseNick));
	actions.push_back(std::make_pair("user", &Pending::_parseUser));
	
	for (size_t	i(0); i < actions.size(); i++)
	{
		std::map<std::string, std::string>::const_iterator	cmdPtr(cmds.find(actions[i].first));
		if (cmdPtr != cmds.end())
		{
			std::cout << "execute : " << cmdPtr->first << std::endl;
			(this->*actions[i].second)(cmdPtr->second);
		}
	}
}

void	Pending::_parseNick(const std::string& in)
{
	std::cout << "nick STEP " << _currentStep << std::endl;
	if (in.empty())
		return Response(*this).errNoNicknameGiven("*");
	if (_currentStep == 0)
		return ;
	if (!User::isNickName(in))
		return Response(*this).errErrOneusNickname(in);
	if (IRCServer::getInstance().user(strToLower(in)))
		return Response(*this).errNicknameInUse(in);
	_userInfo.nick(in);
	_id = strToLower(in);
	_currentStep++;
}

void	Pending::_parsePass(const std::string& in)
{
	std::cout << "PASS STEP " << _currentStep << std::endl;
	if (in.empty())
		return Response(*this).errNeedMoreParams("*", "PASS");
	_password = in;
	_currentStep++;
}

void	Pending::_parseUser(const std::string& in)
{
	std::stringstream	builder(in);
	std::string			uname;
	std::string			host;
	std::string			server;
	std::string			realname;
	char				ddot;

	std::cout << "USER STEP " << _currentStep << std::endl;
	if (_currentStep == 0)
		return ;
	std::cout << "JUMP 1" << std::endl;
	if (_currentStep == 1)
		return Response(*this).errNotRegistered("*");
	std::cout << "JUMP 2" << std::endl;
	builder >> uname;
	builder >> host;
	builder >> server;
	builder >> ddot;
	std::getline(builder, realname);

	if (uname.empty() || host.empty() || server.empty() || realname.empty())
		return Response(*this).errNeedMoreParams(_userInfo.nick(), "USER");
	_userInfo.uname(uname)
		.host(host)
		.server(server)
		.realname(realname);
	_currentStep++;
	std::cout << "JUMP 3" << std::endl;
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