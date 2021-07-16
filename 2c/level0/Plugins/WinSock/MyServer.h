// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CMyServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyOBJ2_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_MyOBJ2_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"
//#include "MyClient.h"

#include "IOCPServer.h"


class CMyServer : public CValue  
{
DECLARE_DYNCREATE(CMyServer );
public:
	CMyServer();
	virtual ~CMyServer();
	

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
	//static void NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);

	//////////////////////////////////////////////////////////////////////
	//���������������� ������
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CMyServer ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Run(CValue &rez,CValue**ppV);
	BOOL Stop(CValue &rez,CValue**ppV);
	BOOL Send(CValue &rez,CValue**ppV);
	BOOL Port(CValue &rez,CValue**ppV);
	BOOL GetLocalAddress(CValue &rez,CValue**ppV);


	//������
	CIOCPServer		m_iocpServer;
	int				m_nPort;
	//CValue			m_vContext;

	ClientContext	*m_pCurContext;


};

#endif // !defined(AFX_MyOBJ2_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
