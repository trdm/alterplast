/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Apr 30 10:53:43 2007
 */
/* Compiler settings for c:\Visual Studio\Projects\telepat\telepat.idl:
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

#ifndef __telepat_h__
#define __telepat_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPlugin_FWD_DEFINED__
#define __IPlugin_FWD_DEFINED__
typedef interface IPlugin IPlugin;
#endif 	/* __IPlugin_FWD_DEFINED__ */


#ifndef __DTelepatEvents_FWD_DEFINED__
#define __DTelepatEvents_FWD_DEFINED__
typedef interface DTelepatEvents DTelepatEvents;
#endif 	/* __DTelepatEvents_FWD_DEFINED__ */


#ifndef __Plugin_FWD_DEFINED__
#define __Plugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class Plugin Plugin;
#else
typedef struct Plugin Plugin;
#endif /* __cplusplus */

#endif 	/* __Plugin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __TELEPATLib_LIBRARY_DEFINED__
#define __TELEPATLib_LIBRARY_DEFINED__

/* library TELEPATLib */
/* [helpstring][version][uuid] */ 


DEFINE_GUID(LIBID_TELEPATLib,0x0BB7CAE2,0xD194,0x425D,0xA6,0xA2,0x0D,0x8F,0x95,0xE4,0xF2,0x78);

#ifndef __IPlugin_INTERFACE_DEFINED__
#define __IPlugin_INTERFACE_DEFINED__

/* interface IPlugin */
/* [unique][helpstring][dual][uuid][object] */ 


DEFINE_GUID(IID_IPlugin,0x12A76B58,0xD814,0x4BFB,0x85,0xA0,0x49,0xF5,0x6D,0x05,0xE1,0x7E);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("12A76B58-D814-4BFB-85A0-49F56D05E17E")
    IPlugin : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Components( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Components( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Language( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UseStdMethodDlg( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UseStdMethodDlg( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NoOrderMethodDlg( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NoOrderMethodDlg( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FilterMethodDlg( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FilterMethodDlg( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoParamInfo( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoParamInfo( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParamInfoAddMethDescr( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ParamInfoAddMethDescr( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParamInfoAddParamDescr( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ParamInfoAddParamDescr( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoActiveCountSymbols( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoActiveCountSymbols( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisableTemplateInRemString( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisableTemplateInRemString( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConvertTemplate( 
            /* [in] */ BSTR TemplateText,
            /* [optional][in] */ VARIANT TemplateName,
            /* [retval][out] */ BSTR __RPC_FAR *Result) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AddTemplate( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AddTemplate( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Jump( 
            /* [in] */ long Line,
            /* [defaultvalue][optional][in] */ long Col = -1,
            /* [defaultvalue][optional][in] */ long LineEnd = -1,
            /* [defaultvalue][optional][in] */ long ColEnd = -1,
            /* [defaultvalue][optional][in] */ BSTR ModuleName = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMethodText( 
            /* [in] */ long Line,
            /* [in] */ long Col,
            /* [retval][out] */ BSTR __RPC_FAR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReReadWords( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPlugin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPlugin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPlugin __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Components )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Components )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Language )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Language )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UseStdMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UseStdMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NoOrderMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NoOrderMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FilterMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FilterMethodDlg )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AutoParamInfo )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AutoParamInfo )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ParamInfoAddMethDescr )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ParamInfoAddMethDescr )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ParamInfoAddParamDescr )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ParamInfoAddParamDescr )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AutoActiveCountSymbols )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AutoActiveCountSymbols )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DisableTemplateInRemString )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DisableTemplateInRemString )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConvertTemplate )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ BSTR TemplateText,
            /* [optional][in] */ VARIANT TemplateName,
            /* [retval][out] */ BSTR __RPC_FAR *Result);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AddTemplate )( 
            IPlugin __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AddTemplate )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Jump )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ long Line,
            /* [defaultvalue][optional][in] */ long Col,
            /* [defaultvalue][optional][in] */ long LineEnd,
            /* [defaultvalue][optional][in] */ long ColEnd,
            /* [defaultvalue][optional][in] */ BSTR ModuleName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMethodText )( 
            IPlugin __RPC_FAR * This,
            /* [in] */ long Line,
            /* [in] */ long Col,
            /* [retval][out] */ BSTR __RPC_FAR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReReadWords )( 
            IPlugin __RPC_FAR * This);
        
        END_INTERFACE
    } IPluginVtbl;

    interface IPlugin
    {
        CONST_VTBL struct IPluginVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPlugin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPlugin_get_Components(This,pVal)	\
    (This)->lpVtbl -> get_Components(This,pVal)

#define IPlugin_put_Components(This,newVal)	\
    (This)->lpVtbl -> put_Components(This,newVal)

#define IPlugin_get_Language(This,pVal)	\
    (This)->lpVtbl -> get_Language(This,pVal)

#define IPlugin_put_Language(This,newVal)	\
    (This)->lpVtbl -> put_Language(This,newVal)

#define IPlugin_get_UseStdMethodDlg(This,pVal)	\
    (This)->lpVtbl -> get_UseStdMethodDlg(This,pVal)

#define IPlugin_put_UseStdMethodDlg(This,newVal)	\
    (This)->lpVtbl -> put_UseStdMethodDlg(This,newVal)

#define IPlugin_get_NoOrderMethodDlg(This,pVal)	\
    (This)->lpVtbl -> get_NoOrderMethodDlg(This,pVal)

#define IPlugin_put_NoOrderMethodDlg(This,newVal)	\
    (This)->lpVtbl -> put_NoOrderMethodDlg(This,newVal)

#define IPlugin_get_FilterMethodDlg(This,pVal)	\
    (This)->lpVtbl -> get_FilterMethodDlg(This,pVal)

#define IPlugin_put_FilterMethodDlg(This,newVal)	\
    (This)->lpVtbl -> put_FilterMethodDlg(This,newVal)

#define IPlugin_get_AutoParamInfo(This,pVal)	\
    (This)->lpVtbl -> get_AutoParamInfo(This,pVal)

#define IPlugin_put_AutoParamInfo(This,newVal)	\
    (This)->lpVtbl -> put_AutoParamInfo(This,newVal)

#define IPlugin_get_ParamInfoAddMethDescr(This,pVal)	\
    (This)->lpVtbl -> get_ParamInfoAddMethDescr(This,pVal)

#define IPlugin_put_ParamInfoAddMethDescr(This,newVal)	\
    (This)->lpVtbl -> put_ParamInfoAddMethDescr(This,newVal)

#define IPlugin_get_ParamInfoAddParamDescr(This,pVal)	\
    (This)->lpVtbl -> get_ParamInfoAddParamDescr(This,pVal)

#define IPlugin_put_ParamInfoAddParamDescr(This,newVal)	\
    (This)->lpVtbl -> put_ParamInfoAddParamDescr(This,newVal)

#define IPlugin_get_AutoActiveCountSymbols(This,pVal)	\
    (This)->lpVtbl -> get_AutoActiveCountSymbols(This,pVal)

#define IPlugin_put_AutoActiveCountSymbols(This,newVal)	\
    (This)->lpVtbl -> put_AutoActiveCountSymbols(This,newVal)

#define IPlugin_get_DisableTemplateInRemString(This,pVal)	\
    (This)->lpVtbl -> get_DisableTemplateInRemString(This,pVal)

#define IPlugin_put_DisableTemplateInRemString(This,newVal)	\
    (This)->lpVtbl -> put_DisableTemplateInRemString(This,newVal)

#define IPlugin_ConvertTemplate(This,TemplateText,TemplateName,Result)	\
    (This)->lpVtbl -> ConvertTemplate(This,TemplateText,TemplateName,Result)

#define IPlugin_get_AddTemplate(This,pVal)	\
    (This)->lpVtbl -> get_AddTemplate(This,pVal)

#define IPlugin_put_AddTemplate(This,newVal)	\
    (This)->lpVtbl -> put_AddTemplate(This,newVal)

#define IPlugin_Jump(This,Line,Col,LineEnd,ColEnd,ModuleName)	\
    (This)->lpVtbl -> Jump(This,Line,Col,LineEnd,ColEnd,ModuleName)

#define IPlugin_GetMethodText(This,Line,Col,pRetVal)	\
    (This)->lpVtbl -> GetMethodText(This,Line,Col,pRetVal)

#define IPlugin_ReReadWords(This)	\
    (This)->lpVtbl -> ReReadWords(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_Components_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_Components_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_Components_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IPlugin_put_Components_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_Language_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_Language_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IPlugin_put_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_UseStdMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_UseStdMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_UseStdMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_UseStdMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_NoOrderMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_NoOrderMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_NoOrderMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_NoOrderMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_FilterMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_FilterMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_FilterMethodDlg_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_FilterMethodDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_AutoParamInfo_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_AutoParamInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_AutoParamInfo_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_AutoParamInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_ParamInfoAddMethDescr_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_ParamInfoAddMethDescr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_ParamInfoAddMethDescr_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_ParamInfoAddMethDescr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_ParamInfoAddParamDescr_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_ParamInfoAddParamDescr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_ParamInfoAddParamDescr_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_ParamInfoAddParamDescr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_AutoActiveCountSymbols_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_AutoActiveCountSymbols_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_AutoActiveCountSymbols_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB IPlugin_put_AutoActiveCountSymbols_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_DisableTemplateInRemString_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_DisableTemplateInRemString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_DisableTemplateInRemString_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IPlugin_put_DisableTemplateInRemString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPlugin_ConvertTemplate_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ BSTR TemplateText,
    /* [optional][in] */ VARIANT TemplateName,
    /* [retval][out] */ BSTR __RPC_FAR *Result);


void __RPC_STUB IPlugin_ConvertTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugin_get_AddTemplate_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IPlugin_get_AddTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPlugin_put_AddTemplate_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IPlugin_put_AddTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPlugin_Jump_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ long Line,
    /* [defaultvalue][optional][in] */ long Col,
    /* [defaultvalue][optional][in] */ long LineEnd,
    /* [defaultvalue][optional][in] */ long ColEnd,
    /* [defaultvalue][optional][in] */ BSTR ModuleName);


void __RPC_STUB IPlugin_Jump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPlugin_GetMethodText_Proxy( 
    IPlugin __RPC_FAR * This,
    /* [in] */ long Line,
    /* [in] */ long Col,
    /* [retval][out] */ BSTR __RPC_FAR *pRetVal);


void __RPC_STUB IPlugin_GetMethodText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPlugin_ReReadWords_Proxy( 
    IPlugin __RPC_FAR * This);


void __RPC_STUB IPlugin_ReReadWords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPlugin_INTERFACE_DEFINED__ */


#ifndef __DTelepatEvents_DISPINTERFACE_DEFINED__
#define __DTelepatEvents_DISPINTERFACE_DEFINED__

/* dispinterface DTelepatEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID_DTelepatEvents,0xb5aae64e,0xd429,0x41ed,0xae,0x12,0xbd,0x9b,0xb9,0xe9,0xad,0xc1);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("b5aae64e-d429-41ed-ae12-bd9bb9e9adc1")
    DTelepatEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DTelepatEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            DTelepatEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            DTelepatEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            DTelepatEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            DTelepatEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            DTelepatEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            DTelepatEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            DTelepatEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } DTelepatEventsVtbl;

    interface DTelepatEvents
    {
        CONST_VTBL struct DTelepatEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DTelepatEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define DTelepatEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define DTelepatEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define DTelepatEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define DTelepatEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define DTelepatEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define DTelepatEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DTelepatEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Plugin,0x28BC7A6D,0x9F0F,0x42B0,0xB9,0x34,0x59,0xDA,0xBA,0xA2,0x27,0x55);

#ifdef __cplusplus

class DECLSPEC_UUID("28BC7A6D-9F0F-42B0-B934-59DABAA22755")
Plugin;
#endif
#endif /* __TELEPATLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
