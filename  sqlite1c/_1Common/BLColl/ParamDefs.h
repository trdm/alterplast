// ParamDefs.h: interface for the CParamDefs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMDEFS_H__3B7722E4_086E_4439_A697_F0DF8129DC51__INCLUDED_)
#define AFX_PARAMDEFS_H__3B7722E4_086E_4439_A697_F0DF8129DC51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <algorithm>
#include "istrmap.h"

//#include <vector>
//#include <afxtempl.h>

/*struct NameBindOfType {
    CString name;
    CString type;
    CString value;
    bool bIsVal;
    NameBindOfType (const CString& n, const CString& t, const CString& v = CString(""), bool bIs = false) : name(n), type(t), value(v), bIsVal(bIs) {}
};*/

class CParamDefs : public CObject  
{
public:
  //struct NameBindOfType *p;
  typedef CArray<struct NameBindOfType*, struct NameBindOfType*> ARR_TYPE_NAME;  

  // позиция метода свободной записи свойств
  int nPosOnWriteFreeProp;
  // позиция метода свободного чтения свойств
  int nPosOnReadFreeProp;

  //позиция конструктора класса, -1 - нет конструктора
  int nPosConstructor;
  //позиция деструктора класса,  -1 - нет деструктора
  int nPosDestructor;

  //Здесь могут храниться имена свободных свойств и их индексы
  CMapStringToPtr mapNamesFreeProp;
  
  CMapStringToOb* pMapFunc;

  struct paramdefs {
		CString Names[2];
		int HasReturnValue;
		int NumberOfParams;
    int nProcPos;
    bool isUnLimParMethod; // имеет или нет метод неявные параметры
    bool bMethHaveVal;     // имеет или нет метод параметры передаваемые по значению
    ARR_TYPE_NAME* pDefParam;    

    inline void Init(const char *e_name, const char *r_name, int procPos, int hrv, int num, bool isUP, 
      bool bHaveVal, ARR_TYPE_NAME* pDefPar = NULL)
    {
      Names[0] = e_name;
      Names[1] = r_name;      
      
      HasReturnValue = hrv;
      NumberOfParams = num;
      isUnLimParMethod = isUP;
      bMethHaveVal = bHaveVal;
      nProcPos  = procPos;
      pDefParam = pDefPar;      
    }
    inline void Clear()
    {
      Names[0].Empty();
      Names[1].Empty();      

      HasReturnValue = 0;
      NumberOfParams = 0;
      nProcPos = -1;
      isUnLimParMethod = false;
      bMethHaveVal = false;
      pDefParam = NULL;
    }
    paramdefs () 
    {      
      HasReturnValue = 0;
      NumberOfParams = 0;
      nProcPos = -1;
      isUnLimParMethod = false;
      bMethHaveVal = false;
      pDefParam = NULL;
    }
    paramdefs (const char *e_name, const char *r_name, int procPos=-1, int hrv=0, int num=0, 
               bool isUP = false, bool bHaveVal = false, ARR_TYPE_NAME* pDefPar = NULL)
    {      
      Init(e_name, r_name,procPos, hrv,num,isUP, bHaveVal, pDefPar);
    }
    ~paramdefs()
    {
      Clear();
    }
    paramdefs (const paramdefs& par)
    {     
      Init(par.Names[0], par.Names[1], par.nProcPos ,par.HasReturnValue, par.NumberOfParams, par.isUnLimParMethod, par.bMethHaveVal, par.pDefParam);
    }
    inline paramdefs& operator = (const paramdefs& par)
    {
      Clear();
      Init(par.Names[0], par.Names[1], par.nProcPos, par.HasReturnValue, par.NumberOfParams, par.isUnLimParMethod, par.bMethHaveVal, par.pDefParam);
      return *this;
    }
    inline operator==(const paramdefs& p)
    {     
      return p.Names[0].CompareNoCase(Names[0]) == 0 && p.Names[1].CompareNoCase(Names[1]) == 0;
    }

  };

  CParamDefs() : nPosOnReadFreeProp(-1), nPosOnWriteFreeProp(-1), nPosConstructor(-1), nPosDestructor(-1), pMapFunc(NULL) {}
  ~CParamDefs(){}

  inline bool AddParam(const char *eng_descr, const char *rus_descr, int procPos, int is_func = 0, 
                       int nNumParam = 0, bool isUP = false, bool bHaveVal = false)
  {
    ARR_TYPE_NAME* pDefParam = NULL;
    if (pMapFunc)
      pMapFunc->Lookup(rus_descr, (CObject*&)pDefParam);

    paramdefs par(eng_descr, rus_descr, procPos,is_func, nNumParam, isUP, bHaveVal); 

    if (find(vec_p_def.begin(), vec_p_def.end(), par) == vec_p_def.end())
    {
      vec_p_def.push_back(par);
      int pos = vec_p_def.size() - 1;      
      if (eng_descr)
        mapIndexes[eng_descr] = pos;
      if (rus_descr)
        mapIndexes[rus_descr] = pos;
      
      return true;
    }
    else
      return false;
    }
  inline bool RemoveParam(const char *eng_descr, const char *rus_descr)
  {
    vector<paramdefs>::iterator Iter;
    paramdefs par(eng_descr, rus_descr); 

    Iter = find(vec_p_def.begin(), vec_p_def.end(), par);

    if (Iter != vec_p_def.end())
    {
      if (eng_descr)
        mapIndexes.RemoveKey(eng_descr);
      if (rus_descr)
        mapIndexes.RemoveKey(rus_descr);

      vec_p_def.erase(Iter);      
      return true;
    }
    else
      return false;
  }
  inline int Size() const
  {
    return vec_p_def.size();
  }
  inline bool Empty() const
  {
    return vec_p_def.empty();
  }
  inline const paramdefs& operator [](int index) const
  {
    return vec_p_def[index];
  }
  inline void SetPoolSize(int nContMeth, int nContVars)
  {    
    if (nContMeth > 0)
    {
      vec_p_def.reserve(nContMeth);    
      mapIndexes.InitHashTable(static_cast<unsigned int>(nContMeth*1.5));
    }
    if (nContVars > 0)
      mapIndOfProperty.InitHashTable(nContVars);
  }
  inline int GetIndexByName(const char* lpMethodName) const
  {
    int res = -1;
    
    if (mapIndexes.Lookup(lpMethodName, res))    
      return res;
    
    return -1;
  }
  inline int GetIndexPropetry(const CString& strNameProp, CBLModule *pMod, bool isSet)
  {
    CString strNamePropRus = isSet ? "ПриЗаписи_" : "ПриПолучении_";
    CString strNamePropEng = isSet ? "OnWrite_"   : "OnGet_";
    
    strNamePropRus += strNameProp;    
    int iProc = -1;

    if (mapIndOfProperty.Lookup(strNamePropRus,iProc))
      return iProc;
    else if (mapIndOfProperty.Lookup(strNamePropEng,iProc))
      return iProc;
    else
    {
      iProc = pMod->FindProc(strNamePropRus, 0);
      if (iProc == -1)
      {      
        strNamePropEng += strNameProp;
        iProc = pMod->FindProc(strNamePropEng, 1);
        if (iProc == -1)
          mapIndOfProperty[strNamePropRus] = iProc;
        else
          mapIndOfProperty[strNamePropEng] = iProc;        
      }    
      else      
        mapIndOfProperty[strNamePropRus] = iProc;
              
      return iProc;
    }
  }
private:
  vector<paramdefs> vec_p_def;
  typedef vector<paramdefs>::const_iterator CONST_ITER;
  CIStringMap<int, int> mapIndexes;  
  CIStringMap<int, int> mapIndOfProperty;  

};

#endif // !defined(AFX_PARAMDEFS_H__3B7722E4_086E_4439_A697_F0DF8129DC51__INCLUDED_)
