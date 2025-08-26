#include "ASocketClient.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>

ASocketClient::ASocketClient(int fd):
	_fd(fd),
	_buffer()
{}

ASocketClient::ASocketClient(const ASocketClient& o):
	_fd(o._fd),
	_buffer(o._buffer)
{}

ASocketClient::~ASocketClient()
{}

void	ASocketClient::send(const std::string& data) const
{
	if (_fd != -1)
	{
		std::cout << OUT_COLOR << data << std::endl << DEFAULT_COLOR;
		std::string msg = data + "\r\n";
		::send(_fd, msg.c_str(), msg.length(), 0);
	}
}

void	ASocketClient::input(const std::string& data)
{
	_buffer += data;
	std::map<std::string, std::string>	cmds;

	while (!_buffer.empty())
	{
		size_t	space(_buffer.find(' '));
		size_t	cr(_buffer.find('\r'));
		size_t	lf(_buffer.find('\n'));
		if (lf == std::string::npos)
			break;
		if (space == std::string::npos)
			_buffer = _buffer.substr(lf + 1);
		else if (space < lf)
		{
			size_t offset = space + 1;
			std::string	line(_buffer.substr(0, lf));
			size_t		length(lf - offset);
			if (cr != std::string::npos)
				length--;
			std::string index(strToLower(_buffer.substr(0, space)));
			std::string value(_buffer.substr(offset, length));
			_buffer = _buffer.substr(lf + 1);
			cmds[index] = value;
			std::cout << IN_COLOR << "(" << index << ")[" << value << "]" << std::endl << DEFAULT_COLOR;
		}
	}
	parse(cmds);
}

int	ASocketClient::fd(void) const
{
	return (_fd);
}