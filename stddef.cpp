// Precompiled header
#include "stddef.h"

#ifdef WIN32
string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return string();
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], wstr.size(), nullptr, 0, nullptr, nullptr);
	string strTo(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], wstr.size(), &strTo[0], size, nullptr, nullptr);
	return strTo;
}

std::wstring utf8_decode(const string& str)
{
	if (str.empty()) return std::wstring();
	int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], str.size(), nullptr, 0);
	std::wstring wstrTo(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], str.size(), &wstrTo[0], size);
	return wstrTo;
}
#endif