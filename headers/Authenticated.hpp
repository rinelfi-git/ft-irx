#ifndef AUTHENTICATED_HPP
# define AUTHENTICATED_HPP

#include <iostream>

class Authenticated
{
	public:
		Authenticated(/* args */);
		~Authenticated();
		Authenticated	*parseMode(std::string arg);
		Authenticated	*parsePrivMsg(std::string arg);
		Authenticated	*parsePing(std::string arg);
		Authenticated	*parseJoin(std::string arg);
		Authenticated	*parseInvite(std::string arg);
		Authenticated	*iMode(bool enable, std::string name);
		Authenticated	*tMode(bool enable, std::string name);
		Authenticated	*kMode(bool enable, std::string name, std::string password);
		Authenticated	*oMode(bool enable, std::string name, std::string user);
		Authenticated	*IMode(bool enable, std::string name, std::string limit);
};

#endif
