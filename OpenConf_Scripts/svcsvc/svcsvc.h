/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 23 10:56:08 2007
 */
/* Compiler settings for S:\OpenConf_Scripts\—истемные‘айлы\svcsvc\svcsvc.idl:
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

#ifndef __svcsvc_h__
#define __svcsvc_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IService_FWD_DEFINED__
#define __IService_FWD_DEFINED__
typedef interface IService IService;
#endif 	/* __IService_FWD_DEFINED__ */


#ifndef __Service_FWD_DEFINED__
#define __Service_FWD_DEFINED__

#ifdef __cplusplus
typedef class Service Service;
#else
typedef struct Service Service;
#endif /* __cplusplus */

#endif 	/* __Service_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __SVCSVCLib_LIBRARY_DEFINED__
#define __SVCSVCLib_LIBRARY_DEFINED__

/* library SVCSVCLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SVCSVCLib;

#ifndef __IService_INTERFACE_DEFINED__
#define __IService_INTERFACE_DEFINED__

/* interface IService */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A5194858-F330-4E3A-980A-FF487A5C3F6F")
    IService : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AnsiToOEM( 
            /* [in] */ BSTR Ansi,
            /* [retval][out] */ BSTR __RPC_FAR *OEM) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OEMToAnsi( 
            /* [in] */ BSTR OEM,
            /* [retval][out] */ BSTR __RPC_FAR *Ansi) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectFile( 
            /* [in] */ VARIANT_BOOL ForSave,
            /* [in] */ BSTR FileName,
            /* [in] */ BSTR Filter,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL MultiSelect,
            /* [retval][out] */ BSTR __RPC_FAR *FilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectFolder( 
            /* [defaultvalue][optional][in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ BSTR StartFolder,
            /* [defaultvalue][optional][in] */ long Flags,
            /* [defaultvalue][optional][in] */ BSTR Root,
            /* [retval][out] */ BSTR __RPC_FAR *FolderPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileA2O( 
            /* [in] */ BSTR Path) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileO2A( 
            /* [in] */ BSTR Path) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectValue( 
            /* [in] */ BSTR Values,
            /* [in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
            /* [retval][out] */ BSTR __RPC_FAR *Select) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectInTree( 
            /* [in] */ BSTR Values,
            /* [in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL OnlyItems,
            /* [retval][out] */ BSTR __RPC_FAR *Select) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PopupMenu( 
            /* [in] */ BSTR Values,
            /* [defaultvalue][optional][in] */ short ShowIn,
            /* [defaultvalue][optional][in] */ long X,
            /* [defaultvalue][optional][in] */ long Y,
            /* [retval][out] */ BSTR __RPC_FAR *Select) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FilterValue( 
            /* [in] */ BSTR Values,
            /* [defaultvalue][optional][in] */ short Flags,
            /* [defaultvalue][optional][in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ long X,
            /* [defaultvalue][optional][in] */ long Y,
            /* [defaultvalue][optional][in] */ long W,
            /* [defaultvalue][optional][in] */ long H,
            /* [retval][out] */ BSTR __RPC_FAR *Result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowText( 
            /* [defaultvalue][optional][in] */ long WH,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL IsList,
            /* [retval][out] */ BSTR __RPC_FAR *Result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWindowText( 
            /* [defaultvalue][optional][in] */ long WH,
            /* [in] */ BSTR Text) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetImageList( 
            /* [in] */ BSTR Values,
            /* [retval][out] */ BSTR __RPC_FAR *Result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IService __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IService __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IService __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IService __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IService __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IService __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IService __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AnsiToOEM )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Ansi,
            /* [retval][out] */ BSTR __RPC_FAR *OEM);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OEMToAnsi )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR OEM,
            /* [retval][out] */ BSTR __RPC_FAR *Ansi);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectFile )( 
            IService __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL ForSave,
            /* [in] */ BSTR FileName,
            /* [in] */ BSTR Filter,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL MultiSelect,
            /* [retval][out] */ BSTR __RPC_FAR *FilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectFolder )( 
            IService __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ BSTR StartFolder,
            /* [defaultvalue][optional][in] */ long Flags,
            /* [defaultvalue][optional][in] */ BSTR Root,
            /* [retval][out] */ BSTR __RPC_FAR *FolderPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FileA2O )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Path);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FileO2A )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Path);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectValue )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Values,
            /* [in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
            /* [retval][out] */ BSTR __RPC_FAR *Select);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectInTree )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Values,
            /* [in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL OnlyItems,
            /* [retval][out] */ BSTR __RPC_FAR *Select);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PopupMenu )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Values,
            /* [defaultvalue][optional][in] */ short ShowIn,
            /* [defaultvalue][optional][in] */ long X,
            /* [defaultvalue][optional][in] */ long Y,
            /* [retval][out] */ BSTR __RPC_FAR *Select);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FilterValue )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Values,
            /* [defaultvalue][optional][in] */ short Flags,
            /* [defaultvalue][optional][in] */ BSTR Caption,
            /* [defaultvalue][optional][in] */ long X,
            /* [defaultvalue][optional][in] */ long Y,
            /* [defaultvalue][optional][in] */ long W,
            /* [defaultvalue][optional][in] */ long H,
            /* [retval][out] */ BSTR __RPC_FAR *Result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindowText )( 
            IService __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ long WH,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL IsList,
            /* [retval][out] */ BSTR __RPC_FAR *Result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWindowText )( 
            IService __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ long WH,
            /* [in] */ BSTR Text);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Version )( 
            IService __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImageList )( 
            IService __RPC_FAR * This,
            /* [in] */ BSTR Values,
            /* [retval][out] */ BSTR __RPC_FAR *Result);
        
        END_INTERFACE
    } IServiceVtbl;

    interface IService
    {
        CONST_VTBL struct IServiceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IService_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IService_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IService_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IService_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IService_AnsiToOEM(This,Ansi,OEM)	\
    (This)->lpVtbl -> AnsiToOEM(This,Ansi,OEM)

#define IService_OEMToAnsi(This,OEM,Ansi)	\
    (This)->lpVtbl -> OEMToAnsi(This,OEM,Ansi)

#define IService_SelectFile(This,ForSave,FileName,Filter,MultiSelect,FilePath)	\
    (This)->lpVtbl -> SelectFile(This,ForSave,FileName,Filter,MultiSelect,FilePath)

#define IService_SelectFolder(This,Caption,StartFolder,Flags,Root,FolderPath)	\
    (This)->lpVtbl -> SelectFolder(This,Caption,StartFolder,Flags,Root,FolderPath)

#define IService_FileA2O(This,Path)	\
    (This)->lpVtbl -> FileA2O(This,Path)

#define IService_FileO2A(This,Path)	\
    (This)->lpVtbl -> FileO2A(This,Path)

#define IService_SelectValue(This,Values,Caption,Mark,Select)	\
    (This)->lpVtbl -> SelectValue(This,Values,Caption,Mark,Select)

#define IService_SelectInTree(This,Values,Caption,Mark,OnlyItems,Select)	\
    (This)->lpVtbl -> SelectInTree(This,Values,Caption,Mark,OnlyItems,Select)

#define IService_PopupMenu(This,Values,ShowIn,X,Y,Select)	\
    (This)->lpVtbl -> PopupMenu(This,Values,ShowIn,X,Y,Select)

#define IService_FilterValue(This,Values,Flags,Caption,X,Y,W,H,Result)	\
    (This)->lpVtbl -> FilterValue(This,Values,Flags,Caption,X,Y,W,H,Result)

#define IService_GetWindowText(This,WH,IsList,Result)	\
    (This)->lpVtbl -> GetWindowText(This,WH,IsList,Result)

#define IService_SetWindowText(This,WH,Text)	\
    (This)->lpVtbl -> SetWindowText(This,WH,Text)

#define IService_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IService_SetImageList(This,Values,Result)	\
    (This)->lpVtbl -> SetImageList(This,Values,Result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_AnsiToOEM_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Ansi,
    /* [retval][out] */ BSTR __RPC_FAR *OEM);


void __RPC_STUB IService_AnsiToOEM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_OEMToAnsi_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR OEM,
    /* [retval][out] */ BSTR __RPC_FAR *Ansi);


void __RPC_STUB IService_OEMToAnsi_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SelectFile_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL ForSave,
    /* [in] */ BSTR FileName,
    /* [in] */ BSTR Filter,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL MultiSelect,
    /* [retval][out] */ BSTR __RPC_FAR *FilePath);


void __RPC_STUB IService_SelectFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SelectFolder_Proxy( 
    IService __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ BSTR Caption,
    /* [defaultvalue][optional][in] */ BSTR StartFolder,
    /* [defaultvalue][optional][in] */ long Flags,
    /* [defaultvalue][optional][in] */ BSTR Root,
    /* [retval][out] */ BSTR __RPC_FAR *FolderPath);


void __RPC_STUB IService_SelectFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_FileA2O_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Path);


void __RPC_STUB IService_FileA2O_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_FileO2A_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Path);


void __RPC_STUB IService_FileO2A_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SelectValue_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Values,
    /* [in] */ BSTR Caption,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
    /* [retval][out] */ BSTR __RPC_FAR *Select);


void __RPC_STUB IService_SelectValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SelectInTree_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Values,
    /* [in] */ BSTR Caption,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL Mark,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL OnlyItems,
    /* [retval][out] */ BSTR __RPC_FAR *Select);


void __RPC_STUB IService_SelectInTree_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_PopupMenu_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Values,
    /* [defaultvalue][optional][in] */ short ShowIn,
    /* [defaultvalue][optional][in] */ long X,
    /* [defaultvalue][optional][in] */ long Y,
    /* [retval][out] */ BSTR __RPC_FAR *Select);


void __RPC_STUB IService_PopupMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_FilterValue_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Values,
    /* [defaultvalue][optional][in] */ short Flags,
    /* [defaultvalue][optional][in] */ BSTR Caption,
    /* [defaultvalue][optional][in] */ long X,
    /* [defaultvalue][optional][in] */ long Y,
    /* [defaultvalue][optional][in] */ long W,
    /* [defaultvalue][optional][in] */ long H,
    /* [retval][out] */ BSTR __RPC_FAR *Result);


void __RPC_STUB IService_FilterValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_GetWindowText_Proxy( 
    IService __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ long WH,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL IsList,
    /* [retval][out] */ BSTR __RPC_FAR *Result);


void __RPC_STUB IService_GetWindowText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SetWindowText_Proxy( 
    IService __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ long WH,
    /* [in] */ BSTR Text);


void __RPC_STUB IService_SetWindowText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IService_get_Version_Proxy( 
    IService __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IService_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IService_SetImageList_Proxy( 
    IService __RPC_FAR * This,
    /* [in] */ BSTR Values,
    /* [retval][out] */ BSTR __RPC_FAR *Result);


void __RPC_STUB IService_SetImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IService_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Service;

#ifdef __cplusplus

class DECLSPEC_UUID("6CDD75A4-C42E-4DE3-B87A-C75DAB91DD02")
Service;
#endif
#endif /* __SVCSVCLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
