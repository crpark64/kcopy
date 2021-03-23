#pragma once

#include <atlstr.h>
#include <atlcoll.h>
#include <shlobj_core.h>

#pragma warning( disable : 4477 )

CString GetClipboardText();
void MakePathArray( CString str, CAtlArray<CString>& arr );
BOOL ArgParse( CString& src, CString& dst );
int CreateDirectoryRecursively( LPCTSTR path );

