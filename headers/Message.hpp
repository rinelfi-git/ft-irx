#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include <string>
# include "User.hpp"

class	Message
{
private:
	std::string	_content;
	std::string	_to;
	User		_sender;
public:
	Message(const User& sender, const std::string& content);
	~Message();
	const Message&	to(const std::string& set);
	std::string		toString(void) const;
};
#endif