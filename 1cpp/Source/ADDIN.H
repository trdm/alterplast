// AddIn.h: Definition of the CAddIn class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_)
#define AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef __REG__
#include "resource_reg.h"       // main symbols
#else
#include "resource.h"       // main symbols
#endif
#include "AddInTlb.h"
#include "atlctl.h"
#include "SetOfHierarchy.h"
#include "dml.h"
#include "DynaCrypt.h"
#include "OXY_Value.h"

#include "mycontextbase.h"
#include "stdafx.h"

#include "MethodsWrapper.h" // artbear

extern CBkEndUI * pBkEndUI;

/*
  These are error codes that should be used in AddError
*/
enum AddInErrors {
                  ADDIN_E_NONE = 1000,
                  ADDIN_E_ORDINARY = 1001,
                  ADDIN_E_ATTENTION = 1002,
                  ADDIN_E_IMPORTANT = 1003,
                  ADDIN_E_VERY_IMPORTANT = 1004,
                  ADDIN_E_INFO = 1005,
                  ADDIN_E_FAIL = 1006,
                  ADDIN_E_MSGBOX_ATTENTION = 1007,
                  ADDIN_E_MSGBOX_INFO = 1008,
                  ADDIN_E_MSGBOX_FAIL = 1009
                 };

class CAddIn;
extern CAddIn* pMainAddIn;

// kms++: вспомогательный класс для парсинга строк в CBrowseParser
class CBrowseParser
{
public:
	CBrowseParser();
	~CBrowseParser();

	Parse(LPCSTR lpsz);

	enum 
	{
		e_FONT,
		e_BRUSH,
		e_FONT_S,
		e_BRUSH_S,
		e_COUNT,
		e_INDENT,
		e_FONTSET,
		e_FONTSET_S,
		e_LAST__
	};

	struct S_par
	{
		// ключ найден / не найден
		// если bFound == FALSE, остальные значения структуры не определены
		BOOL bFound;

		// параметр найден, явно указано "NONE"
		BOOL bNone;

		// транслированное значение параметра
		long num;

		// указатель на начало параметра в оригинальной строке
		// позволяет выполнять произвольный парсинг параметра вне класса
		LPCSTR lpsz;

		// длина параметра в оригинальной строке
		int len;
	};

	S_par m_par[e_LAST__];

private:
	CIStringMap<S_par*, S_par*> * m_map;
};


class CBrowseWrapper
{
public:
	static void SetWrap(void);
	static void OffWrap(void);
	void  OnPaintWrap(void);
	int  WrapDrawItem(class CDC & DC,class CRect & Rect,class CString & st,int flag);
	CWnd*  WrapEdit(class CRect & a_Rect,class CBrowse * a_pBrowse);

private:
	static CBrowseParser m_parser;
};

class CGetDlgWrapped
{
public:
	static void  SetWrap(void);
	LRESULT myWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

////////////////////////////////////////////////////////////////////////////
//  class CAddInPropertyCollection

//	массив свойств-настроек плагина, к которым обращаются по имени
// класс по работе с настройками ВК, 
// упрощается работа со множеством настроек + единая схема обращения, загрузки и сохранения.

typedef void (*CVoidFuncPtr)(void);

class CAddIn;
class CAddInPropertyCollection
{
private:
	struct CPropertyFields
	{
		BOOL defaultValue;
		BOOL trueValue;
		BOOL falseValue;
		BOOL* pRealVariable;
	};

	struct CPropertiesFunc
	{
		CVoidFuncPtr pInit;
		CVoidFuncPtr pDestroy;
	};

	CIStringMap<BOOL, BOOL> m_PropertyMap;
	CIStringMap<CPropertyFields*, CPropertyFields*> m_PropertyFieldsMap;

	CMap<int, int, CString, CString> m_PropertyKeyMapOnDialogID;

	// карта наименований (русских и английских) для удобного использования
	CMapStringToString m_PropertyNameMap;
	
	// карта функций, которые вызываются при установке значения
	CIStringMap<CPropertiesFunc*, CPropertiesFunc*> m_PropertyFuncMap;
	bool m_bFlagEnableFunction; // true - разрешено запускать функции

	IPropertyProfile *m_iProfile;
	
	bool LoadOneProperty(CString key, BOOL defaultValue, BOOL trueValue = 1);
	bool SaveOneProperty(CString key, BOOL trueValue = 1, BOOL falseValue = 0);

	void CallInitFunction(CPropertiesFunc* ptrFunc);
public: 
	DECLARE_PROPERTY_ARRAY(Item, BOOL, CString)
	
	//DECLARE_PROPERTY_ARRAY(ItemByDialogID, CString, int)

	//BOOL& operator[](CString key);

	CAddInPropertyCollection():m_bFlagEnableFunction(false), m_iProfile(NULL)
	{ };

	~CAddInPropertyCollection();

	void Init(IPropertyProfile *iiProfile)
	{
		m_iProfile = iiProfile;
	}

	void Add(CString key, int DialogID, BOOL defaultValue, BOOL trueValue = 1, BOOL falseValue = 0, BOOL* pRealVariable = NULL);
	void AddName(CString sRealName, CString sAddName);

	void Load();
	void Save();

	CString GetKeyByDialogID(int dialogID);

	void AddFunction(CString sName, CVoidFuncPtr pInit1, CVoidFuncPtr pDestroy1);
	void SetupPropertyFunction(void); // запустить ассоциированные функции
};

typedef CBLContext* (*PF_CreateInstance)(char const *);

/////////////////////////////////////////////////////////////////////////////
// CCreateInstanceBoost
//

// класс-одиночка с функционалом - 
// перехват создания объекта - т.е. когда пользователь пишет СоздатьОбъект(ТипОбъекта)
// здесь самое начало ООП - возможность создавать собственные типы !
//	также выполнено ускорение создания любых объектов в 1С
//
class CCreateInstanceBoost
{
private:
	// структура для хранения данных о создаваемых типах
	class CInfo : public CObject
	{
	private:
		CInfo(): pRuntimeClass(NULL), IsBLContext(false) 
		{
			szFastCreationString = "";
		};
	public:
		CRuntimeClass* pRuntimeClass; // для ускорения создания контекстов
		bool IsBLContext;
		// TODO для произвольных строк нужно использовать CString !
		// для фиксированных строк (например, статичных в модуле) можно использовать LPCSTR для ускорения
		//CString szFastCreationString; // если LPCSTR - тогда ссылки не сохраняются, и 1С падает
		LPCSTR szFastCreationString; // если LPCSTR - тогда ссылки не сохраняются, и 1С падает

		CInfo(CRuntimeClass* pRuntimeClassA): pRuntimeClass(pRuntimeClassA), IsBLContext(true)
		{
			szFastCreationString = "";
		};
		
		CInfo(LPCSTR szFastCreationStringA): pRuntimeClass(NULL), IsBLContext(false) 
		{
			szFastCreationString = szFastCreationStringA;
		};
		~CInfo()
		{
		}
	}; // end of CInfo

	typedef CInfo* CInfoPtr;
	typedef CIStringMap< CInfoPtr, CInfoPtr > CIStringMapToInfo;
	//typedef CIStringMap<CInfo*, CInfo*> CIStringMapToInfo;

	CDllMethodWrapper<PF_CreateInstance> m_wrapperCreateInstance;

	sh_ptr<CIStringMapToInfo> m_pClassNameHash; //CIStringMapToInfo* m_pClassNameHash;
	CValue m_ValueForFastCreation; // временный объект для быстрого создания объектов

	CCreateInstanceBoost(){};
	CCreateInstanceBoost(CCreateInstanceBoost&){};

	// метод с ускорением
	static CBLContext* CreateInstanceBoosted(char const * szClassName);

public:	
	//метод без ускорения
	static CBLContext* CCreateInstanceBoost::CreateInstanceWithoutBoosted(char const * szClassName);

	~CCreateInstanceBoost(){};

	static CCreateInstanceBoost& TheObject();

	void Init(void);
	void Destroy(void);

	void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration);

	// быстрое создание объектов
	void AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString);
};

/////////////////////////////////////////////////////////////////////////////
// CAddIn

class CAddIn : 
    public IInitDone,
    public ILanguageExtender,
    public IPropertyLink,
    public CComObjectRoot,
    public CComCoClass<CAddIn, &CLSID_ExtraC>,
    public IPropertyPageImpl<CAddIn>,
    public CDialogImpl<CAddIn>
{
    enum 
    {
        LastProp      // Always last
    };

    enum 
    {
        LastMethod      // Always last
    };

public:
	CAddIn() : m_EnableOptimizate(TRUE), m_EnabledCheckType(FALSE), m_EnableDebugging(FALSE), m_AccessCheckType(TRUE), 
        m_AccessOptimizate(TRUE), m_AccessEnableDebugging(TRUE)
  {     
    //AFX_MANAGE_STATE(AfxGetStaticModuleState())  
    m_dwTitleID = IDS_PROPPAGE_CAPTION; 
    m_iErrorLog = NULL;
    m_iProfile = NULL;
    m_iStatusLine = NULL;
    m_iAsyncEvent = NULL;
	  m_iExtWndsSupport = NULL;
  }

  ~CAddIn() 
  { 

  }

// Attributes
    IErrorLog *m_iErrorLog;
    IPropertyProfile *m_iProfile;
    IStatusLine *m_iStatusLine;
    IAsyncEvent *m_iAsyncEvent;
	  IExtWndsSupport *m_iExtWndsSupport;


    BOOL m_EnabledCheckType;        // This property enables or disables component
    CButton m_boxEnabledCheckType;  // Corresponded check box
    BOOL m_AccessCheckType;

    BOOL m_EnableOptimizate;            
    CButton m_boxOptimizate;  
    BOOL m_AccessOptimizate;            

    BOOL m_EnableDebugging;        
    CButton m_boxEnableDebugging;     
    BOOL m_AccessEnableDebugging;            

    CButton m_boxEnabledColor;
	  CButton m_boxLocalColor;
	  CButton m_boxShowKL;

//	массив свойств-настроек плагина, к которым обращаются по имени
private:
	//typedef CIStringMap<int, int> CIStringMapToInt;
	//CIStringMap<BOOL, BOOL> m_PropertyMap;
	CAddInPropertyCollection m_PropertyCollection;

public: 
	/*BOOL get_Property(CString name);
	void put_Property(CString name, BOOL newValue);
	__declspec(property(get=get_Property, put=put_Property)) BOOL Property[];*/
	DECLARE_PROPERTY_ARRAY(Property, BOOL, CString)
	void InitProperties(); // только настраивает свойства, из реестра ничего не читается !!

    // Methods

    BOOL LoadProperties();
    void SaveProperties();
      // This method is very useful in ILanguageExtender implementation
    CString TermString(UINT uiResID,long nAlias);
      // These two methods is convenient way to access function 
      // parameters from SAFEARRAY vector of variants
    VARIANT GetNParam(SAFEARRAY *pArray,long lIndex);
    void PutNParam(SAFEARRAY *pArray,long lIndex,VARIANT vt);

private:
	// счетчик числа инициализаций компоненты
	static int m_nInitDoneCount;

// Interfaces
BEGIN_COM_MAP(CAddIn)
    COM_INTERFACE_ENTRY(IInitDone)
    COM_INTERFACE_ENTRY(ILanguageExtender)
    COM_INTERFACE_ENTRY(IPropertyLink)
    COM_INTERFACE_ENTRY_IMPL(IPropertyPage)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CAddIn) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

enum {IDD = IDD_ADDIN_PROP_PAGE};

LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
LRESULT OnCommand(WORD wNotify, WORD wID, HWND hWnd, BOOL& bHandled);

BEGIN_MSG_MAP(CAddIn)
    MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
      // Sample WM_COMMAND handler
    COMMAND_CODE_HANDLER(BN_CLICKED,OnCommand)
    COMMAND_CODE_HANDLER(EN_CHANGE,OnCommand)
    CHAIN_MSG_MAP(IPropertyPageImpl<CAddIn>)
    
END_MSG_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_AddIn)

// IInitDone
public:
    STDMETHOD(Init)(IDispatch *pConnection);
    STDMETHOD(Done)();
    STDMETHOD(GetInfo)(SAFEARRAY **pInfo);

// ILanguageExtender
public:

    STDMETHOD(RegisterExtensionAs)(BSTR *bstrExtensionName);

	STDMETHOD(GetNProps)(long *plProps);
	STDMETHOD(FindProp)(BSTR bstrPropName,long *plPropNum);
	STDMETHOD(GetPropName)(long lPropNum,long lPropAlias,BSTR *pbstrPropName);
	STDMETHOD(GetPropVal)(long lPropNum,VARIANT *pvarPropVal);
	STDMETHOD(SetPropVal)(long lPropNum,VARIANT *pvarPropVal);
	STDMETHOD(IsPropReadable)(long lPropNum,BOOL *pboolPropRead);
	STDMETHOD(IsPropWritable)(long lPropNum,BOOL *pboolPropWrite);

	STDMETHOD(GetNMethods)(long *plMethods);
	STDMETHOD(FindMethod)(BSTR bstrMethodName,long *plMethodNum);
	STDMETHOD(GetMethodName)(long lMethodNum,long lMethodAlias,BSTR *pbstrMethodName);
	STDMETHOD(GetNParams)(long lMethodNum,long *plParams);
	STDMETHOD(GetParamDefValue)(long lMethodNum,long lParamNum,VARIANT *pvarParamDefValue);
	STDMETHOD(HasRetVal)(long lMethodNum,BOOL *pboolRetValue);
	STDMETHOD(CallAsProc)(long lMethodNum,SAFEARRAY **paParams);
	STDMETHOD(CallAsFunc)(long lMethodNum,VARIANT *pvarRetValue,SAFEARRAY **paParams);

/* This interface implements connection between property page and object itself.
   Remember: object and its property page are implemented in ONE class but
   they work in different instances. So we should specify additional link between them
   and override SetObjects.
*/
// IPropertyLink
public:
    STDMETHOD(get_Enabled)(unsigned char *pData, int Id);
    STDMETHOD(put_Enabled)(unsigned char *pData, int Id);
    
// IPropertyPage
public: 
      /* Unnecessary functions are simply commented instead of calling 
         parent class implementation 
      */
    //STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);
    //STDMETHOD(Activate)(HWND hWndParent,LPCRECT prc,BOOL bModal);
    STDMETHOD(Deactivate)();
    //STDMETHOD(GetPageInfo)(PROPPAGEINFO *pPageInfo);
    //STDMETHOD(SetObjects)(ULONG cObjects,IUnknown **ppUnk);
    //STDMETHOD(Show)(UINT nCmdShow);
    //STDMETHOD(Move)(LPCRECT prc);
    //STDMETHOD(IsPageDirty)();
    STDMETHOD(Apply)();
    //STDMETHOD(Help)(LPCOLESTR pszHelpDir);
    //STDMETHOD(TranslateAccelerator)(LPMSG pMsg);
private:
  HMODULE hLibBasic;
};

//DMS:BEGIN
extern char buf[];//4096 bytes
extern int IsDMSContext(const char *);
extern char **pst;
//DMS:END

#endif // !defined(AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_)
