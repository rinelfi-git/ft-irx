#ifndef ASOCKET_SERVER_OBSERVER_HPP
# define ASOCKET_SERVER_OBSERVER_HPP
# include <string>

class	SocketServer;

class	ASocketServerObserver
{
private:
	SocketServer&	_observable;
public:
	ASocketServerObserver(SocketServer& observable);
	virtual ~ASocketServerObserver();
	virtual void	onConnection(int fd) = 0;
	virtual void	onData(int fd, const std::string& data) = 0;
	virtual void	onDisconnect(int fd) = 0;
};
#endif