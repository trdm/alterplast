// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// ValueFileSystem.h: interface for the CValueFileSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueFileSystem_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
#define AFX_ValueFileSystem_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CValueFileSystem : public CValue  
{
DECLARE_DYNCREATE(CValueFileSystem);
public:
	CValueFileSystem();
	virtual ~CValueFileSystem();
	int nTimer;
	HANDLE hFindFile;
	CMapStringToPtr aListHandle;



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


	void CloseAllHandle();

};

#endif // !defined(AFX_ValueFileSystem_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
