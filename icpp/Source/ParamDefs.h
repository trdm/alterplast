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
#include "compare.h"
#include "SimpleVector.h"
#include <boost/utility.hpp>

class CMethodsParamData {
public:
	typedef boost::shared_ptr<CMethodsParamData> Ptr;
	typedef CSimpleVector<Ptr> Vector;  
	typedef boost::shared_ptr<Vector> VectorPTR;

	static Ptr Create(const CString& n, const CString& t, const CString& v = CString(""), bool bIs = false)
	{ return Ptr(new CMethodsParamData (n, t, v, bIs)); };

	const CString& GetName() const { return name; };
	const CString& GetType() const { return type; };
	const CString& GetValue() const { return value; };

	bool IsByRef() const { return !bIsVal; };
	bool IsByVal() const { return bIsVal; };

	int CompareWithType(LPCTSTR szCompareString) const { return GetType().CompareNoCase(szCompareString); };

private:
	CMethodsParamData (const CString& n, const CString& t, const CString& v, bool bIs) : name(n), type(t), value(v), bIsVal(bIs) {}

	CString name;
	CString type;
	CString value;
	bool bIsVal;
};

// Увязывает указатели на экземпляры классов с указателями на список значений парметров
// полученных вместо точек в методе (...)
class InfoUnlimitsParams : boost::noncopyable
{
public:
	typedef boost::shared_ptr<InfoUnlimitsParams> CPtr;

	static CPtr Create() { return CPtr(new InfoUnlimitsParams); };
	~InfoUnlimitsParams()
	{
		int i  = 1;
	};

	void Setup(int nRes, int nNParams);
	void RemoveAllFromValueList();
	void AddValueIntoValueList(CValue& param1, CValue& param2);

	typedef boost::shared_ptr<CValueListContextFriend> CListContextPtr;
	CListContextPtr GetValueListContext() { return m_pValueListContext; };

	int GetCountOfAllParams() const { return m_nNumAllParams; };
	int GetCountOfExplicitParams() const { return m_nNumExplicitParams; };

	std::vector<CValue*> vecPointOfPars;

private:
	InfoUnlimitsParams(): m_pValueListContext(new CValueListContextFriend())
	{
		int i = 1;
	};

	CListContextPtr m_pValueListContext;
	int         m_nNumAllParams;
	int         m_nNumExplicitParams;
};

typedef CIStringMap<InfoUnlimitsParams::CPtr, InfoUnlimitsParams::CPtr&> CMapOfInfoUnlimitsParamsPtr;

class CMethodsData { // класс можно копировать
	CMethodsParamData::VectorPTR m_pParamsByValueVector;
	CMethodsParamData::VectorPTR m_pUnlimitsParamsVector;
	InfoUnlimitsParams::CPtr m_pUnlimitsParamsInfo;
public:
	typedef boost::shared_ptr<CMethodsData> CPtr;

	CMethodsData() {};
	~CMethodsData() {};

	CMethodsParamData::VectorPTR ParamsByValueVector() const { return m_pParamsByValueVector; }
	void ParamsByValueVector(CMethodsParamData::VectorPTR val) { m_pParamsByValueVector = val; }

	CMethodsParamData::VectorPTR UnlimitsParamsVector() const { return m_pUnlimitsParamsVector; }
	void UnlimitsParamsVector(CMethodsParamData::VectorPTR val) { m_pUnlimitsParamsVector = val; }

	InfoUnlimitsParams::CPtr UnlimitsParamsInfo() const { return m_pUnlimitsParamsInfo; }
	void UnlimitsParamsInfo(InfoUnlimitsParams::CPtr val) { m_pUnlimitsParamsInfo = val; }
};

class CMethodsDataManager {
	//typedef CIStringMap<CMethodsData::CPtr, CMethodsData::CPtr&> Map;
	//Map m_map;
	class CMethodsDataManagerImpl;
	boost::scoped_ptr<CMethodsDataManagerImpl> m_pImpl;
public:

	CMethodsDataManager();
	~CMethodsDataManager();

	bool Lookup(const CString& strKeyOfUserClassMethod, CMethodsData::CPtr& ptr);

	void Set(const CString& strKeyOfUserClassMethod, CMethodsData::CPtr& ptr);
	void SetParamsByValueVector(const CString& strKeyOfUserClassMethod, CMethodsParamData::VectorPTR& ptr);
	void SetUnlimitsParamsVector(const CString& strKeyOfUserClassMethod, CMethodsParamData::VectorPTR& ptr);
	void SetUnlimitsParamsInfo(const CString& strKeyOfUserClassMethod, InfoUnlimitsParams::CPtr& ptr);

	CMethodsData::CPtr Remove(const CString& strKeyOfUserClassMethod);
};

//class CParamDefs : public CObject  
class CParamDefs
{
private:
	//typedef CArray<CMethodsParamData::Ptr, CMethodsParamData::Ptr&> CArrayOfMethodsParamData;  
	typedef CMethodsParamData::Vector CArrayOfMethodsParamData;  
public:
	typedef CMethodsParamData::VectorPTR CArrayOfMethodsParamData_PTR;
	static CArrayOfMethodsParamData_PTR Create_ArrayOfMethodsParamData() { return CArrayOfMethodsParamData_PTR(new CArrayOfMethodsParamData); };

  //// позиция метода свободной записи свойств
  //int nPosOnWriteFreeProp;
  //// позиция метода свободного чтения свойств
  //int nPosOnReadFreeProp;

  //bool IsDefineFreePropFunctions() const { return nPosOnReadFreeProp != -1 && nPosOnWriteFreeProp != -1; };

  ////позиция конструктора класса, -1 - нет конструктора
  //int nPosConstructor;
  ////позиция деструктора класса,  -1 - нет деструктора
  //int nPosDestructor;

  //int nPosOfIsSerializableMethod;
  //int nPosOfSaveToStringMethod;
  //int nPosOfLoadFromStringMethod;
  //bool IsDefineSerializableFunctions() const { return nPosOfIsSerializableMethod != -1 && nPosOfSaveToStringMethod != -1 && nPosOfLoadFromStringMethod != -1; };

  typedef CIStringMap<CArrayOfMethodsParamData_PTR, CArrayOfMethodsParamData_PTR&> CMapMethodsNameToArrayOfMethodsParamData;
  typedef boost::shared_ptr<CMapMethodsNameToArrayOfMethodsParamData> CPtrOfMapMethodsNameToArrayOfMethodsParamData;
private:  
	CPtrOfMapMethodsNameToArrayOfMethodsParamData m_pMapFunc; //CMapStringToOb* m_pMapFunc;
public:
	const CPtrOfMapMethodsNameToArrayOfMethodsParamData& GetPtrOfMapMethodsNameToArrayOfMethodsParamData() const { return m_pMapFunc; };
	CPtrOfMapMethodsNameToArrayOfMethodsParamData& GetPtrOfMapMethodsNameToArrayOfMethodsParamData() { return m_pMapFunc; }; // TODO для этой функций нужен рефакторинг
	// 	void pMapFunc_Set(const CPtrOfMapMethodsNameToArrayOfMethodsParamData& pNew) { m_pMapFunc = pNew; };
	void CleanPtrOfMapMethodsNameToArrayOfMethodsParamData() { m_pMapFunc.reset(); };
  
  struct paramdefs { // класс можно копировать
		CString Names[2];
		int HasReturnValue;
		int NumberOfParams;
    int nProcPos;
    bool m_bIsMethodHaveUnlimitedCountOfParams; // имеет или нет метод неявные параметры
    bool m_bIsMethodHaveParamsReceivedByValue;     // имеет или нет метод параметры передаваемые по значению
	BOOL m_bHaveDefinitionsDiff;
    CArrayOfMethodsParamData_PTR m_pArrayOfMethodsParamData;    

  private:
    inline void Init(const char *e_name, const char *r_name, int procPos, int hrv, int num, bool isUP, 
      bool bHaveVal, BOOL bHaveDefinitionsDiff, CArrayOfMethodsParamData_PTR pDefPar = CArrayOfMethodsParamData_PTR())
    {
      Names[0] = e_name;
      Names[1] = r_name;      
      
      HasReturnValue = hrv;
      NumberOfParams = num;
      m_bIsMethodHaveUnlimitedCountOfParams = isUP;
      m_bIsMethodHaveParamsReceivedByValue = bHaveVal;
	  m_bHaveDefinitionsDiff = bHaveDefinitionsDiff;
      nProcPos  = procPos;
      m_pArrayOfMethodsParamData = pDefPar;      
    }

    inline void Clear()
    {
      Names[0].Empty();
      Names[1].Empty();      

      HasReturnValue = 0;
      NumberOfParams = 0;
      nProcPos = -1;
      m_bIsMethodHaveUnlimitedCountOfParams = false;
      m_bIsMethodHaveParamsReceivedByValue = false;
	  m_bHaveDefinitionsDiff = FALSE;
      m_pArrayOfMethodsParamData.reset();
    }
  public:
    paramdefs () 
    {      
      HasReturnValue = 0;
      NumberOfParams = 0;
      nProcPos = -1;
      m_bIsMethodHaveUnlimitedCountOfParams = false;
      m_bIsMethodHaveParamsReceivedByValue = false;
	  m_bHaveDefinitionsDiff = FALSE;
      //m_pArrayOfMethodsParamData = NULL;
    }
    paramdefs (const char *e_name, const char *r_name, int procPos=-1, int hrv=0, int num=0, 
               bool isUP = false, bool bHaveVal = false, BOOL bHaveDefinitionsDiff = FALSE, CArrayOfMethodsParamData_PTR pDefPar = CArrayOfMethodsParamData_PTR())
    {      
      Init(e_name, r_name,procPos, hrv,num,isUP, bHaveVal, bHaveDefinitionsDiff, pDefPar);
    }
    ~paramdefs()
    {
      Clear();
    }

	inline bool operator==(const paramdefs& p)
    {     
	  return this == &p || FastCompareNoCase.Compare(p.Names[0], Names[0]) == 0 && FastCompareNoCase.Compare(p.Names[1], Names[1]) == 0;
    }

	BOOL GetHaveDefinitionsDiff() const {return m_bHaveDefinitionsDiff; }
  }; // paramdefs

  //CParamDefs() : nPosOnReadFreeProp(-1), nPosOnWriteFreeProp(-1), nPosConstructor(-1), nPosDestructor(-1),
		//		nPosOfIsSerializableMethod(-1), nPosOfSaveToStringMethod(-1), nPosOfLoadFromStringMethod(-1)
  CParamDefs()
  {}

  virtual ~CParamDefs(){}

  inline bool AddParam(const char *eng_descr, const char *rus_descr, int procPos, int is_func = 0, int nNumParam = 0, bool isUP = false, bool bHaveVal = false, BOOL bHaveDefinitionsDiff = FALSE)
  {
    paramdefsptr par(new paramdefs(eng_descr, rus_descr, procPos,is_func, nNumParam, isUP, bHaveVal, bHaveDefinitionsDiff)); 

	if (Find(par) == vec_p_def.end())
    {
      vec_p_def.push_back(par);
      int pos = vec_p_def.size() - 1;      
      if (eng_descr)
        mapIndexes[eng_descr] = pos;
	  if (rus_descr){
        mapIndexes[rus_descr] = pos;
	  }      
      return true;
    }
    else
      return false;
    }
  inline bool RemoveParam(const char *eng_descr, const char *rus_descr)
  {
	paramdefsptr par(new paramdefs(eng_descr, rus_descr)); 

	ITER Iter = Find(par);

    if (Iter != vec_p_def.end())
    {
      if (eng_descr)
        mapIndexes.RemoveKey(eng_descr);
	  if (rus_descr){
        mapIndexes.RemoveKey(rus_descr);
	  }

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
	return *vec_p_def[index].get();
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
	typedef boost::shared_ptr<paramdefs> paramdefsptr;
	typedef std::vector<paramdefsptr> collection;
	collection vec_p_def;
	typedef collection::iterator ITER;
	typedef collection::const_iterator CONST_ITER;
	CIStringMap<int, int> mapIndexes;  
	CIStringMap<int, int> mapIndOfProperty;  

  struct CEqFunctor
  {
	  const paramdefsptr& m_par1;
	  CEqFunctor(const paramdefsptr& par1) : m_par1(par1){}
	  bool operator () (const paramdefsptr& par2) const 
	  { 
		  return *m_par1 == *par2; 
	  };
  };

  ITER Find(const paramdefsptr& par)
  {
	  return std::find_if(vec_p_def.begin(), vec_p_def.end(), CEqFunctor(par));
  }
};

#endif // !defined(AFX_PARAMDEFS_H__3B7722E4_086E_4439_A697_F0DF8129DC51__INCLUDED_)
