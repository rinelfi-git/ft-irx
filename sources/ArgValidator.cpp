#include "ArgValidator.hpp"
#include <string>
#include <sstream>
#include <stdexcept>

ArgValidator::ArgValidator(int argc, char** argv)
{
	if (argc != 3)
		throw std::invalid_argument("Invalid number of arguments. ./ircserv <port> <password>");
	std::stringstream	builder(argv[1]);

	builder >> _port;
	if (builder.fail())
		throw std::invalid_argument("Invalid port number.");
	_password = std::string(argv[2]);
}

int	ArgValidator::port(void) const
{
	return (_port);
}

const std::string&	ArgValidator::password(void) const
{
	return (_password);
}