#ifndef UTILS_HPP
# define UTILS_HPP
# include <string>
# define OUT_COLOR "\033[32m"
# define IN_COLOR "\033[34m"
# define DEFAULT_COLOR "\033[0m"

std::string	strToLower(const std::string& str);
bool		isnumber(const std::string& str);
#endif