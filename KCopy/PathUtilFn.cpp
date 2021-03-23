#include "PathUtilFn.h"



CString GetClipboardText()
{
	CString str;
	HANDLE hData = NULL;

	do
	{
		if ( FALSE == IsClipboardFormatAvailable( CF_UNICODETEXT ) )
		{
			break;
		}

		if ( TRUE == OpenClipboard( 0 ) )
		{
			hData = GetClipboardData( CF_UNICODETEXT );
			if ( hData != nullptr )
			{
				LPWSTR pszText = static_cast<LPWSTR>(GlobalLock( hData ));
				if ( pszText != nullptr )
				{
					str = pszText;

					// Release the lock
					GlobalUnlock( hData );
				}
			}

			// Release the clipboard
			CloseClipboard();
		}


	} while ( 0 );

	return str;
}

void MakePathArray( CString str, CAtlArray<CString>& arr )
{
	CString Seperator = _T( "\n" );
	int Position = 0;
	CString Token;

	Token = str.Tokenize( Seperator, Position );
	while ( !Token.IsEmpty() )
	{
		Token.Trim();
		if ( 0 < Token.GetLength() )
		{
			arr.Add( Token );
		}
		Token = str.Tokenize( Seperator, Position );
	}
}

BOOL ArgParse( CString& src, CString& dst )
{
	CString str;
	CAtlArray<CString> arr;
	LPWSTR* szArglist;
	int nArgs;
	int i;

	szArglist = CommandLineToArgvW( GetCommandLineW(), &nArgs );
	if ( NULL == szArglist )
	{
		wprintf( L"CommandLineToArgvW failed \r\n" );
		return FALSE;
	}
	else
	{
		for ( i = 0; i < nArgs; i++ )
		{
			printf( "%d: %ws\n", i, szArglist[i] );
			str = szArglist[i];
			str.Trim();
			if ( 0 < str.GetLength() )
			{
				arr.Add( szArglist[i] );
			}

		}
	}
	LocalFree( szArglist );


	if ( 3 == arr.GetCount() )
	{
		src = arr.GetAt( 1 );
		dst = arr.GetAt( 2 );
		return TRUE;
	}

	return FALSE;
}


int CreateDirectoryRecursively( LPCTSTR path )
{
	return SHCreateDirectoryEx( NULL, path, NULL );
}
