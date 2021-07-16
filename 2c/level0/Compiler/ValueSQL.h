// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueSQL.h: interface for the CValueSQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUESQL_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
#define AFX_VALUESQL_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "MySQL\MySQL.h"

class CValueSQL : public CValue  
{
DECLARE_DYNCREATE(CValueSQL);
public:
	CValueSQL();
	virtual ~CValueSQL();

enum
{
	enConnect=0,
	enConnectODBC,
	enConnectODBC2,
	enDisconnect,
	enQuery,
	enGetFieldsName,
	enCountFields,
	enGetLine,
	enGetField,
	enGetFieldsValue,
	enIsTableExist,
	enGetInsertID,
	enGetAllValues,

	enBeginTrans,
	enCommitTrans,
	enRollback,
	enGetConnect,
};


	CMySQL DB;


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

#endif // !defined(AFX_VALUESQL_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
