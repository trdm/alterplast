// MetaDataOfClasses.h: interface for the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
#define AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SetOfHierarchy.h"
#include "Preprocessor.h"

#include <boost\shared_ptr.hpp>

class CRangesOfDescr;

struct NameBindOfType {
    CString name;
    CString type;
    CString value;
    bool bIsVal;
    NameBindOfType (const CString& n, const CString& t, const CString& v = CString(""), bool bIs = false) : name(n), type(t), value(v), bIsVal(bIs) {}
};

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
	CMapStringToString m_ClassBindFile;
	
  // Хранит строки названий классов и связанный с ним список названий базовых классов
	typedef boost::shared_ptr<CStringList> CBaseClassesListPtr;
	typedef CIStringMap<CBaseClassesListPtr, CBaseClassesListPtr> CBaseClassesMap;
	CBaseClassesMap m_ClassesHierarchyMap;
  
  // Содержит синонимы каталогов из файла ini компоненты
  CMapStringToString m_AliasOfPaths;

  // Содержит типы методов классов для строгой проверки
  CMapStringToOb m_TypeOfClassMeth;  

  typedef CArray<NameBindOfType*, NameBindOfType*> ARR_TYPE_NAME;

  // Ключ - имя класса плюс имя метода в котором присутствует параметр передаваемый по значению
  CMapStringToOb m_ValParamInMeth;

  // Ключ - имя класса плюс имя метода в котором присутствует неограниченное количество параметров (...)
  CMapStringToOb m_UnlimitsParams;
  
  // Содержит полный путь к файлу описания для класса
  CMapStringToString m_FullFileNameOfDescrFile;
  
  static CMetaDataOfClasses* m_oneMetadata;
  
  CMapStringToPtr m_SymbolOfPreprocessor;
  CMapStringToString m_LoadedIncludeFiles;
  CString pathBase1C;

  static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);

	// artbear для проверки рекурсии
	// 1 - рекурсия, 0 - нет
	bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy);

};

#endif // !defined(AFX_METADATAOFCLASSES_H__F1BE676F_903A_4D46_99F1_123B72722A0A__INCLUDED_)
