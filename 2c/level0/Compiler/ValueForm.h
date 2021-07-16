// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueForm.h: interface for the CValueForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_)
#define AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueControl.h"

//��������� �����
#define PWINDOW ((CWnd *)pWnd)
class CValueForm:public CValueControl
{

	DECLARE_DYNCREATE(CValueForm);

	CValueForm();
	~CValueForm();

	BOOL bIsView;
	BOOL bMicroForm;
public:
	CValue vParam;//�������� ��� �����
	CValue vLayers;//�������� ��� ����� (��� 7.7)


	virtual int Modify(int);

	void Attach(void *pObj);
	CValue GetValue(int);
	CString GetTypeString(void)const;



	//������ ��� ����������� �������
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	virtual CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	virtual void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	virtual CValue Method(int iName,CValue **aParams);//����� ������
	virtual void SetAttribute(int iName,CValue &Val);//��������� ��������
	virtual CValue GetAttribute(int iName);//�������� ��������


	virtual int  FindAttribute(CString csName);
	virtual CString GetAttributeName(int nNumber,int nAlias=0);
	virtual int  GetNAttributes(void);

	void SetString(CString Str)
	{
		Caption(Str);
	};
	CString GetString()
	{
		return Caption();
	};

	virtual int Visible()
	{
		ASSERT(pWnd);
		return PWINDOW->IsWindowVisible();
	};
	virtual int Visible(BOOL bStat)
	{
		ASSERT(pWnd);
		int nRes=Visible();
		PWINDOW->ShowWindow(bStat);
		return nRes;
	};

	//�����������
	virtual int Enable()
	{
		ASSERT(pWnd);
		return PWINDOW->IsWindowEnabled();
	};
	virtual int Enable(BOOL bStat)
	{
		ASSERT(pWnd);
		int nRes=Enable();
		PWINDOW->EnableWindow(bStat);
		return nRes;
	};

	//���������
	virtual CString Caption();
	void Caption(CString Str);
	virtual void Close();

};
#undef PWINDOW

#endif // !defined(AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_)
