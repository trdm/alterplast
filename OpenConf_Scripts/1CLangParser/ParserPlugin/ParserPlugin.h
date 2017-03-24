/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jan 22 15:20:13 2007
 */
/* Compiler settings for S:\OpenConf_Scripts\—истемные‘айлы\1CLangParser\ParserPlugin\ParserPlugin.idl:
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

#ifndef __ParserPlugin_h__
#define __ParserPlugin_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICodeAnalyser_FWD_DEFINED__
#define __ICodeAnalyser_FWD_DEFINED__
typedef interface ICodeAnalyser ICodeAnalyser;
#endif 	/* __ICodeAnalyser_FWD_DEFINED__ */


#ifndef __CodeAnalyser_FWD_DEFINED__
#define __CodeAnalyser_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeAnalyser CodeAnalyser;
#else
typedef struct CodeAnalyser CodeAnalyser;
#endif /* __cplusplus */

#endif 	/* __CodeAnalyser_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ICodeAnalyser_INTERFACE_DEFINED__
#define __ICodeAnalyser_INTERFACE_DEFINED__

/* interface ICodeAnalyser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICodeAnalyser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("513B102D-9967-485E-8E1A-7D5829C46AD4")
    ICodeAnalyser : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Line( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Line( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Column( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Column( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTypeInPos( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimeSpent( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TypePropertyName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TypePropertyName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TypePropertyType( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TypePropertyType( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TypeName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TypeName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddType( 
            /* [in] */ BSTR TypeName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTypeProperty( 
            /* [in] */ BSTR PropName,
            /* [in] */ BSTR PropType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindType( 
            /* [in] */ BSTR TypeName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartPropsIteration( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NextProperty( 
            /* [retval][out] */ BSTR __RPC_FAR *pPropName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindVar( 
            /* [in] */ BSTR bstrVarName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindTypeProp( 
            BSTR bstrPropName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindVarProp( 
            /* [in] */ BSTR bstrPropName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VarPropType( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VarPropType( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VarPropName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VarPropName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartVarPropsIteration( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NextVarProp( 
            /* [retval][out] */ BSTR __RPC_FAR *pPropName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddExemplarModifyingMethod( 
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long nParam_PropName,
            /* [in] */ long nParam_PropType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AnalyseGlobalModule( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAllTypes( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveType( 
            /* [in] */ BSTR bstrTypeName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LC_AddVar( 
            /* [in] */ BSTR bstrVarName,
            /* [in] */ BSTR bstrVarType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LC_AddFunction( 
            /* [in] */ BSTR bstrFuncName,
            /* [in] */ BSTR bstrFuncType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LC_AddVarProperty( 
            /* [in] */ BSTR bstrVarName,
            /* [in] */ BSTR bstrPropName,
            /* [in] */ BSTR bstrPropType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetLocalContext( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddBaseType( 
            /* [in] */ BSTR bstrBaseType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AnalyseModule( 
            /* [in] */ BSTR bstrModuleBody,
            /* [in] */ long line,
            /* [in] */ long col) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICodeAnalyserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICodeAnalyser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICodeAnalyser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Line )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Line )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Column )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Column )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInPos )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimeSpent )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TypePropertyName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TypePropertyName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TypePropertyType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TypePropertyType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TypeName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TypeName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR TypeName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddTypeProperty )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR PropName,
            /* [in] */ BSTR PropType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR TypeName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartPropsIteration )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextProperty )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pPropName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindVar )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrVarName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindTypeProp )( 
            ICodeAnalyser __RPC_FAR * This,
            BSTR bstrPropName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindVarProp )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrPropName,
            /* [retval][out] */ BOOL __RPC_FAR *pFound);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VarPropType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VarPropType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VarPropName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VarPropName )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartVarPropsIteration )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextVarProp )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pPropName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddExemplarModifyingMethod )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long nParam_PropName,
            /* [in] */ long nParam_PropType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AnalyseGlobalModule )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveAllTypes )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrTypeName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LC_AddVar )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrVarName,
            /* [in] */ BSTR bstrVarType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LC_AddFunction )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrFuncName,
            /* [in] */ BSTR bstrFuncType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LC_AddVarProperty )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrVarName,
            /* [in] */ BSTR bstrPropName,
            /* [in] */ BSTR bstrPropType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetLocalContext )( 
            ICodeAnalyser __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddBaseType )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrBaseType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AnalyseModule )( 
            ICodeAnalyser __RPC_FAR * This,
            /* [in] */ BSTR bstrModuleBody,
            /* [in] */ long line,
            /* [in] */ long col);
        
        END_INTERFACE
    } ICodeAnalyserVtbl;

    interface ICodeAnalyser
    {
        CONST_VTBL struct ICodeAnalyserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICodeAnalyser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeAnalyser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICodeAnalyser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICodeAnalyser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeAnalyser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeAnalyser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeAnalyser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeAnalyser_get_Line(This,pVal)	\
    (This)->lpVtbl -> get_Line(This,pVal)

#define ICodeAnalyser_put_Line(This,newVal)	\
    (This)->lpVtbl -> put_Line(This,newVal)

#define ICodeAnalyser_get_Column(This,pVal)	\
    (This)->lpVtbl -> get_Column(This,pVal)

#define ICodeAnalyser_put_Column(This,newVal)	\
    (This)->lpVtbl -> put_Column(This,newVal)

#define ICodeAnalyser_GetTypeInPos(This)	\
    (This)->lpVtbl -> GetTypeInPos(This)

#define ICodeAnalyser_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ICodeAnalyser_get_TimeSpent(This,pVal)	\
    (This)->lpVtbl -> get_TimeSpent(This,pVal)

#define ICodeAnalyser_put_TypePropertyName(This,newVal)	\
    (This)->lpVtbl -> put_TypePropertyName(This,newVal)

#define ICodeAnalyser_get_TypePropertyName(This,pVal)	\
    (This)->lpVtbl -> get_TypePropertyName(This,pVal)

#define ICodeAnalyser_put_TypePropertyType(This,newVal)	\
    (This)->lpVtbl -> put_TypePropertyType(This,newVal)

#define ICodeAnalyser_get_TypePropertyType(This,pVal)	\
    (This)->lpVtbl -> get_TypePropertyType(This,pVal)

#define ICodeAnalyser_get_TypeName(This,pVal)	\
    (This)->lpVtbl -> get_TypeName(This,pVal)

#define ICodeAnalyser_put_TypeName(This,newVal)	\
    (This)->lpVtbl -> put_TypeName(This,newVal)

#define ICodeAnalyser_AddType(This,TypeName)	\
    (This)->lpVtbl -> AddType(This,TypeName)

#define ICodeAnalyser_AddTypeProperty(This,PropName,PropType)	\
    (This)->lpVtbl -> AddTypeProperty(This,PropName,PropType)

#define ICodeAnalyser_FindType(This,TypeName,pFound)	\
    (This)->lpVtbl -> FindType(This,TypeName,pFound)

#define ICodeAnalyser_StartPropsIteration(This)	\
    (This)->lpVtbl -> StartPropsIteration(This)

#define ICodeAnalyser_NextProperty(This,pPropName)	\
    (This)->lpVtbl -> NextProperty(This,pPropName)

#define ICodeAnalyser_FindVar(This,bstrVarName,pFound)	\
    (This)->lpVtbl -> FindVar(This,bstrVarName,pFound)

#define ICodeAnalyser_FindTypeProp(This,bstrPropName,pFound)	\
    (This)->lpVtbl -> FindTypeProp(This,bstrPropName,pFound)

#define ICodeAnalyser_FindVarProp(This,bstrPropName,pFound)	\
    (This)->lpVtbl -> FindVarProp(This,bstrPropName,pFound)

#define ICodeAnalyser_get_VarPropType(This,pVal)	\
    (This)->lpVtbl -> get_VarPropType(This,pVal)

#define ICodeAnalyser_put_VarPropType(This,newVal)	\
    (This)->lpVtbl -> put_VarPropType(This,newVal)

#define ICodeAnalyser_get_VarPropName(This,pVal)	\
    (This)->lpVtbl -> get_VarPropName(This,pVal)

#define ICodeAnalyser_put_VarPropName(This,newVal)	\
    (This)->lpVtbl -> put_VarPropName(This,newVal)

#define ICodeAnalyser_StartVarPropsIteration(This)	\
    (This)->lpVtbl -> StartVarPropsIteration(This)

#define ICodeAnalyser_NextVarProp(This,pPropName)	\
    (This)->lpVtbl -> NextVarProp(This,pPropName)

#define ICodeAnalyser_AddExemplarModifyingMethod(This,bstrMethodName,nParam_PropName,nParam_PropType)	\
    (This)->lpVtbl -> AddExemplarModifyingMethod(This,bstrMethodName,nParam_PropName,nParam_PropType)

#define ICodeAnalyser_AnalyseGlobalModule(This)	\
    (This)->lpVtbl -> AnalyseGlobalModule(This)

#define ICodeAnalyser_RemoveAllTypes(This)	\
    (This)->lpVtbl -> RemoveAllTypes(This)

#define ICodeAnalyser_RemoveType(This,bstrTypeName)	\
    (This)->lpVtbl -> RemoveType(This,bstrTypeName)

#define ICodeAnalyser_LC_AddVar(This,bstrVarName,bstrVarType)	\
    (This)->lpVtbl -> LC_AddVar(This,bstrVarName,bstrVarType)

#define ICodeAnalyser_LC_AddFunction(This,bstrFuncName,bstrFuncType)	\
    (This)->lpVtbl -> LC_AddFunction(This,bstrFuncName,bstrFuncType)

#define ICodeAnalyser_LC_AddVarProperty(This,bstrVarName,bstrPropName,bstrPropType)	\
    (This)->lpVtbl -> LC_AddVarProperty(This,bstrVarName,bstrPropName,bstrPropType)

#define ICodeAnalyser_ResetLocalContext(This)	\
    (This)->lpVtbl -> ResetLocalContext(This)

#define ICodeAnalyser_AddBaseType(This,bstrBaseType)	\
    (This)->lpVtbl -> AddBaseType(This,bstrBaseType)

#define ICodeAnalyser_AnalyseModule(This,bstrModuleBody,line,col)	\
    (This)->lpVtbl -> AnalyseModule(This,bstrModuleBody,line,col)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_Line_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_Line_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_Line_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ICodeAnalyser_put_Line_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_Column_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_Column_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_Column_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ICodeAnalyser_put_Column_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_GetTypeInPos_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_GetTypeInPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_Type_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_TimeSpent_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_TimeSpent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_TypePropertyName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICodeAnalyser_put_TypePropertyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_TypePropertyName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_TypePropertyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_TypePropertyType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICodeAnalyser_put_TypePropertyType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_TypePropertyType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_TypePropertyType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_TypeName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_TypeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_TypeName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICodeAnalyser_put_TypeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AddType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR TypeName);


void __RPC_STUB ICodeAnalyser_AddType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AddTypeProperty_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR PropName,
    /* [in] */ BSTR PropType);


void __RPC_STUB ICodeAnalyser_AddTypeProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_FindType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR TypeName,
    /* [retval][out] */ BOOL __RPC_FAR *pFound);


void __RPC_STUB ICodeAnalyser_FindType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_StartPropsIteration_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_StartPropsIteration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_NextProperty_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pPropName);


void __RPC_STUB ICodeAnalyser_NextProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_FindVar_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrVarName,
    /* [retval][out] */ BOOL __RPC_FAR *pFound);


void __RPC_STUB ICodeAnalyser_FindVar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_FindTypeProp_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    BSTR bstrPropName,
    /* [retval][out] */ BOOL __RPC_FAR *pFound);


void __RPC_STUB ICodeAnalyser_FindTypeProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_FindVarProp_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrPropName,
    /* [retval][out] */ BOOL __RPC_FAR *pFound);


void __RPC_STUB ICodeAnalyser_FindVarProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_VarPropType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_VarPropType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_VarPropType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICodeAnalyser_put_VarPropType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_get_VarPropName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICodeAnalyser_get_VarPropName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_put_VarPropName_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICodeAnalyser_put_VarPropName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_StartVarPropsIteration_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_StartVarPropsIteration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_NextVarProp_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pPropName);


void __RPC_STUB ICodeAnalyser_NextVarProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AddExemplarModifyingMethod_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ long nParam_PropName,
    /* [in] */ long nParam_PropType);


void __RPC_STUB ICodeAnalyser_AddExemplarModifyingMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AnalyseGlobalModule_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_AnalyseGlobalModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_RemoveAllTypes_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_RemoveAllTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_RemoveType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrTypeName);


void __RPC_STUB ICodeAnalyser_RemoveType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_LC_AddVar_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrVarName,
    /* [in] */ BSTR bstrVarType);


void __RPC_STUB ICodeAnalyser_LC_AddVar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_LC_AddFunction_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrFuncName,
    /* [in] */ BSTR bstrFuncType);


void __RPC_STUB ICodeAnalyser_LC_AddFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_LC_AddVarProperty_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrVarName,
    /* [in] */ BSTR bstrPropName,
    /* [in] */ BSTR bstrPropType);


void __RPC_STUB ICodeAnalyser_LC_AddVarProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_ResetLocalContext_Proxy( 
    ICodeAnalyser __RPC_FAR * This);


void __RPC_STUB ICodeAnalyser_ResetLocalContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AddBaseType_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrBaseType);


void __RPC_STUB ICodeAnalyser_AddBaseType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeAnalyser_AnalyseModule_Proxy( 
    ICodeAnalyser __RPC_FAR * This,
    /* [in] */ BSTR bstrModuleBody,
    /* [in] */ long line,
    /* [in] */ long col);


void __RPC_STUB ICodeAnalyser_AnalyseModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICodeAnalyser_INTERFACE_DEFINED__ */



#ifndef __PARSERPLUGINLib_LIBRARY_DEFINED__
#define __PARSERPLUGINLib_LIBRARY_DEFINED__

/* library PARSERPLUGINLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PARSERPLUGINLib;

EXTERN_C const CLSID CLSID_CodeAnalyser;

#ifdef __cplusplus

class DECLSPEC_UUID("05C61FC5-A72C-42DA-82C0-0513A9618FBA")
CodeAnalyser;
#endif
#endif /* __PARSERPLUGINLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
