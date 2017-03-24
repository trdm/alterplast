// Preprocessor.h: interface for the CPreprocessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_)
#define AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// todo #include "SetOfHierarchy.h" - рекурсия !!
#include "SetOfHierarchy.h"
#include "ModuleString.h"

class CRangesOfDescr //: boost::noncopyable
{
	friend class CRangesOfDescrImpl;
public:
  CRangesOfDescr();
  ~CRangesOfDescr();
  void AddRange(const CString& fn, int pos);
  CString GetNameOfFileDescrOnPos(int pos) const;
  void Print();

private:
	boost::scoped_ptr<CRangesOfDescrImpl> m_pImpl;
};

class CPreprocessor
{
public:
	static void PreprocessStringForUserClass(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, const CString& strName);

	static void PreprocessString(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL);

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
};

#endif // !defined(AFX_PREPROCESSOR_H__3CE66346_BE09_4B9F_B8D2_5428E0F9CFEF__INCLUDED_)
