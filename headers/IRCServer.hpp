#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP
# include <string>
# include <iostream>
# include <map>
# include "ASocketServerObserver.hpp"

class	User;
class	UserInfo;
class	Channel;
class	Authenticated;
class	Pending;
class	ASocketClient;

class IRCServer: public ASocketServerObserver
{
    private:
        std::string	_password;
		std::map<int, ASocketClient*>	_socketClients;
		std::map<std::string, User*>	_users;
		std::map<std::string, Channel*>	_channels;
		static IRCServer*	_instance;
        User&	_createUser(const UserInfo& info, const Pending& pending);
		static void	_sigint(int num);
        public:
        static IRCServer& getInstance(void);
        IRCServer(const std::string& password);
        ~IRCServer();
        bool			auth(const Pending& pending);
        User*			user(const std::string& nick) const;
        Channel*		channel(const std::string& name) const;
		virtual void	onConnect(int fd);
		virtual void	onData(int fd, const std::string& data);
		virtual void	onDisconnect(int fd);
        void			createChannel(const std::string& name, User* first);
};
#endif