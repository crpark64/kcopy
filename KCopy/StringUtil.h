#pragma once

#include <atlstr.h>
#include <string>
#include <vector>
#include <map>

class CStringUtil
{
public:
	typedef std::multimap<std::string, std::string> CookieMapA;
	typedef std::multimap<std::wstring, std::wstring> CookieMapW;

public:
	CStringUtil();
	virtual ~CStringUtil();

	static bool TokenizeEmptyAllowedA(__in const std::string source, __in const std::string delimiter, __out std::vector<std::string> &v);
	static bool TokenizeA(__in const std::string source, __in const std::string delimiter, __out std::vector<std::string> &v);
	static bool TokenizeW(__in const std::wstring source, __in const std::wstring delimiter, __out std::vector<std::wstring> &v);

	//static bool ParseCookieStringA();
	static bool ParseCookieStringW(__in const std::wstring cookieString, __out std::wstring& cookieKeyName, __out CookieMapW &map);

	static std::string URLEncodeA(std::string str);
	static std::string URLDecodeA(std::string str);

	// Command Line 에서 첫뻔째 Param(=실행파일명)을 빼고, 제공
	static BOOL CommandLineExcludeFirstArg(/* OUT */ CString& excludedFirstArgCommandLine);
	static void OutputDebugStringW(LPCWSTR format, ...);

private:
};

