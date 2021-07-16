// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// SystemFunctions.h: interface for the SystemFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMFUNCTIONS_H__82B9C843_2174_4B12_AD05_5126D5C6D6B6__INCLUDED_)
#define AFX_SYSTEMFUNCTIONS_H__82B9C843_2174_4B12_AD05_5126D5C6D6B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
/*typedef CValue (*DEFFUNCTION)(CValue **);

//Структура элемента заголовков системных функций
extern struct SCallFunction
{
public:
	char *sName;
	int nCountParams;
	DEFFUNCTION Func;
	char *sShortDescription;
	char *sLongDescription;
}DefSystemFunctionsArray[];
*///определена в Value.h





//Декларирование системных функций


//*************************************
//--Реализация системных функций языка

//--Константы:
#define PageBreak CValue("\n\n")
#define LineBreak CValue("\n")
#define TabSymbol CValue("\t")

//#define Dim (int)sizeof



//*************************************
//--Функции и процедуры

//---Математические:
NUMBER Round(CValue &cValue,int Prec=0,int nMode=1);
CValue Int(CValue &cNumber);
NUMBER Log10(CValue &cValue);
NUMBER Ln(CValue &cValue);
CValue Max(CValue **p);
CValue Min(CValue **p);

//---Строковые:
int StrLen(CValue &cValue);
int IsBlankString(CValue &cValue);
CString TrimL(CValue &cValue);
CString TrimR(CValue &cValue);
CString TrimAll(CValue &cValue);
CString Left(CValue &cValue,int nCount);
CString Right(CValue &cValue,int nCount);
CString Mid(CValue &cValue,int nFirst);
CString Mid(CValue &cValue,int nFirst,int nCount);
int Find(CValue &cValue,CValue &cValue2);
int Find(CValue &cValue,CValue &cValue2,int nStart);
CString StrReplace(CValue &cSource,CValue &cValue1,CValue &cValue2);
int StrCountOccur(CValue &cSource,CValue &cValue1);
int StrLineCount(CValue &cSource);
CString StrGetLine(CValue &cValue,int nLine);
CString Upper(CValue &cSource);
CString Upper(CString cSource);
CString Lower(CValue &cSource);
CString Chr(int nCode);
int Asc(CValue &cSource);
CString OemToAnsi(CValue &cSource);
CString AnsiToOem(CValue &cSource);

//---Работа с датой

CValue CurDate(void);
CValue WorkingDate(CValue &cValue,int nMode);
CValue AddMonth(CValue &cData,int nMonthAdd=1);
CValue BegOfMonth(CValue &cData);
CValue EndOfMonth(CValue &cData);
CValue BegOfQuart(CValue &cData);
CValue EndOfQuart(CValue &cData);
CValue BegOfYear(CValue &cData);
CValue EndOfYear(CValue &cData);
CValue BegOfWeek(CValue &cData);
CValue EndOfWeek(CValue &cData);
int GetYear(CValue &cData);
int GetMonth(CValue &cData);
int GetDay(CValue &cData);
int GetWeekOfYear(CValue &cData);
int GetDayOfYear(CValue &cData);
int GetDayOfWeek(CValue &cData);
int GetQuartOfYear(CValue &cData);
CString PeriodStr(CValue &cData1,CValue &cData2);


//---Работа со временем
CValue CurrentTime(CValue &Ch,CValue &Min,CValue &Sec);
CValue CurrentTime(void);
CValue CurrentTime(CValue &Ch);
CValue CurrentTime(CValue &Ch,CValue &Min);

//---Преобразование типов 
//реализация в теле класса CValue
CValue Number(CValue cParam);
CValue String(CValue cParam);
CValue Date(CValue cParam);
CValue Date(CValue cParam,int &nYear,int &nMonth,int &nDay,BOOL bReturn);
CValue Date(int nYear,int nMonth,int nDay);


//*************************************
void Message(CValue cMessage,CValue cType);
void Message(CValue cMessage);
void DoMessageBox(CValue cMessage);
void SetStatus(CString s);
void ClearMessageWindow();
//
void SetError(CString Str);
void Raise();//ВызватьИсключение
CString GetErrorDescription();//ОписаниеОшибки
BOOL OpenForm(CString csName,CValue Param1,CValue Param2);


CValue EvalExp(CString csExp);
CString Template(CString csStr);


//тестовая функция
void FunctionUnitTest(void);

#endif // !defined(AFX_SYSTEMFUNCTIONS_H__82B9C843_2174_4B12_AD05_5126D5C6D6B6__INCLUDED_)
