#include "UserInfo.hpp"

UserInfo::UserInfo()
{}

UserInfo::~UserInfo()
{}

UserInfo& UserInfo::operator=(const UserInfo& another)
{
	nick(another.nick());
	uname(another.uname());
	realname(another.realname());
	host(another.host());
	server(another.server());
	return (*this);
}


void	UserInfo::nick(const std::string& set)
{
	_nick = set;
}
void	UserInfo::uname(const std::string& set)
{
	_uname = set;
}
void	UserInfo::realname(const std::string& set)
{
	_realname = set;
}
void	UserInfo::host(const std::string& set)
{
	_host = set;
}
void	UserInfo::server(const std::string& set)
{
	_server = set;
}
const std::string&	UserInfo::nick() const
{
	return (_nick);
}

const std::string&	UserInfo::uname() const
{
	return (_uname);
}

const std::string&	UserInfo::realname() const
{
	return (_realname);
}

const std::string&	UserInfo::host() const
{
	return (_host);
}

const std::string&	UserInfo::server() const
{
	return (_server);
}
