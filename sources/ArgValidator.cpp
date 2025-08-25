#include "ArgValidator.hpp"
#include "utils.hpp"
#include <string>
#include <stdexcept>
#include <cstdlib>

ArgValidator::ArgValidator(int argc, char** argv)
{
	if (argc != 3)
		throw std::invalid_argument("Invalid number of arguments.\n./ircserv <port> <password>");
	if (!isnumber(std::string(argv[1])))
		throw std::invalid_argument("Invalid port number.");
	_port = std::atoi(argv[1]);
	if (_port < 1024 || _port > 49151)
		throw std::invalid_argument("Port number must be between 1024 and 49151.");
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