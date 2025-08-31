#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <string>

class	ASocketClient;
class	Channel;

class	Response
{
private:
	ASocketClient&	_client;
public:
	Response(ASocketClient& client);
	void	rplChannelModeIs(const std::string& user, const Channel& channel) const;
	void	rplChannelModeIs(const std::string& user, const Channel& channel, const std::string& modes) const;
	void	errNoSuchChannel(const std::string& user, const std::string& channel) const;
	void	errNeedMoreParams(const std::string& user, const std::string& command) const;
	void	errNoSuchNick(const std::string& user, const std::string& nick) const;
};
#endif