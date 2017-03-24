/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Feb 06 14:47:16 2007
 */
/* Compiler settings for c:\Visual Studio\Projects\config\interfaces.idl:
    Os (OptLev=s), W0, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __interfaces_h__
#define __interfaces_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMetaData_FWD_DEFINED__
#define __IMetaData_FWD_DEFINED__
typedef interface IMetaData IMetaData;
#endif 	/* __IMetaData_FWD_DEFINED__ */


#ifndef __IMetaDataObj_FWD_DEFINED__
#define __IMetaDataObj_FWD_DEFINED__
typedef interface IMetaDataObj IMetaDataObj;
#endif 	/* __IMetaDataObj_FWD_DEFINED__ */


#ifndef __IMetaDataObjArray_FWD_DEFINED__
#define __IMetaDataObjArray_FWD_DEFINED__
typedef interface IMetaDataObjArray IMetaDataObjArray;
#endif 	/* __IMetaDataObjArray_FWD_DEFINED__ */


#ifndef __ICfgWindows_FWD_DEFINED__
#define __ICfgWindows_FWD_DEFINED__
typedef interface ICfgWindows ICfgWindows;
#endif 	/* __ICfgWindows_FWD_DEFINED__ */


#ifndef __IScripts_FWD_DEFINED__
#define __IScripts_FWD_DEFINED__
typedef interface IScripts IScripts;
#endif 	/* __IScripts_FWD_DEFINED__ */


#ifndef __IPlugins_FWD_DEFINED__
#define __IPlugins_FWD_DEFINED__
typedef interface IPlugins IPlugins;
#endif 	/* __IPlugins_FWD_DEFINED__ */


#ifndef __ITaskDef_FWD_DEFINED__
#define __ITaskDef_FWD_DEFINED__
typedef interface ITaskDef ITaskDef;
#endif 	/* __ITaskDef_FWD_DEFINED__ */


#ifndef __IConstants_FWD_DEFINED__
#define __IConstants_FWD_DEFINED__
typedef interface IConstants IConstants;
#endif 	/* __IConstants_FWD_DEFINED__ */


#ifndef __ICfgDocuments_FWD_DEFINED__
#define __ICfgDocuments_FWD_DEFINED__
typedef interface ICfgDocuments ICfgDocuments;
#endif 	/* __ICfgDocuments_FWD_DEFINED__ */


#ifndef __ICfgDoc_FWD_DEFINED__
#define __ICfgDoc_FWD_DEFINED__
typedef interface ICfgDoc ICfgDoc;
#endif 	/* __ICfgDoc_FWD_DEFINED__ */


#ifndef __ICfgWindow_FWD_DEFINED__
#define __ICfgWindow_FWD_DEFINED__
typedef interface ICfgWindow ICfgWindow;
#endif 	/* __ICfgWindow_FWD_DEFINED__ */


#ifndef __ICfgTimer_FWD_DEFINED__
#define __ICfgTimer_FWD_DEFINED__
typedef interface ICfgTimer ICfgTimer;
#endif 	/* __ICfgTimer_FWD_DEFINED__ */


#ifndef __IAllTypedObj_FWD_DEFINED__
#define __IAllTypedObj_FWD_DEFINED__
typedef interface IAllTypedObj IAllTypedObj;
#endif 	/* __IAllTypedObj_FWD_DEFINED__ */


#ifndef __IMDWindow_FWD_DEFINED__
#define __IMDWindow_FWD_DEFINED__
typedef interface IMDWindow IMDWindow;
#endif 	/* __IMDWindow_FWD_DEFINED__ */


#ifndef __IDialogDoc_FWD_DEFINED__
#define __IDialogDoc_FWD_DEFINED__
typedef interface IDialogDoc IDialogDoc;
#endif 	/* __IDialogDoc_FWD_DEFINED__ */


#ifndef __MDWindow_FWD_DEFINED__
#define __MDWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class MDWindow MDWindow;
#else
typedef struct MDWindow MDWindow;
#endif /* __cplusplus */

#endif 	/* __MDWindow_FWD_DEFINED__ */


#ifndef __IConfigurator_FWD_DEFINED__
#define __IConfigurator_FWD_DEFINED__
typedef interface IConfigurator IConfigurator;
#endif 	/* __IConfigurator_FWD_DEFINED__ */


#ifndef __IConfigEvents_FWD_DEFINED__
#define __IConfigEvents_FWD_DEFINED__
typedef interface IConfigEvents IConfigEvents;
#endif 	/* __IConfigEvents_FWD_DEFINED__ */


#ifndef __Configurator_FWD_DEFINED__
#define __Configurator_FWD_DEFINED__

#ifdef __cplusplus
typedef class Configurator Configurator;
#else
typedef struct Configurator Configurator;
#endif /* __cplusplus */

#endif 	/* __Configurator_FWD_DEFINED__ */


#ifndef __IConfigPlugin_FWD_DEFINED__
#define __IConfigPlugin_FWD_DEFINED__
typedef interface IConfigPlugin IConfigPlugin;
#endif 	/* __IConfigPlugin_FWD_DEFINED__ */


#ifndef __Scripts_FWD_DEFINED__
#define __Scripts_FWD_DEFINED__

#ifdef __cplusplus
typedef class Scripts Scripts;
#else
typedef struct Scripts Scripts;
#endif /* __cplusplus */

#endif 	/* __Scripts_FWD_DEFINED__ */


#ifndef __Plugins_FWD_DEFINED__
#define __Plugins_FWD_DEFINED__

#ifdef __cplusplus
typedef class Plugins Plugins;
#else
typedef struct Plugins Plugins;
#endif /* __cplusplus */

#endif 	/* __Plugins_FWD_DEFINED__ */


#ifndef __IType1C_FWD_DEFINED__
#define __IType1C_FWD_DEFINED__
typedef interface IType1C IType1C;
#endif 	/* __IType1C_FWD_DEFINED__ */


#ifndef __Type1C_FWD_DEFINED__
#define __Type1C_FWD_DEFINED__

#ifdef __cplusplus
typedef class Type1C Type1C;
#else
typedef struct Type1C Type1C;
#endif /* __cplusplus */

#endif 	/* __Type1C_FWD_DEFINED__ */


#ifndef __IMetaDataObjChilds_FWD_DEFINED__
#define __IMetaDataObjChilds_FWD_DEFINED__
typedef interface IMetaDataObjChilds IMetaDataObjChilds;
#endif 	/* __IMetaDataObjChilds_FWD_DEFINED__ */


#ifndef __MetaDataObjChilds_FWD_DEFINED__
#define __MetaDataObjChilds_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaDataObjChilds MetaDataObjChilds;
#else
typedef struct MetaDataObjChilds MetaDataObjChilds;
#endif /* __cplusplus */

#endif 	/* __MetaDataObjChilds_FWD_DEFINED__ */


#ifndef __IMetaDataObjProps_FWD_DEFINED__
#define __IMetaDataObjProps_FWD_DEFINED__
typedef interface IMetaDataObjProps IMetaDataObjProps;
#endif 	/* __IMetaDataObjProps_FWD_DEFINED__ */


#ifndef __MetaDataObjProps_FWD_DEFINED__
#define __MetaDataObjProps_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaDataObjProps MetaDataObjProps;
#else
typedef struct MetaDataObjProps MetaDataObjProps;
#endif /* __cplusplus */

#endif 	/* __MetaDataObjProps_FWD_DEFINED__ */


#ifndef __IMigration_FWD_DEFINED__
#define __IMigration_FWD_DEFINED__
typedef interface IMigration IMigration;
#endif 	/* __IMigration_FWD_DEFINED__ */


#ifndef __Migration_FWD_DEFINED__
#define __Migration_FWD_DEFINED__

#ifdef __cplusplus
typedef class Migration Migration;
#else
typedef struct Migration Migration;
#endif /* __cplusplus */

#endif 	/* __Migration_FWD_DEFINED__ */


#ifndef __MetaDataObj_FWD_DEFINED__
#define __MetaDataObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaDataObj MetaDataObj;
#else
typedef struct MetaDataObj MetaDataObj;
#endif /* __cplusplus */

#endif 	/* __MetaDataObj_FWD_DEFINED__ */


#ifndef __MetaDataObjArray_FWD_DEFINED__
#define __MetaDataObjArray_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaDataObjArray MetaDataObjArray;
#else
typedef struct MetaDataObjArray MetaDataObjArray;
#endif /* __cplusplus */

#endif 	/* __MetaDataObjArray_FWD_DEFINED__ */


#ifndef __ISelfScript_FWD_DEFINED__
#define __ISelfScript_FWD_DEFINED__
typedef interface ISelfScript ISelfScript;
#endif 	/* __ISelfScript_FWD_DEFINED__ */


#ifndef __SelfScript_FWD_DEFINED__
#define __SelfScript_FWD_DEFINED__

#ifdef __cplusplus
typedef class SelfScript SelfScript;
#else
typedef struct SelfScript SelfScript;
#endif /* __cplusplus */

#endif 	/* __SelfScript_FWD_DEFINED__ */


#ifndef __IMetaEnumVal_FWD_DEFINED__
#define __IMetaEnumVal_FWD_DEFINED__
typedef interface IMetaEnumVal IMetaEnumVal;
#endif 	/* __IMetaEnumVal_FWD_DEFINED__ */


#ifndef __MetaEnumVal_FWD_DEFINED__
#define __MetaEnumVal_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaEnumVal MetaEnumVal;
#else
typedef struct MetaEnumVal MetaEnumVal;
#endif /* __cplusplus */

#endif 	/* __MetaEnumVal_FWD_DEFINED__ */


#ifndef __IMetaEnum_FWD_DEFINED__
#define __IMetaEnum_FWD_DEFINED__
typedef interface IMetaEnum IMetaEnum;
#endif 	/* __IMetaEnum_FWD_DEFINED__ */


#ifndef __MetaEnum_FWD_DEFINED__
#define __MetaEnum_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaEnum MetaEnum;
#else
typedef struct MetaEnum MetaEnum;
#endif /* __cplusplus */

#endif 	/* __MetaEnum_FWD_DEFINED__ */


#ifndef __IMetaEnums_FWD_DEFINED__
#define __IMetaEnums_FWD_DEFINED__
typedef interface IMetaEnums IMetaEnums;
#endif 	/* __IMetaEnums_FWD_DEFINED__ */


#ifndef __MetaEnums_FWD_DEFINED__
#define __MetaEnums_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaEnums MetaEnums;
#else
typedef struct MetaEnums MetaEnums;
#endif /* __cplusplus */

#endif 	/* __MetaEnums_FWD_DEFINED__ */


#ifndef __MetaData_FWD_DEFINED__
#define __MetaData_FWD_DEFINED__

#ifdef __cplusplus
typedef class MetaData MetaData;
#else
typedef struct MetaData MetaData;
#endif /* __cplusplus */

#endif 	/* __MetaData_FWD_DEFINED__ */


#ifndef __TaskDef_FWD_DEFINED__
#define __TaskDef_FWD_DEFINED__

#ifdef __cplusplus
typedef class TaskDef TaskDef;
#else
typedef struct TaskDef TaskDef;
#endif /* __cplusplus */

#endif 	/* __TaskDef_FWD_DEFINED__ */


#ifndef __AllTypedObj_FWD_DEFINED__
#define __AllTypedObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class AllTypedObj AllTypedObj;
#else
typedef struct AllTypedObj AllTypedObj;
#endif /* __cplusplus */

#endif 	/* __AllTypedObj_FWD_DEFINED__ */


#ifndef __IConstDef_FWD_DEFINED__
#define __IConstDef_FWD_DEFINED__
typedef interface IConstDef IConstDef;
#endif 	/* __IConstDef_FWD_DEFINED__ */


#ifndef __ConstDef_FWD_DEFINED__
#define __ConstDef_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConstDef ConstDef;
#else
typedef struct ConstDef ConstDef;
#endif /* __cplusplus */

#endif 	/* __ConstDef_FWD_DEFINED__ */


#ifndef __Constants_FWD_DEFINED__
#define __Constants_FWD_DEFINED__

#ifdef __cplusplus
typedef class Constants Constants;
#else
typedef struct Constants Constants;
#endif /* __cplusplus */

#endif 	/* __Constants_FWD_DEFINED__ */


#ifndef __CfgDoc_FWD_DEFINED__
#define __CfgDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgDoc CfgDoc;
#else
typedef struct CfgDoc CfgDoc;
#endif /* __cplusplus */

#endif 	/* __CfgDoc_FWD_DEFINED__ */


#ifndef __CfgDocuments_FWD_DEFINED__
#define __CfgDocuments_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgDocuments CfgDocuments;
#else
typedef struct CfgDocuments CfgDocuments;
#endif /* __cplusplus */

#endif 	/* __CfgDocuments_FWD_DEFINED__ */


#ifndef __IWorkBook_FWD_DEFINED__
#define __IWorkBook_FWD_DEFINED__
typedef interface IWorkBook IWorkBook;
#endif 	/* __IWorkBook_FWD_DEFINED__ */


#ifndef __WorkBook_FWD_DEFINED__
#define __WorkBook_FWD_DEFINED__

#ifdef __cplusplus
typedef class WorkBook WorkBook;
#else
typedef struct WorkBook WorkBook;
#endif /* __cplusplus */

#endif 	/* __WorkBook_FWD_DEFINED__ */


#ifndef __ITextDoc_FWD_DEFINED__
#define __ITextDoc_FWD_DEFINED__
typedef interface ITextDoc ITextDoc;
#endif 	/* __ITextDoc_FWD_DEFINED__ */


#ifndef __TextDoc_FWD_DEFINED__
#define __TextDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class TextDoc TextDoc;
#else
typedef struct TextDoc TextDoc;
#endif /* __cplusplus */

#endif 	/* __TextDoc_FWD_DEFINED__ */


#ifndef __ITableDoc_FWD_DEFINED__
#define __ITableDoc_FWD_DEFINED__
typedef interface ITableDoc ITableDoc;
#endif 	/* __ITableDoc_FWD_DEFINED__ */


#ifndef __TableDoc_FWD_DEFINED__
#define __TableDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableDoc TableDoc;
#else
typedef struct TableDoc TableDoc;
#endif /* __cplusplus */

#endif 	/* __TableDoc_FWD_DEFINED__ */


#ifndef __DialogDoc_FWD_DEFINED__
#define __DialogDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class DialogDoc DialogDoc;
#else
typedef struct DialogDoc DialogDoc;
#endif /* __cplusplus */

#endif 	/* __DialogDoc_FWD_DEFINED__ */


#ifndef __CfgWindow_FWD_DEFINED__
#define __CfgWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgWindow CfgWindow;
#else
typedef struct CfgWindow CfgWindow;
#endif /* __cplusplus */

#endif 	/* __CfgWindow_FWD_DEFINED__ */


#ifndef __CfgWindows_FWD_DEFINED__
#define __CfgWindows_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgWindows CfgWindows;
#else
typedef struct CfgWindows CfgWindows;
#endif /* __cplusplus */

#endif 	/* __CfgWindows_FWD_DEFINED__ */


#ifndef __CfgTimer_FWD_DEFINED__
#define __CfgTimer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgTimer CfgTimer;
#else
typedef struct CfgTimer CfgTimer;
#endif /* __cplusplus */

#endif 	/* __CfgTimer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __OpenConf_LIBRARY_DEFINED__
#define __OpenConf_LIBRARY_DEFINED__

/* library OpenConf */
/* [helpstring][version][uuid] */ 
















typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0001
    {	mNone	= 0,
	mBlueTriangle	= 1,
	mExclamation	= 2,
	mExclamation2	= 3,
	mExclamation3	= 4,
	mInformation	= 5,
	mBlackErr	= 6,
	mRedErr	= 7,
	mMetaData	= 8,
	mUnderlinedErr	= 9
    }	MsgLineMarkers;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0002
    {	tmdConstant	= 1,
	tmdRefRekv	= 2,
	tmdReference	= 3,
	tmdRegIzm	= 4,
	tmdRegResurs	= 5,
	tmdRegister	= 6,
	tmdDocHeadRekv	= 7,
	tmdDocTableRekv	= 8,
	tmdDocument	= 9,
	tmdJDGrafa	= 10,
	tmdJournalDoc	= 11,
	tmdEnumVal	= 12,
	tmdEnum	= 13,
	tmdListRefs	= 14,
	tmdFormList	= 15,
	tmdReport	= 16,
	tmdTask	= 17,
	tmdKalendar	= 18,
	tmdCalcKind	= 19,
	tmdCalcGroup	= 20,
	tmdCJRekv	= 21,
	tmdCalcJournal	= 22,
	tmdCalcVar	= 23,
	tmdRegRekv	= 25,
	tmdCommonRekv	= 26,
	tmdAccountChart	= 27,
	tmdAccRekv	= 28,
	tmdAcount	= 30,
	tmdAccountSubc	= 31,
	tmdProvRekv	= 32,
	tmdOperRekv	= 33,
	tmdSelGraf	= 34,
	tmdNumbers	= 35,
	tmdSubconto	= 36,
	tmdStream	= 43,
	tmdRecalcRule	= 44
    }	TypeOfMetaDataObj;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0003
    {	cmdOpenConfigWnd	= 33188,
	cmdLoadMD	= 33206,
	cmdMergeMD	= 33260,
	cmdSave	= 0xe103
    }	ConfigCommands;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0004
    {	mbOK	= 0,
	mbOkCancel	= 1,
	mbAbortRetryIgnore	= 2,
	mbYesNoCancel	= 3,
	mbYesNo	= 4,
	mbRetryCancel	= 5
    }	MsgBoxStyles;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0005
    {	mbaOK	= 1,
	mbaCancel	= 2,
	mbaAbort	= 3,
	mbaRetry	= 4,
	mbaIgnore	= 5,
	mbaYes	= 6,
	mbaNo	= 7
    }	MsgBoxAnswers;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0006
    {	ecSession	= 0,
	ecAdmin	= 1,
	ecChangeData	= 2,
	ecInfo	= 3,
	ecWarning	= 4,
	ecError	= 5
    }	EventCategories;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0007
    {	rmEnterprise	= -1,
	rmDebugger	= 2,
	rmMonitor	= 4
    }	RunAppModes;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0008
    {	appBinDir	= 0,
	appIBDir	= 0x1,
	appUserDir	= 0x2,
	appTempDir	= 0x3,
	appCfgPath	= 0x4,
	appDDPath	= 0x5,
	appDDSPath	= 0x6,
	appDBAPath	= 0x7,
	appMDPath	= 0x8,
	appLCKPath	= 0x9,
	appIBName	= 0xc,
	appUserName	= 0xd,
	appFullUserName	= 0xe,
	appRightName	= 0x10,
	appInterfaceName	= 0x11,
	appRegSettingsPath	= 0x12
    }	AppProperties;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0000_0009
    {	mdaSelect	= 0,
	mdaProps	= 1,
	mdaEdit	= 0x83af,
	mdaEditDescr	= 0x83b6,
	mdaDelete	= 0x83ad,
	mdaMoveUp	= 0x83b1,
	mdaMoveDown	= 0x83b0,
	mdaOrder	= 0x83b2,
	mdaTestRefers	= 0x81f4
    }	mdActions;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0211_0001
    {	capOpenIn	= 0,
	capShowMacros	= 1,
	capIconNum	= 2,
	capGetMenu	= 3
    }	PluginCaps;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0211_0002
    {	paMenu	= 0
    }	PluginAction;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0214_0001
    {	ttVoid	= 0,
	ttNumber	= 1,
	ttString	= 2,
	ttDate	= 3,
	ttEnum	= 10,
	ttReference	= 11,
	ttDocument	= 12,
	ttKalendar	= 13,
	ttCalcKind	= 14,
	ttAccnt	= 15,
	ttSbcKind	= 16,
	ttAccntChrt	= 17
    }	tTypes;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0217_0001
    {	mrAll	= 0,
	mrCreate	= 1,
	mrCreateCentr	= 2
    }	MigrationRules;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0229_0001
    {	docUnknown	= 0,
	docWorkBook	= 1,
	docText	= 2,
	docTable	= 3,
	docDEdit	= 4
    }	DocTypes;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0233_0001
    {	tbMxl	= 0,
	tbExc	= tbMxl + 1,
	tbHtml	= tbExc + 1,
	tbText	= tbHtml + 1
    }	TableSaveTypes;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0234_0001
    {	ctrlSkipEnter	= 0x4,
	ctrlQuickChoise	= 0x8,
	ctrlUseHelpAsTip	= 0x10,
	ctrlHaveSelBtn	= 0x20,
	ctrlDefaultFont	= 0x40,
	ctrlAutoFontColor	= 0x80,
	ctrlPictureStretch	= 0x100,
	ctrlDisableAutoChoise	= 0x200,
	ctrlPictureCenter	= 0x400,
	ctrlPictureAspect	= 0x800,
	ctrlMultiLine	= 0x1000,
	ctrlUnVisible	= 0x2000,
	ctrlDisabled	= 0x4000,
	ctrlTransparent	= 0x10000,
	ctrlBorderSimple	= 0x20000,
	ctrlBorderEtch	= 0x40000,
	ctrlBorderRaise	= 0x80000,
	ctrlAutoChoise	= 0x100000,
	ctrlOnNextRow	= 0x400000,
	ctrlInThisColumn	= 0x800000,
	ctrlDontSaveSettings	= 0x1000000,
	ctrlSelList	= 0x2000000,
	ctrlNegativeRed	= 0x4000000,
	ctrlUsePicture	= 0x8000000,
	ctrlDisableEdit	= 0x10000000
    }	CtrlFlags;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0234_0002
    {	cpLeft	= 0,
	cpTop	= 1,
	cpWidth	= 2,
	cpHeight	= 3,
	cpStrID	= 4,
	cpTitle	= 5,
	cpFormul	= 6,
	cpToolTip	= 7,
	cpHelpString	= 8,
	cpLayer	= 9,
	cpMetaID	= 10,
	cpFlags	= 11,
	cpFlagsEx	= 12,
	cpLinkWith	= 13,
	cpFontName	= 14,
	cpFontSize	= 15,
	cpFontWeight	= 16,
	cpFontItalic	= 17,
	cpFontUnderline	= 18,
	cpFontColor	= 19,
	cpHotKey	= 20,
	cpPictID	= 21,
	cpPictMode	= 22
    }	CtrlPropsIdx;

typedef /* [helpstring] */ 
enum __MIDL___MIDL_itf_interfaces_0234_0003
    {	fpWidth	= 0,
	fpHeight	= 1,
	fpToolBar	= 2,
	fpAutoSize	= 3,
	fpAutoLayout	= 4,
	fpUseAutoFont	= 5,
	fpFontName	= 6,
	fpFontSize	= 7,
	fpFontWeight	= 8,
	fpFontItalic	= 9,
	fpFontUnderline	= 10,
	fpTitle	= 11,
	fpFlags	= 12,
	fpPictID	= 13,
	fpPictMode	= 14,
	fpBkColor	= 15
    }	FormPropsIdx;


DEFINE_GUID(LIBID_OpenConf,0x578F85C8,0x89EB,0x4cda,0xAC,0x7E,0x8F,0x3B,0xB3,0x4E,0x8B,0x54);

#ifndef __IMetaData_INTERFACE_DEFINED__
#define __IMetaData_INTERFACE_DEFINED__

/* interface IMetaData */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IMetaData,0x2576069C,0xDBF4,0x4b30,0xA8,0x39,0x36,0x35,0x5F,0x46,0x1F,0x50);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2576069C-DBF4-4b30-A839-36355F461F50")
    IMetaData : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TaskDef( 
            /* [retval][out] */ ITaskDef __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Modified( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindObject( 
            /* [in] */ long id,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RefreshMDWnd( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveMDToFile( 
            /* [in] */ BSTR path,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL FireEvent,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *retVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Describe( 
            /* [retval][out] */ BSTR __RPC_FAR *pRet) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LongCopyright( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShortCopyright( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveMMDS( 
            BSTR Path) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Migration( 
            /* [in] */ long Id,
            /* [retval][out] */ IMigration __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Constants( 
            /* [retval][out] */ IConstants __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TestRefers( 
            /* [in] */ long ID,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL FindAll,
            /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pRetArr) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enums( 
            /* [retval][out] */ IMetaEnums __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaData __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaData __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaData __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TaskDef )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ ITaskDef __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modified )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindObject )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ long id,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshMDWnd )( 
            IMetaData __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveMDToFile )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ BSTR path,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL FireEvent,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *retVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Describe )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LongCopyright )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShortCopyright )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveMMDS )( 
            IMetaData __RPC_FAR * This,
            BSTR Path);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Migration )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ long Id,
            /* [retval][out] */ IMigration __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Constants )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ IConstants __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TestRefers )( 
            IMetaData __RPC_FAR * This,
            /* [in] */ long ID,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL FindAll,
            /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pRetArr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Enums )( 
            IMetaData __RPC_FAR * This,
            /* [retval][out] */ IMetaEnums __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaDataVtbl;

    interface IMetaData
    {
        CONST_VTBL struct IMetaDataVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaData_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaData_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaData_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaData_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaData_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaData_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaData_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaData_get_TaskDef(This,pVal)	\
    (This)->lpVtbl -> get_TaskDef(This,pVal)

#define IMetaData_get_Modified(This,pVal)	\
    (This)->lpVtbl -> get_Modified(This,pVal)

#define IMetaData_FindObject(This,id,pObj)	\
    (This)->lpVtbl -> FindObject(This,id,pObj)

#define IMetaData_RefreshMDWnd(This)	\
    (This)->lpVtbl -> RefreshMDWnd(This)

#define IMetaData_SaveMDToFile(This,path,FireEvent,retVal)	\
    (This)->lpVtbl -> SaveMDToFile(This,path,FireEvent,retVal)

#define IMetaData_Describe(This,pRet)	\
    (This)->lpVtbl -> Describe(This,pRet)

#define IMetaData_get_LongCopyright(This,pVal)	\
    (This)->lpVtbl -> get_LongCopyright(This,pVal)

#define IMetaData_get_ShortCopyright(This,pVal)	\
    (This)->lpVtbl -> get_ShortCopyright(This,pVal)

#define IMetaData_SaveMMDS(This,Path)	\
    (This)->lpVtbl -> SaveMMDS(This,Path)

#define IMetaData_get_Migration(This,Id,pVal)	\
    (This)->lpVtbl -> get_Migration(This,Id,pVal)

#define IMetaData_get_Constants(This,pVal)	\
    (This)->lpVtbl -> get_Constants(This,pVal)

#define IMetaData_TestRefers(This,ID,FindAll,pRetArr)	\
    (This)->lpVtbl -> TestRefers(This,ID,FindAll,pRetArr)

#define IMetaData_get_Enums(This,pVal)	\
    (This)->lpVtbl -> get_Enums(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_TaskDef_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ ITaskDef __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_TaskDef_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_Modified_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_Modified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_FindObject_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [in] */ long id,
    /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pObj);


void __RPC_STUB IMetaData_FindObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_RefreshMDWnd_Proxy( 
    IMetaData __RPC_FAR * This);


void __RPC_STUB IMetaData_RefreshMDWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_SaveMDToFile_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [in] */ BSTR path,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL FireEvent,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *retVal);


void __RPC_STUB IMetaData_SaveMDToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_Describe_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pRet);


void __RPC_STUB IMetaData_Describe_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_LongCopyright_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_LongCopyright_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_ShortCopyright_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_ShortCopyright_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_SaveMMDS_Proxy( 
    IMetaData __RPC_FAR * This,
    BSTR Path);


void __RPC_STUB IMetaData_SaveMMDS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_Migration_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [in] */ long Id,
    /* [retval][out] */ IMigration __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_Migration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_Constants_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ IConstants __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_Constants_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaData_TestRefers_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [in] */ long ID,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL FindAll,
    /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pRetArr);


void __RPC_STUB IMetaData_TestRefers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaData_get_Enums_Proxy( 
    IMetaData __RPC_FAR * This,
    /* [retval][out] */ IMetaEnums __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaData_get_Enums_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaData_INTERFACE_DEFINED__ */


#ifndef __IMetaDataObj_INTERFACE_DEFINED__
#define __IMetaDataObj_INTERFACE_DEFINED__

/* interface IMetaDataObj */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IMetaDataObj,0x057D4F98,0xD060,0x4ad6,0xBF,0x96,0xBB,0x3D,0xD1,0x3A,0xA1,0xD7);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("057D4F98-D060-4ad6-BF96-BB3DD13AA1D7")
    IMetaDataObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WhatIsIt( 
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Present( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Present( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Descr( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Descr( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FullName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Props( 
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Childs( 
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ IType1C __RPC_FAR *newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DescribeObject( 
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Parent( 
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaDataObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaDataObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaDataObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaDataObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WhatIsIt )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Present )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Present )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Descr )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Descr )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Props )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Childs )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IMetaDataObj __RPC_FAR * This,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DescribeObject )( 
            IMetaDataObj __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IMetaDataObj __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaDataObjVtbl;

    interface IMetaDataObj
    {
        CONST_VTBL struct IMetaDataObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaDataObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaDataObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaDataObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaDataObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaDataObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaDataObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaDataObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaDataObj_get_WhatIsIt(This,pVal)	\
    (This)->lpVtbl -> get_WhatIsIt(This,pVal)

#define IMetaDataObj_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IMetaDataObj_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IMetaDataObj_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IMetaDataObj_get_Present(This,pVal)	\
    (This)->lpVtbl -> get_Present(This,pVal)

#define IMetaDataObj_put_Present(This,newVal)	\
    (This)->lpVtbl -> put_Present(This,newVal)

#define IMetaDataObj_get_Descr(This,pVal)	\
    (This)->lpVtbl -> get_Descr(This,pVal)

#define IMetaDataObj_put_Descr(This,newVal)	\
    (This)->lpVtbl -> put_Descr(This,newVal)

#define IMetaDataObj_IsValid(This,pRet)	\
    (This)->lpVtbl -> IsValid(This,pRet)

#define IMetaDataObj_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define IMetaDataObj_get_Props(This,pVal)	\
    (This)->lpVtbl -> get_Props(This,pVal)

#define IMetaDataObj_get_Childs(This,pVal)	\
    (This)->lpVtbl -> get_Childs(This,pVal)

#define IMetaDataObj_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IMetaDataObj_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IMetaDataObj_DescribeObject(This,WithChilds,retVal)	\
    (This)->lpVtbl -> DescribeObject(This,WithChilds,retVal)

#define IMetaDataObj_get_Parent(This,pVal)	\
    (This)->lpVtbl -> get_Parent(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_WhatIsIt_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_WhatIsIt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_ID_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Name_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_put_Name_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMetaDataObj_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Present_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Present_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_put_Present_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMetaDataObj_put_Present_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Descr_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Descr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_put_Descr_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMetaDataObj_put_Descr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_IsValid_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);


void __RPC_STUB IMetaDataObj_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_FullName_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_FullName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Props_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Props_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Childs_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Childs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Type_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_put_Type_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [in] */ IType1C __RPC_FAR *newVal);


void __RPC_STUB IMetaDataObj_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_DescribeObject_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
    /* [retval][out] */ BSTR __RPC_FAR *retVal);


void __RPC_STUB IMetaDataObj_DescribeObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObj_get_Parent_Proxy( 
    IMetaDataObj __RPC_FAR * This,
    /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObj_get_Parent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaDataObj_INTERFACE_DEFINED__ */


#ifndef __IMetaDataObjArray_INTERFACE_DEFINED__
#define __IMetaDataObjArray_INTERFACE_DEFINED__

/* interface IMetaDataObjArray */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IMetaDataObjArray,0x9F1E178B,0xC0AD,0x4d9a,0xA0,0x83,0x9A,0x95,0x2F,0x2B,0xDF,0x31);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9F1E178B-C0AD-4d9a-A083-9A952F2BDF31")
    IMetaDataObjArray : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaDataObjArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaDataObjArray __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaDataObjArray __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IMetaDataObjArray __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaDataObjArrayVtbl;

    interface IMetaDataObjArray
    {
        CONST_VTBL struct IMetaDataObjArrayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaDataObjArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaDataObjArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaDataObjArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaDataObjArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaDataObjArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaDataObjArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaDataObjArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaDataObjArray_get_Item(This,idx,pVal)	\
    (This)->lpVtbl -> get_Item(This,idx,pVal)

#define IMetaDataObjArray_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjArray_get_Item_Proxy( 
    IMetaDataObjArray __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjArray_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjArray_get_Count_Proxy( 
    IMetaDataObjArray __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjArray_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaDataObjArray_INTERFACE_DEFINED__ */


#ifndef __ICfgWindows_INTERFACE_DEFINED__
#define __ICfgWindows_INTERFACE_DEFINED__

/* interface ICfgWindows */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ICfgWindows,0x765e4da0,0x4951,0x11d8,0x8d,0x5f,0xad,0xb2,0x8b,0x91,0xf4,0x06);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("765e4da0-4951-11d8-8d5f-adb28b91f406")
    ICfgWindows : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MainWnd( 
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveWnd( 
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ActiveWnd( 
            /* [in] */ ICfgWindow __RPC_FAR *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FirstWnd( 
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NextWnd( 
            /* [in] */ ICfgWindow __RPC_FAR *CurrentWnd,
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PanelVisible( 
            /* [in] */ BSTR Caption,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PanelVisible( 
            /* [in] */ BSTR Caption,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgWindowsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgWindows __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgWindows __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgWindows __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MainWnd )( 
            ICfgWindows __RPC_FAR * This,
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ActiveWnd )( 
            ICfgWindows __RPC_FAR * This,
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ActiveWnd )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ ICfgWindow __RPC_FAR *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FirstWnd )( 
            ICfgWindows __RPC_FAR * This,
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NextWnd )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ ICfgWindow __RPC_FAR *CurrentWnd,
            /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PanelVisible )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ BSTR Caption,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PanelVisible )( 
            ICfgWindows __RPC_FAR * This,
            /* [in] */ BSTR Caption,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } ICfgWindowsVtbl;

    interface ICfgWindows
    {
        CONST_VTBL struct ICfgWindowsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgWindows_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgWindows_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgWindows_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgWindows_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgWindows_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgWindows_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgWindows_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgWindows_get_MainWnd(This,pVal)	\
    (This)->lpVtbl -> get_MainWnd(This,pVal)

#define ICfgWindows_get_ActiveWnd(This,pVal)	\
    (This)->lpVtbl -> get_ActiveWnd(This,pVal)

#define ICfgWindows_put_ActiveWnd(This,newVal)	\
    (This)->lpVtbl -> put_ActiveWnd(This,newVal)

#define ICfgWindows_get_FirstWnd(This,pVal)	\
    (This)->lpVtbl -> get_FirstWnd(This,pVal)

#define ICfgWindows_get_NextWnd(This,CurrentWnd,pVal)	\
    (This)->lpVtbl -> get_NextWnd(This,CurrentWnd,pVal)

#define ICfgWindows_get_PanelVisible(This,Caption,pVal)	\
    (This)->lpVtbl -> get_PanelVisible(This,Caption,pVal)

#define ICfgWindows_put_PanelVisible(This,Caption,newVal)	\
    (This)->lpVtbl -> put_PanelVisible(This,Caption,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindows_get_MainWnd_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgWindows_get_MainWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindows_get_ActiveWnd_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgWindows_get_ActiveWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICfgWindows_put_ActiveWnd_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [in] */ ICfgWindow __RPC_FAR *newVal);


void __RPC_STUB ICfgWindows_put_ActiveWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindows_get_FirstWnd_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgWindows_get_FirstWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindows_get_NextWnd_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [in] */ ICfgWindow __RPC_FAR *CurrentWnd,
    /* [retval][out] */ ICfgWindow __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgWindows_get_NextWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindows_get_PanelVisible_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [in] */ BSTR Caption,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICfgWindows_get_PanelVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICfgWindows_put_PanelVisible_Proxy( 
    ICfgWindows __RPC_FAR * This,
    /* [in] */ BSTR Caption,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ICfgWindows_put_PanelVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgWindows_INTERFACE_DEFINED__ */


#ifndef __IScripts_INTERFACE_DEFINED__
#define __IScripts_INTERFACE_DEFINED__

/* interface IScripts */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IScripts,0x1556e6a0,0xb699,0x4f77,0x94,0x96,0xd7,0x45,0xde,0x37,0x44,0x65);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1556e6a0-b699-4f77-9496-d745de374465")
    IScripts : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ VARIANT Idx,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR Path,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReLoad( 
            /* [in] */ VARIANT Idx) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnLoad( 
            /* [in] */ VARIANT Idx) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Engine( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScripts __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScripts __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScripts __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScripts __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScripts __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScripts __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScripts __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT Idx,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IScripts __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IScripts __RPC_FAR * This,
            /* [in] */ BSTR Path,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReLoad )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT Idx);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnLoad )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT Idx);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Engine )( 
            IScripts __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IScriptsVtbl;

    interface IScripts
    {
        CONST_VTBL struct IScriptsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScripts_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScripts_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScripts_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScripts_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScripts_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScripts_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScripts_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScripts_get_item(This,Idx,pVal)	\
    (This)->lpVtbl -> get_item(This,Idx,pVal)

#define IScripts_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IScripts_Load(This,Path,pRetVal)	\
    (This)->lpVtbl -> Load(This,Path,pRetVal)

#define IScripts_ReLoad(This,Idx)	\
    (This)->lpVtbl -> ReLoad(This,Idx)

#define IScripts_UnLoad(This,Idx)	\
    (This)->lpVtbl -> UnLoad(This,Idx)

#define IScripts_get_Path(This,idx,pVal)	\
    (This)->lpVtbl -> get_Path(This,idx,pVal)

#define IScripts_get_Name(This,idx,pVal)	\
    (This)->lpVtbl -> get_Name(This,idx,pVal)

#define IScripts_get_Engine(This,idx,pVal)	\
    (This)->lpVtbl -> get_Engine(This,idx,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScripts_get_item_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT Idx,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IScripts_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScripts_get_Count_Proxy( 
    IScripts __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IScripts_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IScripts_Load_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ BSTR Path,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pRetVal);


void __RPC_STUB IScripts_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IScripts_ReLoad_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT Idx);


void __RPC_STUB IScripts_ReLoad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IScripts_UnLoad_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT Idx);


void __RPC_STUB IScripts_UnLoad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScripts_get_Path_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScripts_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScripts_get_Name_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScripts_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScripts_get_Engine_Proxy( 
    IScripts __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScripts_get_Engine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScripts_INTERFACE_DEFINED__ */


#ifndef __IPlugins_INTERFACE_DEFINED__
#define __IPlugins_INTERFACE_DEFINED__

/* interface IPlugins */
/* [object][helpstring][nonextensible][dual][version][uuid] */ 


DEFINE_GUID(IID_IPlugins,0x59de83b2,0xa571,0x4a2e,0x9c,0x84,0xda,0x78,0x9b,0x58,0x58,0xa6);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59de83b2-a571-4a2e-9c84-da789b5858a6")
    IPlugins : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ VARIANT Idx,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadPlugin( 
            /* [in] */ BSTR From) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPluginsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPlugins __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPlugins __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPlugins __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ VARIANT Idx,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IPlugins __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadPlugin )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ BSTR From);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IPlugins __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IPluginsVtbl;

    interface IPlugins
    {
        CONST_VTBL struct IPluginsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlugins_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPlugins_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPlugins_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPlugins_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPlugins_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPlugins_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPlugins_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPlugins_get_item(This,Idx,pVal)	\
    (This)->lpVtbl -> get_item(This,Idx,pVal)

#define IPlugins_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IPlugins_LoadPlugin(This,From)	\
    (This)->lpVtbl -> LoadPlugin(This,From)

#define IPlugins_get_Path(This,idx,pVal)	\
    (This)->lpVtbl -> get_Path(This,idx,pVal)

#define IPlugins_get_Name(This,idx,pVal)	\
    (This)->lpVtbl -> get_Name(This,idx,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugins_get_item_Proxy( 
    IPlugins __RPC_FAR * This,
    /* [in] */ VARIANT Idx,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IPlugins_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugins_get_Count_Proxy( 
    IPlugins __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IPlugins_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPlugins_LoadPlugin_Proxy( 
    IPlugins __RPC_FAR * This,
    /* [in] */ BSTR From);


void __RPC_STUB IPlugins_LoadPlugin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugins_get_Path_Proxy( 
    IPlugins __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IPlugins_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPlugins_get_Name_Proxy( 
    IPlugins __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IPlugins_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPlugins_INTERFACE_DEFINED__ */


#ifndef __ITaskDef_INTERFACE_DEFINED__
#define __ITaskDef_INTERFACE_DEFINED__

/* interface ITaskDef */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_ITaskDef,0xb6b67039,0x2433,0x4151,0xb3,0x44,0x6a,0xd3,0xce,0x0d,0x4a,0x77);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b6b67039-2433-4151-b344-6ad3ce0d4a77")
    ITaskDef : public IMetaDataObj
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Language( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ImmediateDelete( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ImmediateDelete( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Round15_2( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Round15_2( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskDefVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITaskDef __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITaskDef __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITaskDef __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WhatIsIt )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Present )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Present )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Descr )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Descr )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Props )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Childs )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DescribeObject )( 
            ITaskDef __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Language )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Language )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ImmediateDelete )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ImmediateDelete )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Round15_2 )( 
            ITaskDef __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Round15_2 )( 
            ITaskDef __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } ITaskDefVtbl;

    interface ITaskDef
    {
        CONST_VTBL struct ITaskDefVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskDef_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITaskDef_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITaskDef_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITaskDef_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITaskDef_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITaskDef_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITaskDef_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITaskDef_get_WhatIsIt(This,pVal)	\
    (This)->lpVtbl -> get_WhatIsIt(This,pVal)

#define ITaskDef_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ITaskDef_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ITaskDef_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define ITaskDef_get_Present(This,pVal)	\
    (This)->lpVtbl -> get_Present(This,pVal)

#define ITaskDef_put_Present(This,newVal)	\
    (This)->lpVtbl -> put_Present(This,newVal)

#define ITaskDef_get_Descr(This,pVal)	\
    (This)->lpVtbl -> get_Descr(This,pVal)

#define ITaskDef_put_Descr(This,newVal)	\
    (This)->lpVtbl -> put_Descr(This,newVal)

#define ITaskDef_IsValid(This,pRet)	\
    (This)->lpVtbl -> IsValid(This,pRet)

#define ITaskDef_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define ITaskDef_get_Props(This,pVal)	\
    (This)->lpVtbl -> get_Props(This,pVal)

#define ITaskDef_get_Childs(This,pVal)	\
    (This)->lpVtbl -> get_Childs(This,pVal)

#define ITaskDef_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ITaskDef_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define ITaskDef_DescribeObject(This,WithChilds,retVal)	\
    (This)->lpVtbl -> DescribeObject(This,WithChilds,retVal)

#define ITaskDef_get_Parent(This,pVal)	\
    (This)->lpVtbl -> get_Parent(This,pVal)


#define ITaskDef_get_Language(This,pVal)	\
    (This)->lpVtbl -> get_Language(This,pVal)

#define ITaskDef_put_Language(This,newVal)	\
    (This)->lpVtbl -> put_Language(This,newVal)

#define ITaskDef_get_ImmediateDelete(This,pVal)	\
    (This)->lpVtbl -> get_ImmediateDelete(This,pVal)

#define ITaskDef_put_ImmediateDelete(This,newVal)	\
    (This)->lpVtbl -> put_ImmediateDelete(This,newVal)

#define ITaskDef_get_Round15_2(This,pVal)	\
    (This)->lpVtbl -> get_Round15_2(This,pVal)

#define ITaskDef_put_Round15_2(This,newVal)	\
    (This)->lpVtbl -> put_Round15_2(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITaskDef_get_Language_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITaskDef_get_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITaskDef_put_Language_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ITaskDef_put_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITaskDef_get_ImmediateDelete_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITaskDef_get_ImmediateDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITaskDef_put_ImmediateDelete_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITaskDef_put_ImmediateDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITaskDef_get_Round15_2_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITaskDef_get_Round15_2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITaskDef_put_Round15_2_Proxy( 
    ITaskDef __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITaskDef_put_Round15_2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITaskDef_INTERFACE_DEFINED__ */


#ifndef __IConstants_INTERFACE_DEFINED__
#define __IConstants_INTERFACE_DEFINED__

/* interface IConstants */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IConstants,0x30869711,0x4da3,0x445e,0x86,0x07,0x19,0x53,0xb0,0x85,0x0c,0x46);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("30869711-4da3-445e-8607-1953b0850c46")
    IConstants : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR name,
            /* [defaultvalue][optional][in] */ BSTR Present,
            /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT idx) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConstantsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConstants __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConstants __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConstants __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConstants __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConstants __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConstants __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConstants __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IConstants __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IConstants __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IConstants __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IConstants __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [defaultvalue][optional][in] */ BSTR Present,
            /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IConstants __RPC_FAR * This,
            /* [in] */ VARIANT idx);
        
        END_INTERFACE
    } IConstantsVtbl;

    interface IConstants
    {
        CONST_VTBL struct IConstantsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConstants_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConstants_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConstants_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConstants_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConstants_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConstants_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConstants_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConstants_get_item(This,idx,pVal)	\
    (This)->lpVtbl -> get_item(This,idx,pVal)

#define IConstants_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IConstants_get_Name(This,idx,pVal)	\
    (This)->lpVtbl -> get_Name(This,idx,pVal)

#define IConstants_Add(This,name,Present,pRetVal)	\
    (This)->lpVtbl -> Add(This,name,Present,pRetVal)

#define IConstants_Remove(This,idx)	\
    (This)->lpVtbl -> Remove(This,idx)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConstants_get_item_Proxy( 
    IConstants __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConstants_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConstants_get_Count_Proxy( 
    IConstants __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IConstants_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConstants_get_Name_Proxy( 
    IConstants __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConstants_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConstants_Add_Proxy( 
    IConstants __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [defaultvalue][optional][in] */ BSTR Present,
    /* [retval][out] */ IConstDef __RPC_FAR *__RPC_FAR *pRetVal);


void __RPC_STUB IConstants_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConstants_Remove_Proxy( 
    IConstants __RPC_FAR * This,
    /* [in] */ VARIANT idx);


void __RPC_STUB IConstants_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConstants_INTERFACE_DEFINED__ */


#ifndef __ICfgDocuments_INTERFACE_DEFINED__
#define __ICfgDocuments_INTERFACE_DEFINED__

/* interface ICfgDocuments */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ICfgDocuments,0x16623674,0xecad,0x484b,0xa3,0x99,0x75,0x12,0x29,0x94,0x0c,0xdc);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("16623674-ecad-484b-a399-751229940cdc")
    ICfgDocuments : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ BSTR name,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR Path,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DocFromID( 
            /* [in] */ long id,
            /* [in] */ DocTypes type,
            /* [in] */ BSTR Path,
            /* [in] */ BSTR kind,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pRetDoc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE New( 
            /* [in] */ DocTypes type,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumAllTypedObj( 
            /* [retval][out] */ IAllTypedObj __RPC_FAR *__RPC_FAR *pRet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgDocumentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgDocuments __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgDocuments __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgDocuments __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ BSTR Path,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DocFromID )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ long id,
            /* [in] */ DocTypes type,
            /* [in] */ BSTR Path,
            /* [in] */ BSTR kind,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pRetDoc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *New )( 
            ICfgDocuments __RPC_FAR * This,
            /* [in] */ DocTypes type,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumAllTypedObj )( 
            ICfgDocuments __RPC_FAR * This,
            /* [retval][out] */ IAllTypedObj __RPC_FAR *__RPC_FAR *pRet);
        
        END_INTERFACE
    } ICfgDocumentsVtbl;

    interface ICfgDocuments
    {
        CONST_VTBL struct ICfgDocumentsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgDocuments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgDocuments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgDocuments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgDocuments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgDocuments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgDocuments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgDocuments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgDocuments_get_item(This,name,pVal)	\
    (This)->lpVtbl -> get_item(This,name,pVal)

#define ICfgDocuments_Open(This,Path,pDoc)	\
    (This)->lpVtbl -> Open(This,Path,pDoc)

#define ICfgDocuments_DocFromID(This,id,type,Path,kind,pRetDoc)	\
    (This)->lpVtbl -> DocFromID(This,id,type,Path,kind,pRetDoc)

#define ICfgDocuments_New(This,type,pDoc)	\
    (This)->lpVtbl -> New(This,type,pDoc)

#define ICfgDocuments_EnumAllTypedObj(This,pRet)	\
    (This)->lpVtbl -> EnumAllTypedObj(This,pRet)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDocuments_get_item_Proxy( 
    ICfgDocuments __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgDocuments_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDocuments_Open_Proxy( 
    ICfgDocuments __RPC_FAR * This,
    /* [in] */ BSTR Path,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc);


void __RPC_STUB ICfgDocuments_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDocuments_DocFromID_Proxy( 
    ICfgDocuments __RPC_FAR * This,
    /* [in] */ long id,
    /* [in] */ DocTypes type,
    /* [in] */ BSTR Path,
    /* [in] */ BSTR kind,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pRetDoc);


void __RPC_STUB ICfgDocuments_DocFromID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDocuments_New_Proxy( 
    ICfgDocuments __RPC_FAR * This,
    /* [in] */ DocTypes type,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pDoc);


void __RPC_STUB ICfgDocuments_New_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDocuments_EnumAllTypedObj_Proxy( 
    ICfgDocuments __RPC_FAR * This,
    /* [retval][out] */ IAllTypedObj __RPC_FAR *__RPC_FAR *pRet);


void __RPC_STUB ICfgDocuments_EnumAllTypedObj_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgDocuments_INTERFACE_DEFINED__ */


#ifndef __ICfgDoc_INTERFACE_DEFINED__
#define __ICfgDoc_INTERFACE_DEFINED__

/* interface ICfgDoc */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ICfgDoc,0xd1c2bb80,0x7206,0x11d8,0x9f,0xcd,0xe9,0x05,0x38,0x13,0xda,0x08);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d1c2bb80-7206-11d8-9fcd-e9053813da08")
    ICfgDoc : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ DocTypes __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Kind( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsOpen( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgDocVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgDoc __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgDoc __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgDoc __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgDoc __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgDoc __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ DocTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Kind )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ICfgDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOpen )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            ICfgDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ICfgDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } ICfgDocVtbl;

    interface ICfgDoc
    {
        CONST_VTBL struct ICfgDocVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgDoc_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgDoc_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgDoc_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgDoc_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgDoc_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ICfgDoc_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ICfgDoc_get_Kind(This,pVal)	\
    (This)->lpVtbl -> get_Kind(This,pVal)

#define ICfgDoc_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define ICfgDoc_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define ICfgDoc_get_IsOpen(This,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,pVal)

#define ICfgDoc_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define ICfgDoc_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_Type_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ DocTypes __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_ID_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_Kind_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_Kind_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_Path_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDoc_Open_Proxy( 
    ICfgDoc __RPC_FAR * This);


void __RPC_STUB ICfgDoc_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_IsOpen_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_IsOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgDoc_Save_Proxy( 
    ICfgDoc __RPC_FAR * This);


void __RPC_STUB ICfgDoc_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgDoc_get_Name_Proxy( 
    ICfgDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICfgDoc_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgDoc_INTERFACE_DEFINED__ */


#ifndef __ICfgWindow_INTERFACE_DEFINED__
#define __ICfgWindow_INTERFACE_DEFINED__

/* interface ICfgWindow */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ICfgWindow,0x6e8f8c6a,0xb9fb,0x46fc,0xaf,0xfa,0xcb,0x0f,0x7e,0xb9,0x3f,0xb9);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6e8f8c6a-b9fb-46fc-affa-cb0f7eb93fb9")
    ICfgWindow : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_hWnd( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Document( 
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Maximized( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Maximized( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Minimized( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Minimized( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bottom( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Right( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ long left,
            /* [in] */ long top,
            /* [in] */ long right,
            /* [in] */ long bottom) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ClientWidth( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ClientHeight( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgWindow __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgWindow __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgWindow __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Caption )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Caption )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Document )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            ICfgWindow __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Maximized )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Maximized )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Minimized )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Minimized )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Top )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Left )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bottom )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Right )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Move )( 
            ICfgWindow __RPC_FAR * This,
            /* [in] */ long left,
            /* [in] */ long top,
            /* [in] */ long right,
            /* [in] */ long bottom);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ClientWidth )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ClientHeight )( 
            ICfgWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } ICfgWindowVtbl;

    interface ICfgWindow
    {
        CONST_VTBL struct ICfgWindowVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgWindow_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgWindow_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgWindow_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgWindow_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgWindow_get_hWnd(This,pVal)	\
    (This)->lpVtbl -> get_hWnd(This,pVal)

#define ICfgWindow_get_Caption(This,pVal)	\
    (This)->lpVtbl -> get_Caption(This,pVal)

#define ICfgWindow_put_Caption(This,newVal)	\
    (This)->lpVtbl -> put_Caption(This,newVal)

#define ICfgWindow_get_Document(This,pVal)	\
    (This)->lpVtbl -> get_Document(This,pVal)

#define ICfgWindow_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define ICfgWindow_get_Maximized(This,pVal)	\
    (This)->lpVtbl -> get_Maximized(This,pVal)

#define ICfgWindow_put_Maximized(This,newVal)	\
    (This)->lpVtbl -> put_Maximized(This,newVal)

#define ICfgWindow_get_Minimized(This,pVal)	\
    (This)->lpVtbl -> get_Minimized(This,pVal)

#define ICfgWindow_put_Minimized(This,newVal)	\
    (This)->lpVtbl -> put_Minimized(This,newVal)

#define ICfgWindow_get_Top(This,pVal)	\
    (This)->lpVtbl -> get_Top(This,pVal)

#define ICfgWindow_get_Left(This,pVal)	\
    (This)->lpVtbl -> get_Left(This,pVal)

#define ICfgWindow_get_Bottom(This,pVal)	\
    (This)->lpVtbl -> get_Bottom(This,pVal)

#define ICfgWindow_get_Right(This,pVal)	\
    (This)->lpVtbl -> get_Right(This,pVal)

#define ICfgWindow_Move(This,left,top,right,bottom)	\
    (This)->lpVtbl -> Move(This,left,top,right,bottom)

#define ICfgWindow_get_ClientWidth(This,pVal)	\
    (This)->lpVtbl -> get_ClientWidth(This,pVal)

#define ICfgWindow_get_ClientHeight(This,pVal)	\
    (This)->lpVtbl -> get_ClientHeight(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_hWnd_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_hWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Caption_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Caption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICfgWindow_put_Caption_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ICfgWindow_put_Caption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Document_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Document_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgWindow_Close_Proxy( 
    ICfgWindow __RPC_FAR * This);


void __RPC_STUB ICfgWindow_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Maximized_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Maximized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICfgWindow_put_Maximized_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ICfgWindow_put_Maximized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Minimized_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Minimized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICfgWindow_put_Minimized_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ICfgWindow_put_Minimized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Top_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Left_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Bottom_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Bottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_Right_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_Right_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgWindow_Move_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [in] */ long left,
    /* [in] */ long top,
    /* [in] */ long right,
    /* [in] */ long bottom);


void __RPC_STUB ICfgWindow_Move_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_ClientWidth_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_ClientWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgWindow_get_ClientHeight_Proxy( 
    ICfgWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgWindow_get_ClientHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgWindow_INTERFACE_DEFINED__ */


#ifndef __ICfgTimer_INTERFACE_DEFINED__
#define __ICfgTimer_INTERFACE_DEFINED__

/* interface ICfgTimer */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_ICfgTimer,0x04539fb9,0x4790,0x4556,0x90,0x55,0xda,0x5d,0x10,0xcf,0xa0,0xa5);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("04539fb9-4790-4556-9055-da5d10cfa0a5")
    ICfgTimer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Time( 
            /* [in] */ long Num,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTimer( 
            /* [in] */ long Interval,
            /* [in] */ VARIANT_BOOL OneTimeOnly,
            /* [retval][out] */ long __RPC_FAR *TimerID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KillTimer( 
            /* [in] */ long TimerID) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KillAll( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [in] */ long Num,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgTimerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgTimer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgTimer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgTimer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Time )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ long Num,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTimer )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ long Interval,
            /* [in] */ VARIANT_BOOL OneTimeOnly,
            /* [retval][out] */ long __RPC_FAR *TimerID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *KillTimer )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ long TimerID);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ICfgTimer __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *KillAll )( 
            ICfgTimer __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ICfgTimer __RPC_FAR * This,
            /* [in] */ long Num,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } ICfgTimerVtbl;

    interface ICfgTimer
    {
        CONST_VTBL struct ICfgTimerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgTimer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgTimer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgTimer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgTimer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgTimer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgTimer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgTimer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgTimer_get_Time(This,Num,pVal)	\
    (This)->lpVtbl -> get_Time(This,Num,pVal)

#define ICfgTimer_SetTimer(This,Interval,OneTimeOnly,TimerID)	\
    (This)->lpVtbl -> SetTimer(This,Interval,OneTimeOnly,TimerID)

#define ICfgTimer_KillTimer(This,TimerID)	\
    (This)->lpVtbl -> KillTimer(This,TimerID)

#define ICfgTimer_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define ICfgTimer_KillAll(This)	\
    (This)->lpVtbl -> KillAll(This)

#define ICfgTimer_get_ID(This,Num,pVal)	\
    (This)->lpVtbl -> get_ID(This,Num,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgTimer_get_Time_Proxy( 
    ICfgTimer __RPC_FAR * This,
    /* [in] */ long Num,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgTimer_get_Time_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgTimer_SetTimer_Proxy( 
    ICfgTimer __RPC_FAR * This,
    /* [in] */ long Interval,
    /* [in] */ VARIANT_BOOL OneTimeOnly,
    /* [retval][out] */ long __RPC_FAR *TimerID);


void __RPC_STUB ICfgTimer_SetTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgTimer_KillTimer_Proxy( 
    ICfgTimer __RPC_FAR * This,
    /* [in] */ long TimerID);


void __RPC_STUB ICfgTimer_KillTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgTimer_get_Count_Proxy( 
    ICfgTimer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgTimer_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgTimer_KillAll_Proxy( 
    ICfgTimer __RPC_FAR * This);


void __RPC_STUB ICfgTimer_KillAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICfgTimer_get_ID_Proxy( 
    ICfgTimer __RPC_FAR * This,
    /* [in] */ long Num,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ICfgTimer_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgTimer_INTERFACE_DEFINED__ */


#ifndef __IAllTypedObj_INTERFACE_DEFINED__
#define __IAllTypedObj_INTERFACE_DEFINED__

/* interface IAllTypedObj */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IAllTypedObj,0x19484b30,0x4771,0x453b,0xaf,0x31,0x5e,0xe0,0x2d,0xe5,0x44,0xd8);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19484b30-4771-453b-af31-5ee02de544d8")
    IAllTypedObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            VARIANT idx,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ChildsCount( 
            BSTR Parent,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ChildName( 
            BSTR Parent,
            long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectCount( 
            BSTR Parent,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAllTypedObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAllTypedObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAllTypedObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAllTypedObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAllTypedObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAllTypedObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAllTypedObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAllTypedObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IAllTypedObj __RPC_FAR * This,
            VARIANT idx,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IAllTypedObj __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChildsCount )( 
            IAllTypedObj __RPC_FAR * This,
            BSTR Parent,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ChildName )( 
            IAllTypedObj __RPC_FAR * This,
            BSTR Parent,
            long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ObjectCount )( 
            IAllTypedObj __RPC_FAR * This,
            BSTR Parent,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } IAllTypedObjVtbl;

    interface IAllTypedObj
    {
        CONST_VTBL struct IAllTypedObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAllTypedObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAllTypedObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAllTypedObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAllTypedObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAllTypedObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAllTypedObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAllTypedObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAllTypedObj_get_item(This,idx,pVal)	\
    (This)->lpVtbl -> get_item(This,idx,pVal)

#define IAllTypedObj_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IAllTypedObj_get_ChildsCount(This,Parent,pVal)	\
    (This)->lpVtbl -> get_ChildsCount(This,Parent,pVal)

#define IAllTypedObj_get_ChildName(This,Parent,idx,pVal)	\
    (This)->lpVtbl -> get_ChildName(This,Parent,idx,pVal)

#define IAllTypedObj_get_ObjectCount(This,Parent,pVal)	\
    (This)->lpVtbl -> get_ObjectCount(This,Parent,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAllTypedObj_get_item_Proxy( 
    IAllTypedObj __RPC_FAR * This,
    VARIANT idx,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IAllTypedObj_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAllTypedObj_get_Count_Proxy( 
    IAllTypedObj __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IAllTypedObj_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAllTypedObj_get_ChildsCount_Proxy( 
    IAllTypedObj __RPC_FAR * This,
    BSTR Parent,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IAllTypedObj_get_ChildsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAllTypedObj_get_ChildName_Proxy( 
    IAllTypedObj __RPC_FAR * This,
    BSTR Parent,
    long idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IAllTypedObj_get_ChildName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAllTypedObj_get_ObjectCount_Proxy( 
    IAllTypedObj __RPC_FAR * This,
    BSTR Parent,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IAllTypedObj_get_ObjectCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAllTypedObj_INTERFACE_DEFINED__ */


#ifndef __IMDWindow_INTERFACE_DEFINED__
#define __IMDWindow_INTERFACE_DEFINED__

/* interface IMDWindow */
/* [object][helpstring][nonextensible][dual][version][uuid] */ 


DEFINE_GUID(IID_IMDWindow,0xa0c9c161,0x59bd,0x42d4,0x95,0xed,0x0d,0x42,0x75,0xcb,0x82,0x0f);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a0c9c161-59bd-42d4-95ed-0d4275cb820f")
    IMDWindow : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TabCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TabName( 
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveTab( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ActiveTab( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoAction( 
            /* [in] */ BSTR PathInTree,
            /* [defaultvalue][optional][in] */ mdActions Action = mdaEdit) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_hWnd( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSelected( 
            /* [retval][out] */ BSTR __RPC_FAR *Path) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMDWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMDWindow __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMDWindow __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMDWindow __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            IMDWindow __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TabCount )( 
            IMDWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TabName )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ActiveTab )( 
            IMDWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ActiveTab )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoAction )( 
            IMDWindow __RPC_FAR * This,
            /* [in] */ BSTR PathInTree,
            /* [defaultvalue][optional][in] */ mdActions Action);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )( 
            IMDWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelected )( 
            IMDWindow __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *Path);
        
        END_INTERFACE
    } IMDWindowVtbl;

    interface IMDWindow
    {
        CONST_VTBL struct IMDWindowVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMDWindow_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMDWindow_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMDWindow_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMDWindow_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMDWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMDWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMDWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMDWindow_Activate(This)	\
    (This)->lpVtbl -> Activate(This)

#define IMDWindow_get_TabCount(This,pVal)	\
    (This)->lpVtbl -> get_TabCount(This,pVal)

#define IMDWindow_get_TabName(This,idx,pVal)	\
    (This)->lpVtbl -> get_TabName(This,idx,pVal)

#define IMDWindow_get_ActiveTab(This,pVal)	\
    (This)->lpVtbl -> get_ActiveTab(This,pVal)

#define IMDWindow_put_ActiveTab(This,newVal)	\
    (This)->lpVtbl -> put_ActiveTab(This,newVal)

#define IMDWindow_DoAction(This,PathInTree,Action)	\
    (This)->lpVtbl -> DoAction(This,PathInTree,Action)

#define IMDWindow_get_hWnd(This,pVal)	\
    (This)->lpVtbl -> get_hWnd(This,pVal)

#define IMDWindow_GetSelected(This,Path)	\
    (This)->lpVtbl -> GetSelected(This,Path)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDWindow_Activate_Proxy( 
    IMDWindow __RPC_FAR * This);


void __RPC_STUB IMDWindow_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDWindow_get_TabCount_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMDWindow_get_TabCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDWindow_get_TabName_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [in] */ long idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMDWindow_get_TabName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDWindow_get_ActiveTab_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMDWindow_get_ActiveTab_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMDWindow_put_ActiveTab_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IMDWindow_put_ActiveTab_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDWindow_DoAction_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [in] */ BSTR PathInTree,
    /* [defaultvalue][optional][in] */ mdActions Action);


void __RPC_STUB IMDWindow_DoAction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMDWindow_get_hWnd_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMDWindow_get_hWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMDWindow_GetSelected_Proxy( 
    IMDWindow __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *Path);


void __RPC_STUB IMDWindow_GetSelected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMDWindow_INTERFACE_DEFINED__ */


#ifndef __IDialogDoc_INTERFACE_DEFINED__
#define __IDialogDoc_INTERFACE_DEFINED__

/* interface IDialogDoc */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IDialogDoc,0x2e1ccc5f,0x1fc5,0x41a0,0x9b,0xf7,0xf4,0xb7,0x70,0x33,0xa2,0x3c);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2e1ccc5f-1fc5-41a0-9bf7-f4b77033a23c")
    IDialogDoc : public ICfgDoc
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Stream( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Stream( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLayer( 
            /* [in] */ BSTR Name,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Visible = -1) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LayerCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LayerName( 
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LayerName( 
            /* [in] */ long idx,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LayerVisible( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LayerVisible( 
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveLayer( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ActiveLayer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteLayer( 
            /* [in] */ BSTR Name,
            /* [defaultvalue][optional][in] */ BSTR CopyControlsTo = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveLayer( 
            /* [in] */ BSTR From,
            /* [in] */ VARIANT_BOOL Down) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ctrlCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ctrlType( 
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ctrlUID( 
            /* [in] */ long idx,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ctrlIdx( 
            /* [in] */ long UID,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Selection( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Selection( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ctrlProp( 
            /* [in] */ long ctrlIdx,
            /* [in] */ CtrlPropsIdx propIdx,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ctrlProp( 
            /* [in] */ long ctrlIdx,
            /* [in] */ CtrlPropsIdx propIdx,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_formProp( 
            /* [in] */ FormPropsIdx formPropIdx,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_formProp( 
            /* [in] */ FormPropsIdx formPropIdx,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ctrlType1C( 
            /* [in] */ long idx,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ctrlType1C( 
            /* [in] */ long idx,
            /* [in] */ IType1C __RPC_FAR *newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDialogDocVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDialogDoc __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDialogDoc __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDialogDoc __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ DocTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Kind )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IDialogDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOpen )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IDialogDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToFile )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadFromFile )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Stream )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Stream )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddLayer )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Visible);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayerCount )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayerName )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LayerName )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayerVisible )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LayerVisible )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ActiveLayer )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ActiveLayer )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteLayer )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [defaultvalue][optional][in] */ BSTR CopyControlsTo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveLayer )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR From,
            /* [in] */ VARIANT_BOOL Down);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ctrlCount )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ctrlType )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ctrlUID )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ctrlIdx )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long UID,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Selection )( 
            IDialogDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Selection )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ctrlProp )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long ctrlIdx,
            /* [in] */ CtrlPropsIdx propIdx,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ctrlProp )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long ctrlIdx,
            /* [in] */ CtrlPropsIdx propIdx,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_formProp )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ FormPropsIdx formPropIdx,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_formProp )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ FormPropsIdx formPropIdx,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ctrlType1C )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ctrlType1C )( 
            IDialogDoc __RPC_FAR * This,
            /* [in] */ long idx,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        END_INTERFACE
    } IDialogDocVtbl;

    interface IDialogDoc
    {
        CONST_VTBL struct IDialogDocVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDialogDoc_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDialogDoc_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDialogDoc_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDialogDoc_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDialogDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDialogDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDialogDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDialogDoc_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IDialogDoc_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IDialogDoc_get_Kind(This,pVal)	\
    (This)->lpVtbl -> get_Kind(This,pVal)

#define IDialogDoc_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IDialogDoc_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define IDialogDoc_get_IsOpen(This,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,pVal)

#define IDialogDoc_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IDialogDoc_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)


#define IDialogDoc_SaveToFile(This,FileName,bSucces)	\
    (This)->lpVtbl -> SaveToFile(This,FileName,bSucces)

#define IDialogDoc_LoadFromFile(This,FileName,bSuccess)	\
    (This)->lpVtbl -> LoadFromFile(This,FileName,bSuccess)

#define IDialogDoc_get_Stream(This,pVal)	\
    (This)->lpVtbl -> get_Stream(This,pVal)

#define IDialogDoc_put_Stream(This,newVal)	\
    (This)->lpVtbl -> put_Stream(This,newVal)

#define IDialogDoc_AddLayer(This,Name,Visible)	\
    (This)->lpVtbl -> AddLayer(This,Name,Visible)

#define IDialogDoc_get_LayerCount(This,pVal)	\
    (This)->lpVtbl -> get_LayerCount(This,pVal)

#define IDialogDoc_get_LayerName(This,idx,pVal)	\
    (This)->lpVtbl -> get_LayerName(This,idx,pVal)

#define IDialogDoc_put_LayerName(This,idx,newVal)	\
    (This)->lpVtbl -> put_LayerName(This,idx,newVal)

#define IDialogDoc_get_LayerVisible(This,Name,pVal)	\
    (This)->lpVtbl -> get_LayerVisible(This,Name,pVal)

#define IDialogDoc_put_LayerVisible(This,Name,newVal)	\
    (This)->lpVtbl -> put_LayerVisible(This,Name,newVal)

#define IDialogDoc_get_ActiveLayer(This,pVal)	\
    (This)->lpVtbl -> get_ActiveLayer(This,pVal)

#define IDialogDoc_put_ActiveLayer(This,newVal)	\
    (This)->lpVtbl -> put_ActiveLayer(This,newVal)

#define IDialogDoc_DeleteLayer(This,Name,CopyControlsTo)	\
    (This)->lpVtbl -> DeleteLayer(This,Name,CopyControlsTo)

#define IDialogDoc_MoveLayer(This,From,Down)	\
    (This)->lpVtbl -> MoveLayer(This,From,Down)

#define IDialogDoc_get_ctrlCount(This,pVal)	\
    (This)->lpVtbl -> get_ctrlCount(This,pVal)

#define IDialogDoc_get_ctrlType(This,idx,pVal)	\
    (This)->lpVtbl -> get_ctrlType(This,idx,pVal)

#define IDialogDoc_ctrlUID(This,idx,pVal)	\
    (This)->lpVtbl -> ctrlUID(This,idx,pVal)

#define IDialogDoc_ctrlIdx(This,UID,pVal)	\
    (This)->lpVtbl -> ctrlIdx(This,UID,pVal)

#define IDialogDoc_get_Selection(This,pVal)	\
    (This)->lpVtbl -> get_Selection(This,pVal)

#define IDialogDoc_put_Selection(This,newVal)	\
    (This)->lpVtbl -> put_Selection(This,newVal)

#define IDialogDoc_get_ctrlProp(This,ctrlIdx,propIdx,pVal)	\
    (This)->lpVtbl -> get_ctrlProp(This,ctrlIdx,propIdx,pVal)

#define IDialogDoc_put_ctrlProp(This,ctrlIdx,propIdx,newVal)	\
    (This)->lpVtbl -> put_ctrlProp(This,ctrlIdx,propIdx,newVal)

#define IDialogDoc_get_formProp(This,formPropIdx,pVal)	\
    (This)->lpVtbl -> get_formProp(This,formPropIdx,pVal)

#define IDialogDoc_put_formProp(This,formPropIdx,newVal)	\
    (This)->lpVtbl -> put_formProp(This,formPropIdx,newVal)

#define IDialogDoc_get_ctrlType1C(This,idx,pVal)	\
    (This)->lpVtbl -> get_ctrlType1C(This,idx,pVal)

#define IDialogDoc_put_ctrlType1C(This,idx,newVal)	\
    (This)->lpVtbl -> put_ctrlType1C(This,idx,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_SaveToFile_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);


void __RPC_STUB IDialogDoc_SaveToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_LoadFromFile_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);


void __RPC_STUB IDialogDoc_LoadFromFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_Stream_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_Stream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_Stream_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDialogDoc_put_Stream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_AddLayer_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL Visible);


void __RPC_STUB IDialogDoc_AddLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_LayerCount_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_LayerCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_LayerName_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_LayerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_LayerName_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDialogDoc_put_LayerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_LayerVisible_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_LayerVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_LayerVisible_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IDialogDoc_put_LayerVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_ActiveLayer_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_ActiveLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_ActiveLayer_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDialogDoc_put_ActiveLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_DeleteLayer_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [defaultvalue][optional][in] */ BSTR CopyControlsTo);


void __RPC_STUB IDialogDoc_DeleteLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_MoveLayer_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR From,
    /* [in] */ VARIANT_BOOL Down);


void __RPC_STUB IDialogDoc_MoveLayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_ctrlCount_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_ctrlCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_ctrlType_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_ctrlType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_ctrlUID_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_ctrlUID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDialogDoc_ctrlIdx_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long UID,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_ctrlIdx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_Selection_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_Selection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_Selection_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDialogDoc_put_Selection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_ctrlProp_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long ctrlIdx,
    /* [in] */ CtrlPropsIdx propIdx,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_ctrlProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_ctrlProp_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long ctrlIdx,
    /* [in] */ CtrlPropsIdx propIdx,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IDialogDoc_put_ctrlProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_formProp_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ FormPropsIdx formPropIdx,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_formProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_formProp_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ FormPropsIdx formPropIdx,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IDialogDoc_put_formProp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDialogDoc_get_ctrlType1C_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IDialogDoc_get_ctrlType1C_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDialogDoc_put_ctrlType1C_Proxy( 
    IDialogDoc __RPC_FAR * This,
    /* [in] */ long idx,
    /* [in] */ IType1C __RPC_FAR *newVal);


void __RPC_STUB IDialogDoc_put_ctrlType1C_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDialogDoc_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MDWindow,0xcfaedc45,0x305a,0x4a97,0xb2,0x1b,0x89,0x14,0x85,0x33,0xeb,0x21);

#ifdef __cplusplus

class DECLSPEC_UUID("cfaedc45-305a-4a97-b21b-89148533eb21")
MDWindow;
#endif

#ifndef __IConfigurator_INTERFACE_DEFINED__
#define __IConfigurator_INTERFACE_DEFINED__

/* interface IConfigurator */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IConfigurator,0xFF862867,0xBB20,0x44b2,0x94,0x9C,0x5E,0xA7,0xC0,0x98,0x42,0xEE);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF862867-BB20-44b2-949C-5EA7C09842EE")
    IConfigurator : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IBName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IBName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IBDir( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BinDir( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CommandLine( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Message( 
            /* [in] */ BSTR msgLine,
            /* [defaultvalue][optional][in] */ MsgLineMarkers marker = mNone) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Status( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenOLEForm( 
            /* [in] */ BSTR progID,
            /* [in] */ BSTR title,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MetaData( 
            /* [retval][out] */ IMetaData __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendCommand( 
            /* [in] */ ConfigCommands cmd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsConfigWndOpen( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pResult) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Plugins( 
            /* [retval][out] */ IPlugins __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Scripts( 
            /* [retval][out] */ IScripts __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Windows( 
            /* [retval][out] */ ICfgWindows __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Quit( 
            /* [defaultvalue][optional][in] */ VARIANT_BOOL PromptSave = -1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LogMessageWrite( 
            /* [in] */ EventCategories EventCategory,
            /* [in] */ BSTR Message,
            /* [defaultvalue][optional][in] */ BSTR strEventType = L"User",
            /* [defaultvalue][optional][in] */ BSTR Event = L"UserMsg") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run1CApp( 
            /* [defaultvalue][optional][in] */ RunAppModes mode = rmEnterprise) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Documents( 
            /* [retval][out] */ ICfgDocuments __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CfgTimer( 
            /* [retval][out] */ ICfgTimer __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AppProps( 
            /* [in] */ AppProperties idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MDWnd( 
            /* [retval][out] */ IMDWindow __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CancelHotKey( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CancelHotKey( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfiguratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigurator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigurator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConfigurator __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IBName )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IBName )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IBDir )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BinDir )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CommandLine )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Message )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ BSTR msgLine,
            /* [defaultvalue][optional][in] */ MsgLineMarkers marker);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Status )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ BSTR text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenOLEForm )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ BSTR progID,
            /* [in] */ BSTR title,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Version )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MetaData )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ IMetaData __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendCommand )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ ConfigCommands cmd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsConfigWndOpen )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pResult);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Plugins )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ IPlugins __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Scripts )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ IScripts __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Windows )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ ICfgWindows __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Quit )( 
            IConfigurator __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL PromptSave);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogMessageWrite )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ EventCategories EventCategory,
            /* [in] */ BSTR Message,
            /* [defaultvalue][optional][in] */ BSTR strEventType,
            /* [defaultvalue][optional][in] */ BSTR Event);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run1CApp )( 
            IConfigurator __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ RunAppModes mode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Documents )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ ICfgDocuments __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CfgTimer )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ ICfgTimer __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AppProps )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ AppProperties idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MDWnd )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ IMDWindow __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CancelHotKey )( 
            IConfigurator __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CancelHotKey )( 
            IConfigurator __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IConfiguratorVtbl;

    interface IConfigurator
    {
        CONST_VTBL struct IConfiguratorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigurator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigurator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigurator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigurator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConfigurator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConfigurator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConfigurator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConfigurator_get_IBName(This,pVal)	\
    (This)->lpVtbl -> get_IBName(This,pVal)

#define IConfigurator_put_IBName(This,newVal)	\
    (This)->lpVtbl -> put_IBName(This,newVal)

#define IConfigurator_get_IBDir(This,pVal)	\
    (This)->lpVtbl -> get_IBDir(This,pVal)

#define IConfigurator_get_BinDir(This,pVal)	\
    (This)->lpVtbl -> get_BinDir(This,pVal)

#define IConfigurator_get_CommandLine(This,pVal)	\
    (This)->lpVtbl -> get_CommandLine(This,pVal)

#define IConfigurator_Message(This,msgLine,marker)	\
    (This)->lpVtbl -> Message(This,msgLine,marker)

#define IConfigurator_Status(This,text)	\
    (This)->lpVtbl -> Status(This,text)

#define IConfigurator_OpenOLEForm(This,progID,title,ppDisp)	\
    (This)->lpVtbl -> OpenOLEForm(This,progID,title,ppDisp)

#define IConfigurator_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IConfigurator_get_MetaData(This,pVal)	\
    (This)->lpVtbl -> get_MetaData(This,pVal)

#define IConfigurator_SendCommand(This,cmd)	\
    (This)->lpVtbl -> SendCommand(This,cmd)

#define IConfigurator_IsConfigWndOpen(This,pResult)	\
    (This)->lpVtbl -> IsConfigWndOpen(This,pResult)

#define IConfigurator_get_Plugins(This,pVal)	\
    (This)->lpVtbl -> get_Plugins(This,pVal)

#define IConfigurator_get_Scripts(This,pVal)	\
    (This)->lpVtbl -> get_Scripts(This,pVal)

#define IConfigurator_get_Windows(This,pVal)	\
    (This)->lpVtbl -> get_Windows(This,pVal)

#define IConfigurator_Quit(This,PromptSave)	\
    (This)->lpVtbl -> Quit(This,PromptSave)

#define IConfigurator_LogMessageWrite(This,EventCategory,Message,strEventType,Event)	\
    (This)->lpVtbl -> LogMessageWrite(This,EventCategory,Message,strEventType,Event)

#define IConfigurator_Run1CApp(This,mode)	\
    (This)->lpVtbl -> Run1CApp(This,mode)

#define IConfigurator_get_Documents(This,pVal)	\
    (This)->lpVtbl -> get_Documents(This,pVal)

#define IConfigurator_get_CfgTimer(This,pVal)	\
    (This)->lpVtbl -> get_CfgTimer(This,pVal)

#define IConfigurator_get_AppProps(This,idx,pVal)	\
    (This)->lpVtbl -> get_AppProps(This,idx,pVal)

#define IConfigurator_get_MDWnd(This,pVal)	\
    (This)->lpVtbl -> get_MDWnd(This,pVal)

#define IConfigurator_get_CancelHotKey(This,pVal)	\
    (This)->lpVtbl -> get_CancelHotKey(This,pVal)

#define IConfigurator_put_CancelHotKey(This,newVal)	\
    (This)->lpVtbl -> put_CancelHotKey(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_IBName_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_IBName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConfigurator_put_IBName_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IConfigurator_put_IBName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_IBDir_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_IBDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_BinDir_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_BinDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_CommandLine_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_CommandLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_Message_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ BSTR msgLine,
    /* [defaultvalue][optional][in] */ MsgLineMarkers marker);


void __RPC_STUB IConfigurator_Message_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_Status_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ BSTR text);


void __RPC_STUB IConfigurator_Status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_OpenOLEForm_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ BSTR progID,
    /* [in] */ BSTR title,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);


void __RPC_STUB IConfigurator_OpenOLEForm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_Version_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_MetaData_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ IMetaData __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_MetaData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_SendCommand_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ ConfigCommands cmd);


void __RPC_STUB IConfigurator_SendCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_IsConfigWndOpen_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pResult);


void __RPC_STUB IConfigurator_IsConfigWndOpen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_Plugins_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ IPlugins __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_Plugins_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_Scripts_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ IScripts __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_Scripts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_Windows_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ ICfgWindows __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_Windows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_Quit_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL PromptSave);


void __RPC_STUB IConfigurator_Quit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_LogMessageWrite_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ EventCategories EventCategory,
    /* [in] */ BSTR Message,
    /* [defaultvalue][optional][in] */ BSTR strEventType,
    /* [defaultvalue][optional][in] */ BSTR Event);


void __RPC_STUB IConfigurator_LogMessageWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigurator_Run1CApp_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [defaultvalue][optional][in] */ RunAppModes mode);


void __RPC_STUB IConfigurator_Run1CApp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_Documents_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ ICfgDocuments __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_Documents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_CfgTimer_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ ICfgTimer __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_CfgTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_AppProps_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ AppProperties idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_AppProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_MDWnd_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ IMDWindow __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_MDWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConfigurator_get_CancelHotKey_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IConfigurator_get_CancelHotKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConfigurator_put_CancelHotKey_Proxy( 
    IConfigurator __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IConfigurator_put_CancelHotKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigurator_INTERFACE_DEFINED__ */


#ifndef __IConfigEvents_DISPINTERFACE_DEFINED__
#define __IConfigEvents_DISPINTERFACE_DEFINED__

/* dispinterface IConfigEvents */
/* [helpstring][nonextensible][version][uuid] */ 


DEFINE_GUID(DIID_IConfigEvents,0xFF862868,0xBB20,0x44b2,0x94,0x9C,0x5E,0xA7,0xC0,0x98,0x42,0xEE);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FF862868-BB20-44b2-949C-5EA7C09842EE")
    IConfigEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IConfigEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConfigEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConfigEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConfigEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConfigEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IConfigEventsVtbl;

    interface IConfigEvents
    {
        CONST_VTBL struct IConfigEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConfigEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConfigEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConfigEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IConfigEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Configurator,0xFF862869,0xBB20,0x44b2,0x94,0x9C,0x5E,0xA7,0xC0,0x98,0x42,0xEE);

#ifdef __cplusplus

class DECLSPEC_UUID("FF862869-BB20-44b2-949C-5EA7C09842EE")
Configurator;
#endif

#ifndef __IConfigPlugin_INTERFACE_DEFINED__
#define __IConfigPlugin_INTERFACE_DEFINED__

/* interface IConfigPlugin */
/* [object][helpstring][version][uuid] */ 


DEFINE_GUID(IID_IConfigPlugin,0x8BDEC9EA,0xC6F4,0x4b74,0xAC,0x73,0xF3,0xB0,0xED,0x3B,0x8D,0x4B);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8BDEC9EA-C6F4-4b74-AC73-F3B0ED3B8D4B")
    IConfigPlugin : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IConfigurator __RPC_FAR *pConfigurator,
            /* [retval][out] */ BSTR __RPC_FAR *PluginName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Done( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPluginCaps( 
            /* [in] */ PluginCaps capNum,
            /* [retval][out] */ VARIANT __RPC_FAR *pResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoAction( 
            /* [in] */ PluginAction Action,
            /* [in] */ VARIANT Param,
            /* [retval][out] */ VARIANT __RPC_FAR *pResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigPlugin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigPlugin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigPlugin __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IConfigPlugin __RPC_FAR * This,
            /* [in] */ IConfigurator __RPC_FAR *pConfigurator,
            /* [retval][out] */ BSTR __RPC_FAR *PluginName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Done )( 
            IConfigPlugin __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPluginCaps )( 
            IConfigPlugin __RPC_FAR * This,
            /* [in] */ PluginCaps capNum,
            /* [retval][out] */ VARIANT __RPC_FAR *pResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoAction )( 
            IConfigPlugin __RPC_FAR * This,
            /* [in] */ PluginAction Action,
            /* [in] */ VARIANT Param,
            /* [retval][out] */ VARIANT __RPC_FAR *pResult);
        
        END_INTERFACE
    } IConfigPluginVtbl;

    interface IConfigPlugin
    {
        CONST_VTBL struct IConfigPluginVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigPlugin_Init(This,pConfigurator,PluginName)	\
    (This)->lpVtbl -> Init(This,pConfigurator,PluginName)

#define IConfigPlugin_Done(This)	\
    (This)->lpVtbl -> Done(This)

#define IConfigPlugin_GetPluginCaps(This,capNum,pResult)	\
    (This)->lpVtbl -> GetPluginCaps(This,capNum,pResult)

#define IConfigPlugin_DoAction(This,Action,Param,pResult)	\
    (This)->lpVtbl -> DoAction(This,Action,Param,pResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigPlugin_Init_Proxy( 
    IConfigPlugin __RPC_FAR * This,
    /* [in] */ IConfigurator __RPC_FAR *pConfigurator,
    /* [retval][out] */ BSTR __RPC_FAR *PluginName);


void __RPC_STUB IConfigPlugin_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigPlugin_Done_Proxy( 
    IConfigPlugin __RPC_FAR * This);


void __RPC_STUB IConfigPlugin_Done_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigPlugin_GetPluginCaps_Proxy( 
    IConfigPlugin __RPC_FAR * This,
    /* [in] */ PluginCaps capNum,
    /* [retval][out] */ VARIANT __RPC_FAR *pResult);


void __RPC_STUB IConfigPlugin_GetPluginCaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConfigPlugin_DoAction_Proxy( 
    IConfigPlugin __RPC_FAR * This,
    /* [in] */ PluginAction Action,
    /* [in] */ VARIANT Param,
    /* [retval][out] */ VARIANT __RPC_FAR *pResult);


void __RPC_STUB IConfigPlugin_DoAction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigPlugin_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Scripts,0xbf10b93e,0xeba9,0x473f,0xb5,0xc7,0x04,0xe1,0x38,0xee,0xe7,0xf3);

#ifdef __cplusplus

class DECLSPEC_UUID("bf10b93e-eba9-473f-b5c7-04e138eee7f3")
Scripts;
#endif

DEFINE_GUID(CLSID_Plugins,0x72227927,0x708f,0x4b6f,0x93,0x7f,0x85,0x0a,0x34,0xb3,0x03,0x5c);

#ifdef __cplusplus

class DECLSPEC_UUID("72227927-708f-4b6f-937f-850a34b3035c")
Plugins;
#endif

#ifndef __IType1C_INTERFACE_DEFINED__
#define __IType1C_INTERFACE_DEFINED__

/* interface IType1C */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IType1C,0x9f07065b,0x8434,0x41c8,0xbc,0x54,0x2c,0x64,0xc4,0xe9,0x01,0x10);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9f07065b-8434-41c8-bc54-2c64c4e90110")
    IType1C : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ tTypes __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ tTypes newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FullName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Code( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ID( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Length( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Precision( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Precision( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PositiveOnly( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PositiveOnly( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumSeparated( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumSeparated( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IType1CVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IType1C __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IType1C __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IType1C __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IType1C __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IType1C __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IType1C __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IType1C __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ tTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IType1C __RPC_FAR * This,
            /* [in] */ tTypes newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ID )( 
            IType1C __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Length )( 
            IType1C __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Precision )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Precision )( 
            IType1C __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PositiveOnly )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PositiveOnly )( 
            IType1C __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumSeparated )( 
            IType1C __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NumSeparated )( 
            IType1C __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IType1CVtbl;

    interface IType1C
    {
        CONST_VTBL struct IType1CVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IType1C_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IType1C_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IType1C_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IType1C_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IType1C_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IType1C_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IType1C_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IType1C_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IType1C_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IType1C_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define IType1C_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IType1C_get_Code(This,pVal)	\
    (This)->lpVtbl -> get_Code(This,pVal)

#define IType1C_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IType1C_put_ID(This,newVal)	\
    (This)->lpVtbl -> put_ID(This,newVal)

#define IType1C_get_Length(This,pVal)	\
    (This)->lpVtbl -> get_Length(This,pVal)

#define IType1C_put_Length(This,newVal)	\
    (This)->lpVtbl -> put_Length(This,newVal)

#define IType1C_get_Precision(This,pVal)	\
    (This)->lpVtbl -> get_Precision(This,pVal)

#define IType1C_put_Precision(This,newVal)	\
    (This)->lpVtbl -> put_Precision(This,newVal)

#define IType1C_get_PositiveOnly(This,pVal)	\
    (This)->lpVtbl -> get_PositiveOnly(This,pVal)

#define IType1C_put_PositiveOnly(This,newVal)	\
    (This)->lpVtbl -> put_PositiveOnly(This,newVal)

#define IType1C_get_NumSeparated(This,pVal)	\
    (This)->lpVtbl -> get_NumSeparated(This,pVal)

#define IType1C_put_NumSeparated(This,newVal)	\
    (This)->lpVtbl -> put_NumSeparated(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_Type_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ tTypes __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_Type_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ tTypes newVal);


void __RPC_STUB IType1C_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_FullName_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_FullName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_Name_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_Code_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_Code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_ID_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_ID_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IType1C_put_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_Length_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_Length_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IType1C_put_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_Precision_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_Precision_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_Precision_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IType1C_put_Precision_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_PositiveOnly_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_PositiveOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_PositiveOnly_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IType1C_put_PositiveOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IType1C_get_NumSeparated_Proxy( 
    IType1C __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IType1C_get_NumSeparated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IType1C_put_NumSeparated_Proxy( 
    IType1C __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IType1C_put_NumSeparated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IType1C_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Type1C,0xd3ae2151,0x3a84,0x4df2,0xb8,0x01,0x43,0xde,0xd9,0x9a,0x01,0x3d);

#ifdef __cplusplus

class DECLSPEC_UUID("d3ae2151-3a84-4df2-b801-43ded99a013d")
Type1C;
#endif

#ifndef __IMetaDataObjChilds_INTERFACE_DEFINED__
#define __IMetaDataObjChilds_INTERFACE_DEFINED__

/* interface IMetaDataObjChilds */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IMetaDataObjChilds,0x5b0cde51,0x3bb2,0x4c60,0x8a,0xa5,0x63,0x73,0x9f,0xd6,0x78,0x73);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5b0cde51-3bb2-4c60-8aa5-63739fd67873")
    IMetaDataObjChilds : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [in] */ VARIANT idx,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaDataObjChildsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaDataObjChilds __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaDataObjChilds __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IMetaDataObjChilds __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaDataObjChildsVtbl;

    interface IMetaDataObjChilds
    {
        CONST_VTBL struct IMetaDataObjChildsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaDataObjChilds_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaDataObjChilds_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaDataObjChilds_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaDataObjChilds_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaDataObjChilds_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaDataObjChilds_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaDataObjChilds_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaDataObjChilds_get_item(This,idx,pVal)	\
    (This)->lpVtbl -> get_item(This,idx,pVal)

#define IMetaDataObjChilds_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IMetaDataObjChilds_get_Name(This,idx,Rus,pVal)	\
    (This)->lpVtbl -> get_Name(This,idx,Rus,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjChilds_get_item_Proxy( 
    IMetaDataObjChilds __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ IMetaDataObjArray __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjChilds_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjChilds_get_Count_Proxy( 
    IMetaDataObjChilds __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjChilds_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjChilds_get_Name_Proxy( 
    IMetaDataObjChilds __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjChilds_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaDataObjChilds_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MetaDataObjChilds,0xe8855e48,0xe8a4,0x48d2,0x8e,0x50,0x5e,0x19,0x70,0x33,0x1a,0x7b);

#ifdef __cplusplus

class DECLSPEC_UUID("e8855e48-e8a4-48d2-8e50-5e1970331a7b")
MetaDataObjChilds;
#endif

#ifndef __IMetaDataObjProps_INTERFACE_DEFINED__
#define __IMetaDataObjProps_INTERFACE_DEFINED__

/* interface IMetaDataObjProps */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IMetaDataObjProps,0xb2aa9fd4,0xc643,0x4d40,0xa0,0xd7,0xd5,0x2b,0xab,0xf5,0x5b,0x3d);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b2aa9fd4-c643-4d40-a0d7-d52babf55b3d")
    IMetaDataObjProps : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [in] */ VARIANT idx,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaDataObjPropsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaDataObjProps __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaDataObjProps __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_item )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IMetaDataObjProps __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaDataObjPropsVtbl;

    interface IMetaDataObjProps
    {
        CONST_VTBL struct IMetaDataObjPropsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaDataObjProps_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaDataObjProps_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaDataObjProps_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaDataObjProps_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaDataObjProps_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaDataObjProps_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaDataObjProps_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaDataObjProps_get_item(This,idx,pVal)	\
    (This)->lpVtbl -> get_item(This,idx,pVal)

#define IMetaDataObjProps_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IMetaDataObjProps_get_Name(This,idx,Rus,pVal)	\
    (This)->lpVtbl -> get_Name(This,idx,Rus,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjProps_get_item_Proxy( 
    IMetaDataObjProps __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjProps_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjProps_get_Count_Proxy( 
    IMetaDataObjProps __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjProps_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaDataObjProps_get_Name_Proxy( 
    IMetaDataObjProps __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL Rus,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaDataObjProps_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaDataObjProps_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MetaDataObjProps,0x8d97b6f5,0xf5b9,0x41db,0x91,0x93,0xa9,0x21,0xa6,0x3b,0xb0,0xf6);

#ifdef __cplusplus

class DECLSPEC_UUID("8d97b6f5-f5b9-41db-9193-a921a63bb0f6")
MetaDataObjProps;
#endif

#ifndef __IMigration_INTERFACE_DEFINED__
#define __IMigration_INTERFACE_DEFINED__

/* interface IMigration */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IMigration,0x59f10fca,0x4e12,0x4381,0x94,0x40,0xb5,0x47,0x5a,0x4d,0xdd,0x6d);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59f10fca-4e12-4381-9440-b5475a4ddd6d")
    IMigration : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RuleOfMigration( 
            /* [retval][out] */ MigrationRules __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RuleOfMigration( 
            /* [in] */ MigrationRules newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoRegister( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoRegister( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AdditDBSigns( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AdditDBSigns( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMigrationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMigration __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMigration __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMigration __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMigration __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMigration __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMigration __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMigration __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RuleOfMigration )( 
            IMigration __RPC_FAR * This,
            /* [retval][out] */ MigrationRules __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RuleOfMigration )( 
            IMigration __RPC_FAR * This,
            /* [in] */ MigrationRules newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AutoRegister )( 
            IMigration __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AutoRegister )( 
            IMigration __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AdditDBSigns )( 
            IMigration __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AdditDBSigns )( 
            IMigration __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IMigrationVtbl;

    interface IMigration
    {
        CONST_VTBL struct IMigrationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMigration_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMigration_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMigration_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMigration_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMigration_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMigration_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMigration_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMigration_get_RuleOfMigration(This,pVal)	\
    (This)->lpVtbl -> get_RuleOfMigration(This,pVal)

#define IMigration_put_RuleOfMigration(This,newVal)	\
    (This)->lpVtbl -> put_RuleOfMigration(This,newVal)

#define IMigration_get_AutoRegister(This,pVal)	\
    (This)->lpVtbl -> get_AutoRegister(This,pVal)

#define IMigration_put_AutoRegister(This,newVal)	\
    (This)->lpVtbl -> put_AutoRegister(This,newVal)

#define IMigration_get_AdditDBSigns(This,pVal)	\
    (This)->lpVtbl -> get_AdditDBSigns(This,pVal)

#define IMigration_put_AdditDBSigns(This,newVal)	\
    (This)->lpVtbl -> put_AdditDBSigns(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMigration_get_RuleOfMigration_Proxy( 
    IMigration __RPC_FAR * This,
    /* [retval][out] */ MigrationRules __RPC_FAR *pVal);


void __RPC_STUB IMigration_get_RuleOfMigration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMigration_put_RuleOfMigration_Proxy( 
    IMigration __RPC_FAR * This,
    /* [in] */ MigrationRules newVal);


void __RPC_STUB IMigration_put_RuleOfMigration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMigration_get_AutoRegister_Proxy( 
    IMigration __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IMigration_get_AutoRegister_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMigration_put_AutoRegister_Proxy( 
    IMigration __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IMigration_put_AutoRegister_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMigration_get_AdditDBSigns_Proxy( 
    IMigration __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMigration_get_AdditDBSigns_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMigration_put_AdditDBSigns_Proxy( 
    IMigration __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMigration_put_AdditDBSigns_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMigration_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Migration,0x81477568,0xabbb,0x4ff3,0x96,0x27,0xe1,0x40,0x29,0x85,0xb1,0x08);

#ifdef __cplusplus

class DECLSPEC_UUID("81477568-abbb-4ff3-9627-e1402985b108")
Migration;
#endif

DEFINE_GUID(CLSID_MetaDataObj,0xB5E36B88,0xCDA7,0x4107,0x82,0xBF,0x6B,0x20,0xA8,0x15,0x8B,0x7A);

#ifdef __cplusplus

class DECLSPEC_UUID("B5E36B88-CDA7-4107-82BF-6B20A8158B7A")
MetaDataObj;
#endif

DEFINE_GUID(CLSID_MetaDataObjArray,0x8cda8e20,0x913a,0x4eeb,0x91,0x72,0xa1,0x56,0x9f,0x66,0x3e,0x16);

#ifdef __cplusplus

class DECLSPEC_UUID("8cda8e20-913a-4eeb-9172-a1569f663e16")
MetaDataObjArray;
#endif

#ifndef __ISelfScript_INTERFACE_DEFINED__
#define __ISelfScript_INTERFACE_DEFINED__

/* interface ISelfScript */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ISelfScript,0xd7ba8eb5,0xb237,0x4323,0xb9,0x71,0xbc,0xb3,0x7b,0xf3,0xd0,0x1e);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d7ba8eb5-b237-4323-b971-bcb37bf3d01e")
    ISelfScript : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddNamedItem( 
            /* [in] */ BSTR Name,
            /* [in] */ IDispatch __RPC_FAR *Object,
            /* [in] */ VARIANT_BOOL AsGlobal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISelfScriptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISelfScript __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISelfScript __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISelfScript __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISelfScript __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISelfScript __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISelfScript __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISelfScript __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ISelfScript __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddNamedItem )( 
            ISelfScript __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [in] */ IDispatch __RPC_FAR *Object,
            /* [in] */ VARIANT_BOOL AsGlobal);
        
        END_INTERFACE
    } ISelfScriptVtbl;

    interface ISelfScript
    {
        CONST_VTBL struct ISelfScriptVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISelfScript_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISelfScript_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISelfScript_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISelfScript_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISelfScript_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISelfScript_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISelfScript_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISelfScript_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ISelfScript_AddNamedItem(This,Name,Object,AsGlobal)	\
    (This)->lpVtbl -> AddNamedItem(This,Name,Object,AsGlobal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISelfScript_get_Name_Proxy( 
    ISelfScript __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISelfScript_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISelfScript_AddNamedItem_Proxy( 
    ISelfScript __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [in] */ IDispatch __RPC_FAR *Object,
    /* [in] */ VARIANT_BOOL AsGlobal);


void __RPC_STUB ISelfScript_AddNamedItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISelfScript_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_SelfScript,0x02a7af19,0xb1ad,0x4f20,0xbe,0x50,0xd6,0x44,0xbb,0x9d,0xdd,0x8f);

#ifdef __cplusplus

class DECLSPEC_UUID("02a7af19-b1ad-4f20-be50-d644bb9ddd8f")
SelfScript;
#endif

#ifndef __IMetaEnumVal_INTERFACE_DEFINED__
#define __IMetaEnumVal_INTERFACE_DEFINED__

/* interface IMetaEnumVal */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IMetaEnumVal,0x9205eda0,0x4587,0x486e,0x81,0x6d,0x50,0xe9,0xaa,0x24,0x73,0x8d);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9205eda0-4587-486e-816d-50e9aa24738d")
    IMetaEnumVal : public IMetaDataObj
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Title( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Title( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaEnumValVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaEnumVal __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaEnumVal __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WhatIsIt )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Present )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Present )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Descr )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Descr )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Props )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Childs )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DescribeObject )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Title )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Title )( 
            IMetaEnumVal __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IMetaEnumValVtbl;

    interface IMetaEnumVal
    {
        CONST_VTBL struct IMetaEnumValVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaEnumVal_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaEnumVal_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaEnumVal_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaEnumVal_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaEnumVal_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaEnumVal_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaEnumVal_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaEnumVal_get_WhatIsIt(This,pVal)	\
    (This)->lpVtbl -> get_WhatIsIt(This,pVal)

#define IMetaEnumVal_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IMetaEnumVal_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IMetaEnumVal_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IMetaEnumVal_get_Present(This,pVal)	\
    (This)->lpVtbl -> get_Present(This,pVal)

#define IMetaEnumVal_put_Present(This,newVal)	\
    (This)->lpVtbl -> put_Present(This,newVal)

#define IMetaEnumVal_get_Descr(This,pVal)	\
    (This)->lpVtbl -> get_Descr(This,pVal)

#define IMetaEnumVal_put_Descr(This,newVal)	\
    (This)->lpVtbl -> put_Descr(This,newVal)

#define IMetaEnumVal_IsValid(This,pRet)	\
    (This)->lpVtbl -> IsValid(This,pRet)

#define IMetaEnumVal_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define IMetaEnumVal_get_Props(This,pVal)	\
    (This)->lpVtbl -> get_Props(This,pVal)

#define IMetaEnumVal_get_Childs(This,pVal)	\
    (This)->lpVtbl -> get_Childs(This,pVal)

#define IMetaEnumVal_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IMetaEnumVal_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IMetaEnumVal_DescribeObject(This,WithChilds,retVal)	\
    (This)->lpVtbl -> DescribeObject(This,WithChilds,retVal)

#define IMetaEnumVal_get_Parent(This,pVal)	\
    (This)->lpVtbl -> get_Parent(This,pVal)


#define IMetaEnumVal_get_Title(This,pVal)	\
    (This)->lpVtbl -> get_Title(This,pVal)

#define IMetaEnumVal_put_Title(This,newVal)	\
    (This)->lpVtbl -> put_Title(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaEnumVal_get_Title_Proxy( 
    IMetaEnumVal __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMetaEnumVal_get_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMetaEnumVal_put_Title_Proxy( 
    IMetaEnumVal __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMetaEnumVal_put_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaEnumVal_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MetaEnumVal,0xf40fd0f0,0x54af,0x4fed,0x89,0xa6,0x6d,0x42,0x33,0xe2,0x82,0xe5);

#ifdef __cplusplus

class DECLSPEC_UUID("f40fd0f0-54af-4fed-89a6-6d4233e282e5")
MetaEnumVal;
#endif

#ifndef __IMetaEnum_INTERFACE_DEFINED__
#define __IMetaEnum_INTERFACE_DEFINED__

/* interface IMetaEnum */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IMetaEnum,0x0a75dad6,0xe6f3,0x4116,0x86,0x2b,0x13,0x3e,0x26,0xbd,0x83,0x77);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0a75dad6-e6f3-4116-862b-133e26bd8377")
    IMetaEnum : public IMetaDataObj
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ValsCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnumVal( 
            VARIANT idx,
            /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT idx) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaEnumVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaEnum __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaEnum __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaEnum __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WhatIsIt )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Present )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Present )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Descr )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Descr )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Props )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Childs )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DescribeObject )( 
            IMetaEnum __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ValsCount )( 
            IMetaEnum __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnumVal )( 
            IMetaEnum __RPC_FAR * This,
            VARIANT idx,
            /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IMetaEnum __RPC_FAR * This,
            /* [in] */ VARIANT idx);
        
        END_INTERFACE
    } IMetaEnumVtbl;

    interface IMetaEnum
    {
        CONST_VTBL struct IMetaEnumVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaEnum_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaEnum_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaEnum_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaEnum_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaEnum_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaEnum_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaEnum_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaEnum_get_WhatIsIt(This,pVal)	\
    (This)->lpVtbl -> get_WhatIsIt(This,pVal)

#define IMetaEnum_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IMetaEnum_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IMetaEnum_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IMetaEnum_get_Present(This,pVal)	\
    (This)->lpVtbl -> get_Present(This,pVal)

#define IMetaEnum_put_Present(This,newVal)	\
    (This)->lpVtbl -> put_Present(This,newVal)

#define IMetaEnum_get_Descr(This,pVal)	\
    (This)->lpVtbl -> get_Descr(This,pVal)

#define IMetaEnum_put_Descr(This,newVal)	\
    (This)->lpVtbl -> put_Descr(This,newVal)

#define IMetaEnum_IsValid(This,pRet)	\
    (This)->lpVtbl -> IsValid(This,pRet)

#define IMetaEnum_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define IMetaEnum_get_Props(This,pVal)	\
    (This)->lpVtbl -> get_Props(This,pVal)

#define IMetaEnum_get_Childs(This,pVal)	\
    (This)->lpVtbl -> get_Childs(This,pVal)

#define IMetaEnum_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IMetaEnum_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IMetaEnum_DescribeObject(This,WithChilds,retVal)	\
    (This)->lpVtbl -> DescribeObject(This,WithChilds,retVal)

#define IMetaEnum_get_Parent(This,pVal)	\
    (This)->lpVtbl -> get_Parent(This,pVal)


#define IMetaEnum_get_ValsCount(This,pVal)	\
    (This)->lpVtbl -> get_ValsCount(This,pVal)

#define IMetaEnum_get_EnumVal(This,idx,pVal)	\
    (This)->lpVtbl -> get_EnumVal(This,idx,pVal)

#define IMetaEnum_Add(This,Name,pVal)	\
    (This)->lpVtbl -> Add(This,Name,pVal)

#define IMetaEnum_Remove(This,idx)	\
    (This)->lpVtbl -> Remove(This,idx)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaEnum_get_ValsCount_Proxy( 
    IMetaEnum __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaEnum_get_ValsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaEnum_get_EnumVal_Proxy( 
    IMetaEnum __RPC_FAR * This,
    VARIANT idx,
    /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaEnum_get_EnumVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaEnum_Add_Proxy( 
    IMetaEnum __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ IMetaEnumVal __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaEnum_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaEnum_Remove_Proxy( 
    IMetaEnum __RPC_FAR * This,
    /* [in] */ VARIANT idx);


void __RPC_STUB IMetaEnum_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaEnum_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MetaEnum,0x03a11169,0x8a38,0x42d4,0x9b,0x96,0x7b,0x78,0x85,0x37,0xc3,0xe5);

#ifdef __cplusplus

class DECLSPEC_UUID("03a11169-8a38-42d4-9b96-7b788537c3e5")
MetaEnum;
#endif

#ifndef __IMetaEnums_INTERFACE_DEFINED__
#define __IMetaEnums_INTERFACE_DEFINED__

/* interface IMetaEnums */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IMetaEnums,0x6d5c5f96,0x05d5,0x4444,0x8f,0xbd,0x32,0x47,0xea,0x6a,0xc4,0xca);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6d5c5f96-05d5-4444-8fbd-3247ea6ac4ca")
    IMetaEnums : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enum( 
            VARIANT idx,
            /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT idx) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMetaEnumsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMetaEnums __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMetaEnums __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMetaEnums __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Enum )( 
            IMetaEnums __RPC_FAR * This,
            VARIANT idx,
            /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IMetaEnums __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ VARIANT idx);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IMetaEnums __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } IMetaEnumsVtbl;

    interface IMetaEnums
    {
        CONST_VTBL struct IMetaEnumsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMetaEnums_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMetaEnums_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMetaEnums_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMetaEnums_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMetaEnums_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMetaEnums_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMetaEnums_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMetaEnums_get_Enum(This,idx,pVal)	\
    (This)->lpVtbl -> get_Enum(This,idx,pVal)

#define IMetaEnums_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IMetaEnums_Remove(This,idx)	\
    (This)->lpVtbl -> Remove(This,idx)

#define IMetaEnums_Add(This,Name,pVal)	\
    (This)->lpVtbl -> Add(This,Name,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaEnums_get_Enum_Proxy( 
    IMetaEnums __RPC_FAR * This,
    VARIANT idx,
    /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaEnums_get_Enum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMetaEnums_get_Count_Proxy( 
    IMetaEnums __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMetaEnums_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaEnums_Remove_Proxy( 
    IMetaEnums __RPC_FAR * This,
    /* [in] */ VARIANT idx);


void __RPC_STUB IMetaEnums_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMetaEnums_Add_Proxy( 
    IMetaEnums __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ IMetaEnum __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IMetaEnums_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMetaEnums_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MetaEnums,0x036e513a,0x1a68,0x41f9,0x8c,0x39,0xb9,0x7e,0x1f,0x8d,0x86,0x27);

#ifdef __cplusplus

class DECLSPEC_UUID("036e513a-1a68-41f9-8c39-b97e1f8d8627")
MetaEnums;
#endif

DEFINE_GUID(CLSID_MetaData,0x43A6FDE2,0x747B,0x44dd,0xA5,0x95,0x51,0x21,0x83,0xEE,0x5B,0xEB);

#ifdef __cplusplus

class DECLSPEC_UUID("43A6FDE2-747B-44dd-A595-512183EE5BEB")
MetaData;
#endif

DEFINE_GUID(CLSID_TaskDef,0xd685f452,0x6ab6,0x4e57,0xb5,0xc2,0x4c,0x3c,0x9f,0xc9,0xe0,0xd2);

#ifdef __cplusplus

class DECLSPEC_UUID("d685f452-6ab6-4e57-b5c2-4c3c9fc9e0d2")
TaskDef;
#endif

DEFINE_GUID(CLSID_AllTypedObj,0xea8c84ee,0x0c03,0x4c8d,0x95,0x48,0x88,0x37,0xf3,0xcd,0x16,0x3a);

#ifdef __cplusplus

class DECLSPEC_UUID("ea8c84ee-0c03-4c8d-9548-8837f3cd163a")
AllTypedObj;
#endif

#ifndef __IConstDef_INTERFACE_DEFINED__
#define __IConstDef_INTERFACE_DEFINED__

/* interface IConstDef */
/* [object][helpstring][version][nonextensible][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_IConstDef,0x6abd59ab,0x7517,0x4c6c,0x9f,0x8d,0x30,0xa3,0x12,0x07,0x3a,0xd7);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6abd59ab-7517-4c6c-9f8d-30a312073ad7")
    IConstDef : public IMetaDataObj
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Periodic( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Periodic( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConstDefVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConstDef __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConstDef __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConstDef __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WhatIsIt )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ TypeOfMetaDataObj __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Present )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Present )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Descr )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Descr )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pRet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FullName )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Props )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjProps __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Childs )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObjChilds __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ IType1C __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ IType1C __RPC_FAR *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DescribeObject )( 
            IConstDef __RPC_FAR * This,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL WithChilds,
            /* [retval][out] */ BSTR __RPC_FAR *retVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ IMetaDataObj __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Periodic )( 
            IConstDef __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Periodic )( 
            IConstDef __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IConstDefVtbl;

    interface IConstDef
    {
        CONST_VTBL struct IConstDefVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConstDef_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConstDef_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConstDef_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConstDef_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConstDef_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConstDef_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConstDef_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConstDef_get_WhatIsIt(This,pVal)	\
    (This)->lpVtbl -> get_WhatIsIt(This,pVal)

#define IConstDef_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IConstDef_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IConstDef_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IConstDef_get_Present(This,pVal)	\
    (This)->lpVtbl -> get_Present(This,pVal)

#define IConstDef_put_Present(This,newVal)	\
    (This)->lpVtbl -> put_Present(This,newVal)

#define IConstDef_get_Descr(This,pVal)	\
    (This)->lpVtbl -> get_Descr(This,pVal)

#define IConstDef_put_Descr(This,newVal)	\
    (This)->lpVtbl -> put_Descr(This,newVal)

#define IConstDef_IsValid(This,pRet)	\
    (This)->lpVtbl -> IsValid(This,pRet)

#define IConstDef_get_FullName(This,pVal)	\
    (This)->lpVtbl -> get_FullName(This,pVal)

#define IConstDef_get_Props(This,pVal)	\
    (This)->lpVtbl -> get_Props(This,pVal)

#define IConstDef_get_Childs(This,pVal)	\
    (This)->lpVtbl -> get_Childs(This,pVal)

#define IConstDef_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IConstDef_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IConstDef_DescribeObject(This,WithChilds,retVal)	\
    (This)->lpVtbl -> DescribeObject(This,WithChilds,retVal)

#define IConstDef_get_Parent(This,pVal)	\
    (This)->lpVtbl -> get_Parent(This,pVal)


#define IConstDef_get_Periodic(This,pVal)	\
    (This)->lpVtbl -> get_Periodic(This,pVal)

#define IConstDef_put_Periodic(This,newVal)	\
    (This)->lpVtbl -> put_Periodic(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IConstDef_get_Periodic_Proxy( 
    IConstDef __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IConstDef_get_Periodic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IConstDef_put_Periodic_Proxy( 
    IConstDef __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IConstDef_put_Periodic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConstDef_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ConstDef,0x1b100ee6,0x0a82,0x40c1,0x92,0x85,0xea,0x32,0x9d,0x8f,0xfa,0x9c);

#ifdef __cplusplus

class DECLSPEC_UUID("1b100ee6-0a82-40c1-9285-ea329d8ffa9c")
ConstDef;
#endif

DEFINE_GUID(CLSID_Constants,0x756afc1e,0x9a0a,0x4b8d,0xa4,0x59,0x97,0xdf,0x55,0xa7,0x9c,0x70);

#ifdef __cplusplus

class DECLSPEC_UUID("756afc1e-9a0a-4b8d-a459-97df55a79c70")
Constants;
#endif

DEFINE_GUID(CLSID_CfgDoc,0xf9f6c920,0x7206,0x11d8,0x9f,0xcd,0xe9,0x05,0x38,0x13,0xda,0x08);

#ifdef __cplusplus

class DECLSPEC_UUID("f9f6c920-7206-11d8-9fcd-e9053813da08")
CfgDoc;
#endif

DEFINE_GUID(CLSID_CfgDocuments,0x30ecb402,0x4e56,0x44bd,0x8e,0x7c,0x65,0x21,0xef,0xdb,0x58,0xce);

#ifdef __cplusplus

class DECLSPEC_UUID("30ecb402-4e56-44bd-8e7c-6521efdb58ce")
CfgDocuments;
#endif

#ifndef __IWorkBook_INTERFACE_DEFINED__
#define __IWorkBook_INTERFACE_DEFINED__

/* interface IWorkBook */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_IWorkBook,0x3904cfac,0x714b,0x4241,0xaf,0x65,0x5f,0xd6,0x96,0x7a,0x06,0x5c);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3904cfac-714b-4241-af65-5fd6967a065c")
    IWorkBook : public ICfgDoc
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CountPages( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NamePage( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NamePage( 
            /* [in] */ VARIANT idx,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Page( 
            /* [in] */ VARIANT idx,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActivePage( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ActivePage( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddPage( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *ppNewPage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemovePage( 
            /* [in] */ VARIANT idx) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWorkBookVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWorkBook __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWorkBook __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWorkBook __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ DocTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Kind )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IWorkBook __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOpen )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IWorkBook __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CountPages )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NamePage )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NamePage )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Page )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ VARIANT idx,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ActivePage )( 
            IWorkBook __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ActivePage )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddPage )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *ppNewPage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemovePage )( 
            IWorkBook __RPC_FAR * This,
            /* [in] */ VARIANT idx);
        
        END_INTERFACE
    } IWorkBookVtbl;

    interface IWorkBook
    {
        CONST_VTBL struct IWorkBookVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWorkBook_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWorkBook_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWorkBook_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWorkBook_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWorkBook_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWorkBook_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWorkBook_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWorkBook_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IWorkBook_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define IWorkBook_get_Kind(This,pVal)	\
    (This)->lpVtbl -> get_Kind(This,pVal)

#define IWorkBook_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IWorkBook_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define IWorkBook_get_IsOpen(This,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,pVal)

#define IWorkBook_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IWorkBook_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)


#define IWorkBook_get_CountPages(This,pVal)	\
    (This)->lpVtbl -> get_CountPages(This,pVal)

#define IWorkBook_get_NamePage(This,idx,pVal)	\
    (This)->lpVtbl -> get_NamePage(This,idx,pVal)

#define IWorkBook_put_NamePage(This,idx,newVal)	\
    (This)->lpVtbl -> put_NamePage(This,idx,newVal)

#define IWorkBook_get_Page(This,idx,pVal)	\
    (This)->lpVtbl -> get_Page(This,idx,pVal)

#define IWorkBook_get_ActivePage(This,pVal)	\
    (This)->lpVtbl -> get_ActivePage(This,pVal)

#define IWorkBook_put_ActivePage(This,newVal)	\
    (This)->lpVtbl -> put_ActivePage(This,newVal)

#define IWorkBook_AddPage(This,Name,ppNewPage)	\
    (This)->lpVtbl -> AddPage(This,Name,ppNewPage)

#define IWorkBook_RemovePage(This,idx)	\
    (This)->lpVtbl -> RemovePage(This,idx)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWorkBook_get_CountPages_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IWorkBook_get_CountPages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWorkBook_get_NamePage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IWorkBook_get_NamePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWorkBook_put_NamePage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [in] */ BSTR newVal);


void __RPC_STUB IWorkBook_put_NamePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWorkBook_get_Page_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ VARIANT idx,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IWorkBook_get_Page_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWorkBook_get_ActivePage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IWorkBook_get_ActivePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IWorkBook_put_ActivePage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IWorkBook_put_ActivePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWorkBook_AddPage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ ICfgDoc __RPC_FAR *__RPC_FAR *ppNewPage);


void __RPC_STUB IWorkBook_AddPage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWorkBook_RemovePage_Proxy( 
    IWorkBook __RPC_FAR * This,
    /* [in] */ VARIANT idx);


void __RPC_STUB IWorkBook_RemovePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWorkBook_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_WorkBook,0x46d26a73,0x3d4c,0x4ee7,0xab,0x1f,0x26,0xf4,0x2e,0x02,0x89,0x76);

#ifdef __cplusplus

class DECLSPEC_UUID("46d26a73-3d4c-4ee7-ab1f-26f42e028976")
WorkBook;
#endif

#ifndef __ITextDoc_INTERFACE_DEFINED__
#define __ITextDoc_INTERFACE_DEFINED__

/* interface ITextDoc */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ITextDoc,0xdee7bd6f,0x1af8,0x4dac,0xa2,0x03,0x4e,0x27,0xf8,0xc6,0x08,0xf7);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("dee7bd6f-1af8-4dac-a203-4e27f8c608f7")
    ITextDoc : public ICfgDoc
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Text( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineCount( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineLen( 
            /* [in] */ long LineNum,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsModule( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsModule( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReadOnly( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReadOnly( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Range( 
            /* [in] */ long StartLine,
            /* [optional][in] */ VARIANT StartCol,
            /* [optional][in] */ VARIANT EndLine,
            /* [optional][in] */ VARIANT EndCol,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Range( 
            /* [in] */ long StartLine,
            /* [optional][in] */ VARIANT StartCol,
            /* [optional][in] */ VARIANT EndLine,
            /* [optional][in] */ VARIANT EndCol,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelStartLine( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelStartCol( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelEndLine( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelEndCol( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentWord( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveCaret( 
            /* [in] */ long LineStart,
            /* [in] */ long ColStart,
            /* [optional][in] */ VARIANT LineEnd,
            /* [optional][in] */ VARIANT ColEnd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BookMark( 
            /* [in] */ long LineNum,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BookMark( 
            /* [in] */ long LineNum,
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NextBookmark( 
            /* [in] */ long StartLine,
            /* [retval][out] */ long __RPC_FAR *pNextBookmark) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrevBookmark( 
            /* [in] */ long StartLine,
            /* [retval][out] */ long __RPC_FAR *pRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearAllBookMark( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CanUndo( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CanRedo( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Undo( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Redo( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Cut( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Paste( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CommentSel( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UncommentSel( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FormatSel( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITextDocVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITextDoc __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITextDoc __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITextDoc __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ DocTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Kind )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOpen )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToFile )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadFromFile )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Text )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Text )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineCount )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineLen )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long LineNum,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsModule )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsModule )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReadOnly )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ReadOnly )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Range )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long StartLine,
            /* [optional][in] */ VARIANT StartCol,
            /* [optional][in] */ VARIANT EndLine,
            /* [optional][in] */ VARIANT EndCol,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Range )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long StartLine,
            /* [optional][in] */ VARIANT StartCol,
            /* [optional][in] */ VARIANT EndLine,
            /* [optional][in] */ VARIANT EndCol,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelStartLine )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelStartCol )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelEndLine )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelEndCol )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentWord )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveCaret )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long LineStart,
            /* [in] */ long ColStart,
            /* [optional][in] */ VARIANT LineEnd,
            /* [optional][in] */ VARIANT ColEnd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BookMark )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long LineNum,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BookMark )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long LineNum,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextBookmark )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long StartLine,
            /* [retval][out] */ long __RPC_FAR *pNextBookmark);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrevBookmark )( 
            ITextDoc __RPC_FAR * This,
            /* [in] */ long StartLine,
            /* [retval][out] */ long __RPC_FAR *pRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearAllBookMark )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CanUndo )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CanRedo )( 
            ITextDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Undo )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Redo )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cut )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Paste )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CommentSel )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UncommentSel )( 
            ITextDoc __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FormatSel )( 
            ITextDoc __RPC_FAR * This);
        
        END_INTERFACE
    } ITextDocVtbl;

    interface ITextDoc
    {
        CONST_VTBL struct ITextDocVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextDoc_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITextDoc_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITextDoc_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITextDoc_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITextDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITextDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITextDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITextDoc_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ITextDoc_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ITextDoc_get_Kind(This,pVal)	\
    (This)->lpVtbl -> get_Kind(This,pVal)

#define ITextDoc_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define ITextDoc_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define ITextDoc_get_IsOpen(This,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,pVal)

#define ITextDoc_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define ITextDoc_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)


#define ITextDoc_SaveToFile(This,FileName,bSucces)	\
    (This)->lpVtbl -> SaveToFile(This,FileName,bSucces)

#define ITextDoc_LoadFromFile(This,FileName,bSuccess)	\
    (This)->lpVtbl -> LoadFromFile(This,FileName,bSuccess)

#define ITextDoc_get_Text(This,pVal)	\
    (This)->lpVtbl -> get_Text(This,pVal)

#define ITextDoc_put_Text(This,newVal)	\
    (This)->lpVtbl -> put_Text(This,newVal)

#define ITextDoc_get_LineCount(This,pVal)	\
    (This)->lpVtbl -> get_LineCount(This,pVal)

#define ITextDoc_get_LineLen(This,LineNum,pVal)	\
    (This)->lpVtbl -> get_LineLen(This,LineNum,pVal)

#define ITextDoc_get_IsModule(This,pVal)	\
    (This)->lpVtbl -> get_IsModule(This,pVal)

#define ITextDoc_put_IsModule(This,newVal)	\
    (This)->lpVtbl -> put_IsModule(This,newVal)

#define ITextDoc_get_ReadOnly(This,pVal)	\
    (This)->lpVtbl -> get_ReadOnly(This,pVal)

#define ITextDoc_put_ReadOnly(This,newVal)	\
    (This)->lpVtbl -> put_ReadOnly(This,newVal)

#define ITextDoc_get_Range(This,StartLine,StartCol,EndLine,EndCol,pVal)	\
    (This)->lpVtbl -> get_Range(This,StartLine,StartCol,EndLine,EndCol,pVal)

#define ITextDoc_put_Range(This,StartLine,StartCol,EndLine,EndCol,newVal)	\
    (This)->lpVtbl -> put_Range(This,StartLine,StartCol,EndLine,EndCol,newVal)

#define ITextDoc_get_SelStartLine(This,pVal)	\
    (This)->lpVtbl -> get_SelStartLine(This,pVal)

#define ITextDoc_get_SelStartCol(This,pVal)	\
    (This)->lpVtbl -> get_SelStartCol(This,pVal)

#define ITextDoc_get_SelEndLine(This,pVal)	\
    (This)->lpVtbl -> get_SelEndLine(This,pVal)

#define ITextDoc_get_SelEndCol(This,pVal)	\
    (This)->lpVtbl -> get_SelEndCol(This,pVal)

#define ITextDoc_get_CurrentWord(This,pVal)	\
    (This)->lpVtbl -> get_CurrentWord(This,pVal)

#define ITextDoc_MoveCaret(This,LineStart,ColStart,LineEnd,ColEnd)	\
    (This)->lpVtbl -> MoveCaret(This,LineStart,ColStart,LineEnd,ColEnd)

#define ITextDoc_get_BookMark(This,LineNum,pVal)	\
    (This)->lpVtbl -> get_BookMark(This,LineNum,pVal)

#define ITextDoc_put_BookMark(This,LineNum,newVal)	\
    (This)->lpVtbl -> put_BookMark(This,LineNum,newVal)

#define ITextDoc_NextBookmark(This,StartLine,pNextBookmark)	\
    (This)->lpVtbl -> NextBookmark(This,StartLine,pNextBookmark)

#define ITextDoc_PrevBookmark(This,StartLine,pRet)	\
    (This)->lpVtbl -> PrevBookmark(This,StartLine,pRet)

#define ITextDoc_ClearAllBookMark(This)	\
    (This)->lpVtbl -> ClearAllBookMark(This)

#define ITextDoc_get_CanUndo(This,pVal)	\
    (This)->lpVtbl -> get_CanUndo(This,pVal)

#define ITextDoc_get_CanRedo(This,pVal)	\
    (This)->lpVtbl -> get_CanRedo(This,pVal)

#define ITextDoc_Undo(This)	\
    (This)->lpVtbl -> Undo(This)

#define ITextDoc_Redo(This)	\
    (This)->lpVtbl -> Redo(This)

#define ITextDoc_Cut(This)	\
    (This)->lpVtbl -> Cut(This)

#define ITextDoc_Copy(This)	\
    (This)->lpVtbl -> Copy(This)

#define ITextDoc_Paste(This)	\
    (This)->lpVtbl -> Paste(This)

#define ITextDoc_CommentSel(This)	\
    (This)->lpVtbl -> CommentSel(This)

#define ITextDoc_UncommentSel(This)	\
    (This)->lpVtbl -> UncommentSel(This)

#define ITextDoc_FormatSel(This)	\
    (This)->lpVtbl -> FormatSel(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_SaveToFile_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);


void __RPC_STUB ITextDoc_SaveToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_LoadFromFile_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);


void __RPC_STUB ITextDoc_LoadFromFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_Text_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITextDoc_put_Text_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITextDoc_put_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_LineCount_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_LineCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_LineLen_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long LineNum,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_LineLen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_IsModule_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_IsModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITextDoc_put_IsModule_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITextDoc_put_IsModule_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_ReadOnly_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_ReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITextDoc_put_ReadOnly_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITextDoc_put_ReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_Range_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long StartLine,
    /* [optional][in] */ VARIANT StartCol,
    /* [optional][in] */ VARIANT EndLine,
    /* [optional][in] */ VARIANT EndCol,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_Range_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITextDoc_put_Range_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long StartLine,
    /* [optional][in] */ VARIANT StartCol,
    /* [optional][in] */ VARIANT EndLine,
    /* [optional][in] */ VARIANT EndCol,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITextDoc_put_Range_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_SelStartLine_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_SelStartLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_SelStartCol_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_SelStartCol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_SelEndLine_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_SelEndLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_SelEndCol_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_SelEndCol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_CurrentWord_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_CurrentWord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_MoveCaret_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long LineStart,
    /* [in] */ long ColStart,
    /* [optional][in] */ VARIANT LineEnd,
    /* [optional][in] */ VARIANT ColEnd);


void __RPC_STUB ITextDoc_MoveCaret_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_BookMark_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long LineNum,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_BookMark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITextDoc_put_BookMark_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long LineNum,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITextDoc_put_BookMark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_NextBookmark_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long StartLine,
    /* [retval][out] */ long __RPC_FAR *pNextBookmark);


void __RPC_STUB ITextDoc_NextBookmark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_PrevBookmark_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [in] */ long StartLine,
    /* [retval][out] */ long __RPC_FAR *pRet);


void __RPC_STUB ITextDoc_PrevBookmark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_ClearAllBookMark_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_ClearAllBookMark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_CanUndo_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_CanUndo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITextDoc_get_CanRedo_Proxy( 
    ITextDoc __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ITextDoc_get_CanRedo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_Undo_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_Undo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_Redo_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_Redo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_Cut_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_Cut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_Copy_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_Paste_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_Paste_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_CommentSel_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_CommentSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_UncommentSel_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_UncommentSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITextDoc_FormatSel_Proxy( 
    ITextDoc __RPC_FAR * This);


void __RPC_STUB ITextDoc_FormatSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITextDoc_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_TextDoc,0x08b07dc4,0x919a,0x4d05,0x94,0x12,0xa1,0xcc,0x7f,0x01,0xc4,0xb9);

#ifdef __cplusplus

class DECLSPEC_UUID("08b07dc4-919a-4d05-9412-a1cc7f01c4b9")
TextDoc;
#endif

#ifndef __ITableDoc_INTERFACE_DEFINED__
#define __ITableDoc_INTERFACE_DEFINED__

/* interface ITableDoc */
/* [object][helpstring][version][nonextensible][dual][uuid] */ 


DEFINE_GUID(IID_ITableDoc,0xcefb0920,0x7a8f,0x11d8,0x9f,0xcd,0xd6,0x26,0x8c,0x95,0xb9,0x08);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("cefb0920-7a8f-11d8-9fcd-d6268c95b908")
    ITableDoc : public ICfgDoc
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFileAs( 
            /* [in] */ BSTR FileName,
            /* [defaultvalue][optional][in] */ TableSaveTypes type,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableDocVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableDoc __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableDoc __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableDoc __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ DocTypes __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Kind )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            ITableDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsOpen )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            ITableDoc __RPC_FAR * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ITableDoc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToFile )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadFromFile )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToFileAs )( 
            ITableDoc __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [defaultvalue][optional][in] */ TableSaveTypes type,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);
        
        END_INTERFACE
    } ITableDocVtbl;

    interface ITableDoc
    {
        CONST_VTBL struct ITableDocVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableDoc_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableDoc_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableDoc_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableDoc_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableDoc_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ITableDoc_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ITableDoc_get_Kind(This,pVal)	\
    (This)->lpVtbl -> get_Kind(This,pVal)

#define ITableDoc_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define ITableDoc_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define ITableDoc_get_IsOpen(This,pVal)	\
    (This)->lpVtbl -> get_IsOpen(This,pVal)

#define ITableDoc_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define ITableDoc_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)


#define ITableDoc_SaveToFile(This,FileName,bSucces)	\
    (This)->lpVtbl -> SaveToFile(This,FileName,bSucces)

#define ITableDoc_LoadFromFile(This,FileName,bSuccess)	\
    (This)->lpVtbl -> LoadFromFile(This,FileName,bSuccess)

#define ITableDoc_SaveToFileAs(This,FileName,type,bSucces)	\
    (This)->lpVtbl -> SaveToFileAs(This,FileName,type,bSucces)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableDoc_SaveToFile_Proxy( 
    ITableDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);


void __RPC_STUB ITableDoc_SaveToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableDoc_LoadFromFile_Proxy( 
    ITableDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSuccess);


void __RPC_STUB ITableDoc_LoadFromFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableDoc_SaveToFileAs_Proxy( 
    ITableDoc __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [defaultvalue][optional][in] */ TableSaveTypes type,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bSucces);


void __RPC_STUB ITableDoc_SaveToFileAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableDoc_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_TableDoc,0xcefb0921,0x7a8f,0x11d8,0x9f,0xcd,0xd6,0x26,0x8c,0x95,0xb9,0x08);

#ifdef __cplusplus

class DECLSPEC_UUID("cefb0921-7a8f-11d8-9fcd-d6268c95b908")
TableDoc;
#endif

DEFINE_GUID(CLSID_DialogDoc,0xa1c16c44,0xabb4,0x48da,0xa7,0x7d,0x1e,0xa3,0x49,0xbb,0xe1,0x97);

#ifdef __cplusplus

class DECLSPEC_UUID("a1c16c44-abb4-48da-a77d-1ea349bbe197")
DialogDoc;
#endif

DEFINE_GUID(CLSID_CfgWindow,0x06c7e522,0xcbc8,0x431b,0xb6,0xad,0xc3,0x67,0x15,0x2f,0xe1,0x89);

#ifdef __cplusplus

class DECLSPEC_UUID("06c7e522-cbc8-431b-b6ad-c367152fe189")
CfgWindow;
#endif

DEFINE_GUID(CLSID_CfgWindows,0x765e4da1,0x4951,0x11d8,0x8d,0x5f,0xad,0xb2,0x8b,0x91,0xf4,0x06);

#ifdef __cplusplus

class DECLSPEC_UUID("765e4da1-4951-11d8-8d5f-adb28b91f406")
CfgWindows;
#endif

DEFINE_GUID(CLSID_CfgTimer,0x62520e90,0xffc6,0x4a86,0x94,0xdd,0x83,0xbb,0xc1,0x94,0x35,0xcb);

#ifdef __cplusplus

class DECLSPEC_UUID("62520e90-ffc6-4a86-94dd-83bbc19435cb")
CfgTimer;
#endif
#endif /* __OpenConf_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
