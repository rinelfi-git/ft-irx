#include "../headers/Authenticated.hpp"

Authenticated::Authenticated(/* args */)
{}

Authenticated::~Authenticated()
{}

Authenticated	*Authenticated::parseMode(std::string arg)
{
	(void)arg;
	return(this);
}

Authenticated	*Authenticated::parsePrivMsg(std::string arg)
{
	(void)arg;
	return(this);
}

Authenticated	*Authenticated::parsePing(std::string arg)
{
	(void)arg;
	return(this);
}

Authenticated	*Authenticated::parseJoin(std::string arg)
{
	(void)arg;
	return(this);

}

Authenticated	*Authenticated::parseInvite(std::string arg)
{
	(void)arg;
	return(this);

}

Authenticated	*Authenticated::iMode(bool enable, std::string name)
{
	(void)enable;
	(void)name;
	return(this);
}

Authenticated	*Authenticated::tMode(bool enable, std::string name)
{
	(void)enable;
	(void)name;
	return(this);


}

Authenticated	*Authenticated::kMode(bool enable, std::string name, std::string password)
{
	(void)enable;
	(void)name;
	(void)password;
	return(this);

}

Authenticated	*Authenticated::oMode(bool enable, std::string name, std::string user)
{
	(void)enable;
	(void)name;
	(void)user;
	return(this);

}

Authenticated	*Authenticated::IMode(bool enable, std::string name, std::string limit)
{
	(void)enable;
	(void)name;
	(void)limit;

}

