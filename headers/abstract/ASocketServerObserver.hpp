#ifndef ASOCKET_SERVER_OBSERVER_HPP
# define ASOCKET_SERVER_OBSERVER_HPP
# include <string>

class	SocketServer;

class	ASocketServerObserver
{
private:
	SocketServer*	_observable;
public:
	ASocketServerObserver(void);
	virtual ~ASocketServerObserver();
	virtual void	onConnect(int fd) = 0;
	virtual void	onData(int fd, const std::string& data) = 0;
	virtual void	onDisconnect(int fd) = 0;
	void			bind(SocketServer& observable);
	void			unbind(SocketServer& observable);
	SocketServer&	observable(void) const;
};
#endif