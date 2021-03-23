// KCopy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PathUtilFn.h"



int wmain( int argc, wchar_t *argv[ ], wchar_t *envp[ ] )
{
    CString str;
	CAtlArray<CString> arr;


	// Ŭ�����忡�� ������
	str = GetClipboardText();
	MakePathArray(str, arr);


	// 2���� ���� ��� �Ű������� ����
	CString src, dst;
	if ( 3 != argc )
	{
		wprintf(L"Invalid arg, arg count: %d. arg count must be 3. \r\n", argc);
		return -1;
	}


	// TotalCommander ���� Parameter �� ������ ���� �ۼ�
	// "%x%P\" "%x%T\"
	// ���� �ڿ� \ �� �ִ� ������, Total Commander ���� %x%P ���� ���� �ڿ� \ �� �⺻������ �پ ���� ��
	// ������ Cmd ���� \�� �̹� ������, "%x%P" �� �ϸ� "c:\" �� �Ǳ� ������ \ �� ���� Escape ���ڷ� �ν� ��
	// \\ �� �ǵ��� "%x%P\" �� ó���Ͽ� "c:\\" �� �Ͽ� Escape ���� 
	if ( FALSE == ArgParse( src, dst ) )
	{
		wprintf( L"Invalid arg, parse failed. \r\n");
		return -1;
	}

	// 2���� ��ΰ� �����ϴ��� Ȯ��
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

	// Path �� \\ ����
	if ( L"\\" != src.Right( 1 ) )
	{
		src +=  L"\\";
	}
	if ( L"\\" != dst.Right( 1 ) )
	{
		dst += L"\\";
	}


	// Ŭ������ ���븸ŭ Loop ����
	size_t siCount = arr.GetCount();
	for ( size_t i = 0; i < siCount; i++ )
	{
		CString strUri = arr.GetAt(i);
		strUri.Replace(_T("/"), _T("\\"));


		// ���� ��� ����
		CString srcFile; srcFile.Format(L"%s%s", src, strUri);
		CString dstFile; dstFile.Format(L"%s%s", dst, strUri);


		if ( FALSE == PathFileExists( srcFile ) )
		{
			continue;
		}


		// ��� ���� ���� (Recursive)
		CString dstPathOnly = dstFile;
		dstPathOnly.Truncate( dstPathOnly.ReverseFind( '\\' ) );
		CreateDirectoryRecursively(dstPathOnly);
		
		// ���� ����
		CopyFile(srcFile, dstFile, FALSE);
		// ���� ����
		DeleteFile(srcFile);
	}
}

