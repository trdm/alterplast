// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (deedd976).
//
// d:\visual studio\project\config\vis1cpp\semidebug\zlibeng.tlh
//
// C++ source equivalent of Win32 type library d:\Program Files\1CQL\BIN\zlibeng.dll
// compiler-generated file created 04/01/03 at 14:15:42 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

//
// Forward references and typedefs
//

struct __declspec(uuid("aa6267a0-7aec-11d2-9f23-008048da120f"))
/* dual interface */ IzipEntry;
struct /* coclass */ zlibEngine;
struct __declspec(uuid("c5276445-f299-11d0-8149-004095e1f439"))
/* interface */ IzlibEngine;
struct __declspec(uuid("be44fe00-7afc-11d2-9f23-008048da120f"))
/* interface */ IEnum_zipEntry;
struct /* coclass */ jpegEngine;
struct __declspec(uuid("e196d996-fb8c-11d0-ae1d-d9180f4c4641"))
/* interface */ IjpegEngine;
struct /* coclass */ tiffEngine;
struct __declspec(uuid("e196d998-fb8c-11d0-ae1d-d9180f4c4641"))
/* interface */ ItiffEngine;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IzipEntry, __uuidof(IzipEntry));
_COM_SMARTPTR_TYPEDEF(IzlibEngine, __uuidof(IzlibEngine));
_COM_SMARTPTR_TYPEDEF(IEnum_zipEntry, __uuidof(IEnum_zipEntry));
_COM_SMARTPTR_TYPEDEF(IjpegEngine, __uuidof(IjpegEngine));
_COM_SMARTPTR_TYPEDEF(ItiffEngine, __uuidof(ItiffEngine));

//
// Type library items
//

struct __declspec(uuid("aa6267a0-7aec-11d2-9f23-008048da120f"))
IzipEntry : IDispatch
{
    //
    // Property data
    //

    __declspec(property(get=GetComment))
    _bstr_t Comment;
    __declspec(property(get=GetCompressedSize))
    unsigned long CompressedSize;
    __declspec(property(get=GetCrc32))
    unsigned long Crc32;
    __declspec(property(get=GetMethod))
    unsigned short Method;
    __declspec(property(get=GetName))
    _bstr_t Name;
    __declspec(property(get=GetSize))
    unsigned long Size;
    __declspec(property(get=GetTime))
    unsigned long Time;
    __declspec(property(get=GetFlags))
    unsigned long Flags;

    //
    // Wrapper methods for error-handling
    //

    _bstr_t GetComment ( );
    unsigned long GetCompressedSize ( );
    unsigned long GetCrc32 ( );
    unsigned short GetMethod ( );
    _bstr_t GetName ( );
    unsigned long GetSize ( );
    unsigned long GetTime ( );
    unsigned long GetFlags ( );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_Comment (
        BSTR * strComment ) = 0;
    virtual HRESULT __stdcall get_CompressedSize (
        unsigned long * nSize ) = 0;
    virtual HRESULT __stdcall get_Crc32 (
        unsigned long * nCrc ) = 0;
    virtual HRESULT __stdcall get_Method (
        unsigned short * nMethod ) = 0;
    virtual HRESULT __stdcall get_Name (
        BSTR * strName ) = 0;
    virtual HRESULT __stdcall get_Size (
        unsigned long * nSize ) = 0;
    virtual HRESULT __stdcall get_Time (
        unsigned long * nTime ) = 0;
    virtual HRESULT __stdcall get_Flags (
        unsigned long * nFlags ) = 0;
};

struct __declspec(uuid("c5276425-f299-11d0-8149-004095e1f439"))
zlibEngine;
    // [ default ] interface IzlibEngine

struct __declspec(uuid("c5276445-f299-11d0-8149-004095e1f439"))
IzlibEngine : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT pkZip (
        LPWSTR pszCommandLine );
    HRESULT pkUnzip (
        LPWSTR pszCommandLine );
    HRESULT pkCompress (
        struct IStream * lpstrmFrom,
        struct IStream * lpstrmTo );
    HRESULT pkDecompress (
        struct IStream * lpstrmFrom,
        struct IStream * lpstrmTo );
    HRESULT pkAppendFile (
        struct IStream * lpstrmFrom,
        LPWSTR pszFileTo,
        LPWSTR pszStreamName,
        LPWSTR pszPassword );
    HRESULT pkReadFile (
        LPWSTR pszFileFrom,
        LPWSTR pszStreamName,
        struct IStream * lpstrmTo,
        LPWSTR pszPassword );
    HRESULT pkReadEntry (
        LPWSTR pzsFileFrom,
        struct IEnum_zipEntry * * ppEnumEntry );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall raw_pkZip (
        LPWSTR pszCommandLine ) = 0;
    virtual HRESULT __stdcall raw_pkUnzip (
        LPWSTR pszCommandLine ) = 0;
    virtual HRESULT __stdcall raw_pkCompress (
        struct IStream * lpstrmFrom,
        struct IStream * lpstrmTo ) = 0;
    virtual HRESULT __stdcall raw_pkDecompress (
        struct IStream * lpstrmFrom,
        struct IStream * lpstrmTo ) = 0;
    virtual HRESULT __stdcall raw_pkAppendFile (
        struct IStream * lpstrmFrom,
        LPWSTR pszFileTo,
        LPWSTR pszStreamName,
        LPWSTR pszPassword ) = 0;
    virtual HRESULT __stdcall raw_pkReadFile (
        LPWSTR pszFileFrom,
        LPWSTR pszStreamName,
        struct IStream * lpstrmTo,
        LPWSTR pszPassword ) = 0;
    virtual HRESULT __stdcall raw_pkReadEntry (
        LPWSTR pzsFileFrom,
        struct IEnum_zipEntry * * ppEnumEntry ) = 0;
};

struct __declspec(uuid("be44fe00-7afc-11d2-9f23-008048da120f"))
IEnum_zipEntry : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT Next (
        unsigned long celt,
        struct IzipEntry * * rgelt,
        unsigned long * pceltFetched );
    HRESULT Skip (
        unsigned long celt );
    HRESULT Reset ( );
    HRESULT Clone (
        struct IEnum_zipEntry * * ppzipEntry );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall raw_Next (
        unsigned long celt,
        struct IzipEntry * * rgelt,
        unsigned long * pceltFetched ) = 0;
    virtual HRESULT __stdcall raw_Skip (
        unsigned long celt ) = 0;
    virtual HRESULT __stdcall raw_Reset ( ) = 0;
    virtual HRESULT __stdcall raw_Clone (
        struct IEnum_zipEntry * * ppzipEntry ) = 0;
};

struct __declspec(uuid("e196d997-fb8c-11d0-ae1d-d9180f4c4641"))
jpegEngine;
    // [ default ] interface IjpegEngine

struct __declspec(uuid("e196d996-fb8c-11d0-ae1d-d9180f4c4641"))
IjpegEngine : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT CreateDIBFromJPEG (
        LPWSTR pszPathName,
        unsigned int * hBitmap );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall raw_CreateDIBFromJPEG (
        LPWSTR pszPathName,
        unsigned int * hBitmap ) = 0;
};

struct __declspec(uuid("e196d999-fb8c-11d0-ae1d-d9180f4c4641"))
tiffEngine;
    // [ default ] interface ItiffEngine

struct __declspec(uuid("e196d998-fb8c-11d0-ae1d-d9180f4c4641"))
ItiffEngine : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT CreateDIBFromTIFF (
        LPWSTR pszPathName,
        unsigned int * hBitmap );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall raw_CreateDIBFromTIFF (
        LPWSTR pszPathName,
        unsigned int * hBitmap ) = 0;
};

//
// Wrapper method implementations
//

#include "d:\visual studio\project\config\vis1cpp\zlibeng.tli"

#pragma pack(pop)
