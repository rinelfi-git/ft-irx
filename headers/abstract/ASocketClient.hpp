#ifndef A_SOCKET_CLIENT_HPP
# define A_SOCKET_CLIENT_HPP
# include <string>
# include <map>

class	ASocketClient
{
private:
	const int		_fd;
	std::string	_buffer;
public:
	ASocketClient(int fd);
	ASocketClient(const ASocketClient& o);
	virtual ~ASocketClient();
	void	send(const std::string& data) const;
	void	input(const std::string& data);
	virtual void	parse(const std::map<std::string, std::string>& cmds) = 0;
};
#endif