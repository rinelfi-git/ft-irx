#ifndef MODE_HPP
# define MODE_HPP

typedef	unsigned int	t_uint;

class	Mode
{
private:
	bool	_inviteOnly;
	t_uint	_memberLimit;
	bool	_topicRestricted;
public:
	Mode(void);
	~Mode();
	bool	isInviteOnly(void) const;
	bool	isTopicRestricted(void) const;
	t_uint	memberLimit(void) const;
	void	inviteOnly(bool set);
	void	topicRestricted(bool set);
	void	memberLimit(t_uint set);
};
#endif