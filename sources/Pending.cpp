#include "Pending.hpp"
#include "ASocketClient.hpp"
#include "UserInfo.hpp"
#include <string>
#include <map>

Pending::Pending(int fd):
	ASocketClient(fd),
	_userInfo(),
	_password()
{}

Pending::~Pending()
{}

bool	Pending::auth(void)
{}

void	Pending::parse(const map<std::string, std::string>& cmds)
{}

void	Pending::_parseNick(const std::string& in)
{}

void	Pending::_parsePass(const std::string& in)
{}

void	Pending::_parseUser(const std::string& in)
{}

void	Pending::_parseCap(const std::string& in)
{}