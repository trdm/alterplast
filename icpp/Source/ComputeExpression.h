// ComputeExpression.h: interface for the CComputeExpression class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPUTEEXPRESSION_H__6E6D7382_203D_4478_BA18_A2EC1F337D20__INCLUDED_)
#define AFX_COMPUTEEXPRESSION_H__6E6D7382_203D_4478_BA18_A2EC1F337D20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComputeExpression  
{
public:
  CComputeExpression(CString& strCode, const CMapStringToPtr& map);
	virtual ~CComputeExpression();
  operator bool() { return GetResult(); }
	bool GetResult();
	

private:
  void eval_exp (bool &res);
	void eval_exp1(bool &res);
  void eval_exp2(bool &res);	
 	void eval_exp3(bool &res);	

  enum typesTokens {UNDEFTOK, NOTEQL, OR, AND, EQL, NEGATE, OPENBRACKET, CLOSEBRACKET, SYMBOL, FINISHED};

	void get_token();

  CString m_strToken; // Содержит текстовое представление лексемы
  typesTokens m_Token_Type;
  CString m_strCode;
  const CMapStringToPtr& m_map;
  int m_nCurPos; // текущая позиция в строке кода  
};

#endif // !defined(AFX_COMPUTEEXPRESSION_H__6E6D7382_203D_4478_BA18_A2EC1F337D20__INCLUDED_)
