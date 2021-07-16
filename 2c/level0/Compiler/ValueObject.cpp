// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueObject.cpp: implementation of the CValueObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueObject,CValueControl);

CValueObject::CValueObject()
{
	nType=100;
	bReadOnly=1;
	pRun=0;
	nMethodGetAttr=-1;
	nMethodSetAttr=-1;
	bHasParent=0;
}
 
CValueObject::~CValueObject()
{
	if(pRun) 
	{
		pRun->CallFunction("Деструктор");
		delete pRun;
	}
}

void CValueObject::SetObjectType(CString StrTypeVid)
{
	//полное имя класса объекта
	csObjectName=StrTypeVid;

	//тип объекта 
	int nIndex=csObjectName.Find(".");
	if(nIndex>=0)
	{
		//вид объекта
		csObjectType=csObjectName.Left(nIndex);
		csObjectKind=csObjectName.Mid(nIndex+1);
	}
	else
	{
		csObjectType=csObjectName;
		csObjectKind="";
	}

	csObjectType.TrimRight();
	csObjectType.TrimLeft();
}

//Создание (проецирование) нового исполняемого модуля в памяти
void CValueObject::Init(CString StrTypeVid,CValue Param)
{
	ASSERT(AfxGetModuleManager());

	SetObjectType(StrTypeVid);
	if(AfxGetModuleManager())
		pRun=AfxGetModuleManager()->GetNewRunModule(OBJECTSNAME+"\\"+csObjectType+"\\"+OBJMODULE+"\\"+ENTERPRISEMODULENAME);
	if(!pRun)
	{
		CString csStr;
		csStr.Format("Ошибка создания макрообъекта %s (ошибка создания модуля)",csObjectName);
		Error(csStr.GetBuffer(0));
	}

	InitObject(pRun,Param);
}

void CValueObject::InitObject(CProcUnit *pSetRun,CValue Param)
{
	static int nLevel=0;
	nLevel++;
	if(nLevel>=MAX_OBJECTS_LEVEL)
		Error(CString("Рекурсивный вызов объекта ")+csObjectName);

	bHasParent=0;

	pRun=pSetRun;
	pRun->pCurrentObj=this;
	//Контекст = ссылка на объект
	//pRun->cCurContext.cRefLocVars[0]=this;//!
	pRun->cCurContext.pRefLocVars[0]=this;

	//ищем родителя
	for(int i=0;i<pRun->GetParentCount();i++)
	{
		CProcUnit *pCurRun=pRun->GetParent(i);
		ASSERT(pCurRun->pByteCode);
		if(pCurRun->pByteCode->csModuleName.Right(ENTERPRISEMODULENAME.GetLength())==ENTERPRISEMODULENAME)
		{
			int nType;
			CString csName=AfxGetModuleManager()->GetObjectName(pCurRun->pByteCode->csModuleName,nType);

			//создаем оболочку объекта на базе уже спроецированного в память модуля
			CValueObject *p=new CValueObject();
			p->SetObjectType(csName);
			p->InitObject(pCurRun,Param);

			pRun->cCurContext.pRefLocVars[1]=p;
			vParent=ValueByRef(p);
			break;
		}
	}

	bHasParent=vParent.GetType();
	SetParentContext();

	ASSERT(pRun);
	pRun->CallFunction("Конструктор",String(csObjectKind),Param);
	nLevel--;
}

//________________________________________________________________________
//присваиваем дочерний контекст всем родителям
void CValueObject::SetParentContext()//для полиморфизма через атрибут Контекст
{
	if(bHasParent)
	if(vParent.pRef)
	{
		if(vParent.pRef->GetRuntimeClass()==&classCValueObject)
		{
			CValueObject *pParent=(CValueObject*)vParent.pRef;
			//Контекст = ссылка на объект
			//pParent->pRun->cCurContext.cRefLocVars[0]=this;//!
			pParent->pRun->cCurContext.pRefLocVars[0]=this;
			pParent->SetParentContext();
		}
	}
}

#define DELTA_COUNT	1000000//максимальное количество методов/строк модуля
//________________________________________________________________________
CValue CValueObject::Method(int iName,CValue **aParams)
{
#ifdef _DEBUG2
	if(pRun)
	{
		static int nGetName=0;
		if(!nGetName)
		{
			nGetName=1;
			csName=pRun->CallFunction("ПолучитьПредставлениеОбъекта").GetString();
			nGetName=0;
		}
	}
#endif

	int nFound=1;
	if(iName>=DELTA_COUNT)
	{
		nFound=0;
		iName=iName-DELTA_COUNT;
	}

	if(nFound)
	{
		if(iName>=0)
			return pRun->CallFunction(iName,aParams);
	}
	if(bHasParent)
		return vParent.Method(iName,aParams);

	return CValue();
}

//________________________________________________________________________
int  CValueObject::FindMethod(CString csName)
{
	int n=(int)pRun->FindExportFunction(csName);
	if(n>=0)
	{
		return n;
	}
	if(bHasParent)
	{
		n=vParent.FindMethod(csName);
		if(n>=0)
			return DELTA_COUNT+n;
	}
	return -1;
}

//________________________________________________________________________
CString csTempAttrName;
int  CValueObject::FindAttribute(CString csName)
{
	csTempAttrName=mUpper(csName);
	return 0;
}
//________________________________________________________________________
CValue CValueObject::GetAttribute(int )//получение значения атрибута
{
	if(!pRun)
		return CValue();
	//смотрим может у модуля есть экспортные переменные (к таким переменным выполняем прямое обращение)
	int nVar=pRun->FindAttribute(csTempAttrName);
	if(nVar>=0)
		return pRun->GetAttribute(nVar);

	if(nMethodGetAttr==-1)
		nMethodGetAttr=(int)pRun->FindFunction("ПолучитьАтрибут");
	if(nMethodGetAttr==-1)
	{
		if(bHasParent)
		{
			nVar=vParent.FindAttribute(csTempAttrName);
			if(nVar>=0)
				return vParent.GetAttribute(nVar);
		}

		CString csStr;
		csStr.Format("Поле (%s) агрегатного объекта не обнаружено",csTempAttrName);
		Error(csStr.GetBuffer(0));
	}

	CValue Attr;
	Attr.SetString(csTempAttrName);
	return pRun->CallFunction(nMethodGetAttr,Attr);
}
void CValueObject::SetAttribute(int ,CValue &Val)//установка атрибута
{
	if(!pRun)
		return;
	//смотрим может у модуля есть экспортные переменные (к таким переменным выполняем прямое обращение)
	int nVar=pRun->FindAttribute(mUpper(csTempAttrName));
	if(nVar>=0)
	{
		pRun->SetAttribute(nVar,Val);
		return;
	}

	if(nMethodSetAttr==-1)
		nMethodSetAttr=(int)pRun->FindFunction("УстановитьАтрибут");
	if(nMethodSetAttr==-1)
	{
		if(bHasParent)
		{
			nVar=vParent.FindAttribute(csTempAttrName);
			if(nVar>=0)
			{
				vParent.SetAttribute(nVar,Val);
				return;
			}
		}

		CString csStr;
		csStr.Format("Поле (%s) агрегатного объекта не обнаружено",csTempAttrName);
		Error(csStr.GetBuffer(0));
	}

	CValue Attr;
	Attr.SetString(csTempAttrName);
	pRun->CallFunction(nMethodSetAttr,Attr,Val);
}
int  CValueObject::GetNAttributes(void)
{
	if(!pRun)
		return 0;
	int nExport=pRun->pByteCode->ExportVarList.GetCount();
	int nInner=pRun->CallFunction("ПолучитьКоличествоАтрибутов");

	int nParent=0;
	if(bHasParent)
		nParent=vParent.GetNAttributes();

	return nExport+nInner+nParent;
}
CString CValueObject::GetAttributeName(int nNumber,int nAlias)
{
	if(!pRun)
		return "";
	int nExport=pRun->pByteCode->ExportVarList.GetCount();

	if(nNumber<nExport)
	{
		void* p;
		POSITION pos;
		CString Key;
		int nCount=0;
		for( pos = pRun->pByteCode->ExportVarList.GetStartPosition(); pos != NULL; )
		{
			pRun->pByteCode->ExportVarList.GetNextAssoc( pos, Key, p );
			if(nCount==nNumber)
			{
				csTempAttrName=Key;
				return Key;
			}
			nCount++;
		}

	}
	nNumber-=nExport;

	int nInner=pRun->CallFunction("ПолучитьКоличествоАтрибутов");
	if(nNumber<nInner)
	{
		csTempAttrName=pRun->CallFunction("ПолучитьИмяАтрибута",CValue(nNumber+1)).GetString();
		return csTempAttrName;
	}
	nNumber-=nInner;

	if(bHasParent)
		return  vParent.GetAttributeName(nNumber,nAlias);

	csTempAttrName="";

	return csTempAttrName;
}


void CValueObject::SaveToString(CString &Str)
{
	CString csTypeVid=csObjectName;
	int nIndex=csTypeVid.Find(".");
	if(nIndex==-1)
	{
		CString csVid=CallFunction("Вид").GetString();
		if(!csVid.IsEmpty())
			csTypeVid.Format("%s.%s",csObjectName,csVid);
	}

	Str.Format("{%s/%s}",csTypeVid,CallFunction("ПолучитьИдентификаторОбъекта").GetString());
}
void CValueObject::LoadFromString(CString &Str)
{ 
	int nIndex1=Str.Find("/");
	CValue cVal=String(Str.Mid(nIndex1+1));
	cVal.sData.TrimRight('}');
	CallFunction("УстановитьИдентификаторОбъекта",cVal);
}
CString CValueObject::GetString(void)const
{
	if(((CValueObject*)this)->FindMethod("ПолучитьПредставлениеОбъекта")<0)
		return GetTypeString();
	return ((CValueObject*)this)->CallFunction("ПолучитьПредставлениеОбъекта").GetString();
}
CString CValueObject::GetTypeString(void)const
{
	CString Str=((CValueObject*)this)->CallFunction("ПолучитьТипОбъекта").GetString();
	if(Str.IsEmpty())
		return csObjectType;
	else
		return Str;
}


void CValueObject::SetValue(CValue &Val)
{
	int  nSetValue=FindMethod("УстановитьЗначение");
	if(nSetValue>=0)
		CallFunction("УстановитьЗначение",Val);
	if(pWnd)
		((CWnd *)pWnd)->SetWindowText(GetString());
}


#define MAX_VAR_PARAMS	10
CValue ParamMas2[MAX_VAR_PARAMS];
CValue **GetArray2()
{
	CValue **ppParams2=new CValue*[MAX_VAR_PARAMS];
	for(int i=0;i<MAX_VAR_PARAMS;i++)
		ppParams2[i]=&ParamMas2[i];
	return ppParams2;
}

CValue CValueObject::CallFunction(CString csName)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValueObject::CallFunction(CString csName,CValue &vParam1)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	ppParams[0]=&vParam1;
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}
CValue CValueObject::CallFunction(CString csName,CValue &vParam1,CValue &vParam2)
{
	int  n=FindMethod(csName);
	if(n<0)
		return CValue();
	CValue **ppParams=GetArray2();
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2; 
	CValue Ret=Method(n,ppParams);
	delete []ppParams;
	return Ret;
}

BOOL CValueObject::IsEmpty(void)
{
	if(CallFunction("Выбран").GetNumber())
		return 0;
	else
		return 1;
}
