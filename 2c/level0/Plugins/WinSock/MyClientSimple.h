// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CMyClientSimple class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyOBJ33_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_MyOBJ33_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"

class CMyClientSimple : public CValue  
{
DECLARE_DYNCREATE(CMyClientSimple );
public:
	CMyClientSimple();
	virtual ~CMyClientSimple();
	

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
	typedef BOOL (CMyClientSimple ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Connect(CValue &rez,CValue**ppV);
	BOOL Send(CValue &rez,CValue**ppV);
	BOOL Receive(CValue &rez,CValue**ppV);
	BOOL Receive0(CValue &rez,CValue**ppV);
	BOOL Close(CValue &rez,CValue**ppV);
	BOOL LoadToFile(CValue &rez,CValue**ppV);
	BOOL Port(CValue &rez,CValue**ppV);

	//�������� ������� �������
	SOCKET conn;
	int nWasConnect;
	int	m_nPort;

    CMemFile f;
	//��������������� ������
	int ParseURL(CString m_url, CString &server, CString &filepath, CString &filename);
};

#endif // !defined(AFX_MyOBJ3_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
