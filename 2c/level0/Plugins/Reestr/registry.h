//////////////////////////////////////////////////////////////////////////
//Registry.h 
//Плагин 2C: Registry версия 16/01/2004 
//Разработчик AlexQC (alexqc@narod.ru) 
//////////////////////////////////////////////////////////////////////

#if !defined(__REGISTRY_H__)
#define __REGISTRY_H__

#include <atlbase.h>

 
class CValueRegistry : public CValue  
{

DECLARE_DYNCREATE(CValueRegistry );
public:
	CValueRegistry();
	CValueRegistry(HKEY,LPCTSTR);
	virtual ~CValueRegistry();

	CRegKey RKEY;

	inline LONG TryOpenRW_RO(HKEY,LPCTSTR);

	struct tagEnumKeys{
		int nCount,curIndex;
		DWORD MaxNameLenght;
		CString CurName;
		CValue CurKey;
		void Clear(){
			nCount=-1;
			curIndex=-1;
			MaxNameLenght=0;
			CurName="";
			CurKey=CValue ();
		};
	}EnumKeys;
	
	struct tagEnumVals{
		int nCount,curIndex;
		DWORD MaxNameLenght,MaxValLenght;
		CString CurName,CurVal;
		void Clear(){
				nCount=-1;
				curIndex=-1;
				MaxNameLenght=0;
				MaxValLenght=0;
				CurName="";
				CurVal="";
		};
	}EnumVals;
	
	

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода


	CString csObjectName;
	virtual CString GetString(void);
	virtual void Init(CString StrVid,CValue Param);
	virtual CString GetTypeString(void)const{return "Реестр";};
	virtual CValue GetAttribute(int iName);//значение атрибута

	typedef BOOL (CValueRegistry ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL CreateKey(CValue &rez,CValue**ppV);
	BOOL Open(CValue &rez,CValue**ppV);
	BOOL IsOpen(CValue &rez,CValue**ppV);
	BOOL Close(CValue &rez,CValue**ppV);
	BOOL QueryValue(CValue &rez,CValue**ppV);
	BOOL SetValue(CValue &rez,CValue**ppV);
	BOOL DeleteValue(CValue &rez,CValue**ppV);
	BOOL DeleteSubKey(CValue &rez,CValue**ppV);

	BOOL ExistSubKey(CValue &rez,CValue**ppV);
	BOOL ExistValue(CValue &rez,CValue**ppV);

	BOOL SelectValues(CValue &rez,CValue**ppV);
	BOOL GetValue(CValue &rez,CValue**ppV);

	BOOL SelectKeys(CValue &rez,CValue**ppV);
	BOOL GetKey(CValue &rez,CValue**ppV);

	void Clear();
	static HKEY RegKeyFromVal(CValue *v,HKEY defKey=NULL);
	static HKEY RegKeyFromString(CString s,HKEY defKey=NULL);

};

#endif // __REGISTRY_H__

