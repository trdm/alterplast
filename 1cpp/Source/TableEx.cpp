
#include "stdafx.h"
#include "TableEx.h"

#ifdef VK_1CPP_SourceTable

DWORD CTableWrapEx::dwCSheetDocVTable = 0;
int CTableWrapEx::iMethNum_SourceTable = -1;
int CTableWrapEx::iMethNum_Options = -1;
int CTableWrapEx::iCountStream = 0;
CMetaDataCont* CTableWrapEx::pMetaDataCont = NULL;

class SDocNames{
	DWORD m_id;
	CString m_strFolder;
public:

	SDocNames(DWORD id, const CString& strFolder) : m_id(id), m_strFolder(strFolder) {};
	LPCSTR GetFolderName() const { return m_strFolder; };
	DWORD GetID() const { return m_id; };
};

CIStringMap<SDocNames*, SDocNames*> DocNamesMap;

class CSheetViewMy: public CSheetView
{
public:
	virtual void  OnInitialUpdate(void)
	{
		CSheetView::OnInitialUpdate();
	};

};

typedef int(CBLContext::*P_CONT_CALLASP)(int,CValue * *);
typedef int(CBLContext::*P_CONT_CALLASF)(int,CValue &,CValue * *);
typedef int(CBLContext::*P_CONT_GETNPARAMS)(int);
typedef int(CBLContext::*P_CONT_HASRETVAL)(int);
typedef int(CBLContext::*P_CONT_GETPARAMDEFVALUE)(int,int,CValue*);
typedef void(CBLContext::*P_CONT_DESTRUCTOR)(void);

static P_CONT_CALLASP pCallAsProcDbl;

static P_CONT_CALLASP pCallAsProc;
static P_CONT_CALLASF pCallAsFunc;
static P_CONT_GETNPARAMS pGetNParams;
static P_CONT_GETPARAMDEFVALUE pGetParamDefValue;
static P_CONT_HASRETVAL pHasRetVal;
static P_CONT_DESTRUCTOR pDestructor;

int CTableWrapEx::procAddAllMDInnerFormsName(class CString &str,long l1,long l2,enum PageType pt)
{
	if(!str.CollateNoCase("ModuleText"))
		return 1;

	//CMetaDataCont *pMD = GetMetaData();
	CMetaDataObj* pMDO = pMetaDataCont->FindObject(l1);
	CString str1 = GetModuleFullName(l1, str, 0, pMetaDataCont);
	//str1.MakeUpper(); // не нужно менять регистр, т.к. CIStringMap использует регистронезависимый поиск, т.е. еще некоторое ускорение !

	SDocNames *pDocNames = new SDocNames(l1, str); // TODO нет удаления !!
	DocNamesMap.SetAt(str1, pDocNames);

	return 1;
}

class COleStorageGuard
{
	COleStorage& m_OleStorage;
public:
	COleStorageGuard(COleStorage& OleStorage) : m_OleStorage(OleStorage) {};
	~COleStorageGuard() { m_OleStorage.Release(TRUE); };
};

class COleStreamFileGuard
{
	COleStorage& m_OleStorage;
	COleStreamFile* m_pOleStreamFile;
public:
	COleStreamFileGuard(COleStorage& OleStorage, COleStreamFile* pOleStreamFile) : 
	  m_OleStorage(OleStorage), m_pOleStreamFile(pOleStreamFile) {};
	  ~COleStreamFileGuard() { Destroy(); };
	
	void Destroy()
		{ if (m_pOleStreamFile) {
				m_OleStorage.ReleaseStream(m_pOleStreamFile, TRUE);
				m_pOleStreamFile = NULL; }
		};
};

//CTableWrapEx::CSetTableReturnValue CTableWrapEx::SetTable(CValue* pNameValue, const CString& strNameSrc, const CString& strPathSrc)
CTableWrapEx::CSetTableReturnValue CTableWrapEx::SetTable(const CString& strNameSrc, const CString& strPathSrc)
{
	CString strPath = AllTrim(strPathSrc);
	CString strName = strNameSrc;
	
	if(strPath.IsEmpty())
		return SET_TBL_NEED_SYSTEM_CALL; //1

	strPath.MakeUpper();
	
	ReplaceClassNameToMDName(strPath);
	
	if(IsMDInnerFormPath(strPath))
		return SetTableForMDInnerForms(strName, strPath);

	return SetTableForExternalFiles(strName, strPath);
}

CTableWrapEx::CSetTableReturnValue CTableWrapEx::SetTableForMDInnerForms(CString& strName, const CString& strPath)
{
	CString strDoc = strPath.Mid(3);
	
	SDocNames *pDocNames = NULL;
	if(!DocNamesMap.Lookup(strDoc, pDocNames)) //if(!DocNamesArray.Lookup(strDoc, (void*&)pDocNames))
		return SET_TBL_PATH_ERROR; //2;

	CApp7* pApp7 = (CApp7*) AfxGetApp();
	CProfile7* pProfile7 = pApp7->GetProps();
	CString Prop8 = pProfile7->GetStringProp(0x08); // полный путь к файлу 1Cv7.MD (с именем файла)
	CString strFull = "";
	strFull.Format("%s\\%s\\%s_Number%d\\WorkBook", Prop8, pDocNames->GetFolderName(), pDocNames->GetFolderName(), pDocNames->GetID());
	
	CWorkBookDoc* pDoc = static_cast<CWorkBookDoc*>(CConfigCont::PathToDocument(strFull));
	if(!pDoc)
	{
		CString str;
		CTypedCont *pTypedCont;
		if(CConfigCont::IDToPath(pDocNames->GetID(), pDocNames->GetFolderName(), str, &pTypedCont,0))
			pDoc = pTypedCont->GetWorkBook();
	}
	if(!pDoc)
		return SET_TBL_OTHER_ERROR; //4;

	IfEmptyNameThenSetFromFirstTableName(pDoc, strName);

	if (!FindDocumentByName(pDoc, strName))
 		return SET_TBL_NAME_ERROR; // 3

	SetSheetAndUpdateAllViewsForSheetDocIn();

	return SET_TBL_SUCCESS; // 0
}

CTableWrapEx::CSetTableReturnValue CTableWrapEx::SetTableForExternalFiles(CString& strName, const CString& strPath)
{
	if(IsExternalMxlFilePath(strPath))
	{
		//*pNameValue = strPath;
		return SET_TBL_NEED_REPLACE_NAME_FROM_PATH_AND_NEED_SYSTEM_CALL; //1;
	}
	
	CSetTableReturnValue ResultValue;
	if (SetTableForOpenedErtFile(strName, strPath, ResultValue))
		return ResultValue;

	return SetTableForNotOpenedErtFile(strName, strPath);
}

bool CTableWrapEx::SetTableForOpenedErtFile(CString& strName, const CString& strPath, CSetTableReturnValue& ResultValue)
{
	CTemplate7* pTempl = CTemplate7::FromID(0xBA);

	if(pTempl == NULL){
		ResultValue = SET_TBL_OTHER_ERROR; //4;
		return true;
	}

	POSITION pos = pTempl->GetFirstDocPosition();
	while(pos)
	{
		CWorkBookDoc* pTemplDoc = static_cast<CWorkBookDoc*>(pTempl->GetNextDoc(pos));
		if(pTemplDoc)
		{
			CString strSrc = pTemplDoc->GetSourcePathName();
			if(strSrc.CompareNoCase(strPath))
				continue;

			IfEmptyNameThenSetFromFirstTableName(pTemplDoc, strName);

			if (!FindDocumentByName(pTemplDoc, strName)){
 				ResultValue = SET_TBL_NAME_ERROR; // 3
				return true;
			}

			SetSheetAndUpdateAllViewsForSheetDocIn();

			ResultValue = SET_TBL_SUCCESS; //0;
			return true;
		}
	}

	return false;
}

CTableWrapEx::CSetTableReturnValue CTableWrapEx::SetTableForNotOpenedErtFile(CString& strName, const CString& strPath)
{
	CTemplate7* pTempl = CTemplate7::FromID(0xA1);

	if(pTempl == NULL)
		return SET_TBL_OTHER_ERROR; //4;

	LPCSTR sz_Moxel_WorkPlace = "Moxel WorkPlace";
	if(strName.IsEmpty())
		strName = "Moxel WorkPlace";

	COleStorage pOleStorage(NULL); 
	CFileException excepFile;
	if(!pOleStorage.Open(strPath, CFile::shareExclusive, &excepFile))
		return SET_TBL_PATH_ERROR; //2;

	COleStorageGuard OleStorageGuard(pOleStorage);

	COleStreamFile * pOleStream = pOleStorage.OpenStream(CContainer::GetContentsStreamName(), CFile::shareExclusive, &excepFile);
	if(!pOleStream)
		return SET_TBL_OTHER_ERROR; //4;
	
	COleStreamFileGuard OleStreamFileGuard(pOleStorage, pOleStream);

	DWORD StreamLen = pOleStream->GetLength();
	vector<char> buf(StreamLen); //char* buf = (char*)malloc(StreamLen);
	pOleStream->Read(&buf[0], StreamLen);
	
	int n = 0;

	CItemList list1;
	list1.LoadFromString(&buf[0], &n);
	
	CString strStreamName = "";
	LPCSTR sz_Moxcel_Worksheet = "Moxcel.Worksheet";
	for(int j = list1.FindGenericItem(sz_Moxcel_Worksheet, -1); j >= 0; j = list1.FindGenericItem(sz_Moxcel_Worksheet, ++j))
	{
		CItem* pItem = (CItem*)list1.GetItemAt(j);
		const int w = pItem->WhatIsIt();
		if(w != 1)
			continue;

		CGenericItem* pGenericItem = reinterpret_cast<CGenericItem*>(pItem);
		CItem* pItem1 = static_cast<CItem*>(pGenericItem->m_array.GetAt(1));
		CItem* pItem2 = static_cast<CItem*>(pGenericItem->m_array.GetAt(2));
		
		CString& strItem2String = pItem2->m_str;
		if (!strName.CompareNoCase(strItem2String) ||
			(!strItem2String.CompareNoCase("Moxel WorkPlace") &&
			 !strName.CompareNoCase("Таблица")))
		{
			strStreamName = pItem1->m_str;			
			break; // artbear TODO скорее всего, здесь правильнее break !!
		}
	}
	//free(buf);
	OleStreamFileGuard.Destroy();//pOleStorage.ReleaseStream(pOleStream, TRUE);

	if(strStreamName.IsEmpty())
		return SET_TBL_NAME_ERROR; //3;

	pOleStream = pOleStorage.OpenStream(strStreamName, CFile::shareExclusive, &excepFile);
	if(!pOleStream)
		return SET_TBL_OTHER_ERROR; //4;

	COleStreamFileGuard OleStreamFileGuard2(pOleStorage, pOleStream);

	CArchive ar(pOleStream, CArchive::load);
	
	DeleteOwnCreatedSheetDoc(m_pSheetDocIn);

	m_pSheetDocIn = static_cast<CSheetDoc*>(CSheetDoc::CreateObject());//new CSheetDoc;
//							CTemplate7::RegisterDocument(m_pSheetDocIn);
//							pTempl->AddDocument(m_pSheetDocIn);

	m_pSheetDocIn->DeleteContents();
	m_pSheetDocIn->Serialize(ar);

	SetSheetAndUpdateAllViewsForSheetDocIn();

	CTemplate7::UnRegisterDocument(m_pSheetDocIn);
	//pOleStorage.ReleaseStream(pOleStream, TRUE);
	
	return SET_TBL_SUCCESS; //0;

	//pOleStorage.Release(TRUE);
}

// пытаемся очистить ранее созданные нами CSheetDoc
void CTableWrapEx::DeleteOwnCreatedSheetDoc(CSheetDoc* pSheetDoc)
{
	if(pSheetDoc)
		if(!CTemplate7::GetDocumentID(pSheetDoc))
			delete pSheetDoc;
}

void CTableWrapEx::ReplaceClassNameToMDName(CString& strPath)
{
	int nn = strPath.Find("@MD");
	if(nn >= 0)
	{
		CString strPathTemp = strPath.Left(nn);
		strPath = CString("MD.Обработка.")+AllTrim(strPathTemp)+".Форма";
		strPath.MakeUpper();
	}
}

void CTableWrapEx::SetSheetAndUpdateAllViewsForSheetDocIn()
{
	DWORD* pdw = (DWORD*)m_pSheetDocIn;
	m_pSheet = (CSheet*)&pdw[0x2C];
	if(m_pSheetDocOut && (CTemplate7::GetDocumentID(m_pSheetDocOut) != 0))
	{
		m_pSheetDocOut->UpdateAllViews(NULL, 8, NULL);
	}
}

void CTableWrapEx::IfEmptyNameThenSetFromFirstTableName(CWorkBookDoc* pDoc, CString& strName)
{
	if(!strName.IsEmpty())
		return;

	CStringArray cStringArray;
	pDoc->GetTablesName(cStringArray);
	if(cStringArray.GetSize() > 0)
		strName = cStringArray.GetAt(0);
}

bool CTableWrapEx::FindDocumentByName(CWorkBookDoc* pDoc, LPCSTR szName)
{
	int n = 0;
	CSheetDoc* pSheetDoc = (CSheetDoc*)pDoc->FindDocument(szName, &n);
	if(!pSheetDoc)
		return false;

	m_pSheetDocIn = pSheetDoc;

	return true;
}

void CTableWrapEx::DecrRef(void)
{
	int aRefCount = m_RefCount;
	CSheetDoc* pSheetDoc = m_pSheetDocIn;

	(this->*pDestructor)();

	// пытаемся очистить ранее созданные нами CSheetDoc
	// artbear: временная заглушка - код sedmin-а из 203
	// ---  так было  -----
	//if((--aRefCount == 0) && pSheetDoc && (*(DWORD*)pSheetDoc == dwCSheetDocVTable) && (CTemplate7::GetDocumentID(pSheetDoc) == 0))
	//		delete pSheetDoc;
	// ------- ---------
	//  * Source/TableEx.cpp [z_1cpp_2-0-3] 1.9.4.4:
    //	[-] устранена ошибка, приводившая к падению 1С во время печати документа
	{

		if (--aRefCount || !pSheetDoc) return;
		
		try {
			if (*(DWORD*)pSheetDoc != dwCSheetDocVTable) return;
		}
		catch (...) {}
		
		DeleteOwnCreatedSheetDoc(pSheetDoc);
	}
}

int CTableWrapEx::CallAsProc(int iMethNum,class CValue **ppValue)
{
	//if(strMeth.CompareNoCase("ИсходнаяТаблица") == 0)
	if(iMethNum == iMethNum_SourceTable)
	{
		int result;
		CValue dummy;
		if (RunSourceTableMethod(*ppValue[0], *ppValue[1], *ppValue[2], dummy, result))
			return result;
		
		return CallOriginalMethod_SourceTable(this, ppValue);
	}
	//else if(strMeth.CompareNoCase("Опции") == 0)
	else if(iMethNum == iMethNum_Options)
		return RunOptionsMethod(ppValue);

	return (this->*pCallAsProc)(iMethNum, ppValue);
};

int CTableWrapEx::CallAsFunc(int iMethNum,class CValue & rValue,class CValue **ppValue)
{
	//if(strMeth.CompareNoCase("ИсходнаяТаблица") == 0)
	if(iMethNum == iMethNum_SourceTable)
	{
		int result;
		if (RunSourceTableMethod(*ppValue[0], *ppValue[1], *ppValue[2], rValue, result))
			return result;

		return CallOriginalMethod_SourceTable(this, ppValue);
	}

	return (this->*pCallAsFunc)(iMethNum, rValue, ppValue);
};

int CTableWrapEx::CallOriginalMethod_SourceTable(CBLContext* pCont, class CValue **ppValue)
{
	return (pCont->*pCallAsProc)(iMethNum_SourceTable, ppValue);
};

bool CTableWrapEx::RunSourceTableMethod(CValue& pNameValue, const CValue& pPathValue, const CValue& pExceptionFlagValue, CValue& rValue, int& ResultValue)
{
	CSetTableReturnValue res = SetTable(pNameValue.GetString(), pPathValue.GetString());

	if (SET_TBL_NEED_REPLACE_NAME_FROM_PATH_AND_NEED_SYSTEM_CALL == res){
		res = SET_TBL_NEED_SYSTEM_CALL;

		pNameValue = pPathValue;
	}
	
	const CSetTableReturnValue result = res;
	rValue = result;

	const int bCallExept = pExceptionFlagValue.GetNumeric();
	
	if(bCallExept == 0){
		ResultValue = S_OK;
		return true;
	}

	if(result == SET_TBL_SUCCESS) { //0){
		rValue = 1L;
		ResultValue = S_OK;
		return true;
	}
	else if(result != SET_TBL_NEED_SYSTEM_CALL) //1)
	{
		if(result == SET_TBL_PATH_ERROR) //2)
			RuntimeError("Неверно задан путь к исходной таблице!");
		else if(result == SET_TBL_NAME_ERROR) //3)
			RuntimeError("Неверно задано имя исходной таблицы!");
		//else
			RuntimeError("Ошибка при установке исходной таблицы!");

		//ResultValue = S_FALSE;
		//return true;
	}
	
	rValue = 2L;
	return false;
}

int CTableWrapEx::RunOptionsMethod(CValue** ppValues)
{
	int result = (this->*pCallAsProc)(iMethNum_Options, ppValues);

	POSITION pos = m_pSheetDocOut->GetFirstViewPosition();
	while(pos)
	{
		CSheetViewMy* pSheetView = (CSheetViewMy*)m_pSheetDocOut->GetNextView(pos);
		CSheetSelection pSel = *(pSheetView->GetTailSelection());
		int nPosVert = pSheetView->GetScrollPos(SB_VERT);
		int nPosHorz = pSheetView->GetScrollPos(SB_HORZ);
		pSheetView->OnInitialUpdate();
		pSheetView->ClearSelection();
		pSheetView->SelectArea(pSel);
		pSheetView->ScrollToPosition(nPosHorz,nPosVert);
	}

	return result;
}

int CTableWrapEx::HasRetVal(int iMethNum)const
{
	//if(strMeth.CompareNoCase("ИсходнаяТаблица") == 0)
	if(iMethNum == iMethNum_SourceTable)
		return 1;

	return (this->*pHasRetVal)(iMethNum);
};

int CTableWrapEx::GetNParams(int iMethNum)const
{
	//if(strMeth.CompareNoCase("ИсходнаяТаблица") == 0)
	if(iMethNum == iMethNum_SourceTable)
		return 3;

	return (this->*pGetNParams)(iMethNum);
};

int CTableWrapEx::GetParamDefValue(int iMethNum,int iParamNum,class CValue * pDefValue)const
{
	//if(strMeth.CompareNoCase("ИсходнаяТаблица") == 0)
	if(iMethNum == iMethNum_SourceTable)
	{
		if(iParamNum == 1)
		{
			*pDefValue = "";
			return 1;
		}
		if(iParamNum == 2)
		{
			*pDefValue = 1;
			return 1;
		}
	}

	return (this->*pGetParamDefValue)(iMethNum, iParamNum, pDefValue);
};

union{
	void* pV;
	P_CONT_CALLASP pCallAsProcU;
	P_CONT_CALLASF pCallAsFuncU;
	P_CONT_GETNPARAMS pGetNParamsU;
	P_CONT_GETPARAMDEFVALUE pGetParamDefValueU;
	P_CONT_HASRETVAL pHasRetValU;
	P_CONT_DESTRUCTOR pDestructorU;
	DWORD dwF;
} ConvCallTab;

void CTableWrapEx::InitTableWrap()
{
#ifndef VK_1CPP_SourceTable
	return;
#endif
//	if (!pMainAddIn->Property["EnableTableSourceTable"])
//		return ;
	
	ChangeTableOutputContextVirtualTable();

	ChangeGroupContextVirtualTable();

	iCountStream = 0;
	pMetaDataCont = GetMetaData();
	CConfigCont::GetAllTypedItem(procAddAllMDInnerFormsName,0,ptDialog,0);
};

void CTableWrapEx::ChangeTableOutputContextVirtualTable()
{
	CBLPtr<CBLContext> PtrTableOutputContext("Table");
	if(!PtrTableOutputContext)
		return;
	
	CBLContext* pTableOutputContext = PtrTableOutputContext;

	auto_ptr<CTableWrapEx> pTableWrapEx(new CTableWrapEx);

	DWORD* pTableOutputContext_VirtualTable = (DWORD*)*(DWORD*)pTableOutputContext;
	DWORD* pTableWrapper_VirtualTable = (DWORD*)*(DWORD*)pTableWrapEx.get();

	DWORD old;
	VirtualProtect(pTableOutputContext_VirtualTable,0x100,PAGE_EXECUTE_READWRITE,&old);

	CSheetDoc mSheetDoc;
	dwCSheetDocVTable = *(DWORD*)&mSheetDoc;

	//*/
	const int iDecrRefMethodNumber = 0x06;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable + iDecrRefMethodNumber);
	pDestructor = ConvCallTab.pDestructorU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable + iDecrRefMethodNumber);
	P_CONT_DESTRUCTOR pD = ConvCallTab.pDestructorU;
	DWORD dwDestructor = *(DWORD*)&pD;
	pTableOutputContext_VirtualTable[iDecrRefMethodNumber] = dwDestructor;
	//*/

	//*/
	const int iCallAsFuncMethodNumber = 0x20;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable + iCallAsFuncMethodNumber);
	pCallAsFunc = ConvCallTab.pCallAsFuncU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable + iCallAsFuncMethodNumber);
	P_CONT_CALLASF pF = ConvCallTab.pCallAsFuncU;
	DWORD dwCallAsFunc = *(DWORD*)&pF;
	pTableOutputContext_VirtualTable[iCallAsFuncMethodNumber] = dwCallAsFunc;
	//*/

	//*/
	const int iCallAsProcMethodNumber = 0x1F;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable+iCallAsProcMethodNumber);
	pCallAsProc = ConvCallTab.pCallAsProcU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable+iCallAsProcMethodNumber);
	P_CONT_CALLASP pP = ConvCallTab.pCallAsProcU;
	DWORD dwCallAsProc = *(DWORD*)&pP;
	pTableOutputContext_VirtualTable[iCallAsProcMethodNumber] = dwCallAsProc;
	//*/

	//*/
	const int iGetNParamsMethodNumber = 0x1C;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable+iGetNParamsMethodNumber);
	pGetNParams = ConvCallTab.pGetNParamsU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable+iGetNParamsMethodNumber);
	P_CONT_GETNPARAMS pGP = ConvCallTab.pGetNParamsU;
	DWORD dwGetNParams = *(DWORD*)&pGP;
	pTableOutputContext_VirtualTable[iGetNParamsMethodNumber] = dwGetNParams;
	//*/

	//*/
	const int iGetParamDefValueMethodNumber = 0x1D;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable+iGetParamDefValueMethodNumber);
	pGetParamDefValue = ConvCallTab.pGetParamDefValueU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable+iGetParamDefValueMethodNumber);
	P_CONT_GETPARAMDEFVALUE pGD = ConvCallTab.pGetParamDefValueU;
	DWORD dwGetParamDefValue = *(DWORD*)&pGD;
	pTableOutputContext_VirtualTable[iGetParamDefValueMethodNumber] = dwGetParamDefValue;
	//*/

	//*/
	const int iHasRetValMethodNumber = 0x1E;
	ConvCallTab.dwF = *(pTableOutputContext_VirtualTable+iHasRetValMethodNumber);
	pHasRetVal = ConvCallTab.pHasRetValU;
	ConvCallTab.dwF = *(pTableWrapper_VirtualTable+iHasRetValMethodNumber);
	P_CONT_HASRETVAL pHRV = ConvCallTab.pHasRetValU;
	DWORD dwHasRetVal = *(DWORD*)&pHRV;
	pTableOutputContext_VirtualTable[iHasRetValMethodNumber] = dwHasRetVal;
	//*/

	iMethNum_SourceTable = pTableOutputContext->FindMethod("SourceTable");
	iMethNum_Options = pTableOutputContext->FindMethod("Options");

	//delete pTableWrapEx;
}

void CTableWrapEx::ChangeGroupContextVirtualTable()
{
	auto_ptr<CTableWrapExDbl> pTableWrapExDbl(new CTableWrapExDbl);

	/* artbear CGroupContext* pGrCont = (CGroupContext*)(RUNTIME_CLASS(CGroupContext)->CreateObject());*/
	CBLPtr<CGroupContext> ptrGrCont("ГрупповойКонтекст");
	if(ptrGrCont)
	{
		CGroupContext* pGroupContext = ptrGrCont;// artbear

		DWORD* pGroupContext_VirtualTable = (DWORD*)*(DWORD*)pGroupContext;
		DWORD* pTableWrapperExDbl_VirtualTable = (DWORD*)*(DWORD*)pTableWrapExDbl.get();

		DWORD old;
		VirtualProtect(pGroupContext_VirtualTable, 0x100, PAGE_EXECUTE_READWRITE, &old);

	 	const int iCallAsProcMethodNumber = 0x1F;
		ConvCallTab.dwF = *(pGroupContext_VirtualTable+iCallAsProcMethodNumber);
		pCallAsProcDbl = ConvCallTab.pCallAsProcU;
		ConvCallTab.dwF = *(pTableWrapperExDbl_VirtualTable+iCallAsProcMethodNumber);
		P_CONT_CALLASP pP = ConvCallTab.pCallAsProcU;
		DWORD dwCallAsProc = *(DWORD*)&pP;
		pGroupContext_VirtualTable[iCallAsProcMethodNumber] = dwCallAsProc;

		// delete pGrCont; // artbear
	}
	//*/

	//delete pTableWrapExDbl;
}

int CTableWrapExDbl::CallAsProc(int iMethNum,class CValue **ppValue)
{
	int ret = (this->*pCallAsProcDbl)(iMethNum, ppValue);
	
	CString strMethodNameAsOptions = this->GetMethodName(iMethNum, 1);
//Msg("CTableWrapExDbl::CallAsProc strMeth = %s", strMeth);
	if(strMethodNameAsOptions.CompareNoCase("Опции"))
		return ret;

	int sz = GetSize();
	for(int i = 0; i < sz; i++)
	{
		CBLContext* pCont = GetContext(i);
		if(!pCont)
			continue;

		int iMethodNumber = pCont->FindMethod(strMethodNameAsOptions);
		if(iMethodNumber < 0)
			continue;

		CTableWrapEx* pTabl = (CTableWrapEx*)pCont;
		POSITION pos = pTabl->GetSheetDocOut()->GetFirstViewPosition();
		while(pos)
		{
			CSheetViewMy* pSheetView = (CSheetViewMy*)pTabl->GetSheetDocOut()->GetNextView(pos);
			pSheetView->OnInitialUpdate();
		}
	}

	return ret;
};

#endif