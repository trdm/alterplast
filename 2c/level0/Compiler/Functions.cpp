// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// Functions.cpp: implementation of the Functions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <Windows.h>
#include "Functions.h"
#include "Definition.h"

#include <ctype.h>
#include <string.h>
#include "setjmp.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <TCHAR.H>
#include <afxwin.h>
#include "CompileModule.h"
#include "../ModuleManager.h"
#include "ValueArray.h"
//extern int afxCountTryCatchList;
extern int afxEventMessage;
CString afxStrErrorMessage;
BOOL afxMessageFromConfig=0;
BOOL afxSimpleErrorMessage=0;
extern int bStopDebug;

class CRunContext *AfxGetCurrentRunContext();
CString GetModuleText(CString csPath,BOOL &bError);
extern BOOL bHasError;

//////////////////////////////////////////////////////////////////////
//Constant:Список сообщений об ошибках
//////////////////////////////////////////////////////////////////////
char* aErrors[]=
{
	"Usage: %s <filename>",
	"Ошибка чтения файла %s",
	"Ошибка открытия файла %s",
	"ASSERT: Системная ошибка модуля %s в строке %d",
	"ASSERT_VALID: Системная ошибка модуля %s в строке %d",
	"Системная ошибка (выход за пределы массива) при попытке обработать ошибку с номером %d",
	"Ожидается символ:\n%s",
	"Ожидается слово:\n%s",
	"Ожидается число-константа:\n%s",
	"Ожидается строка-константа:\n%s",
	"Ожидается дата-константа:\n%s",
	"%s\nДублирование идентификатора %s",//ERROR_IDENTIFIER_DUPLICATE
	"%s\nМетка '%s' не определена",
	"%s\nОжидается одно из ключевых слов!",
	"%s\nОжидается код модуля",
	"%s\nОжидается идентификатор",//ERROR_IDENTIFIER_DEFINE
	"%s\nОжидается ключевое слово или идентификатор",//ERROR_CODE
	"%s\nОжидается символ '%s'",//ERROR_DELIMETER
	"%s\nОжидается закрывающаяся скобка или запятая",//ERROR_FUNC_DELIMETER
	"%s\nОжидается ключевое слово объявления функции или процедуры",//ERROR_FUNC_DEFINE
	"%s\nМодуль не может иметь оператор возврата",//ERROR_RETURN
	"%s\nОжидается константа",//ERROR_CONST_DEFINE
	"%s\nОжидается оператор завершения процедуры или функции!",//ERROR_ENDFUNC_DEFINE
	"Ошибка записи файла: %s",//ERROR_FILE_WRITE
	"Ошибка в выражении:\n%s",//ERROR_EXPRESSION
	"%s\nОжидается ключевое слово %s",//ERROR_KEYWORD
	"%s\nИдентификатор '%s' не определен",//ERROR_IDENTIFIER_NOT_FOUND
	"%s\nОпереатор Break(Прервать) может употребляться только внутри цикла",//ERROR_USE_BREAK
	"%s\nОпереатор Continue (Продолжить)  может употребляться только внутри цикла",//ERROR_USE_CONTINUE
	"%s\nОператор Return (Возврат) не может употребляться вне процедуры или функции",//ERROR_USE_RETURN
	"%s\nОжидаются операторы программы",//ERROR_USE_BLOCK
	"%s\nОжидается выражение",//ERROR_EXPRESSION_REQUIRE
	"%s\nПроцедура или функция не обнаружена (%s)",//ERROR_CALL_FUNCTION
	"%s\nПроцедура или функция с указанным именем уже определена в текущем модуле (%s)",//ERROR_DEF_FUNCTION
	"%s\nСлишком много фактических параметров",//ERROR_MANY_PARAMS
	"%s\nНедостаточно фактических параметров",//ERROR_FEW_PARAMS
	"%s\nПеременная не определена (%s)",//ERROR_VAR_NOT_FOUND
	"%s\nНеожиданное завершение кода программы",//ERROR_END_PROGRAM
	"%s\nОжидается целочисленная константа положительного знака",//ERROR_ARRAY_SIZE_CONST
	"%s\nПовторный импорт родительского модуля",//ERROR_DUBLICATE_IMPORT
	"%s\nМодуль не найден",//ERROR_MODULE_NOT_FOUND
	"%s\nОператор импорта должен находиться в начале модуля",//ERROR_USE_IMPORT
	"%s\nОжидается завершающий оператор условной компиляции",//ERROR_USE_ENDDEF
	
	"%s\nОшибка задания типа",//ERROR_TYPE_DEF
	"%s\nПлохой тип переменной",//ERROR_BAD_TYPE
	"%s\nПлохой тип значения",//ERROR_BAD_TYPE_EXPRESSION
	"%s\nПеременная должна быть числового типа",//ERROR_NUMBER_TYPE
	"%s\nОжидается числовое значение",//ERROR_BAD_TYPE_EXPRESSION_N
	"%s\nОжидается строковое значение",//ERROR_BAD_TYPE_EXPRESSION_S
	"%s\nОжидается значение даты",//ERROR_BAD_TYPE_EXPRESSION_D

	"%s\nТип переменной не поддерживает данную операцию",//ERROR_TYPE_OPERATION
	
	
	
	

	
	
	
};




//обмен сообщениями с конфигуратором
void SendStringToConfig(HWND hwnd,CString c);
void SendStringToConfig(CString csMessage);


void DoDebugLoop(CString csModule,int nLine,CRunContext *pRunContext);
void Message(CValue cMessage);
extern CRunContext *afxCurrentRunContext;
void ResetByteCode();


CString sCurError;
//////////////////////////////////////////////////////////////////////
// Обработка ошибок
//////////////////////////////////////////////////////////////////////
CString CTranslateError::GetErrorMessage()
{
	return sCurError;
}


//служебные процедуры обработки ошибок
void ErrorV(LPCTSTR fmt,va_list &list)
{
	CString buf;
	buf.FormatV(fmt,list);
	va_end(list);

	if(afxSimpleErrorMessage||afxMessageFromConfig)
		buf.Replace("\n","  ");

	buf.TrimRight();
	afxStrErrorMessage=buf;
	sCurError=buf;
	throw (new CTranslateError());
}

void Error(LPCTSTR fmt,...)
{
	va_list list;
	va_start(list,fmt);
	ErrorV(fmt,list);
}

void Error(int nErr,...)
{
	CString buf;
	va_list list;
	if(0<=nErr && nErr<LastError)
	{
		va_start(list,nErr);
		ErrorV(aErrors[nErr],list);
	}
	else {
		list = (va_list)&nErr;
		ErrorV(aErrors[ERROR_SYS1],list);
	}
}


void DoError()
{
	if(afxMessageFromConfig)
	{
		return;
	}

	if(sCurError.IsEmpty())
		return;
	CString s=sCurError;


	CByteCode *pByteCode=afxErrorPlace.pByteCode;
	int nCurLine=afxErrorPlace.nLine;
	afxErrorPlace.Reset();//ошибка обработана

	if(!afxSimpleErrorMessage)
	{
		afxEventMessage=0;
		try
		{
			if(pByteCode)
			{
				int nCurPos=0;
				CString csModuleName=pByteCode->csModuleName;
				if(nCurLine>=0&&nCurLine<pByteCode->CodeList.GetSize())
				{
					nCurPos=pByteCode->CodeList[nCurLine].nStringNumber;
					csModuleName=pByteCode->CodeList[nCurLine].csModuleName;
					BOOL bError;
					CString csModule;
					if(mUpper(csModuleName.Right(3))==".FD")
						csModule=pByteCode->csModule;
					else
						csModule=GetModuleText(csModuleName,bError);
					if(!csModule.IsEmpty())
					{
						CString csErrorLine=FindErrorCodeLine(csModule,nCurPos,0);
						s=csModuleName+"\t"+csErrorLine+"\n"+s;
					}
				}
			}
		}
		catch(...)
		{
		}
	}
	afxSimpleErrorMessage=0;
	sCurError=s;

	bHasError=1;
	SendStringToConfig(s);
	OutToDebug("Error: %s\n",s);
	Message(String(s));

	if(pByteCode)
	{
		//int nLine=pByteCode->CodeList[nCurLine].nNumberLine;
		//todo: все рушится
		//DoDebugLoop(pByteCode->csModuleName,nLine+1,AfxGetCurrentRunContext());//даем возможность пользователю посмотреть лок. переменные
	}
	if(bStopDebug)
		bStopDebug=-1;//для останова Eval функции клавишей Escape

	
	//sCurError="";
	ResetByteCode();//Обнуление стека

	//throw (new CTranslateError());
}


void Error(int nErr,CString s1,int d2)
{
	if(nErr>=0&&nErr<LastError)
	{
		Error(aErrors[nErr],s1,d2);
	}
	Error(aErrors[ERROR_SYS1],nErr);
}


//////////////////////////////////////////////////////////////////////
// Системные функции
//////////////////////////////////////////////////////////////////////
bool FileExist(CString sFileName)
{
	WIN32_FIND_DATA data;
	HANDLE hFile=FindFirstFile(sFileName,&data);
	if(hFile==INVALID_HANDLE_VALUE||hFile==0)
	{
		return FALSE;
	}
	FindClose(hFile);
	return TRUE;
}
char *LoadFromFileBin(CString sFileName,int &size)
{
	if(FileExist(sFileName))
	{
		HANDLE hFile=CreateFile(sFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if(hFile)
		{
			DWORD dwRead;
			size=GetFileSize(hFile,NULL);
			if(size>=0)
			{
				char *pBuf=new char [size+1];
				pBuf[size]=0;
				if(!ReadFile(hFile,pBuf,size,&dwRead,NULL))
				{
					CloseHandle (hFile);
					Error(ERROR_FILE_READ,sFileName.GetBuffer(0));
				}
				CloseHandle (hFile);
				return pBuf;
			}
		}
	}

	//Error(ERROR_FILE_OPEN,sFileName.GetBuffer(0));
	return NULL;
}
CString LoadFromFile(CString csFileName)
{
	int size; 
	char *s=LoadFromFileBin(csFileName,size);
	CString csStr;
	memcpy(csStr.GetBuffer(size),s,size);
	csStr.ReleaseBuffer(size);
	if(s)
		delete []s;
	return csStr;
}
CString GetModuleText(CString csPath,BOOL &bError)
{
	bError=0;
#ifndef SMALL_TRANSLATE
	ASSERT(AfxGetModuleManager());

	try
	{
	return AfxGetModuleManager()->GetModule(csPath);
	}
	catch(...)
	{
		bError=1;
	}
#endif
	return "";
/*
	CMetaObject *pObject=AfxGetModuleManager()->pMetadata->GetMetaObject(csPath,"",1);
	if(!pObject)
#endif
	{
		csPath.Replace("/","\\");
		if(!FileExist(csPath))
		{
			bError=1;
			return "";
		}
		return LoadFromFile(csPath);
	}
#ifndef SMALL_TRANSLATE
	else
	{
		return pObject->csFile;
	}
#endif
*/
}

void WritoToFile(CString sFileName,CString Str)
{
	HANDLE hFile=CreateFile(sFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,NULL,NULL);
	if(hFile)
	{
		DWORD dwWrite=Str.GetLength();
		WriteFile(hFile,Str.GetBuffer(0),dwWrite,&dwWrite,NULL);
		CloseHandle (hFile);
	}
}
void WritoToFileText(CString csCFile,CString Str)
{
	if(FILE *sFile=fopen(csCFile,"wt"))
	{
		fprintf(sFile,"%s",Str);
		fclose(sFile);
	}
}


char *strmid(char *sSource,int nStart,int nLen)
{
	char *sRes=0;
	int nSize=nLen;
	if(nStart>=0)
	{
		if(nSize==0)
		{
			nSize=strlen(sSource)-nStart+1;
		}
		sRes=new char[nSize+1];
		memcpy(sRes,&sSource[nStart],nSize);
		sRes[nSize]=0;
	}
	return sRes;
}
char *strupper(char *s)
{
	return _tcsupr(s);
}



CString FindErrorCodeLine(CString &Buffer,int nCurPos,int nOnlyCodeLine)
{
	int nSizeText=Buffer.GetLength();
	if(nCurPos>=nSizeText)
		nCurPos=nSizeText-1;
	if(nCurPos<0)
		nCurPos=0;

	int nStart=0;
	for(int i=nCurPos;i>0;i--)//ищем начало строки в которой выдается сообщение об ошибке трансляции
		if(Buffer[i]=='\n')
		{
			nStart=i+1;
			break;
		};
	int nEnd=nSizeText;
	for(i=nCurPos+1;i<nSizeText;i++)//ищем конец строки в которой выдается сообщение об ошибке трансляции
		if(Buffer[i]=='\n')
		{
			//nEnd=i-1;
			nEnd=i;
			break;
		};

	//определяем номер строки
	int nNumber=1+Buffer.Left(nStart).Replace("\n","\n");

	CString strError;
	if(nOnlyCodeLine)//это режим поиска текущей строки программы
	{
		if(2==nOnlyCodeLine)
		{
			strError=Buffer.Mid(nStart,nEnd-nStart);
			strError.Replace("\r","");
			strError.Replace("\t"," ");
		}
		else
		if(3==nOnlyCodeLine)
		{
			strError.Format("(#Строка %d):     %s",nNumber,Buffer.Mid(nStart,nEnd-nStart));
			strError.Replace("\r","");
			strError.Replace("\t"," ");
		}
		else
		{
			strError.Format("(#Строка %d)::%s",nNumber,Buffer.Mid(nStart,nEnd-nStart));
			strError.Replace("\r","");
			strError.Replace("\t"," ");
		}
	}
	else
	{
		if(afxMessageFromConfig)
			strError.Format("%s <<?>> %s",Buffer.Mid(nStart,nCurPos-nStart),Buffer.Mid(nCurPos,nEnd-nCurPos));
		else
			strError.Format("(#Строка %d):\n%s <<?>> %s",nNumber,Buffer.Mid(nStart,nCurPos-nStart),Buffer.Mid(nCurPos,nEnd-nCurPos));
	}


	strError.TrimLeft();
	strError.TrimRight();
	return strError;
}
/*
CString mUpper2(CString Str)
{
	if(Str.Find(' ')>=0)
		Str.Replace(" ","");
	Str.MakeUpper();
	return Str;
}

/*
CString mUpper(CString Str)
{
	Str.TrimRight();
	Str.TrimLeft();
	Str.MakeUpper();
	return Str;
}
*/



//#pragma comment (lib, "user32.lib")
typedef VOID (WINAPI *MYPROC)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

/*
Чтобы сделать прозрачным все приложение - вызываем эту функцию из InitInstance() приложения и передаем ей хандлер главной рамки
setLayeredWindow( m_pMainWnd->m_hWnd, 200 );

Для диалоговых окон - соответственно из OnInitDialog() и m_hWnd диалога.
setLayeredWindow( m_hWnd );
*/
void setLayeredWindow( HWND hWnd, BYTE ab)
{
	LONG st = ::GetWindowLong( hWnd, GWL_EXSTYLE );
	::SetWindowLong( hWnd, GWL_EXSTYLE, st | 0x00080000 /*WS_EX_LAYERED*/ );
	
	HMODULE user32 = LoadLibrary( "User32.dll" );
	if ( user32 )
	{
		MYPROC ProcAdd = (MYPROC)GetProcAddress( user32, "SetLayeredWindowAttributes" );
		if ( ProcAdd )
			(ProcAdd)(hWnd, RGB(0x80, 0x80, 0x80), ab, 2/*LWA_ALPHA*/);
		
		FreeLibrary( user32 );
	}
}
void RunModule(CString Str)
{
	try
	{
		CCompileModule cModule;
		cModule.Compile(Str);

		CProcUnit cRun;
		cRun.Exec(cModule.cByteCode);
	}
	catch(CTranslateError *)
	{
		AfxMessageBox(sCurError);
	}
}

CValue Eval(CString StrRun)
{
	return CProcUnit::Eval(StrRun);
}


/////////////////////////////////////////////////////////////////////////////
//Функции работы с паролями
/////////////////////////////////////////////////////////////////////////////
CString GetHash(CString csPassword)
{
	CString csHash="",csHash2;
	for(int i=0;i<csPassword.GetLength();i++)
	{
		srand(i);
		csHash2.Format("%s%3d",csHash,(rand()%256)^(char) csPassword[i]^0xFF);
		csHash=csHash2;
	}
	csHash.Replace(" ","0");
	return csHash;
}
CString GetPassword(CString csHash)
{
	CString csPassword,csPassword2;
	for(int i=0;i<csHash.GetLength()/3;i++)
	{
		srand(i);
		CString str=csHash.Mid(i*3,3);
		byte c=atoi(str);
		csPassword2.Format("%s%c",csPassword,(rand()%256)^c^0xFF);
		csPassword=csPassword2;
	}
	csHash.Replace(" ","0");
	return csPassword;
}

//#include "Windows.h"
int IsWindows5()
{
	static int bIsWindows2000orLater=-1;
	if(-1==bIsWindows2000orLater)
	{
		OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx (&osvi);
		bIsWindows2000orLater = osvi.dwMajorVersion >= 5;
	}
	return bIsWindows2000orLater;
}


CString GetGetLastStringError()
{
	CString csRes;
	DWORD dwLastError=GetLastError();

    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //

    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //

    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
		csRes=MessageBuffer;

        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }
	return csRes;
}



//Поддержка проверки прав доступа.
//Автор: "A.S.Bobylkin" <asbobylk@svsokol.lipetsk.ru> 
//Проверка вставляется в модулях:
//SystemFunctions.cpp::OpenForm()
//DocUnit.cpp::OnOpenDocument()
//MainFrm.cpp::OnSelectMenu()
//--------------------------------------------------------------
BOOL GrantCheck(CString csPathName)
{
	try
	{
		CProcUnit *pGModuleCheckRights;
		ASSERT(AfxGetModuleManager());
		pGModuleCheckRights=AfxGetModuleManager()->GetRunModule(MODULESNAME+"\\Поддержка проверки прав доступа",0,0);
		CValue Right="Использование";
		CValue PathName=String(csPathName);
		CValue CheckRights=pGModuleCheckRights->CallFunction("ПравоДоступа",Right,PathName);
		if (CheckRights.GetString()=="-1")
		{
			//Проверка прав не включена
		}	
		else 
		{
			if (CheckRights.GetString()=="1")
			{
				//Проверка успешно пройдена
			} 
			else 
            {
				//Проверка не пройдена
				if (CheckRights.GetString()=="0")
				{
					AfxMessageBox("Недостаточно прав доступа!..",MB_OK | MB_ICONSTOP|MB_APPLMODAL);
					return 0;
				}
			}	
		}
	}
	catch(...) 
	{
		//Message(CString("Ошибка при проверке прав доступа. Возможно, отсутствует Общий модуль <Поддержка проверки прав доступа> ("+CTranslateError::GetErrorMessage()+")"));
	} 
	
	return 1;
}
//-----------------------------------------------------
CValueArray aEvents;
int RegisterEvent(CString csEventName)//ЗарегистрироватьСобытие
{
	//AfxMessageBox("регистрация события: "+p[0]->GetString(),MB_OK | MB_ICONSTOP|MB_APPLMODAL);
	if (aEvents.FindId(csEventName))
	{
		//AfxMessageBox("Не зарегистрировано событие: "+p[0]->GetString(),MB_OK | MB_ICONSTOP|MB_APPLMODAL);
		return 0;
	}
	//CValue aEventName;
	//aEventName.CreateObject("Массив");
	//CValueArray *pEventName=(CValueArray *)aEventName.pRef;
	//pEventName->SetAt(1,1);

	//aEvents.SetAt(csEventName,aEventName);
	aEvents.SetAt(csEventName,CValueArray());
	//Message(CValue("Зарегистрировано событие: "+csEventName));
	return 1;
}

int NotifyEvent(CString csEventName, CValue **p)//ВызватьСобытие
{
	CValue aEventName;
	aEventName.CreateObject("Массив");
	CValueArray *pEventName=(CValueArray *)aEventName.pRef;
	if(aEvents.GetAt(csEventName).GetType()==TYPE_EMPTY) return 0;
	pEventName->Load(aEvents.GetAt(csEventName),0,0);
	int nDimStrSize=pEventName->GetSizeStrArray(); //КоличествоОбработчиков
	//Message(CValue("Кол. обработчиков: "+CValue(nDimStrSize)));
	if (nDimStrSize==0)
	{
		//Message(CValue("Cобытие: "+p[0]->GetString()+" не существует"));
		return -1;
	}
	pEventName->Sort();
	
	CValue *aP[7];
	int ii=0;
	while(p[ii]->nType!=TYPE_EMPTY)
	{
		aP[ii]=p[ii];
		ii++;
	}
	int nDimSize=ii; //КоличествоПараметров
	CString csFunctionName;
	CValue CheckEnd=CValue(0);
	for (int j=1;j<=nDimStrSize;j++)
	{
		
		csFunctionName=pEventName->GetIdentifierByNumber(j-1);
		CString csModuleName=Mid(aEventName.GetAt(csFunctionName),3,0);
		CProcUnit *pModule;
		ASSERT(AfxGetModuleManager());
		pModule=AfxGetModuleManager()->GetRunModule(csModuleName,0,0);
		CheckEnd=pModule->CallFunction(csFunctionName,aP,nDimSize);
		//Message(CValue("-Cобытие: "+p[0]->GetString()+" вызов обработчика "+csFunctionName+" из модуля:"+csModuleName));
		if (CheckEnd.GetNumber()!=0) break;
	}

	return CheckEnd;
}
