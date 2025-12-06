#include "Response.hpp"
#include "ASocketClient.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "IRCServer.hpp"
#include <string>



Response::Response(const ASocketClient& client):
	_client(&client)
{}

void	Response::rplWelcome(const User& user) const
{
	_client->send(":" + IRCServer::serverName + " 001 " + user.info().nick() + " :Welcome to the Internet Relay Network " + user.networkId());
}

void	Response::rplYourHost(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 002 " + nick + " :Your host is ft_irc, running version homemade-0.1");
}

void	Response::rplCreated(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 003 " + nick + " :This server was created 2025-08-08 14:31");
}

void	Response::rplMyInfo(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 004 " + nick + " ft_irc homemade-0.1  oiklt");
}

void	Response::errNoMOTD(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 422 " + nick + " :MOTD File is missing");
}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel) const
{
	_client->send(":" + IRCServer::serverName + " 324 " + user + " " + channel.name() + " :" + channel.modeResume());
}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel, const std::string& modes) const
{
	_client->send(":" + IRCServer::serverName + " 324 " + user + " " + channel.name() + " :" + channel.modeResume(modes));
}

void	Response::rplInviting(const std::string& host, const std::string& guest, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 341 " + host + " " + guest + " " + channel);
}

void	Response::errNoSuchNick(const std::string& user, const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 401 " + user + " " + nick + " :No such nick");
}

void	Response::errNoSuchChannel(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 403 " + user + " " + channel + " :No such channel");
}

void	Response::errCannotSendToChannel(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 404 " + user + " " + channel + " :Cannot send to channel");
}

void	Response::errEmptyRecipient(const std::string& user) const
{
	_client->send(":" + IRCServer::serverName + " 411 " + user + " :No recipient given (PRIVMSG)");
}

void	Response::errEmptyContent(const std::string& user) const
{
	_client->send(":" + IRCServer::serverName + " 412 " + user + " :No text to send");
}

void	Response::errErrOneusNickname(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 432 * " + nick + " :Erroneous nickname");
}

void	Response::errNicknameInUse(const std::string& nick) const
{
	_client->send(":" + IRCServer::serverName + " 433 * " + nick + " :Nickname is already in use");
}

void	Response::errNotOnThatChannel(const std::string& op, const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 441 " + op + " " + channel + " " + user + " :They aren't on that channel");
}

void	Response::errNotOnThatChannel(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 442 " + user + " " + channel + " :You're not on that channel");
}

void	Response::errUserOnChannel(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 443 " + user + " " + channel + " :is already on channel");
}

void	Response::errNeedMoreParams(const std::string& user, const std::string& command) const
{
	_client->send(":" + IRCServer::serverName + " 461 " + user + " " + command + " :Not enough parameters");
}

void	Response::errPasswdMismatch(void) const
{
	_client->send(":" + IRCServer::serverName + " 464 :Password incorrect");
}

void	Response::errCannotJoinLimited(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 471 " + user + " " + channel + " :Cannot join channel (+l)");
}

void	Response::errCannotJoinInvite(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 473 " + user + " " + channel + " :Cannot join channel (+i)");
}

void	Response::errCannotJoinPassword(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 475 " + user + " " + channel + " :Cannot join channel (+k)");
}

void	Response::errChanOPrivsNeeded(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 482 " + user + " " + channel + " :You're not channel operator");
}

void	Response::errInvalidLimit(const std::string& user, const std::string& channel) const
{
	_client->send(":" + IRCServer::serverName + " 696 " + user + " " + channel + " l :Invalid limit");
}

void	Response::errNotRegistered(const std::string& user) const
{
	_client->send(":" + IRCServer::serverName + " 451 " + user + " :You have not registered");
}

void	Response::errNoNicknameGiven(const std::string& user) const
{
	_client->send(":" SERVER_NAME " 431 " + user + " :No nickname given");
}

void	Response::errAlreadyRegistered(void) const
{
	_client->send(":" SERVER_NAME " 462 :You may not reregister");
}
