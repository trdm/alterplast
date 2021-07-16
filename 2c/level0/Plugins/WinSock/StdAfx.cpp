// stdafx.cpp : source file that includes just the standard includes
//	Test.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
CString GetGetLastStringError()
{
	CString csRes;
	DWORD dwLastError=GetLastError();

    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //

    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //

    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
		csRes=MessageBuffer;

        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }
	return csRes;
}