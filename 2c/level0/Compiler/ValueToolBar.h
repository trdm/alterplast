// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002-2004�.
// SQLLite.h: interface for the CValueToolBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBAR_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_TOOLBAR_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"

class CValueToolBar : public CValue  
{
DECLARE_DYNCREATE(CValueToolBar );
public:
	CValueToolBar();
	virtual ~CValueToolBar();
	
	CString csObjectName;

	//������ ��� ����������� �������
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������
	virtual CString GetString(void);
	//virtual CValue GetAttribute(int iName);//�������� ��������


	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};

	//////////////////////////////////////////////////////////////////////
	//���������������� ������
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CValueToolBar ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL AddButton(CValue &rez,CValue**p);
	BOOL GetCount(CValue &rez,CValue**p);
	BOOL ShowWindow(CValue &rez,CValue**p);
	BOOL RemoveButton(CValue &rez,CValue**p);
	BOOL GetMainTool(CValue &rez,CValue**p);
	BOOL GetEditTool(CValue &rez,CValue**p);
	BOOL GetModuleTool(CValue &rez,CValue**p);
	BOOL Name(CValue &rez,CValue**p);
	BOOL GetImage(CValue &rez,CValue**p);
	

	CString m_csName;

	//��������������� ������
	void Attach(class CExtToolControlBar	*m_pTool);
private:
	void Create();
	class CExtToolControlBar		*m_pToolBar;

};

#endif // !defined(AFX_TOOLBAR_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
