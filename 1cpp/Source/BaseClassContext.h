// BaseClassContext.h: interface for the CBaseClassContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASECLASSCONTEXT_H__0422FF6E_AD4A_4970_902D_FF53C4F923DF__INCLUDED_)
#define AFX_BASECLASSCONTEXT_H__0422FF6E_AD4A_4970_902D_FF53C4F923DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseClassContext {

public:
//  inline bool Is1C_BaseContext() const {return is1CBaseClass;}
	//inline void SetContext(CBLContext *pNewCont);
  inline void SetContext(CBLContext *pNewCont) {pCont = pNewCont;}
  inline int FastFindMeth(const char* ch) const {return pCont->FindMethod(ch);}//{return defFnNames->GetIndexByName(ch);}
	
  CBaseClassContext(CBLContext* _pCont);
	~CBaseClassContext();

  inline CBLContext* GetInnerCont() const
  {
    return pCont;
  }
  inline CBLContext* operator->() {return pCont;}
  
  
private:
  CBLContext *pCont;

  //bool is1CBaseClass;

  //CParamDefs *defFnNames;
  
  //static CMapStringToPtr map;
};

#endif // !defined(AFX_BASECLASSCONTEXT_H__0422FF6E_AD4A_4970_902D_FF53C4F923DF__INCLUDED_)
