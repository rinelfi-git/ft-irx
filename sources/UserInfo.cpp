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


UserInfo&	UserInfo::nick(const std::string& set)
{
	_nick = set;
	return (*this);
}
UserInfo&	UserInfo::uname(const std::string& set)
{
	_uname = set;
	return (*this);
}
UserInfo&	UserInfo::realname(const std::string& set)
{
	_realname = set;
	return (*this);
}
UserInfo&	UserInfo::host(const std::string& set)
{
	_host = set;
	return (*this);
}
UserInfo&	UserInfo::server(const std::string& set)
{
	_server = set;
	return (*this);
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
