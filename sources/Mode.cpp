#include "Mode.hpp"

Mode::Mode(void):
	_inviteOnly(false),
	_memberLimit(0),
	_topicRestricted(false)
{}

Mode::~Mode()
{}

bool	Mode::isInviteOnly(void) const
{
	return (_inviteOnly);
}

bool	Mode::isTopicRestricted(void) const
{
	return (_topicRestricted);
}

t_uint	Mode::memberLimit(void) const
{
	return (_memberLimit);
}

void	Mode::inviteOnly(bool set)
{
	_inviteOnly = set;
}

void	Mode::topicRestricted(bool set)
{
	_topicRestricted = set;
}

void	Mode::memberLimit(t_uint set)
{
	_memberLimit = set;
}