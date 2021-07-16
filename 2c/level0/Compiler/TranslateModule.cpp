// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// TranslateModule.cpp: implementation of the CTranslateModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TranslateModule.h"
#include "Definition.h"

extern BOOL afxMessageFromConfig;
CString LoadFromFile(CString csFileName);
CString GetModuleText(CString csPath,BOOL &bError);
extern CMapStringToPtr	aHelpDescription;//описание ключевых слов и системных функций
extern CMapStringToPtr	aHashKeywordList;
extern CDefList glDefList;//глобальный массив определений

//#ifdef MYDEBUG
//	#define MYDEBUGOUT
//#endif

//#include "\\Gena\YLINTERPRETE\InstructionList.h"
//#include "InstructionList.h"
 
//////////////////////////////////////////////////////////////////////
// Global array
//////////////////////////////////////////////////////////////////////
struct aKeyWordsDef aKeyWords[] = 
{
	{"Если","if"},
	{"Тогда","then"},
	{"Иначе","else"},
	{"ИначеЕсли","elseif"},
	{"КонецЕсли","endif"},
	{"Для","for"},
	{"По","to"},
	{"Цикл","do"},
	{"КонецЦикла","enddo"},
	{"Пока","while"},
	{"Перейти","goto"},
	{"Не","not"},
	{"И","and"},
	{"ИЛИ","or"},
	{"Процедура","procedure"},
	{"КонецПроцедуры","endprocedure"},
	{"Функция","function"},
	{"КонецФункции","endfunction"},
	{"Далее","forward"},
	{"Экспорт","export"},
	{"Знач","val"},
	{"Возврат","return"},
	{"Попытка","try"},
	{"Исключение","except"},
	{"КонецПопытки","endtry"},
	{"Продолжить","continue"},
	{"Прервать","break"},
	{"ВызватьИсключение","raise"},
	{"Перем","var"},
	//расширение языка 1с:
	//	{"Класс","class"},
	//	{"КонецКласса","endclass"},
	{"List","List"},
	{"Виртуально","virtual"},
//	{"Статично","static"},
	{"",""},
	//{"#Импортировать","#import"},
	{"#Загрузить","#include","#Загрузить \"Имя\" - загрузка текста файла или произвольного модуля"},
	{"#ЗагрузитьИзФайла","#LoadFromFile","#ЗагрузитьИзФайла \"Имя\" - загрузка текста файла или произвольного модуля"},
	{"#Задать","#define","#Задать Идентификатор <Выражение> - объявление произвольногшо выражения в качестве краткого идентификатора"},
	{"#Удалить","#undef","#Удалить Идентификатор - отмена объявления идентификатора"},
	{"#Если","#ifdef","#Если Идентификатор - компилирование текста при наличии ранее объявленного идентификатора"},
	{"#ЕслиНе","#ifndef","#ЕслиНе Идентификатор - компилирование текста при отстутствии ранее объявленного идентификатора"},
	{"#Иначе","#else"},
	{"#КонецЕсли","#endif","#КонецЕсли - конец условного компилирования"},

	//Типизация переменных
/*	{"Число","Number"},
	{"Строка","String"},
	{"Дата","Date"},
	{"Массив","Array"},*/

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTranslateModule::CTranslateModule()
{
	//подготовка буфера ключевых слов
	if(HashKeyWords.GetCount()==0)//только один раз
		LoadKeyWords();
	Clear();

	aDefList=0;
	bAutoDeleteDefList=0;
	nModePreparing=LEXEM_ADD;
}

CTranslateModule::~CTranslateModule()
{
	if(bAutoDeleteDefList&&aDefList)
		delete aDefList;

}

//подготовка буфера ключевых слов
void CTranslateModule::LoadKeyWords()
{
	HashKeyWords.RemoveAll();
	for(int i=0;i<LastKeyWord;i++)
	{
		CString csRus=aKeyWords[i].Rus;
		csRus.MakeUpper();
		CString csEng=aKeyWords[i].Eng;
		csEng.MakeUpper();

		HashKeyWords[csRus]=(void*)(i+1);
		HashKeyWords[csEng]=(void*)(i+1);

		//добавляем в массив для синтаксического анализатора
		aHashKeywordList[mUpper(csRus)]=(void*)1;
		aHashKeywordList[mUpper(csEng)]=(void*)1;

		aHelpDescription[csRus]=aKeyWords[i].sShortDescription;
		aHelpDescription[csEng]=aKeyWords[i].sShortDescription;
	}
}

//////////////////////////////////////////////////////////////////////
// Translating
//////////////////////////////////////////////////////////////////////


//____________________________________________________________________
//Clear
//Назначение:
//Подготовить переменные для начала компиляции
//Возвращаемое значение:
//нет
void CTranslateModule::Clear()
{
	Buffer="";
	BUFFER="";
	nSizeText=0;
	nPos=0;
	nCurNumberLine=0;
}


//____________________________________________________________________
//LoadFile
//Назначение:
//Загрузить буфер исходным текстом из файла + подготовить переменные для компиляции
//Возвращаемое значение:
//нет
void CTranslateModule::LoadFile(char *sFileName)
{
//	Clear();
//	Load(LoadFromFileBin(sFileName,nSizeText));
//	BUFFER=Buffer;
//	BUFFER.MakeUpper();
}

//____________________________________________________________________
//SaveFile
//Назначение:
//Выгрузить массив байт-кодов в файл
//Возвращаемое значение:
//нет
void CTranslateModule::SaveToFile(char *sFileName)
{
	HANDLE hFile=CreateFile(sFileName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,NULL,NULL);
	if(!hFile)
		Error(ERROR_FILE_OPEN,sFileName);
	else
	{
		//Сигнатура файла
		CString csStr="Lexem";
//		DWORD dwWrite;
//		WriteFile(hFile,csStr.GetBuffer(0),csStr.GetLength(),&dwWrite,NULL);
/*
		for(int i=0;i<cByteCode.GetSize();i++)
		{
			CLexem *b=&cByteCode.Get(i);
			if(!b->SaveToFile(hFile))
			{
				Error(ERROR_FILE_WRITE,sFileName);
				break;
			}
		}
*/
		CloseHandle (hFile);
	}
}


//____________________________________________________________________
//Load
//Назначение:
//Загрузить буфер исходным текстом + подготовить переменные для компиляции
//Возвращаемое значение:
//нет
void CTranslateModule::Load(CString s)
{
	Clear();
	Buffer=s;
	Buffer.TrimRight();
	nSizeText=Buffer.GetLength();
	if(nSizeText==0)
		return;
	BUFFER=Buffer;
	BUFFER.MakeUpper();
}



//____________________________________________________________________
//SetError
//Назначение:
//Запомнить ошибку трансляции и вызвать исключение
//Возвращаемое значение:
//Метод не возвращает управление!
void CTranslateModule::SetError(int nErr,int nCurPos,CString s)
{
	CString strError;
	if(afxMessageFromConfig)
		strError=FindErrorCodeLine(Buffer,nCurPos,0);
	else
		strError.Format("%s %s",csModuleName,FindErrorCodeLine(Buffer,nCurPos,0));

	Error(nErr,strError.GetBuffer(0),s.GetBuffer(0));

}



//____________________________________________________________________
//SkipSpaces
//Назначение:
//Пропустить все незначащие пробелы из буфера ввода
//плюс комментарии из буфера ввода перенаправлять в буфер вывода
//Возвращаемое значение:
//НЕТ
void CTranslateModule::SkipSpaces(void)
{
	for(int i=nPos;i<nSizeText;i++)
	{
		char c=BUFFER[i];
		if(c!=' '&&c!='\t'&&c!='\n'&&c!='\r')
		{
			if(c=='/')//может это комментарий
			{
				if(i+1<nSizeText)
					if(BUFFER[i+1]=='/')//пропускаем комментарии
					{
						for(int j=i;j<nSizeText;j++)
						{
							nPos=j;
							if(BUFFER[j]=='\n'||BUFFER[j]==13)
							{
								//обрабатываем следующую строку
								SkipSpaces();
								return;
							}
						}
						i=nPos+1;
					}
			}
			nPos=i;
			break;
		}
		else
		{
			if(c=='\n')
				nCurNumberLine++;
		}
	}
	if(i==nSizeText)
		nPos=nSizeText;
}



//____________________________________________________________________
//IsByte
//Назначение:
//Проверить является ли следующий байт (без учета пробелов) равным
//ЗАДАННОМУ байту
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsByte(char c)
{
	SkipSpaces();
	if(nPos>=nSizeText)
		return false;
	if(BUFFER[nPos]==c)
		return true;
	return false;
}


//____________________________________________________________________
//GetByte
//Назначение:
//Получить из выборки байт (без учета пробелов)
//если такого байта нет, то генерится исключение
//Возвращаемое значение:
//Байт из буфера
char CTranslateModule::GetByte()
{
	SkipSpaces();
	if(nPos<nSizeText)
	{
		char c=Buffer[nPos];
		nPos++;
		return c;
	}
	SetError(ERROR_TRANSLATE_BYTE,nPos);
	return 0;
}


//____________________________________________________________________
//IsWord
//Назначение:
//Проверить (не изменяя позиции текущего крсора)
//является ли следующий набор букв словом (пропуская пробелы и пр.)
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsWord(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
	if(
		(BUFFER[nPos]=='_')||
		(BUFFER[nPos]>='A'&&BUFFER[nPos]<='Z')||
		(BUFFER[nPos]>='А'&&BUFFER[nPos]<='Я')||
		(BUFFER[nPos]=='#')
		)
		return true;
	return false;
}


//____________________________________________________________________
//GetWord
//Назначение:
//Выбрать из буфера следующее слово
//если слова нет (т.е. следующий набор букв не является словом), то генерится исключение
//Параметр: bGetPoint
//true - учитывать точку как составную часть слова (для получения числа константы)
//Возвращаемое значение:
//Слово из буфера
CString CTranslateModule::GetWord(bool bOrigin,bool bGetPoint,CString *psOrig)
{
	SkipSpaces();
	if(nPos>=nSizeText)
		SetError(ERROR_TRANSLATE_WORD,nPos);
	int nNext=nPos;
	for(int i=nPos;i<nSizeText;i++)
	{
		if(
			(BUFFER[i]=='_')||
			(BUFFER[i]>='A'&&BUFFER[i]<='Z')||
			(BUFFER[i]>='А'&&BUFFER[i]<='Я')||
			(BUFFER[i]>='0'&&BUFFER[i]<='9')||
			(BUFFER[i]=='#'&&i==nPos)||//если первый символ # - это служебное слово
			(BUFFER[i]=='.'&&bGetPoint)
			)
		{
			if(BUFFER[i]=='.'&&bGetPoint)
				bGetPoint=false;////точка должна встречаться только один раз
			nNext=i+1;
		}
		else
			break;
	}
	int nFirst=nPos;
	nPos=nNext;
	if(nFirst==nNext)
		SetError(ERROR_TRANSLATE_WORD,nFirst);
	if(bOrigin)
		return Buffer.Mid(nFirst,nNext-nFirst);
	else
	{
		if(psOrig)
			*psOrig=Buffer.Mid(nFirst,nNext-nFirst);
		return BUFFER.Mid(nFirst,nNext-nFirst);
	}
}



//____________________________________________________________________
//GetStrToEndLine
//Назначение:
//Получить всю строку до конца (символа 13 или конца кода программы)
//Строка
CString CTranslateModule::GetStrToEndLine(void)
{
	int nStart=nPos;
	for(int i=nPos;i<nSizeText;i++)
	if(Buffer[i]=='\r'||Buffer[i]=='\n')
	{
		i++;
		//nCurNumberLine++;
		break;
	}
	nPos=i;
	int nEnd=nPos;
	return Buffer.Mid(nStart,nEnd-nStart);
}

//____________________________________________________________________
//IsNumber
//Назначение:
//Проверить (не изменяя позиции текущего крсора)
//является ли следующий набор букв числом-константой (пропуская пробелы и пр.)
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsNumber(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
	if(
		(BUFFER[nPos]>='0'&&BUFFER[nPos]<='9')
//		||(BUFFER[nPos]=='.')
		)
		return true;
	return false;
}

//____________________________________________________________________
//GetNumber
//Назначение:
//Получить из выборки число
//если никакого число нет, то генерится исключение
//Возвращаемое значение:
//Число из буфера
CString CTranslateModule::GetNumber(void)
{
	if(!IsNumber())
		SetError(ERROR_TRANSLATE_NUMBER,nPos);
	return GetWord(false,true);
}

//____________________________________________________________________
//IsString
//Назначение:
//Проверить является ли следующий набор символов (без учета пробелов) строкой-константой, заключенной в кавычки
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsString(void)
{
	return IsByte('\"');
/*	if(IsByte('\"'))
		return 1;

	//смотрим - вдруг далее идут не числа - значит это не задание даты - а такая строковая константа
	nPos++;
	bool bRes=IsNumber();
	nPos--;
	return bRes;
*/
}

//____________________________________________________________________
//GetString
//Назначение:
//Получить из выборки строку, заключенную в кавычки
//если такой строки нет, то генерится исключение
//Возвращаемое значение:
//Строка из буфера
CString CTranslateModule::GetString(void)
{
	CString csRes;
	if(!IsString())
		SetError(ERROR_TRANSLATE_STRING,nPos);
	SkipSpaces();
	int nStart=0;
	for(int i=nPos+1;i<nSizeText-1;i++)
	{
		char c=Buffer[i];
		if(c=='\"')
		{
			i++;
			c=Buffer[i];
			if(c!='\"')
				break;
		}
		else//поддержка многострочного текста
		if(c=='\n')
			nStart=csRes.GetLength();
		else
		if(c=='|'&&nStart>0)
		{//удаление ненужной части
			csRes=csRes.Left(nStart);
			nStart=0;
			continue;
		}
		else
		if(c=='/'&&nStart>0)
		{
			if(Buffer[i+1]=='/')//это комментарий - пропускаем
			{
				i=Buffer.Find('\n',i+1);
				if(i<0)
				{
					i=nSizeText;
					break;
				}
				continue;
			}
		}
		else
		if(c!='\t'&&c!=' ')
			nStart=0;

		csRes+=CString(c);
	}
	nPos=i;
	return csRes;
}

//____________________________________________________________________
//IsDate
//Назначение:
//Проверить является ли следующий набор символов (без учета пробелов) датой-константой, заключенной в апострофы
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsDate(void)
{
	return IsByte('\'');
}

//____________________________________________________________________
//GetDate
//Назначение:
//Получить из выборки дату, заключенную в апострофы
//если такой даты нет, то генерится исключение
//Возвращаемое значение:
//Дата из буфера, заключенная в кавычки
CString CTranslateModule::GetDate(void)
{
	if(!IsDate())
		SetError(ERROR_TRANSLATE_DATE,nPos);

	int nCount=0;
	SkipSpaces();
	if(nPos>=nSizeText)
		SetError(ERROR_TRANSLATE_WORD,nPos);
	int nNext=nPos;
	for(int i=nPos;i<nSizeText;i++)
	{
	if(
		(nCount<2)
		)
	{
		if(BUFFER[i]=='\'')
			nCount++;
		nNext=i+1;
	}
		else
			break;
	}
	int nFirst=nPos;
	nPos=nNext;
	if(nFirst==nNext)
		SetError(ERROR_TRANSLATE_DATE,nFirst);
	CString sRes=Buffer.Mid(nFirst+1,nNext-nFirst-2);
	return sRes; 
}


//____________________________________________________________________
//IsEnd
//Назначение:
//Проверить признак окончания транслирования (т.е. дошли до конца буфера)
//Возвращаемое значение:
//true,false
bool CTranslateModule::IsEnd(void)
{
	SkipSpaces();
	if(nPos<nSizeText)
		return false;
	else
		return true;
}



//____________________________________________________________________
//IsKeyWord
//Назначение:
//Определяет явлеется ли заданное слово служебным оператором
//Возвращаемое значение,если:
//-1: нет
//бельше или равно 0: номер в списке служебных слов
int CTranslateModule::IsKeyWord(CString s)
{
	return ((int)HashKeyWords[s])-1;
}

/*
//____________________________________________________________________
//TranslateModule
//Назначение:
//Трансляция исходного текста в байт код
//Возвращаемое значение:
//true,false
bool CTranslateModule::PrepareCode()
{
	PrepareLexem();
	return true;
}
*/
//____________________________________________________________________
//PrepareLexem
//ПРОХОД1 - загрузка лексем для последующего быстрого доступа при распознавании
bool CTranslateModule::PrepareLexem()
{
	if(!aDefList)
	{
		aDefList=new CDefList();
		aDefList->SetParent(&glDefList);
		bAutoDeleteDefList=1;//признак, что массив с определениями создали мы (а не передан в качестве трансляции определений)
	}

	CString s;
	LexemList.RemoveAll();

	while(!IsEnd())
	{
		CLexem bytecode;
		bytecode.nStringNumber=nPos;//если в дальнейшем произойдет ошибка, то именно эту строку нужно выдать пользователю
		bytecode.nNumberLine=nCurNumberLine;
		bytecode.csModuleName=csModuleName;
		if(IsWord())
		{
			CString csOrig;
			s=GetWord(0,0,&csOrig);

			//обработка определений пользователя (#define)
			if(aDefList->HasDef(s))
			{
				CLexemList *pDef=aDefList->GetDef(s);
				for(int i=0;i<pDef->GetSize();i++)
				{
					CLexem lex=pDef->GetAt(i);
					lex.nStringNumber=nPos;
					lex.nNumberLine=nCurNumberLine;//для точек останова
					lex.csModuleName=csModuleName;
					LexemList.Add(lex);
				}

				continue;
			}

			int n=IsKeyWord(s);
			bytecode.vData=String(csOrig);
			if(n>=0)
			{
				bytecode.nType=KEYWORD;
				bytecode.nData=n;
			}
			else
			{
				bytecode.nType=IDENTIFIER;
			}
		}
		else
		if(IsNumber()||IsString()||IsDate())
		{
			bytecode.nType=CONSTANT;
			if(IsNumber())
			{
				bytecode.vData.SetNumber(GetNumber());

				int n=LexemList.GetSize()-1;
				if(n>=0)
				if(LexemList[n].nType==DELIMETER&&(LexemList[n].nData=='-'||LexemList[n].nData=='+'))
				{
					n--;
					if(n>=0)
					if(LexemList[n].nType==DELIMETER&&(LexemList[n].nData=='['||LexemList[n].nData=='('||LexemList[n].nData==','||LexemList[n].nData=='<'||LexemList[n].nData=='>'||LexemList[n].nData=='='))
					{
						n++;
						if(LexemList[n].nData=='-')
							bytecode.vData.fData=-bytecode.vData.fData;
						LexemList[n]=bytecode;
						continue;
					}
				}
			}
			else
			if(IsString())
				bytecode.vData.SetString(GetString());
			else
			if(IsDate())
				bytecode.vData.SetDate(GetDate());

			LexemList.Add(bytecode);
			continue;
		}
		else
		if(IsByte('~'))
		{
			s="";
			GetByte();//пропускаем разделитель и вспомог. символ метки (как лишние)
			continue;
		}
		else
		{
			s="";
			bytecode.nType=DELIMETER;
			bytecode.nData=GetByte();
			if(bytecode.nData<=13)
				continue;
		}
		bytecode.sData=s;


		if(bytecode.nType==KEYWORD)
		{
			if(bytecode.nData==KEY_INCLUDE||bytecode.nData==KEY_INCLUDE2)//подключение произвольного модуля (файла)
			{
				if(!IsString())
					SetError(ERROR_TRANSLATE_STRING,nPos);

				BOOL bError;
				CString csPath=GetString();
				csPath.Replace("/","\\");
				CString csCompile=GetModuleText(csPath,bError);

				if(bError)
				{
					SetError(ERROR_MODULE_NOT_FOUND,nPos);
				}

				if(!csCompile.IsEmpty())
				{
					CTranslateModule translate;
					translate.aDefList=aDefList;//устанавливаем массив определений
					translate.csModuleName=csPath;
					translate.Load(csCompile);
					translate.PrepareLexem();
					for(int i=0;i<translate.LexemList.GetSize()-1;i++)//без учета ENDPROGRAM
						LexemList.Add(translate.LexemList[i]);
				}
				continue;
			}
			else
			if(bytecode.nData==KEY_DEFINE&&nModePreparing!=LEXEM_ADDDEF)//задание произвольного идентификатора
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);

				CString csName=GetWord();
//				if(aDefList->HasDef(csName))
//					SetError(ERROR_IDENTIFIER_DUPLICATE,nPos);

				//результат транслирования добавляем в список определений
				if(LEXEM_ADD==nModePreparing)
					PrepareFromCurrent(LEXEM_ADDDEF,csName);
				else
					PrepareFromCurrent(LEXEM_IGNORE,csName);
				continue; 
			}
			else
			if(bytecode.nData==KEY_UNDEF)//удаление идентификатора
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);

				CString csName=GetWord();
				aDefList->RemoveDef(csName);
				continue;
			}
			else
			if(bytecode.nData==KEY_IFDEF||bytecode.nData==KEY_IFNDEF)//условное компилирование
			{
				if(!IsWord())
					SetError(ERROR_IDENTIFIER_DEFINE,nPos);
				CString csName=GetWord();
				BOOL bHasDef=aDefList->HasDef(csName);
				if(bytecode.nData==KEY_IFNDEF)
					bHasDef=!bHasDef;

				
				//транслируем весь блок пока не встретится #else или #endif
				int nMode=0;

				if(bHasDef)
					nMode=LEXEM_ADD;//результат транслирования добавляем в список лексем
				else
					nMode=LEXEM_IGNORE;//иначе игнорируем
				PrepareFromCurrent(nMode);

				if(!IsWord())
					SetError(ERROR_USE_ENDDEF,nPos);

				CString csWord=GetWord();
				if(IsKeyWord(csWord)==KEY_ELSEDEF)//вдруг #else
				{
					//еще раз транслируем
					if(!bHasDef)
						nMode=LEXEM_ADD;//результат транслирования добавляем в список лексем
					else
						nMode=LEXEM_IGNORE;//иначе игнорируем
					PrepareFromCurrent(nMode);

					if(!IsWord())
						SetError(ERROR_USE_ENDDEF,nPos);
					csWord=GetWord();
				}

				//Требуем #endif
				if(IsKeyWord(csWord)!=KEY_ENDIFDEF)
					SetError(ERROR_USE_ENDDEF,nPos);

				continue;
			}
			else
			if(bytecode.nData==KEY_ENDIFDEF)//конец условного компилирования
			{
				nPos=bytecode.nStringNumber;//здесь мы сохраняли предыдущее значение
				break;
			}
			else
			if(bytecode.nData==KEY_ELSEDEF)//"Иначе" условного компилирования
			{
				//возвращаемся на начала условного оператора
				nPos=bytecode.nStringNumber;//здесь мы сохраняли предыдущее значение
				break;
			}
		}

		LexemList.Add(bytecode);
	}
//	if(LexemList.GetSize()==0)
//		return false;

	CLexem bytecode;
	bytecode.nType=ENDPROGRAM;
	bytecode.nData=0;
	bytecode.nStringNumber=nPos;
	LexemList.Add(bytecode);

#ifdef MYDEBUGOUT
	ToDebug();
#endif
	return true;
}

void CTranslateModule::ToDebug()
{
#ifdef _DEBUG
	int i;
	OutToDebug("\nLexemList: %d\n",LexemList.GetSize());
	for(i=0;i<LexemList.GetSize();i++)
	{
		OutToDebug("%d.\t%d",i,LexemList[i].nType);
		OutToDebug("\t%c(%d)",LexemList[i].nData,LexemList[i].nData);
		OutToDebug("\t%s",LexemList[i].vData.GetString());
		OutToDebug("\t%s",LexemList[i].sData);
		OutToDebug("\t%d",LexemList[i].nStringNumber);
		OutToDebug("\n");
	}
#endif
}


CDefList::~CDefList()
{
	CLexemList *pArray;
	POSITION pos;
	CString csKey;
	for( pos = DefList.GetStartPosition(); pos != NULL; )
	{
		DefList.GetNextAssoc( pos, csKey, (void*&)pArray );
		if(pArray)
			pArray->RemoveAll();
	}
}
void CDefList::RemoveDef(CString &csName)
{
	DefList.RemoveKey(mUpper(csName));
}
BOOL CDefList::HasDef(CString &csName)
{
	CLexemList *p;
	BOOL bRes=DefList.Lookup(mUpper(csName),(void*&)p);
	if(bRes)
		return 1;

	static int nLevel=0;
	nLevel++;
	if(nLevel>MAX_OBJECTS_LEVEL)
		Error("Рекурсивный вызов модулей (#3)");

	//ищем в родителях
	bRes=0;
	if(pParent)
	{
		bRes=pParent->HasDef(csName);
	}

	nLevel--;
	return bRes;
}
CLexemList *CDefList::GetDef(CString &csName)
{
	CLexemList *p=(CLexemList *)DefList[mUpper(csName)];
	if(!p)
	{
		//ищем в родителях
		if(pParent)
			if(pParent->HasDef(csName))
				return pParent->GetDef(csName);

		p=new CLexemList();
		DefList[mUpper(csName)]=p;
	}
	return p;
}
void CDefList::SetDef(CString &csName,CLexemList *pDef)
{
	CLexemList *pList=GetDef(csName);
	pList->RemoveAll();
	if(pDef)
	for(int i=0;i<pDef->GetSize();i++)
		pList->Add(pDef->GetAt(i));
}
void CDefList::SetDef(CString &csName,CString csValue)
{
	CLexemList List;
	if(csValue.GetLength()>0)
	{
		CLexem Lex;
		Lex.nType=CONSTANT;
		if(csValue[0]=='-'||csValue[0]=='+'||(csValue[0]>='0'&&csValue[0]<='9'))//число
			Lex.vData.SetNumber(csValue);
		else
			Lex.vData.SetString(csValue);
		List.Add(Lex);
		SetDef(mUpper(csName),&List);
	}
	else
	{
		SetDef(mUpper(csName),NULL);
	}
}

void CDefList::SetParent(CDefList *p)
{
	pParent=p;
}

//создание лексем начиная с текущей позиции
void CTranslateModule::PrepareFromCurrent(int nMode,CString csName)
{

	CTranslateModule translate;
	translate.aDefList=aDefList;
	translate.nModePreparing=nMode;
	translate.csModuleName=csModuleName;
	translate.Load(Buffer);
	
	//начальный номер строки
	translate.nCurNumberLine=nCurNumberLine;
	translate.nPos=nPos; 

	//конечный номер строки
	if(nMode==LEXEM_ADDDEF)
	{
		GetStrToEndLine();
		translate.nSizeText=nPos;
	}

	translate.PrepareLexem();
	if(nMode==LEXEM_ADDDEF)
	{
		if(translate.LexemList.GetSize())
			translate.LexemList.SetSize(translate.LexemList.GetSize()-1);//без учета ENDPROGRAM

		aDefList->SetDef(csName,&translate.LexemList);
		//nCurNumberLine++;
		nCurNumberLine=translate.nCurNumberLine;
	}
	else
	if(nMode==LEXEM_ADD)
	{
		for(int i=0;i<translate.LexemList.GetSize()-1;i++)//без учета ENDPROGRAM
			LexemList.Add(translate.LexemList[i]);
		nPos=translate.nPos;
		nCurNumberLine=translate.nCurNumberLine;
	}
	else
	{
		nPos=translate.nPos;
		nCurNumberLine=translate.nCurNumberLine;
	}
}


void CTranslateModule::OnSetParent(CTranslateModule *pSetParent)
{
	if(!aDefList)
	{
		aDefList=new CDefList();
		aDefList->SetParent(&glDefList);
		bAutoDeleteDefList=1;//признак автоудаления
	}
	if(pSetParent)
		aDefList->pParent=pSetParent->aDefList;
	else
		aDefList->pParent=&glDefList;
}
