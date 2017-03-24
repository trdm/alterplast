// SetOfHierarchy.h: interface for the CSetOfHierarchy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
#define AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/***********************************************************
* Последовательность инициализации
* CSetOfHierarchy *pHierarchy = CSetOfHierarchy::GetHierarchy(); // Получаешь указаетль на созданный экз
* pHierarchy->Init("Путь к каталогу текущей ИБ","Путь к каталогу Bin 1С"); // Инициализируешь пути
* pHierarchy->ProcessDefines(); // Пошел процесс чтения файла определений
************************************************************/


#define DLLEXPORT __declspec( dllexport )



// Данный класс отвечает за считывание файла настройки классов для 1С.
// Управляет созданием и инициализацией экземпляров классов компонент, для их дальнейшего использования в программе.
// Экземпляр данного класса присутсвует в системе в единичном виде в глобальной памяти.

class CSetOfHierarchy  
{
public:
///	typedef CArray<COXY_Value*, COXY_Value*> ARRAY_OXY_VAL;

	
  CComponentClass* GetComponentClassCurExModule(CBLModule* pModWrapper = NULL);

  void ClearImplicitParams(CComponentClass *pComp);

  CBLContext* GetImplicitParamsList(CComponentClass *pComp, const char* lpcsNameMeth);

  bool GetImplicitParams(CComponentClass *pComp, CValue **ppPar, int nNumMeth);
  int SetImplicitParamOfInd(CComponentClass *pComp, const char* lpcsNameMeth, int nIndex, CValue* pVal);

  int GetNParams(const CComponentClass *pComp, int nNumMeth, int nNParams);

  // Восстанавливает параметры которые переданны в метод по значению после вызова
	void RestoreValParam(CComponentClass *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_OXY_VAL *arrValOfMeth);
  // Сохраняет параметры, которые переданны в метод по значению перед вызовом
	bool SaveValParam(CComponentClass* pComponentClass, int nNumMeth, CValue** ppParam, ARRAY_OXY_VAL *arrValOfMeth);
  // Возвращает строку с путем и именем файла реализации класса по переданному названию
	CString& GetNameOfFile(const CString& strNameOfMod);
  
  // Контсруктор
	CSetOfHierarchy();
  // Деструктор
	~CSetOfHierarchy();

  // Выполняет инициализацию экземпляра компоненты на основе информации полученного из файла составленного пользователем
	bool InitInstanceComp(CComponentClass* pComponent);
  void DeInitInstanceComp(CBLModule *pModWrapper);

	// Выполняет регистрацию имен компонентов 1С полученных из файла объявлений
	void RegistersContextClasses();
  
  // Проверяет типы параметров метода и его возвращаемого значения
  bool CheckTypeOfParam(CComponentClass* pComponentClass,
					   int nNumMeth,
					   CValue** ppParamOfMeth, 
             CBLProcInfo* pProcInfo,
					   CValue* pRetValue = NULL);
  // Устанавливает для параметров значения по умолчанию перед вызовом метода
  int GetParamDefValue(int iMethodNum, int iParamNum, CValue* pDefValue, const CComponentClass* pComponentClass);

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

  // Увязывает указатели на экземпляры классов с указателями на список значений парметров 
  // полученных вместо точек в методе (...)
  struct InfoUnlimitsParams
  {
    CBLContext* cont_ValueList;
    vector<CValue*> vecPointOfPars;
    int         nNumAllParams;
    int         nNumExplicitParams;
  };
  
  static void RemoveNeedlessBlank(CString &str);
  
  typedef CArray<NameBindOfType*, NameBindOfType*> ARR_TYPE_NAME;
  
  bool TheClassIsDerive(const CString& derive, const CString& base) const;  

private:
	static void PrintError(const char* cstrError, const char* cstrFileName, int nNumLine);
  // Процедура удаляет лишние пробелы из строки	
  
  CMetaDataOfClasses m_Meta;
  	
  //CMapPtrToPtr   m_BindInstaceValueList;  
  // Массив с параметрами переданными по значению в метод для последующего восстановления
  //CArray<COXY_Value*, COXY_Value*> m_ParamValOfMeth;
  
  // Единый экземпляр класса CSetOfHierarchy
  static CSetOfHierarchy* pGlobalHierarchy;

  // Указатель на текущий создаваемый объекта КОП
  // Необходим для установки его как контекста во всех модулях иерархии
  CComponentClass* pCreatedCompModule;

  // Карта с указателями на модули с их реальными контекстами
  // Ключ - указатель на модуль, Занчение - указателя на контекст CComponentClass
  CMapPtrToPtr m_MapOfModulesAndComp;
   
  // Содержит имена загруженных файлов директивой include в качестве ключей
  CMapStringToString m_LoadedIncludeFiles;
  
  // Данный вектор может содержать номера параметров, для которых необходимо пропускать проверку типов.
  // Такие параметры могут появлятся в случае установки их значений по умолчанию равное = ""
  vector<int> m_VecNumOfParForPassCheckType;

public:  		
	
  void Init(const char* IBDir = NULL, const char* BinDir = NULL);

	static void MakeFullFileName(CString* name);
	bool IsMethExist(const char* pNameClass, const char* pNameMeth) const;
	ARR_TYPE_NAME* GetInfoParamMeth(const char *pstrNameClass, const char *pstrNameMeth) const;
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
	void InitParamDefsClass(CParamDefs * par, CString strClass);
	bool isMethHaveParVal(CString strName);
	bool ThisIsUnLimMethod(CString strName);
	int m_CountParametersInCallMethod;
  
  // определеяет все внутренние структуры
  void ProcessDefines();

	CString GetPathFromAlias(CString strAls);
  // Текущее имя класса и метода разделенные знаком :
  CString m_strClassAndMethImplicit;
  CString m_strClassAndMethCurrent;  
};

#endif // !defined(AFX_SETOFHIERARCHY_H__B6166793_8666_47BA_8450_BAF6FC6D5FEA__INCLUDED_)
