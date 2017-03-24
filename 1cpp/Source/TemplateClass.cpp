// TemplateClass.cpp: implementation of the CTemplateClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "TemplateClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CTemplateClass, CBLContext);

class CObjID CTemplateClass::ObjID;

//////////////////////////////////////////////////////////////////////
// Описание методов и функций класса для 1С языка
// {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
//////////////////////////////////////////////////////////////////////
struct CTemplateClass::paramdefs CTemplateClass::defFnNames[] = {
	{NULL,0,0}
};

enum {
	lastMethod
	};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemplateClass::CTemplateClass()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());	
}

CTemplateClass::~CTemplateClass()
{  
}

int  CTemplateClass::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{

	switch(iMethNum)
  {
    case 1:
      {       
        
      }
      break;
    default:
      {
      }
  };
  return 0;
}

int  CTemplateClass::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	switch(iMethNum)
  {
    case 1:
      {       
        
      }
      break;
    default:
      {
      }
  };
  return 0;
}


int  CTemplateClass::FindMethod(char const * lpMethodName)const
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

char const *  CTemplateClass::GetMethodName(int iMethodNum,int iMethodAlias)const
{
  return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CTemplateClass::GetNMethods(void)const
{
	return lastMethod;
}

int  CTemplateClass::HasRetVal(int iMethodNum)const
{
  return defFnNames[iMethodNum].HasReturnValue ;
}

int CTemplateClass::GetNParams(int iMethodNum)const
{
  return defFnNames[iMethodNum].NumberOfParams;
}

int  CTemplateClass::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	switch(iMethodNum)
  {
    case 1:
      {       
        
      }
      break;
    default:
      {
      }
  };
  return 0;
}



void  CTemplateClass::DecrRef(void)
{
	CBLContext::DecrRef();
}


char const *  CTemplateClass::GetCode(void)const
{
	return 0;
}

int  CTemplateClass::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CTemplateClass::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CTemplateClass::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CTemplateClass::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}




long  CTemplateClass::GetTypeID(void)const
{
	return 100;
}

char const *  CTemplateClass::GetTypeString(void)const
{
	return "CTemplateClass;
}

class CType   CTemplateClass::GetValueType(void)const
{
	CType tType(100);	
	return tType;
}


void  CTemplateClass::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CTemplateClass::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CTemplateClass::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CTemplateClass::IsExactValue(void)const
{
	return 0;
}

int  CTemplateClass::IsOleContext(void)const
{
	return 0;
}

int  CTemplateClass::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CTemplateClass::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CTemplateClass::IsSerializable(void)
{
	return 0;
}

int  CTemplateClass::SaveToString(class CString & csStr)
{	
	return 0;
}

void  CTemplateClass::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CTemplateClass::GetNProps(void)const
{
	return 0;
}

char const *  CTemplateClass::GetPropName(int A,int B)const
{	
	return NULL;
}

int  CTemplateClass::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return 0;
}

int  CTemplateClass::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return 0;
}

int  CTemplateClass::FindProp(char const * Name)const
{
	return 0;
}
