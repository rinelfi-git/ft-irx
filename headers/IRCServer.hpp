#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP
# include <string>
# include <iostream>
# include <map>
# include "ASocketServerObserver.hpp"

class	User;
class	UserInfo;
class	ASocketClient;
class	Channel;
class	Pending;

class IRCServer: public ASocketServerObserver
{
    private:
        std::string	_password;
		std::map<int, ASocketClient*>	_pendings;
		std::map<std::string, User*>	_users;
		std::map<std::string, Channel*>	_channels;
		static IRCServer*	_instance;
        void	_createUser(UserInfo info, ASocketClient* socket);
        void	_createChannel(User first, std::string name);
    public:
        static IRCServer& getInstance(void);
        IRCServer(const std::string& password);
        ~IRCServer();
        void			joinChannel(const User& user, const std::string& name);
        void			auth(const UserInfo& user, const std::string& password);
        User*			user(const std::string& nick) const;
        Channel*		channel(const std::string& name) const;
		virtual void	onConnect(int fd);
		virtual void	onData(int fd, const std::string& data);
		virtual void	onDisconnect(int fd);
};
#endif