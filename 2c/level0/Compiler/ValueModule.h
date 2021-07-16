// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueModule.h: interface for the CValueModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
#define AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"

class CValueModule : public CValue  
{
DECLARE_DYNCREATE(CValueModule);
public:

	CValueModule();
	virtual ~CValueModule();

	CProcUnit *pRun;
	CValue vContext;
	void Clear();



	//������ ��� ����������� �������
	CString csObjectName;
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������


	CString GetString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};

};

#endif // !defined(AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
