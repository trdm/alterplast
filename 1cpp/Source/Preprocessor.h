// Preprocessor.h: interface for the CPreprocessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_)
#define AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SetOfHierarchy.h"
#include "ModuleString.h"

class CRangesOfDescr
{
public:
  CRangesOfDescr() {}
  void AddRange(const CString& fn, int pos)
  {
    m_ListRanges.push_back(RangesFilesDescr(fn, pos));
  }
  CString GetNameOfFileDescrOnPos(int pos)
  {
    CString res;
    ITER end = m_ListRanges.end();
    ITER findIt = end;
    for (ITER i = m_ListRanges.begin(); i!=end; ++i)
    {      
      if ((*i).nPosition>pos)      
        break;
      findIt = i;
    }
    if (findIt != end)
      res = (*findIt).full_name;

    return res;
  }
  void Print()
  {
    ITER end = m_ListRanges.end();
    for (ITER i = m_ListRanges.begin(); i!=end; ++i)
    {
      CString text;  
      text.Format("%s = {%d}",(*i).full_name, (*i).nPosition);
      pBkEndUI->DoMessageLine(text, mmInformation);      
    }
  }


private:
  struct RangesFilesDescr {
    CString full_name;
    int nPosition;
    RangesFilesDescr(const CString& fn, int p) : full_name(fn), nPosition(p) {}
    RangesFilesDescr() : nPosition(0) {}
  };
  
  typedef list<RangesFilesDescr> LIST_RANGES_DESCR;
  typedef list<RangesFilesDescr>::iterator ITER;
  LIST_RANGES_DESCR m_ListRanges;
};



class CPreprocessor  
{
  enum eKindStrings {/* 0  = */ PREPROCESSOR, 
                     /* 1  = */ IF, 
                     /* 2  = */ ELSE, 
                     /* 3  = */ ELIF, 
                     /* 4  = */ DEFINE, 
                     /* 5  = */ ENDIF, 
                     /* 6  = */ UNDEF, 
                     /* 7  = */ WARNING, 
                     /* 8  = */ ERR, 
                     /* 9  = */ STRING,
                     /* 10 = */ EXINFO,
                     /* 11 = */ INCLUDE,
                     /* 12 = */ NEWLINE,
                     /* 13 = */ CURMETHINFO};
  
  // Атрибуты строки
  struct AtrAndString {
    eKindStrings type;    // тип строки
    LPCTSTR pStartString; // указатель на начало строки
    int nLenString;       // полная длина строки
    //AtrAndString(eKindStrings t, CString& s) : type(t), strSource(s) {}
  };
  typedef vector<AtrAndString>::const_iterator CONST_ITER;
  typedef vector<AtrAndString>::iterator ITER;

public:
	// artbear,sedmin - рефакторинг (в трех местах кода встречаются почти одинаковые выражения)
	//  конструктор создания и немедленного запуска (start)
	static void Preprocess(bool bMake_NOW_PREPARE_CLASS, CString& strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL);

  class ExeptionPreprocessor {
  public:
    ExeptionPreprocessor(const char *pS, const CString& pC, int n) : strError(pS), strCode(pC), m_Num(n) {}
    inline CString GetErrorString() { return strError;}
    inline CString GetCodeString()  { return strCode; }
    inline int GetNumString()       { return m_Num;   }
  private:
    CString strError;
    CString strCode;
    int m_Num;
  };

private:  
	CPreprocessor(CString& strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL);
	//virtual ~CPreprocessor();
	void Start();

  bool NestedProcess(LPTSTR& lpText, int& nCurNum, CString const *FN, AtrAndString* pAtr = NULL, bool bProcIF = false, bool bIsNoCommentText = true);
  bool GetTokenType(LPTSTR& pText, AtrAndString& Atr);
  void CountAndFillingStrings(CString &strDest, CString const *strFileName = NULL);


  CString &m_strCode;
  CMapStringToPtr &m_RefMap;  
  vector<AtrAndString> m_MassStrings;  
  CStringArray m_ArrUseStr;
  int m_nCurStr;
  int m_nLen;
  CMapStringToString& m_mapLoadedIncludeFiles;
  const CString& m_strNameProcessFile;
  CRangesOfDescr *m_pRanges;
};

#endif // !defined(AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_)
