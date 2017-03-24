// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (64f2860d).
//
// d:\visual studio\project\config\semidebug\olepro32.tlh
//
// C++ source equivalent of Win32 type library olepro32.dll
// compiler-generated file created 10/29/03 at 14:53:44 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>



//
// Type library items
//


struct __declspec(uuid("7bf80981-bf32-101a-8bbb-00aa00300cab"))
Picture : IDispatch
{
    //
    // Property data
    //

    __declspec(property(get=GetHandle))
    OLE_HANDLE Handle;
    __declspec(property(get=GethPal,put=PuthPal))
    OLE_HANDLE hPal;
    __declspec(property(get=GetType))
    short Type;
    __declspec(property(get=GetWidth))
    OLE_XSIZE_HIMETRIC Width;
    __declspec(property(get=GetHeight))
    OLE_YSIZE_HIMETRIC Height;

    //
    // Wrapper methods for error-handling
    //

    // Methods:
    // Method 'Render' not emitted because argument 'lprcWBounds' has invalid type: void *

    // Properties:
    OLE_HANDLE GetHandle ( );
    OLE_HANDLE GethPal ( );
    void PuthPal ( OLE_HANDLE _val );
    short GetType ( );
    OLE_XSIZE_HIMETRIC GetWidth ( );
    OLE_YSIZE_HIMETRIC GetHeight ( );
};

_COM_SMARTPTR_TYPEDEF(Picture, __uuidof(IDispatch));
#include "olepro32.cpp"
#pragma pack(pop)
