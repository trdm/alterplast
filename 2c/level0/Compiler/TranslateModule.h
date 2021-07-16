// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// TranslateModule.h: interface for the CTranslateModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSLATEMODULE_H__990B9717_1909_4AF6_936A_DEE5DAF38FA1__INCLUDED_)
#define AFX_TRANSLATEMODULE_H__990B9717_1909_4AF6_936A_DEE5DAF38FA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Functions.h"
#include <string.h>
#include <stdlib.h>
#include "stdio.h"

#include "Value.h"
//#define CValue int

//������ �������� ����
struct aKeyWordsDef{
	char *Rus;
	char *Eng;
	char *sShortDescription;
};
extern struct aKeyWordsDef aKeyWords[];

enum
{
	LEXEM_ADD=0,
	LEXEM_ADDDEF,
	LEXEM_IGNORE,
};


//�����������

//�������� ������ ��������� �� ��������� ����
class CLexem
{
public:
	CLexem()
	{
		nType=0;
		nData=0;
		nStringNumber=0;
		nNumberLine=-1;
	};

	//��� �������:
	int	nType;
	//���������� �������:
	int		nData;		//����� ���������� �����(KEYWORD) ��� ������ �����������(DELIMETER)
	CValue	vData;		//��������, ���� ��� ��������� ��� �������� ��� ��������������
	CString	sData;		//��� ��� �������������� (����������, ������� � ��.)
	int	nStringNumber;	//����� ��������� ������ (��� ������ ������)
	int	nNumberLine;	//����� ������ ��������� ������ (��� ����� ��������)
	CString csModuleName;//��� ������ (�.�. �������� include ����������� �� ������ �������)
};

typedef CArray<CLexem,CLexem &> CLexemList;
//����� ��� �������� ����������� ������������
class CDefList
{
public:
	~CDefList();
	CDefList()
	{
		pParent=0;
	};
	CMapStringToPtr DefList;//�������� ������� ������
	CDefList *pParent;

	void SetParent(CDefList *p);
	void RemoveDef(CString &csName);
	BOOL HasDef(CString &csName);
	CLexemList *GetDef(CString &csName);
	void SetDef(CString &csName,CLexemList *);
	void SetDef(CString &csName,CString csValue);
};

/************************************************
CTranslateModule-���� �������� ��������� ����
����� ����� - ��������� Load() � TranslateModule(). 
������ ��������� ��������� ������������� ���������� � �������� 
������ ������������ ����, ������ ��������� ��������� ���������� 
(������� ����). � �������� ���������� � ��������� ������ ����������� 
������ "������" ����-���� � ���������� cByteCode.
*************************************************/
class CTranslateModule  
{
public:
	CTranslateModule();
	virtual ~CTranslateModule();

	//��������:
	CString csModuleName;//��� �������������� ������ (��� ������ ���������� ��� �������)
	void OnSetParent(CTranslateModule *pSetParent);

	int nSizeText;//������ ��������� ������
public:
	//�������� �����:
	CString Buffer;
public:
	//������������� ������ � ���������
	CLexemList LexemList;

	//��������� "��������"
private:
	CDefList *aDefList;
	BOOL bAutoDeleteDefList;
	int nModePreparing;
public:
	BOOL HasDef(CString &csName)
	{
		if(aDefList)
			return aDefList->HasDef(csName);
		else
			return 0;
	};

public:
	//������ � ���������� ��� �������� ������
	CString BUFFER;//����� - �������� ��� � ������� ��������

public:
	int nPos;//������� ������� ��������������� ������
	int nCurNumberLine;

	//������:
	void LoadFile(char *sFileName);
	void Load(CString s);
	//bool PrepareCode();

	void SaveToFile(char *sFileName);
	///bool LoadFromFile(char *sFileName);


public:
	void Clear();
	bool PrepareLexem();
	void SetError(int nErr,int nCurPos,CString s="");

public:
	void SkipSpaces(void);
	bool IsByte(char c);
	char GetByte();

	bool IsWord(void);
	CString GetWord(bool bOrigin=false,bool bGetPoint=false,CString *psOrig=0);
	
	bool IsNumber(void);
	CString GetNumber(void);
	
	bool IsString(void);
	CString GetString(void);
	
	bool IsDate(void);
	CString GetDate(void);

	bool IsEnd(void);
	void Block(void);

	int IsKeyWord(CString s);
	CString GetStrToEndLine(void);
	void PrepareFromCurrent(int nMode,CString csName="");

	void ToDebug();

public:
	static CMapStringToPtr	HashKeyWords;
	static void LoadKeyWords();
};

#endif // !defined(AFX_TRANSLATEMODULE_H__990B9717_1909_4AF6_936A_DEE5DAF38FA1__INCLUDED_)
