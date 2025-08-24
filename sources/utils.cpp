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