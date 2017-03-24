// VectorValue.cpp: implementation of the CVectorValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "VectorValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CVectorValue, CBLContext);

class CObjID CVectorValue::ObjID;


//////////////////////////////////////////////////////////////////////
// Описание методов и функций класса для 1С языка
// {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
//////////////////////////////////////////////////////////////////////
struct CVectorValue::paramdefs CVectorValue::defFnNames[] = {
  {"IsEmpty","Пустой",1,0},
  {"AddValue","ДобавитьЗначение",1,1},
  {"GetAt","Получить",1,1},
  {"GetSize","Размер",1,0},
  {"Reserve","Резервировать",0,1},
  
	{NULL,NULL,0,0}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVectorValue::CVectorValue()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());	
}

CVectorValue::~CVectorValue()
{  
}

int  CVectorValue::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
  switch(iMethNum)
  {
    case funcIsEmpty:
    { 
      rValue =  CNumeric(m_VectorOfValue.empty() ? 1 : 0);
      return 1;
    }
    break;
    case funcAddValue:
    {      
      m_VectorOfValue.push_back(ppValue[0]);           
      return 1;
    }
    break;
    case funcGetAt:
    {
      rValue = *(m_VectorOfValue[ppValue[0]->GetNumeric()]);
    }
    break;
    case funcGetSize:
    { 
      rValue = m_VectorOfValue.size();
      return 1;
    }
    break;
  default:
    {     
    }
  };
  return -1;
}

int  CVectorValue::CallAsProc(int iMethNum,class CValue * * ppValue)
{
  switch(iMethNum)
  {
    case procReserve:
    { 
      m_VectorOfValue.reserve(ppValue[0]->GetNumeric());
      return 1;
    }
    break;    
  default:
    {     
    }
  };
  
  return -1;
}


int  CVectorValue::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
  return -1;
}

char const *  CVectorValue::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (iMethodNum >= lastMethod) 
		return "mError";
	else
		return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CVectorValue::GetNMethods(void)const
{
	return lastMethod;
}

int  CVectorValue::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CVectorValue::GetNParams(int iMethodNum)const
{	
  return defFnNames[iMethodNum].NumberOfParams;
}

int  CVectorValue::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	return 0;
}



void  CVectorValue::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  CVectorValue::GetCode(void)const
{
	return 0;
}

int  CVectorValue::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CVectorValue::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CVectorValue::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CVectorValue::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}




long  CVectorValue::GetTypeID(void)const
{
	return 100;
}

char const *  CVectorValue::GetTypeString(void)const
{
	return "EventManager";
}

class CType   CVectorValue::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  CVectorValue::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CVectorValue::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CVectorValue::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CVectorValue::IsExactValue(void)const
{
	return 0;
}

int  CVectorValue::IsOleContext(void)const
{
	return 0;
}

int  CVectorValue::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CVectorValue::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CVectorValue::IsSerializable(void)
{
	return 0;
}

int  CVectorValue::SaveToString(class CString & csStr)
{
	csStr = "DJK";
	return 1;
}

void  CVectorValue::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CVectorValue::GetNProps(void)const
{
	return 0;
}

char const *  CVectorValue::GetPropName(int A,int B)const
{
	return NULL;
}

int  CVectorValue::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return -1;
}

int  CVectorValue::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return -1;
}

int  CVectorValue::FindProp(char const * Name)const
{
	return -1;
}
