// јвторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
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

//—писок ключевых слов
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


//определени€

//хранение одного примитива из исходного кода
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

	//тип лексемы:
	int	nType;
	//содержание лексемы:
	int		nData;		//номер служебного слова(KEYWORD) или символ разделитель(DELIMETER)
	CValue	vData;		//значение, если это константа или реальное им€ идентификатора
	CString	sData;		//или им€ идентификатора (переменной, функции и пр.)
	int	nStringNumber;	//номер исходного текста (дл€ вывода ошибок)
	int	nNumberLine;	//номер строки исходного текста (дл€ точек останова)
	CString csModuleName;//им€ модул€ (т.к. возможны include подключени€ из разных модулей)
};

typedef CArray<CLexem,CLexem &> CLexemList;
//класс дл€ хранени€ определений пользовател€
class CDefList
{
public:
	~CDefList();
	CDefList()
	{
		pParent=0;
	};
	CMapStringToPtr DefList;//содержит массивы лексем
	CDefList *pParent;

	void SetParent(CDefList *p);
	void RemoveDef(CString &csName);
	BOOL HasDef(CString &csName);
	CLexemList *GetDef(CString &csName);
	void SetDef(CString &csName,CLexemList *);
	void SetDef(CString &csName,CString csValue);
};

/************************************************
CTranslateModule-этап парсинга исходного кода
“очка входа - процедуры Load() и TranslateModule(). 
ѕерва€ процедура выполн€ет инициализацию переменных и загрузку 
текста выполн€емого кода, втора€ процедура выполн€ет трансл€цию 
(парсинг кода). ¬ качестве результата в структуре класса заполн€етс€ 
массив "сырого" байт-кода в переменной cByteCode.
*************************************************/
class CTranslateModule  
{
public:
	CTranslateModule();
	virtual ~CTranslateModule();

	//атрибуты:
	CString csModuleName;//им€ компилируемого модул€ (дл€ вывода информации при ошибках)
	void OnSetParent(CTranslateModule *pSetParent);

	int nSizeText;//размер исходного текста
public:
	//исходный текст:
	CString Buffer;
public:
	//промежуточный массив с лексемами
	CLexemList LexemList;

	//ѕоддержка "дефайнов"
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
	//методы и переменные дл€ парсинга текста
	CString BUFFER;//буфер - исходный код в верхнем регистре

public:
	int nPos;//текуща€ позици€ обрабатываемого текста
	int nCurNumberLine;

	//методы:
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
