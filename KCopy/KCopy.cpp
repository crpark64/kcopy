// KCopy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PathUtilFn.h"



int wmain( int argc, wchar_t *argv[ ], wchar_t *envp[ ] )
{
    CString str;
	CAtlArray<CString> arr;


	// 클립보드에서 가져옴
	str = GetClipboardText();
	MakePathArray(str, arr);


	// 2개의 절대 경로 매개변수로 받음
	CString src, dst;
	if ( 3 != argc )
	{
		wprintf(L"Invalid arg, arg count: %d. arg count must be 3. \r\n", argc);
		return -1;
	}


	// TotalCommander 에서 Parameter 에 다음과 같이 작성
	// "%x%P\" "%x%T\"
	// 가장 뒤에 \ 를 넣는 이유는, Total Commander 에서 %x%P 사용시 가장 뒤에 \ 가 기본적으로 붙어서 제공 됨
	// 윈도우 Cmd 에서 \가 이미 있으면, "%x%P" 로 하면 "c:\" 로 되기 때문에 \ 에 의해 Escape 문자로 인식 됨
	// \\ 가 되도록 "%x%P\" 로 처리하여 "c:\\" 로 하여 Escape 방지 
	if ( FALSE == ArgParse( src, dst ) )
	{
		wprintf( L"Invalid arg, parse failed. \r\n");
		return -1;
	}

	// 2개의 경로가 존재하는지 확인
	if ( FALSE == PathFileExists( src ) )
	{
		wprintf( L"Src path not found. (%s)\r\n", src);
		return -1;
	}
	if ( FALSE == PathFileExists( dst ) )
	{
		wprintf( L"Dst path not found. (%s)\r\n", dst );
		return -1;
	}

	// Path 에 \\ 붙임
	if ( L"\\" != src.Right( 1 ) )
	{
		src +=  L"\\";
	}
	if ( L"\\" != dst.Right( 1 ) )
	{
		dst += L"\\";
	}


	// 클립보드 내용만큼 Loop 진행
	size_t siCount = arr.GetCount();
	for ( size_t i = 0; i < siCount; i++ )
	{
		CString strUri = arr.GetAt(i);
		strUri.Replace(_T("/"), _T("\\"));


		// 폴더 경로 생성
		CString srcFile; srcFile.Format(L"%s%s", src, strUri);
		CString dstFile; dstFile.Format(L"%s%s", dst, strUri);


		if ( FALSE == PathFileExists( srcFile ) )
		{
			continue;
		}


		// 대상 폴더 생성 (Recursive)
		CString dstPathOnly = dstFile;
		dstPathOnly.Truncate( dstPathOnly.ReverseFind( '\\' ) );
		CreateDirectoryRecursively(dstPathOnly);
		
		// 파일 복사
		CopyFile(srcFile, dstFile, FALSE);
		// 원본 삭제
		DeleteFile(srcFile);
	}
}

