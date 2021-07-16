// Авторские права - VTOOLS.RU (info@vtools.ru)
// ModuleManager.h: interface for the CModuleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ModuleManager_H__14109D1F_1492_4BF0_A344_0B8ED648F256__INCLUDED_)
#define AFX_ModuleManager_H__14109D1F_1492_4BF0_A344_0B8ED648F256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Compiler/CompileModule.h"
#include "Compiler/ProcUnit.h"

#include "MetaData.h"

class CModuleManager* AfxGetModuleManager();
enum
{
	TYPE_NONE=0,
	TYPE_OBJECT_CONTEXT,
	TYPE_METADATA_CONTEXT,
};
class CModuleManager  
{
public:
	CModuleManager();
	virtual ~CModuleManager();

	//АТРИБУТЫ:
	CMetadata *pMetadata;//данные конфигурации
	CMapStringToPtr ListModules;//список скомилированных модулей конфигурации
	CMapStringToPtr ListRunModules;//список запущенных модулей


	CDocument *pTree;

private:
	CCompileModule *pCommonParent;	//общий родитель всех объектов (складывается из общих модулей)
	CProcUnit *pCommonRunParent;	//запущенный общий родитель всех объектов (складывается из общих модулей)

	//список родителей объектов (если у объекта есть общие модули, то 
	//их родителем является Общие модули конфигурации -> Общие модули объекта)
	CMapStringToPtr ListObjectParent;
	CMapStringToPtr ListObjectRunParent;

	//масив для задания порядка запуска (сначала Модуль инициализации, затем объектов, заием конфигурации)
	CArray <CProcUnit *,CProcUnit *> aRunModules;


private:
	//static CModuleManager* afx_ModuleManager;
	BOOL bConfigMode;

public:
	CString csMetadataPath;

	//МЕТОДЫ:
	BOOL GetAttrVariable(CString csModuleName,CArray<CString,CString> &aVariable,CArray<CString,CString> &aTypeVariable);
private:
	void GetAttrVariableFromMetaArray(class CValueArray *pArray,CArray<CString,CString> &aVariable,CArray<CString,CString> &aTypeVariable);
public:

	void SetAttrVariable(CCompileModule *pModule);

	CString GetObjectName(CString csModulePath,int &nType);
	BOOL FindObject(CString csFormName0,CString csFileName,CMetaObject* &pObject,CString csObjGroup,CString csCommonObjGroup);
	BOOL FindForm(CString csFormName0,CString csFileName,CMetaObject* &pObject);
	CString GetTable(CString csFileName);
	CString GetModule(CString csFileName);
	int RunFormModal(CString csFormName,CValue Param,CString csFileName="",CString csFormID="");
	int RunForm(CString csFormName0,CValue Param,CString csFileName="",CString csFormID="",int nMode=0);

	BOOL IsCanDestroy();
	CCompileModule *GetParent(CString csPath,CCompileModule *&pStopParent,CCompileModule *&pContinueParent,BOOL bEnterpriseModule=1);
	CProcUnit *GetRunParent(CString csPath);
	CProcUnit *GetRunParent(CCompileModule *pCompile);
	//загрузка и инициализация конфигурации (компиляция общих модулей и макрообъектов конфигурации)
	void Load(BOOL bRunFunction);
	//извлечение модуля конфигурации (если еще не скомилирован - то производится компиляция)
	CCompileModule *GetCompileModule(CString csModuleName,CCompileModule *pSetParent=0,CCompileModule *pSetStopParent=0,BOOL bRetZero=0);
	CProcUnit *GetNewRunModule(CString csModuleName,BOOL bRunModule=1,CProcUnit *pSetRunParent=0);//запускает новую копию модуля на исполнение - возвращает новый объект - контекст исполнения

	//получение уже адреса запущенного модуля (если такого нет, то он запускается)
	CProcUnit *GetRunModule(CString csModuleName,BOOL bRunModule=0,CProcUnit *pSetRunParent=0);


//	static CModuleManager* AfxGetModuleManager(){return afx_ModuleManager;};
private:
	//извлечение и компилирование модуля конфигурации
	CCompileModule *Compile2(CString csModuleName,CCompileModule *pSetParent=0);
	void Clear();
	void ClearCompileList(CMapStringToPtr &List);
	void ClearRunList(CMapStringToPtr &List);
	
};

#endif // !defined(AFX_ModuleManager_H__14109D1F_1492_4BF0_A344_0B8ED648F256__INCLUDED_)
