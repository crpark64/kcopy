// KCopy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include "PathUtilFn.h"
#include "StringUtil.h"



int wmain( int argc, wchar_t *argv[ ], wchar_t *envp[ ] )
{
    CString str;
    CAtlArray<CString> arr;


    // Ŭ�����忡�� ������
    str = GetClipboardText();
    MakePathArray(str, arr);


    // 2���� ���� ��� �Ű������� ����
    CString srcDir, dstDir;
    if ( 3 != argc )
    {
        CStringUtil::OutputDebugStringW(L"Invalid arg, arg count: %d. arg count must be 3. \r\n", argc);
        return -1;
    }


    // TotalCommander ���� Parameter �� ������ ���� �ۼ�
    // "%x%P\" "%x%T\"
    // ���� �ڿ� \ �� �ִ� ������, Total Commander ���� %x%P ���� ���� �ڿ� \ �� �⺻������ �پ ���� ��
    // ������ Cmd ���� \�� �̹� ������, "%x%P" �� �ϸ� "c:\" �� �Ǳ� ������ \ �� ���� Escape ���ڷ� �ν� ��
    // \\ �� �ǵ��� "%x%P\" �� ó���Ͽ� "c:\\" �� �Ͽ� Escape ���� 
    if ( FALSE == ArgParse( srcDir, dstDir ) )
    {
        CStringUtil::OutputDebugStringW( L"Invalid arg, parse failed. \r\n");
        return -1;
    }

    // 2���� ��ΰ� �����ϴ��� Ȯ��
    if ( FALSE == PathFileExists( srcDir ) )
    {
        CStringUtil::OutputDebugStringW( L"Src path not found. (%s)\r\n", srcDir);
        return -1;
    }
    if ( FALSE == PathFileExists( dstDir ) )
    {
        CStringUtil::OutputDebugStringW( L"Dst path not found. (%s)\r\n", dstDir );
        return -1;
    }


    CStringUtil::OutputDebugStringW( L"\r\n" );
    CStringUtil::OutputDebugStringW( L"Src path. (%s)\r\n", srcDir );
    CStringUtil::OutputDebugStringW( L"Dst path. (%s)\r\n", dstDir );
    CStringUtil::OutputDebugStringW( L"\r\n" );


    // Path �� \\ ����
    if ( L"\\" != srcDir.Right( 1 ) )
    {
        srcDir +=  L"\\";
    }
    if ( L"\\" != dstDir.Right( 1 ) )
    {
        dstDir += L"\\";
    }


    // Ŭ������ ���븸ŭ Loop ����
    size_t siCount = arr.GetCount();
    for ( size_t i = 0; i < siCount; i++ )
    {
        CString strUri = arr.GetAt(i);
        strUri.Replace(_T("/"), _T("\\"));


        // ���� ��� ����
        CString srcFile; srcFile.Format(L"%s%s", srcDir, strUri);
        CString dstFile; dstFile.Format(L"%s%s", dstDir, strUri);

        

        if ( FALSE == PathFileExists( srcFile ) )
        {
            CStringUtil::OutputDebugStringW( L"(%d/%d) Skipped: %s \r\n", (i+1), siCount, srcFile );

            // ���� ��ε� üũ
            if (PathFileExists(strUri) && CStringUtil::FindNoCase(strUri, srcDir) == 0)
            {
                CString strNewUri = strUri.Mid(srcDir.GetLength());
                if (strNewUri.Left(1) == "\\")
                {
                    strNewUri = strNewUri.Mid(1);
                }

                // �� ��� ����
                srcFile = strUri;
                dstFile.Format(L"%s%s", dstDir, strNewUri);
            }
            else
            {
                continue;
            }
        }

        CStringUtil::OutputDebugStringW( L"(%d/%d) File: %s \r\n", (i+1), siCount, srcFile );


        // ��� ���� ���� (Recursive)
        CString dstPathOnly = dstFile;
        dstPathOnly.Truncate( dstPathOnly.ReverseFind( '\\' ) );
        CreateDirectoryRecursively(dstPathOnly);
        
        // ���� ����
        CopyFile(srcFile, dstFile, FALSE);
        // ���� ����
        DeleteFile(srcFile);

    }

#ifndef _DEBUG
    wprintf(L"Press any key to continue...");
    _getch();
#endif
}

