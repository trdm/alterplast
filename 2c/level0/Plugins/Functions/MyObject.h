// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CSQLLite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"

class CSQLLite : public CValue  
{
DECLARE_DYNCREATE(CSQLLite );
public:
	CSQLLite();
	virtual ~CSQLLite();
	

	//������ ��� ����������� �������
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������
	virtual CString GetString(void);
	virtual CString GetTypeString(void)const;
	//virtual CValue GetAttribute(int iName);//�������� ��������

	//////////////////////////////////////////////////////////////////////
	//���������������� ������
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CSQLLite ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Connect(CValue &rez,CValue**ppV);

	//�������� ������� �������
	char **pResultSQL;
	int nRowCount;
	int nColCount;
	int nCurLine;

	//��������������� ������
	void Clear();
	int Query(CString Str);
	CString GetField(int n);
};

#endif // !defined(AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
