// MetaDataOfClasses.cpp: implementation of the CMetaDataOfClasses class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "MetaDataOfClasses.h"
#include "System\Compare.h"
#include "System\GlobalMethods.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMetaDataOfClasses::CMetaDataOfClasses() : m_bInsideDynamicAdd(false)
{
}

void RemoveNeedlessBlank(CString &str) //void CMetaDataOfClasses::RemoveNeedlessBlank(CString &str)
{ 
  str.TrimLeft(); str.TrimRight();
  int nStrLen = str.GetLength();  
  int nFindBlank = str.Find(' '); //(" ");
  int st = 0;
  for (int j = 0; j < nStrLen; j++)
  {
    int nCountBlank = 0;
    
    if (str[j] == '\"' && st == 0)
      st = 1;
    else if (str[j] == '\"' && st == 1)
      st = 0;

    if (str[j] == ' ' && st == 0)
    {
      for (int i = j + 1; i < nStrLen; i++)
      {
        if (str[i] == ' ')
        {
          nCountBlank++;          
        }
        else
          break;
      }
      if (nCountBlank > 0)
      {
        str.Delete(j + 1, nCountBlank);
      }
      nStrLen = str.GetLength();      
    }
  }
}

bool CMetaDataOfClasses::LookupUnLimMethod(const CString& strName, CParamDefs::CArrayOfMethodsParamData_PTR& param) const
{
	if (m_UnlimitsParams.Lookup(strName, param))
		return true;
	else
		return false;
}

bool CMetaDataOfClasses::ThisIsUnLimMethod(const CString& strName) const
{
	CParamDefs::CArrayOfMethodsParamData_PTR param;  
	if (m_UnlimitsParams.Lookup(strName, param))
		return true;
	else
		return false;
}

bool CMetaDataOfClasses::LookupMethHaveParVal(const CString& strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR& param) const
{
	if (m_ValParamInMeth.Lookup(strKeyOfUserClassMethod, param))
		return true;
	else
		return false;
}

// TODO баг из-за совместного использовани€ ресурса - баг возникает при сбросе оптимизации дл€ методов с неопределенным числом параметров !!
void CMetaDataOfClasses::AddMethHaveParVal(const CString& strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR param)
{
	m_ValParamInMeth[strKeyOfUserClassMethod] = param;
}

bool CMetaDataOfClasses::IsClassExist(const char *pcstrName) const
{
  CString strResult;
  return m_ClassBindFile.Lookup(pcstrName, strResult) != 0;
}

int CMetaDataOfClasses::GetCountClasses() const
{
  return m_ClassBindFile.GetCount();
}

int CMetaDataOfClasses::GetCountMethods() const
{
  CString key;
  POSITION pos;
  int res = 0;
  for (pos = m_MapClassNamesToMethodsMap.GetStartPosition(); pos != NULL;)
  {
    CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
    CString key;
    m_MapClassNamesToMethodsMap.GetNextAssoc( pos, key, pMapFunc);
    res += pMapFunc->GetCount();
    
  }  
  return res;
}

CString CMetaDataOfClasses::GetAllClassesNames(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_ClassBindFile.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_ClassBindFile.GetNextAssoc( pos, key, val);    
    res += key;
    res += " = ";
    res += val;
    res += sep;        
  }  
  return res;
}

CString CMetaDataOfClasses::GetAllAliasAndPath(const char* sep) const
{
  CString key;
  POSITION pos;
  CString res;  
  for (pos = m_AliasOfPaths.GetStartPosition(); pos != NULL;)
  {
    CString val;    
    m_AliasOfPaths.GetNextAssoc( pos, key, val);    
    res += key;
    res += " = ";
    res += val;
    res += sep;
  }  
  return res;
}

CString CMetaDataOfClasses::GetBaseClasses(const char* strNameCl, const char* sep) const
{
  CString str;
  CStringList_SharedPtr pList;
  m_ClassesHierarchyMap.Lookup(strNameCl, pList);
  if (pList)
  {
    //for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
    //{
      //str += pList->GetNext( posList );
    CUserClassMetaData::CStringVectorConstIterator iter_end = pList->end();
    for(CUserClassMetaData::CStringVectorIterator iter = pList->begin(); iter != iter_end; iter++)
    {
	  str += *iter;
      str += sep;
    }    
  }
  return str;
}

CString CMetaDataOfClasses::GetNamesMethOfClass(const char *pNameClass, const char *sep) const
{
  CString res;
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if (m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
    if (pMapFunc)
    {
      for (POSITION pos = pMapFunc->GetStartPosition(); pos != NULL;)
      {
        CParamDefs::CArrayOfMethodsParamData_PTR param; // = NULL;
        CString key;
        pMapFunc->GetNextAssoc( pos, key, param);
        res += key;
        res += sep;
      }
    }
    return res;
}

CParamDefs::CArrayOfMethodsParamData_PTR CMetaDataOfClasses::GetInfoParamMeth(const char *pNameClass, const char *pNameMeth) const
{  
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if (m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
    if (pMapFunc)
    {
      CParamDefs::CArrayOfMethodsParamData_PTR arr; // = NULL;
      if (pMapFunc->Lookup(pNameMeth, arr))
        return arr;      
    }
    return CParamDefs::CArrayOfMethodsParamData_PTR(); //NULL;
}

bool CMetaDataOfClasses::IsMethExist(const char *pNameClass, const char *pNameMeth) const
{
  CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc;
  if(m_MapClassNamesToMethodsMap.Lookup(pNameClass, pMapFunc))
  {
    CParamDefs::CArrayOfMethodsParamData_PTR arr; // = NULL;
    if (pMapFunc->Lookup(pNameMeth, arr))
      return true;
  }
  return false;
}

// чтобы название класса дл€ “ип«начени€—тр( ќѕ) писалось не как пользователь напишет в —оздатьќбъект(“ипќбъекта),
// а бралось из файла описани€ - artbear
// используетс€ в CComponentClass::InitObject - т.е. когда в 1— пишем —оздатьќбъект(“ипќбъекта)
//
CString CMetaDataOfClasses::GetClassNameFromDefinition(const CString& ClassName) const
{
	LPCSTR strResult;
	m_ClassBindFile.LookupKey(ClassName, strResult);
	
	return strResult;
}

CString CMetaDataOfClasses::GetPathFromAlias(const CString& strAls) const
{
  //return m_AliasOfPaths[strAls];
	CString Result;
	m_AliasOfPaths.Lookup(strAls, Result);
	return Result;
}

// artbear дл€ проверки рекурсии
// 1 - рекурси€, 0 - нет
bool CMetaDataOfClasses::VerifyRecursiveInBaseClasses(CString ClassName, CString BaseClassName, CString& sRecursiveHierarchy) const
{
//   CStringList* pList = (CStringList*)m_ClassesHierarchyMap[BaseClassName];
//   if (pList)
  CStringList_SharedPtr pList;
  if (m_ClassesHierarchyMap.Lookup(BaseClassName, pList) && pList)
  {
	sRecursiveHierarchy = BaseClassName+ " - "+sRecursiveHierarchy;
//     for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
//     {
// 		CString strBaseClassName = pList->GetNext( posList );
	CUserClassMetaData::CStringVectorConstIterator iter_end = pList->end();
	for(CUserClassMetaData::CStringVectorIterator iter = pList->begin(); iter != iter_end; iter++)
	{
		CString strBaseClassName = *iter;

		//if (!ClassName.CompareNoCase(strBaseClassName))
		if (!FastCompareNoCase.Compare(ClassName, strBaseClassName))
		{
			sRecursiveHierarchy = strBaseClassName + " - "+sRecursiveHierarchy;
			return true;
		}

		bool bHaveRecursive = VerifyRecursiveInBaseClasses(ClassName, strBaseClassName, sRecursiveHierarchy);	

		if (bHaveRecursive)
			return true;

    }
  }
  return false;
}

CString CMetaDataOfClasses::GetNameOfFile(const CString &strNameOfMod) const
{
	CString Result;
	m_ClassBindFile.Lookup(strNameOfMod, Result); //return m_ClassBindFile[strNameOfMod];
	return Result;
}

bool CMetaDataOfClasses::TheClassIsDerive(const CString& derive, const CString& base) const
{
  //CStringList *pList = (CStringList*)(const_cast<CIStringMap<CStringList*, CStringList*>*>(&m_Meta.m_ClassesHierarchyMap)->operator[](derive));
  CStringList_SharedPtr pList;
  if (m_ClassesHierarchyMap.Lookup(derive, pList) && pList)
  {
    //CString str;
//     for(POSITION posList = pStrList->GetHeadPosition(); posList != NULL; )
//     {
//		if (IsObject(pStrList->GetNext( posList ), base))
	  CUserClassMetaData::CStringVectorConstIterator iter_end = pList->end();
	  for(CUserClassMetaData::CStringVectorIterator iter = pList->begin(); iter != iter_end; iter++)
	  {
		  if (IsObject(*iter, base))
			return true;
    }
    
  }
  return false;
}

bool CMetaDataOfClasses::IsObject(const CString& sClassName, const CString& sTryName) const
{
	//if (!sClassName.CompareNoCase(sTryName))
	if (!FastCompareNoCase.Compare(sClassName, sTryName))
		return true;
	else
		return TheClassIsDerive(sClassName, sTryName);
}  

bool CMetaDataOfClasses::IsFunction( const CString& strKeyOfUserClassMethod) const
{
	CString temp;
	return m_IsFunctionMap.Lookup(strKeyOfUserClassMethod, temp) ? true : false;
}