// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (64f2860d).
//
// d:\visual studio\project\config\semidebug\olepro32.tli
//
// Wrapper implementations for Win32 type library olepro32.dll
// compiler-generated file created 10/29/03 at 14:53:44 - DO NOT EDIT!

#pragma once


//
// dispinterface Picture wrapper method implementations
//

inline OLE_HANDLE Picture::GetHandle ( ) {
    OLE_HANDLE _result;
    _com_dispatch_propget(this, 0x0, VT_I4, (void*)&_result);
    return _result;
}

inline OLE_HANDLE Picture::GethPal ( ) {
    OLE_HANDLE _result;
    _com_dispatch_propget(this, 0x2, VT_I4, (void*)&_result);
    return _result;
}

inline void Picture::PuthPal ( OLE_HANDLE _val ) {
    _com_dispatch_propput(this, 0x2, VT_I4, _val);
}

inline short Picture::GetType ( ) {
    short _result;
    _com_dispatch_propget(this, 0x3, VT_I2, (void*)&_result);
    return _result;
}

inline OLE_XSIZE_HIMETRIC Picture::GetWidth ( ) {
    OLE_XSIZE_HIMETRIC _result;
    _com_dispatch_propget(this, 0x4, VT_I4, (void*)&_result);
    return _result;
}

inline OLE_YSIZE_HIMETRIC Picture::GetHeight ( ) {
    OLE_YSIZE_HIMETRIC _result;
    _com_dispatch_propget(this, 0x5, VT_I4, (void*)&_result);
    return _result;
}
