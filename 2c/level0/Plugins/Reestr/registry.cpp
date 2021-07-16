//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "registry.h"


CMethods CValueRegistry::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueRegistry,CValue);
CValueRegistry::CValueRegistry()
{
	nRef=0;
	nType=100;
	EnumKeys.Clear();
	EnumVals.Clear();

}

LONG CValueRegistry::TryOpenRW_RO(HKEY Parent,LPCTSTR SubKey){
	if(ERROR_SUCCESS != RKEY.Open(Parent,SubKey,KEY_READ|KEY_WRITE))
		return RKEY.Open(Parent,SubKey,KEY_READ);
	return ERROR_SUCCESS;

}

CValueRegistry::CValueRegistry(HKEY Parent,LPCTSTR SubKey)
{
	CValueRegistry();
	TryOpenRW_RO(Parent,SubKey);

}

CValueRegistry::~CValueRegistry()
{
	EnumKeys.Clear();
	EnumVals.Clear();
}

//******************************
//Работа как агрегатного объекта
//******************************

static struct {
	CValueRegistry::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]={
	{CValueRegistry::Open,"Open","Открыть"},
	{CValueRegistry::QueryValue,"Get","Получить"},
	{CValueRegistry::CreateKey,"CreateKey","СоздатьКлюч"},
	{CValueRegistry::IsOpen,"IsOpen","Открыт"},
	{CValueRegistry::Close, "Close", "Закрыть" },
	{CValueRegistry::SetValue, "Set", "Установить" },
	{CValueRegistry::DeleteValue, "DeleteValue", "УдалитьЗначение" },
	{CValueRegistry::DeleteSubKey, "DeleteSubKey", "УдалитьПодключ" },

	{CValueRegistry::ExistSubKey, "ExistSubKey", "ЕстьПодключ" },
	{CValueRegistry::ExistValue, "ExistValue", "ЕстьЗначение" },

	{CValueRegistry::SelectValues, "SelectValues", "ВыбратьЗначения" },
	{CValueRegistry::GetValue, "GetValue", "ПолучитьЗначение" },

	{CValueRegistry::SelectKeys, "SelectKeys", "ВыбратьКлючи" },
	{CValueRegistry::GetKey, "GetKey", "ПолучитьКлюч" },

};

SEngRus ArrayProp[]={
	{"Key","Ключ"},
	{"KeyName","ИмяКлюча"},
	{"Value","Значение"},
	{"ValueName","ИмяЗначения"},

};

enum {propKey=0,propKName,propVal,propVName,propLast};

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CValueRegistry::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,ArrayProp,propLast);
}

BOOL CValueRegistry::Open(CValue &rez,CValue **p){ //Родитель -- Имя ключа	=0 - неудача
	Clear();
	RKEY.Close();
	Init(p[0]->GetString(),*p[1]);
	return IsOpen(rez,p);
}

BOOL CValueRegistry::Close(CValue &rez,CValue **p){ 
	Clear();
	RKEY.Close();
	return TRUE;
}

BOOL CValueRegistry::IsOpen(CValue &rez,CValue **p){ //=0 - закрыто
	
	rez = ( NULL != (HKEY)RKEY);
	return TRUE;
}

HKEY CValueRegistry::RegKeyFromString(CString s,HKEY defKey){
	HKEY parKey=defKey;
	s.MakeUpper();
	if("HKCR"==s 
		|| "HKEY_CLASSES_ROOT"==s)parKey=HKEY_CLASSES_ROOT;

	else if("HKLM"==s
		|| "HKEY_LOCAL_MACHINE"==s)parKey=HKEY_LOCAL_MACHINE;

	else if("HKCU"==s
		|| "HKEY_CURRENT_USER"==s)parKey=HKEY_CURRENT_USER;

	else if("HKU"==s 
		|| "HKEY_USERS"==s )parKey=HKEY_USERS;
	return parKey;
}


HKEY CValueRegistry::RegKeyFromVal(CValue *v,HKEY defKey){
	if(!v)return NULL;
	if(TYPE_EMPTY == v->GetType())return defKey;
	if(TYPE_REFFER == v->GetType())
	{  CValue *vRef=v->pRef;
		if(!vRef || vRef==v)return defKey;
		return RegKeyFromVal(vRef,defKey);
	}

	if(v->IsKindOf(RUNTIME_CLASS(CValueRegistry) ) ){
		return ((CValueRegistry*)v)->RKEY;
	}
	return RegKeyFromString(v->GetString(),defKey);
}

BOOL CValueRegistry::QueryValue(CValue &rez,CValue **p){  //Имя
	DWORD cCount=255;
	CString bufString;
	CString sname=p[0]->GetString();
	LPCTSTR name= sname.IsEmpty()? NULL : (LPCTSTR)sname;
	LPTSTR buf=bufString.GetBuffer(cCount);
	BOOL bRez=( ERROR_SUCCESS==RKEY.QueryValue(buf,name,&cCount) );
	bufString.ReleaseBuffer();
	if(bRez){
		rez.SetString(bufString);
	}
	return bRez;
}

BOOL CValueRegistry::SetValue(CValue &rez,CValue **p){ //Имя -- Значение =0 - неудача
	CString sname=p[0]->GetString();
	LPCTSTR name= sname.IsEmpty()?NULL:(LPCTSTR)sname;
	int bRez;
	do{
		if(TYPE_NUMBER==p[1]->GetType()){
			double d=p[1]->GetNumber();
			if(d == (DWORD)d ){
				bRez=RKEY.SetValue( (DWORD)d, name);
				break;
			}
		}
		CString val=p[1]->GetString();
		bRez=RKEY.SetValue(LPCTSTR(val),name);
	}while(0);
	rez=(ERROR_SUCCESS==bRez);
	return TRUE;
}

BOOL CValueRegistry::DeleteValue(CValue &rez,CValue **p){ //Имя   =0 - неудача
	CString sname=p[0]->GetString();
	LPCTSTR name=sname.IsEmpty()?NULL:(LPCTSTR)sname;
	rez=(ERROR_SUCCESS==RKEY.DeleteValue(name));
	EnumVals.Clear();
	return TRUE;
}

BOOL CValueRegistry::DeleteSubKey(CValue &rez,CValue **p){ //Имя   =0 - неудача
	CString sname=p[0]->GetString();
	EnumKeys.Clear();
	rez=(ERROR_SUCCESS==RKEY.RecurseDeleteKey(sname));
	return TRUE;
}

BOOL CValueRegistry::CreateKey(CValue &rez,CValue **p){ //Родитель -- Имя подключа   =0 - неудача
	rez=0;
	Clear();
	HKEY parKey=RegKeyFromVal(p[0]);
	CString sname=p[1]->GetString();
	if(parKey)
		rez=(ERROR_SUCCESS==RKEY.Create(parKey,sname));
	else RKEY.Close();
	return TRUE;
}

BOOL CValueRegistry::ExistSubKey(CValue &rez,CValue **p){ // Имя подключа   =0 - нету
	rez=0;
	HKEY parKey=RKEY;
	CString sname=p[0]->GetString();
	if(parKey){
		CRegKey tkey;
		rez=(ERROR_SUCCESS==tkey.Open(parKey,sname,KEY_READ));
	}
	return TRUE;
}

BOOL CValueRegistry::ExistValue(CValue &rez,CValue **p){ //Имя значения   =0 - не существует
	rez=0;
	CString sname=p[0]->GetString();
	LPCTSTR name=sname.IsEmpty()?NULL:(LPCTSTR)sname;

	HKEY parKey=RKEY;
	if(parKey)
		rez=( ERROR_SUCCESS==::RegQueryValueEx(RKEY,name,0,NULL,NULL,NULL) );
	return TRUE;
}





CValue CValueRegistry::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CValueRegistry::GetString(){
	return "Реестр";
	//return "Registry";
}


void CValueRegistry::Init(CString StrVid,CValue Param){
	CString tStr=StrVid;
	int pos_dot=tStr.Find(_T('.'));
	if(0 <= pos_dot)tStr=tStr.Mid(pos_dot+1);
	HKEY parKey=RegKeyFromString(tStr,NULL);
	CString keyname=Param.GetString();
	if(parKey)
		TryOpenRW_RO(parKey,keyname);

}

void CValueRegistry::Clear(){
	EnumKeys.Clear();
	EnumVals.Clear();
}

BOOL CValueRegistry::SelectValues(CValue &rez,CValue**ppV){
	EnumVals.Clear();
	if(!HKEY(RKEY) ){
		rez=0;
		return FALSE;
	}

	int bRez=::RegQueryInfoKey(
		(HKEY) RKEY,                      // handle to key
		NULL,                 // class buffer
		NULL,               // size of class buffer
		0,             // reserved
		NULL,             // number of subkeys
		NULL,        // longest subkey name
		NULL,         // longest class string
		(DWORD*)&EnumVals.nCount,              // number of value entries
		&EnumVals.MaxNameLenght,     // longest value name
		&EnumVals.MaxValLenght,         // longest value data
		NULL, // descriptor length
		NULL // last write time
		);
	if(ERROR_SUCCESS==bRez){
		rez=EnumVals.nCount;
		EnumVals.curIndex=0;
		EnumVals.MaxNameLenght++;     // Учтем заключительный \0
		EnumVals.MaxValLenght++;
		return TRUE;
	}
	else {
		EnumVals.nCount=-1;
		EnumVals.MaxNameLenght=0;
		EnumVals.MaxValLenght=0;
		return FALSE;
	}

}

BOOL CValueRegistry::GetValue(CValue &rez,CValue**ppV){
	rez=0;
	if(!HKEY(RKEY)
		|| -1==EnumVals.nCount)return FALSE;
	
	if (EnumVals.curIndex<EnumVals.nCount) {
		
		LPTSTR bufName=EnumVals.CurName.GetBuffer(EnumVals.MaxNameLenght);
		LPTSTR bufVal=EnumVals.CurVal.GetBuffer(EnumVals.MaxValLenght);
		int nName=EnumVals.MaxNameLenght,nVal=EnumVals.MaxValLenght;
		DWORD tType;
		int bRez=::RegEnumValue(
			RKEY,             // handle to key to query
			EnumVals.curIndex,         // index of value to query
			bufName,    // value buffer
			(DWORD*)&nName,  // size of value buffer
			0,    // reserved
			&tType,        // type buffer
			(LPBYTE)bufVal,         // data buffer
			(DWORD*)&nVal// size of data buffer
			);
		EnumVals.CurName.ReleaseBuffer(nName);
		EnumVals.curIndex++;
		if(ERROR_SUCCESS == bRez){
			switch(tType) {
			case REG_DWORD:
				{
					DWORD dw=*(DWORD*)bufVal;
					*bufVal=0;
					EnumVals.CurVal.ReleaseBuffer();
					EnumVals.CurVal.Format("0x%08X",dw);
				}
				break;
			case REG_MULTI_SZ:
				{
					for(LPTSTR ss=bufVal
						;ss[0] || ss[1]
						;ss++)
						if(!*ss)*ss='\n'; // \0 заменяем на \n
						
						EnumVals.CurVal.ReleaseBuffer();
						
				}
				break;
			case REG_EXPAND_SZ:
			case REG_SZ:
			default:
				EnumVals.CurVal.ReleaseBuffer(nVal);
				break;
			}
			rez=1;
		}
	}
	return TRUE;
}


BOOL CValueRegistry::SelectKeys(CValue &rez,CValue**ppV){
	EnumKeys.Clear();
	if(!HKEY(RKEY) ){
		rez=0;
		return FALSE;
	}

	int bRez=::RegQueryInfoKey(
		(HKEY) RKEY,                      // handle to key
		NULL,                 // class buffer
		NULL,               // size of class buffer
		0,             // reserved
		(DWORD*)&EnumKeys.nCount,             // number of subkeys
		(DWORD*)&EnumKeys.MaxNameLenght ,        // longest subkey name
		NULL,         // longest class string
		NULL,              // number of value entries
		NULL,     // longest value name
		NULL,         // longest value data
		NULL, // descriptor length
		NULL // last write time
		);
	if(ERROR_SUCCESS==bRez){
		rez=EnumKeys.nCount;
		EnumKeys.MaxNameLenght++;     // Учтем заключительный \0
		EnumKeys.curIndex=-1;
		return TRUE;
	}
	else {
		EnumKeys.nCount=-1;
		EnumKeys.MaxNameLenght=0;
		return FALSE;
	}
}


BOOL CValueRegistry::GetKey(CValue &rez,CValue**ppV){
	rez=0;
	if(TYPE_EMPTY!=EnumKeys.CurKey.GetType())
		EnumKeys.CurKey=CValue();
	EnumKeys.CurName.Empty();
	if(!HKEY(RKEY)
		|| -1==EnumKeys.nCount)return FALSE;
	
	if (EnumKeys.nCount <= EnumKeys.curIndex+1)	{
		return FALSE;
	}
	EnumKeys.curIndex++;
	int nName=EnumKeys.MaxNameLenght;
	LPTSTR bufName=EnumKeys.CurName.GetBuffer(nName);
	int bRez=::RegEnumKeyEx(RKEY,EnumKeys.curIndex,bufName
		,(DWORD*)&nName,0,NULL,NULL,NULL);
	EnumKeys.CurName.ReleaseBuffer(nName);
	if(ERROR_SUCCESS!=bRez){
		EnumKeys.Clear();
		return FALSE;
	}
	rez=1;
	return TRUE;
}



CValue CValueRegistry::GetAttribute(int iName)//значение атрибута
{

	CValue Ret;
	switch(iName){
	case propKey:
			if(TYPE_EMPTY==EnumKeys.CurKey.GetType()
				&& -1<EnumKeys.nCount
				&& !EnumKeys.CurName.IsEmpty()){
				CValueRegistry*vr=new CValueRegistry(RKEY,EnumKeys.CurName);
				if(vr){
					EnumKeys.CurKey.nType=TYPE_REFFER;
					EnumKeys.CurKey.pRef=vr;
					vr->IncrRef();
				}
			}
		Ret=EnumKeys.CurKey;
		break;
	case propKName:
		Ret=CValue(EnumKeys.CurName);
		break;
	case propVal:
		Ret=CValue(EnumVals.CurVal);
		break;
	case propVName:
		Ret=CValue(EnumVals.CurName);
		break;
	
	default:

		break;
	}
	return Ret;
}
