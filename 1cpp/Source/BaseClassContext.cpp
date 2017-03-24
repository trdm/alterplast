// BaseClassContext.cpp: implementation of the CBaseClassContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "BaseClassContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CMapStringToPtr CBaseClassContext::map;

CBaseClassContext::CBaseClassContext(CBLContext* _pCont)
{
  SetContext(_pCont);
}

CBaseClassContext::~CBaseClassContext()
{

}

/*void CBaseClassContext::SetContext(CBLContext *pNewCont)
{
  pCont = pNewCont;
  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
  CString strName = pNewCont->GetTypeString();
  if (!pHierarchy->IsClassExist(strName))
  {
    is1CBaseClass = true;
    if (!map.Lookup(strName, (void*&)defFnNames))
    {
      defFnNames = new CParamDefs;
      map[strName] = defFnNames;
      int nMeth = pNewCont->GetNMethods();
      defFnNames->SetPoolSize(nMeth + 1, 0);
      for (int i = 0; i < nMeth; i++)
      {                
        defFnNames->AddParam(pNewCont->GetMethodName(i, 0),pNewCont->GetMethodName(i, 1), 
          i, pCont->HasRetVal(i), pNewCont->GetNParams(i));
      }      
    }
  }

}*/
