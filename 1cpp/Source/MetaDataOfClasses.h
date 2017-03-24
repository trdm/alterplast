// MetaDataOfClasses.h: interface for the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
#define AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SetOfHierarchy.h"
//#include "Preprocessor.h"
#include "istrmap.h"

//#include "system\\GlobalMethods.h"
void RuntimeError(const char *msg, ...);

class CRangesOfDescr;

struct CMetaDataOfClasses  
{

	CMetaDataOfClasses();
	~CMetaDataOfClasses();
  static void RemoveNeedlessBlank(CString &str);

  // Метод разбирает строку текста и заполняет внутренние структуры класса. 
  // Строка должна состоять из объявлений классов с привязкой к именам файлов 
  // хранящих реализацию и иерархической структуры этих классов	
  void ParsingString(CString& StringForParse, CRangesOfDescr& RangesDescr);

  // Хранит связи названий классов 1С (для возможности использовать 
  // СоздатьОбъект(<ИмяКласса>) и файлов в которых находиться реализация данных классов
	CIStringMapToString m_ClassBindFile;

  // тоже самое для динамически добавляемых классов
	CIStringMapToString m_DynamicAddedClassBindFile;
	bool m_bInsideDynamicAdd;
	
  // Хранит строки названий классов и связанный с ним список названий базовых классов
	// TODO проще всего переделать на vector или list
    typedef boost::shared_ptr<CStringList> CStringList_SharedPtr;
	CIStringMap<CStringList_SharedPtr, CStringList_SharedPtr&> m_ClassesHierarchyMap; //CIStringMap<CStringList*, CStringList*> m_ClassesHierarchyMap;
  
  // Содержит синонимы каталогов из файла ini компоненты
  CIStringMapToString m_AliasOfPaths;

  // Содержит весь набор данных о методах классов и их параметрах
  typedef CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData CPtrToMap;
  CIStringMap<CPtrToMap, CPtrToMap&> m_MapClassNamesToMethodsMap;

  bool ThisIsUnLimMethod(const CString& strName) const;

  void AddMethHaveParVal(const CString& strNameOfClassAndMeth, CParamDefs::CArrayOfMethodsParamData_PTR param);
  bool LookupMethHaveParVal(const CString& strName, CParamDefs::CArrayOfMethodsParamData_PTR& param) const;
  bool isMethHaveParVal(const CString& strName) const 
	{ CParamDefs::CArrayOfMethodsParamData_PTR param; return LookupMethHaveParVal(strName, param); }

  bool IsClassExist(const char *pcstrName) const;
  int GetCountClasses() const;
  int GetCountMethods() const;

  CString GetAllClassesNames(const char* sep) const;
  CString GetAllAliasAndPath(const char* sep) const;
  CString GetBaseClasses(const char* strNameCl, const char* sep) const;
  CString GetNamesMethOfClass(const char *pNameClass, const char *sep) const;
  CParamDefs::CArrayOfMethodsParamData_PTR GetInfoParamMeth(const char *pNameClass, const char *pNameMeth) const;
  bool IsMethExist(const char *pNameClass, const char *pNameMeth) const;

  CString GetClassNameFromDefinition(const CString& ClassName) const;
  CString GetPathFromAlias(const CString& strAls) const;

  // artbear для проверки рекурсии // 1 - рекурсия, 0 - нет
  bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy) const;

  CString GetNameOfFile(const CString &strNameOfMod) const;
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;
  bool IsObject(const CString& sClassName, const CString& sTryName) const;

private:

  // Ключ - имя класса плюс имя метода в котором присутствует параметр передаваемый по значению
	CParamDefs::CMapMethodsNameToArrayOfMethodsParamData m_ValParamInMeth; //CIStringMapToOb m_ValParamInMeth;

  // Ключ - имя класса плюс имя метода в котором присутствует неограниченное количество параметров (...)
  CParamDefs::CMapMethodsNameToArrayOfMethodsParamData m_UnlimitsParams;//CIStringMapToOb m_UnlimitsParams;
  
  // Содержит полный путь к файлу описания для класса
  CIStringMapToString m_FullFileNameOfDescrFile;

	void MessageBox(
		HWND hWnd ,
		const CString& lpText,
		const CString& lpCaption,
		UINT uType)
	{
		if(!m_bInsideDynamicAdd)
			::MessageBox(hWnd, lpText, lpCaption, uType);
		else
			RuntimeError("%s -- %s", lpCaption, lpText);
	}
};

#endif // !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
