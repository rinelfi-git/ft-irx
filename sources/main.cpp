#include "IRCServer.hpp"
#include "SocketServer.hpp"
#include "ArgValidator.hpp"
#include <exception>
#include <iostream>

int	main(int argc, char** argv)
{
	try
	{
		ArgValidator	validator(argc, argv);
		SocketServer	socket(validator.port());
		IRCServer		server(validator.password());
		server.bind(socket);
		socket.start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return (42);
	}
	return (0);
}