#include <string>
#include <cctype>

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