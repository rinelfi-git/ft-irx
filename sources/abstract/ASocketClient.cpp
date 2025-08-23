#include "ASocketClient.hpp"
#include <string>

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
{}

void	ASocketClient::input(const std::string& data)
{}