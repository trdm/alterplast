// Value.cpp: implementation of the CValue class.
//
//Авторские права - www.vtools.ru (info@vtools.ru)
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "Value.h"
#include "OutToDebug.h"
#include "ValueOLE.h"
#include "ValueArray.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CValue,CObject);
IMPLEMENT_DYNCREATE(CValueNumber0,CValue);
IMPLEMENT_DYNCREATE(CValueString0,CValue);
IMPLEMENT_DYNCREATE(CValueDate0,CValue);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef MYDEBUG
 //#define _MYDEBUG
#endif

//bReadOnly

static int nCreateCount=0;
#ifdef _MYDEBUG

#define _MYDEBUG_CREATE() OutToDebug("Create %d\n",nCreateCount++)

#else

#define _MYDEBUG_CREATE() 

#endif
CValue::CValue()
{
	pRef=0;
	nRef=0;
	bReadOnly=0;
	nType=0;
	fData=0;

	_MYDEBUG_CREATE();
}

//конструктор копирования
CValue::CValue (const CValue& Val)
{
	pRef=0;
	nRef=0;
	bReadOnly=0;
	nType=0;
	fData=0;
	Copy(Val);
	_MYDEBUG_CREATE();
}

//Конструкторы по типам
#define CVALUE_BYTYPE(v_parclass,v_type,v_val) \
CValue::CValue (v_parclass cParam) \
{\
	pRef=0;\
	nRef=0;\
	bReadOnly=0;\
	nType=v_type;\
	v_val=cParam;\
	_MYDEBUG_CREATE();\
}

CVALUE_BYTYPE(NUMBER,TYPE_NUMBER,fData);
CVALUE_BYTYPE(int,TYPE_NUMBER,fData);
CVALUE_BYTYPE(DWORD,TYPE_NUMBER,fData);
CVALUE_BYTYPE(__int64,TYPE_NUMBER,fData);

CVALUE_BYTYPE(CString,TYPE_STRING,sData);
CVALUE_BYTYPE(LPCTSTR,TYPE_STRING,sData);

#undef CVALUE_BYTYPE




CValue::~CValue()
{
	if(nType==TYPE_REFFER&& pRef && pRef!=this)
		pRef->DecrRef();
#ifdef _MYDEBUG
	nCreateCount--;
	OutToDebug("Delete %d\n",nCreateCount);
#endif
}


//МЕТОДЫ:

void CValue::SetError(CString Str)
{
	Error(Str.GetBuffer(0));
}


void CValue::Copy(const CValue& cOld)
{
	if(this==&cOld)
		return;

	Reset();

	fData=0;
	nType=cOld.nType;
	switch(nType)
	{
		case TYPE_NUMBER:
			fData=cOld.fData;
			return;
		case TYPE_STRING:
			sData=cOld.sData;
			return;
		case TYPE_DATE:
			dData=cOld.dData;
			return;
		case TYPE_REFFER:
			pRef=cOld.pRef;
			pRef->IncrRef();
			return;
		case TYPE_EMPTY:
			return;
		default:
			nType=0;
	}
	return;
}

CValue::operator = (__int64 Val)
{
	SetValue(CValue(Val));
}
CValue::operator = (CValue &Val)
{
	if(this!=&Val)
	if(nType&&bReadOnly)
	{
		SetValue(Val);
		//return *this;
	}
	else
	{
		Copy(Val);
		//return *this;
	}
}


void CValue::SetValue(CValue &Val)
{
	if(this==&Val)
		return;

	if(nType==TYPE_REFFER)
	{
		pRef->SetValue(Val);
	}
	else
	{
		Copy(Val);
	}
}
void CValue::SetData(CValue &Val)
{
	if(this==&Val)
		return;

	switch(nType)
	{
	case TYPE_NUMBER:
		SetNumber(Val);
		return;
	case TYPE_STRING:
		SetString(Val);
		return;
	case TYPE_DATE:
		SetDate(Val);
		return;
	case TYPE_REFFER:
		if(pRef)
			pRef->SetData(Val);
		return;
	}

	SetValue(Val);
}

__int64 hex_string_to_long(CString hexStg);
void convert_long_to_hex(CString &str, __int64 num);

void CValue::SetNumber(CString Val)
{
	if(bReadOnly&&nType==TYPE_REFFER)
	{
		pRef->SetNumber(Val);
		return;
	}
	Reset();
	nType=TYPE_NUMBER;

	Val.TrimLeft();
	Val.MakeUpper();
	if(Val.Left(2)=="0X")
		fData=hex_string_to_long(Val.Mid(2));
	else
		fData=atof(Val);
}
void CValue::SetString(CString Val)
{
	if(bReadOnly&&nType==TYPE_REFFER)
	{
		pRef->SetString(Val);
		return;
	}

	Reset();
	nType=TYPE_STRING;
	sData=Val;
}
void CValue::SetDate(CString s)
{
	Copy(Date(s));
}


NUMBER CValue::GetNumber(void)const
{
	switch(nType)
	{
		case TYPE_NUMBER:
			return fData;
		case TYPE_STRING:
			{
				CString Val=GetString();
				Val.TrimLeft();
				Val.MakeUpper();
				if(Val.Left(2)=="0X")
					return hex_string_to_long(Val.Mid(2));
				else
					return atof(Val);
			}
		case TYPE_DATE:
			return GetDate();
		case TYPE_REFFER:
			return pRef->GetNumber();
	}
	return fData;
} 

CString CValue::GetString(void)const
{
	switch(nType)
	{
		case TYPE_NUMBER:
			{
				CString Str;
				NUMBER f=fData;
				Str.Format("%.6f",f);
				Str.TrimRight('0');
				Str.TrimRight('.');
				//Str.TrimRight(',');
				return Str;
			}
		case TYPE_STRING:
			return sData;
		case TYPE_DATE:
			{
				int nYear,nMonth,nDay;
				DateFromIntToYMD(dData,nYear,nMonth,nDay);
				CString strDate;
				strDate.Format("%02d.%02d.%04d",nDay,nMonth,nYear);
				return strDate;
			}
		case TYPE_REFFER:
			return pRef->GetString();
	};
	return sData;
}

int CValue::GetDate(void)const
{
	switch(nType)
	{
		case TYPE_NUMBER:
			{
				return (int)fData;
			}
		case TYPE_STRING:
			return Date(sData).dData;
		case TYPE_DATE:
			{
				return dData;
			}
		case TYPE_REFFER:
			return pRef->GetDate();
	};
	return dData;
}
int CValue::FromDate(int &nYear,int &nMonth,int &nDay)const
{
	int nCurDate=GetDate();
	DateFromIntToYMD(nCurDate,nYear,nMonth,nDay);
	return nCurDate;
}
int CValue::FromDate(int &nYear,int &nMonth,int &nDay,int &DayOfWeek,int &DayOfYear, int &WeekOfYear)const
{
	int nCurDate=GetDate();
	DateFromIntToYMD(nCurDate,nYear,nMonth,nDay);
	WeekOfYear=DayOfWeek=DayOfYear=0;
	if(nCurDate>DATE_DELTA)
	{
		COleDateTime oleTime(nYear, nMonth, nDay, 0, 0, 0);
		DayOfYear=oleTime.GetDayOfYear();
		DayOfWeek=oleTime.GetDayOfWeek()-1;
		if(DayOfWeek<1)
			DayOfWeek=7;

		WeekOfYear=1+(DayOfYear-1)/7;
		int nD=(1+(DayOfYear-1)%7);
		if(nD>DayOfWeek)
			WeekOfYear++;
	}
	return nCurDate;
}


void CValue::Detach()
{
	if(nType==TYPE_REFFER)
		pRef->Detach();
}

void CValue::Attach(void *pObj)
{
	if(nType==TYPE_REFFER)
		pRef->Attach(pObj);
}

void *CValue::GetAttach()
{
	return 0;
}

BOOL CValue::IsEmpty(void)
{
	switch(nType)
	{
		case TYPE_NUMBER:
			return !GetNumber();
		case TYPE_STRING:
			return GetString().IsEmpty();
		case TYPE_DATE:
			return !GetDate();//dData!=GetDate();
		case TYPE_REFFER:
			return pRef->IsEmpty();
		break;
	};
	return TRUE;
}

int CValue::GetType(void)const
{
	if(nType==TYPE_REFFER)
		return pRef->GetType();
	else
		return nType;
}
CString CValue::GetTypeString(void)const
{
	switch(nType)
	{
		case TYPE_NUMBER:
			return "Число";
		case TYPE_STRING:
			return "Строка";
		case TYPE_DATE:
			return "Дата";
		case TYPE_REFFER:
			return pRef->GetTypeString();
		case 100:
			return NOT_DEFINED;
		break;
	};
	return NOT_DEFINED;
}



void CValue::SaveToString(CString &Str)
{
	switch(nType)
	{
		case TYPE_NUMBER:
			{
				NUMBER f=GetNumber();
				if(f==int(f))
					Str.Format("{1/%d}",int(f));
				else
					Str.Format("{1/%f}",f);
				return;
			}
		case TYPE_STRING:

			Str=GetString();

			ReplaceBySave(Str);

			Str=CString("{2/")+Str+"}";

			return;
		case TYPE_DATE:
			Str.Format("{3/%d}",GetDate());
			return;
		case TYPE_REFFER:
			pRef->SaveToString(Str);
			return;
		break;
	};
	Str="{0/0}";
}
void CValue::LoadFromString(CString &Str)
{
	Reset();

	if(Str.GetLength()==0)
		return;
	if(Str.GetAt(0)!='{')
		return;

	int nIndex1=Str.Find("/");
	if(nIndex1>1)
	{
		CString csType=Str.Mid(1,nIndex1-1);
		int nDataType=atoi(csType);
		if(nDataType==0&&nIndex1>2)
		{
			CValue::CreateObject(csType);
			pRef->LoadFromString(Str);
			return;
		}
		nType=nDataType;

		int nIndex2=Str.Find("}");
		CString csValue=Str.Mid(nIndex1+1,nIndex2-nIndex1-1);
		switch(nType)
		{
			case TYPE_NUMBER:
				fData=atof(csValue);
				return;
			case TYPE_STRING:
				sData=csValue;
				ReplaceByLoad(sData);
				return;
			case TYPE_DATE:
				dData=atoi(csValue);
				return;
			case TYPE_ARRAY:
				{
					CValue::CreateObject("Массив");
					pRef->LoadFromString(Str);
					return;
				}
			default:
				nType=TYPE_EMPTY;
		};
	}
}

//Поддержка массивов
CValue& CValue::GetAt(int nKey)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	return ((CValueArray *)pRef)->GetAt(nKey);
}
void CValue::SetAt(int nKey,CValue Val)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	((CValueArray *)pRef)->SetAt(nKey,Val);
}  
CValue& CValue::GetAt(CString Key)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	return ((CValueArray *)pRef)->GetAt(Key);
};
void CValue::SetAt(CString Key,CValue Val)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	((CValueArray *)pRef)->SetAt(Key,Val);
};


int CValue::GetSizeArray(void)
{
	if(GetType()==TYPE_ARRAY)
		return 	((CValueArray *)pRef)->GetSizeArray();
	else
		return 0;
}
void CValue::SetSizeArray(int nSize)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	((CValueArray *)pRef)->SetSizeArray(nSize);
}

int CValue::GetSizeStrArray(void)
{
	if(GetType()==TYPE_ARRAY)
		return 	((CValueArray *)pRef)->GetSizeStrArray();
	else
		return 0;
}
void CValue::SetSizeStrArray(int nSize)
{
	if(GetType()!=TYPE_ARRAY)
		SetError("Объект не является массивом");
	((CValueArray *)pRef)->SetSizeStrArray(nSize);
}



#define BOOL_IMPLEMENT_OPERATOR(x)\
BOOL CValue::operator x (CValue &cVal)\
{\
	int nMode=nType;\
	if(cVal.nType!=nMode)\
		nMode=0;\
	switch(nMode)\
	{\
		case TYPE_NUMBER:\
			return GetNumber() x cVal.GetNumber();\
		break;\
		case TYPE_DATE:\
			return GetDate() x cVal.GetDate();\
		break;\
		default:\
			{\
				CString Str1=GetString();\
				CString Str2=cVal.GetString();\
				return Str1 x Str2;\
			}\
	};\
	return FALSE;\
}


BOOL_IMPLEMENT_OPERATOR(>)
BOOL_IMPLEMENT_OPERATOR(>=)
BOOL_IMPLEMENT_OPERATOR(<)
BOOL_IMPLEMENT_OPERATOR(<=)
BOOL_IMPLEMENT_OPERATOR(==)


CValue::operator NUMBER ()
{
	return GetNumber();
};

CValue::operator CString ()
{
	return GetString();
};




//******************************
//поддержка ссылок на объекты
//******************************
void  CValue::IncrRef(void)
{
	nRef++;
}
void  CValue::DecrRef(void)
{
	nRef--;
	if(nRef==0)
		delete this;
}







//******************************
//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
//******************************
CMethods* CValue::GetPMethods(void)
{
	return NULL;
}

void CValue::PrepareNames(void)
{
}


#define GET_THIS \
CValue *pThis;\
if(nType==TYPE_REFFER&&pRef)\
	pThis=pRef;\
else\
	pThis=this;

#define GET_THIS0 \
CValue *pThis=0;\
if(nType==TYPE_REFFER&&pRef)\
	pThis=pRef;


CValue CValue::Method(CString csName,CValue **aParams)
{
	GET_THIS
	int iName=FindMethod(csName);
	if(iName>=0)
		return pThis->Method(iName,aParams);

	SetError(CString("Значение не представляет агрегатный объект (")+GetTypeString()+"."+csName+")");
	return *this;
}

//CValue CValue::Method(CString csName,CValue **aParams)
//{return Method(csName,aParams);}


CValue CValue::Method(int iName,CValue **aParams)
{
	GET_THIS
	if(iName>=0)
		return pThis->Method(iName,aParams);

	SetError("Значение не представляет агрегатный объект");
	return *this;
}

void CValue::SetAttribute(CString csName,CValue &Val)
{
	int iName=FindAttribute(csName);
	GET_THIS0
	if(iName<0||pThis==0)
		SetError(CString("Значение не представляет агрегатный объект (")+csName+")");
	pThis->SetAttribute(iName,Val);
}
void CValue::SetAttribute(int iName,CValue &Val)
{
	GET_THIS0
	if(iName<0||pThis==0)
		SetError("Значение не представляет агрегатный объект");
	pThis->SetAttribute(iName,Val);
}
CValue CValue::GetAttribute(CString csName)
{
	int iName=FindAttribute(csName);
	GET_THIS0
	if(iName<0||pThis==0)
		SetError(CString("Значение не представляет агрегатный объект (")+csName+")");
	return pThis->GetAttribute(iName);
}
CValue CValue::GetAttribute(int iName)
{
	GET_THIS0
	if(iName>=0&&pThis)
		return pThis->GetAttribute(iName);
	SetError("Значение не представляет агрегатный объект");
	return *this;
}


#define GET_PMETHODS \
CValue *pThis;\
if(nType==TYPE_REFFER)\
	pThis=pRef;\
else\
	pThis=this;\
CMethods* pMethods=pThis->GetPMethods();

int CValue::FindMethod(CString  csName)
{
	GET_PMETHODS
	if(pMethods)
		return pMethods->FindMethod(csName);
	if(pRef&&nType==TYPE_REFFER)
		return pRef->FindMethod(csName);
	return -1;
}
int  CValue::FindAttribute(CString  csName)
{
	GET_PMETHODS
	if(pMethods)
	{
		int nRes=pMethods->FindAttribute(csName);
		if(nRes>=0)
			return nRes;
	}
	if(pRef&&nType==TYPE_REFFER)
		return pRef->FindAttribute(csName);
	return -1;
}
CString CValue::GetMethodName(int nNumber,int nAlias)
{
	GET_PMETHODS
	if(pMethods)
		return pMethods->GetMethodName(nNumber,nAlias);
	if(pRef&&nType==TYPE_REFFER)
		return pThis->GetMethodName(nNumber,nAlias);
	return "";
}
char* CValue::GetMethodDescription(int nNumber,int nAlias)
{
	GET_PMETHODS
	if(pMethods)
		return pMethods->GetMethodDescription(nNumber,nAlias);
	if(pRef&&nType==TYPE_REFFER)
		return pThis->GetMethodDescription(nNumber,nAlias);
	return 0;
}

CString CValue::GetAttributeName(int nNumber,int nAlias)
{
	GET_PMETHODS
	if(pMethods)
	{
		CString Res=pMethods->GetAttributeName(nNumber,nAlias);
		if(!Res.IsEmpty())
			return Res;
	}
	if(pRef&&nType==TYPE_REFFER)
		return pThis->GetAttributeName(nNumber,nAlias);
	return "";
}
int CValue::GetNMethods(void)
{
	GET_PMETHODS
	if(pMethods)
		return pMethods->GetNMethods();
	if(pRef&&nType==TYPE_REFFER)
		return pThis->GetNMethods();
	return 0;
}
int CValue::GetNAttributes(void)
{
	GET_PMETHODS
	if(pMethods)
	{
		int nRes=pMethods->GetNAttributes();
		if(nRes>0)
			return nRes;
	}
	if(pRef&&nType==TYPE_REFFER)
		return pThis->GetNAttributes();
	return 0;
}



//******************************
//ДИНАМИЧЕСКОЕ СОЗДАНИЕ ОБЪЕКТОВ
//******************************
CMapStringToPtr CValue::ObjectsTree;//дерево поиска класса объекта по нормализованному имени (в верх. регистре)
CMap <void*,void*,CString,CString&> ObjectsName;
void CValue::CreateObject(CString Str,CValue Param)
{
	Reset();

	CString STR=Str;
	STR.MakeUpper();
	CRuntimeClass* pRuntimeClass=(CRuntimeClass*)ObjectsTree[STR];
	if(!pRuntimeClass)
	if(STR.Find("."))//в строку создания передан вид объекта
	{
		CString csTYPE=STR.Left(STR.Find("."));
		pRuntimeClass=(CRuntimeClass*)ObjectsTree[csTYPE];
	}

	if(!pRuntimeClass)
	{
		//ищем в OLE объектах
		CValueOLE *pVal=new CValueOLE();
		BOOL bRes=pVal->Create(Str);
		if(bRes)
		{
			pRef=pVal;
			pRef->sData=Str;
			pRef->IncrRef();
			pRef->nType=100;
			nType=TYPE_REFFER;
			return;
		}
		delete pVal;

		SetError(CString("Ошибка создания объекта ")+Str);
	}

	CValue* Val=((CValue*) pRuntimeClass->CreateObject());
	pRef=Val;
	pRef->IncrRef();
	//pRef->nType=100;
	nType=TYPE_REFFER;
	pRef->Init(Str,Param);
}
void CValue::Init(CString StrVid,CValue Val)
{
}


CValue CreateObject(CString Str,CValue Val)
{
#ifdef CREATE_DEBUG
//	CValue::csCreateName="Res";
#endif
	CValue Res;
	Res.CreateObject(Str,Val);
	return Res;
}

//Пример: RegisterObject(RUNTIME_CLASS(CValue),"Значение");
void RegisterObject(CRuntimeClass* pRuntimeClass,CString Str)
{
	if(Str.IsEmpty())
		return;
		//Error("Попытка зарегистрировать пустое значение как объект");

	void *p=(void*)pRuntimeClass;
	Str.TrimRight();
	CString STR=Str;
	STR.MakeUpper();
	CValue::ObjectsTree[STR]=p;

	//запоминаем список зарегистрированных объектов
	if(ObjectsName[p].IsEmpty())
		ObjectsName[p]=Str;

	//инициализируем наименования методов и атрибутов
	CValue *pThis = ((CValue*) pRuntimeClass->CreateObject());
	pThis->PrepareNames();

	delete pThis;
}
void UnRegisterObject(CString Str)
{
	CString STR=Str;
	STR.MakeUpper();


	void *p=CValue::ObjectsTree[STR];
	if(p)
	{
		ObjectsName[p]="";
		CValue::ObjectsTree[STR]=0;
	}
}

BOOL IsRegisterObject(CString Str)
{
	int nIndex=Str.Find(".");
	if(nIndex>0)
		Str=Str.Left(nIndex);

	CString STR=Str;
	STR.MakeUpper();
	void *Value;
	return CValue::ObjectsTree.Lookup(STR,Value);
}

void AttachRegisterObject(CString StrParent,CString StrChild)
{
	CString STRPARENT=StrParent;
	STRPARENT.MakeUpper();

	CRuntimeClass* p=(CRuntimeClass* )CValue::ObjectsTree[STRPARENT];
	if(!p)
		Error(CString("Объект ")+StrParent+" не найден");

	CString STRCHILD=StrChild;
	STRCHILD.MakeUpper();
	CValue::ObjectsTree[STRCHILD]=p;

	if(ObjectsName[p].IsEmpty())
		ObjectsName[p]=StrParent;
}

CMap <void*,void*,CString,CString&> *GetRegistersObjectsArray()
{
	return &ObjectsName;

}

//*****************
//СИСТЕМНЫЕ ФУНКЦИИ
//*****************
CValue Number(NUMBER cParam)
{
	return cParam;
}

CValue String(CString cParam)
{
	return CValue(cParam);
}

CValue String(LPCTSTR cParam)
{
	return CValue(cParam);
}

CValue Number(CValue cParam)
{//преобразование параметра в CValue типа число
	CValue Ret;
	Ret.nType=TYPE_NUMBER;
	Ret.fData=cParam.GetNumber();
	return Ret;
}
CValue String(CValue cParam)
{
	CValue Ret;
	Ret.nType=TYPE_STRING;
	Ret.sData=cParam.GetString();
	return Ret;
}
CValue Date(int cParam)
{
	CValue Ret;
	Ret.nType=TYPE_DATE;
	Ret.dData=cParam;
	return Ret;
}

//---------------------------------------------------------------------------------------
//Добавлено по сообщениям alexqc с форума http://www.vtools.ru/forum/viewtopic.php?p=126
//---------------------------------------------------------------------------------------
enum 
{ 
	DayInYear = 365, 
	DayIn4Year = DayInYear*4+1, 
	DayIn100Year = DayIn4Year *25 -1, 
	DayIn400Year = DayIn100Year *4 +1, 

	OFFS_Date_Julian = 1721119   //Смещение юлианских дат 
}; 
void DateFromYMDToInt(int &nCurDate,int nYear,int nMonth,int nDay){ 
   nCurDate=0; 
   if(0>=nYear 
      || 0>=nMonth && 12<nMonth 
      || 0>=nDay )return; 

  if(2 < nMonth) 
     nMonth -=3; 
  else { 
     nMonth +=9; 
     nYear -=1; 
  } 

  nCurDate = 365*nYear + nYear/4 - nYear/100 + nYear/400; 
  nCurDate += (153*nMonth + 2) / 5; 
  nCurDate += nDay; 
  nCurDate += OFFS_Date_Julian; 
      
} 
void DateFromIntToYMD(int nCurDate,int &nYear,int &nMonth,int &nDay) 
{ 
   nYear=nMonth=nDay=0; 

   nCurDate -= OFFS_Date_Julian; 
   if(0 >= nCurDate )return; 

   int x = 4 * nCurDate- 1; 
   nYear = (x / DayIn400Year) * 100; 
   x= (x % DayIn400Year) | 3; 

   nYear += x / DayIn4Year; 
   x = 5* (x % DayIn4Year / 4)+2; 

   nMonth = x/153 + 1; 
   nDay = (x%153)/5 + 1; 

  if (nMonth < 11) 
       nMonth += 2; 
   else { 
      nYear++; 
      nMonth -= 10; 
   } 
} 
CValue Date(int nYear,int nMonth,int nDay) 
{ 
   CValue cRes; 
   cRes.nType=TYPE_DATE; 
   if(nYear==0&&nMonth==0&&nDay==0) 
      return cRes; 
   if(nYear<20) 
      nYear+=2000; 
   if(nYear<100) 
      nYear+=1900; 
   DateFromYMDToInt(cRes.dData,nYear,nMonth,nDay); 
   return cRes; 
} 
CValue Date(CString Str,int &nYear,int &nMonth,int &nDay) 
{ 
   if(3 == sscanf(Str,"%2d%*c%2d%*c%4d",&nDay,&nMonth,&nYear)) 
   { 
      return Date(nYear, nMonth, nDay); 
   } 
   if(Str.IsEmpty())
	   return Date(CValue()); 
   else
	   return Date(CValue()); //return String(Str);//поддержка задания строк через апострофы
} 
//---------------------------------------------------------------------------------------
/*
CValue Date(int nYear,int nMonth,int nDay)
{
	CValue cRes;
	cRes.nType=TYPE_DATE;
	if(nYear==0&&nMonth==0&&nDay==0)
		return cRes;
	if(nYear<20)
		nYear+=2000;
	if(nYear<100)
		nYear+=1900; 
	struct tm time={ 0, 0, 0, nDay, nMonth-1, nYear-1900 };
	cRes.dData=(mktime( &time)-SEC1_DELTA)/(SEC2_DELTA)+DATE_DELTA;
	return cRes;
}
CValue Date(CString Str,int &nYear,int &nMonth,int &nDay)
{
	int i1=Str.Find(".");
	int i2=Str.Find(".",i1+1);
	if(i2>0)
	{
		nDay=atoi(Str.Left(i1));
		nMonth=atoi(Str.Mid(i1+1,i2-i1-1));
		nYear=atoi(Str.Mid(i2+1,4));

		return Date(nYear, nMonth, nDay);
	}
	return CValue();
}
void DateFromIntToYMD(int nCurDate,int &nYear,int &nMonth,int &nDay)
{
	nYear=nMonth=nDay=0;
	if(nCurDate>DATE_DELTA)
	{
		long long_time=(nCurDate-DATE_DELTA)*SEC2_DELTA+SEC1_DELTA;
		CTime tempTime=long_time;
		nDay=tempTime.GetDay();
		nMonth=tempTime.GetMonth();
		nYear=tempTime.GetYear();
	}
}
*/
CValue Date(CString Str)
{
	int nYear,nMonth,nDay;
	return Date(Str,nYear,nMonth,nDay);
}

CValue Date(CValue cParam,int &nYear,int &nMonth,int &nDay,BOOL bReturn)
{
	CValue cRes;
	cRes.nType=TYPE_DATE;
	switch(cParam.nType)
	{
		case TYPE_NUMBER:
			cRes.dData=(int)cParam.GetNumber();
		break;
		case TYPE_STRING:
			return Date(cParam.GetString());
		break;
		case TYPE_DATE:
			cRes.dData=cParam.GetDate();
		break;
	}

	if(bReturn)
	{
		Date(cRes.GetString(),nYear,nMonth,nDay);
	}

	return cRes;
}
CValue Date(CValue cParam)
{
	int nYear,nMonth,nDay;
	return Date(cParam,nYear,nMonth,nDay);
}
CValue ValueByRef (CValue *pRef)
{
	CValue Val;
	Val.nType=TYPE_REFFER;
	Val.pRef=pRef;
	pRef->IncrRef();
	return Val;
}

//получить текущее значение (актуального для агрегатных объектов или объектов диалога)
CValue CValue::GetValue(int nThis)
{
	if(nThis) 
		return ValueByRef(this);
	if(nType==TYPE_REFFER)  
		return pRef->GetValue(1);//цифра 1 - признак создания новой переменной - ссылки на агрегатный объект
	else
		return *this;
}



/*
void Test()
{
	CMap <CString2 ,CString2 ,int ,int > aValueList;

	CString2 Str1="Приход";
	aValueList[Str1]=100;

	CString2 Str2="Расход";
	aValueList[Str2]=200;

	CString2 Str3="Test3";
	aValueList[Str3]=300;

	int nRes1=aValueList[Str1];
	int nRes2=aValueList[Str2];
	int nRes3=aValueList[Str3];
};


class AutoRun0
{
public:
	AutoRun0()
	{
		Test();
	};
}cRun;

//*/



/*
//********
//UnitTest
//********
void CValue::UnitTest(void)
{

	CString Str;



	RegisterObject(RUNTIME_CLASS(CValue),"Значение");


	//CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CValue);
	//CObject* pObject = pRuntimeClass->CreateObject();

	CValue V1;
	CValue V2;
	CValue V3;
	V1=::CreateObject("Значение");
	V2=::CreateObject("Значение");
	V3=::CreateObject("Значение");
	V1=V2=V3;

	V1=1;
	V3=::CreateObject("Значение");
	CValue V=::CreateObject("Значение");
	V=::CreateObject("Значение");
	V=::CreateObject("Значение");
	V=V;

	//CData::UnitTest();

	//проверка даты:
	if(_D("01.01.2001")!=_D("01.01.2001"))AfxMessageBox("Error Data 1");
	CValue D1=Date("01.01.2001");
	CValue D2=Date("01.01.01");
	if(_D("01.01.2001")!=_D("01.01.01"))AfxMessageBox("Error Data 2");
	if(_D("01.01.2001")==_D("01.01.1001"))AfxMessageBox("Error Data 3");
	if(_D("01.01.1997")!=_D("01.01.97"))AfxMessageBox("Error Data 4");
	if(_D("01.01.1980")!=_D("01.01.80"))AfxMessageBox("Error Data 5");
	if(_D("01.01.2010")!=_D("01.01.10"))AfxMessageBox("Error Data 6");
	//операции арифметики c датой
	D1=D1+1;
	if(D2!=D1-1)AfxMessageBox("Error Data 7");
	if(D2*3!=(D1-1)*3)AfxMessageBox("Error Data 8");
	if(D2+1!=D1)AfxMessageBox("Error Data 9");
	if(D2!=_S("01.01.2001"))AfxMessageBox("Error Data 9");
	if(D1!=_S("02.01.2001"))AfxMessageBox("Error Data 9");
	if(D2!=_S("01.01.01"))AfxMessageBox("Error Data 9");
	if(_S("01.01.2001")!=D2)AfxMessageBox("Error Data 10");
	if(_S("01.01.01")==D2)AfxMessageBox("Error Data 11");

	D1=D1+9;

	if(D1-D2!=10)AfxMessageBox("Error Data 12");
	if(10!=_N(D1-D2))AfxMessageBox("Error Data 13");
	if(10!=D1-D2)AfxMessageBox("Error Data 14");

	//сравнение дат:
	if(_D("01.01.01")!=_D("01.01.01"))AfxMessageBox("Error Data 12");
	if(_D("01.01.01")==_D("02.01.01"))AfxMessageBox("Error Data 13");
	if(_D("01.01.01")<_D("01.01.01"))AfxMessageBox("Error Data 14");
	if(_D("01.01.01")>_D("01.01.01"))AfxMessageBox("Error Data 15");
	if(!(_D("02.01.01")>_D("01.01.01")))AfxMessageBox("Error Data 16");
	if(!(_D("01.01.02")>_D("01.01.01")))AfxMessageBox("Error Data 17");
	if(!(_D("31.12.02")<_D("01.01.03")))AfxMessageBox("Error Data 18");
	if(!(_D("31.12.02")<=_D("31.12.02")))AfxMessageBox("Error Data 19");
	if(!(_D("31.12.02")>=_D("31.12.02")))AfxMessageBox("Error Data 20");


	//преобразования строка, дата, число
	CValue N1=10;
	N1+=1;
	if(N1!=11)AfxMessageBox("Error Type 1");
	if(11!=N1)AfxMessageBox("Error Type 2");
	if(10!=N1-1)AfxMessageBox("Error Type 3");
	N1=N1+3;
	if(N1!=14)AfxMessageBox("Error Type 4");
	N1=N1-7;
	if(N1!=7)AfxMessageBox("Error Type 5");

	N1=11+Date("01.01.01");
	if(N1!=2451922)AfxMessageBox("Error Type 6");
	if(2451922!=N1)AfxMessageBox("Error Type 6-1");
	if(2451911!=Date("01.01.01"))AfxMessageBox("Error Type 6-2");
	if(Date("01.01.01")!=2451911)AfxMessageBox("Error Type 6-3");
	N1=11+Date("01.01.01")-_V("2451912");
	if(N1!=10)AfxMessageBox("Error Type 7");
	N1="123";
	N1=N1+4;
	if(N1!=1234)AfxMessageBox("Error Type 8");
	if(N1!="1234")AfxMessageBox("Error Type 9");
	N1="";
	N1=N1+"123";
	if(Date(2451911)!=Date("01.01.01"))AfxMessageBox("Error Type 10");
	if(Date("01.01.01")!=Date(2451911))AfxMessageBox("Error Type 11");

	//число - строка
	if(Number("12345.12345")!=12345.12345)AfxMessageBox("Error Type 12");
	if(12345.12345!=Number("12345.12345"))AfxMessageBox("Error Type 13");
	N1="12345.12345";
	if(12345.12345!=Number(N1))AfxMessageBox("Error Type 14");
	N1=Number("123.1");
	if(N1!=123.1)AfxMessageBox("Error Type 15");
	N1=123.01;
	if(N1!="123.01")AfxMessageBox("Error Type 16");
	if("123.01"!=N1)AfxMessageBox("Error Type 17");

	N1="";
	N1=N1+"123"+N1+"12"+22+"!!"+"SS";

//	CValue T=Number(Date("01.01.2001"));
	Str=N1;

	//сравнение чисел
	if(_V(12323232)!=_V(12323232))AfxMessageBox("Error Number 1");
	if(_V(12323232)<_V(12323232))AfxMessageBox("Error Number 2");
	if(_V(12323232)>_V(12323232))AfxMessageBox("Error Number 3");
	if(!(_V(12323232)>=_V(12323232)))AfxMessageBox("Error Number 4");
	if(!(_V(12323232.12344)<=_V(12323232.12344)))AfxMessageBox("Error Number 5");
	if(!(_V(12323232.12344)!=_V(12323232.123441)))AfxMessageBox("Error Number 6");


	//проверка ссылчного типа объекта
	RegisterObject(RUNTIME_CLASS(CValue),"CValue");
	{
		{
		CValue V1;
		V1.CreateObject("CValue");
		CValue V2=V1;
		CValue V3=V1;
		V1=125;
		if(125!=V1)AfxMessageBox("Error Reffer 1");
		if(125==V2)AfxMessageBox("Error Reffer 2");
		if(V3==125)AfxMessageBox("Error Reffer 3");
		V3=V1+125;
		if(V3!=250)AfxMessageBox("Error Reffer 4");
		if(V1!=125)AfxMessageBox("Error Reffer 5");
		Str=V1;
		}
	}
	UnRegisterObject("CValue");

}
*/
//Авторские права - www.vtools.ru (info@vtools.ru)


//Вспомогательные функции для использования из C++
#define MAX_VAR_PARAMS	10
CValue ParamMas[MAX_VAR_PARAMS];
/*CValue **GetArray()
{
	CValue **ppParams=new CValue*[MAX_VAR_PARAMS];
	for(int i=0;i<MAX_VAR_PARAMS;i++)
		ppParams[i]=&ParamMas[i];
	return ppParams;
} */


CValue CValue::CallFunctionV(CString csName,CValue **p)
{
	return Method(csName,p); 
}

CValue CValue::CallFunction(CString csName,...)
{
	va_list lst;
	va_start(lst,csName);
	CValue **ppParams=(CValue**) lst;
	va_end(lst);
	CValue Ret=Method(csName,ppParams);
	return Ret;
}
/*CValue CValue::CallFunction(CString csName,CValue &vParam1)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}

CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	ppParams[4]=&vParam5;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5,CValue &vParam6)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	ppParams[4]=&vParam5;
	ppParams[5]=&vParam6;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5,CValue &vParam6,CValue &vParam7)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	ppParams[4]=&vParam5;
	ppParams[5]=&vParam6;
	ppParams[6]=&vParam7;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValue::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5,CValue &vParam6,CValue &vParam7,CValue &vParam8)
{
	CValue **ppParams=GetArray();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	ppParams[4]=&vParam5;
	ppParams[5]=&vParam6;
	ppParams[6]=&vParam7;
	ppParams[7]=&vParam8;
	CValue Ret=Method(csName,ppParams);
	delete []ppParams;
	return Ret;
} */



__int64 hex_string_to_long(CString hexStg)
{
  int n= 0;				 // position in string
  __int64 intValue = 0;  // integer value of hex string
  CUIntArray digit;		 // hold values to convert

  //tokenize all digits to an array containing their value
  while (n < strlen(hexStg))
  {
     if (hexStg[n]=='\0')
        break;
     if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
        digit.Add(hexStg[n] & 0x0f);            //convert to int
     else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
        digit.Add((hexStg[n] & 0x0f) + 9);      //convert to int
     else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
        digit.Add((hexStg[n] & 0x0f) + 9);      //convert to int
     else break;
    n++;
  }
  
  n--;					//back one digit
  int p=0;				//power factor
  while(n >=0) 
  {
     intValue = intValue + (digit.GetAt(n)*(__int64)pow (2,p));
     n--;   // next digit to process
	 p+=4;	//everytime the power goes up in 4
  }
  
  return intValue;

}


//convert a long to a hex string
void convert_long_to_hex(CString &str, __int64 num)
{
	//1) divide the number to 16
	//2) use the reminder to index it to get the digit in hex
	//3) divide the result from before (less the reminder) in 16 and so on

	CStringArray digits;//array of all hex digit we will index it in reverse later
	str="";//reset the string
	__int64 rem=0;//reminder
	
	char hex[16][2]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};

	
	do 
	{
	  rem=num%16;
		if (rem==0 && num==0)
			break;
	  digits.Add (hex[rem]);
	  num/=16;
	} while (num>=0);

	//combine to a string 

	for (int add=digits.GetSize()-1;add>=0;add--)
		str+=digits.GetAt(add);

}


/*
__int64 hex_string_to_long(CString hexStg)
{
	int n= 0;                              // position in string
	__int64 intValue = 0;  // integer value of hex string
	
	LPCTSTR lpcBuf=hexStg;
	
	int digit;
	
	for(;;)
	{
		if( '0'<=*hexStg && *hexStg<='9' )
			digit=*hexStg-'0';
		else if( 'A'<=*hexStg && *hexStg<='F' )
			digit=*hexStg-'A'+10;
		else if( 'a'<=*hexStg && *hexStg<='f' )
			digit=*hexStg-'a'+10;
		else
			break;
		
		intValue<<=4;
		intValue+=digit;
	}
	return intValue;
}

void convert_long_to_hex(CString &str, __int64 num)
{
     str.Format("%I64X",num);
}
*/

CValue GetTypeValue(CString Str)
{
	CValue Value;
	Str.TrimRight();
	if(Str=="Число")
	{
		Value.nType=TYPE_NUMBER;
	}
	else
	if(Str=="Дата")
	{
		Value.nType=TYPE_DATE;
	}
	else
	if(Str=="Строка")
	{
		Value.nType=TYPE_STRING;
	}
	else
	if(!Str.IsEmpty())
		Value.CreateObject(Str);
	return Value;
}
CString GetCellString(CValue &Value)
{
	if(Value.nType==TYPE_NUMBER&&Value.fData==0)
		return "";
	if(Value.nType==TYPE_DATE&&Value.dData==0)
		return "";
	return Value.GetString();
}



