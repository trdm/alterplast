// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CMyServerSimple class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyOBJ22_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_MyOBJ22_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"

class CMyServerSimple;
struct CContextData
{
	CContextData()
	{
		ID=0;
		socket=0;
		nPort=0;
		pThis=0;
		pRunContext=0;
		bClose=0;
	};
	int ID;
	SOCKET socket;
	int	nPort;
	CMyServerSimple *pThis;
	CValue *pRunContext;
	BOOL bClose;
};


class CMyServerSimple : public CValue  
{
DECLARE_DYNCREATE(CMyServerSimple );
public:
	CMyServerSimple();
	virtual ~CMyServerSimple();
	

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
	typedef BOOL (CMyServerSimple ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Run(CValue &rez,CValue**ppV);
	BOOL Stop(CValue &rez,CValue**ppV);
	BOOL Send(CValue &rez,CValue**ppV);
	BOOL Port(CValue &rez,CValue**ppV);
	BOOL GetLocalAddress(CValue &rez,CValue**ppV);


	//������
	int				m_nPort;
	SOCKET			m_socket;
	CContextData	m_context;
	CValue			m_vRun;
	BOOL			m_bErr;

	CContextData	*m_pClient;



	//��������������� ������
	bool GetLocalAddress2(LPTSTR strAddress, UINT nSize);
	CString GetLocalAddress1();

	void SetClient(CContextData &client);
	CString GetHostName(SOCKET socket);
};

#endif // !defined(AFX_MyOBJ2_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
