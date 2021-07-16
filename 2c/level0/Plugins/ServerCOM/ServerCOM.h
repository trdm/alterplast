/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Jun 02 22:09:21 2004
 */
/* Compiler settings for C:\Visual C++\2C\Enterprise\PLUGINS\ServerCOM\ServerCOM.idl:
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

#ifndef __ServerCOM_h__
#define __ServerCOM_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IConnect_FWD_DEFINED__
#define __IConnect_FWD_DEFINED__
typedef interface IConnect IConnect;
#endif 	/* __IConnect_FWD_DEFINED__ */


#ifndef __Connect_FWD_DEFINED__
#define __Connect_FWD_DEFINED__

#ifdef __cplusplus
typedef class Connect Connect;
#else
typedef struct Connect Connect;
#endif /* __cplusplus */

#endif 	/* __Connect_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IConnect_INTERFACE_DEFINED__
#define __IConnect_INTERFACE_DEFINED__

/* interface IConnect */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IConnect;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03FD46F1-4FEE-4B0B-8B40-1E943652CA08")
    IConnect : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test( 
            int a) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test2( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test3( 
            VARIANT A) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Test4( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Test5( 
            BSTR A,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test6( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test7( 
            BSTR A) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EvalExpr( 
            BSTR Str) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EvalExpr2( 
            /* [in] */ BSTR Str,
            /* [retval][out] */ VARIANT __RPC_FAR *Ret) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Test8( 
            /* [retval][out] */ BSTR __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Test8( 
            /* [in] */ BSTR __RPC_FAR *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Test9( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Test9( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Test10( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Test10( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Test11( 
            /* [retval][out] */ VARIANT __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Array( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Array( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnect __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnect __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConnect __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConnect __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConnect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConnect __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test )( 
            IConnect __RPC_FAR * This,
            int a);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test2 )( 
            IConnect __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test3 )( 
            IConnect __RPC_FAR * This,
            VARIANT A);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Test4 )( 
            IConnect __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Test5 )( 
            IConnect __RPC_FAR * This,
            BSTR A,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test6 )( 
            IConnect __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test7 )( 
            IConnect __RPC_FAR * This,
            BSTR A);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EvalExpr )( 
            IConnect __RPC_FAR * This,
            BSTR Str);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EvalExpr2 )( 
            IConnect __RPC_FAR * This,
            /* [in] */ BSTR Str,
            /* [retval][out] */ VARIANT __RPC_FAR *Ret);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Test8 )( 
            IConnect __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Test8 )( 
            IConnect __RPC_FAR * This,
            /* [in] */ BSTR __RPC_FAR *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Test9 )( 
            IConnect __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Test9 )( 
            IConnect __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Test10 )( 
            IConnect __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Test10 )( 
            IConnect __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Test11 )( 
            IConnect __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Array )( 
            IConnect __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Array )( 
            IConnect __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } IConnectVtbl;

    interface IConnect
    {
        CONST_VTBL struct IConnectVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnect_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnect_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnect_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnect_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConnect_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConnect_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConnect_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConnect_Test(This,a)	\
    (This)->lpVtbl -> Test(This,a)

#define IConnect_Test2(This)	\
    (This)->lpVtbl -> Test2(This)

#define IConnect_Test3(This,A)	\
    (This)->lpVtbl -> Test3(This,A)

#define IConnect_put_Test4(This,newVal)	\
    (This)->lpVtbl -> put_Test4(This,newVal)

#define IConnect_put_Test5(This,A,newVal)	\
    (This)->lpVtbl -> put_Test5(This,A,newVal)

#define IConnect_Test6(This)	\
    (This)->lpVtbl -> Test6(This)

#define IConnect_Test7(This,A)	\
    (This)->lpVtbl -> Test7(This,A)

#define IConnect_EvalExpr(This,Str)	\
    (This)->lpVtbl -> EvalExpr(This,Str)

#define IConnect_EvalExpr2(This,Str,Ret)	\
    (This)->lpVtbl -> EvalExpr2(This,Str,Ret)

#define IConnect_get_Test8(This,pVal)	\
    (This)->lpVtbl -> get_Test8(This,pVal)

#define IConnect_put_Test8(This,newVal)	\
    (This)->lpVtbl -> put_Test8(This,newVal)

#define IConnect_get_Test9(This,pVal)	\
    (This)->lpVtbl -> get_Test9(This,pVal)

#define IConnect_put_Test9(This,newVal)	\
    (This)->lpVtbl -> put_Test9(This,newVal)

#define IConnect_get_Test10(This,pVal)	\
    (This)->lpVtbl -> get_Test10(This,pVal)

#define IConnect_put_Test10(This,newVal)	\
    (This)->lpVtbl -> put_Test10(This,newVal)

#define IConnect_get_Test11(This,pVal)	\
    (This)->lpVtbl -> get_Test11(This,pVal)

#define IConnect_get_Array(This,pVal)	\
    (This)->lpVtbl -> get_Array(This,pVal)

#define IConnect_put_Array(This,newVal)	\
    (This)->lpVtbl -> put_Array(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_Test_Proxy( 
    IConnect __RPC_FAR * This,
    int a);


void __RPC_STUB IConnect_Test_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_Test2_Proxy( 
    IConnect __RPC_FAR * This);


void __RPC_STUB IConnect_Test2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_Test3_Proxy( 
    IConnect __RPC_FAR * This,
    VARIANT A);


void __RPC_STUB IConnect_Test3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Test4_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IConnect_put_Test4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Test5_Proxy( 
    IConnect __RPC_FAR * This,
    BSTR A,
    /* [in] */ BSTR newVal);


void __RPC_STUB IConnect_put_Test5_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_Test6_Proxy( 
    IConnect __RPC_FAR * This);


void __RPC_STUB IConnect_Test6_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_Test7_Proxy( 
    IConnect __RPC_FAR * This,
    BSTR A);


void __RPC_STUB IConnect_Test7_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_EvalExpr_Proxy( 
    IConnect __RPC_FAR * This,
    BSTR Str);


void __RPC_STUB IConnect_EvalExpr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnect_EvalExpr2_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ BSTR Str,
    /* [retval][out] */ VARIANT __RPC_FAR *Ret);


void __RPC_STUB IConnect_EvalExpr2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConnect_get_Test8_Proxy( 
    IConnect __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConnect_get_Test8_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Test8_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ BSTR __RPC_FAR *newVal);


void __RPC_STUB IConnect_put_Test8_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConnect_get_Test9_Proxy( 
    IConnect __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConnect_get_Test9_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Test9_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IConnect_put_Test9_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConnect_get_Test10_Proxy( 
    IConnect __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IConnect_get_Test10_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Test10_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IConnect_put_Test10_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConnect_get_Test11_Proxy( 
    IConnect __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConnect_get_Test11_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConnect_get_Array_Proxy( 
    IConnect __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IConnect_get_Array_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConnect_put_Array_Proxy( 
    IConnect __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IConnect_put_Array_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnect_INTERFACE_DEFINED__ */



#ifndef __SERVERCOMLib_LIBRARY_DEFINED__
#define __SERVERCOMLib_LIBRARY_DEFINED__

/* library SERVERCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SERVERCOMLib;

EXTERN_C const CLSID CLSID_Connect;

#ifdef __cplusplus

class DECLSPEC_UUID("0497D803-C0F6-4E37-9247-6FC0080D1096")
Connect;
#endif
#endif /* __SERVERCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
