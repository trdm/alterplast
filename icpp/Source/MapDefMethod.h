// MapDefMethod.h: interface for the CMapDefMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)
#define AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "paramdefs.h"
#include "istrmap.h"

#include "BLModuleWrapper.h"

struct CMyParamDefs : public CParamDefs
{

	CMyParamDefs() : nPosOnReadFreeProp(-1), nPosOnWriteFreeProp(-1), nPosConstructor(-1), nPosDestructor(-1),
		nPosOfIsSerializableMethod(-1), nPosOfSaveToStringMethod(-1), nPosOfLoadFromStringMethod(-1)
		, m_nPosOfldrInit(-1), m_nPosOfldrDone(-1)
		, m_nPosOfldrAddColumn(-1), m_nPosOfldrAddRow(-1), m_nPosOfGetLastError(-1)
	{}

	// позиция метода свободной записи свойств
	int nPosOnWriteFreeProp;
	// позиция метода свободного чтения свойств
	int nPosOnReadFreeProp;

	bool IsDefineFreePropFunctions() const { return nPosOnReadFreeProp != -1 && nPosOnWriteFreeProp != -1; };

	//позиция конструктора класса, -1 - нет конструктора
	int nPosConstructor;
	//позиция деструктора класса,  -1 - нет деструктора
	int nPosDestructor;

	int nPosOfIsSerializableMethod;
	int nPosOfSaveToStringMethod;
	int nPosOfLoadFromStringMethod;
	bool IsDefineSerializableFunctions() const { return nPosOfIsSerializableMethod != -1 && nPosOfSaveToStringMethod != -1 && nPosOfLoadFromStringMethod != -1; };

	int m_nPosOfldrInit;
	int m_nPosOfldrDone;
	int m_nPosOfldrAddColumn;
	int m_nPosOfldrAddRow;
	int m_nPosOfGetLastError;
	bool IsDefineLoaderFunctions() const { return (-1 != m_nPosOfldrInit) && (-1 != m_nPosOfldrDone) && 
		(-1 != m_nPosOfldrAddColumn) && (-1 != m_nPosOfldrAddRow) && (-1 != m_nPosOfGetLastError); };
};

typedef boost::shared_ptr<CMyParamDefs> CMyParamDefsWithRefCountPtr; //typedef boost::shared_ptr<CParamDefs> CParamDefsWithRefCountPtr;

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

  void AddModule(const char *compName, CComponentClassModulePtr pMod, CMyParamDefsWithRefCountPtr pDefs)
  {
    //if(pMod.isValid())
    if(pMod)
      mapModule[compName] = pMod;
    mapParam[compName] = pDefs;
  }

  CComponentClassModulePtr GetModule(const char *nameMod)
  {
    //CComponentClassModulePtr pM(NULL);
	CComponentClassModulePtr pM;
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

  CMyParamDefsWithRefCountPtr GetDefs(const char *nameComp)
  {
    CMyParamDefsWithRefCountPtr pPD;
    mapParam.Lookup(nameComp, pPD);
    return pPD;
  }
private:
  CIStringMap<CMyParamDefsWithRefCountPtr, CMyParamDefsWithRefCountPtr&> mapParam;

  CIStringMap<CComponentClassModulePtr, CComponentClassModulePtr&> mapModule;
  
  // не вызываем delete !!
  void _Clear(bool bWithDelete = true) // artbear
  {
    mapModule.RemoveAll();
    mapParam.RemoveAll();
  }
};

#endif // !defined(AFX_MAPDEFMETHOD_H__B5622B62_E78E_46AB_84C5_8104E43C1C43__INCLUDED_)