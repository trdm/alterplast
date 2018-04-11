/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Oct 02 19:54:30 2006
 */
/* Compiler settings for C:\Doc\C++\RWidjets\RWidjets.idl:
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

#ifndef __RWidjets_i_h__
#define __RWidjets_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IInitDone_FWD_DEFINED__
#define __IInitDone_FWD_DEFINED__
typedef interface IInitDone IInitDone;
#endif 	/* __IInitDone_FWD_DEFINED__ */


#ifndef __IPropertyProfile_FWD_DEFINED__
#define __IPropertyProfile_FWD_DEFINED__
typedef interface IPropertyProfile IPropertyProfile;
#endif 	/* __IPropertyProfile_FWD_DEFINED__ */


#ifndef __IAsyncEvent_FWD_DEFINED__
#define __IAsyncEvent_FWD_DEFINED__
typedef interface IAsyncEvent IAsyncEvent;
#endif 	/* __IAsyncEvent_FWD_DEFINED__ */


#ifndef __ILanguageExtender_FWD_DEFINED__
#define __ILanguageExtender_FWD_DEFINED__
typedef interface ILanguageExtender ILanguageExtender;
#endif 	/* __ILanguageExtender_FWD_DEFINED__ */


#ifndef __IStatusLine_FWD_DEFINED__
#define __IStatusLine_FWD_DEFINED__
typedef interface IStatusLine IStatusLine;
#endif 	/* __IStatusLine_FWD_DEFINED__ */


#ifndef __IExtWndsSupport_FWD_DEFINED__
#define __IExtWndsSupport_FWD_DEFINED__
typedef interface IExtWndsSupport IExtWndsSupport;
#endif 	/* __IExtWndsSupport_FWD_DEFINED__ */


#ifndef __IPropertyLink_FWD_DEFINED__
#define __IPropertyLink_FWD_DEFINED__
typedef interface IPropertyLink IPropertyLink;
#endif 	/* __IPropertyLink_FWD_DEFINED__ */


#ifndef __IRAddIn_FWD_DEFINED__
#define __IRAddIn_FWD_DEFINED__
typedef interface IRAddIn IRAddIn;
#endif 	/* __IRAddIn_FWD_DEFINED__ */


#ifndef __AddIn_FWD_DEFINED__
#define __AddIn_FWD_DEFINED__

#ifdef __cplusplus
typedef class AddIn AddIn;
#else
typedef struct AddIn AddIn;
#endif /* __cplusplus */

#endif 	/* __AddIn_FWD_DEFINED__ */


#ifndef __AddInConnection_FWD_DEFINED__
#define __AddInConnection_FWD_DEFINED__

#ifdef __cplusplus
typedef class AddInConnection AddInConnection;
#else
typedef struct AddInConnection AddInConnection;
#endif /* __cplusplus */

#endif 	/* __AddInConnection_FWD_DEFINED__ */


#ifndef __RAddIn_FWD_DEFINED__
#define __RAddIn_FWD_DEFINED__

#ifdef __cplusplus
typedef class RAddIn RAddIn;
#else
typedef struct RAddIn RAddIn;
#endif /* __cplusplus */

#endif 	/* __RAddIn_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IInitDone_INTERFACE_DEFINED__
#define __IInitDone_INTERFACE_DEFINED__

/* interface IInitDone */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInitDone;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB634001-F13D-11d0-A459-004095E1DAEA")
    IInitDone : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IDispatch __RPC_FAR *pConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Done( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [out][in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInitDoneVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInitDone __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInitDone __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInitDone __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IInitDone __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Done )( 
            IInitDone __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IInitDone __RPC_FAR * This,
            /* [out][in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInfo);
        
        END_INTERFACE
    } IInitDoneVtbl;

    interface IInitDone
    {
        CONST_VTBL struct IInitDoneVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInitDone_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInitDone_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInitDone_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInitDone_Init(This,pConnection)	\
    (This)->lpVtbl -> Init(This,pConnection)

#define IInitDone_Done(This)	\
    (This)->lpVtbl -> Done(This)

#define IInitDone_GetInfo(This,pInfo)	\
    (This)->lpVtbl -> GetInfo(This,pInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitDone_Init_Proxy( 
    IInitDone __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pConnection);


void __RPC_STUB IInitDone_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitDone_Done_Proxy( 
    IInitDone __RPC_FAR * This);


void __RPC_STUB IInitDone_Done_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInitDone_GetInfo_Proxy( 
    IInitDone __RPC_FAR * This,
    /* [out][in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *pInfo);


void __RPC_STUB IInitDone_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInitDone_INTERFACE_DEFINED__ */


#ifndef __IPropertyProfile_INTERFACE_DEFINED__
#define __IPropertyProfile_INTERFACE_DEFINED__

/* interface IPropertyProfile */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPropertyProfile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB634002-F13D-11d0-A459-004095E1DAEA")
    IPropertyProfile : public IPropertyBag
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RegisterProfileAs( 
            BSTR bstrProfileName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyProfileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyProfile __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyProfile __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyProfile __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IPropertyProfile __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [out][in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IPropertyProfile __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ VARIANT __RPC_FAR *pVar);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterProfileAs )( 
            IPropertyProfile __RPC_FAR * This,
            BSTR bstrProfileName);
        
        END_INTERFACE
    } IPropertyProfileVtbl;

    interface IPropertyProfile
    {
        CONST_VTBL struct IPropertyProfileVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyProfile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyProfile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyProfile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyProfile_Read(This,pszPropName,pVar,pErrorLog)	\
    (This)->lpVtbl -> Read(This,pszPropName,pVar,pErrorLog)

#define IPropertyProfile_Write(This,pszPropName,pVar)	\
    (This)->lpVtbl -> Write(This,pszPropName,pVar)


#define IPropertyProfile_RegisterProfileAs(This,bstrProfileName)	\
    (This)->lpVtbl -> RegisterProfileAs(This,bstrProfileName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyProfile_RegisterProfileAs_Proxy( 
    IPropertyProfile __RPC_FAR * This,
    BSTR bstrProfileName);


void __RPC_STUB IPropertyProfile_RegisterProfileAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyProfile_INTERFACE_DEFINED__ */


#ifndef __IAsyncEvent_INTERFACE_DEFINED__
#define __IAsyncEvent_INTERFACE_DEFINED__

/* interface IAsyncEvent */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAsyncEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ab634004-f13d-11d0-a459-004095e1daea")
    IAsyncEvent : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventBufferDepth( 
            long lDepth) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventBufferDepth( 
            long __RPC_FAR *plDepth) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExternalEvent( 
            BSTR bstrSource,
            BSTR bstrMessage,
            BSTR bstrData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CleanBuffer( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAsyncEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAsyncEvent __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAsyncEvent __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAsyncEvent __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEventBufferDepth )( 
            IAsyncEvent __RPC_FAR * This,
            long lDepth);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventBufferDepth )( 
            IAsyncEvent __RPC_FAR * This,
            long __RPC_FAR *plDepth);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExternalEvent )( 
            IAsyncEvent __RPC_FAR * This,
            BSTR bstrSource,
            BSTR bstrMessage,
            BSTR bstrData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CleanBuffer )( 
            IAsyncEvent __RPC_FAR * This);
        
        END_INTERFACE
    } IAsyncEventVtbl;

    interface IAsyncEvent
    {
        CONST_VTBL struct IAsyncEventVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAsyncEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAsyncEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAsyncEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAsyncEvent_SetEventBufferDepth(This,lDepth)	\
    (This)->lpVtbl -> SetEventBufferDepth(This,lDepth)

#define IAsyncEvent_GetEventBufferDepth(This,plDepth)	\
    (This)->lpVtbl -> GetEventBufferDepth(This,plDepth)

#define IAsyncEvent_ExternalEvent(This,bstrSource,bstrMessage,bstrData)	\
    (This)->lpVtbl -> ExternalEvent(This,bstrSource,bstrMessage,bstrData)

#define IAsyncEvent_CleanBuffer(This)	\
    (This)->lpVtbl -> CleanBuffer(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAsyncEvent_SetEventBufferDepth_Proxy( 
    IAsyncEvent __RPC_FAR * This,
    long lDepth);


void __RPC_STUB IAsyncEvent_SetEventBufferDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAsyncEvent_GetEventBufferDepth_Proxy( 
    IAsyncEvent __RPC_FAR * This,
    long __RPC_FAR *plDepth);


void __RPC_STUB IAsyncEvent_GetEventBufferDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAsyncEvent_ExternalEvent_Proxy( 
    IAsyncEvent __RPC_FAR * This,
    BSTR bstrSource,
    BSTR bstrMessage,
    BSTR bstrData);


void __RPC_STUB IAsyncEvent_ExternalEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAsyncEvent_CleanBuffer_Proxy( 
    IAsyncEvent __RPC_FAR * This);


void __RPC_STUB IAsyncEvent_CleanBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAsyncEvent_INTERFACE_DEFINED__ */


#ifndef __ILanguageExtender_INTERFACE_DEFINED__
#define __ILanguageExtender_INTERFACE_DEFINED__

/* interface ILanguageExtender */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ILanguageExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB634003-F13D-11d0-A459-004095E1DAEA")
    ILanguageExtender : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RegisterExtensionAs( 
            /* [out][in] */ BSTR __RPC_FAR *bstrExtensionName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetNProps( 
            /* [out][in] */ long __RPC_FAR *plProps) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindProp( 
            /* [in] */ BSTR bstrPropName,
            /* [out][in] */ long __RPC_FAR *plPropNum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPropName( 
            /* [in] */ long lPropNum,
            /* [in] */ long lPropAlias,
            /* [out][in] */ BSTR __RPC_FAR *pbstrPropName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPropVal( 
            /* [in] */ long lPropNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetPropVal( 
            /* [in] */ long lPropNum,
            /* [in] */ VARIANT __RPC_FAR *varPropVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsPropReadable( 
            /* [in] */ long lPropNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolPropRead) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsPropWritable( 
            /* [in] */ long lPropNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolPropWrite) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetNMethods( 
            /* [out][in] */ long __RPC_FAR *plMethods) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindMethod( 
            BSTR bstrMethodName,
            /* [out][in] */ long __RPC_FAR *plMethodNum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMethodName( 
            /* [in] */ long lMethodNum,
            /* [in] */ long lMethodAlias,
            /* [out][in] */ BSTR __RPC_FAR *pbstrMethodName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetNParams( 
            /* [in] */ long lMethodNum,
            /* [out][in] */ long __RPC_FAR *plParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetParamDefValue( 
            /* [in] */ long lMethodNum,
            /* [in] */ long lParamNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarParamDefValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE HasRetVal( 
            /* [in] */ long lMethodNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolRetValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CallAsProc( 
            /* [in] */ long lMethodNum,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CallAsFunc( 
            /* [in] */ long lMethodNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarRetValue,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILanguageExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILanguageExtender __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILanguageExtender __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterExtensionAs )( 
            ILanguageExtender __RPC_FAR * This,
            /* [out][in] */ BSTR __RPC_FAR *bstrExtensionName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNProps )( 
            ILanguageExtender __RPC_FAR * This,
            /* [out][in] */ long __RPC_FAR *plProps);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindProp )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ BSTR bstrPropName,
            /* [out][in] */ long __RPC_FAR *plPropNum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPropName )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lPropNum,
            /* [in] */ long lPropAlias,
            /* [out][in] */ BSTR __RPC_FAR *pbstrPropName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPropVal )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lPropNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPropVal )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lPropNum,
            /* [in] */ VARIANT __RPC_FAR *varPropVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPropReadable )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lPropNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolPropRead);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPropWritable )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lPropNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolPropWrite);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNMethods )( 
            ILanguageExtender __RPC_FAR * This,
            /* [out][in] */ long __RPC_FAR *plMethods);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindMethod )( 
            ILanguageExtender __RPC_FAR * This,
            BSTR bstrMethodName,
            /* [out][in] */ long __RPC_FAR *plMethodNum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMethodName )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [in] */ long lMethodAlias,
            /* [out][in] */ BSTR __RPC_FAR *pbstrMethodName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNParams )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [out][in] */ long __RPC_FAR *plParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParamDefValue )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [in] */ long lParamNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarParamDefValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HasRetVal )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [out][in] */ BOOL __RPC_FAR *pboolRetValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CallAsProc )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CallAsFunc )( 
            ILanguageExtender __RPC_FAR * This,
            /* [in] */ long lMethodNum,
            /* [out][in] */ VARIANT __RPC_FAR *pvarRetValue,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams);
        
        END_INTERFACE
    } ILanguageExtenderVtbl;

    interface ILanguageExtender
    {
        CONST_VTBL struct ILanguageExtenderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILanguageExtender_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILanguageExtender_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILanguageExtender_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILanguageExtender_RegisterExtensionAs(This,bstrExtensionName)	\
    (This)->lpVtbl -> RegisterExtensionAs(This,bstrExtensionName)

#define ILanguageExtender_GetNProps(This,plProps)	\
    (This)->lpVtbl -> GetNProps(This,plProps)

#define ILanguageExtender_FindProp(This,bstrPropName,plPropNum)	\
    (This)->lpVtbl -> FindProp(This,bstrPropName,plPropNum)

#define ILanguageExtender_GetPropName(This,lPropNum,lPropAlias,pbstrPropName)	\
    (This)->lpVtbl -> GetPropName(This,lPropNum,lPropAlias,pbstrPropName)

#define ILanguageExtender_GetPropVal(This,lPropNum,pvarPropVal)	\
    (This)->lpVtbl -> GetPropVal(This,lPropNum,pvarPropVal)

#define ILanguageExtender_SetPropVal(This,lPropNum,varPropVal)	\
    (This)->lpVtbl -> SetPropVal(This,lPropNum,varPropVal)

#define ILanguageExtender_IsPropReadable(This,lPropNum,pboolPropRead)	\
    (This)->lpVtbl -> IsPropReadable(This,lPropNum,pboolPropRead)

#define ILanguageExtender_IsPropWritable(This,lPropNum,pboolPropWrite)	\
    (This)->lpVtbl -> IsPropWritable(This,lPropNum,pboolPropWrite)

#define ILanguageExtender_GetNMethods(This,plMethods)	\
    (This)->lpVtbl -> GetNMethods(This,plMethods)

#define ILanguageExtender_FindMethod(This,bstrMethodName,plMethodNum)	\
    (This)->lpVtbl -> FindMethod(This,bstrMethodName,plMethodNum)

#define ILanguageExtender_GetMethodName(This,lMethodNum,lMethodAlias,pbstrMethodName)	\
    (This)->lpVtbl -> GetMethodName(This,lMethodNum,lMethodAlias,pbstrMethodName)

#define ILanguageExtender_GetNParams(This,lMethodNum,plParams)	\
    (This)->lpVtbl -> GetNParams(This,lMethodNum,plParams)

#define ILanguageExtender_GetParamDefValue(This,lMethodNum,lParamNum,pvarParamDefValue)	\
    (This)->lpVtbl -> GetParamDefValue(This,lMethodNum,lParamNum,pvarParamDefValue)

#define ILanguageExtender_HasRetVal(This,lMethodNum,pboolRetValue)	\
    (This)->lpVtbl -> HasRetVal(This,lMethodNum,pboolRetValue)

#define ILanguageExtender_CallAsProc(This,lMethodNum,paParams)	\
    (This)->lpVtbl -> CallAsProc(This,lMethodNum,paParams)

#define ILanguageExtender_CallAsFunc(This,lMethodNum,pvarRetValue,paParams)	\
    (This)->lpVtbl -> CallAsFunc(This,lMethodNum,pvarRetValue,paParams)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_RegisterExtensionAs_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [out][in] */ BSTR __RPC_FAR *bstrExtensionName);


void __RPC_STUB ILanguageExtender_RegisterExtensionAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetNProps_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [out][in] */ long __RPC_FAR *plProps);


void __RPC_STUB ILanguageExtender_GetNProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_FindProp_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ BSTR bstrPropName,
    /* [out][in] */ long __RPC_FAR *plPropNum);


void __RPC_STUB ILanguageExtender_FindProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetPropName_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lPropNum,
    /* [in] */ long lPropAlias,
    /* [out][in] */ BSTR __RPC_FAR *pbstrPropName);


void __RPC_STUB ILanguageExtender_GetPropName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetPropVal_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lPropNum,
    /* [out][in] */ VARIANT __RPC_FAR *pvarPropVal);


void __RPC_STUB ILanguageExtender_GetPropVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_SetPropVal_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lPropNum,
    /* [in] */ VARIANT __RPC_FAR *varPropVal);


void __RPC_STUB ILanguageExtender_SetPropVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_IsPropReadable_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lPropNum,
    /* [out][in] */ BOOL __RPC_FAR *pboolPropRead);


void __RPC_STUB ILanguageExtender_IsPropReadable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_IsPropWritable_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lPropNum,
    /* [out][in] */ BOOL __RPC_FAR *pboolPropWrite);


void __RPC_STUB ILanguageExtender_IsPropWritable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetNMethods_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [out][in] */ long __RPC_FAR *plMethods);


void __RPC_STUB ILanguageExtender_GetNMethods_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_FindMethod_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    BSTR bstrMethodName,
    /* [out][in] */ long __RPC_FAR *plMethodNum);


void __RPC_STUB ILanguageExtender_FindMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetMethodName_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [in] */ long lMethodAlias,
    /* [out][in] */ BSTR __RPC_FAR *pbstrMethodName);


void __RPC_STUB ILanguageExtender_GetMethodName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetNParams_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [out][in] */ long __RPC_FAR *plParams);


void __RPC_STUB ILanguageExtender_GetNParams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_GetParamDefValue_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [in] */ long lParamNum,
    /* [out][in] */ VARIANT __RPC_FAR *pvarParamDefValue);


void __RPC_STUB ILanguageExtender_GetParamDefValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_HasRetVal_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [out][in] */ BOOL __RPC_FAR *pboolRetValue);


void __RPC_STUB ILanguageExtender_HasRetVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_CallAsProc_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams);


void __RPC_STUB ILanguageExtender_CallAsProc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ILanguageExtender_CallAsFunc_Proxy( 
    ILanguageExtender __RPC_FAR * This,
    /* [in] */ long lMethodNum,
    /* [out][in] */ VARIANT __RPC_FAR *pvarRetValue,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *paParams);


void __RPC_STUB ILanguageExtender_CallAsFunc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILanguageExtender_INTERFACE_DEFINED__ */


#ifndef __IStatusLine_INTERFACE_DEFINED__
#define __IStatusLine_INTERFACE_DEFINED__

/* interface IStatusLine */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IStatusLine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ab634005-f13d-11d0-a459-004095e1daea")
    IStatusLine : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetStatusLine( 
            BSTR bstrStatusLine) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ResetStatusLine( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStatusLineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IStatusLine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IStatusLine __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IStatusLine __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetStatusLine )( 
            IStatusLine __RPC_FAR * This,
            BSTR bstrStatusLine);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetStatusLine )( 
            IStatusLine __RPC_FAR * This);
        
        END_INTERFACE
    } IStatusLineVtbl;

    interface IStatusLine
    {
        CONST_VTBL struct IStatusLineVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStatusLine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStatusLine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStatusLine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStatusLine_SetStatusLine(This,bstrStatusLine)	\
    (This)->lpVtbl -> SetStatusLine(This,bstrStatusLine)

#define IStatusLine_ResetStatusLine(This)	\
    (This)->lpVtbl -> ResetStatusLine(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStatusLine_SetStatusLine_Proxy( 
    IStatusLine __RPC_FAR * This,
    BSTR bstrStatusLine);


void __RPC_STUB IStatusLine_SetStatusLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStatusLine_ResetStatusLine_Proxy( 
    IStatusLine __RPC_FAR * This);


void __RPC_STUB IStatusLine_ResetStatusLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStatusLine_INTERFACE_DEFINED__ */


#ifndef __IExtWndsSupport_INTERFACE_DEFINED__
#define __IExtWndsSupport_INTERFACE_DEFINED__

/* interface IExtWndsSupport */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IExtWndsSupport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("efe19ea0-09e4-11d2-a601-008048da00de")
    IExtWndsSupport : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAppMainFrame( 
            /* [out][in] */ HWND __RPC_FAR *hwnd) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAppMDIFrame( 
            /* [out][in] */ HWND __RPC_FAR *hwnd) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateAddInWindow( 
            /* [in] */ BSTR bstrProgID,
            /* [in] */ BSTR bstrWindowName,
            /* [in] */ long dwStyles,
            /* [in] */ long dwExStyles,
            /* [in] */ RECT __RPC_FAR *rctl,
            /* [in] */ long Flags,
            /* [out][in] */ HWND __RPC_FAR *pHwnd,
            /* [out][in] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IExtWndsSupportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IExtWndsSupport __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IExtWndsSupport __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IExtWndsSupport __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAppMainFrame )( 
            IExtWndsSupport __RPC_FAR * This,
            /* [out][in] */ HWND __RPC_FAR *hwnd);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAppMDIFrame )( 
            IExtWndsSupport __RPC_FAR * This,
            /* [out][in] */ HWND __RPC_FAR *hwnd);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAddInWindow )( 
            IExtWndsSupport __RPC_FAR * This,
            /* [in] */ BSTR bstrProgID,
            /* [in] */ BSTR bstrWindowName,
            /* [in] */ long dwStyles,
            /* [in] */ long dwExStyles,
            /* [in] */ RECT __RPC_FAR *rctl,
            /* [in] */ long Flags,
            /* [out][in] */ HWND __RPC_FAR *pHwnd,
            /* [out][in] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp);
        
        END_INTERFACE
    } IExtWndsSupportVtbl;

    interface IExtWndsSupport
    {
        CONST_VTBL struct IExtWndsSupportVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IExtWndsSupport_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IExtWndsSupport_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IExtWndsSupport_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IExtWndsSupport_GetAppMainFrame(This,hwnd)	\
    (This)->lpVtbl -> GetAppMainFrame(This,hwnd)

#define IExtWndsSupport_GetAppMDIFrame(This,hwnd)	\
    (This)->lpVtbl -> GetAppMDIFrame(This,hwnd)

#define IExtWndsSupport_CreateAddInWindow(This,bstrProgID,bstrWindowName,dwStyles,dwExStyles,rctl,Flags,pHwnd,pDisp)	\
    (This)->lpVtbl -> CreateAddInWindow(This,bstrProgID,bstrWindowName,dwStyles,dwExStyles,rctl,Flags,pHwnd,pDisp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IExtWndsSupport_GetAppMainFrame_Proxy( 
    IExtWndsSupport __RPC_FAR * This,
    /* [out][in] */ HWND __RPC_FAR *hwnd);


void __RPC_STUB IExtWndsSupport_GetAppMainFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IExtWndsSupport_GetAppMDIFrame_Proxy( 
    IExtWndsSupport __RPC_FAR * This,
    /* [out][in] */ HWND __RPC_FAR *hwnd);


void __RPC_STUB IExtWndsSupport_GetAppMDIFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IExtWndsSupport_CreateAddInWindow_Proxy( 
    IExtWndsSupport __RPC_FAR * This,
    /* [in] */ BSTR bstrProgID,
    /* [in] */ BSTR bstrWindowName,
    /* [in] */ long dwStyles,
    /* [in] */ long dwExStyles,
    /* [in] */ RECT __RPC_FAR *rctl,
    /* [in] */ long Flags,
    /* [out][in] */ HWND __RPC_FAR *pHwnd,
    /* [out][in] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp);


void __RPC_STUB IExtWndsSupport_CreateAddInWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IExtWndsSupport_INTERFACE_DEFINED__ */


#ifndef __IPropertyLink_INTERFACE_DEFINED__
#define __IPropertyLink_INTERFACE_DEFINED__

/* interface IPropertyLink */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPropertyLink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52512A61-2A9D-11d1-A4D6-004095E1DAEA")
    IPropertyLink : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            BOOL __RPC_FAR *boolEnabled) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            BOOL boolEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPropertyLinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPropertyLink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPropertyLink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPropertyLink __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Enabled )( 
            IPropertyLink __RPC_FAR * This,
            BOOL __RPC_FAR *boolEnabled);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Enabled )( 
            IPropertyLink __RPC_FAR * This,
            BOOL boolEnabled);
        
        END_INTERFACE
    } IPropertyLinkVtbl;

    interface IPropertyLink
    {
        CONST_VTBL struct IPropertyLinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPropertyLink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPropertyLink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPropertyLink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPropertyLink_get_Enabled(This,boolEnabled)	\
    (This)->lpVtbl -> get_Enabled(This,boolEnabled)

#define IPropertyLink_put_Enabled(This,boolEnabled)	\
    (This)->lpVtbl -> put_Enabled(This,boolEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyLink_get_Enabled_Proxy( 
    IPropertyLink __RPC_FAR * This,
    BOOL __RPC_FAR *boolEnabled);


void __RPC_STUB IPropertyLink_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPropertyLink_put_Enabled_Proxy( 
    IPropertyLink __RPC_FAR * This,
    BOOL boolEnabled);


void __RPC_STUB IPropertyLink_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPropertyLink_INTERFACE_DEFINED__ */


#ifndef __IRAddIn_INTERFACE_DEFINED__
#define __IRAddIn_INTERFACE_DEFINED__

/* interface IRAddIn */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRAddIn;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("58E9679E-F2E6-11D5-AE41-0002B31F6532")
    IRAddIn : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IRAddInVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRAddIn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRAddIn __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRAddIn __RPC_FAR * This);
        
        END_INTERFACE
    } IRAddInVtbl;

    interface IRAddIn
    {
        CONST_VTBL struct IRAddInVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRAddIn_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRAddIn_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRAddIn_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRAddIn_INTERFACE_DEFINED__ */



#ifndef __RWidjetsLib_LIBRARY_DEFINED__
#define __RWidjetsLib_LIBRARY_DEFINED__

/* library RWidjetsLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_RWidjetsLib;

EXTERN_C const CLSID CLSID_AddIn;

#ifdef __cplusplus

class DECLSPEC_UUID("04024C31-0E7B-11d1-A486-004095E1DAEA")
AddIn;
#endif

EXTERN_C const CLSID CLSID_AddInConnection;

#ifdef __cplusplus

class DECLSPEC_UUID("912B7391-250D-11d1-A4C8-004095E1DAEA")
AddInConnection;
#endif

EXTERN_C const CLSID CLSID_RAddIn;

#ifdef __cplusplus

class DECLSPEC_UUID("58E96722-F2E6-11D5-AE41-0002B31F6532")
RAddIn;
#endif
#endif /* __RWidjetsLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long __RPC_FAR *, unsigned long            , HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long __RPC_FAR *, HWND __RPC_FAR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long __RPC_FAR *, unsigned long            , LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
