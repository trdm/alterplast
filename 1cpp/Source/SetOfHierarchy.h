// SetOfHierarchy.h: interface for the CSetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ComponentClass.h"
#include "Preprocessor.h"
#include "OXY_Value.h"
#include <algorithm>
#include "MetaDataOfClasses.h"

/***********************************************************
* Последовательность инициализации
* CSetOfHierarchy *pHierarchy = CSetOfHierarchy::GetHierarchy(); // Получаешь указаетль на созданный экз
* pHierarchy->Init("Путь к каталогу текущей ИБ","Путь к каталогу Bin 1С"); // Инициализируешь пути
* pHierarchy->ProcessDefines(); // Пошел процесс чтения файла определений
************************************************************/


extern CBkEndUI * pBkEndUI;

#define DLLEXPORT __declspec( dllexport )

// Данный класс отвечает за считывание файла настройки классов для 1С.
// Управляет созданием и инициализацией экземпляров классов компонент, для их дальнейшего использования в программе.
// Экземпляр данного класса присутсвует в системе в единичном виде в глобальной памяти.

class CSetOfHierarchy  
{
public:
  
	// Возвращает строку с путем и именем файла реализации класса по переданному названию
	CString GetNameOfFile(const CString& strNameOfMod);
  
  // Контсруктор
	CSetOfHierarchy();
  // Деструктор
	~CSetOfHierarchy();

	// artbear для проверки рекурсии
	// 1 - рекурсия, 0 - нет
	bool VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy);

	// artbear - флаг открытия формы класса через OpenForm/ОткрытьФорму для отладки
  bool bIsOpenCompForm;
  inline void SetIsOpenCompForm(bool _bIsOpenCompForm) { bIsOpenCompForm = _bIsOpenCompForm; };
  inline bool IsOpenCompForm() { return bIsOpenCompForm; };

  // Выполняет регистрацию имен компонентов 1С полученных из файла объявлений
	void RegistersContextClasses();
  
  // Проверяет типы параметров метода и его возвращаемого значения
  bool CheckTypeOfParam(CComponentClass* pComponentClass,
					   int nNumMeth,
					   CValue** ppParamOfMeth, 
             CBLProcInfo* pProcInfo,
					   CValue* pRetValue = NULL);

  // Устанавливает для параметров значения по умолчанию перед вызовом метода
  enum CParamDefValueEnum{
		pdve_DefValueNotExists = 0,
		pdve_DefValueIsExists = 1,
		pdve_ParamDefineWithoutDefValue = 2,
  };
  
  CParamDefValueEnum GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, 
	  const CComponentClass* pComponentClass);

  // Получить единственный экземпляр класса CSetOfHierarchy
  static CSetOfHierarchy* GetHierarchy();
  
  // Удалить единственный экземпляр класса CSetOfHierarchy
  static void DeInitHierarchy();
  
  // Путь к текущей базе данных 1С
  CString pathBase1C;
  
  // Путь к каталогу в котором храняться установленые файлы 1С
  CString pathBin1C;

  //Символы для препроцессора
  CMapStringToPtr m_SymbolOfPreprocessor;
  
  static void RemoveNeedlessBlank(CString &str);
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;  
  bool IsObject(const CString& sClassName, const CString& sTryName) const;  

private:
	static void PrintError(const char* cstrError, const char* cstrFileName, int nNumLine);
  // Процедура удаляет лишние пробелы из строки	
  
  CMetaDataOfClasses m_Meta;
  	
  // Массив с параметрами переданными по значению в метод для последующего восстановления
  //CArray<COXY_Value*, COXY_Value*> m_ParamValOfMeth;
  
  // Единый экземпляр класса CSetOfHierarchy
  static CSetOfHierarchy* pGlobalHierarchy;

  // Содержит имена загруженных файлов директивой include в качестве ключей
  CMapStringToString m_LoadedIncludeFiles;
  
  // Данный вектор может содержать номера параметров, для которых необходимо пропускать проверку типов.
  // Такие параметры могут появлятся в случае установки их значений по умолчанию равное = ""
  vector<int> m_VecNumOfParForPassCheckType;

public:  		
	void ReadDefinesAndCompile(CString &str, const CString &strNameFile);
	
  void Init(const char* IBDir = NULL, const char* BinDir = NULL);

	static void MakeFullFileName(CString* name, LPCSTR sSecondPath = NULL);
	bool IsMethExist(const char* pNameClass, const char* pNameMeth) const;
	CParamDefs::CArrayOfMethodsParamData_PTR GetInfoParamMeth(const char *pstrNameClass, const char *pstrNameMeth) const;
	CString GetNamesMethOfClass(const char* pNameClass, const char* sep = ",") const;
  // метод возвращает строку с названиями базовых классов для strNameCl, разделенными sep
	CString GetBaseClasses(const char* strNameCl, const char* sep = ",") const;
	void LoadListNameOfIncludeFiles(CStringArray& arr);
	CString GetAllAliasAndPath(const char* sep = "\r\n") const;
	CString GetAllClassesNames(const char* sep = "\r\n") const;
	CString GetDefSymbolOfPreproc(const char* sep = "\r\n") const;
	CString GetLoadIncludeFiles(const char* sep = "\r\n") const;
	int GetCountMethods() const;
	int GetCountClasses() const;
	bool IsClassExist(const char*);
	void InitParamDefsClass(CParamDefs * par, const CString& strClass);
	bool isMethHaveParVal(const CString& strName);
	bool ThisIsUnLimMethod(const CString& strName);
  
  // определеяет все внутренние структуры
  void ProcessDefines();

	CString GetPathFromAlias(const CString& strAls);
  // Текущее имя класса и метода разделенные знаком :

  CString GetClassNameFromDefinition(const CString& ClassName) const;

  CMetaDataOfClasses& GetMetaDataOfClasses(void)
  { return m_Meta; }

  void LoadNewClasssesDefine(CString &str, const CString &strNameFile);

private:
	CParamDefs::CArrayOfMethodsParamData_PTR GetMethodParamData(const CComponentClass* pComponentClass, int iMethodNum) const;
};

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
