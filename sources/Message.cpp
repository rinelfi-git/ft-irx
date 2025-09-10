#include <string>
#include "Message.hpp"
#include "User.hpp"
#include <sstream>

Message::Message(const User& sender, const std::string& content , const std::string& to):
	_content(content),
	_to(to),
	_sender(sender)
{}

Message::~Message()
{}

const Message&	Message::to(const std::string& set)
{
	_to = set;
	return (*this);
}

std::string	Message::toString(void) const
{
	std::ostringstream oss;
    oss << ":" << _sender.networkId()
        << " PRIVMSG " << _to
        << " :" << _content;
    return oss.str();
}