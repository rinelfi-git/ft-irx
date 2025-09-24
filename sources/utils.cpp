#include <string>
#include <cctype>
#include <iostream>
#include <vector>
#include <sstream>

std::string	strToLower(const std::string& str)
{
	std::string	out(str);

	std::string::iterator	outPtr(out.begin());
	while (outPtr != out.end())
	{
		*outPtr = std::tolower(*outPtr);
		outPtr++;
	}
	return (out);
}

bool	isnumber(const std::string& str)
{
	std::string::const_iterator	strPtr(str.begin());

	if (str.empty())
		return (false);
	if (*strPtr == '+' || *strPtr == '-')
		strPtr++;
	while (strPtr != str.end())
	{
		if (!std::isdigit(*strPtr))
			return (false);
		strPtr++;
	}
	return (true);
}

bool	isLower(char c)
{
	return (c >= 'a' && c <= 'z');
}

bool	isUpper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

bool	isAlpha(char c)
{
	return (isLower(c) || isUpper(c));
}

std::vector<std::string>	ft_split(const std::string& str, char delimiteur)
{
	std::vector<std::string> result;
	size_t	start(0), end(0);
	
	while ((end = str.find(delimiteur, start)) != std::string::npos)
	{
		result.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	result.push_back(str.substr(start));
	return (result);
}
