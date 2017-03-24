// HashMap.cpp: implementation of the CHashMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "HashMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CHashMap, CBLContext);

class CObjID CHashMap::ObjID;


//////////////////////////////////////////////////////////////////////
// Описание методов и функций класса для 1С языка
// {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
//////////////////////////////////////////////////////////////////////
struct CHashMap::paramdefs CHashMap::defFnNames[] = {
  {"IsEmpty","Пустой",1,0},
  {"AddValue","ДобавитьЗначение",1,2},
  {"Get","Получить",1,1},
  {"GetSize","Размер",1,0},
  {"Reserve","Резервировать",0,1},
  {"RemoveAll","УдалитьВсе",0,0},  
  {"RemoveValue","УдалитьЗначение",1,1}, 
  {"SelectValue","ВыбратьЗначения",1,1}, 
  {"GetValue","ПолучитьЗначение",1,2}, 
	{NULL,NULL,0,0}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHashMap::CHashMap()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());	
  m_pos = NULL;
}

CHashMap::~CHashMap()
{  
  POSITION pos;
  CValue *pVal = NULL;
  CString key;
  

  for( pos = map.GetStartPosition(); pos != NULL; )
   {     
     map.GetNextAssoc( pos, key, (void*&)pVal );
     
     if (pos != NULL)        
     {
       delete pVal;            
     }
   }
  map.RemoveAll();
}

int  CHashMap::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
  switch(iMethNum)
  {
    case funcIsEmpty:
    { 
      rValue =  CNumeric(map.IsEmpty() ? 1 : 0);
      return 1;
    }
    break;
    case funcAddValue:
    { 
      CValue *pVal = NULL;
      CValue *pValDel = NULL;
      CString strKey = ppValue[1]->GetString();
      
      if (map.Lookup(strKey, (void*&)pValDel))
      {
        map.RemoveKey(strKey);               
        pBkEndUI->DoMessageLine("map.Lookup", mmBlueTriangle);        
      }
      
      pVal = new CValue(*ppValue[0]);           
      map[strKey] = pVal;                   
      return 1;
    }
    break;
    case funcGetAt:
    {
      CValue *pVal = NULL;
      if (map.Lookup(ppValue[0]->GetString(), (void*&)pVal))
      {
        rValue = *pVal;
      }      
    }
    break;
    case funcGetSize:
    { 
      rValue = map.GetCount();
      return 1;
    }
    break;
    case funcRemoveValue:
    { 
      rValue = map.RemoveKey(ppValue[0]->GetString());
      return 1;
    }
    break;
    case funcSelectValue:
    { 
      if (map.IsEmpty())
      {
        rValue = 0L;
      }
      else
      {
        m_pos = NULL;
        m_pos = map.GetStartPosition();
        rValue = 1L;
      }
      return 1;
    }
    break;
    case funcGetValue:
    { 
      if (m_pos == NULL)
      {
        rValue = 0L;
      }
      else
      {
        CValue *pVal = NULL;
        CString strKey;
        map.GetNextAssoc( m_pos, strKey, (void*&)pVal);
        if (m_pos != 0)
        {
          *(ppValue[0]) = *pVal;
          *(ppValue[1]) = strKey;
          rValue = 1L;
        }
        else
          rValue = 0L;
      }      
      return 1;
    }
    break;
  default:
    {     
    }
  };
  return -1;
}

int  CHashMap::CallAsProc(int iMethNum,class CValue * * ppValue)
{
  switch(iMethNum)
  {
    case procReserve:
    { 
      map.InitHashTable(ppValue[0]->GetNumeric());
      return 1;
    }
    break;    
    case procRemoveAll:
    { 
      map.RemoveAll();
      return 1;
    }
  default:
    {     
    }
  };
  
  return -1;
}


int  CHashMap::FindMethod(char const * lpMethodName)const
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

char const *  CHashMap::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (iMethodNum >= lastMethod) 
		return "mError";
	else
		return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CHashMap::GetNMethods(void)const
{
	return lastMethod;
}

int  CHashMap::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CHashMap::GetNParams(int iMethodNum)const
{	
  return defFnNames[iMethodNum].NumberOfParams;
}

int  CHashMap::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	return 0;
}



void  CHashMap::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  CHashMap::GetCode(void)const
{
	return 0;
}

int  CHashMap::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CHashMap::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CHashMap::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CHashMap::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}




long  CHashMap::GetTypeID(void)const
{
	return 100;
}

char const *  CHashMap::GetTypeString(void)const
{
	return "HashMap";
}

class CType   CHashMap::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  CHashMap::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CHashMap::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CHashMap::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CHashMap::IsExactValue(void)const
{
	return 0;
}

int  CHashMap::IsOleContext(void)const
{
	return 0;
}

int  CHashMap::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CHashMap::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CHashMap::IsSerializable(void)
{
	return 0;
}

int  CHashMap::SaveToString(class CString & csStr)
{
	csStr = "DJK";
	return 1;
}

void  CHashMap::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CHashMap::GetNProps(void)const
{
	return 0;
}

char const *  CHashMap::GetPropName(int A,int B)const
{
	return NULL;
}

int  CHashMap::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return -1;
}

int  CHashMap::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return -1;
}

int  CHashMap::FindProp(char const * Name)const
{
	return -1;
}
