/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Oct 02 19:54:30 2006
 */
/* Compiler settings for C:\Doc\C++\RWidjets\RWidjets.idl:
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


#include "RWidjets_i.h"

#define TYPE_FORMAT_STRING_SIZE   1121                              
#define PROC_FORMAT_STRING_SIZE   985                               

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


/* Object interface: IInitDone, ver. 0.0,
   GUID={0xAB634001,0xF13D,0x11d0,{0xA4,0x59,0x00,0x40,0x95,0xE1,0xDA,0xEA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IInitDone_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IInitDone_FormatStringOffsetTable[] = 
    {
    0,
    28,
    50
    };

static const MIDL_SERVER_INFO IInitDone_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IInitDone_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IInitDone_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IInitDone_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(6) _IInitDoneProxyVtbl = 
{
    &IInitDone_ProxyInfo,
    &IID_IInitDone,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IInitDone::Init */ ,
    (void *)-1 /* IInitDone::Done */ ,
    (void *)-1 /* IInitDone::GetInfo */
};

const CInterfaceStubVtbl _IInitDoneStubVtbl =
{
    &IID_IInitDone,
    &IInitDone_ServerInfo,
    6,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IPropertyBag, ver. 0.0,
   GUID={0x55272A00,0x42CB,0x11CE,{0x81,0x35,0x00,0xAA,0x00,0x4B,0xB8,0x51}} */


/* Object interface: IPropertyProfile, ver. 0.0,
   GUID={0xAB634002,0xF13D,0x11d0,{0xA4,0x59,0x00,0x40,0x95,0xE1,0xDA,0xEA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPropertyProfile_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IPropertyProfile_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    78
    };

static const MIDL_SERVER_INFO IPropertyProfile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPropertyProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPropertyProfile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPropertyProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(6) _IPropertyProfileProxyVtbl = 
{
    &IPropertyProfile_ProxyInfo,
    &IID_IPropertyProfile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IPropertyBag_Read_Proxy */ ,
    0 /* (void *)-1 /* IPropertyBag::Write */ ,
    (void *)-1 /* IPropertyProfile::RegisterProfileAs */
};


static const PRPC_STUB_FUNCTION IPropertyProfile_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IPropertyProfileStubVtbl =
{
    &IID_IPropertyProfile,
    &IPropertyProfile_ServerInfo,
    6,
    &IPropertyProfile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAsyncEvent, ver. 0.0,
   GUID={0xab634004,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAsyncEvent_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IAsyncEvent_FormatStringOffsetTable[] = 
    {
    106,
    134,
    162,
    202
    };

static const MIDL_SERVER_INFO IAsyncEvent_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAsyncEvent_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IAsyncEvent_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAsyncEvent_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(7) _IAsyncEventProxyVtbl = 
{
    &IAsyncEvent_ProxyInfo,
    &IID_IAsyncEvent,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IAsyncEvent::SetEventBufferDepth */ ,
    (void *)-1 /* IAsyncEvent::GetEventBufferDepth */ ,
    (void *)-1 /* IAsyncEvent::ExternalEvent */ ,
    (void *)-1 /* IAsyncEvent::CleanBuffer */
};

const CInterfaceStubVtbl _IAsyncEventStubVtbl =
{
    &IID_IAsyncEvent,
    &IAsyncEvent_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: ILanguageExtender, ver. 0.0,
   GUID={0xAB634003,0xF13D,0x11d0,{0xA4,0x59,0x00,0x40,0x95,0xE1,0xDA,0xEA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ILanguageExtender_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short ILanguageExtender_FormatStringOffsetTable[] = 
    {
    224,
    252,
    280,
    314,
    354,
    388,
    422,
    456,
    490,
    518,
    552,
    592,
    626,
    666,
    700,
    734
    };

static const MIDL_SERVER_INFO ILanguageExtender_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ILanguageExtender_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ILanguageExtender_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ILanguageExtender_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(19) _ILanguageExtenderProxyVtbl = 
{
    &ILanguageExtender_ProxyInfo,
    &IID_ILanguageExtender,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* ILanguageExtender::RegisterExtensionAs */ ,
    (void *)-1 /* ILanguageExtender::GetNProps */ ,
    (void *)-1 /* ILanguageExtender::FindProp */ ,
    (void *)-1 /* ILanguageExtender::GetPropName */ ,
    (void *)-1 /* ILanguageExtender::GetPropVal */ ,
    (void *)-1 /* ILanguageExtender::SetPropVal */ ,
    (void *)-1 /* ILanguageExtender::IsPropReadable */ ,
    (void *)-1 /* ILanguageExtender::IsPropWritable */ ,
    (void *)-1 /* ILanguageExtender::GetNMethods */ ,
    (void *)-1 /* ILanguageExtender::FindMethod */ ,
    (void *)-1 /* ILanguageExtender::GetMethodName */ ,
    (void *)-1 /* ILanguageExtender::GetNParams */ ,
    (void *)-1 /* ILanguageExtender::GetParamDefValue */ ,
    (void *)-1 /* ILanguageExtender::HasRetVal */ ,
    (void *)-1 /* ILanguageExtender::CallAsProc */ ,
    (void *)-1 /* ILanguageExtender::CallAsFunc */
};

const CInterfaceStubVtbl _ILanguageExtenderStubVtbl =
{
    &IID_ILanguageExtender,
    &ILanguageExtender_ServerInfo,
    19,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IStatusLine, ver. 0.0,
   GUID={0xab634005,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IStatusLine_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IStatusLine_FormatStringOffsetTable[] = 
    {
    774,
    28
    };

static const MIDL_SERVER_INFO IStatusLine_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IStatusLine_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IStatusLine_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IStatusLine_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(5) _IStatusLineProxyVtbl = 
{
    &IStatusLine_ProxyInfo,
    &IID_IStatusLine,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IStatusLine::SetStatusLine */ ,
    (void *)-1 /* IStatusLine::ResetStatusLine */
};

const CInterfaceStubVtbl _IStatusLineStubVtbl =
{
    &IID_IStatusLine,
    &IStatusLine_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IExtWndsSupport, ver. 0.0,
   GUID={0xefe19ea0,0x09e4,0x11d2,{0xa6,0x01,0x00,0x80,0x48,0xda,0x00,0xde}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IExtWndsSupport_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IExtWndsSupport_FormatStringOffsetTable[] = 
    {
    802,
    830,
    858
    };

static const MIDL_SERVER_INFO IExtWndsSupport_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IExtWndsSupport_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IExtWndsSupport_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IExtWndsSupport_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(6) _IExtWndsSupportProxyVtbl = 
{
    &IExtWndsSupport_ProxyInfo,
    &IID_IExtWndsSupport,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IExtWndsSupport::GetAppMainFrame */ ,
    (void *)-1 /* IExtWndsSupport::GetAppMDIFrame */ ,
    (void *)-1 /* IExtWndsSupport::CreateAddInWindow */
};

const CInterfaceStubVtbl _IExtWndsSupportStubVtbl =
{
    &IID_IExtWndsSupport,
    &IExtWndsSupport_ServerInfo,
    6,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IPropertyLink, ver. 0.0,
   GUID={0x52512A61,0x2A9D,0x11d1,{0xA4,0xD6,0x00,0x40,0x95,0xE1,0xDA,0xEA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPropertyLink_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IPropertyLink_FormatStringOffsetTable[] = 
    {
    928,
    956
    };

static const MIDL_SERVER_INFO IPropertyLink_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPropertyLink_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPropertyLink_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPropertyLink_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(5) _IPropertyLinkProxyVtbl = 
{
    &IPropertyLink_ProxyInfo,
    &IID_IPropertyLink,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IPropertyLink::get_Enabled */ ,
    (void *)-1 /* IPropertyLink::put_Enabled */
};

const CInterfaceStubVtbl _IPropertyLinkStubVtbl =
{
    &IID_IPropertyLink,
    &IPropertyLink_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IRAddIn, ver. 0.0,
   GUID={0x58E9679E,0xF2E6,0x11D5,{0xAE,0x41,0x00,0x02,0xB3,0x1F,0x65,0x32}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[4];

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

CINTERFACE_PROXY_VTABLE(3) _IRAddInProxyVtbl = 
{
    0,
    &IID_IRAddIn,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};

const CInterfaceStubVtbl _IRAddInStubVtbl =
{
    &IID_IRAddIn,
    0,
    3,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[4] = 
        {
            
            {
            LPSAFEARRAY_UserSize
            ,LPSAFEARRAY_UserMarshal
            ,LPSAFEARRAY_UserUnmarshal
            ,LPSAFEARRAY_UserFree
            },
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            },
            {
            HWND_UserSize
            ,HWND_UserMarshal
            ,HWND_UserUnmarshal
            ,HWND_UserFree
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

	/* Procedure Init */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pConnection */

/* 16 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 22 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ResetStatusLine */


	/* Procedure Done */

/* 28 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 30 */	NdrFcLong( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x4 ),	/* 4 */
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


	/* Return value */

/* 44 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 46 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetInfo */

/* 50 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 52 */	NdrFcLong( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x8 ),	/* 8 */
/* 64 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pInfo */

/* 66 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 68 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 70 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 72 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 74 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RegisterProfileAs */

/* 78 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 80 */	NdrFcLong( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 86 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrProfileName */

/* 94 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 96 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 98 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 100 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 102 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetEventBufferDepth */

/* 106 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 108 */	NdrFcLong( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 114 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 116 */	NdrFcShort( 0x8 ),	/* 8 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter lDepth */

/* 122 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 124 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 126 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 128 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 130 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEventBufferDepth */

/* 134 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 136 */	NdrFcLong( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 142 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 148 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter plDepth */

/* 150 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
/* 152 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 154 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 156 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 158 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ExternalEvent */

/* 162 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 164 */	NdrFcLong( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 176 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrSource */

/* 178 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 180 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 182 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrMessage */

/* 184 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 186 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 188 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrData */

/* 190 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 192 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 194 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 196 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 198 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CleanBuffer */

/* 202 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x6 ),	/* 6 */
#ifndef _ALPHA_
/* 210 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x8 ),	/* 8 */
/* 216 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 220 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RegisterExtensionAs */

/* 224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 232 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 238 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrExtensionName */

/* 240 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 242 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 244 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 248 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNProps */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 260 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 262 */	NdrFcShort( 0x8 ),	/* 8 */
/* 264 */	NdrFcShort( 0x10 ),	/* 16 */
/* 266 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter plProps */

/* 268 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 270 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 272 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 274 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 276 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 278 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FindProp */

/* 280 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 282 */	NdrFcLong( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 288 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 292 */	NdrFcShort( 0x10 ),	/* 16 */
/* 294 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrPropName */

/* 296 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 298 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 300 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter plPropNum */

/* 302 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 304 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 306 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 310 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPropName */

/* 314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x6 ),	/* 6 */
#ifndef _ALPHA_
/* 322 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 324 */	NdrFcShort( 0x10 ),	/* 16 */
/* 326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 328 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter lPropNum */

/* 330 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 332 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lPropAlias */

/* 336 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 338 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbstrPropName */

/* 342 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 344 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 346 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 348 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 350 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 352 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPropVal */

/* 354 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 356 */	NdrFcLong( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 362 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 364 */	NdrFcShort( 0x8 ),	/* 8 */
/* 366 */	NdrFcShort( 0x8 ),	/* 8 */
/* 368 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lPropNum */

/* 370 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 372 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 374 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pvarPropVal */

/* 376 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 378 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 380 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 384 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPropVal */

/* 388 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 396 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 400 */	NdrFcShort( 0x8 ),	/* 8 */
/* 402 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lPropNum */

/* 404 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 406 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varPropVal */

/* 410 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 412 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 414 */	NdrFcShort( 0x408 ),	/* Type Offset=1032 */

	/* Return value */

/* 416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 418 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsPropReadable */

/* 422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 430 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 432 */	NdrFcShort( 0x10 ),	/* 16 */
/* 434 */	NdrFcShort( 0x10 ),	/* 16 */
/* 436 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPropNum */

/* 438 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 440 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pboolPropRead */

/* 444 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 446 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 452 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsPropWritable */

/* 456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 464 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 466 */	NdrFcShort( 0x10 ),	/* 16 */
/* 468 */	NdrFcShort( 0x10 ),	/* 16 */
/* 470 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPropNum */

/* 472 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 474 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pboolPropWrite */

/* 478 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 480 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 482 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 484 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 486 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 488 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNMethods */

/* 490 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 492 */	NdrFcLong( 0x0 ),	/* 0 */
/* 496 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 498 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 502 */	NdrFcShort( 0x10 ),	/* 16 */
/* 504 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter plMethods */

/* 506 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 508 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 512 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 514 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 516 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FindMethod */

/* 518 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 520 */	NdrFcLong( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 526 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 528 */	NdrFcShort( 0x8 ),	/* 8 */
/* 530 */	NdrFcShort( 0x10 ),	/* 16 */
/* 532 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrMethodName */

/* 534 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 536 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 538 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter plMethodNum */

/* 540 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 542 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 546 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 548 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMethodName */

/* 552 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 560 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 562 */	NdrFcShort( 0x10 ),	/* 16 */
/* 564 */	NdrFcShort( 0x8 ),	/* 8 */
/* 566 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter lMethodNum */

/* 568 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 570 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 572 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lMethodAlias */

/* 574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 576 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 578 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbstrMethodName */

/* 580 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 582 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 584 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 586 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 588 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNParams */

/* 592 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 594 */	NdrFcLong( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 600 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 602 */	NdrFcShort( 0x10 ),	/* 16 */
/* 604 */	NdrFcShort( 0x10 ),	/* 16 */
/* 606 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lMethodNum */

/* 608 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 610 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plParams */

/* 614 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 616 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 622 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetParamDefValue */

/* 626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 634 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 636 */	NdrFcShort( 0x10 ),	/* 16 */
/* 638 */	NdrFcShort( 0x8 ),	/* 8 */
/* 640 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter lMethodNum */

/* 642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 644 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lParamNum */

/* 648 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 650 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pvarParamDefValue */

/* 654 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 656 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 658 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 662 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HasRetVal */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 674 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 676 */	NdrFcShort( 0x10 ),	/* 16 */
/* 678 */	NdrFcShort( 0x10 ),	/* 16 */
/* 680 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lMethodNum */

/* 682 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 684 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pboolRetValue */

/* 688 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
#ifndef _ALPHA_
/* 690 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 692 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 694 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 696 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 698 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CallAsProc */

/* 700 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 702 */	NdrFcLong( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 708 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 710 */	NdrFcShort( 0x8 ),	/* 8 */
/* 712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 714 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lMethodNum */

/* 716 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 718 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter paParams */

/* 722 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 724 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 726 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Return value */

/* 728 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 730 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 732 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CallAsFunc */

/* 734 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 736 */	NdrFcLong( 0x0 ),	/* 0 */
/* 740 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 742 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 744 */	NdrFcShort( 0x8 ),	/* 8 */
/* 746 */	NdrFcShort( 0x8 ),	/* 8 */
/* 748 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter lMethodNum */

/* 750 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 752 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pvarRetValue */

/* 756 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 758 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 760 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter paParams */

/* 762 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 764 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 766 */	NdrFcShort( 0x41e ),	/* Type Offset=1054 */

	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 770 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetStatusLine */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 782 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrStatusLine */

/* 790 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 792 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 794 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 796 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 798 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAppMainFrame */

/* 802 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 804 */	NdrFcLong( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 810 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 816 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter hwnd */

/* 818 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 820 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 822 */	NdrFcShort( 0x444 ),	/* Type Offset=1092 */

	/* Return value */

/* 824 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 826 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAppMDIFrame */

/* 830 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 832 */	NdrFcLong( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 838 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 844 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter hwnd */

/* 846 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 848 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 850 */	NdrFcShort( 0x444 ),	/* Type Offset=1092 */

	/* Return value */

/* 852 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 854 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateAddInWindow */

/* 858 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 860 */	NdrFcLong( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 866 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 868 */	NdrFcShort( 0x30 ),	/* 48 */
/* 870 */	NdrFcShort( 0x8 ),	/* 8 */
/* 872 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x9,		/* 9 */

	/* Parameter bstrProgID */

/* 874 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 876 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 878 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrWindowName */

/* 880 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 882 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 884 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter dwStyles */

/* 886 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 888 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 890 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dwExStyles */

/* 892 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 894 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 896 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter rctl */

/* 898 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
#ifndef _ALPHA_
/* 900 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 902 */	NdrFcShort( 0x452 ),	/* Type Offset=1106 */

	/* Parameter Flags */

/* 904 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 906 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 908 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pHwnd */

/* 910 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
#ifndef _ALPHA_
/* 912 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 914 */	NdrFcShort( 0x444 ),	/* Type Offset=1092 */

	/* Parameter pDisp */

/* 916 */	NdrFcShort( 0x201b ),	/* Flags:  must size, must free, in, out, srv alloc size=8 */
#ifndef _ALPHA_
/* 918 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 920 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Return value */

/* 922 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 924 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 926 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */

/* 928 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 930 */	NdrFcLong( 0x0 ),	/* 0 */
/* 934 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 936 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 940 */	NdrFcShort( 0x8 ),	/* 8 */
/* 942 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter boolEnabled */

/* 944 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
/* 946 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 950 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 952 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */

/* 956 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 958 */	NdrFcLong( 0x0 ),	/* 0 */
/* 962 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 964 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 968 */	NdrFcShort( 0x8 ),	/* 8 */
/* 970 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter boolEnabled */

/* 972 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 974 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 978 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 980 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 982 */	0x8,		/* FC_LONG */
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
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0x20400 ),	/* 132096 */
/*  8 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 14 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 16 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 18 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 20 */	
			0x11, 0x0,	/* FC_RP */
/* 22 */	NdrFcShort( 0x3aa ),	/* Offset= 938 (960) */
/* 24 */	
			0x13, 0x10,	/* FC_OP */
/* 26 */	NdrFcShort( 0x2 ),	/* Offset= 2 (28) */
/* 28 */	
			0x13, 0x0,	/* FC_OP */
/* 30 */	NdrFcShort( 0x390 ),	/* Offset= 912 (942) */
/* 32 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 34 */	NdrFcShort( 0x18 ),	/* 24 */
/* 36 */	NdrFcShort( 0xa ),	/* 10 */
/* 38 */	NdrFcLong( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0x6c ),	/* Offset= 108 (150) */
/* 44 */	NdrFcLong( 0xd ),	/* 13 */
/* 48 */	NdrFcShort( 0x9e ),	/* Offset= 158 (206) */
/* 50 */	NdrFcLong( 0x9 ),	/* 9 */
/* 54 */	NdrFcShort( 0xba ),	/* Offset= 186 (240) */
/* 56 */	NdrFcLong( 0xc ),	/* 12 */
/* 60 */	NdrFcShort( 0x280 ),	/* Offset= 640 (700) */
/* 62 */	NdrFcLong( 0x24 ),	/* 36 */
/* 66 */	NdrFcShort( 0x2a8 ),	/* Offset= 680 (746) */
/* 68 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 72 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (780) */
/* 74 */	NdrFcLong( 0x10 ),	/* 16 */
/* 78 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (810) */
/* 80 */	NdrFcLong( 0x2 ),	/* 2 */
/* 84 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (840) */
/* 86 */	NdrFcLong( 0x3 ),	/* 3 */
/* 90 */	NdrFcShort( 0x30c ),	/* Offset= 780 (870) */
/* 92 */	NdrFcLong( 0x14 ),	/* 20 */
/* 96 */	NdrFcShort( 0x324 ),	/* Offset= 804 (900) */
/* 98 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (97) */
/* 100 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 102 */	NdrFcShort( 0x2 ),	/* 2 */
/* 104 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 106 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 108 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 110 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 112 */	NdrFcShort( 0x8 ),	/* 8 */
/* 114 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (100) */
/* 116 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 118 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 120 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 122 */	NdrFcShort( 0x4 ),	/* 4 */
/* 124 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 130 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 132 */	NdrFcShort( 0x4 ),	/* 4 */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x1 ),	/* 1 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 142 */	0x13, 0x0,	/* FC_OP */
/* 144 */	NdrFcShort( 0xffffffde ),	/* Offset= -34 (110) */
/* 146 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 148 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 150 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 154 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 156 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 158 */	NdrFcShort( 0x4 ),	/* 4 */
/* 160 */	NdrFcShort( 0x4 ),	/* 4 */
/* 162 */	0x11, 0x0,	/* FC_RP */
/* 164 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (120) */
/* 166 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 168 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 170 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 180 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 182 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 184 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 186 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 188 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 200 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 202 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (170) */
/* 204 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 206 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 208 */	NdrFcShort( 0x8 ),	/* 8 */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x6 ),	/* Offset= 6 (218) */
/* 214 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 216 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 218 */	
			0x11, 0x0,	/* FC_RP */
/* 220 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (188) */
/* 222 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 226 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 230 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 234 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 236 */	NdrFcShort( 0xffffff16 ),	/* Offset= -234 (2) */
/* 238 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 240 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 242 */	NdrFcShort( 0x8 ),	/* 8 */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x6 ),	/* Offset= 6 (252) */
/* 248 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 250 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 252 */	
			0x11, 0x0,	/* FC_RP */
/* 254 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (222) */
/* 256 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 258 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 260 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 262 */	NdrFcShort( 0x2 ),	/* Offset= 2 (264) */
/* 264 */	NdrFcShort( 0x10 ),	/* 16 */
/* 266 */	NdrFcShort( 0x2b ),	/* 43 */
/* 268 */	NdrFcLong( 0x3 ),	/* 3 */
/* 272 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 274 */	NdrFcLong( 0x11 ),	/* 17 */
/* 278 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 280 */	NdrFcLong( 0x2 ),	/* 2 */
/* 284 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 286 */	NdrFcLong( 0x4 ),	/* 4 */
/* 290 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 292 */	NdrFcLong( 0x5 ),	/* 5 */
/* 296 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 298 */	NdrFcLong( 0xb ),	/* 11 */
/* 302 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 304 */	NdrFcLong( 0xa ),	/* 10 */
/* 308 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 310 */	NdrFcLong( 0x6 ),	/* 6 */
/* 314 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (528) */
/* 316 */	NdrFcLong( 0x7 ),	/* 7 */
/* 320 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 322 */	NdrFcLong( 0x8 ),	/* 8 */
/* 326 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (534) */
/* 328 */	NdrFcLong( 0xd ),	/* 13 */
/* 332 */	NdrFcShort( 0xffffff5e ),	/* Offset= -162 (170) */
/* 334 */	NdrFcLong( 0x9 ),	/* 9 */
/* 338 */	NdrFcShort( 0xfffffeb0 ),	/* Offset= -336 (2) */
/* 340 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 344 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (538) */
/* 346 */	NdrFcLong( 0x24 ),	/* 36 */
/* 350 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (542) */
/* 352 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 356 */	NdrFcShort( 0xba ),	/* Offset= 186 (542) */
/* 358 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 362 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (594) */
/* 364 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 368 */	NdrFcShort( 0xe6 ),	/* Offset= 230 (598) */
/* 370 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 374 */	NdrFcShort( 0xe4 ),	/* Offset= 228 (602) */
/* 376 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 380 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (606) */
/* 382 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 386 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (610) */
/* 388 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 392 */	NdrFcShort( 0xce ),	/* Offset= 206 (598) */
/* 394 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 398 */	NdrFcShort( 0xcc ),	/* Offset= 204 (602) */
/* 400 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 404 */	NdrFcShort( 0xd2 ),	/* Offset= 210 (614) */
/* 406 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 410 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (610) */
/* 412 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 416 */	NdrFcShort( 0xca ),	/* Offset= 202 (618) */
/* 418 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 422 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (622) */
/* 424 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 428 */	NdrFcShort( 0xc6 ),	/* Offset= 198 (626) */
/* 430 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 434 */	NdrFcShort( 0xc4 ),	/* Offset= 196 (630) */
/* 436 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 440 */	NdrFcShort( 0xbe ),	/* Offset= 190 (630) */
/* 442 */	NdrFcLong( 0x10 ),	/* 16 */
/* 446 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 448 */	NdrFcLong( 0x12 ),	/* 18 */
/* 452 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 454 */	NdrFcLong( 0x13 ),	/* 19 */
/* 458 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 460 */	NdrFcLong( 0x16 ),	/* 22 */
/* 464 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 466 */	NdrFcLong( 0x17 ),	/* 23 */
/* 470 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 472 */	NdrFcLong( 0xe ),	/* 14 */
/* 476 */	NdrFcShort( 0x9e ),	/* Offset= 158 (634) */
/* 478 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 482 */	NdrFcShort( 0xa4 ),	/* Offset= 164 (646) */
/* 484 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 488 */	NdrFcShort( 0x6a ),	/* Offset= 106 (594) */
/* 490 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 494 */	NdrFcShort( 0x68 ),	/* Offset= 104 (598) */
/* 496 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 500 */	NdrFcShort( 0x66 ),	/* Offset= 102 (602) */
/* 502 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 506 */	NdrFcShort( 0x60 ),	/* Offset= 96 (602) */
/* 508 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 512 */	NdrFcShort( 0x5a ),	/* Offset= 90 (602) */
/* 514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x0 ),	/* Offset= 0 (518) */
/* 520 */	NdrFcLong( 0x1 ),	/* 1 */
/* 524 */	NdrFcShort( 0x0 ),	/* Offset= 0 (524) */
/* 526 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (525) */
/* 528 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 530 */	NdrFcShort( 0x8 ),	/* 8 */
/* 532 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 534 */	
			0x13, 0x0,	/* FC_OP */
/* 536 */	NdrFcShort( 0xfffffe56 ),	/* Offset= -426 (110) */
/* 538 */	
			0x13, 0x0,	/* FC_OP */
/* 540 */	NdrFcShort( 0x192 ),	/* Offset= 402 (942) */
/* 542 */	
			0x13, 0x0,	/* FC_OP */
/* 544 */	NdrFcShort( 0x1e ),	/* Offset= 30 (574) */
/* 546 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 548 */	NdrFcLong( 0x2f ),	/* 47 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 558 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 560 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 562 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 564 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 566 */	NdrFcShort( 0x1 ),	/* 1 */
/* 568 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 570 */	NdrFcShort( 0x4 ),	/* 4 */
/* 572 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 574 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 576 */	NdrFcShort( 0x10 ),	/* 16 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0xa ),	/* Offset= 10 (590) */
/* 582 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 584 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 586 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (546) */
/* 588 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 590 */	
			0x13, 0x0,	/* FC_OP */
/* 592 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (564) */
/* 594 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 596 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 598 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 600 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 602 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 604 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 606 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 608 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 610 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 612 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 614 */	
			0x13, 0x0,	/* FC_OP */
/* 616 */	NdrFcShort( 0xffffffa8 ),	/* Offset= -88 (528) */
/* 618 */	
			0x13, 0x10,	/* FC_OP */
/* 620 */	NdrFcShort( 0xffffffaa ),	/* Offset= -86 (534) */
/* 622 */	
			0x13, 0x10,	/* FC_OP */
/* 624 */	NdrFcShort( 0xfffffe3a ),	/* Offset= -454 (170) */
/* 626 */	
			0x13, 0x10,	/* FC_OP */
/* 628 */	NdrFcShort( 0xfffffd8e ),	/* Offset= -626 (2) */
/* 630 */	
			0x13, 0x10,	/* FC_OP */
/* 632 */	NdrFcShort( 0xffffffa2 ),	/* Offset= -94 (538) */
/* 634 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 636 */	NdrFcShort( 0x10 ),	/* 16 */
/* 638 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 640 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 642 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 644 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 646 */	
			0x13, 0x0,	/* FC_OP */
/* 648 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (634) */
/* 650 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 652 */	NdrFcShort( 0x20 ),	/* 32 */
/* 654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 656 */	NdrFcShort( 0x0 ),	/* Offset= 0 (656) */
/* 658 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 660 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 662 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 664 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 666 */	NdrFcShort( 0xfffffe66 ),	/* Offset= -410 (256) */
/* 668 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 670 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 672 */	NdrFcShort( 0x4 ),	/* 4 */
/* 674 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 680 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 682 */	NdrFcShort( 0x4 ),	/* 4 */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x1 ),	/* 1 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	0x13, 0x0,	/* FC_OP */
/* 694 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (650) */
/* 696 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 698 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 700 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x6 ),	/* Offset= 6 (712) */
/* 708 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 710 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 712 */	
			0x11, 0x0,	/* FC_RP */
/* 714 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (670) */
/* 716 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 718 */	NdrFcShort( 0x4 ),	/* 4 */
/* 720 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 726 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 728 */	NdrFcShort( 0x4 ),	/* 4 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x1 ),	/* 1 */
/* 734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 738 */	0x13, 0x0,	/* FC_OP */
/* 740 */	NdrFcShort( 0xffffff5a ),	/* Offset= -166 (574) */
/* 742 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 744 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 746 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 748 */	NdrFcShort( 0x8 ),	/* 8 */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0x6 ),	/* Offset= 6 (758) */
/* 754 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 756 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 758 */	
			0x11, 0x0,	/* FC_RP */
/* 760 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (716) */
/* 762 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 764 */	NdrFcShort( 0x8 ),	/* 8 */
/* 766 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 768 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 770 */	NdrFcShort( 0x10 ),	/* 16 */
/* 772 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 774 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 776 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (762) */
			0x5b,		/* FC_END */
/* 780 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 782 */	NdrFcShort( 0x18 ),	/* 24 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0xa ),	/* Offset= 10 (796) */
/* 788 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 790 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 792 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (768) */
/* 794 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 796 */	
			0x11, 0x0,	/* FC_RP */
/* 798 */	NdrFcShort( 0xfffffd9e ),	/* Offset= -610 (188) */
/* 800 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 802 */	NdrFcShort( 0x1 ),	/* 1 */
/* 804 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 810 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 812 */	NdrFcShort( 0x8 ),	/* 8 */
/* 814 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 816 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 818 */	NdrFcShort( 0x4 ),	/* 4 */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	0x13, 0x0,	/* FC_OP */
/* 824 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (800) */
/* 826 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 828 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 830 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 832 */	NdrFcShort( 0x2 ),	/* 2 */
/* 834 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 840 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 844 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 846 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 848 */	NdrFcShort( 0x4 ),	/* 4 */
/* 850 */	NdrFcShort( 0x4 ),	/* 4 */
/* 852 */	0x13, 0x0,	/* FC_OP */
/* 854 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (830) */
/* 856 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 858 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 860 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 862 */	NdrFcShort( 0x4 ),	/* 4 */
/* 864 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 870 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 874 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 876 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 878 */	NdrFcShort( 0x4 ),	/* 4 */
/* 880 */	NdrFcShort( 0x4 ),	/* 4 */
/* 882 */	0x13, 0x0,	/* FC_OP */
/* 884 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (860) */
/* 886 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 888 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 890 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 894 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 898 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 900 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 904 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 906 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 908 */	NdrFcShort( 0x4 ),	/* 4 */
/* 910 */	NdrFcShort( 0x4 ),	/* 4 */
/* 912 */	0x13, 0x0,	/* FC_OP */
/* 914 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (890) */
/* 916 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 918 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 920 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 922 */	NdrFcShort( 0x8 ),	/* 8 */
/* 924 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 926 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 928 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 932 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 934 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 936 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 938 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (920) */
/* 940 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 942 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 944 */	NdrFcShort( 0x28 ),	/* 40 */
/* 946 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (928) */
/* 948 */	NdrFcShort( 0x0 ),	/* Offset= 0 (948) */
/* 950 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 952 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 954 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 956 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffc63 ),	/* Offset= -925 (32) */
			0x5b,		/* FC_END */
/* 960 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x4 ),	/* 4 */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0xfffffc50 ),	/* Offset= -944 (24) */
/* 970 */	
			0x12, 0x0,	/* FC_UP */
/* 972 */	NdrFcShort( 0xfffffca2 ),	/* Offset= -862 (110) */
/* 974 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 976 */	NdrFcShort( 0x1 ),	/* 1 */
/* 978 */	NdrFcShort( 0x4 ),	/* 4 */
/* 980 */	NdrFcShort( 0x0 ),	/* 0 */
/* 982 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (970) */
/* 984 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 986 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 988 */	
			0x11, 0x0,	/* FC_RP */
/* 990 */	NdrFcShort( 0x6 ),	/* Offset= 6 (996) */
/* 992 */	
			0x13, 0x0,	/* FC_OP */
/* 994 */	NdrFcShort( 0xfffffc8c ),	/* Offset= -884 (110) */
/* 996 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 998 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1000 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (992) */
/* 1006 */	
			0x11, 0x0,	/* FC_RP */
/* 1008 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1014) */
/* 1010 */	
			0x13, 0x0,	/* FC_OP */
/* 1012 */	NdrFcShort( 0xfffffe96 ),	/* Offset= -362 (650) */
/* 1014 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1016 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1018 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1010) */
/* 1024 */	
			0x11, 0x0,	/* FC_RP */
/* 1026 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1032) */
/* 1028 */	
			0x12, 0x0,	/* FC_UP */
/* 1030 */	NdrFcShort( 0xfffffe84 ),	/* Offset= -380 (650) */
/* 1032 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1034 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1036 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1040 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1028) */
/* 1042 */	
			0x11, 0x0,	/* FC_RP */
/* 1044 */	NdrFcShort( 0xa ),	/* Offset= 10 (1054) */
/* 1046 */	
			0x12, 0x10,	/* FC_UP */
/* 1048 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1050) */
/* 1050 */	
			0x12, 0x0,	/* FC_UP */
/* 1052 */	NdrFcShort( 0xffffff92 ),	/* Offset= -110 (942) */
/* 1054 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1056 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1058 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1062 */	NdrFcShort( 0xfffffff0 ),	/* Offset= -16 (1046) */
/* 1064 */	
			0x11, 0x0,	/* FC_RP */
/* 1066 */	NdrFcShort( 0x1a ),	/* Offset= 26 (1092) */
/* 1068 */	
			0x13, 0x0,	/* FC_OP */
/* 1070 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1072) */
/* 1072 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x48,		/* 72 */
/* 1074 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1076 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1078 */	NdrFcLong( 0x48746457 ),	/* 1215587415 */
/* 1082 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 1084 */	NdrFcLong( 0x52746457 ),	/* 1383359575 */
/* 1088 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 1090 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (1089) */
/* 1092 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1094 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1096 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1100 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (1068) */
/* 1102 */	
			0x11, 0x0,	/* FC_RP */
/* 1104 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1106) */
/* 1106 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1108 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1110 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1112 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1114 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1116 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 1118 */	NdrFcShort( 0xfffffba4 ),	/* Offset= -1116 (2) */

			0x0
        }
    };

const CInterfaceProxyVtbl * _RWidjets_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IInitDoneProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPropertyProfileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ILanguageExtenderProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAsyncEventProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IStatusLineProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPropertyLinkProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IRAddInProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IExtWndsSupportProxyVtbl,
    0
};

const CInterfaceStubVtbl * _RWidjets_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IInitDoneStubVtbl,
    ( CInterfaceStubVtbl *) &_IPropertyProfileStubVtbl,
    ( CInterfaceStubVtbl *) &_ILanguageExtenderStubVtbl,
    ( CInterfaceStubVtbl *) &_IAsyncEventStubVtbl,
    ( CInterfaceStubVtbl *) &_IStatusLineStubVtbl,
    ( CInterfaceStubVtbl *) &_IPropertyLinkStubVtbl,
    ( CInterfaceStubVtbl *) &_IRAddInStubVtbl,
    ( CInterfaceStubVtbl *) &_IExtWndsSupportStubVtbl,
    0
};

PCInterfaceName const _RWidjets_InterfaceNamesList[] = 
{
    "IInitDone",
    "IPropertyProfile",
    "ILanguageExtender",
    "IAsyncEvent",
    "IStatusLine",
    "IPropertyLink",
    "IRAddIn",
    "IExtWndsSupport",
    0
};

const IID *  _RWidjets_BaseIIDList[] = 
{
    0,
    &IID_IPropertyBag,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


#define _RWidjets_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _RWidjets, pIID, n)

int __stdcall _RWidjets_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _RWidjets, 8, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _RWidjets, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _RWidjets, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _RWidjets, 8, *pIndex )
    
}

const ExtendedProxyFileInfo RWidjets_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _RWidjets_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _RWidjets_StubVtblList,
    (const PCInterfaceName * ) & _RWidjets_InterfaceNamesList,
    (const IID ** ) & _RWidjets_BaseIIDList,
    & _RWidjets_IID_Lookup, 
    8,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
