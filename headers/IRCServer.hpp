/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tteloah <ttelolah@student.42antananariv    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:03:13 by tteloah           #+#    #+#             */
/*   Updated: 2025/08/23 17:20:38 by tteloah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <string>
#include <iostream>

class User;
class UserInfo;
class ASocketClient;

class IRCServer
{
    private:
        std::string     _password;
        int             _port;

        IRCServer(int port, std:: password);
        void createUser(UserInfo info, ASocketClient socket);
        void createChannel(User first, std::string name);
        
    public:
        IRCServer* getInstance(int port, std::string password);
        ~IRCServer();

        void joinChannel(User user, std::string name);
        void auth(UserInfo user, std::string password);
        User* user(std::string nick);
        Channel* channel(std::string name);

};

#endif