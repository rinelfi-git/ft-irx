#include "ASocketClient.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

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

bool	ASocketClient::input(const std::string& data)
{
	if (data.empty() || data.find('\r') == 0 || data.find('\n') == 0)
		return (false);
	
	_buffer += data;
	std::map<std::string, std::string>	cmds;
	while (!_buffer.empty())
	{
		size_t	space(_buffer.find(' '));
		size_t	carriageReturn(_buffer.find('\r'));
		size_t	lineFeed(_buffer.find('\n'));
		bool	hasCarriageReturn(carriageReturn != std::string::npos);
		if (lineFeed == std::string::npos)
			break;
		// have no space
		if (space == std::string::npos)
		{
			size_t	length(lineFeed);
			if (hasCarriageReturn)
				length--;
			std::string	index(strToLower(_buffer.substr(0, length)));
			if (!index.empty())
			{
				cmds[index] = "";
				std::cout << IN_COLOR << '{' << _fd << "} (" << index << ") EMPTY PARAMETER" << std::endl << DEFAULT_COLOR;
			}
			_buffer = _buffer.substr(lineFeed + 1);
		}
		else if (space < lineFeed)
		{
			size_t offset = space + 1;
			std::string	line(_buffer.substr(0, lineFeed));
			size_t		length(lineFeed - offset);
			if (hasCarriageReturn)
				length--;
			std::string index(strToLower(_buffer.substr(0, space)));
			if (!index.empty())
			{
				std::string value(_buffer.substr(offset, length));
				cmds[index] = value;
				std::cout << IN_COLOR << '{' << _fd << "} (" << index << ")[" << value << "]" << std::endl << DEFAULT_COLOR;
			}
			_buffer = _buffer.substr(lineFeed + 1);
		}
	}
	parse(cmds);
	return (true);
}

int	ASocketClient::fd(void) const
{
	return (_fd);
}

void	ASocketClient::close(void) const
{
	if (_fd != -1)
		::close(_fd);
}