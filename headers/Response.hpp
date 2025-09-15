#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <string>

class	Channel;
class	ASocketClient;
class	Response
{
private:
	const ASocketClient*	_client;
public:
	Response(const ASocketClient& client);
	void	rplChannelModeIs(const std::string& user, const Channel& channel) const;
	void	rplChannelModeIs(const std::string& user, const Channel& channel, const std::string& modes) const;
	void	errNoSuchChannel(const std::string& user, const std::string& channel) const;
	void	errNeedMoreParams(const std::string& user, const std::string& command) const;
	void	errNoSuchNick(const std::string& user, const std::string& nick) const;
	void	errNotOperator(const std::string& user, const std::string& channel) const;
	void	errInvalidLimit(const std::string& user, const std::string& channel) const;
	void	errEmptyRecipient(const std::string& user) const;
	void	errEmptyContent(const std::string& user) const;
	void	errCannotSendToChannel(const std::string& user, const std::string& channel) const;
	void	errCannotJoinLimited(const std::string& user, const std::string& channel) const;
	void	errCannotJoinInvite(const std::string& user, const std::string& channel) const;
	void	errCannotJoinPassword(const std::string& user, const std::string& channel) const;
};
#endif