#ifndef STRINGHELPERS_H_
#define STRINGHELPERS_H_

std::wstring toWideString(std::string shortString)
{
	std::wstring wideString;
	return wideString.assign(shortString.begin(), shortString.end());
}

std::string toShortString(std::wstring wideString)
{
	std::string shortString;
	return shortString.assign(wideString.begin(), wideString.end());
}

#endif
