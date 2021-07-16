// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// SystemFunctions.cpp: implementation of the SystemFunctions class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "spell.h"
#include "SystemFunctions.h"
#ifndef SMALL_TRANSLATE
	#include "../MetadataTree.h"
	#include "../InputDialog.h"
	#include "../InputDate.h"
	#include "../InputNumeric.h"
	#include "../InputPeriod.h"
#endif
#include "ValueArray.h"
#include "../MsgBox/timedmsgbox.h"
#include "../MainFrm.h"
#include "../OpenForm.h"

int glVirtKey=0;
extern CString sCurError;
extern CString csTitleWnd;
extern CString csUserPath;//каталог пользователя для записи настроек
extern CString csUserName;
extern CString csUserPassword;
extern int bSingleMode;//Монопольный режим
extern CMainFrame *pMainFrame;
BOOL bHasError=0;

CValue WorkDate;
CString GetHash(CString csPassword);
CString GetPassword(CString csHash);
int RegisterEvent(CString csEventName);
int NotifyEvent(CString csEventName, CValue **p);
CMap <void*,void*,CString,CString&> *GetRegistersObjectsArray();
 

#ifdef SMALL_TRANSLATE
	int nReturnStatus=1;//Статус возврата
#endif


#define DO_KEY(x)\
		aKeyList[#x]=(void*)x;

void LoadKeyString(CMapStringToPtr &aKeyList)
{
		DO_KEY(VK_BACK)
		DO_KEY(VK_TAB)

		DO_KEY(VK_CLEAR)
		DO_KEY(VK_RETURN)

		DO_KEY(VK_SHIFT)
		DO_KEY(VK_CONTROL)
		DO_KEY(VK_MENU)
		DO_KEY(VK_PAUSE)
		DO_KEY(VK_CAPITAL)

		DO_KEY(VK_KANA)
		DO_KEY(VK_HANGEUL)
		DO_KEY(VK_HANGUL)
		DO_KEY(VK_JUNJA)
		DO_KEY(VK_FINAL)
		DO_KEY(VK_HANJA)
		DO_KEY(VK_KANJI)

		DO_KEY(VK_ESCAPE)

		DO_KEY(VK_CONVERT)
		DO_KEY(VK_NONCONVERT)
		DO_KEY(VK_ACCEPT)
		DO_KEY(VK_MODECHANGE)

		DO_KEY(VK_SPACE)
		DO_KEY(VK_PRIOR)
		DO_KEY(VK_NEXT)
		DO_KEY(VK_END)
		DO_KEY(VK_HOME)
		DO_KEY(VK_LEFT)
		DO_KEY(VK_UP)
		DO_KEY(VK_RIGHT)
		DO_KEY(VK_DOWN)
		DO_KEY(VK_SELECT)
		DO_KEY(VK_PRINT)
		DO_KEY(VK_EXECUTE)
		DO_KEY(VK_SNAPSHOT)
		DO_KEY(VK_INSERT)
		DO_KEY(VK_DELETE)
		DO_KEY(VK_HELP)

		DO_KEY(VK_LWIN)
		DO_KEY(VK_RWIN)
		DO_KEY(VK_APPS)

		DO_KEY(VK_NUMPAD0)
		DO_KEY(VK_NUMPAD1)
		DO_KEY(VK_NUMPAD2)
		DO_KEY(VK_NUMPAD3)
		DO_KEY(VK_NUMPAD4)
		DO_KEY(VK_NUMPAD5)
		DO_KEY(VK_NUMPAD6)
		DO_KEY(VK_NUMPAD7)
		DO_KEY(VK_NUMPAD8)
		DO_KEY(VK_NUMPAD9)
		DO_KEY(VK_MULTIPLY)
		DO_KEY(VK_ADD)
		DO_KEY(VK_SEPARATOR)
		DO_KEY(VK_SUBTRACT)
		DO_KEY(VK_DECIMAL)
		DO_KEY(VK_DIVIDE)
		DO_KEY(VK_F1)
		DO_KEY(VK_F2)
		DO_KEY(VK_F3)
		DO_KEY(VK_F4)
		DO_KEY(VK_F5)
		DO_KEY(VK_F6)
		DO_KEY(VK_F7)
		DO_KEY(VK_F8)
		DO_KEY(VK_F9)
		DO_KEY(VK_F10)
		DO_KEY(VK_F11)
		DO_KEY(VK_F12)
		DO_KEY(VK_F13)
		DO_KEY(VK_F14)
		DO_KEY(VK_F15)
		DO_KEY(VK_F16)
		DO_KEY(VK_F17)
		DO_KEY(VK_F18)
		DO_KEY(VK_F19)
		DO_KEY(VK_F20)
		DO_KEY(VK_F21)
		DO_KEY(VK_F22)
		DO_KEY(VK_F23)
		DO_KEY(VK_F24)

		DO_KEY(VK_NUMLOCK)
		DO_KEY(VK_SCROLL)


}

int GetKeyCode(CString csKey)
{
	csKey=mUpper(csKey);
	if(csKey.Left(3)!="VK_")
		csKey=CString("VK_")+mUpper(csKey);

	static CMapStringToPtr aKeyList;
	if(aKeyList.GetCount()==0)
	{
		LoadKeyString(aKeyList);
	}
	return (int)aKeyList[csKey];
}
/*
void CALLBACK MessageBoxTimer(HWND hwnd, UINT uiMsg, UINT idEvent, DWORD dwTime)
{
	PostMessage(hwnd,WM_QUIT,0,0);
}
*/
//TODO: все функции нужно переписать с учетом специфики вызова!
//////////////////////////////////////////////////////////////////////
// системные функции языка
//////////////////////////////////////////////////////////////////////
//Определения

#define PROCEDURE0(x) CValue x(CValue **p){x();return 1;};
#define PROCEDURE1(x) CValue x(CValue **p){x(*p[0]);return 1;};
#define PROCEDURE2(x) CValue x(CValue **p){x(*p[0],*p[1]);return 1;};
#define PROCEDURE3(x) CValue x(CValue **p){x(*p[0],*p[1],*p[2]);return 1;};

#define FUNCTION0(x) CValue x(CValue **p){return x();};
#define FUNCTION1(x) CValue x(CValue **p){return x(*p[0]);};
#define FUNCTION2(x) CValue x(CValue **p){return x(*p[0],*p[1]);};
#define FUNCTION3(x) CValue x(CValue **p){return x(*p[0],*p[1],*p[2]);};
#define FUNCTION4(x) CValue x(CValue **p){return x(*p[0],*p[1],*p[2],*p[3]);};
#define FUNCTION5(x) CValue x(CValue **p){return x(*p[0],*p[1],*p[2],*p[3],*p[4]);};

#define SFUNCTION1(x) CValue x(CValue **p){return String(x(*p[0]));};
#define SFUNCTION2(x) CValue x(CValue **p){return String(x(*p[0],*p[1]));};
#define SFUNCTION3(x) CValue x(CValue **p){return String(x(*p[0],*p[1],*p[2]));};

//*************************************
PROCEDURE2(Message);
PROCEDURE1(SetStatus);
PROCEDURE0(ClearMessageWindow);

//---Математические:
//FUNCTION3(Round);
FUNCTION1(Int);
FUNCTION1(Log10);
FUNCTION1(Ln);

//---Строковые:
FUNCTION1(StrLen);
FUNCTION1(IsBlankString);
//SFUNCTION1(TrimL);
//SFUNCTION1(TrimR);
SFUNCTION2(Left);
SFUNCTION2(Right);
SFUNCTION3(StrReplace);
FUNCTION2(StrCountOccur);
FUNCTION1(StrLineCount);

SFUNCTION1(Upper);
SFUNCTION1(Lower);

SFUNCTION3(Mid);
FUNCTION3(Find);

SFUNCTION2(StrGetLine);
SFUNCTION1(Chr);
FUNCTION1(Asc);
SFUNCTION1(OemToAnsi);
SFUNCTION1(AnsiToOem);




//---Преобразование типов 
FUNCTION1(Number);
FUNCTION1(String);
//FUNCTION3(Date);


//---Работа с датой
FUNCTION0(CurDate);
FUNCTION2(WorkingDate);

FUNCTION2(AddMonth);
FUNCTION1(BegOfMonth);
FUNCTION1(EndOfMonth);
FUNCTION1(BegOfQuart);
FUNCTION1(EndOfQuart);
FUNCTION1(BegOfYear);
FUNCTION1(EndOfYear);
FUNCTION1(BegOfWeek);
FUNCTION1(EndOfWeek);
FUNCTION1(GetYear);
FUNCTION1(GetMonth);
FUNCTION1(GetDay);
FUNCTION1(GetWeekOfYear);
FUNCTION1(GetDayOfYear);
FUNCTION1(GetDayOfWeek);
FUNCTION1(GetQuartOfYear);
SFUNCTION2(PeriodStr);

//---Работа со временем
CValue CurrentTime(CValue **p)
{
	CValue cRes;
    CTime timeNow=CTime ::GetCurrentTime();
	*p[0]=timeNow.GetHour();
	*p[1]=timeNow.GetMinute();
	*p[2]=timeNow.GetSecond();
	cRes=String(timeNow.Format("%H:%M:%S"));
	return cRes;
}
//---Преобразование типов
CValue Number2(CValue **p)
{
	CValue Ret;
	Ret.nType=TYPE_NUMBER;
	Ret.fData=p[0]->GetNumber();
	return Ret;
}
CValue String2(CValue **p)
{
	CValue Ret;
	Ret.nType=TYPE_STRING;
	Ret.sData=p[0]->GetString();
	return Ret;
}
CValue Date3(CValue **p)
{
	if(p[1]->nType==TYPE_EMPTY)
		return Date(*p[0]);
	else
		return Date(p[0]->GetNumber(), p[1]->GetNumber(), p[2]->GetNumber());
}


//Специальные

CValue CreateObject(CString Str,CValue Param);
FUNCTION2(CreateObject);


 
CValue TypeValue(CValue **p)//ТипЗначения
{
	return p[0]->GetType();
}

CValue ValueTypeStr(CValue **p)//ТипЗначенияСтр
{
	return String(p[0]->GetTypeString());
}

CValue TrimAll(CValue **p)//СокрЛП
{
	CString csStr=p[0]->GetString();
	csStr.TrimRight();
	csStr.TrimLeft();
	return String(csStr);
}

CValue Dim(CValue **p)//Разм
{
	if(p[0]->GetType()==TYPE_ARRAY)//это массив
	{
		int nSize=((CValueArray*)p[0]->pRef)->GetSizeArray();
		if(p[1]->nType!=TYPE_EMPTY)
			((CValueArray*)p[0]->pRef)->SetSizeArray(p[1]->GetNumber());
		return nSize;
	}
	else
	{
		return 0;
	}
}
CValue DimStr(CValue **p)//РазмСтр
{
	if(p[0]->GetType()==TYPE_ARRAY)//это массив
	{
		int nSize=((CValueArray*)p[0]->pRef)->GetSizeStrArray();
		if(p[1]->nType!=TYPE_EMPTY)
			((CValueArray*)p[0]->pRef)->SetSizeStrArray(p[1]->GetNumber());
		return nSize;
	}
	else
	{
		return 0;
	}
}

CValue ValueToString(CValue **p)//ЗначениеВСтроку
{
	CString csStr;
	p[0]->SaveToString(csStr);
	return String(csStr);
}

CValue ValueFromString(CValue **p)//ЗначениеИзСтроки
{
	CValue Ret; 
	Ret.LoadFromString(p[0]->GetString());
	return Ret;
} 

void WritoToFile(CString csCFile,CString Str);
char *LoadFromFileBin(CString sFileName,int &size);

void ValueToFile(CValue *Val,CString csFileName)
{
	CString csStr;
	Val->SaveToString(csStr);
	WritoToFile(csFileName,csStr);
}
CValue ValueFromFile(CString csFileName)
{
	int size; 
	char *s=LoadFromFileBin(csFileName,size);
	CString csStr=s;
	CValue Ret;
	Ret.LoadFromString(csStr);
	if(s)
		delete []s;
	return Ret;
}

CValue ValueToFile(CValue **p)//ЗначениеВФайл
{
	ValueToFile(p[1],p[0]->GetString());
	return 1;
}
CValue ValueFromFile(CValue **p)//ЗначениеИзФайла
{
	*p[1]=ValueFromFile(p[0]->GetString());
	return 1;
}


CValue EmptyValue(CValue **p)//ПустоеЗначение
{
	CValue Ret=p[0]->IsEmpty();
	return Ret;
}

CValue AttachRegisterObject(CValue **p)//ЗарегистрироватьОбъект
{
	AttachRegisterObject(p[0]->GetString(),p[1]->GetString());
	return CValue();
}
CValue UnRegisterObject(CValue **p)//ОтменитьРегистрациюОбъекта
{
	UnRegisterObject(p[0]->GetString());
	return CValue();
}


CValue SetReadOnly(CValue **p)//УстановитьТолькоЧтение
{
	p[0]->bReadOnly=p[1]->GetNumber();
	return CValue();
}


extern CString csCurrentExeDir;
extern CString csIBDir;

//Среда исполнения
CValue BinDir(CValue **p)//КаталогПрограммы
{
	csCurrentExeDir.TrimRight('\\');
	return String(csCurrentExeDir+"\\");
}
CValue IBDir(CValue **p)//КаталогИБ
{
	csIBDir.TrimRight('\\');
	return String(csIBDir+"\\");
}
CValue UserDir(CValue **p)//КаталогПользователя
{
	csUserPath.TrimRight('\\');
	CValue rez=String(csUserPath+"\\");
	if(p[0]->nType)
	{
		csUserPath=p[0]->GetString();
		csUserPath.TrimRight('\\');
	}
	return rez;
}
CValue TempFilesDir(CValue **p)//КаталогВременныхФайлов
{
	CString Str;
	GetTempPath(MAX_PATH,Str.GetBuffer(MAX_PATH));
	Str.ReleaseBuffer();
	Str.TrimRight('\\');
	return String(Str+"\\");
}

CValue UserName(CValue **p)//ИмяПользователя
{
	CValue rez=String(csUserName);
	if(p[0]->nType)
	{
		csUserName=p[0]->GetString();
	}
	return rez;
}
CValue UserPassword(CValue **p)//ПарольПользователя
{
	CValue rez=String(GetHash(csUserPassword));
	if(p[0]->nType)
	{
		csUserPassword=p[0]->GetString();
	}
	return rez;
}
CValue SingleMode(CValue **p)//МонопольныйРежим
{
	return bSingleMode;
}
CValue GeneralLanguage(CValue **p)//ОсновнойЯзык
{
	return 1;
}


CValue ComputerName(CValue **p)//ИмяКомпьютера
{
	CString csName;
	unsigned long nLength=MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(csName.GetBuffer(MAX_COMPUTERNAME_LENGTH + 1),&nLength);
	csName.ReleaseBuffer();
	return String(csName);
}
CValue RunApp(CValue **p)//ЗапуститьПриложение
{
	if(p[2]->nType==TYPE_EMPTY||p[2]->GetNumber()==0)
		ShellExecute(NULL,NULL,p[0]->GetString(),p[1]->GetString(),NULL,SW_SHOW);
	else
		ShellExecute(NULL,NULL,p[0]->GetString(),p[1]->GetString(),NULL,SW_HIDE);
	return CValue();
}

CValue InputPeriod(CValue **p)//ВвестиПериод
{
#ifndef SMALL_TRANSLATE
	CInputPeriod dlg;

	int nYear, nMonth, nDay;
	int nHour=0;
	int nMinute=0;
	int nSecond=0;
	
	p[0]->FromDate(nYear,nMonth,nDay);

	SYSTEMTIME sTime;
	sTime.wYear		=nYear;
	sTime.wMonth	=nMonth;
	sTime.wDay		=nDay;
	sTime.wHour		=nHour;
	sTime.wMinute	=nMinute;
	sTime.wSecond	=nSecond;
	dlg.m_DateBeg=CTime(sTime);
	
	p[1]->FromDate(nYear,nMonth,nDay);
	sTime.wYear		=nYear;
	sTime.wMonth	=nMonth;
	sTime.wDay		=nDay;
	sTime.wHour		=nHour;
	sTime.wMinute	=nMinute;
	sTime.wSecond	=nSecond;
	dlg.m_DateEnd=CTime(sTime);

	dlg.m_Date1=CTime(sTime);
	
	dlg.csTitle=p[2]->GetString();
	dlg.m_RadioInterval=3;

	if(dlg.DoModal()==IDOK)
	{
		CTime ctTime=dlg.m_DateBeg1;
		
		nYear =ctTime.GetYear();
		nMonth=ctTime.GetMonth();
		nDay  =ctTime.GetDay();
		p[0]->dData=Date(nYear,nMonth,nDay);
		
		ctTime=dlg.m_DateEnd1;
		nYear =ctTime.GetYear();
		nMonth=ctTime.GetMonth();
		nDay  =ctTime.GetDay();
		p[1]->dData=Date(nYear,nMonth,nDay);
		return 1;
	}
	
#endif
	return 0;
}

//Диалог ввода числа
CValue InputNumeric(CValue **p)//ВвестиЧисло
{
#ifndef SMALL_TRANSLATE
	CInputNumeric dlg;
	dlg.csValue=String(CValue(p[0]->GetNumber()));
	dlg.csTitle=p[1]->GetString();
	dlg.nLength=p[2]->GetNumber();
	dlg.nPrec=p[3]->GetNumber();
	dlg.nTimer=p[4]->GetNumber();
	if(dlg.DoModal()==IDOK)
	{
		*p[0]=String(dlg.csValue);
		return 1;
	}
	else
	{
		if(dlg.nTimer==-1)
			return -1;
	}
#endif
	return 0;
}

//Диалог ввода строки
CValue InputDialog(CValue **p)//ВвестиСтроку
{
#ifndef SMALL_TRANSLATE
	CInputDialog dlg;
	dlg.csString=p[0]->GetString();
	dlg.csTitle=p[1]->GetString();
	dlg.nLimit=p[2]->GetNumber();
	dlg.nMultiLine=p[3]->GetNumber();
	dlg.nTimer=p[4]->GetNumber();
	if(dlg.DoModal()==IDOK)
	{
		*p[0]=String(dlg.csString);
		return 1;
	}
	else
	{
		if(dlg.nTimer==-1)
			return -1;
	}
#endif
	return 0;
}

//Диалог ввода даты
CValue InputDate(CValue **p)//ВвестиДату
{
#ifndef SMALL_TRANSLATE
	CInputDate dlg;
	int nYear, nMonth, nDay;
	int nHour=0;
	int nMinute=0;
	int nSecond=0;
	
	p[0]->FromDate(nYear,nMonth,nDay);

	SYSTEMTIME sTime;
	sTime.wYear		=nYear;
	sTime.wMonth	=nMonth;
	sTime.wDay		=nDay;
	sTime.wHour		=nHour;
	sTime.wMinute	=nMinute;
	sTime.wSecond	=nSecond;
	dlg.m_Date2=CTime(sTime); 
	
	dlg.csTitle	=p[1]->GetString();
	dlg.nTimer	=p[2]->GetNumber();
		
	if(dlg.DoModal()==IDOK)
	{
		CTime ctTime=dlg.m_Date2;
		
		nYear =ctTime.GetYear();
		nMonth=ctTime.GetMonth();
		nDay  =ctTime.GetDay();
		p[0]->dData=Date(nYear,nMonth,nDay);
		return 1;
	}
	else
	{
		if(dlg.nTimer==-1)
			return -1;
	}

#endif
	return 0;
}

//Диалог ввода значения
CValue InputValue(CValue **p)//ВвестиЗначение
{
#ifndef SMALL_TRANSLATE
	CString csType=p[2]->GetString();
	int ret;
	CValue *p1[4];
	p1[0]=p[0];
	p1[1]=p[1];
	p1[2]=p[3];
	p1[3]=p[4];
	csType.MakeUpper();
	if (csType=="ЧИСЛО")
		ret=InputNumeric(p1);
	else if (csType=="СТРОКА")
		ret=InputDialog(p1);
	else if (csType=="ДАТА")
	{
		p1[2]=p[4];
		ret=InputDate(p1);
	}
	else
		ret=0;
	p[0]=p1[0];
	return ret;
#endif
	return 0;
}

#define PARSE_STRING(x,y)\
if(csStr.Find(x)>=0)\
{\
	nStrMode=nLang;\
	nMode=y;\
}
CValue DoQueryBox(CValue **p)//Вопрос
{
	int nStrMode=0;
	int nMode=int(p[1]->GetNumber());
	if(nMode==0)
	{
		CString csStr=mUpper(p[1]->GetString());
		csStr.Replace(" ","");

		int nLang=1;//английский
		PARSE_STRING("OK",MB_OK)
		PARSE_STRING("OK+CANCEL",MB_OKCANCEL)
		PARSE_STRING("ABORT+RETRY+IGNORE",MB_ABORTRETRYIGNORE)
		PARSE_STRING("YES+NO",MB_YESNO)
		PARSE_STRING("YES+NO+CANCEL",MB_YESNOCANCEL)
		PARSE_STRING("RETRY+CANCEL",MB_RETRYCANCEL)

		nLang=2;//русский
		PARSE_STRING("ОК",MB_OK)
		PARSE_STRING("ОК+ОТМЕНА",MB_OKCANCEL)
		PARSE_STRING("СТОП+ПОВТОР+ПРОПУСТИТЬ",MB_ABORTRETRYIGNORE)
		PARSE_STRING("ДА+НЕТ",MB_YESNO)
		PARSE_STRING("ДА+НЕТ+ОТМЕНА",MB_YESNOCANCEL)
		PARSE_STRING("ПОВТОР+ОТМЕНА",MB_RETRYCANCEL)

	}
	if(nMode>5)
		nMode=0;

	HWND hWnd=0;
	if(AfxGetMainWnd())
		hWnd=AfxGetMainWnd()->m_hWnd;

	BOOL	stoppedByUser;
	int nRet = CDlgTimedMessageBox::TimedMessageBox(nMode|MB_ICONQUESTION|MB_APPLMODAL, 
			p[0]->GetString(), 
			"Вопрос", 
			1000*p[2]->GetNumber(), 0, 
			p[3]->GetString(),//"\nin the next %lu sec !", 
			hWnd, &stoppedByUser);

	if(nRet<1||stoppedByUser==0)
		nRet=0;
	if(nStrMode)
	{
		if(nStrMode==1)
		{
			CString MasStr[8]={"Timeout","OK","Cancel","Abort","Retry","Ignore","Yes","No"};
			return String(MasStr[nRet%8]);
		}
		else
		{
			CString MasStr[8]={"Таймаут","ОК","Отмена","Стоп","Повтор","Пропустить","Да","Нет"};
			return String(MasStr[nRet%8]);
		}
	}
	if(stoppedByUser==0)
		return -1;

	return nRet;
}
CValue DoMessageBox(CValue **p)
{
	BOOL	stoppedByUser;
	UINT	erg=0;

	HWND hWnd=0;
	if(AfxGetMainWnd())
		hWnd=AfxGetMainWnd()->m_hWnd;
	if(hWnd)
	{
		erg  = CDlgTimedMessageBox::TimedMessageBox(MB_OK|MB_ICONHAND, 
				p[0]->GetString(), 
				"Предупреждение", 
				1000*p[1]->GetNumber(), IDOK, 
				p[2]->GetString(),//"\nin the next %lu sec !", 
				hWnd, &stoppedByUser);
	}
	return 0;
}

#define	FORM_PARAM_COUNT	8

CValue OpenForm(CValue **p,int nMode)//ОткрытьФорму
{
	static int nUseModal=0;
	int _nUseModal=nUseModal;
	if(1==nMode)
		nUseModal=1;//устанавливаем режим открытия всех последующих форм также модальными
	if(nUseModal)
		nMode=1;

	

	int nRes=0;
	if(p[0]->nType==TYPE_REFFER&&p[0]->GetType()>=10)
	{
		try 
		{
			//*p[5]=GetRunContext();//контекст открытия
			nRes=p[0]->CallFunction("ОткрытьФормуОбъекта",p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
		}
		catch(CTranslateError *)
		{
			Message(CString("Объект ")+p[0]->GetTypeString()+" не поддерживает формы ("+CTranslateError::GetErrorMessage()+")");
		}
	}
	else
	{
		
		if(p[2]->GetString().IsEmpty() && !p[0]->GetString().IsEmpty() && IsRegisterObject(p[0]->GetString()))
		{
			CValue Val;
			Val.CreateObject(p[0]->GetString());
			//*p[5]=GetRunContext();//контекст открытия
			nRes=Val.CallFunction("ОткрытьФормуОбъекта",p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
		}
		else
		{
			CValue vContext;
			nRes=OpenFormExt(p[0]->GetString(),vContext,*p[1],p[2]->GetString(),p[3]->GetString(),nMode,*p[5],p[6]->GetNumber(),*p[7]);
			*p[1]=vContext;
		}
	}
	nUseModal=_nUseModal;
	return nRes;
}

CValue OpenForm(CValue **p)//ОткрытьФорму
{
	return OpenForm(p,0);
}

CValue OpenFormModal(CValue **p)//ОткрытьФормуМодально
{
	return OpenForm(p,1);
}
CValue OpenFormEdit(CValue **p)//ОткрытьФормуРедактирования
{
#ifndef SMALL_TRANSLATE
	CValue vContext;
	int nRes=OpenFormExt(p[0]->GetString(),vContext,*p[1],p[2]->GetString(),p[3]->GetString(),2);
	*p[1]=vContext;
	return nRes;
#endif
	return 0;

}
/*
ОткрытьФорму(<Документ>,<КонтекстФормы>,<РежимПросмотра>)
ОткрытьФорму(<ОписательОбъекта>,<КонтекстФормы>,<ДокументОснование>)
ОткрытьФорму(<ОписательОбъекта>,<КонтекстФормы>)
ОткрытьФорму(<ОписательОбъекта>,<КонтекстФормы>,<ИмяФайла>)
ОткрытьФорму(<ОписательОбъекта>,,,<УстНаДату>)
ОткрытьФорму(<ОписательОбъекта>,<КонтекстФормы>,<ГруппаРодитель>,<ФлагГруппы>,<ЭлементВладелец>)

OpenFormExt(CString csFormName,CValue &vContext, CValue Param,CString csFileName,CString csFormID,int nMode,  CValue vChoiceContext,int nChoiceMode,CMicroForm* pControl,CWnd *pParent)
OpenFormExt(p[0]->GetString(),vContext,*p[1],p[2]->GetString(),p[3]->GetString(),nMode);

ОткрытьПодбор(<Объект>,<ИмяФормы>,<КонтекстФормы>,<ФлагМножВыбора>,<ТекЗнач>)
*/

CValue OpenPermanentChoice(CValue **p)// ОткрытьПодбор
{
	//приводим к универсальному типу вызова открытия формы
	CValue v2[FORM_PARAM_COUNT];
	CValue *p2[FORM_PARAM_COUNT];
	for(int i=0;i<FORM_PARAM_COUNT;i++)
		p2[i]=&v2[i];

	CString Str;
	Str=p[0]->GetString();
	if(!p[1]->GetString().IsEmpty())
		Str=Str+"."+p[1]->GetString();
	*p2[0]=String(Str);
	p2[1]=p[2];//контекст

	v2[5]=GetRunContext();//контекст открытия
	p2[6]=p[3];//Режим подбора
	p2[7]=p[4];//ТекЗнач
	

	return OpenForm(p2,0);
}


/*
CValue GetMetaTree(CValue **p)//ПолучитьДеревоМетаданных
{
#ifndef SMALL_TRANSLATE
	
	CMetadataTree* pMetaTree=0;
	pMetaTree=AfxGetMetadataTree();
	if(!pMetaTree)
		Error("Дерево метаданных недоступно");

	CValue vRes2;


	vRes2.CreateObject("ДеревоЗначений");
	((CValueTreeCtrl*)vRes2.pRef)->SetTree(&pMetaTree->GetTreeCtrl());
	((CValueTreeCtrl*)vRes2.pRef)->pMetaTree=pMetaTree;

	return vRes2;
#endif
	return 0;
}
*/

CValue ReturnStatus(CValue **p)//СтатусВозврата;
{
	CValue Ret=nReturnStatus;
	if(p[0]->nType)
		nReturnStatus=p[0]->GetNumber();
	return Ret;
}

CByteCode *AfxGetCurrentByteCode();
CValue Raise(CValue **p)//Ошибка
{
	afxErrorPlace.Reset();//инициализация места ошибки
	afxErrorPlace.pSkipByteCode=AfxGetCurrentByteCode();//возвращаемся назад в вызываемый модуль (если он есть)

	//AfxPopBackErrorByteCode();//возвращаемся назад в вызываемый модуль (если он есть)
	if(p[0]->nType!=TYPE_EMPTY)
		Error(p[0]->GetString().GetBuffer(0));
	else
		Error("Ошибка выполнения!");
	return CValue();
}


CValue GetKeyState(CValue **p)//СостояниеКлавиши
{
	if(p[0]->IsEmpty())
		Error("Не задан код клавиши");
	int nKey=p[0]->GetNumber();
	if(p[0]->GetType()==TYPE_STRING)
	{
		nKey=GetKeyCode(p[0]->GetString());
		if(nKey==0)
			Error("Неправильно задано наименование клавиши");
	}
	BOOL bRes=((GetKeyState(nKey) & (1 << (sizeof(SHORT)*8-1))) != 0 );
	return bRes;//GetAsyncKeyState(nKey);
}



CValue GetTickCount(CValue **p)//_GetPerformanceCounter
{
	return GetTickCount();
}



CMap <UINT,UINT,CValue,CValue&> aTimerList;//таймер -> модуль
CMap <UINT,UINT,CString,CString&> aTimerStr;//таймер -> функция
CMapStringToPtr aStrToTimer;//функция -> таймер

void DeleteTimer(CString Str)
{
	UINT m_idTimer=(UINT)aStrToTimer[mUpper(Str)];
	if(m_idTimer)
		::KillTimer(NULL, m_idTimer);
}
extern BOOL bDebugLoop;
extern BOOL bDebugStopLine;
extern int nCurrentNumberStopContext;
extern BOOL bUseDebug;

void CALLBACK GlobalTimerProc(HWND hwnd, UINT uiMsg, UINT_PTR m_idTimer, DWORD t)
{
	if(bUseDebug)
	if(bDebugLoop)//||bDebugStopLine||nCurrentNumberStopContext)
		return;
	CValue vContext=aTimerList[m_idTimer];
	CString Str=aTimerStr[m_idTimer];
	if(!Str.IsEmpty())
	{
		Str=Str.Mid(Str.Find(":")+1);
		if(!Str.IsEmpty())
		if(vContext.GetType()==100)
		{
			try
			{
				vContext.CallFunction(Str);
			}
			catch(CTranslateError *)
			{
				Message(CTranslateError::GetErrorMessage());
			}
			return;
		}
		DeleteTimer(aTimerStr[m_idTimer]);
	}
}

CValue SetTimer(CValue **p)//УстановитьТаймер
{
	CString Str;
	CValue vContext=*afxCurrentRunModule->pByteCode->aExternValue[0];//GetAttribute(0);
	Str.Format("%d:%s",(UINT)vContext.pRef,p[0]->GetString());
	DeleteTimer(Str);
	int nTime=p[1]->GetNumber();
	if(nTime>0)
	{
		UINT m_idTimer = ::SetTimer(NULL, (UINT)vContext.pRef, nTime, (TIMERPROC) GlobalTimerProc);
		if(m_idTimer)
		{
			aTimerList[m_idTimer]=vContext;
			aTimerStr[m_idTimer]=Str;
			aStrToTimer[mUpper(Str)]=(void*)m_idTimer;
			//vContext.CallFunction(Str);
			return 1;
		}
	}
	return 0;
}
CValue DelTimer(CValue **p)//УдалитьТаймер
{
	CString Str;
	CValue vContext=*afxCurrentRunModule->pByteCode->aExternValue[0];//GetAttribute(0);
	Str.Format("%d:%s",(UINT)vContext.pRef,p[0]->GetString());
	DeleteTimer(Str);
	return 1;
}



#ifndef SMALL_TRANSLATE
#include "..\MessageBar.h"
extern CMessageBar *pMessageWindow;
extern CStatusBar *pStatusBar;
#endif

#include <afxdisp.h>

//*************************************
char *apMonth[13]={"",
"Январь",
"Февраль",
"Март",
"Апрель",
"Май",
"Июнь",
"Июль",
"Август",
"Сентябрь",
"Октябрь",
"Ноябрь",
"Декабрь",
};



//*************************************
//--Реализация системных функций языка


//*************************************
//--Функции и процедуры

#define  MAX_PREC	15
//---Математические:
//TODO - VarR8Round
CValue Round(CValue **p)
{
	NUMBER fNumber=p[0]->GetNumber();
	int Prec=p[1]->GetNumber();
	int nMode=1;
	if(p[2]->nType!=TYPE_EMPTY)
		nMode=p[2]->GetNumber();
	if(Prec>MAX_PREC)
		Prec=MAX_PREC;

	__int64 nDelta=0;
	if(Prec>0)
	{
		nDelta=__int64(fNumber);
		fNumber=fNumber-nDelta;
	}

	__int64 N=__int64(fNumber*pow(10,Prec+1));
	//округление - в зависимости от метода
	if(nMode)
	{
		if(N%10>=5)
			N=N/10+1;
		else
			N=N/10;
	}
	else
	{
		if(N%10>=6)
			N=N/10+1;
		else
			N=N/10;
	}
	return CValue(nDelta)+(N/pow(10,Prec));
}

CValue Int(CValue &cNumber)
{
	return int(cNumber.GetNumber());
}

CValue Max(CValue **p)
{
	int i=1;
	CValue *pRes=p[0];
	while(p[i]->nType!=TYPE_EMPTY)
	{
		if(p[i]->fData>pRes->fData)
			pRes=p[i];
		i++;
	}
	return *pRes;
};

CValue Min(CValue **p)
{
	int i=1;
	CValue *pRes=p[0];
	while(p[i]->nType!=TYPE_EMPTY)
	{
		if(p[i]->fData<pRes->fData)
			pRes=p[i];
		i++;
	}
	return *pRes;
};


NUMBER Log10(CValue &cValue)
{
	NUMBER fNumber=cValue.GetNumber();
	return log10(fNumber);
}

NUMBER Ln(CValue &cValue)
{
	NUMBER fNumber=cValue.GetNumber();
	return log(fNumber);
}

//---Строковые:
int StrLen(CValue &cValue)
{
	return cValue.GetString().GetLength();
}
int IsBlankString(CValue &cValue)
{
	CString csStr=cValue.GetString();
	csStr.TrimRight();
	return csStr.IsEmpty();
}
/*CString TrimL(CValue &cValue)
{
	CString csStr=cValue.GetString();
	csStr.TrimLeft();
	return csStr;
}
CString TrimR(CValue &cValue)
{
	CString csStr=cValue.GetString();
	csStr.TrimRight();
	return csStr;
}
*/
CString Left(CValue &cValue,int nCount)
{
	CString csStr=cValue.GetString();
	return csStr.Left(nCount);
}
CString Right(CValue &cValue,int nCount)
{
	CString csStr=cValue.GetString();
	return csStr.Right(nCount);
}

CString Mid(CValue &cValue,int nFirst,int nCount)
{
	CString csStr=cValue.GetString();
	if(nCount==0)
		return csStr.Mid(nFirst-1);
	else
		return csStr.Mid(nFirst-1,nCount);
}
int Find(CValue &cValue,CValue &cValue2,int nStart)
{
	if(nStart<1)
		nStart=1;
	CString csStr=cValue.GetString();
	return 1+csStr.Find(cValue2.GetString(),nStart-1);
}
CString StrReplace(CValue &cSource,CValue &cValue1,CValue &cValue2)
{
	CString csSource=cSource.GetString();
	CString csStr1=cValue1.GetString();
	CString csStr2=cValue2.GetString();
	csSource.Replace(csStr1,csStr2);
	return csSource;
}

int StrCountOccur(CValue &cSource,CValue &cValue1)
{
	CString csSource=cSource.GetString();
	CString csStr1=cValue1.GetString();
	int nCount=csSource.Replace(csStr1,"");
	return nCount;
}
int StrLineCount(CValue &cSource)
{
	CString csSource=cSource.GetString();
	int nCount=csSource.Replace("\n","")+1;
	return nCount;
}



/*CString _csStaticSource;
int _nStaticLast=0;
int _nStaticLine=0;
*/
CString StrGetLine(CValue &cValue,int nLine)
{
	CString csSource=cValue.GetString()+"\r\n";
	int nLast=0;
	int nStartLine=1;

	//********блок для ускорения
	static CString _csStaticSource;
	static int _nStaticLast=0;
	static int _nStaticLine=0;
	if(_csStaticSource==csSource)
		if(_nStaticLine<=nLine)
		{
			nLast=_nStaticLast;//т.е. начинаем поиск не с начала
			nStartLine=_nStaticLine;
		}
	/***************************/


	//перебираем строчки в тупую
	for(int i=nStartLine;;i++)
	{
		int nIndex=csSource.Find("\r\n",nLast);
		if(nIndex<0)
			return "";
		if(i==nLine)
		{
			_csStaticSource=csSource;
			_nStaticLast=nIndex+2;
			_nStaticLine=nLine+1;

			return csSource.Mid(nLast,nIndex-nLast);
		}
		nLast=nIndex+2;
	}
	return "";
}
CString Upper(CValue &cSource)
{
	CString csSource=cSource.GetString();
	csSource.MakeUpper();
	return csSource;
}
CString Upper(CString cSource)
{
	CString csSource=cSource;
	csSource.MakeUpper();
	return csSource;
}
CString Lower(CValue &cSource)
{
	CString csSource=cSource.GetString();
	csSource.MakeLower();
	return csSource;
}

CString Chr(int nCode)
{
	CString csSource=" ";
	csSource.SetAt(0,(byte)(UINT)nCode);
	return csSource;
}
int Asc(CValue &cSource)
{
	CString csSource=cSource.GetString();
	if(csSource.GetLength()==0)
		return 0;
	return (UINT)(byte)csSource.GetAt(0);
}

CString OemToAnsi(CValue &cSource)
{
	CString csSource=cSource.GetString();
	csSource.OemToAnsi();
	return csSource;
}

CString AnsiToOem(CValue &cSource)
{
	CString csSource=cSource.GetString();
	csSource.AnsiToOem();
	return csSource;
}

//---Работа с датой

CValue CurDate(void)
{
    COleDateTime oleTime=COleDateTime::GetCurrentTime();
	oleTime.GetCurrentTime();
	return Date(oleTime.GetYear(),oleTime.GetMonth(),oleTime.GetDay());

}
CValue WorkingDate(CValue &cValue,int nMode)
{
	if (cValue.nType)
		WorkDate=Date(cValue);
	if (WorkDate==Date(0))
	{
		WorkDate=CurDate();
		return CurDate();
	}
	else
		return WorkDate;
}
CValue AddMonth(CValue &cData,int nMonthAdd)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	int SummaMonth=nYear*12+nMonth-1;
	SummaMonth+=nMonthAdd;
	nYear=SummaMonth/12;
	nMonth=SummaMonth%12+1;
	return CValue(Date(nYear,nMonth,nDay));
}
CValue BegOfMonth(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return CValue(Date(nYear,nMonth,1));
}
CValue EndOfMonth(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	CValue Date1=Date(CValue(AddMonth(Date(nYear,nMonth,1))-1));
	return Date1;
}
CValue BegOfQuart(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return Date(nYear,1+((nMonth-1)/3)*3,1);
}
CValue EndOfQuart(CValue &cData)
{
	return Date(CValue(AddMonth(BegOfQuart(cData),3)-1));
}
CValue BegOfYear(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return CValue(Date(nYear,1,1));
}
CValue EndOfYear(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return CValue(Date(nYear,12,31));
}
CValue BegOfWeek(CValue &cData)
{
	int nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear;
	cData.FromDate(nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear);
	CValue Date1=Date(CValue(Date(nYear,nMonth,nDay)-DayOfWeek+1));
	return Date1;
}
CValue EndOfWeek(CValue &cData)
{
	int nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear;
	cData.FromDate(nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear);
	CValue Date1=Date(CValue(Date(nYear,nMonth,nDay)+(7-DayOfWeek)));
	return Date1;
}
int GetYear(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return nYear;
}
int GetMonth(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return nMonth;
}
int GetDay(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return nDay;
}
int GetWeekOfYear(CValue &cData)
{
	int nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear;
	cData.FromDate(nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear);
	return WeekOfYear;
}
int GetDayOfYear(CValue &cData)
{
	int nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear;
	cData.FromDate(nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear);
	return DayOfYear;
}
int GetDayOfWeek(CValue &cData)
{
	int nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear;
	cData.FromDate(nYear,nMonth,nDay,DayOfWeek,DayOfYear,WeekOfYear);
	return DayOfWeek;
}
int GetQuartOfYear(CValue &cData)
{
	int nYear,nMonth,nDay;
	cData.FromDate(nYear,nMonth,nDay);
	return 1+((nMonth-1)/3);
}
CString PeriodStr(CValue &cData1,CValue &cData2)
{
	cData1=Date(cData1);
	cData2=Date(cData2);
	spell_date &sd=Speller::GetDefaultSpeller().DatePart;
	CString Str;
	int nYear1,nMonth1,nDay1;
	int nYear2,nMonth2,nDay2;
	cData1.FromDate(nYear1,nMonth1,nDay1);
	cData2.FromDate(nYear2,nMonth2,nDay2);
	//месяц
	if(cData1==BegOfMonth(cData1)&&cData2==EndOfMonth(cData1))
		Str.Format("%s %d %s",sd[nMonth1],nYear1,sd.SmallYear);
	else
	//квартал
	if(cData1==BegOfQuart(cData1)&&cData2==EndOfQuart(cData1))
		Str.Format("%d %s %d %s", int((nMonth1-1)/3)+1,sd.Quart,nYear1,sd.SmallYear);
	else
	if(cData1==BegOfYear(cData1))
	{ 
		// 6 месяцев
		if(cData2==EndOfMonth(Date(nYear1,6,1)))
			Str.Format("1 %s %d %s", sd.SemiYear, nYear1, sd.SmallYear);
		if(cData2==EndOfMonth(Date(nYear1,9,1)))
			Str.Format("%s %d %s", sd.Month_9, nYear1, sd.SmallYear);
		if(cData2==EndOfYear(Date(nYear1,1,1)))
			Str.Format("%d %s", nYear1,sd.SmallYear);
	}
	else
		//остальное - просто диапазон
		Str=cData1.GetString()+" - "+cData2.GetString();

	return Str;
}


/*
//*************************************
//тестовая функция
void FunctionUnitTest(void)
{
	//CValue
//	CValue::UnitTest();




	CString AA=EndOfQuart("16.01.88");

	AA=PeriodStr("01.01.03","31.03.03");

	
	if(PeriodStr("01.10.02","31.12.02")!="4 Квартал 2002 г.")AfxMessageBox("Error Data 18");
	if(PeriodStr("01.06.02","30.06.02")!="Июнь 2002 г.")AfxMessageBox("Error Data 18");
	if(PeriodStr("01.01.02","31.01.02")!="Январь 2002 г.")AfxMessageBox("Error Data 18");

	if(GetWeekOfYear("15.06.02")!=24)AfxMessageBox("Error Data 17");
	if(GetDayOfYear("15.06.02")!=166)AfxMessageBox("Error Data 16");
	if(GetDayOfWeek("15.06.02")!=6)AfxMessageBox("Error Data 18");

	if(GetDay("15.06.02")!=15)AfxMessageBox("Error Data 15");
	if(GetMonth("15.06.02")!=6)AfxMessageBox("Error Data 15");
	if(GetYear("15.06.02")!=2002)AfxMessageBox("Error Data 15");

	if(EndOfWeek("15.06.02")!=_D("16.06.02"))AfxMessageBox("Error Data 14");
	if(EndOfWeek("16.06.02")!=_D("16.06.02"))AfxMessageBox("Error Data 14");
	if(EndOfWeek("17.06.02")!=_D("23.06.02"))AfxMessageBox("Error Data 14");

	if(BegOfWeek("15.06.02")!=_D("10.06.02"))AfxMessageBox("Error Data 13");
	if(BegOfWeek("16.06.02")!=_D("10.06.02"))AfxMessageBox("Error Data 13");
	if(BegOfWeek("17.06.02")!=_D("17.06.02"))AfxMessageBox("Error Data 13");


	if(BegOfYear("16.06.02")!=_D("01.01.02"))AfxMessageBox("Error Data 12");
	if(EndOfYear("16.05.88")!=_D("31.12.88"))AfxMessageBox("Error Data 12");
	
	if(EndOfQuart("16.06.02")!=_D("30.06.02"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("16.05.88")!=_D("30.06.88"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("16.04.02")!=_D("30.06.02"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("11.01.88")!=_D("31.03.88"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("11.02.88")!=_D("31.03.88"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("11.03.88")!=_D("31.03.88"))AfxMessageBox("Error Data 11");
	if(EndOfQuart("11.07.88")!=_D("30.09.88"))AfxMessageBox("Error Data 11");

	if(BegOfQuart("16.06.88")!=_D("01.04.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("16.05.88")!=_D("01.04.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("16.04.88")!=_D("01.04.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("11.01.88")!=_D("01.01.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("11.02.88")!=_D("01.01.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("11.03.88")!=_D("01.01.88"))AfxMessageBox("Error Data 10");
	if(BegOfQuart("11.07.88")!=_D("01.07.88"))AfxMessageBox("Error Data 10");


	if(EndOfMonth("16.06.88")!=_D("30.06.88"))AfxMessageBox("Error Data 9");
	if(EndOfMonth("28.02.01")!=_D("28.02.01"))AfxMessageBox("Error Data 9");
	if(EndOfMonth("17.02.01")!=_D("28.02.01"))AfxMessageBox("Error Data 9");
	if(EndOfMonth("2.08.02")!=_D("31.08.02"))AfxMessageBox("Error Data 9");
	if(BegOfMonth("15.01.01")!=_D("01.01.01"))AfxMessageBox("Error Data 9");
	if(BegOfMonth("30.07.88")!=_D("01.07.88"))AfxMessageBox("Error Data 9");

	if(AddMonth("01.01.00")!=_D("01.02.00"))AfxMessageBox("Error Data 8");
	if(AddMonth("01.12.00")!=_D("01.01.01"))AfxMessageBox("Error Data 8");
	if(AddMonth("11.03.01",15)!=_D("11.06.02"))AfxMessageBox("Error Data 8");

	
	if(GetWeekOfYear("01.01.00")!=1)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("02.01.00")!=1)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("03.01.00")!=2)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("04.01.00")!=2)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("05.01.00")!=2)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("06.01.00")!=2)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("07.01.00")!=2)AfxMessageBox("Error Data 7");
	if(GetWeekOfYear("08.01.00")!=2)AfxMessageBox("Error Data 7");



	CString Str;
	Str=StrGetLine("Hi!\nHelo\nWord",1);
	Str=StrGetLine("Hi!\nHelo\nWord",2);
	Str=StrGetLine("Hi!\nHelo\nWord",3);
	Str=StrGetLine("Hi!\nHelo\nWord\nLala",4);



	CValue A=123456.72345678901;
	CValue C=Round(A,2);
	NUMBER f=C.fData;
	Str=C;
	Str=CValue(Round(A,3));
	Str=CValue(Round(A,4));
	Str=CValue(Round(A,-2));
	if(Round(123456.12,2)!=123456.12)AfxMessageBox("Error 1");
	if(Round(123456.125,2)!=123456.13)AfxMessageBox("Error 2");
	if(Round(123456.1235,3,1)!=123456.124)AfxMessageBox("Error 3");
	if(Round(123456.1235,3,0)!=123456.123)AfxMessageBox("Error 4");
	if(Round(123456.1235,-3)!=123000)AfxMessageBox("Error 5");
	//A=Round(123456.5234,-1);

	
	if(Round(587.349999585938,6)!=587.35)AfxMessageBox("Error 5-2");
	if(Round(12345587.349999585938,2)!=12345587.35)AfxMessageBox("Error 5-3");
	Str=_V(Round(587.349999585938,6));
	double fff=_V(Round(12345587.349999585938,6));
	fff=_V((double)12345678.349999585938);
	fff=12345678.34123432332323;


	if(Int(123456.7235)!=123456)AfxMessageBox("Error 6");
	if(Int("123456.7235")!=123456)AfxMessageBox("Error 7");

	if(Max(1,2,11,4,5)!=11)AfxMessageBox("Error 8");
	if(Min(1,2,11,-1,5)!=-1)AfxMessageBox("Error 9");


	if(Log10("1000")!=3)AfxMessageBox("Error 10");
	if(Round(Ln("1000"),10)!=Round(log(1000),10))AfxMessageBox("Error 11");
	A=123.123;
	CValue B=Ln(A);

	if(StrLen("0123456789")!=10)AfxMessageBox("Error 12");
	if(StrLen("0")!=1)AfxMessageBox("Error 13");
	if(StrLen("")!=0)AfxMessageBox("Error 14");

	
	if(IsBlankString("")!=1)AfxMessageBox("Error 15");
	if(IsBlankString("123")!=0)AfxMessageBox("Error 16");
	if(IsBlankString(" ")!=1)AfxMessageBox("Error 17");
	if(TrimL("  \t\n12345")!="12345")AfxMessageBox("Error 18");
	if(TrimR(" 12345\t \n   ")!=CValue(" 12345"))AfxMessageBox("Error 19");
	if(TrimAll("  \t 12345  \n ")!="12345")AfxMessageBox("Error 20");
	if(Left(" 12345 678",7)!=" 12345 ")AfxMessageBox("Error 21");
	if(Right(" 12345 678",4)!=" 678")AfxMessageBox("Error 22");
	if(Mid(" 12345 678",2,3)!="123")AfxMessageBox("Error 23");
	if(Mid(" 12345 678",5)!="45 678")AfxMessageBox("Error 24");
	if(Find("123 Всем привет!!","при")!=10)AfxMessageBox("Error 25");
	if(StrReplace("123 Всем привет!!","в","В")!="123 Всем приВет!!")AfxMessageBox("Error 26");
	if(StrReplace("123 Всем привет!!","В","Мне и в")!="123 Мне и всем привет!!")AfxMessageBox("Error 27");
	if(StrCountOccur("12345623789933283745651223","23")!=3)AfxMessageBox("Error 28");

	if(StrLineCount("")!=1)AfxMessageBox("Error 29");
	if(StrLineCount("Hi!")!=1)AfxMessageBox("Error 30");
	if(StrLineCount("Hi!\nHelo")!=2)AfxMessageBox("Error 31");

	if(StrGetLine("Hi!\nHelo",1)!="Hi!")AfxMessageBox("Error 32");
	if(StrGetLine("Hi!\nHelo",0)!="")AfxMessageBox("Error 33");
	if(StrGetLine("Hi!\nHelo",3)!="")AfxMessageBox("Error 34");

	if(StrGetLine("Hi!\nHelo\nWord\nLala",1)!="Hi!")AfxMessageBox("Error 34-1");
	if(StrGetLine("Hi!\nHelo\nWord\nLala",2)!="Helo")AfxMessageBox("Error 34-2");
	if(StrGetLine("Hi!\nHelo\nWord\nLala",3)!="Word")AfxMessageBox("Error 34-3");
	if(StrGetLine("Hi!\nHelo\nWord\nLala",4)!="Lala")AfxMessageBox("Error 34-4");


	if(Chr(48)!="0")AfxMessageBox("Error 35");
	if(Chr(255)!="я")AfxMessageBox("Error 36");
	if(Chr(32)!=" ")AfxMessageBox("Error 37");
	if(Asc(Chr(33))!=33)AfxMessageBox("Error 38");
	if(Asc("1")!=49)AfxMessageBox("Error 39");
	if(Asc("12")!=49)AfxMessageBox("Error 40");
	if(Asc("")!=0)AfxMessageBox("Error 41");

	

	A=Ln("1000");
	B=log(1000);
	CString Str1=A;
	CString Str2=B;
	Str1=Str1;


	CValue A1=PageBreak;
	CValue A2=LineBreak;
	CValue A3=TabSymbol;


	CTranslate::UnitTest();
}
*/


void Message(CValue cMessage,CValue cType)
{
#ifndef SMALL_TRANSLATE
	CString Str=cMessage.GetString();
	if(pMessageWindow)
		if(IsWindow(pMessageWindow->m_hWnd))
			pMessageWindow->Message(Str);
#endif
}
void Message(CValue cMessage)
{
#ifndef SMALL_TRANSLATE
	CString Str=cMessage.GetString();
	if(pMessageWindow)
		pMessageWindow->Message(Str);
#endif
}


void SetStatus(CString s)
{
#ifndef SMALL_TRANSLATE
	if(pStatusBar)
		pStatusBar->SetPaneText (0,s, TRUE);
#endif
}
void ClearMessageWindow()
{
#ifndef SMALL_TRANSLATE
	if(pMessageWindow)
		pMessageWindow->OnClear();
#endif
}


/*


//
CValue EvalExp(CString csExp)
{
	return CTranslate::EvalExp(csExp);
}
CString Template(CString csStr)
{
	CString csRes=csStr;
	int nIndex1=csStr.Find("[");
	int nIndex2=csStr.Find("]");
	if(nIndex1>=0)
	{
		csRes=csStr.Left(nIndex1);
		CString csExpr=csStr.Mid(nIndex1+1,nIndex2-nIndex1-1);
		return csRes+EvalExp(csExpr)+Template(csStr.Mid(nIndex2+1));

	}
	return csRes;
}

*/

CValue GetKeyDown(CValue **p)//НажатаяКлавиша
{
//	int nVirtKey=glVirtKey;
//	glVirtKey=0;
	return glVirtKey;
}

CRunContext *AfxGetCurrentRunContext();
CRunContext *AfxGetPrevRunContext();


CValue GetRunContext()
{
	CRunContext *pContext=AfxGetCurrentRunContext();
	if(pContext)
		return pContext->pProcUnit->vContext;
	else
		return CValue();
}

CValue Eval(CValue **p)//Выражение
{
	sCurError="";
	//CRunContext *p0=AfxGetCurrentRunContext();
	CRunContext *pContext=(CRunContext *)(int)p[1]->GetNumber();

	if(!pContext)
	{
		//OutToDebug("none\n");
		pContext=AfxGetCurrentRunContext();
	}

	//OutToDebug("pContext=%s\n",pContext->pProcUnit->pByteCode->pModule->csModuleName);
	return CProcUnit::Eval(p[0]->GetString(),pContext);
}
CValue GetCallContext(CValue **p)//ПолучитьКонтекстВызова
{
	//CRunContext *p0=AfxGetCurrentRunContext();
	CRunContext *pContext=AfxGetPrevRunContext();
	if(pContext)
		return pContext->pProcUnit->vContext;
	else
		return CValue();//return (int)pContext;
}


CValue ErrorDescription(CValue **p)//ОписаниеОшибки
{
	CValue rez=String(sCurError);
	if(p[0]->nType)
		sCurError=p[0]->GetString();
	return rez;
}

CValue DateToStr(CValue **p)//ДатаВСтроку
{
	CString Str=p[0]->GetString();
	if(Str.GetLength()<8)
		Str="00000000";
	else
		Str=Str.Mid(6)+Str.Mid(3,2)+Str.Mid(0,2);

	if(p[1]->nType)
	{
		Str+=p[1]->GetString();
	}
	Str+="000000000";
	return String(Str.Left(17));
}

CValue StrToDate(CValue **p)//СтрокаВДату
{
	CValue Ret;
	CString Str=p[0]->GetString();
	if(Str.GetLength()<8)
		Ret=Date(CValue(0));
	else
		Ret=Date(Str.Mid(6,2)+"."+Str.Mid(4,2)+"."+Str.Mid(0,4));

	if(p[1]->nType)
	{
		*p[1]=atoi(Str.Mid(8));
	}
	return Ret;
}

CValue GetMode(CValue **p)//ПолучитьРежимРаботы
{
#ifndef SMALL_TRANSLATE
	if(CONFIG_MODE==afxAppRunMode)
		return String("Конфигуратор");
	else
	if(ENTERPRISE_MODE==afxAppRunMode)
		return String("Предприятие");
	else
#endif
		return String("");
}

extern int nCountForEscape;
void DoMessage2C()
{
	MSG msg;
	while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( ! ::GetMessage( &msg, NULL, 0, 0 ) )
			break;
		::TranslateMessage(&msg ); 
		::DispatchMessage( &msg );

//		if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
//			SendMessage(msg.hwnd,msg.message,msg.lParam,msg.wParam);

	}
}
CValue DoMessage(CValue **p)//ОбработатьСообщения
{
	DoMessage2C();
	nCountForEscape=-1;
	return CValue();
}

CValue DoMessage2(CValue **p)//ОбработатьСообщения2
{
	WaitMessage();
	DoMessage2C();
	nCountForEscape=-1;
	return CValue();
}

CValue Pause(CValue **p)//пауза
{
	nCountForEscape=-1;
	Sleep(p[0]->GetNumber());
	return CValue();
}


CValue ArgCount(CValue **p)//КоличествоАргументовПрограммы
{
	return __argc;
}

CValue ArgValue(CValue **p)//ЗначениеАргументаПрограммы
{
	int n=p[0]->GetNumber();
	if(n<0||n>__argc)
		Error("Недопустимый индекс аргумента");
	return String(__argv[n]);
}

CValue SetAppTitle(CValue **p)//ЗаголовокСистемы
{
	CValue Ret=String(csTitleWnd);
	if(p[0]->nType!=TYPE_EMPTY)
	{
		csTitleWnd=p[0]->GetString();

		if(AfxGetMainWnd())
			AfxGetMainWnd()->SetWindowText(csTitleWnd);

		if(AfxGetApp())
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTitle(csTitleWnd);

	}
	return Ret;
}
 
#include "Definition.h"
CValue GetLexemArray(CValue **p)//ПолучитьМассивЛексем
{
	CTranslateModule translate;
	translate.Load(p[0]->GetString());
	translate.PrepareLexem();

	CValue Ret;
	Ret.CreateObject("Массив");
	if(translate.LexemList.GetSize()>100)
		Ret.SetSizeArray(translate.LexemList.GetSize());
	for(int i=0;i<translate.LexemList.GetSize()-1;i++)//без учета ENDPROGRAM
	{
		CLexem lex=translate.LexemList[i];
		CValue vlex;
		vlex.CreateObject("Структура");
		if(lex.nType==KEYWORD)
			lex.nType=IDENTIFIER;
		vlex.SetAt("Тип",lex.nType);

		if(lex.nType==DELIMETER)
			vlex.SetAt("Значение",String(CString(lex.nData)));
		else
			vlex.SetAt("Значение",lex.vData);

//		vlex.SetAt("НомерСтроки",lex.nNumberLine+1);
		vlex.SetAt("НомерПозиции",lex.nStringNumber);
		Ret.SetAt(i+1,vlex);
	}
	return Ret;
}


CString FuncFormat(CValue &v,CString &fmt){
	
	if(fmt.IsEmpty()){
		return v.GetString();
	}

	fmt.MakeUpper();
	int LeadingZero;
	char flag=fmt[0];
	if( "(0)"==fmt.Mid(1,3) ){
		LeadingZero=1;
		fmt=fmt.Mid(4);
	}
	else {
		LeadingZero=0;
		fmt=fmt.Mid(1);
	}

	Speller &spell=Speller::GetDefaultSpeller();

	CString rez;
	switch(flag){
	case 'D': case 'Д':
		{
			spell_date &sd=spell.DatePart;
			int dta=v.GetDate();
			int yy,mm,dd;
			DateFromIntToYMD(dta,yy,mm,dd);
			if(!yy){ //Если год=0 - некорректная (или пустая) дата
				rez=" . . ";
			}
			else
			{
				fmt.TrimRight();
				int octParam=0;
				//Преобразуем формат в 8-ричное число для удобства сравнения
				for(LPCTSTR s=fmt;*s && !(octParam & 030000000000);s++){
					octParam <<= 3;
					switch(*s){
					case 'Y': case 'Г':
						octParam |= 7;
						break;
					case 'D': case 'Д':
						octParam |= 6;
						break;

					case 'M': case 'М':
						octParam |= 5;
						break;


						// 4 не используем, т.к. для контроля переполнения 
						// мы можем использовать только 2 последних бита, а у 4 там нули.
					case 'W': case 'Н':
						octParam |= 2;
						break;

					case 'Q': case 'К':
						octParam |= 3;
						break;
					}
				}
				
				switch(octParam){
				case 0665577: //DDMMYY
					rez.Format( "%02d.%02d.%02d",dd,mm,yy%100);
					break;
				case 066557777: //DDMMYYYY
				default: //он же формат по-умолчанию
					rez.Format( "%02d.%02d.%04d",dd,mm,yy);
					break;

				case 066555577: //DDMMMMYY
					rez.Format((LeadingZero? "%02d %s %02d %s":"%d %s %02d %s"),dd,sd[spell_date::InM + mm] ,yy%100,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 06655557777: //DDMMMMYYYY
					rez.Format((LeadingZero? "%02d %s %04d %s":"%d %s %04d %s"),dd, sd[spell_date::InM + mm],yy,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 05555: //MMMM
					rez.Format( "%s",sd[mm]);
					Speller::FirstCharBig(rez);
					break;

				case 0555577: //MMMMYY
					rez.Format( "%s %02d %s",sd[mm],yy%100,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 055557777: //MMMMYYYY
					rez.Format( "%s %04d %s",sd[mm],yy,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 03333: //QQQQ
					rez.Format( (LeadingZero? "%02d %s":"%d %s")
						,mm/3+1,sd.Quart);
					Speller::FirstCharBig(rez);
					break;

				case 0333377: //QQQQYY
					rez.Format( (LeadingZero? "%02d %s %02d %s":"%d %s %02d %s")
						,mm/3+1,sd.Quart,yy%100,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 033337777: //QQQQYYYY
					rez.Format( (LeadingZero? "%02d %s %04d %s":"%d %s %04d г.")
						,mm/3+1,sd.Quart,yy,sd.SmallYear);
					Speller::FirstCharBig(rez);
					break;

				case 02222: //WWWW
					rez=sd.DayOfWeek[dta%7];
					Speller::FirstCharBig(rez);
					break;

				case 077775566: //YYYYMMDD
					rez.Format("%04d%02d%02d",yy,mm,dd);
					break;

				}
			}
			break;
		}

	//Числа
	case 'Ч': case 'N':
		{
			spell_num &sn=spell.num_Speller;
			
			if('П'==fmt[0]
				|| 'S'==fmt[0])
			{
				
				if(fmt.GetLength()>1 && ('Д'==fmt[1] || 'M'==fmt[1]))
				{
					
					if(fmt.GetLength()>2 && ('С'==fmt[2] || 'H'==fmt[2]))
					{
						rez=sn( double(v.GetNumber())
							,spell.UnitBig,spell.UnitSmall);
					}
					else
					{
						rez=sn( double(v.GetNumber()),spell.UnitBig);
					}
					
				}
				else
				{
					rez=spell.num_Speller( double(v.GetNumber()));
				}
				Speller::FirstCharBig(rez);
				break;
			}
			
			//Обычное число
			//ASSERT(0);
			int width=-1,prec=0,stat=1,move=0;
			char zero=0,dot='.',coma=0; 
			LPCTSTR s=fmt;
			if('0'==*s)
			{
				zero=' ';
				s++;
			}
			else if('-'==*s)
			{
				zero=*s;
				s++;
			}
			for(;*s;s++)
			{
				if('0' <= *s 
					&& *s <= '9')
				{
					if(width<0)
						width=0;
					else 
						width*=10;
					width+=(*s-'0');
				}
				else break;
			}
			
			if('.'==*s)
			{
				for(s++;*s;s++)
				{
					if('0' <= *s 
						&& *s <= '9')
					{
						prec*=10;
						prec+=(*s-'0');
					}
					else break;
				}
			}
			
			if('>'==*s)
			{
				for(s++;*s;s++)
				{
					if('0' <= *s 
						&& *s <= '9')
					{
						move*=10;
						move+=(*s-'0');
					}
					else 
						break;
				}
			}

			if(*s)
			{
				dot=*s;
				s++;
			}
			
			if(*s)
			{
				coma=*s;
			}
			v=CValue(v.GetNumber()/pow(10,move));
			rez=Speller::FloatFormat(double(v.GetNumber())
				,width,prec,LeadingZero,zero,dot,coma);
			break;
		}


	//Строки
	case 'S': case 'С':
		{
			int len=atoi(fmt); //У Format по умолчанию дополнение пробелами слева, у 1С - справа
			CString csStr=v.GetString();
			
			if(!len)
				rez=csStr;
			else 
				if (len<csStr.GetLength())
					csStr=csStr.Left(len);
				rez.Format("%*s",-len,csStr);
			break;
		}
	default: //Не распознан или не задан
		rez=v.GetString();
	}
	return rez;
}

CValue FuncFormat(CValue **p)
{
	return String(FuncFormat(*p[0],p[1]->GetString()));
}

CString Template(CString arg)
{
	LPCTSTR pars=arg;
	CString rez("");

	CRunContext *pContext=AfxGetCurrentRunContext();
	for(;*pars;pars++){
		if('['!=*pars){
			//Обычная строка
			rez += *pars; 
		}
		else { //Выражение
			CString aExpr,aFmt;
			int nQuote=0;
			int nBracket=0;
			for(pars++;*pars;pars++){ 
				if(nQuote){
					//Строка в кавычках - все символы пропускаем
					aExpr+=*pars;

					if('"' == *pars)nQuote = 0;//Строка закончена либо 1-я часть кавычек в строке
					continue;
				}

				if(!nBracket){
					//Обычное выражение
					if(']' == *pars )break;	//Выражение закончено
					if('#' == *pars){//Форматная строка
						for(pars++;*pars;pars++){
							if(']' == *pars)break;//Выражение закончено
							aFmt +=*pars;
						}
						if(aFmt.GetLength()){
							aFmt.TrimLeft();
							aFmt.TrimRight();
						}
						break;
					}
				}

				//Обычные символы либо выражение внутри []
				aExpr +=*pars;
				switch(*pars){
				case '[': nBracket++;break;
				case ']': nBracket--;break;
				case '"': nQuote = 1;break; //строка в кавычках либо 2-я часть кавычек
				}
			}
			if(nBracket || nQuote){
				CString StrError("Незавершенное выражение в шаблоне! ");
				StrError += aExpr;
				Error(StrError);
				break;
			}
			//Вычисляем выражение
			rez += FuncFormat(CProcUnit::Eval(aExpr,pContext),aFmt);
		}

	}
	return rez;
}

CString FixTemplate(CString arg)
{
	LPCTSTR pars=arg;
	CString rez("");
	int len=0;
	CRunContext *pContext=AfxGetCurrentRunContext();
	
	for(;*pars;pars++)
	{
		if('['!=*pars)
		{
			//Обычная строка
			rez += *pars; 
		}
		else
		{ //Выражение
			CString aExpr,aFmt;
			int nQuote=0;
			int nBracket=0;
			len=1;
			for(pars++;*pars;pars++)
			{ 
				if(nQuote)
				{
					//Строка в кавычках - все символы пропускаем
					aExpr+=*pars;

					if('"' == *pars)nQuote = 0;//Строка закончена либо 1-я часть кавычек в строке
					continue;
				}

				if(!nBracket)
				{
					//Обычное выражение
					if(']' == *pars ) 
					{	
						len++;
						aFmt="S"+CString(CValue(len));
						break;	//Выражение закончено
					}
					if('#' == *pars)
					{//Форматная строка
						for(pars++;*pars;pars++)
						{
							if(']' == *pars)break;//Выражение закончено
							aFmt +=*pars;
						}
						if(aFmt.GetLength())
						{
							aFmt.TrimLeft();
							aFmt.TrimRight();
						}
						break;
					}
				}

				//Обычные символы либо выражение внутри []
				aExpr +=*pars;
				len++;
				switch(*pars){
				case '[': nBracket++;break;
				case ']': nBracket--;break;
				case '"': nQuote = 1;break; //строка в кавычках либо 2-я часть кавычек
				}
			}
			if(nBracket || nQuote)
			{
				CString StrError("Незавершенное выражение в шаблоне! ");
				StrError += aExpr;
				Error(StrError);
				break;
			}
			//Вычисляем выражение
			rez += FuncFormat(CProcUnit::Eval(aExpr,pContext),aFmt);
		}

	}
	return rez;
}

CValue FuncTemplate(CValue **p)
{
	return String(Template(p[0]->GetString()));
}

CValue FuncFixTemplate(CValue **p)
{
	return String(FixTemplate(p[0]->GetString()));
}

CValue Activate(CValue **p)//Активизировать
{
	return CProcUnit::Eval(p[0]->GetString()+".Активизировать()");
}


CValue HasError(CValue **p)//ЕстьНеОбработаннаяОшибка
{
	BOOL bRes=bHasError;
	if(p[0]->nType)
	{
		bHasError=p[0]->GetNumber();
	}
	return bRes;
}

CValue EndJob(CValue **p)//ЗавершитьРаботуСистемы
{
	if(!AfxGetMainWnd())
		return 0;

	int nMode=1;
	if(p[0]->nType)
		nMode=p[0]->GetNumber();
	if(nMode)
		if(pMainFrame)
		{
			pMainFrame->PostMessage(WM_CLOSE);
			return 0;
		}

	AfxPostQuitMessage(nMode);
	return 0;
}

CValue TrimR(CValue **p)//СокрП
{
	CString Str=p[0]->GetString();
	if(p[1]->nType)
	{
		Str.TrimRight(p[1]->GetString().GetAt(0));
	}
	else
	{
		Str.TrimRight();
	}
	return String(Str);
}
CValue TrimL(CValue **p)//СокрЛ
{
	CString Str=p[0]->GetString();
	if(p[1]->nType)
	{
		Str.TrimLeft(p[1]->GetString().GetAt(0));
	}
	else
	{
		Str.TrimLeft();
	}
	return String(Str);
}


CValue Beep2(CValue **p)//Beep2
{
	int n1=700;
	int n2=150;
	if(p[0]->nType)
		n1=p[0]->GetNumber();
	if(p[1]->nType)
		n2=p[1]->GetNumber();

	Beep(n1,n2);

	return 0;
}

CValue Beep(CValue **p)//Beep
{
	MessageBeep(MB_ICONASTERISK);
	return 0;
}

class CAutoInit
{
public:
	CAutoInit()
	{
		 srand( (unsigned)time( NULL ) );
	};
}Lala;

CValue Rand(CValue **p)
{
	return rand();
}

CValue GetPasswordHash(CValue **p)//HashПароля
{
	return String(GetHash(p[0]->GetString()));
}


CValue GetSysRegistersObjectsArray(CValue **pv)//ПолучитьМассивСистемныхОбъектов
{
	CMap <void*,void*,CString,CString&> *aNames=GetRegistersObjectsArray();

	CValue vRet;
	vRet.CreateObject("Массив");

	int i=1;
	void *p;
	POSITION pos;
	CString csValue;
	for( pos = aNames->GetStartPosition(); pos != NULL; )
	{
		aNames->GetNextAssoc( pos, p, csValue);
		if(p!=RUNTIME_CLASS(CValueObject))
		{
			vRet.SetAt(i,String(csValue));
			i++;
		}
	}
	vRet.SetAt(i,String("Структура"));

	((CValueArray*)vRet.pRef)->Sort();
	return vRet;
}

extern CValueArray aEvents;
CValue RegisterEvent(CValue **p)//ЗарегистрироватьСобытие
{
    return CValue(RegisterEvent(p[0]->GetString()));
}

CValue NotifyEvent(CValue **p)//ВызватьСобытие
{
	CValue *aP[7];
	for (int i=0;i<7;i++)
		aP[i]=p[i+1];
	return CValue(NotifyEvent(CString(p[0]->GetString()),aP));
}

CValue SubscribeOnEvent(CValue **p)//ПодписатьсяНаСобытие
{

	CString csEventName=p[0]->GetString();
	CString csFunctionName=p[1]->GetString();
	int nErrorFlag;
	if (p[2]->GetType()==TYPE_NUMBER)
		nErrorFlag=p[2]->GetNumber();
	else
		nErrorFlag=0;

	int nPriority;
	if (p[3]->GetType()==TYPE_NUMBER)
		nPriority=p[3]->GetNumber();
	else
		nPriority=50;		

	CRunContext *pCont=AfxGetCurrentRunContext();
	CProcUnit  *pModule;
	pModule=pCont->pProcUnit;
	CByteCode *pByteCode=pModule->pByteCode;
	CString csModuleName=Right(CValue(nPriority+100),2)+pByteCode->csModuleName;
	if (aEvents.FindId(csEventName))
	{
	    CValue aEventName;
		aEventName.CreateObject("Массив");
		CValueArray *pEventName=(CValueArray *)aEventName.pRef;
		if (aEvents.GetAt(csEventName).GetType()==TYPE_ARRAY)
		{
			pEventName->Load(aEvents.GetAt(csEventName),0,0);
			if (pEventName->FindId(csFunctionName))
			{
				//AfxMessageBox("Подписка на событие уже есть: "+p[0]->GetString(),MB_OK | MB_ICONSTOP|MB_APPLMODAL);
				return 0;
			}
			else
			{
				aEventName.SetAt(csFunctionName,CValue(csModuleName));
				aEvents.SetAt(p[0]->GetString(),aEventName);
				//AfxMessageBox("Подписка на событие: "+p[0]->GetString()+" обработчик "+csFunctionName+" из модуля:"+csModuleName,MB_OK | MB_ICONSTOP|MB_APPLMODAL);
			}
		}
		else
		{
			aEventName.SetAt(csFunctionName,CValue(csModuleName));
			aEvents.SetAt(p[0]->GetString(),aEventName);
			//AfxMessageBox("Подписка на событие: "+p[0]->GetString()+" обработчик "+csFunctionName+" из модуля:"+csModuleName,MB_OK | MB_ICONSTOP|MB_APPLMODAL);
		}
	}
	else
	{
		//Message(CValue("Cобытие: "+p[0]->GetString()+" не существует"));
		return 0;
	}
	return 1;
}

CValue GetTextOfActiveWindow(CValue **p)//ПолучитьТекстАктивногоОкна
{
	return CValue("");
}
CValue SetTextOfActiveWindow(CValue **p)//УстановитьТекстАктивногоОкна
{
	return 1;
}
CValue GoToLineActiveWindow(CValue **p)//ПерейтиКСтрокеАктивногоОкна
{
	return 1;
}

//Массив
struct SCallFunction DefSystemFunctionsArray[]=
{
	{"СтрокаВДату",			2,StrToDate,"СтрокаВДату(стрГМД) - Преобразование строки вида ГГГГММДД в значение типа Дата"},
	{"StrToDate",			2,StrToDate,"StrToDate(strYMD) - Convert form string format YYYYMMDD to value type of Date",""},//Уж простите меня за мой корявый английский :)

	{"ДатаВСтроку",			2,DateToStr},
	{"DateToStr",			2,DateToStr},

	{"ОписаниеОшибки",		1,ErrorDescription},
	{"GetErrorDescription",	1,ErrorDescription},

	{"НажатаяКлавиша",		0,GetKeyDown},
	{"GetKeyDown",			0,GetKeyDown},

	{"Сообщить",			2,Message},
	{"Message",				2,Message},
	{"Предупреждение",		3,DoMessageBox},
	{"DoMessageBox",		3,DoMessageBox},

	{"Мин",					10,Min},
	{"Min",					10,Min},
	{"Макс",				10,Max},
	{"Max",					10,Max},

	{"Число",				1,Number2},
	{"Number",				1,Number2},
	{"Строка",				1,String2},
	{"String",				1,String2},
	{"Дата",				3,Date3},
	{"Date",				3,Date3},

	{"ТекущееВремя",		3,CurrentTime},
	{"CurrentTime",			3,CurrentTime},
	
	{"СоздатьОбъект",		2,CreateObject},
	{"CreateObject",		2,CreateObject},

	{"Состояние",			1,SetStatus},
	{"Status",				1,SetStatus},
	{"ОчиститьОкноСообщений",0,ClearMessageWindow},
	{"ClearMessageWindow",	0,ClearMessageWindow},

	{"Ошибка",				1,Raise},
	{"Error",				1,Raise},

	{"ТипЗначения",			1,TypeValue},
	{"TypeValue",			1,TypeValue},

	{"ТипЗначенияСтр",		1,ValueTypeStr},
	{"ValueTypeStr",		1,ValueTypeStr},

//---Математические:
	{"Окр",					3,Round},
	{"Round",				3,Round},
	{"Цел",					1,Int},
	{"Int",					1,Int},
	{"Лог10",				1,Log10},
	{"Log10",				1,Log10},
	{"Лог",					1,Ln},
	{"Log",					1,Ln},

	
//---Строковые:
	{"СокрЛП",				1,TrimAll},
	{"TrimAll",				1,TrimAll},
	{"СтрДлина",			1,StrLen},
	{"StrLen",				1,StrLen},

	{"ПустаяСтрока",		1,IsBlankString},
	{"IsBlankString",		1,IsBlankString},
	{"СокрЛ",				2,TrimL},
	{"TrimL",				2,TrimL},
	{"СокрП",				2,TrimR},
	{"TrimR",				2,TrimR},
	{"Лев",					2,Left},
	{"Left",				2,Left},
	{"Прав",				2,Right},
	{"Right",				2,Right},

	{"СтрЗаменить",			3,StrReplace},
	{"StrReplace",			3,StrReplace},

	{"СтрЧислоВхождений",	2,StrCountOccur},
	{"StrCountOccur",		2,StrCountOccur},

	{"СтрКоличествоСтрок",	1,StrLineCount},
	{"StrLineCount",		1,StrLineCount},

	{"Сред",					3,Mid},
	{"Mid",					3,Mid},

	{"Найти",				3,Find},
	{"Find",				3,Find},

	{"СтрПолучитьСтроку",	2,StrGetLine},
	{"StrGetLine",			2,StrGetLine},

	{"Симв",				1,Chr},
	{"Chr",					1,Chr},
	{"КодСимв",				1,Asc},
	{"Asc",					1,Asc},

	{"OemToAnsi",			1,OemToAnsi},
	{"AnsiToOem",			1,AnsiToOem},

//Форматирование

	{"Формат",		2,FuncFormat},
	{"Format",		2,FuncFormat},
	{"Шаблон",		1,FuncTemplate},
	{"Template",	1,FuncTemplate},
	{"ФиксШаблон",	1,FuncFixTemplate},
	{"FixTemplate",	1,FuncFixTemplate},

	{"РазмерМассива",		2,Dim},
	{"DimArray",			2,Dim},
	{"РазмерСтруктуры",		2,DimStr},
	{"DimStruct",			2,DimStr},

	{"Разм",				2,Dim},
	{"Dim",					2,Dim},

	{"РазмерСтруктуры",		2,DimStr},
	{"РазмСтр",				2,DimStr},
//	{"DimStr",				2,DimStr},



	{"ЗначениеВСтроку",		1,ValueToString},
	{"ValueToString",		1,ValueToString},

	{"ЗначениеИзСтроки",	1,ValueFromString},
	{"ValueFromString",		1,ValueFromString},

	{"ЗначениеВФайл",		3,ValueToFile},
	{"ValueToFile",			3,ValueToFile},

	{"ЗначениеИзФайла",		3,ValueFromFile},
	{"ValueFromFile",		3,ValueFromFile},


	{"ЗначениеВСтрокуВнутр",1,ValueToString},
	{"ValueToStringInternal",1,ValueToString},

	{"ЗначениеИзСтрокиВнутр",1,ValueFromString},
	{"ValueFromStringInternal",1,ValueFromString},
	
	{"ПустоеЗначение",		1,EmptyValue},
	{"EmptyValue",			1,EmptyValue},

	{"ЗарегистрироватьОбъект",2,AttachRegisterObject},
	{"AttachRegisterObject",2,AttachRegisterObject},
	
	{"ОтменитьРегистрациюОбъекта",	1,UnRegisterObject},
	{"UnRegisterObject",			1,UnRegisterObject},


	{"ТекущаяДата",			0,CurDate},
	{"CurDate",				0,CurDate},

	{"РабочаяДата",			2,WorkingDate},
	{"WorkingDate",			2,WorkingDate},


	{"ДобавитьМесяц",		2,AddMonth},
	{"AddMonth",			2,AddMonth},

	{"НачМесяца",			1,BegOfMonth},
	{"BegOfMonth",			1,BegOfMonth},

	{"КонМесяца",			1,EndOfMonth},
	{"EndOfMonth",			1,EndOfMonth},

	{"НачКвартала",			1,BegOfQuart},
	{"BegOfQuart",			1,BegOfQuart},

	{"КонКвартала",			1,EndOfQuart},
	{"EndOfQuart",			1,EndOfQuart},

	{"НачГода",				1,BegOfYear},
	{"BegOfYear",			1,BegOfYear},

	{"КонГода",				1,EndOfYear},
	{"EndOfYear",			1,EndOfYear},

	{"НачНедели",			1,BegOfWeek},
	{"BegOfWeek",			1,BegOfWeek},

	{"КонНедели",			1,EndOfWeek},
	{"EndOfWeek",			1,EndOfWeek},

	{"ДатаГод",				1,GetYear},
	{"GetYear",				1,GetYear},

	{"ДатаМесяц",			1,GetMonth},
	{"GetMonth",			1,GetMonth},

	{"ДатаЧисло",			1,GetDay},
	{"GetDay",				1,GetDay},

	{"НомерНеделиГода",		1,GetWeekOfYear},
	{"GetWeekOfYear",		1,GetWeekOfYear},

	{"НомерДняГода",		1,GetDayOfYear},
	{"GetDayOfYear",		1,GetDayOfYear},

	{"НомерДняНедели",		1,GetDayOfWeek},
	{"GetDayOfWeek",		1,GetDayOfWeek},

	{"НомерКварталаГода",	1,GetQuartOfYear},
	{"GetQuartOfYear",		1,GetQuartOfYear},

	{"ПериодСтр",			2,PeriodStr},
	{"PeriodStr",			2,PeriodStr},

	{"Врег",				1,Upper},
	{"Upper",				1,Upper},

	{"Нрег",				1,Lower},
	{"Lower",				1,Lower},


	{"УстановитьТолькоЧтение",2,SetReadOnly},
	{"SetReadOnly",			2,SetReadOnly},


	{"КаталогПрограммы",	0,BinDir},
	{"BinDir",				0,BinDir},

	{"ИмяКомпьютера",		0,ComputerName},
	{"ComputerName",		0,ComputerName},

	{"ЗапуститьПриложение",	3,RunApp},
	{"RunApp",				3,RunApp},


	{"КаталогИБ",			0,IBDir},
	{"IBDir",				0,IBDir},

	{"ВвестиЧисло",		5,InputNumeric},
	{"InputNumeric",	5,InputNumeric},

	{"ВвестиСтроку",	5,InputDialog},
	{"InputString",		5,InputDialog},

	{"ВвестиДату",		3,InputDate},
	{"InputDate",		3,InputDate},

	{"ВвестиПериод",	5,InputPeriod},
	{"InputPeriod",		5,InputPeriod},

	{"ВвестиЗначение",	5,InputValue},
	{"InputValue",		5,InputValue},

	{"ОткрытьФорму",	FORM_PARAM_COUNT,OpenForm,"ОткрытьФорму(ОписательОбъекта,КонтекстФормы,ИмяФайла,УникальныйИД,Парам1,КонтекстОткрытия,РежимПодбора)"},
	{"OpenForm",		FORM_PARAM_COUNT,OpenForm},

	{"ОткрытьФормуМодально",FORM_PARAM_COUNT,OpenFormModal},
	{"OpenFormModal",		FORM_PARAM_COUNT,OpenFormModal},

	{"ОткрытьФормуРедактирования",	FORM_PARAM_COUNT,OpenFormEdit},
	{"OpenFormEdit",				FORM_PARAM_COUNT,OpenFormEdit},
	
	{"ОткрытьПодбор",				FORM_PARAM_COUNT,OpenPermanentChoice},
	{"OpenPermanentChoice",			FORM_PARAM_COUNT,OpenPermanentChoice},

	


	{"СтатусВозврата",	1,ReturnStatus},
	{"ReturnStatus",	1,ReturnStatus},


	{"Вопрос",			4,DoQueryBox},
	{"DoQueryBox",		4,DoQueryBox},


	{"СостояниеКлавиши",	1,GetKeyState},
	{"GetKeyState",			1,GetKeyState},

	{"_GetPerformanceCounter",0,GetTickCount},

	{"УстановитьТаймер",	2,SetTimer},
	{"SetTimer",			2,SetTimer},

	{"УдалитьТаймер",		1,DelTimer},
	{"DelTimer",			1,DelTimer},

	{"Выражение",			2,Eval},
	{"Expression",			2,Eval},

	{"РежимРаботы",			0,GetMode},
	{"WorkMode",			0,GetMode},

	{"ОбработатьСообщения",	0,DoMessage,"ОбработатьСообщения() - обработка все оконных сообщений"},
	{"DoMessage",			0,DoMessage},

	{"ОбработатьСообщения2",	0,DoMessage2,"ОбработатьСообщения2() - ожидание первого сообщения + обработка все оконных сообщений"},
	{"DoMessage2",			0,DoMessage2},

	{"Пауза",				1,Pause,"Пауза(МилСек)"},
	{"Pause",				1,Pause},

	{"КоличествоАргументовПрограммы",0,ArgCount},
	{"ArgCount",					0,ArgCount},

	{"ЗначениеАргументаПрограммы",	1,ArgValue,"ЗначениеАргументаПрограммы(Индекс) - /индекс изменяется от 0 до КоличествоАргументовПрограммы()/"},
	{"ArgValue",					1,ArgValue},

	{"ПолучитьКонтекстВызова",	0,GetCallContext},
	{"GetCallContext",			0,GetCallContext},

	{"ПолучитьМассивЛексем",	1,GetLexemArray},
	{"GetLexemArray",			1,GetLexemArray},

	{"ЗаголовокСистемы",	1,SetAppTitle},
	{"SetTitle",			1,SetAppTitle},

	{"Активизировать",		1,Activate},
	{"Activate",			1,Activate},

	{"КаталогПользователя",	1,UserDir},
	{"UserDir",				1,UserDir},

	{"ИмяПользователя",		1,UserName},
	{"UserName",			1,UserName},
	{"ПолноеИмяПользователя",	1,UserName},
	{"UserFullName",			1,UserName},

	{"ПарольПользователя",	1,UserPassword},
	{"UserPassword",		1,UserPassword},

	{"МонопольныйРежим",	0,SingleMode},
	{"ExclusiveMode",		0,SingleMode},

	{"КаталогВременныхФайлов",	0,TempFilesDir},
	{"TempFilesDir",			0,TempFilesDir},

	{"ОсновнойЯзык",		0,GeneralLanguage},
	{"GeneralLanguage",		0,GeneralLanguage},

	{"ЕстьНеОбработаннаяОшибка",	1,HasError,"ЕстьНеОбработаннаяОшибка (УстФлаг) - проверяет/устанавливает флаг необработанной ошибки исполнения."},
	{"HasError",					1,HasError},

	{"ЗавершитьРаботуСистемы",		1,EndJob,"ЗавершитьРаботуСистемы(<ФлагСохранения>) - вызывает завершение рабоы системы."},
	{"EndJob",						1,EndJob},

	{"Сигнал",				0,Beep},
	{"Beep",				0,Beep},

	{"Сигнал2",				2,Beep2,"Сигнал(Частота,Продолжительность)"},
	{"Beep2",				2,Beep2},


	{"ПСЧ",					0,Rand},
	{"Rand",				0,Rand},

    {"ПолучитьХэш",			1,GetPasswordHash,"GetHash(<Пароль>) - получить hash-функцию пароля."},
    {"GetHash",				1,GetPasswordHash,"GetHash(<Пароль>) - получить hash-функцию пароля."},


    {"ПолучитьМассивСистемныхОбъектов",	    0,GetSysRegistersObjectsArray,"ПолучитьМассивСистемныхОбъектов()"},
    {"GetSysRegistersObjectsArray",			0,GetSysRegistersObjectsArray},

	{"ЗарегистрироватьСобытие",	    1,RegisterEvent,"ЗарегистрироватьСобытие(ИмяСобытия)"},
    {"RegisterEvent",				1,RegisterEvent},

	{"ВызватьСобытие",			    8,NotifyEvent,"ВызватьСобытие(ИмяСобытия,Параметры...)"},
    {"NotifyEvent",			        8,NotifyEvent},

	{"ПодписатьсяНаСобытие",	    4,SubscribeOnEvent,"ПодписатьсяНаСобытие(ИмяСобытия,ИмяОбработчика,ФлагОшибки)"},
    {"SubscribeOnEvent",			4,SubscribeOnEvent},

	{"ПолучитьТекстАктивногоОкна",	0,GetTextOfActiveWindow,"ПолучитьТекстАктивногоОкна()"},
    {"УстановитьТекстАктивногоОкна",2,SetTextOfActiveWindow,"УстановитьТекстАктивногоОкна(Стр,ФлагМодифицированности)"},
	{"ПерейтиКСтрокеАктивногоОкна", 1,GoToLineActiveWindow,"ПерейтиКСтрокеАктивногоОкна(Номер)"},

	{"END",0,NULL},
};

