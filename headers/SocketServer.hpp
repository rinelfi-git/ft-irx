
#ifndef SOCKETSERVER_HPP
# define SOCKETSERVER_HPP
# define SOCKET_READ_BUFFER 512
# include "ASocketServerObserver.hpp"
# include <vector>
# include <poll.h>

class SocketServer
{
private:
	int _fd;
	bool _running;
	std::vector<struct pollfd> _polls;
	std::vector<ASocketServerObserver*>	_observers;
	void	_acceptClient(void);
	bool	_handleClient(int fd);
public:
	SocketServer(int portIn);
	~SocketServer();
	void	attachObserver(ASocketServerObserver *observerIn);
	void	detachObserver(ASocketServerObserver *observerIn);
	void	start(void);
	void	stop(void);
};
#endif
