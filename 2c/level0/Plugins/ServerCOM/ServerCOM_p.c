/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Jun 02 22:09:21 2004
 */
/* Compiler settings for C:\Visual C++\2C\Enterprise\PLUGINS\ServerCOM\ServerCOM.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "ServerCOM.h"

#define TYPE_FORMAT_STRING_SIZE   1037                              
#define PROC_FORMAT_STRING_SIZE   505                               

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IConnect, ver. 0.0,
   GUID={0x03FD46F1,0x4FEE,0x4B0B,{0x8B,0x40,0x1E,0x94,0x36,0x52,0xCA,0x08}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IConnect_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static const unsigned short IConnect_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    28,
    50,
    78,
    106,
    140,
    162,
    190,
    218,
    252,
    280,
    308,
    336,
    364,
    392,
    420,
    448,
    476
    };

static const MIDL_SERVER_INFO IConnect_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IConnect_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IConnect_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IConnect_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(25) _IConnectProxyVtbl = 
{
    &IConnect_ProxyInfo,
    &IID_IConnect,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IConnect::Test */ ,
    (void *)-1 /* IConnect::Test2 */ ,
    (void *)-1 /* IConnect::Test3 */ ,
    (void *)-1 /* IConnect::put_Test4 */ ,
    (void *)-1 /* IConnect::put_Test5 */ ,
    (void *)-1 /* IConnect::Test6 */ ,
    (void *)-1 /* IConnect::Test7 */ ,
    (void *)-1 /* IConnect::EvalExpr */ ,
    (void *)-1 /* IConnect::EvalExpr2 */ ,
    (void *)-1 /* IConnect::get_Test8 */ ,
    (void *)-1 /* IConnect::put_Test8 */ ,
    (void *)-1 /* IConnect::get_Test9 */ ,
    (void *)-1 /* IConnect::put_Test9 */ ,
    (void *)-1 /* IConnect::get_Test10 */ ,
    (void *)-1 /* IConnect::put_Test10 */ ,
    (void *)-1 /* IConnect::get_Test11 */ ,
    (void *)-1 /* IConnect::get_Array */ ,
    (void *)-1 /* IConnect::put_Array */
};


static const PRPC_STUB_FUNCTION IConnect_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IConnectStubVtbl =
{
    &IID_IConnect,
    &IConnect_ServerInfo,
    25,
    &IConnect_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            },
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Test */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 10 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter a */

/* 16 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 22 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Test2 */

/* 28 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 36 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 38 */	NdrFcShort( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 44 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 46 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Test3 */

/* 50 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 52 */	NdrFcLong( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 58 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x8 ),	/* 8 */
/* 64 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter A */

/* 66 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 70 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 72 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 74 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Test4 */

/* 78 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 80 */	NdrFcLong( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 86 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 94 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 96 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 98 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 100 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 102 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Test5 */

/* 106 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 108 */	NdrFcLong( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 114 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter A */

/* 122 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 124 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 126 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Parameter newVal */

/* 128 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 130 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 132 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 136 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Test6 */

/* 140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 146 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 148 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 154 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 156 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 158 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Test7 */

/* 162 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 164 */	NdrFcLong( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 176 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter A */

/* 178 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 180 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 182 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 186 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EvalExpr */

/* 190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 198 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 204 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter Str */

/* 206 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 208 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 210 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 212 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 214 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 216 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EvalExpr2 */

/* 218 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 220 */	NdrFcLong( 0x0 ),	/* 0 */
/* 224 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 226 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x8 ),	/* 8 */
/* 232 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter Str */

/* 234 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 236 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 238 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Parameter Ret */

/* 240 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 242 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 244 */	NdrFcShort( 0x3dc ),	/* Type Offset=988 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 248 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Test8 */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 260 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 268 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
#ifndef _ALPHA_
/* 270 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 272 */	NdrFcShort( 0x3e6 ),	/* Type Offset=998 */

	/* Return value */

/* 274 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 276 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 278 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Test8 */

/* 280 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 282 */	NdrFcLong( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 288 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */
/* 292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 294 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 296 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 298 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 300 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 302 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 304 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 306 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Test9 */

/* 308 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 316 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x8 ),	/* 8 */
/* 322 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 324 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 326 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 328 */	NdrFcShort( 0x3f2 ),	/* Type Offset=1010 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 332 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Test9 */

/* 336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 344 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 350 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 352 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 354 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 356 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 358 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 360 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 362 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Test10 */

/* 364 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 372 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 374 */	NdrFcShort( 0x0 ),	/* 0 */
/* 376 */	NdrFcShort( 0x8 ),	/* 8 */
/* 378 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 380 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 382 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 384 */	NdrFcShort( 0x3dc ),	/* Type Offset=988 */

	/* Return value */

/* 386 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 388 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 390 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Test10 */

/* 392 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 394 */	NdrFcLong( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 400 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 404 */	NdrFcShort( 0x8 ),	/* 8 */
/* 406 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 408 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 410 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 412 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 414 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 416 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Test11 */

/* 420 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 422 */	NdrFcLong( 0x0 ),	/* 0 */
/* 426 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
/* 428 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0x8 ),	/* 8 */
/* 434 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 436 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
#ifndef _ALPHA_
/* 438 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 440 */	NdrFcShort( 0x404 ),	/* Type Offset=1028 */

	/* Return value */

/* 442 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 444 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 446 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Array */

/* 448 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 450 */	NdrFcLong( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 456 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 462 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 464 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 466 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 468 */	NdrFcShort( 0x3dc ),	/* Type Offset=988 */

	/* Return value */

/* 470 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 472 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Array */

/* 476 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 482 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 484 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x8 ),	/* 8 */
/* 490 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 492 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 494 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 496 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 500 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0x3a8 ),	/* Offset= 936 (940) */
/*  6 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/*  8 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 10 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x2b ),	/* 43 */
/* 18 */	NdrFcLong( 0x3 ),	/* 3 */
/* 22 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 24 */	NdrFcLong( 0x11 ),	/* 17 */
/* 28 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 30 */	NdrFcLong( 0x2 ),	/* 2 */
/* 34 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 36 */	NdrFcLong( 0x4 ),	/* 4 */
/* 40 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 42 */	NdrFcLong( 0x5 ),	/* 5 */
/* 46 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 48 */	NdrFcLong( 0xb ),	/* 11 */
/* 52 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 54 */	NdrFcLong( 0xa ),	/* 10 */
/* 58 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 60 */	NdrFcLong( 0x6 ),	/* 6 */
/* 64 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (278) */
/* 66 */	NdrFcLong( 0x7 ),	/* 7 */
/* 70 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 72 */	NdrFcLong( 0x8 ),	/* 8 */
/* 76 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (284) */
/* 78 */	NdrFcLong( 0xd ),	/* 13 */
/* 82 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (308) */
/* 84 */	NdrFcLong( 0x9 ),	/* 9 */
/* 88 */	NdrFcShort( 0xee ),	/* Offset= 238 (326) */
/* 90 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 94 */	NdrFcShort( 0xfa ),	/* Offset= 250 (344) */
/* 96 */	NdrFcLong( 0x24 ),	/* 36 */
/* 100 */	NdrFcShort( 0x304 ),	/* Offset= 772 (872) */
/* 102 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 106 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (872) */
/* 108 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 112 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (876) */
/* 114 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 118 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (880) */
/* 120 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 124 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (884) */
/* 126 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 130 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (888) */
/* 132 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 136 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (892) */
/* 138 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 142 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (880) */
/* 144 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 148 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (884) */
/* 150 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 154 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (896) */
/* 156 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 160 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (892) */
/* 162 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 166 */	NdrFcShort( 0x2de ),	/* Offset= 734 (900) */
/* 168 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 172 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (904) */
/* 174 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 178 */	NdrFcShort( 0x2da ),	/* Offset= 730 (908) */
/* 180 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 184 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (912) */
/* 186 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 190 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (916) */
/* 192 */	NdrFcLong( 0x10 ),	/* 16 */
/* 196 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 198 */	NdrFcLong( 0x12 ),	/* 18 */
/* 202 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 204 */	NdrFcLong( 0x13 ),	/* 19 */
/* 208 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 210 */	NdrFcLong( 0x16 ),	/* 22 */
/* 214 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 216 */	NdrFcLong( 0x17 ),	/* 23 */
/* 220 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 222 */	NdrFcLong( 0xe ),	/* 14 */
/* 226 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (924) */
/* 228 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 232 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (936) */
/* 234 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 238 */	NdrFcShort( 0x27e ),	/* Offset= 638 (876) */
/* 240 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 244 */	NdrFcShort( 0x27c ),	/* Offset= 636 (880) */
/* 246 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 250 */	NdrFcShort( 0x27a ),	/* Offset= 634 (884) */
/* 252 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 256 */	NdrFcShort( 0x274 ),	/* Offset= 628 (884) */
/* 258 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 262 */	NdrFcShort( 0x26e ),	/* Offset= 622 (884) */
/* 264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 268 */	NdrFcShort( 0x0 ),	/* Offset= 0 (268) */
/* 270 */	NdrFcLong( 0x1 ),	/* 1 */
/* 274 */	NdrFcShort( 0x0 ),	/* Offset= 0 (274) */
/* 276 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (275) */
/* 278 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 282 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 284 */	
			0x12, 0x0,	/* FC_UP */
/* 286 */	NdrFcShort( 0xc ),	/* Offset= 12 (298) */
/* 288 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 290 */	NdrFcShort( 0x2 ),	/* 2 */
/* 292 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 294 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 296 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 298 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 302 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (288) */
/* 304 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 306 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 308 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 320 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 322 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 324 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 326 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 328 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 336 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 338 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 340 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 342 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 344 */	
			0x12, 0x0,	/* FC_UP */
/* 346 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (854) */
/* 348 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 350 */	NdrFcShort( 0x18 ),	/* 24 */
/* 352 */	NdrFcShort( 0xa ),	/* 10 */
/* 354 */	NdrFcLong( 0x8 ),	/* 8 */
/* 358 */	NdrFcShort( 0x58 ),	/* Offset= 88 (446) */
/* 360 */	NdrFcLong( 0xd ),	/* 13 */
/* 364 */	NdrFcShort( 0x78 ),	/* Offset= 120 (484) */
/* 366 */	NdrFcLong( 0x9 ),	/* 9 */
/* 370 */	NdrFcShort( 0x94 ),	/* Offset= 148 (518) */
/* 372 */	NdrFcLong( 0xc ),	/* 12 */
/* 376 */	NdrFcShort( 0xbc ),	/* Offset= 188 (564) */
/* 378 */	NdrFcLong( 0x24 ),	/* 36 */
/* 382 */	NdrFcShort( 0x114 ),	/* Offset= 276 (658) */
/* 384 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 388 */	NdrFcShort( 0x130 ),	/* Offset= 304 (692) */
/* 390 */	NdrFcLong( 0x10 ),	/* 16 */
/* 394 */	NdrFcShort( 0x148 ),	/* Offset= 328 (722) */
/* 396 */	NdrFcLong( 0x2 ),	/* 2 */
/* 400 */	NdrFcShort( 0x160 ),	/* Offset= 352 (752) */
/* 402 */	NdrFcLong( 0x3 ),	/* 3 */
/* 406 */	NdrFcShort( 0x178 ),	/* Offset= 376 (782) */
/* 408 */	NdrFcLong( 0x14 ),	/* 20 */
/* 412 */	NdrFcShort( 0x190 ),	/* Offset= 400 (812) */
/* 414 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (413) */
/* 416 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 418 */	NdrFcShort( 0x4 ),	/* 4 */
/* 420 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 424 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 426 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 428 */	NdrFcShort( 0x4 ),	/* 4 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0x1 ),	/* 1 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	0x12, 0x0,	/* FC_UP */
/* 440 */	NdrFcShort( 0xffffff72 ),	/* Offset= -142 (298) */
/* 442 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 444 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 446 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 448 */	NdrFcShort( 0x8 ),	/* 8 */
/* 450 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 452 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 454 */	NdrFcShort( 0x4 ),	/* 4 */
/* 456 */	NdrFcShort( 0x4 ),	/* 4 */
/* 458 */	0x11, 0x0,	/* FC_RP */
/* 460 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (416) */
/* 462 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 464 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 466 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 478 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 480 */	NdrFcShort( 0xffffff54 ),	/* Offset= -172 (308) */
/* 482 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 484 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x6 ),	/* Offset= 6 (496) */
/* 492 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 494 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 496 */	
			0x11, 0x0,	/* FC_RP */
/* 498 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (466) */
/* 500 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 504 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 508 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 512 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 514 */	NdrFcShort( 0xffffff44 ),	/* Offset= -188 (326) */
/* 516 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 518 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 520 */	NdrFcShort( 0x8 ),	/* 8 */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x6 ),	/* Offset= 6 (530) */
/* 526 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 528 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 530 */	
			0x11, 0x0,	/* FC_RP */
/* 532 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (500) */
/* 534 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 536 */	NdrFcShort( 0x4 ),	/* 4 */
/* 538 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 544 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 546 */	NdrFcShort( 0x4 ),	/* 4 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x1 ),	/* 1 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x12, 0x0,	/* FC_UP */
/* 558 */	NdrFcShort( 0x17e ),	/* Offset= 382 (940) */
/* 560 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 562 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 564 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x6 ),	/* Offset= 6 (576) */
/* 572 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 574 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 576 */	
			0x11, 0x0,	/* FC_RP */
/* 578 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (534) */
/* 580 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 582 */	NdrFcLong( 0x2f ),	/* 47 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 592 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 594 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 596 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 598 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 600 */	NdrFcShort( 0x1 ),	/* 1 */
/* 602 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 604 */	NdrFcShort( 0x4 ),	/* 4 */
/* 606 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 608 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 610 */	NdrFcShort( 0x10 ),	/* 16 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0xa ),	/* Offset= 10 (624) */
/* 616 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 618 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 620 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (580) */
/* 622 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 624 */	
			0x12, 0x0,	/* FC_UP */
/* 626 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (598) */
/* 628 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 630 */	NdrFcShort( 0x4 ),	/* 4 */
/* 632 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 636 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 638 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 640 */	NdrFcShort( 0x4 ),	/* 4 */
/* 642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 644 */	NdrFcShort( 0x1 ),	/* 1 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	0x12, 0x0,	/* FC_UP */
/* 652 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (608) */
/* 654 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 656 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 658 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 660 */	NdrFcShort( 0x8 ),	/* 8 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x6 ),	/* Offset= 6 (670) */
/* 666 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 668 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 670 */	
			0x11, 0x0,	/* FC_RP */
/* 672 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (628) */
/* 674 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 676 */	NdrFcShort( 0x8 ),	/* 8 */
/* 678 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 680 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 682 */	NdrFcShort( 0x10 ),	/* 16 */
/* 684 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 686 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 688 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (674) */
			0x5b,		/* FC_END */
/* 692 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 694 */	NdrFcShort( 0x18 ),	/* 24 */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0xa ),	/* Offset= 10 (708) */
/* 700 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 702 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 704 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (680) */
/* 706 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 708 */	
			0x11, 0x0,	/* FC_RP */
/* 710 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (466) */
/* 712 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 714 */	NdrFcShort( 0x1 ),	/* 1 */
/* 716 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 722 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 724 */	NdrFcShort( 0x8 ),	/* 8 */
/* 726 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 728 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 730 */	NdrFcShort( 0x4 ),	/* 4 */
/* 732 */	NdrFcShort( 0x4 ),	/* 4 */
/* 734 */	0x12, 0x0,	/* FC_UP */
/* 736 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (712) */
/* 738 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 740 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 742 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 744 */	NdrFcShort( 0x2 ),	/* 2 */
/* 746 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 752 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 756 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 758 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 760 */	NdrFcShort( 0x4 ),	/* 4 */
/* 762 */	NdrFcShort( 0x4 ),	/* 4 */
/* 764 */	0x12, 0x0,	/* FC_UP */
/* 766 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (742) */
/* 768 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 770 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 772 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 774 */	NdrFcShort( 0x4 ),	/* 4 */
/* 776 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 782 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 786 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 788 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 790 */	NdrFcShort( 0x4 ),	/* 4 */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	0x12, 0x0,	/* FC_UP */
/* 796 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (772) */
/* 798 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 800 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 802 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 806 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 812 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 816 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 818 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	0x12, 0x0,	/* FC_UP */
/* 826 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (802) */
/* 828 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 830 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 832 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 836 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 838 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 840 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 844 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 846 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 848 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 850 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (832) */
/* 852 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 854 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 856 */	NdrFcShort( 0x28 ),	/* 40 */
/* 858 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (840) */
/* 860 */	NdrFcShort( 0x0 ),	/* Offset= 0 (860) */
/* 862 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 864 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 866 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 868 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (348) */
			0x5b,		/* FC_END */
/* 872 */	
			0x12, 0x0,	/* FC_UP */
/* 874 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (608) */
/* 876 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 878 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 880 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 882 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 884 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 886 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 888 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 890 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 892 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 894 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 896 */	
			0x12, 0x0,	/* FC_UP */
/* 898 */	NdrFcShort( 0xfffffd94 ),	/* Offset= -620 (278) */
/* 900 */	
			0x12, 0x10,	/* FC_UP */
/* 902 */	NdrFcShort( 0xfffffd96 ),	/* Offset= -618 (284) */
/* 904 */	
			0x12, 0x10,	/* FC_UP */
/* 906 */	NdrFcShort( 0xfffffdaa ),	/* Offset= -598 (308) */
/* 908 */	
			0x12, 0x10,	/* FC_UP */
/* 910 */	NdrFcShort( 0xfffffdb8 ),	/* Offset= -584 (326) */
/* 912 */	
			0x12, 0x10,	/* FC_UP */
/* 914 */	NdrFcShort( 0xfffffdc6 ),	/* Offset= -570 (344) */
/* 916 */	
			0x12, 0x10,	/* FC_UP */
/* 918 */	NdrFcShort( 0x2 ),	/* Offset= 2 (920) */
/* 920 */	
			0x12, 0x0,	/* FC_UP */
/* 922 */	NdrFcShort( 0xfffffc66 ),	/* Offset= -922 (0) */
/* 924 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 926 */	NdrFcShort( 0x10 ),	/* 16 */
/* 928 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 930 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 932 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 934 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 936 */	
			0x12, 0x0,	/* FC_UP */
/* 938 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (924) */
/* 940 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 942 */	NdrFcShort( 0x20 ),	/* 32 */
/* 944 */	NdrFcShort( 0x0 ),	/* 0 */
/* 946 */	NdrFcShort( 0x0 ),	/* Offset= 0 (946) */
/* 948 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 950 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 952 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 954 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 956 */	NdrFcShort( 0xfffffc4a ),	/* Offset= -950 (6) */
/* 958 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 960 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x10 ),	/* 16 */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0xfffffc3a ),	/* Offset= -966 (2) */
/* 970 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 972 */	NdrFcShort( 0x1 ),	/* 1 */
/* 974 */	NdrFcShort( 0x4 ),	/* 4 */
/* 976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0xfffffd4a ),	/* Offset= -694 (284) */
/* 980 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 982 */	NdrFcShort( 0x6 ),	/* Offset= 6 (988) */
/* 984 */	
			0x13, 0x0,	/* FC_OP */
/* 986 */	NdrFcShort( 0xffffffd2 ),	/* Offset= -46 (940) */
/* 988 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x10 ),	/* 16 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (984) */
/* 998 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 1000 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1002) */
/* 1002 */	
			0x13, 0x0,	/* FC_OP */
/* 1004 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1010) */
/* 1006 */	
			0x13, 0x0,	/* FC_OP */
/* 1008 */	NdrFcShort( 0xfffffd3a ),	/* Offset= -710 (298) */
/* 1010 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1012 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1014 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1016 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1018 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1006) */
/* 1020 */	
			0x11, 0x0,	/* FC_RP */
/* 1022 */	NdrFcShort( 0xffffffcc ),	/* Offset= -52 (970) */
/* 1024 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1026 */	NdrFcShort( 0xfffffff0 ),	/* Offset= -16 (1010) */
/* 1028 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 1030 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1032) */
/* 1032 */	
			0x13, 0x0,	/* FC_OP */
/* 1034 */	NdrFcShort( 0xffffffd2 ),	/* Offset= -46 (988) */

			0x0
        }
    };

const CInterfaceProxyVtbl * _ServerCOM_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IConnectProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ServerCOM_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IConnectStubVtbl,
    0
};

PCInterfaceName const _ServerCOM_InterfaceNamesList[] = 
{
    "IConnect",
    0
};

const IID *  _ServerCOM_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _ServerCOM_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ServerCOM, pIID, n)

int __stdcall _ServerCOM_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_ServerCOM_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo ServerCOM_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ServerCOM_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ServerCOM_StubVtblList,
    (const PCInterfaceName * ) & _ServerCOM_InterfaceNamesList,
    (const IID ** ) & _ServerCOM_BaseIIDList,
    & _ServerCOM_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
