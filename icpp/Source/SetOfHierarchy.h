// SetOfHierarchy.h: interface for the ISetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComponentClass.h"

// TODO #include "Preprocessor.h" - рекурси€ !!
#include "Preprocessor.h"

#include <algorithm>
//#include "MetaDataOfClasses.h"
//#include "paramdefs.h"

// TODO это важное объ€вление лучше расположить в другом месте !!
#define DLLEXPORT __declspec( dllexport )

//class CComponentClass;
class CMetaDataOfClasses;

// ƒанный класс отвечает за считывание файла настройки классов дл€ 1—.
// ”правл€ет созданием и инициализацией экземпл€ров пользовательских классов, дл€ их дальнейшего использовани€ в программе.
// Ёкземпл€р данного класса присутствует в системе в единичном виде в глобальной пам€ти.

class ISetOfHierarchy  
{
public:
	// ѕолучить единственный экземпл€р класса ISetOfHierarchy
	static ISetOfHierarchy* GetHierarchy();

	// ”далить единственный экземпл€р класса ISetOfHierarchy
	static void DeInitHierarchy();

	static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);
	static const CMetaDataOfClasses& GetMetaDataOfClasses(void);

	virtual ~ISetOfHierarchy() {};

	virtual void Init() = 0;

	// artbear - флаг открыти€ формы класса через OpenForm/ќткрыть‘орму дл€ отладки
	virtual inline void SetIsOpenCompForm(bool _bIsOpenCompForm) = 0;
	virtual inline bool IsOpenCompForm() const = 0;

	// ѕровер€ет типы параметров метода и его возвращаемого значени€
	virtual bool CheckTypeOfParam(CComponentClass* pComponentClass,
		int nNumMeth,
		CValue** ppParamOfMeth, 
		CBLProcInfo* pProcInfo,
		CValue* pRetValue = NULL) = 0;

	// ”станавливает дл€ параметров значени€ по умолчанию перед вызовом метода
	enum CParamDefValueEnum{
		pdve_DefValueNotExists = 0,
		pdve_DefValueIsExists = 1,
		pdve_ParamDefineWithoutDefValue = 2,
	};

	virtual CParamDefValueEnum GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, 
		const CComponentClass* pComponentClass) = 0;

	virtual const CString& GetPathBase1C() const = 0;
	virtual const CString& GetPathBin1C() const = 0;

	//—имволы дл€ препроцессора
	virtual CMapStringToPtr& GetMapOfSymbolOfPreprocessor() = 0;

	virtual bool TheClassIsDerive(const CString& derive, const CString& base) const = 0;

	virtual void ReadDefinesAndCompile(CString &str, const CString &strNameFile) = 0;

	virtual void LoadListNameOfIncludeFiles(CStringArray& arr) = 0;

	virtual CString GetDefSymbolOfPreproc(const char* sep = "\r\n") const = 0;
	virtual CString GetLoadIncludeFiles(const char* sep = "\r\n") const = 0;

	virtual void LoadNewClasssesDefine(CString &str, const CString &strNameFile) = 0;

	//virtual bool VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strKeyOfUserClassMethod, const CBLProcInfo &procInfo) = 0;

	static CString GetKeyOfUserClassMethod(const CString& strClassName, LPCSTR strMethodName)
	{
		CString strKey = strClassName;
		strKey += ':';
		strKey += strMethodName;

		return strKey;
	};
};

//typedef ISetOfHierarchy CSetOfHierarchy; // дл€ совместимости со старым клиентским кодом

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
