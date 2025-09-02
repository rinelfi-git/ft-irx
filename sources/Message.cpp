#include <string>
#include "Message.hpp"
#include "User.hpp"

Message::Message(const User& sender, const std::string& content):
	_content(content),
	_to(),
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
	return("");
}