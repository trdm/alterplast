// Preprocessor.cpp: implementation of the CPreprocessor class. 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Preprocessor.h"

#include "ComputeExpression.h"
#include "MetaDataOfClasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CRangesOfDescrImpl : boost::noncopyable
{
public:
	CRangesOfDescrImpl() {};
	void AddRange(const CString& fn, int pos)
	{
		m_ListRanges.push_back(RangesFilesDescr(fn, pos));
	}
	CString GetNameOfFileDescrOnPos(int pos) const
	{
		CString res;
		CONST_ITER end = m_ListRanges.end();
		CONST_ITER findIt = end;
		for (CONST_ITER i = m_ListRanges.begin(); i!=end; ++i)
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
			text.Format("%s = {%d}",(*i).full_name.operator LPCTSTR(), (*i).nPosition);
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

	typedef std::list<RangesFilesDescr> LIST_RANGES_DESCR;
	typedef std::list<RangesFilesDescr>::iterator ITER;
	typedef std::list<RangesFilesDescr>::const_iterator CONST_ITER;
	LIST_RANGES_DESCR m_ListRanges;
};

CRangesOfDescr::CRangesOfDescr() : m_pImpl(new CRangesOfDescrImpl) {};
CRangesOfDescr::~CRangesOfDescr() {};

void CRangesOfDescr::AddRange(const CString& fn, int pos)
{
	m_pImpl->AddRange(fn, pos);
}
CString CRangesOfDescr::GetNameOfFileDescrOnPos(int pos) const
{
	return 	m_pImpl->GetNameOfFileDescrOnPos(pos);
}

void CRangesOfDescr::Print()
{
	m_pImpl->Print();
}

class CPreprocessorImpl : boost::noncopyable
{
	friend class CPreprocessor;

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

		CString GetString() const { return CString(pStartString, nLenString); };
	};
	// TODO возможно, что хранение в векторе самих объектов, а не указателей слишком дорого обходится ?? artbear
	typedef std::vector<AtrAndString>::const_iterator CONST_ITER;
	typedef std::vector<AtrAndString>::iterator ITER;

	// artbear,sedmin - рефакторинг (в трех местах кода встречаются почти одинаковые выражения)
	//  конструктор создания и немедленного запуска (start)
	static void Preprocess(bool bMake_NOW_PREPARE_CLASS, CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL);

public:
	static void PreprocessStringForUserClass(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, const CString& strName)
	{ CMapStringToString emptymap; Preprocess(true, strCode, m_PreprocessorSymbolsMap, emptymap, strName);	};

	static void PreprocessString(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL)
	{ Preprocess(false, strCode, m_PreprocessorSymbolsMap, mapStrToStr, strName, pRanges);	};

	//class ExeptionPreprocessor {
	//public:
	//	ExeptionPreprocessor(const char *pS, const CString& pC, int n) : strError(pS), strCode(pC), m_Num(n) {}
	//	inline CString GetErrorString() { return strError;}
	//	inline CString GetCodeString()  { return strCode; }
	//	inline int GetNumString()       { return m_Num;   }
	//private:
	//	CString strError;
	//	CString strCode;
	//	int m_Num;
	//};

private:  
	CPreprocessorImpl(CString& strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges = NULL);
	//virtual ~CPreprocessor();
	void Start();

	bool NestedProcess(LPTSTR& lpText, int& nCurNum, CString const *FN, AtrAndString* pAtr = NULL, bool bProcIF = false, bool bIsNoCommentText = true);
	bool GetTokenType(LPTSTR& pText, AtrAndString& Atr);
	void CountAndFillingStrings(CString &strDest, CString const *strFileName = NULL);

	void ThrowExeptionPreprocessor(LPCTSTR szErrorMsg, const AtrAndString& Atr, int nCount)
	{ throw CPreprocessor::ExeptionPreprocessor(szErrorMsg, Atr.GetString(), nCount); };

	CString &m_strCode;
	CMapStringToPtr &m_RefMap;  
	std::vector<AtrAndString> m_MassStrings;  
	CStringArray m_SavedStringArray; // TODO если убрать этот вроде бы не использующийся объект - будет ошибка работы 1С++ :( - т.е. где-то ошибка дизайна :(
	int m_nCurStr;
	int m_nLen;
	CMapStringToString& m_mapLoadedIncludeFiles;
	const CString& m_strNameProcessFile;
	CRangesOfDescr *m_pRanges;
};

void CPreprocessor::PreprocessStringForUserClass(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, const CString& strName)
{ CPreprocessorImpl::PreprocessStringForUserClass(strCode, m_PreprocessorSymbolsMap, strName); };

void CPreprocessor::PreprocessString(CString& strCode, CMapStringToPtr& m_PreprocessorSymbolsMap, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges)
{ CPreprocessorImpl::PreprocessString(strCode, m_PreprocessorSymbolsMap, mapStrToStr, strName, pRanges); };

// artbear,sedmin - рефакторинг (в трех местах кода встречаются почти одинаковые выражения)
//  конструктор создания и немедленного запуска (start)
void CPreprocessorImpl::Preprocess(bool b_NOW_PREPARE_CLASS, CString& strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, const CString& strName, CRangesOfDescr* pRanges)
{
	const char* cstr_NOW_PREPARE_CLASS = "_NOW_PREPARE_CLASS";

    void *pV = NULL;
	bool bPrev_NOW_PREPARE_CLASS = (m.Lookup(cstr_NOW_PREPARE_CLASS, pV) > 0);
	
	if (b_NOW_PREPARE_CLASS)
		m[cstr_NOW_PREPARE_CLASS] = NULL;

	CPreprocessorImpl preproc(strCode, m, mapStrToStr, strName, pRanges);
	preproc.Start();

	if (b_NOW_PREPARE_CLASS || !bPrev_NOW_PREPARE_CLASS)
		m.RemoveKey(cstr_NOW_PREPARE_CLASS);
	else
		if (bPrev_NOW_PREPARE_CLASS)
			m.SetAt(cstr_NOW_PREPARE_CLASS, pV);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreprocessorImpl::CPreprocessorImpl(CString &strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, 
                             const CString& strName, CRangesOfDescr* pRanges) : m_strCode(strCode),
                                                                             m_RefMap(m),
                                                                             m_nCurStr(0),
                                                                             m_nLen(0),
                                                                             m_mapLoadedIncludeFiles(mapStrToStr),
                                                                             m_strNameProcessFile(strName),
                                                                             m_pRanges(pRanges)
{
}

/*
CPreprocessorImpl::~CPreprocessor()
{
}
*/

void CPreprocessorImpl::Start()
{
    CountAndFillingStrings(m_strCode);
    if (m_nLen > 0)
    {   
      CString strNewText;
      int pos = 0;
      LPTSTR lpStr = strNewText.GetBufferSetLength(m_nLen);
      for (CONST_ITER c_iter = m_MassStrings.begin(); c_iter != m_MassStrings.end(); ++c_iter)
      {
        const AtrAndString& Atr = *c_iter;
        switch (Atr.type)
        {
          case PREPROCESSOR:
            {
              c_iter = m_MassStrings.end();
            }
            break;
          case NEWLINE:
            {
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;              
            }
            break;
          case STRING:
            {
              memcpy(&lpStr[pos], Atr.pStartString, Atr.nLenString);
              pos += Atr.nLenString;
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;              
            }
            break;
          case ERR:
            {
              CString strCompute(Atr.pStartString, Atr.nLenString);
              strCompute.Replace("//#error","");
              strCompute.TrimLeft(); strCompute.TrimRight();
              CString strSource;
              strSource.Format("__err__=CreateObject(\"ExecuteModule\");__err__.RaiseError(\"%s\");", strCompute.operator LPCTSTR());              
              int nLen = strSource.GetLength();
              memcpy(&lpStr[pos], strSource, nLen);
              pos += nLen;
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;              
            }
            break;
          case EXINFO:
            {
              LPCSTR lpcExInfo = "__em__1= CreateObject(\"ExecuteModule\");__em__1.ExInfo();__em__1=\"\";";              
              int nLen = strlen(lpcExInfo);
              memcpy(&lpStr[pos], lpcExInfo, nLen);
              pos += nLen;
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;              
            }
            break;
          case CURMETHINFO:
            {
              LPCSTR lpcCurmeth = "__em__1= CreateObject(\"ExecuteModule\");__em__1.InfoAboutCurExeMeth();__em__1=\"\";";
              int nLen = strlen(lpcCurmeth);
              memcpy(&lpStr[pos], lpcCurmeth, nLen);
              pos += nLen;
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;
            }
            break;
          case WARNING:
            {
              CString strCompute(Atr.pStartString, Atr.nLenString);
              strCompute.Replace("//#warning","");
              strCompute.TrimLeft(); strCompute.TrimRight();
              CString strSource;
              strSource.Format("Message(\"%s\",\"!\");", strCompute.operator LPCTSTR());
              int nLen = strSource.GetLength();
              memcpy(&lpStr[pos], strSource, nLen);
              pos += nLen;
              lpStr[pos] = '\r';
              pos++;              
              lpStr[pos] = '\n';
              pos++;              
            }         
            break;
          default:
            {
              pBkEndUI->DoMessageLine("default:", mmBlackErr);
            }
        }
      }      
      m_strCode = strNewText;
    }
}

void CPreprocessorImpl::CountAndFillingStrings(CString &strDest, CString const *strFileName)
{  
//   CString text;  
//   text.Format("%s = {%d}",strFileName!=NULL ? *strFileName:m_strNameProcessFile, m_nLen);
//   pBkEndUI->DoMessageLine(text, mmExclamation);

  if (m_pRanges)  
    m_pRanges->AddRange(strFileName!=NULL ? *strFileName:m_strNameProcessFile, m_nLen);
  
  m_SavedStringArray.Add(strDest);
  LPTSTR lpText = const_cast<LPTSTR>(strDest.operator LPCTSTR());  
  NestedProcess(lpText, m_nCurStr, strFileName);  
  
  //text.Format("%s = {%d}",strFileName!=NULL ? *strFileName:m_strNameProcessFile, m_nLen);
  //pBkEndUI->DoMessageLine(text, mmExclamation2);
  //if (m_pRanges)  
  //  m_pRanges->AddRange(strFileName!=NULL ? *strFileName:m_strNameProcessFile, m_nLen);
}

bool CPreprocessorImpl::GetTokenType(LPTSTR &pText, AtrAndString& Atr)
{    
  if (*pText == '\0')
  {    
    return false;
  }
  
  Atr.type = STRING;

  int nLen = 0;
  LPTSTR pStart = pText;
  while(*pText != '\0')
  {        
    if (*pText == '\r')      
    {
      pText += 2;
      break;
    }
    else
    {
      ++pText;
      ++nLen;    
    }
  } 
  bool bIsZeroEndStr = (*pText == '\0');
  
  if (nLen > 0)
  {        
    int state = 0;
    bool bContunie = true;

    for (int pos = 0; pos < nLen && bContunie; pos++)
    {
      switch(state)
      {
      case 0:
        {
          if      (pStart[pos] == ' ')  state = 0;
          else if (pStart[pos] == '\t') state = 0;
          else if (pStart[pos] == '/')  state = 1;
          else                          bContunie = false;
          break;
        }
      case 1:
        {
          if (pStart[pos] == '/') 
            state = 2; 
          else
            bContunie = false;
          break;
        }      
      case 2:
        {
          if (pStart[pos] == '#') 
            state = 3; 
          else
            bContunie = false;
          break;
        }
      case 3:
        {
          if (pStart[pos] == 'i')
            state = 100;
          else if (pStart[pos] == 'e')
            state = 200;
          else if (pStart[pos] == 'd')
            state = 300;
          else if (pStart[pos] == 'u')
            state = 400;
          else if (pStart[pos] == 'w')
            state = 500;
          else if (pStart[pos] == 'p')
            state = 600;
          else if (pStart[pos] == 'c')
            state = 700;
          else        
            bContunie = false;
          break;
        }
      case 100: // i
        if      (pStart[pos] == 'f') state = 101; //if
        else if (pStart[pos] == 'n') state = 150; //in
        break; // if
      case 101: // if 
        if (pStart[pos] == ' ')
        {
          state = 102; // directive if
          bContunie = false;
          Atr.type = IF;
        }
        break;
      case 150: // in          
        if (pStart[pos] == 'c') state = 151; break; // inc
        break;
      case 151: // inc          
        if (pStart[pos] == 'l') state = 152; break; // incl
        break;
      case 152: // incl          
        if (pStart[pos] == 'u') state = 153; break; // inclu
        break;
      case 153: // inclu
        if (pStart[pos] == 'd') state = 154; break; // includ
        break;
      case 154: // includ
        if (pStart[pos] == 'e') state = 155; break; // include
        break;
      case 155: // include
        if (pStart[pos] == ' ')
        {
          state = 156; // directive include
          bContunie = false;
          Atr.type = INCLUDE;
        }
        break;
      case 200:          
        if      (pStart[pos] == 'l') state = 210; // el
        else if (pStart[pos] == 'n') state = 230; // en
        else if (pStart[pos] == 'r') state = 240; // er
        else if (pStart[pos] == 'x') state = 250; // eх  
        break;
      case 210: // el          
        if      (pStart[pos] == 'i') state = 201; // eli
        else if (pStart[pos] == 's') state = 211; // els
        break;          
      case 201: // eli          
        if (pStart[pos] == 'f') state = 202; break; // elif                      
      case 202: // elif
        if (pStart[pos] == ' ')
        {
          state = 203; // directive elif
          bContunie = false;
          Atr.type = ELIF;
        }
        break;
      case 211: // els
        if (pStart[pos] == 'e')
        {
          state = 212; // else
          bContunie = false;
          Atr.type = ELSE;
        }
        break;                  
      case 230: // en
        if (pStart[pos] == 'd') state = 231; break; //end          
      case 231: // end
        if (pStart[pos] == 'i') state = 232; break; //endi          
      case 232: // endi
        if (pStart[pos] == 'f')
        {
          state = 233; //endif            
          bContunie = false;
          Atr.type = ENDIF;
        }
        break;        
      case 240:
        if (pStart[pos] == 'r') state = 241; break; //err          
      case 241:
        if (pStart[pos] == 'o') state = 242; break; //erro          
      case 242:
        if (pStart[pos] == 'r') state = 243; break; //error          
      case 243:
        if (pStart[pos] == ' ')
        {
          state = 244; //error
          bContunie = false;
          Atr.type = ERR;
        }
        break;
      case 250: //ex
        if (pStart[pos] == 'i') state = 251; break; //exi          
      case 251: //exi
        if (pStart[pos] == 'n') state = 252; break; //exin          
      case 252: //exin
        if (pStart[pos] == 'f') state = 253; break; //exinf          
      case 253: //exinf
        if (pStart[pos] == 'o')
        {
          state = 254; //exinfo
          bContunie = false;
          Atr.type = EXINFO;
        }
        break;
      case 300: //d
        if (pStart[pos] == 'e') state = 301; break; //de          
      case 301: //de
        if (pStart[pos] == 'f') state = 302; break; //def          
      case 302: //def
        if (pStart[pos] == 'i') state = 303; break; //defi          
      case 303: //defi
        if (pStart[pos] == 'n') state = 304; break; //defin          
      case 304: //defin
        if (pStart[pos] == 'e') state = 305; break; //define
      case 305: //define
        if (pStart[pos] == ' ')
        {
          state = 306; //define
          bContunie = false;
          Atr.type = DEFINE;
        }
        break;
      case 400: //u
        if (pStart[pos] == 'n') state = 401; break; //un          
      case 401: //un
        if (pStart[pos] == 'd') state = 402; break; //und          
      case 402: //und
        if (pStart[pos] == 'e') state = 403; break; //unde          
      case 403: //unde
        if (pStart[pos] == 'f') state = 404; break; //undef          
      case 404: //undef
        if (pStart[pos] == ' ')
        {
          state = 404; //undef
          bContunie = false;
          Atr.type = UNDEF;
        } 
        break;          
      case 500: //w
        if (pStart[pos] == 'a') state = 501; break; //wa
      case 501: //wa
        if (pStart[pos] == 'r') state = 502; break; //war
      case 502: //war
        if (pStart[pos] == 'n') state = 503; break; //warn
      case 503: //warn
        if (pStart[pos] == 'i') state = 504; break; //warni
      case 504: //warni
        if (pStart[pos] == 'n') state = 505; break; //warnin
      case 505: //warnin
        if (pStart[pos] == 'g') state = 506; break; //warning
      case 506: //warning
        if (pStart[pos] == ' ')
        {
          state = 507; //warning
          bContunie = false;
          Atr.type = WARNING;
        } 
        break;          
      case 600: //p
        if (pStart[pos] == 'r') state = 601; break; //pr
      case 601: //pr
        if (pStart[pos] == 'e') state = 602; break; //pre
      case 602: //pre
        if (pStart[pos] == 'p') state = 603; break; //prep
      case 603: //prep
        if (pStart[pos] == 'r') state = 604; break; //prepr
      case 604: //prepr
        if (pStart[pos] == 'o') state = 605; break; //prepro
      case 605: //prepro
        if (pStart[pos] == 'c') state = 606; break; //preproc
      case 606: //preproc
        if (pStart[pos] == 'e') state = 607; break; //preproce
      case 607: //preproce
        if (pStart[pos] == 's') state = 608; break; //preproces
      case 608: //preproces
        if (pStart[pos] == 's') state = 609; break; //preprocess
      case 609: //preprocess
        if (pStart[pos] == 'o') state = 610; break; //preprocesso
      case 610: //preprocesso
        if (pStart[pos] == 'r') state = 611; break; //preprocessor
      case 611: //preprocesso
        if (pStart[pos] == ' ') state = 612; break; //preprocessor
      case 612: //preprocesso 
        if (pStart[pos] == 'o') state = 613; break; //preprocessor o
      case 613: //preprocesso o
        if (pStart[pos] == 'f') state = 614; break; //preprocessor of
      case 614: //preprocesso of
        if (pStart[pos] == 'f') 
        {
          state = 615; //preprocessor off
          bContunie = false;
          Atr.type = PREPROCESSOR;
          break; 
        }
      case 700: //c
        if (pStart[pos] == 'u') state = 701; break; //cu
      case 701: //cu
        if (pStart[pos] == 'r') state = 702; break; //cur
      case 702: //cur
        if (pStart[pos] == 'm') state = 703; break; //curm
      case 703: //curm
        if (pStart[pos] == 'e') state = 704; break; //curme
      case 704: //curme
        if (pStart[pos] == 't') state = 705; break; //curmet
      case 705: //curmet
        if (pStart[pos] == 'h') //curmeth
        {
          state = 706; 
          bContunie = false;
          Atr.type = CURMETHINFO;
          break; //curmeth
        }
      default:
        {
          bContunie = false;
        }
      }
    } //switch          
  }
  if (nLen == 0)
  {
    Atr.pStartString = NULL;
    Atr.type = NEWLINE;
  }
  else
  {
    Atr.pStartString = pStart;
    Atr.nLenString = nLen; //bIsZeroEndStr ? nLen+1 : nLen;
  }
  return true;
}

bool CPreprocessorImpl::NestedProcess(LPTSTR& lpText, int &nCurNum, CString const *FN, AtrAndString* pAtr, bool bProcIF, bool bIsNoCommentText)
{
  AtrAndString Atr;
  bool bIsStateIF       = false;
  bool ProcessValue     = false;

  bool bCont = true;
  while (bCont)
  { 
    if (!bProcIF)
    {      
      bCont = GetTokenType(lpText, Atr);
      if (!bCont)
        break;
    }
    else
      Atr = *pAtr;

    switch (Atr.type)
    {
    case PREPROCESSOR:
      {      
        m_MassStrings.push_back(Atr);
        return false;
      }
      break;
    case IF:
      {               
        if (!bProcIF)
        {
          // Каждый if обрабатываем рекурсивно
          if (!NestedProcess(lpText, nCurNum, FN, &Atr, true, bIsNoCommentText))
            throw CPreprocessor::ExeptionPreprocessor("Не найдена директива #endif",CString(""),nCurNum);
          continue;
        }
        else if (bIsNoCommentText)
        {
          nCurNum++;
          bIsStateIF = true;          
          CString strCompute(Atr.pStartString, Atr.nLenString);
          strCompute.Replace("//#if","");
          strCompute.TrimLeft();
          CComputeExpression comp(strCompute, m_RefMap);
          try
          {
            bIsNoCommentText = comp.GetResult();
          }
          catch(CString &err)
          {
            CString strSource(Atr.pStartString, Atr.nLenString);
            throw CPreprocessor::ExeptionPreprocessor(err, strSource, nCurNum);
          }
          ProcessValue     = bIsNoCommentText;
          bProcIF = false;          
        }        
        else
        {
          ProcessValue = true;
          bProcIF      = false;
          bIsStateIF   = true;          
          nCurNum++;
        }
        Atr.type = NEWLINE; m_nLen += 2;
      }
      break;
    case ELSE:
      {
        nCurNum++;
        Atr.type = NEWLINE; m_nLen += 2;

        if (!bIsStateIF)
        {
          CString strSource(Atr.pStartString, Atr.nLenString);
          throw CPreprocessor::ExeptionPreprocessor("Директива //#else не может использоваться без //#if",strSource,nCurNum);
        }
        
        if (!ProcessValue)
        {
          bIsNoCommentText = !bIsNoCommentText;
          ProcessValue     = bIsNoCommentText;
          
        }
        else        
          bIsNoCommentText = false;                          
      }
      break;
    case ELIF:
      {
        nCurNum++;
        Atr.type = NEWLINE;
        m_nLen += 2;

        if (!bIsStateIF)
        {
          CString strSource;
          throw CPreprocessor::ExeptionPreprocessor("Директива //#elif не может использоваться без //#if",strSource,nCurNum);
        }
        
        if (!ProcessValue)          
        {
          CString strCompute(Atr.pStartString, Atr.nLenString);
          strCompute.Replace("//#elif","");
          CComputeExpression comp(strCompute, m_RefMap);
          try
          {
            bIsNoCommentText = comp.GetResult();
          }
          catch(CString &err)
          {
            CString strSource(Atr.pStartString, Atr.nLenString);
            throw CPreprocessor::ExeptionPreprocessor(err, strSource,nCurNum);
          }
          ProcessValue     = bIsNoCommentText;          
        }
        else                  
          bIsNoCommentText = false;                
      }
      break;
    case DEFINE:
      {
        if (bIsNoCommentText)
        {
          CString strCompute(Atr.pStartString, Atr.nLenString);
          strCompute.Replace("//#define","");
          strCompute.TrimLeft(); strCompute.TrimRight();
          m_RefMap[strCompute] = NULL;                    
        }  
        Atr.type = NEWLINE;
        m_nLen += 2;
        nCurNum++;
      }
      break;
    case ENDIF:
      { 
        nCurNum++;
        if (!bIsStateIF)
        {
          CString strSource(Atr.pStartString, Atr.nLenString);
          throw CPreprocessor::ExeptionPreprocessor("Директива //#endif не может использоваться без //#if",strSource,nCurNum);
        }
        bIsStateIF = false;        
        Atr.type = NEWLINE;
        m_nLen += 2;
        m_MassStrings.push_back(Atr);
        return true;        
        
      }
      break;
    case UNDEF:
      {
        if (bIsNoCommentText)
        {
          CString strCompute(Atr.pStartString, Atr.nLenString);
          strCompute.Replace("//#undef","");
          strCompute.TrimLeft(); strCompute.TrimRight();        
          m_RefMap.RemoveKey(strCompute);          
        }
        Atr.type = NEWLINE;
        m_nLen += 2;
        nCurNum++;
      }
      break;
    case WARNING:
      {
        m_nLen += 2;
        if (bIsNoCommentText)
        {          
          int nCountBreaks = 0;
          LPCTSTR lpStr = Atr.pStartString;
          while(*lpStr++ != '/') nCountBreaks++;
          
          lpStr = Atr.pStartString + nCountBreaks + 10;
          while((*lpStr == ' ') || (*lpStr == '\t')) {++nCountBreaks; ++lpStr;}

          lpStr = Atr.pStartString + Atr.nLenString - 1;
          while((*lpStr == ' ') || (*lpStr == '\t')) {nCountBreaks++; lpStr--;}

          LPCSTR lpcWarning = "Message(\"\",\"!\");";
          m_nLen += strlen(lpcWarning);
          m_nLen += Atr.nLenString - nCountBreaks - 10;          
        }
        else
        {
          Atr.type = NEWLINE;          
        }
        nCurNum++;
      }
      break;
    case ERR:
      {
        m_nLen += 2;
        if (bIsNoCommentText)
        {
          //#error
          int nCountBreaks = 0;
          LPCTSTR lpStr = Atr.pStartString;
          
          while(*lpStr++ != '/') nCountBreaks++;
          
          lpStr = Atr.pStartString + nCountBreaks + 8;
          while((*lpStr == ' ') || (*lpStr == '\t')) {++nCountBreaks; ++lpStr;}

          lpStr = Atr.pStartString + Atr.nLenString - 1;
          while((*lpStr == ' ') || (*lpStr == '\t')) {++nCountBreaks; --lpStr;}
                    
          LPCSTR lpcErr = "__err__=CreateObject(\"ExecuteModule\");__err__.RaiseError(\"\");";          
          m_nLen += strlen(lpcErr);
          m_nLen += Atr.nLenString - nCountBreaks - 8;
          
        }
        else
        {
          Atr.type = NEWLINE;          
        }
        nCurNum++;
      }
      break;
    case EXINFO:
      {
        m_nLen += 2;
        if (bIsNoCommentText)
        {
          //#exinfo         
          LPCSTR lpcExInfo = "__em__1= CreateObject(\"ExecuteModule\");__em__1.ExInfo();__em__1=\"\";";          
          m_nLen += strlen(lpcExInfo);
        }
        else
        {
          Atr.type = NEWLINE;          
        }
        nCurNum++;
      }
      break;
    case CURMETHINFO:
      {
        m_nLen += 2;
        if (bIsNoCommentText)
        {
          //#curmeth          
          LPCSTR lpcCurmeth = "__em__1= CreateObject(\"ExecuteModule\");__em__1.InfoAboutCurExeMeth();__em__1=\"\";";
          m_nLen += strlen(lpcCurmeth);
        }
        else
        {
          Atr.type = NEWLINE;
          
        }
        nCurNum++;
      }
      break;
    case STRING:
      {
        m_nLen += 2;
        if (bIsStateIF && !bIsNoCommentText)
        {          
          Atr.type = NEWLINE;
        }
        else
        {
          m_nLen += Atr.nLenString;         
        }
        nCurNum++;
      }
      break;
    case NEWLINE:
      {
        m_nLen += 2;
        Atr.type = NEWLINE;
        nCurNum++;
      }
      break;
    case INCLUDE:
      {
        nCurNum++;
        m_nLen += 2;
        Atr.type = NEWLINE;
        if (bIsNoCommentText)
        {
          //m_MassStrings.push_back(Atr);
          int nCountBreaks = 0;
          LPCTSTR lpStr = Atr.pStartString;
          while(*lpStr++ != '/') nCountBreaks++;

          CString str(Atr.pStartString+nCountBreaks, Atr.nLenString-nCountBreaks);
          str.Replace("\"", "");  
          str.Replace(" ", "");// TODO artbear может быть, все-таки нужно оставлять пробелы
          str.Replace("//#include", "");

          CString strFileName;
          CString strAliasName;
          int state = 0;
          for (int pos = 0; pos < str.GetLength(); pos++)
          {
            CHAR  ch = str[pos];
            if (ch == '#' && state == 0)
            {
              state = 1; // Alias
              continue;
            }
            else if(ch == '\\' && state == 1)
              state = 0; // Path name and file name
          
            if (state == 1)
              strAliasName += ch;
            else if (state == 0)
              strFileName  += ch;
            else
            {
              CString strSource(Atr.pStartString, Atr.nLenString);
              throw CPreprocessor::ExeptionPreprocessor("Ошибка управления разбором include оператора!", strSource,nCurNum);
            }
          }
          if (strFileName.IsEmpty())
          {
            CString strSource(Atr.pStartString, Atr.nLenString);
            throw CPreprocessor::ExeptionPreprocessor("В include директиве не указан путь и название файла!", strSource,nCurNum);
          }
		  {
            CString strFileName_temp = strFileName;
            strFileName_temp.MakeLower();

            if ((strFileName_temp.Find(".prm") == -1)&&(strFileName_temp.Find(".ert") == -1)&&(strFileName_temp.Find(".txt") == -1)&&(strFileName_temp.Find("@md") == -1))
            {
			  CString strSource(Atr.pStartString, Atr.nLenString);
			  throw CPreprocessor::ExeptionPreprocessor("Неверно указано или не указано вовсе расширение файла!",strSource,nCurNum);
            }
		  }
        
          ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
          if (pHierarchy)
          {
            if (!strAliasName.IsEmpty())
            {
              CString path = ISetOfHierarchy::GetMetaDataOfClasses().GetPathFromAlias(strAliasName);
              if (path.IsEmpty())
              {
                CString strError;
                strError.Format("Не найден alias:%s указанный в директиве include!", strAliasName.operator LPCTSTR());
                CString strSource(Atr.pStartString, Atr.nLenString-1);
                throw CPreprocessor::ExeptionPreprocessor(strError,strSource,nCurNum);
              }
              strFileName.Insert(0, path);
            }
			// получу полное имя файла или из каталога ИБ или относительно каталога текущего defcls.prm
			CString sDefFileName = m_pRanges->GetNameOfFileDescrOnPos(m_nLen-2);
			CString sDefFileFolderPath = ::ExtractFileDir(sDefFileName);
            ISetOfHierarchy::MakeFullFileName(&strFileName, sDefFileFolderPath);
          }
          if (!m_strNameProcessFile.CompareNoCase(strFileName)) // сами себя не инкладим
			  break;

          CString strResult;
          if (m_mapLoadedIncludeFiles.Lookup(strFileName, strResult))                     
			break;

          m_mapLoadedIncludeFiles[strFileName] = strResult;

          CModuleString strModule;
          if (!strModule.LoadModule(strFileName))
		  {
			  CString strSource(Atr.pStartString, Atr.nLenString);
			  throw CPreprocessor::ExeptionPreprocessor("Ошибка в директиве include",strSource,nCurNum);
		  }

		  //if (m_pRanges)  
          //  m_pRanges->AddRange(FN!=NULL ? *FN:m_strNameProcessFile, m_nLen);

          CountAndFillingStrings(strModule, &strFileName);                                
            
          if (m_pRanges)  
            m_pRanges->AddRange(FN!=NULL ? *FN:m_strNameProcessFile, m_nLen);
        }
      }
      break;
    }    
    m_MassStrings.push_back(Atr);
  }
  return false;
}
