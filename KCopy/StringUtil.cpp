#include "StringUtil.h"
#include <strsafe.h>

#include <string>
#include <vector>

#include <windows.h>
#include <stdarg.h>

#pragma warning(disable : 4996)
#pragma warning(disable : 4267)
#pragma warning(disable : 4101)

CStringUtil::CStringUtil()
{
}

CStringUtil::~CStringUtil()
{
}


bool CStringUtil::TokenizeEmptyAllowedA( __in const std::string source, __in const std::string delimiter, __out std::vector<std::string>& v )
{
	size_t pos;
	std::string token;
	std::string s = source;

	while ( (pos = s.find( delimiter )) != std::string::npos )
	{
		token = s.substr( 0, pos );

		CStringA temp = token.c_str();
		temp.Trim();
		v.push_back( temp.GetBuffer() );

		s.erase( 0, pos + delimiter.length() );
	}
	v.push_back( s );

	return true;
}

bool CStringUtil::TokenizeA(__in const std::string source, __in const std::string delimiter, __out std::vector<std::string> &v)
{
	size_t pos;
	std::string token;
	std::string s = source;

	while ( (pos = s.find( delimiter )) != std::string::npos )
	{
		token = s.substr( 0, pos );

		CStringA temp = token.c_str();
		temp.Trim();
		if ( 0 < temp.GetLength() )
		{
			v.push_back( temp.GetBuffer() );
		}

		s.erase( 0, pos + delimiter.length() );
	}
	v.push_back( s );
	
	return true;
}


bool CStringUtil::TokenizeW( __in const std::wstring source, __in const std::wstring delimiter, __out std::vector<std::wstring>& v )
{
	size_t pos;
	std::wstring token;
	std::wstring s = source;

	while ( (pos = s.find( delimiter )) != std::wstring::npos )
	{
		token = s.substr( 0, pos );

		CStringW temp = token.c_str();
		temp.Trim();
		if ( 0 < temp.GetLength() )
		{
			v.push_back( temp.GetBuffer() );
		}

		s.erase( 0, pos + delimiter.length() );
	}
	v.push_back( s );

	return true;
}

bool CStringUtil::ParseCookieStringW( __in const std::wstring cookieString, __out std::wstring& cookieKeyName, __out CookieMapW& map )
{
	const std::wstring cookieBodyDelimiter = L";";
	const std::wstring cookieKeyValueDelimiter = L"=";

	std::vector<std::wstring> cookieBody;

	CStringUtil::TokenizeW( cookieString.c_str(), cookieBodyDelimiter, cookieBody );

	for ( std::vector<std::wstring>::iterator it = cookieBody.begin(); it != cookieBody.end(); it++ )
	{
		// eg)
		// Set-Cookie: promotion_data=deleted; expires=Thu, 01-Jan-1970 00:00:01 GMT; Max-Age=0; path=/; domain=test.mercury.com2us.com
		// Set-Cookie: promotion_data=%7B%22user%22%3A%7B%22vid%22%3A%22-703%22%2C%22uid%22%3A%22205841665%22%2C%22did%22%3A%22560913732%22%2C%22game_language%22%3A%22ko%22%2C%22language%22%3A%22ko%22%2C%22hive_country%22%3A%22kr%22%2C%22country%22%3A%22kr%22%2C%22world%22%3A%222%22%2C%22device%22%3A%22Redmi+Note+7%22%2C%22os_version%22%3A%229%22%2C%22advertising_id%22%3A%22a028a95d2bc2457e90e76e3ae2318b78%22%2C%22imei%22%3A%22864357042641158%22%7D%2C%22game%22%3A%7B%22appid%22%3A%22com.com2us.hivesdk.normal.freefull.google.global.android.common%22%2C%22app_version%22%3A%225.4.0%22%2C%22lib_version%22%3A%224.15.0%22%2C%22additionalinfo%22%3A%22%22%7D%2C%22promotion%22%3A%7B%22pid%22%3A%22201951%22%2C%22content_key%22%3A%227777895%22%2C%22webview%22%3A%22news%22%2C%22forced%22%3A%22on%22%2C%22type_camp%22%3A%22all%22%2C%22type_banner%22%3A%22small+%22%7D%7D; expires=Thu, 29-Oct-2020 05:52:17 GMT; Max-Age=3600; path=/ 

		std::vector<std::wstring> cookieKeyValue;
		CStringUtil::TokenizeW( (*it).c_str(), cookieKeyValueDelimiter, cookieKeyValue );
		if ( 2 == cookieKeyValue.size() )
		{
			CStringW key = cookieKeyValue[0].c_str();
			CStringW value = cookieKeyValue[1].c_str();

			if ( it == cookieBody.begin() )
			{
				cookieKeyName = key;
			}
			else
			{
				// 쿠키 이름 외에는 모두 lowercase 로 만듦
				key.MakeLower();
			}

			map.insert(std::pair<std::wstring, std::wstring>(key, value));
		}
	}


	// 기본 키 강제로 설정
	CookieMapW::iterator find;

	if ( map.end() == map.find( L"expires" ) )
	{
		map.insert( std::pair<std::wstring, std::wstring>( L"expires", L"" ) );
	}
	if ( map.end() == map.find( L"max-age" ) )
	{
		map.insert( std::pair<std::wstring, std::wstring>( L"max-age", L"" ) );
	}
	if ( map.end() == map.find( L"path" ) )
	{
		map.insert( std::pair<std::wstring, std::wstring>( L"path", L"" ) );
	}
	if (  map.end() == map.find( L"domain" ) )
	{
		map.insert( std::pair<std::wstring, std::wstring>( L"domain", L"" ) );
	}

	return true;
}

std::string CStringUtil::URLEncodeA(std::string str)
{
	std::string new_str = "";
	char c;
	int ic;
	const char* chars = str.c_str();
	char bufHex[10];
	int len = strlen(chars);

	for(int i=0;i<len;i++){
		c = chars[i];
		ic = c;
		// uncomment this if you want to encode spaces with +
		/*if (c==' ') new_str += '+';   
		else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
		else {
			sprintf(bufHex,"%X",c);
			if(ic < 16) 
				new_str += "%0"; 
			else
				new_str += "%";
			new_str += bufHex;
		}
	}
	return new_str;
}

std::string CStringUtil::URLDecodeA(std::string str)
{
	std::string ret;
	char ch;
	int i, ii, len = str.length();

	for (i=0; i < len; i++){
		if(str[i] != '%'){
			if(str[i] == '+')
				ret += ' ';
			else
				ret += str[i];
		}else{
			sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
	}
	return ret;
}


BOOL CStringUtil::CommandLineExcludeFirstArg(/* OUT */ CString& excludedFirstArgCommandLine)
{
	CString str;

	BOOL bRetval = FALSE;

	LPWSTR* szArglist = NULL;
	int nArgs;
	int i;

	LPWSTR lpszCommandLine;

	do
	{
		lpszCommandLine = GetCommandLineW();
		if ( NULL == lpszCommandLine )
		{
			break;
		}

		szArglist = CommandLineToArgvW( lpszCommandLine, &nArgs );
		if ( NULL == szArglist )
		{
			break;
		}

		if ( 1 <= nArgs )
		{
			CString strCommandLine = lpszCommandLine;

			CString strFirstArg;
			if ( L"\"" == strCommandLine.Left( 1 ) )
			{
				// 첫 번째 경로가 "(=double quote) 로 묶여 있는 경우가 있어서 처리
				strFirstArg.Format(L"\"%s\"", szArglist[0]);
			}
			else
			{
				strFirstArg = szArglist[0];
			}
			
			if ( 0 == strCommandLine.Find( strFirstArg ) )
			{
				// do nothing
			}
			else
			{
				break;
			}

			str = strCommandLine.Mid(strFirstArg.GetLength() + 1);
			str.Trim();
			excludedFirstArgCommandLine = str;

			bRetval = TRUE;
		}

	} while(FALSE);


	if ( NULL != szArglist )
	{
		LocalFree(szArglist);
		szArglist = NULL;
	}


	return bRetval;
}

void CStringUtil::OutputDebugStringW( LPCWSTR format, ... )
{
	WCHAR szBuf[1024] = {0, };

	va_list lpStart;
	va_start( lpStart, format );

	vswprintf_s( szBuf, RTL_NUMBER_OF(szBuf), format, lpStart );

	va_end( lpStart );

	::OutputDebugStringW(szBuf);
    //CStringW strBuf = szBuf;
    //strBuf.Replace(L"%", L"%%"); // wprintf 할 때, 문자열에 % 가 포함되면, 뒤의 Arg 참조 시도하다가 crash 발생
    //wprintf(strBuf);
}
