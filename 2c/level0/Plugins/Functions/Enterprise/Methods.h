//��������� ����� - VTOOLS.RU (info@vtools.ru)
// Methods.h: interface for the CMethods class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODS_H__6D1D1819_D2E4_471C_A311_23F4D700DE30__INCLUDED_)
#define AFX_METHODS_H__6D1D1819_D2E4_471C_A311_23F4D700DE30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SearchTree.h"

#include"plugdetect.h"

//������ �������� ����, ������� �� ����� ���� ������� ���������� � �������
typedef struct SEnglishRussianDef{
	char *Eng;
	char *Rus;
	char *sShortDescription;
}SEngRus;


class CMethods  
{
public:
//��������
	CMapStringToPtr treeMethods;//������ ������������ �������
	CMapStringToPtr treeAttributes;//������ ������������ ���������
	CArray<SEngRus,SEngRus> aMethods; //������ ������� (���� � ���)
	CArray<SEngRus,SEngRus> aAttributes; //������ ��������� (���� � ���)

	int nMethodsCount;
	int nAttributesCount;

//������
	FORPLUGIN CMethods();
	FORPLUGIN virtual ~CMethods();

	FORPLUGIN void Prepare(SEngRus *M,int nCountM,SEngRus *A=NULL,int nCountA=0);

	FORPLUGIN int FindMethod(CString csName);
	FORPLUGIN int FindAttribute(CString csName);

	FORPLUGIN CString GetMethodName(int nName,int nAlias);
	FORPLUGIN char *GetMethodDescription(int nNumber,int nAlias);
	FORPLUGIN CString GetAttributeName(int nName,int nAlias);

	FORPLUGIN int GetNMethods(void);
	FORPLUGIN int GetNAttributes(void);

};
#endif // !defined(AFX_METHODS_H__6D1D1819_D2E4_471C_A311_23F4D700DE30__INCLUDED_)
//��������� ����� - VTOOLS.RU (info@vtools.ru)
