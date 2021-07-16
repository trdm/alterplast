// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueMeta.h: interface for the CValueMeta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueMeta_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_)
#define AFX_ValueMeta_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "../MetaObject.h"

class CValueMeta : public CValue  
{

enum
{
/*	enName=0,
	enName2,
	enAlias,
	enComment,
	enDescription,
	enCommonForms,
	enObjects,
*/
	enArray,
};


DECLARE_DYNCREATE(CValueMeta);
public:
	CValueMeta();
	virtual ~CValueMeta();

	CMetaObject *pObj;


	//������ ��� ����������� �������
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������


	CString csObjectName;
	CString GetString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};
	CValue GetAttribute(int iName);//�������� ��������

};

#endif // !defined(AFX_ValueMeta_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_)
