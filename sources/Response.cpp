#include "Response.hpp"
#include "ASocketClient.hpp"
#include "Channel.hpp"
#include <string>

Response::Response(ASocketClient& client):
	_client(client)
{}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel) const
{
	_client.send("324 " + user + " " + channel.name() + " :" + channel.modeResume());
}

void	Response::rplChannelModeIs(const std::string& user, const Channel& channel, const std::string& modes) const
{
	_client.send("324 " + user + " " + channel.name() + " :" + channel.modeResume(modes));
}

void	Response::errNoSuchChannel(const std::string& user, const std::string& channel) const
{
	_client.send("403 " + user + " " + channel + " :No such channel");
}

void	Response::errNeedMoreParams(const std::string& user, const std::string& command) const
{
	_client.send("461 " + user + " " + command + " :Not enough parameters");
}

void	Response::errNoSuchNick(const std::string& user, const std::string& nick) const
{
	_client.send("401 " + user + " " + nick + " :No such nick");
}