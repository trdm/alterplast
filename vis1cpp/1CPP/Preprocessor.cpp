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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreprocessor::CPreprocessor(CString &strCode, CMapStringToPtr& m, CMapStringToString& mapStrToStr, 
                             const CString& strName, CRangesOfDescr* pRanges) : m_strCode(strCode),
                                                                             m_RefMap(m),
                                                                             m_nCurStr(0),
                                                                             m_nLen(0),
                                                                             mapLoadedIncludeFiles(mapStrToStr),
                                                                             strNameProcessFile(strName),
                                                                             bPreproc(true),
                                                                             m_pRanges(pRanges)
{

}

CPreprocessor::~CPreprocessor()
{

}

void CPreprocessor::Start()
{
  CountAndFillingStrings(m_strCode);
  if (bPreproc)
  {
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
              strSource.Format("__err__=CreateObject(\"ExecuteModule\");__err__.RaiseError(\"%s\");",strCompute);              
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
              strSource.Format("Message(\"%s\",\"!\");", strCompute);
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
}

void CPreprocessor::CountAndFillingStrings(CString &strDest, CString const *strFileName)
{  
  /*CString text;  
  text.Format("%s = {%d}",strFileName!=NULL ? *strFileName:strNameProcessFile, m_nLen);
  pBkEndUI->DoMessageLine(text, mmExclamation);*/

  if (m_pRanges)  
    m_pRanges->AddRange(strFileName!=NULL ? *strFileName:strNameProcessFile, m_nLen);
  
  m_ArrUseStr.Add(strDest);
  LPTSTR lpText = const_cast<LPTSTR>((LPCSTR)strDest);  
  NestedProcess(lpText, m_nCurStr, strFileName);  
  
  /*text.Format("%s = {%d}",strFileName!=NULL ? *strFileName:strNameProcessFile, m_nLen);
  pBkEndUI->DoMessageLine(text, mmExclamation2);*/
  /*if (m_pRanges)  
    m_pRanges->AddRange(strFileName!=NULL ? *strFileName:strNameProcessFile, m_nLen);*/
}

bool CPreprocessor::GetTokenType(LPTSTR &pText, AtrAndString& Atr)
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

bool CPreprocessor::NestedProcess(LPTSTR& lpText, int &nCurNum, CString const *FN, AtrAndString* pAtr, bool bProcIF, bool bIsNoCommentText)
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
            throw ExeptionPreprocessor("Не найдена директива #endif",CString(""),nCurNum);
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
            throw ExeptionPreprocessor(err, strSource, nCurNum);
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
          throw ExeptionPreprocessor("Директива //#else не может использоваться без //#if",strSource,nCurNum);
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
          throw ExeptionPreprocessor("Директива //#elif не может использоваться без //#if",strSource,nCurNum);
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
            throw ExeptionPreprocessor(err, strSource,nCurNum);
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
          throw ExeptionPreprocessor("Директива //#endif не может использоваться без //#if",strSource,nCurNum);
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
          str.Replace(" ", "");
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
              throw ExeptionPreprocessor("Ошибка управления разбором include оператора!", strSource,nCurNum);
            }
          }
          //pBkEndUI->DoMessageLine(strAliasName,mmBlueTriangle);
          
          if (strFileName.IsEmpty())
          {
            CString strSource(Atr.pStartString, Atr.nLenString);
            throw ExeptionPreprocessor("В include директиве не указан путь и название файла!", strSource,nCurNum);
          }
            CString strFileName_temp = strFileName; // artbear
            strFileName_temp.MakeLower();
          if ((strFileName_temp.Find(".prm") == -1)&&(strFileName_temp.Find(".ert") == -1)&&(strFileName_temp.Find(".txt") == -1)&&(strFileName_temp.Find("@md") == -1))
          {
            CString strSource(Atr.pStartString, Atr.nLenString);
            throw ExeptionPreprocessor("Неверно указано или не указано вовсе расширение файла!",strSource,nCurNum);
          }
        
        
          {
            CMetaDataOfClasses *pMeta=CMetaDataOfClasses::m_oneMetadata;
            if (!strAliasName.IsEmpty())
            {              
              
              CString path = pMeta->m_AliasOfPaths[strAliasName];
              if (path.IsEmpty())
              {
                CString strError;
                strError.Format("Не найден alias:%s указанный в директиве include!", strAliasName);
                CString strSource(Atr.pStartString, Atr.nLenString-1);
                throw ExeptionPreprocessor(strError,strSource,nCurNum);
              }              
              
              strFileName.Insert(0, path);
              
            }
            
			// это имя файла, в котором происходит обработка
			CString sDefFileName = m_pRanges->GetNameOfFileDescrOnPos(m_nLen-2);
			CString sDefFileFolderPath = ::ExtractFileDir(sDefFileName);

			// получу полное имя файла или из каталога ИБ или относительно каталога текущего defcls.prm
            CMetaDataOfClasses::MakeFullFileName(&strFileName, sDefFileFolderPath);
              
          }
          if (strNameProcessFile.CompareNoCase(strFileName)) // сами себя не инкладим
          {
            CString strResult;
            if (!mapLoadedIncludeFiles.Lookup(strFileName, strResult))                     
            {
              mapLoadedIncludeFiles[strFileName] = strResult;

              CModuleString strModule;
              if (strModule.LoadModule(strFileName)) // тут читаются defcls.prm
              {            
                /*if (m_pRanges)  
                  m_pRanges->AddRange(FN!=NULL ? *FN:strNameProcessFile, m_nLen);*/

                CountAndFillingStrings(strModule, &strFileName);                                
                
                if (m_pRanges)  
                  m_pRanges->AddRange(FN!=NULL ? *FN:strNameProcessFile, m_nLen);
              }
              else
              {
                CString strError;            
                CString strSource(Atr.pStartString, Atr.nLenString);
                throw ExeptionPreprocessor("Ошибка в директиве include",strSource,nCurNum);
              }
            }
          }        
        }
      }
      break;
    }    
    m_MassStrings.push_back(Atr);
  }
  return false;
}
