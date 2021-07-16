// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueTreeCtrl.h: interface for the CValueTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUETREECTRL_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
#define AFX_VALUETREECTRL_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COMPILER\Value.h"

class CValueTreeCtrl : public CValueControl
{
DECLARE_DYNCREATE(CValueTreeCtrl);

public:
	CValueTreeCtrl();
	virtual ~CValueTreeCtrl();

	//CTreeCtrl *pTree;
	class CMetadataTree *pMetaTree;
	
	BOOL m_bUseCheck;//��������� ������
	HTREEITEM hSelectParentItem;
	HTREEITEM hSelectCurrentItem;


	void SetTree(CTreeCtrl *pSetTree){pWnd=pSetTree;};
	void Attach(CTreeCtrl *pSetTree){pWnd=pSetTree;};


	void SetNonChecked(HTREEITEM CurItem);
	bool IsCheckable(HTREEITEM CurItem);
	void SetBold(HTREEITEM CurItem,int bState);
	int GetBold(HTREEITEM CurItem);

	
	//������ ��� ����������� �������
	CString csObjectName;
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������

	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetString(void)const
	{
		if(pWnd)
			return csObjectName;
		else
			return NOT_DEFINED;
	};
	CString GetTypeString(void)const{return csObjectName;};


};

#endif // !defined(AFX_VALUETREECTRL_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
