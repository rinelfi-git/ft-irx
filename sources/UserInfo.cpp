#include "../headers/UserInfo.hpp"

UserInfo::UserInfo()
{}

UserInfo::~UserInfo()
{}

UserInfo& UserInfo::operator=(const UserInfo& another)
{
	this->setNick(getNick());
	this->setUname(getUname());
	this->setRealName(getRealName());
	this->setHost(getHost());
	this->setServer(getServer());
	return (*this);
}


void	UserInfo::setNick(std::string nick)
{
	this->_nick = nick;
}
void	UserInfo::setUname(std::string uname)
{
	this->_uname = uname;
}
void	UserInfo::setRealName(std::string realName)
{
	this->_realName = realName;
}
void	UserInfo::setHost(std::string host)
{
	this->_host = host;
}
void	UserInfo::setServer(std::string server)
{
	this->_server = server;
}
const std::string&	UserInfo::getNick() const
{
	return (_nick);
}

const std::string&	UserInfo::getUname() const
{
	return (_uname);
}

const std::string&	UserInfo::getRealName() const
{
	return (_realName);
}

const std::string&	UserInfo::getHost() const
{
	return (_host);
}

const std::string&	UserInfo::getServer() const
{
	return (_server);
}
