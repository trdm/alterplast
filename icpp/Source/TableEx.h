
//////////////////////////////////////////////////////////////////////
// TableEx.h: interface for CTableEx class.
//
// Разработчик: Артур Аюханов aka artbear, mailto: artbear@inbox.ru
// Версия: 1.5
//////////////////////////////////////////////////////////////////////

#pragma once

//#undef VK_1CPP_SourceTable
#define VK_1CPP_SourceTable
#ifdef VK_1CPP_SourceTable

// класс для перехвата объекта Таблица
//
// artbear TODO нужно переделать на другую, более логичную схему, где объект-перехвата и сам объект-обработчик перехвата не находятся в одном классе,
// т.к. сейчас есть путаница между данными таблицы (1С) и данными нашего класса-обработчика перехвата
//
class CTableWrapEx : public CBLContext // на самом деле это CTableOutputContext
{
private:
	CSheetDoc* m_pSheetDocOut; //20
	DWORD dwFlag1; //24
	DWORD dwFlag2; //28
	CSheetDoc* m_pSheetDocIn; //2C
	DWORD dwFlag3; //30
	DWORD dwFlag4; //34
	CSheet* m_pSheet; //38
	DWORD dwFlag6; //3C
	DWORD dwFlag7; //40
	DWORD dwFlag8;
	DWORD dwFlag9;
	DWORD dwFlag10;
	DWORD dwFlag11;
	
public:

	virtual void DecrRef(void);

	virtual int GetNParams(int iMethNum)const;
	virtual int GetParamDefValue(int iMethNum,int iParamNum,CValue *)const;
	virtual int HasRetVal(int iMethNum)const;
	virtual int CallAsProc(int iMethNum,CValue **ppValue);
	virtual int CallAsFunc(int iMethNum,CValue & rValue,CValue **ppValue);

	CSheetDoc* GetSheetDocOut() const { return m_pSheetDocOut; }; 

	static void InitTableWrap();
	
private:
	enum CSetTableReturnValue {
		SET_TBL_SUCCESS = 0,			// 0 - таблица установлена
		SET_TBL_NEED_SYSTEM_CALL = 1,	// 1 - необходим вызов штатной обработки
		SET_TBL_PATH_ERROR = 2,		// 2 - ошибка пути
		SET_TBL_NAME_ERROR = 3,		// 3 - ошибка имени таблицы
		SET_TBL_OTHER_ERROR = 4,		// 4 - прочие ошибки

		SET_TBL_NEED_REPLACE_NAME_FROM_PATH_AND_NEED_SYSTEM_CALL = 5,		// 5 - заменить имя таблицы на путь к таблице
	};

	CSetTableReturnValue SetTable(const CString& strName, const CString& strPath);

	CSetTableReturnValue SetTableForMDInnerForms(CString& strName, const CString& strPath);
	CSetTableReturnValue SetTableForExternalFiles(CString& strName, const CString& strPath);
	bool SetTableForOpenedErtFile(CString& strName, const CString& strPath, CSetTableReturnValue& ResultValue);
	CSetTableReturnValue CTableWrapEx::SetTableForNotOpenedErtFile(CString& strName, const CString& strPath);

	static void ChangeTableOutputContextVirtualTable();
	static void ChangeGroupContextVirtualTable();

	bool RunSourceTableMethod(CValue& pNameValue, const CValue& pPathValue, const CValue& pExceptionFlagValue, CValue& rValue, int& ResultValue);
	int RunOptionsMethod(CValue** ppValues);

	static void ReplaceClassNameToMDName(CString& strPath);
	void SetSheetAndUpdateAllViewsForSheetDocIn();
	static void DeleteOwnCreatedSheetDoc(CSheetDoc* pSheetDoc);
	static void IfEmptyNameThenSetFromFirstTableName(CWorkBookDoc* pDoc, CString& strName);
	bool FindDocumentByName(CWorkBookDoc* pDoc, LPCSTR szName);

	static bool IsMDInnerFormPath(const CString& strPath) { return !strPath.Left(3).CompareNoCase("MD."); };
	static bool IsExternalMxlFilePath(const CString& strPath) { return !strPath.Right(3).CompareNoCase("MXL"); };

	static int CallOriginalMethod_SourceTable(CBLContext* pCont, CValue **ppValue);

	static int procAddAllMDInnerFormsName(CString &str,long l1,long l2,enum PageType pt);

	static int iMethNum_SourceTable;
	static int iMethNum_Options;
	static DWORD dwCSheetDocVTable;
	static int iCountStream;
	static CMetaDataCont *pMetaDataCont;
};

class CTableWrapExDbl : public CGroupContext
{
public:
	CSheetDoc* m_pSheetDocOut; //20
	DWORD dwFlag1; //24
	DWORD dwFlag2; //28
	CSheetDoc* m_pSheetDocIn; //2C
	DWORD dwFlag3; //30
	DWORD dwFlag4; //34
	CSheet* m_pSheet; //38
	DWORD dwFlag6; //3C
	DWORD dwFlag7; //40
	DWORD dwFlag8;
	DWORD dwFlag9;
	DWORD dwFlag10;
	DWORD dwFlag11;

	virtual int CallAsProc(int iMethNum,CValue **ppValue);
};
#endif