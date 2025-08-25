#ifndef ARG_VALIDATOR_HPP
# define ARG_VALIDATOR_HPP
# include <string>

class	ArgValidator
{
private:
	int			_port;
	std::string	_password;
public:
	ArgValidator(int argc, char** argv);
	int					port(void) const;
	const std::string&	password(void) const;
};
#endif