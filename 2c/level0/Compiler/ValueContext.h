// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueContext.h: interface for the CValueContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueContext_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
#define AFX_ValueContext_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueObject.h"

class CValueContext : public CValueObject
{
DECLARE_DYNCREATE(CValueContext);
private:
	CValue aContVarList;
	CValue aContValueList;
public:
	CValueContext();
	virtual ~CValueContext();

	//������ ��� ����������� �������
	CValue Method(int iName,CValue **aParams);
	int  FindMethod(CString csName);
	void SetAttribute(int iName,CValue &Val);//��������� ��������
	CValue GetAttribute(int iName);//��������� �������� ��������
	int  FindAttribute(CString csName);


	int  GetType(void)const;
	CString GetTypeString()const;
	CString GetString(void)const;
	NUMBER GetNumber(void)const;
	void SaveToString(CString &Str);
	void LoadFromString(CString &Str);

	void Attach(void *pObj);
	void Detach();

};

#endif // !defined(AFX_ValueContext_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
