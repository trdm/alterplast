// MapDefMethod.h: interface for the CMapDefMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)
#define AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <functional>
#include "paramdefs.h"
#include "istrmap.h"

#include "BLModuleWrapper.h"

//#include "System\SmartPointers.h"
#include "System\sh_ptr.hpp"

// class CComponentClassModule : public CBaseRefCountObject, public CBLModuleWrapper
// {
// public:
// 	CComponentClassModule(const CString& strNameOfComponent, class CBLContext * pContext, char const *pStr, const char* pStrNameFile = NULL, bool bEnableThrow = false):
// 		CBLModuleWrapper(strNameOfComponent, pContext, pStr, pStrNameFile, bEnableThrow) 
// 		{}
// };

// typedef CRefCountPtr<CComponentClassModule> CComponentClassModulePtr;
// // CComponentClassModulePtr ptr = new CComponentClassModule("", NULL, NULL);
typedef sh_ptr<CBLModuleWrapper> CComponentClassModulePtr;

// class CParamDefsWithRefCount : public CBaseRefCountObject, public CParamDefs
// { };
//typedef CRefCountPtr<CParamDefsWithRefCount> CParamDefsWithRefCountPtr;
typedef sh_ptr<CParamDefs> CParamDefsWithRefCountPtr;

class CMapDefMethod  
{
public:
  CMapDefMethod() {}

  ~CMapDefMethod() 
  {
    Clear();
  }
  
  void Clear()
	{ _Clear(true);  }

  // не вызываем delete !!
  void ClearWithoutDelete() // artbear
	{ _Clear(false);  }

  //void AddModule(const char *compName, CBLModule* pMod, CParamDefs *pDefs)
  void AddModule(const char *compName, CComponentClassModulePtr pMod, CParamDefsWithRefCountPtr pDefs)
  {
    //if(pMod != NULL)
    if(pMod.isValid())
      mapModule[compName] = pMod;
    mapParam[compName] = pDefs;
  }

  //CBLModule* GetModule(const char *nameMod)
  CComponentClassModulePtr GetModule(const char *nameMod)
  {
    CComponentClassModulePtr pM(NULL);
    mapModule.Lookup(nameMod, pM);
    return pM;
  }
  
  void DelModule(const char *nameMod)
  {    
    mapModule.RemoveKey(nameMod);    
  }

  void DelDefParam(const char *nameComp)
  {    
    mapParam.RemoveKey(nameComp);
  }

  //CParamDefs* GetDefs(const char *nameComp)
  CParamDefsWithRefCountPtr GetDefs(const char *nameComp)
  {
    //CParamDefs* pPD = NULL;
    CParamDefsWithRefCountPtr pPD(NULL);
    mapParam.Lookup(nameComp, pPD);
    return pPD;
  }
private:
  //CIStringMap<CParamDefs*, CParamDefs*> mapParam;
  CIStringMap<CParamDefsWithRefCountPtr, CParamDefsWithRefCountPtr&> mapParam;

  //CIStringMap<CBLModule*, CBLModule*> mapModule;
  CIStringMap<CComponentClassModulePtr, CComponentClassModulePtr&> mapModule;
  
  // не вызываем delete !!
  void _Clear(bool bWithDelete = true) // artbear
  {
//     POSITION pos = mapModule.GetStartPosition();
//     while (pos != NULL)
//     {
//       CString str;
//       //CBLModule* pMod = NULL;
//       CComponentClassModulePtr pMod = NULL;
//       mapModule.GetNextAssoc(pos,str,pMod);
// 	  if (pMod)
// 		pMod->Unload(); // TODO artbear
// 
//       //LogErr("CMapDefMethod  :: pMod->Unload();  ласс <%s>", str);
// 
// 	  //if (bWithDelete) // artbear
// 		//	delete pMod;
//     }
    mapModule.RemoveAll();

//     pos = mapParam.GetStartPosition();
//     while (pos != NULL)
//     {
//       CString str;
//       //CParamDefs* pParDef = NULL;
//       CParamDefsWithRefCountPtr pParDef = NULL;
//       mapParam.GetNextAssoc(pos,str,pParDef);
// 	  
// //	  if (bWithDelete) // artbear
// //		delete pParDef;  // artbear  
// 
//       //LogErr("CMapDefMethod  :: удал€ю mapParam;  ласс <%s>", str);
//     }
    mapParam.RemoveAll();
  }
};

#endif // !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)





/*
// MapDefMethod.h: interface for the CMapDefMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)
#define AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

//#include <functional>

#include <map>
#include "paramdefs.h"

class CMapDefMethod  
{
  struct lessCString : binary_function<CString, CString, bool> {
    bool operator()(const CString& _X, const CString& _Y) const
    {return (_X < _Y); }
  };
  typedef map<CString, CParamDefs*, lessCString>::iterator ITER_PARAM;
  typedef map<CString, CBLModule7*, lessCString>::iterator ITER_MODULE;
  
  public:
    CMapDefMethod() {}
    virtual ~CMapDefMethod() 
    {
      for (ITER_MODULE Iter1 = mapModule.begin(); Iter1 != mapModule.end(); Iter1++) {
        CBLModule7* pMod = (*Iter1).second;
        if (pMod != NULL) {/*
                           int n =  pMod->GetNStaticVars();
                           for (int i = 0; i < n; i++) {
                           CValue val;
                           //pMod->SetStaticVarValue(i,val,0);	//138
                           pMod->GetStaticVarValue(i, val,0);
                           val = "";
        }*//*
          pMod->Unload();
          //delete pMod;   
        }
      }
      for (ITER_PARAM Iter2 = mapParam.begin(); Iter2 != mapParam.end(); Iter2++)
        delete (*Iter2).second;
    }
    
    void AddModule(const char *nameMod, CBLModule7* pMod)
    {
      if(pMod != NULL)
        mapModule[nameMod] = pMod;
    }
    
    CBLModule7* GetModule(const char *nameMod)
    {
      if(!mapModule.empty()) {
        ITER_MODULE Iter = mapModule.find(nameMod);
        if (Iter != mapModule.end())
          return (*Iter).second;
      }
      return NULL;
    }
    
    void DelModule(const char *nameMod)
    {
      if(!mapModule.empty()) {
        mapModule.erase(nameMod);
      }
    }
    
    void AddDefParam(const char *nameComp, CParamDefs *pDefs)
    {
      mapParam[nameComp] = pDefs;
    }
    void DelDefParam(const char *nameComp)
    {
      if(!mapParam.empty()) {
        mapParam.erase(nameComp);
      }
    }
    CParamDefs* GetDefs(const char *nameComp)
    {
      if(!mapParam.empty()) {
        ITER_PARAM Iter = mapParam.find(nameComp);
        if (Iter != mapParam.end())
          return (*Iter).second;
      }
      return NULL;
    }
  private:
    map<CString, CParamDefs*, lessCString > mapParam;
    map<CString, CBLModule7*, lessCString > mapModule;
};

#endif // !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)
*/