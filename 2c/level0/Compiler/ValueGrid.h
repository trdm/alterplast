// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueGrid.h: interface for the CValueGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
#define AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "../GridCtrl_src/GridCtrl.h"

class CValueGrid : public CValueControl  
{
DECLARE_DYNCREATE(CValueGrid);

public:
	CValueGrid();
	virtual ~CValueGrid();

	//CGridCtrl *pGrid;
	

	void Attach(CGridCtrl *pSetGrid){pWnd=pSetGrid;};
	int nCurRow;


	
	//������ ��� ����������� �������
	CString csObjectName;
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������


	int  FindColumn(CString csName);
	void SetAttribute(int iName,CValue &Val);//��������� ��������
	CValue GetAttribute(int iName);//��������� �������� ��������
	int  FindAttribute(CString csName);
	CString GetAttributeName(int nNumber,int nAlias=0);
	int  GetNAttributes(void);

	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetString(void)const;
	CString GetTypeString(void)const{return GetString();};
	int GetColumnNumber(CValue *pVal);

};

#endif // !defined(AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
