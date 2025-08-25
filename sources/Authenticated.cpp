#include "../headers/Authenticated.hpp"

Authenticated::Authenticated(/* args */)
{}

Authenticated::~Authenticated()
{}

void	Authenticated::parseMode(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::parsePrivMsg(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::parsePing(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::parseJoin(const std::string& arg)
{
	(void)arg;

}

void	Authenticated::parseInvite(const std::string& arg)
{
	(void)arg;
}

void	Authenticated::iMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;
}

void	Authenticated::tMode(bool enable, const std::string& name)
{
	(void)enable;
	(void)name;


}

void	Authenticated::kMode(bool enable, const std::string& name, const std::string& password)
{
	(void)enable;
	(void)name;
	(void)password;

}

void	Authenticated::oMode(bool enable, const std::string& name, const std::string& user)
{
	(void)enable;
	(void)name;
	(void)user;

}

void	Authenticated::lMode(bool enable, const std::string& name, const std::string& limit)
{
	(void)enable;
	(void)name;
	(void)limit;

}

