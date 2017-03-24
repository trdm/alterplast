/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Nov 21 12:49:43 2008
 */
/* Compiler settings for T:\Programming\Visual 1C++\Vis1cppOle_VC6\Vis1cppOle\Vis1cpp.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Vis1cpp_h__
#define __Vis1cpp_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVis1cppPlugin_FWD_DEFINED__
#define __IVis1cppPlugin_FWD_DEFINED__
typedef interface IVis1cppPlugin IVis1cppPlugin;
#endif 	/* __IVis1cppPlugin_FWD_DEFINED__ */


#ifndef ___IVis1cppPluginEvents_FWD_DEFINED__
#define ___IVis1cppPluginEvents_FWD_DEFINED__
typedef interface _IVis1cppPluginEvents _IVis1cppPluginEvents;
#endif 	/* ___IVis1cppPluginEvents_FWD_DEFINED__ */


#ifndef __IClassInfo_FWD_DEFINED__
#define __IClassInfo_FWD_DEFINED__
typedef interface IClassInfo IClassInfo;
#endif 	/* __IClassInfo_FWD_DEFINED__ */


#ifndef __IClasses_FWD_DEFINED__
#define __IClasses_FWD_DEFINED__
typedef interface IClasses IClasses;
#endif 	/* __IClasses_FWD_DEFINED__ */


#ifndef __IDefFile_FWD_DEFINED__
#define __IDefFile_FWD_DEFINED__
typedef interface IDefFile IDefFile;
#endif 	/* __IDefFile_FWD_DEFINED__ */


#ifndef __IDefFiles_FWD_DEFINED__
#define __IDefFiles_FWD_DEFINED__
typedef interface IDefFiles IDefFiles;
#endif 	/* __IDefFiles_FWD_DEFINED__ */


#ifndef __Vis1cppPlugin_FWD_DEFINED__
#define __Vis1cppPlugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class Vis1cppPlugin Vis1cppPlugin;
#else
typedef struct Vis1cppPlugin Vis1cppPlugin;
#endif /* __cplusplus */

#endif 	/* __Vis1cppPlugin_FWD_DEFINED__ */


#ifndef __CoClassInfo_FWD_DEFINED__
#define __CoClassInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoClassInfo CoClassInfo;
#else
typedef struct CoClassInfo CoClassInfo;
#endif /* __cplusplus */

#endif 	/* __CoClassInfo_FWD_DEFINED__ */


#ifndef __CoClasses_FWD_DEFINED__
#define __CoClasses_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoClasses CoClasses;
#else
typedef struct CoClasses CoClasses;
#endif /* __cplusplus */

#endif 	/* __CoClasses_FWD_DEFINED__ */


#ifndef __CoDefFiles_FWD_DEFINED__
#define __CoDefFiles_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDefFiles CoDefFiles;
#else
typedef struct CoDefFiles CoDefFiles;
#endif /* __cplusplus */

#endif 	/* __CoDefFiles_FWD_DEFINED__ */


#ifndef __CoDefFile_FWD_DEFINED__
#define __CoDefFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDefFile CoDefFile;
#else
typedef struct CoDefFile CoDefFile;
#endif /* __cplusplus */

#endif 	/* __CoDefFile_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_Vis1cpp_0000 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_Vis1cpp_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Vis1cpp_0000_v0_0_s_ifspec;

#ifndef __IVis1cppPlugin_INTERFACE_DEFINED__
#define __IVis1cppPlugin_INTERFACE_DEFINED__

/* interface IVis1cppPlugin */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVis1cppPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("169A4E00-9085-46D2-90CE-D3BE8E45BF2E")
    IVis1cppPlugin : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Method1( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FlagGroupByType( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FlagGroupByType( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RefreshAll( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetClasses( 
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *Classes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDefFiles( 
            /* [retval][out] */ IDefFiles __RPC_FAR *__RPC_FAR *DefFiles) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVis1cppPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVis1cppPlugin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVis1cppPlugin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Method1 )( 
            IVis1cppPlugin __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FlagGroupByType )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FlagGroupByType )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshAll )( 
            IVis1cppPlugin __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClasses )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *Classes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefFiles )( 
            IVis1cppPlugin __RPC_FAR * This,
            /* [retval][out] */ IDefFiles __RPC_FAR *__RPC_FAR *DefFiles);
        
        END_INTERFACE
    } IVis1cppPluginVtbl;

    interface IVis1cppPlugin
    {
        CONST_VTBL struct IVis1cppPluginVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVis1cppPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVis1cppPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVis1cppPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVis1cppPlugin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVis1cppPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVis1cppPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVis1cppPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVis1cppPlugin_Method1(This)	\
    (This)->lpVtbl -> Method1(This)

#define IVis1cppPlugin_get_FlagGroupByType(This,pVal)	\
    (This)->lpVtbl -> get_FlagGroupByType(This,pVal)

#define IVis1cppPlugin_put_FlagGroupByType(This,newVal)	\
    (This)->lpVtbl -> put_FlagGroupByType(This,newVal)

#define IVis1cppPlugin_RefreshAll(This)	\
    (This)->lpVtbl -> RefreshAll(This)

#define IVis1cppPlugin_GetClasses(This,Classes)	\
    (This)->lpVtbl -> GetClasses(This,Classes)

#define IVis1cppPlugin_GetDefFiles(This,DefFiles)	\
    (This)->lpVtbl -> GetDefFiles(This,DefFiles)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_Method1_Proxy( 
    IVis1cppPlugin __RPC_FAR * This);


void __RPC_STUB IVis1cppPlugin_Method1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_get_FlagGroupByType_Proxy( 
    IVis1cppPlugin __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IVis1cppPlugin_get_FlagGroupByType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_put_FlagGroupByType_Proxy( 
    IVis1cppPlugin __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IVis1cppPlugin_put_FlagGroupByType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_RefreshAll_Proxy( 
    IVis1cppPlugin __RPC_FAR * This);


void __RPC_STUB IVis1cppPlugin_RefreshAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_GetClasses_Proxy( 
    IVis1cppPlugin __RPC_FAR * This,
    /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *Classes);


void __RPC_STUB IVis1cppPlugin_GetClasses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVis1cppPlugin_GetDefFiles_Proxy( 
    IVis1cppPlugin __RPC_FAR * This,
    /* [retval][out] */ IDefFiles __RPC_FAR *__RPC_FAR *DefFiles);


void __RPC_STUB IVis1cppPlugin_GetDefFiles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVis1cppPlugin_INTERFACE_DEFINED__ */



#ifndef __VIS1CPPLib_LIBRARY_DEFINED__
#define __VIS1CPPLib_LIBRARY_DEFINED__

/* library VIS1CPPLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VIS1CPPLib;

#ifndef ___IVis1cppPluginEvents_DISPINTERFACE_DEFINED__
#define ___IVis1cppPluginEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IVis1cppPluginEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IVis1cppPluginEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("39B1D89D-4037-4BAA-B4A3-20D9A7D22A06")
    _IVis1cppPluginEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IVis1cppPluginEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IVis1cppPluginEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IVis1cppPluginEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IVis1cppPluginEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IVis1cppPluginEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IVis1cppPluginEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IVis1cppPluginEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IVis1cppPluginEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IVis1cppPluginEventsVtbl;

    interface _IVis1cppPluginEvents
    {
        CONST_VTBL struct _IVis1cppPluginEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IVis1cppPluginEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IVis1cppPluginEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IVis1cppPluginEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IVis1cppPluginEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IVis1cppPluginEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IVis1cppPluginEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IVis1cppPluginEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IVis1cppPluginEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IClassInfo_INTERFACE_DEFINED__
#define __IClassInfo_INTERFACE_DEFINED__

/* interface IClassInfo */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IClassInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A21696BE-C0A8-417C-B30B-E5E333D12C9D")
    IClassInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DefFilePath( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SourceText( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BaseClasses( 
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IClassInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IClassInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IClassInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IClassInfo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IClassInfo __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IClassInfo __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IClassInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IClassInfo __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IClassInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IClassInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IClassInfo __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DefFilePath )( 
            IClassInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SourceText )( 
            IClassInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BaseClasses )( 
            IClassInfo __RPC_FAR * This,
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } IClassInfoVtbl;

    interface IClassInfo
    {
        CONST_VTBL struct IClassInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IClassInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IClassInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IClassInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IClassInfo_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IClassInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IClassInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IClassInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IClassInfo_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IClassInfo_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IClassInfo_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define IClassInfo_get_DefFilePath(This,pVal)	\
    (This)->lpVtbl -> get_DefFilePath(This,pVal)

#define IClassInfo_get_SourceText(This,pVal)	\
    (This)->lpVtbl -> get_SourceText(This,pVal)

#define IClassInfo_get_BaseClasses(This,pVal)	\
    (This)->lpVtbl -> get_BaseClasses(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClassInfo_get_Name_Proxy( 
    IClassInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IClassInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClassInfo_get_Path_Proxy( 
    IClassInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IClassInfo_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IClassInfo_Open_Proxy( 
    IClassInfo __RPC_FAR * This);


void __RPC_STUB IClassInfo_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClassInfo_get_DefFilePath_Proxy( 
    IClassInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IClassInfo_get_DefFilePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClassInfo_get_SourceText_Proxy( 
    IClassInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IClassInfo_get_SourceText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClassInfo_get_BaseClasses_Proxy( 
    IClassInfo __RPC_FAR * This,
    /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IClassInfo_get_BaseClasses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IClassInfo_INTERFACE_DEFINED__ */


#ifndef __IClasses_INTERFACE_DEFINED__
#define __IClasses_INTERFACE_DEFINED__

/* interface IClasses */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IClasses;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5CA5F4C-7C1B-49C7-9C38-91EDC8EFD966")
    IClasses : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindByPath( 
            /* [in] */ VARIANT strPath,
            /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pElem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT IndexOrName,
            /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IClassesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IClasses __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IClasses __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IClasses __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IClasses __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IClasses __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IClasses __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IClasses __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IClasses __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindByPath )( 
            IClasses __RPC_FAR * This,
            /* [in] */ VARIANT strPath,
            /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pElem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IClasses __RPC_FAR * This,
            /* [in] */ VARIANT IndexOrName,
            /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pVal);
        
        /* [id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IClasses __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);
        
        END_INTERFACE
    } IClassesVtbl;

    interface IClasses
    {
        CONST_VTBL struct IClassesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IClasses_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IClasses_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IClasses_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IClasses_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IClasses_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IClasses_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IClasses_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IClasses_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IClasses_FindByPath(This,strPath,pElem)	\
    (This)->lpVtbl -> FindByPath(This,strPath,pElem)

#define IClasses_get_Item(This,IndexOrName,pVal)	\
    (This)->lpVtbl -> get_Item(This,IndexOrName,pVal)

#define IClasses_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClasses_get_Count_Proxy( 
    IClasses __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IClasses_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IClasses_FindByPath_Proxy( 
    IClasses __RPC_FAR * This,
    /* [in] */ VARIANT strPath,
    /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pElem);


void __RPC_STUB IClasses_FindByPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IClasses_get_Item_Proxy( 
    IClasses __RPC_FAR * This,
    /* [in] */ VARIANT IndexOrName,
    /* [retval][out] */ IClassInfo __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IClasses_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][restricted][propget] */ HRESULT STDMETHODCALLTYPE IClasses_get__NewEnum_Proxy( 
    IClasses __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);


void __RPC_STUB IClasses_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IClasses_INTERFACE_DEFINED__ */


#ifndef __IDefFile_INTERFACE_DEFINED__
#define __IDefFile_INTERFACE_DEFINED__

/* interface IDefFile */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDefFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0291182A-AC08-486C-B77E-AB860762E41F")
    IDefFile : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Classes( 
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDefFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDefFile __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDefFile __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDefFile __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDefFile __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDefFile __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDefFile __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDefFile __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IDefFile __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IDefFile __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IDefFile __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Classes )( 
            IDefFile __RPC_FAR * This,
            /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } IDefFileVtbl;

    interface IDefFile
    {
        CONST_VTBL struct IDefFileVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDefFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDefFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDefFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDefFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDefFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDefFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDefFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDefFile_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IDefFile_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IDefFile_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define IDefFile_get_Classes(This,pVal)	\
    (This)->lpVtbl -> get_Classes(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDefFile_get_Name_Proxy( 
    IDefFile __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDefFile_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDefFile_get_Path_Proxy( 
    IDefFile __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDefFile_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDefFile_Open_Proxy( 
    IDefFile __RPC_FAR * This);


void __RPC_STUB IDefFile_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDefFile_get_Classes_Proxy( 
    IDefFile __RPC_FAR * This,
    /* [retval][out] */ IClasses __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IDefFile_get_Classes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDefFile_INTERFACE_DEFINED__ */


#ifndef __IDefFiles_INTERFACE_DEFINED__
#define __IDefFiles_INTERFACE_DEFINED__

/* interface IDefFiles */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDefFiles;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08AA2CFC-1FEA-495F-8C1B-C3F63F3F43B7")
    IDefFiles : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindByPath( 
            /* [in] */ VARIANT strPath,
            /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pElem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT IndexOrDefPath,
            /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDefFilesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDefFiles __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDefFiles __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDefFiles __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IDefFiles __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindByPath )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ VARIANT strPath,
            /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pElem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IDefFiles __RPC_FAR * This,
            /* [in] */ VARIANT IndexOrDefPath,
            /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pVal);
        
        /* [id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IDefFiles __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);
        
        END_INTERFACE
    } IDefFilesVtbl;

    interface IDefFiles
    {
        CONST_VTBL struct IDefFilesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDefFiles_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDefFiles_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDefFiles_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDefFiles_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDefFiles_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDefFiles_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDefFiles_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDefFiles_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IDefFiles_FindByPath(This,strPath,pElem)	\
    (This)->lpVtbl -> FindByPath(This,strPath,pElem)

#define IDefFiles_get_Item(This,IndexOrDefPath,pVal)	\
    (This)->lpVtbl -> get_Item(This,IndexOrDefPath,pVal)

#define IDefFiles_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDefFiles_get_Count_Proxy( 
    IDefFiles __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDefFiles_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDefFiles_FindByPath_Proxy( 
    IDefFiles __RPC_FAR * This,
    /* [in] */ VARIANT strPath,
    /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pElem);


void __RPC_STUB IDefFiles_FindByPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDefFiles_get_Item_Proxy( 
    IDefFiles __RPC_FAR * This,
    /* [in] */ VARIANT IndexOrDefPath,
    /* [retval][out] */ IDefFile __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IDefFiles_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][restricted][propget] */ HRESULT STDMETHODCALLTYPE IDefFiles_get__NewEnum_Proxy( 
    IDefFiles __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);


void __RPC_STUB IDefFiles_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDefFiles_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Vis1cppPlugin;

#ifdef __cplusplus

class DECLSPEC_UUID("DF840278-740B-4025-BE44-C5FDE9DB6436")
Vis1cppPlugin;
#endif

EXTERN_C const CLSID CLSID_CoClassInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("20274885-04A8-4661-B2C9-9079641BEF89")
CoClassInfo;
#endif

EXTERN_C const CLSID CLSID_CoClasses;

#ifdef __cplusplus

class DECLSPEC_UUID("7BDC1BAF-CD32-42D3-835F-E8C9C6D5A755")
CoClasses;
#endif

EXTERN_C const CLSID CLSID_CoDefFiles;

#ifdef __cplusplus

class DECLSPEC_UUID("44A90FD6-7CD2-46BC-8921-E3CA83BAC582")
CoDefFiles;
#endif

EXTERN_C const CLSID CLSID_CoDefFile;

#ifdef __cplusplus

class DECLSPEC_UUID("275A3D45-E17B-4174-A139-0C0BA174EF65")
CoDefFile;
#endif
#endif /* __VIS1CPPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
