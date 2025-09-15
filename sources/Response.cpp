#include "Response.hpp"
#include "ASocketClient.hpp"
#include "Channel.hpp"
#include <string>

Response::Response(const ASocketClient& client):
	_client(&client)
{}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel) const
{
	_client->send("324 " + user + " " + channel.name() + " :" + channel.modeResume());
}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel, const std::string& modes) const
{
	_client->send("324 " + user + " " + channel.name() + " :" + channel.modeResume(modes));
}

void	Response::errNoSuchNick(const std::string& user, const std::string& nick) const
{
	_client->send("401 " + user + " " + nick + " :No such nick");
}

void	Response::errNoSuchChannel(const std::string& user, const std::string& channel) const
{
	_client->send("403 " + user + " " + channel + " :No such channel");
}

void	Response::errCannotSendToChannel(const std::string& user, const std::string& channel) const
{
	_client->send("404 " + user + " " + channel + " :Cannot send to channel");
}

void	Response::errEmptyRecipient(const std::string& user) const
{
	_client->send("411 " + user + " :No recipient given (PRIVMSG)");
}

void	Response::errEmptyContent(const std::string& user) const
{
	_client->send("412 " + user + " :No text to send");
}

void	Response::errNeedMoreParams(const std::string& user, const std::string& command) const
{
	_client->send("461 " + user + " " + command + " :Not enough parameters");
}

void	Response::errCannotJoinLimited(const std::string& user, const std::string& channel) const
{
	_client->send("471 " + user + " " + channel + " :Cannot join channel (+l)");
}

void	Response::errCannotJoinInvite(const std::string& user, const std::string& channel) const
{
	_client->send("473 " + user + " " + channel + " :Cannot join channel (+i)");
}

void	Response::errCannotJoinPassword(const std::string& user, const std::string& channel) const
{
	_client->send("475 " + user + " " + channel + " :Cannot join channel (+k)");
}

void	Response::errNotOperator(const std::string& user, const std::string& channel) const
{
	_client->send("482 " + user + " " + channel + " :You're not channel operator");
}

void	Response::errInvalidLimit(const std::string& user, const std::string& channel) const
{
	_client->send("696 " + user + " " + channel + " l :Invalid limit");
}
