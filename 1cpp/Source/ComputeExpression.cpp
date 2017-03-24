// ComputeExpression.cpp: implementation of the CComputeExpression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComputeExpression.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComputeExpression::CComputeExpression(CString &strCode, const CMapStringToPtr& m) : m_strCode(strCode), 
                                                                                     m_nCurPos(0),
                                                                                     m_map(m)
{
  m_strCode.Replace(" ","");  // уберем все пробелы
  m_strCode.Replace("\t",""); // уберем все табуляции
}

CComputeExpression::~CComputeExpression()
{

}

bool CComputeExpression::GetResult()
{
  bool result = false;
  eval_exp(result);
  return result;
}

void CComputeExpression::get_token()
{
  m_Token_Type = UNDEFTOK;
  m_strToken.Empty();

  if (m_strCode.IsEmpty())
    return;

  int nLen = m_strCode.GetLength();
  
  if (nLen == m_nCurPos)
  {
    m_Token_Type = FINISHED;
    return;
  }
  
  if ('!' == m_strCode[m_nCurPos])
  {
    if (m_nCurPos + 1 < nLen)
      if ('=' == m_strCode[m_nCurPos+1])
      {
        m_Token_Type = NOTEQL;
        m_nCurPos += 2;
        m_strToken = "!=";
        return;
      }
      else
      {
        m_Token_Type = NEGATE;
        m_nCurPos++;
        m_strToken = "!";
        return;
      }
    else
    {
      m_Token_Type = NEGATE;
      m_nCurPos++;
      m_strToken = "!";
      return;
    }
  }
  switch(m_strCode[m_nCurPos])
  {
    case '|':
    {
      m_nCurPos++;
      m_Token_Type = OR;
      m_strToken = "|";
      return;
    }
    break;
    case '&':
    {
      m_nCurPos++;
      m_Token_Type = AND;
      m_strToken = "&";
      return;
    }
    break;
    case '=':
    {
      m_nCurPos++;
      m_Token_Type = EQL;
      m_strToken = "=";
      return;
    }
    break;       
    case '(':
    {
      m_nCurPos++;
      m_Token_Type = OPENBRACKET;
      m_strToken = "(";
      return;
    }
    case ')':
    {
      m_nCurPos++;
      m_Token_Type = CLOSEBRACKET;
      m_strToken = ")";
      return;
    }
    break;       
    default:
    {
      while((m_nCurPos < nLen) && (!strchr("|!=&()", m_strCode[m_nCurPos])))
      {                
        m_strToken += m_strCode[m_nCurPos];
        m_nCurPos++;
      }
      //m_strToken.TrimLeft(); m_strToken.TrimRight();
      m_Token_Type = SYMBOL;
      return;
    }    
  }  
}

void CComputeExpression::eval_exp(bool &res)
{
  get_token();
  if (m_strToken.IsEmpty())
    return;
  eval_exp1(res);
}

void CComputeExpression::eval_exp1(bool &res)
{
  bool bTemp = false;  
  eval_exp2(res);  

  typesTokens op = m_Token_Type;
  if (op <= EQL && op >= NOTEQL)
  {
    get_token();
    eval_exp1(bTemp);
    switch(op)
    {
    case NOTEQL:
      res = res != bTemp;
      break;
    case OR:
      res = res || bTemp;
      break;
    case AND:
      res = res && bTemp;
      break;
    case EQL:
      res = res && bTemp;
      break;
    } 
  }
}

void CComputeExpression::eval_exp2(bool &res)
{
  typesTokens op = UNDEFTOK;

  if (m_Token_Type == NEGATE)
  {
    op = NEGATE;
    get_token();
  }
  eval_exp3(res);
  if (op == NEGATE) res = !res;
}

void CComputeExpression::eval_exp3(bool &res)
{
  if (m_Token_Type == OPENBRACKET)
  {
    get_token();
    eval_exp1(res);
    if (m_Token_Type != CLOSEBRACKET)
      throw CString("Ошибка! Не найдена закрывающая скобка ) в выражении:");

    get_token();
  }
  else if (m_Token_Type == SYMBOL)
  {
    void *pV = NULL;
    res = m_map.Lookup(m_strToken, (void*&)pV) > 0 ? true : false;
    get_token();
  }
}
