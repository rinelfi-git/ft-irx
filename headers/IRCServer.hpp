#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP
# include <string>
# include <iostream>
# include <ASocketServerObserver.hpp>

class	User;
class	UserInfo;
class	ASocketClient;
class	Channel;

class IRCServer: public ASocketServerObserver
{
    private:
        std::string     	_password;
        int             	_port;
		static IRCServer*	_instance;
        void	_createUser(UserInfo info, ASocketClient socket);
        void	_createChannel(User first, std::string name);
    public:
        static IRCServer& getInstance();
        IRCServer(int port, const std::string& password);
        ~IRCServer();
        void			joinChannel(User user, std::string name);
        void			auth(UserInfo user, std::string password);
        User*			user(std::string nick);
        Channel*		channel(std::string name);
		virtual void	onConnection(int fd);
		virtual void	onData(int fd, const std::string& data);
		virtual void	onDisconnection(int fd);
};
#endif