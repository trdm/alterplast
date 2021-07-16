// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CMyClient3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"
#include "SocketClass.h"

class CMyClient3 : public CValue  
{
DECLARE_DYNCREATE(CMyClient3 );
public:
	CMyClient3();
	virtual ~CMyClient3();
	

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
	typedef BOOL (CMyClient3 ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Connect(CValue &rez,CValue**ppV);
	BOOL Send(CValue &rez,CValue**ppV);
	BOOL Receive(CValue &rez,CValue**ppV);
	BOOL Close(CValue &rez,CValue**ppV);
	BOOL LoadToFile(CValue &rez,CValue**ppV);
	BOOL Port(CValue &rez,CValue**ppV);

	//������
	CSocketClass	m_socket;
	int				m_nPort;
	//CValue			m_vContext;

	BOOL			bWasDataReceive;
	CString			csData;

//	static CRITICAL_SECTION	m_cs;

};

#endif // !defined(AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
