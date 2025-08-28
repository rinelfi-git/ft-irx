#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <map>
# include <string>
# include "Mode.hpp"

class	User;
class	Message;

class	Channel
{
private:
	std::string						_name;
	std::string						_password;
	std::string						_topic;
	Mode							_mode;
	std::map<std::string, User*>	_members;
	std::map<std::string, User*>	_operators;
	std::map<std::string, User*>	_inviteds;
public:
	Channel(const std::string& name, User* first);
	void	message(const Message& msg) const;
	void	invite(const User& host, const User& guest);
	void	setTopic(const User& setter, const std::string& set);
	void	getTopic(const User& requester) const;
	bool	isMember(const User& user) const;
	bool	isMember(const std::string& id) const;
	bool	isOperator(const User& user) const;
	bool	isOperator(const std::string& id) const;
	bool	isInvited(const User& user) const;
	bool	isInvited(const std::string& id) const;
	void	join(User* user);
	const std::string&	name(void) const;
	const Mode&			mode(void) const;
	std::string			modeResume(void) const;
	std::string			modeResume(const std::string& modes) const;
	std::string			getUsers() const;
	bool				auth(User* user, const std::string& password);
	void				setPassword(const User& setter, const std::string& password);
	void				setTopicMode(const User& setter, bool operatorOnly);
	void				quit(const User& user, const std::string& msg);
	static bool			isChannelName(const std::string& str);
	void				broadcast(const std::string& msg) const;
};
#endif