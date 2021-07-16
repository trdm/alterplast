// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// CompileModule.cpp: implementation of the CCompileModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CompileModule.h"
#include "Definition.h"

//////////////////////////////////////////////////////////////////////
//Constants
//////////////////////////////////////////////////////////////////////

CArray <CString,CString> aGlVariables;
CArray <CValue,CValue> aValueGlVariables;
CArray <CString,CString> aSysFunction;

CMapStringToPtr	aHelpDescription;//описание ключевых слов и системных функций
CMap <void*,void*,BOOL,BOOL> aSysFunctionAddr;


static CMapStringToOb StaticValList;//статические переменные
CMapStringToPtr	aHashKeywordList;

//#define TIME_DEBUG

//Массив приоритетов математических операций
int aPriority[256];

 

//#ifdef MYDEBUG
//	#define MYDEBUGOUT
//#endif

//Массив имен операция для отладки (для метода ToDebug)
char *aOperNames[]=
{
	"NOP",
	"ADD",
	"DIV",
	"MULT",
	"SUB",
	"NOT",
	"AND",
	"OR",
	"RET",
	"GOTO",
	"FOR",
	"IF",
	"LET",
	"CONST",
	"CONSTN",
	"NEXT",
	"MOD",
	"INVERT",
	"?",
	">",
	"==",
	"<",
	">=",
	"<=",
	"<>",
	"LIST",
	"TRY",
	"RAISE",
	"FUNC",
	"ENDFUNC",
	"CALL",
	"PARAM",//SET
	"SETREF",
	"SETCONS",
	"SYSCALL",
	"ADDCONS",
	"DIVCONS",
	"MULTC",
	"SUBCONS",
	"> CONST",
	"= CONST",
	"< CONST",
	">=CONST",
	"<=CONST",
	"<>CONST",
	"% CONST",
	"SET_ATTR",
	"GET_ATTR",
	"CALL_METH",
	"GET_MAS",
	"SET_MAS",
	"CHECK_MAS",
	"SIZE_MAS",
	"ENDTRY",
	"SET_TYPE",


	"END",
};




#ifdef TIME_DEBUG
UINT GetProcessorTimeMySQL(UINT nDiv);
int nFirstTime=0;
BOOL bUseDebugTime=0;
void InitTime()
{
	nFirstTime=GetProcessorTimeMySQL(10);
}
int GetDelta()
{
	int nDelata=GetProcessorTimeMySQL(10)-nFirstTime;
	InitTime();
	return nDelata;
}
void OutTime(int n)
{
	OutToDebug(" (CPU %d : %d)\n",n,GetDelta());
	InitTime();
}
#endif










//////////////////////////////////////////////////////////////////////
// CCompileContext CCompileContext CCompileContext CCompileContext
//////////////////////////////////////////////////////////////////////




//____________________________________________________________________
//Поиск переменной в хэш массиве
//Возвращает 1 - если переменная найдена
BOOL CCompileContext::FindVariable(CString csName)
{
//	int nCount=cVariables.GetCount();
	CVariable CurrentVar;
	return cVariables.Lookup(mUpper(csName),CurrentVar);
}

//____________________________________________________________________
//Добавляет новую переменную в список
//Возвращает добавленную переменную в виде SParam
SParam CCompileContext::AddVariable(CString csName,CString csType,BOOL bExport)
{
	if(!pModule->cByteCode.bOptimize2)
	if(FindVariable(csName))//было объявление + повторное объявление = ошибка
		pModule->SetError(ERROR_IDENTIFIER_DUPLICATE,csName);

	int nCountVar=cVariables.GetCount();
	CVariable CurrentVar;
	CurrentVar.bExist=1;
	CurrentVar.csName=csName;
	CurrentVar.bExport=bExport;
	CurrentVar.csType=csType;
	CurrentVar.nNumber=nCountVar;
	cVariables[csName]=CurrentVar;
	SParam Ret;
	Ret.csType=csType;
	Ret.nArray=0;
	Ret.nIndex=nCountVar;
	return Ret;
}


//____________________________________________________________________
//Функция возвращает номер переменной по строковому имени
//Поиск определения переменной, начиная с текущего контекста до всех родительских
//Если требуемой переменной нет, то создается новое определение переменной
SParam CCompileContext::GetVariable(CString csName,bool bFindInParent,bool bCheckError)
{
	int nCanUseLocalInParent=nFindLocalInParent;
	SParam Variable;
	//ASSERT(csName!="FFF");
	if(!FindVariable(csName))
	{

		if(bFindInParent)//ищем в родительских контекстах(модулях)
		{
			int nParentNumber=0;
			CCompileContext *pCurContext=pParent;
			CCompileContext *pNotParent=pStopParent;
			while(pCurContext)
			{
				nParentNumber++;
				if(nParentNumber>MAX_OBJECTS_LEVEL)
				{
					Message(pCurContext->pModule->csModuleName);
					if(nParentNumber>2*MAX_OBJECTS_LEVEL)
						Error("Рекурсивный вызов модулей!!!");
				}

				if(pCurContext==pNotParent)//текущий модуль != запрещенный прародитель
				{
					//проверяем следующий родитель
					pNotParent=pCurContext->pParent;
					if(pNotParent==pContinueParent)//начиная со следующего - нет запрещенных родителей
						pNotParent=0;
				}
				else
				{
					if(pCurContext->FindVariable(csName))//нашли
					{
						CVariable CurrentVar=pCurContext->cVariables[csName];
						//смотрим это экспортная переменная или нет (если nFindLocalInParent=1, то можно взять локальные переменные родителя)
						if(nCanUseLocalInParent>0||CurrentVar.bExport)
						{
							//определяем номер переменной
							Variable.nArray=nParentNumber;
							Variable.nIndex=CurrentVar.nNumber;
							Variable.csType=CurrentVar.csType;
							return Variable;
						}
					}
				}
				nCanUseLocalInParent--;
				pCurContext=pCurContext->pParent;
			}
		}

		if(bCheckError)//выводим сообщение об ошибке
			pModule->SetError(ERROR_VAR_NOT_FOUND,csName);
			

		//не было еще объявления переменной - добавляем
		AddVariable(csName);
	}

	//определяем номер и тип переменной
	CVariable CurrentVar=cVariables[csName];
	Variable.nArray=0;
	Variable.nIndex=CurrentVar.nNumber;
	Variable.csType=CurrentVar.csType;
	return Variable;
}

//____________________________________________________________________
//Связываение операторов GOTO с метками
void CCompileContext::DoLabels()
{
	ASSERT(pModule);
	for(int i=0;i<cLabels.GetSize();i++)
	{
		CString csName=cLabels[i].csName;
		int nLine=cLabels[i].nLine;

		//ищем такую метку в списке объявленных меток
		void *hLine=cLabelsDef[csName];
		if(!hLine)
		{
			pModule->nCurrentCompile=cLabels[i].nError;
			pModule->SetError(ERROR_LABEL_DEFINE,csName);//произошло дублированное определения меток
		}
		//записываем адрес перехода:
		pModule->cByteCode.CodeList[nLine].Param1.nIndex=(int)hLine;
	}
};

//Создание списка для записи строк байт-кода, в которых встретились команды Continue и Break
void CCompileContext::StartDoList()
{
	//создаем списки для команд Continue и Break (в них будут хранится адреса байт кодов, где встретились соответствующие команды)
	nDoNumber++;
	aContinueList[nDoNumber]=new CDefIntList();
	aBreakList[nDoNumber]=new CDefIntList();
};

//Установка адресов перехода для команд Continue и Break
void CCompileContext::FinishDoList(CByteCode	&cByteCode,int nGotoContinue,int nGotoBreak)
{
	int i;
	CDefIntList *pListC=(CDefIntList *)aContinueList[nDoNumber];
	CDefIntList *pListB=(CDefIntList *)aBreakList[nDoNumber];
	if(pListC==0||pListB==0)
	{
		OutToDebug("Error (FinishDoList) nGotoContinue=%d, nGotoBreak=%d\n",nGotoContinue,nGotoBreak);
		OutToDebug("nDoNumber=%d\n",nDoNumber);
		nDoNumber--;
		return;
	}
	for(i=0;i<pListC->GetSize();i++)
		cByteCode.CodeList[pListC->GetAt(i)].Param1.nIndex=nGotoContinue;
	for(i=0;i<pListB->GetSize();i++)
		cByteCode.CodeList[pListB->GetAt(i)].Param1.nIndex=nGotoBreak;

	aContinueList.RemoveKey(nDoNumber);
	aContinueList.RemoveKey(nDoNumber);
	delete pListC;
	delete pListB;
	nDoNumber--;
};

CCompileContext::~CCompileContext()
{
	CFunction *pFunction;
	POSITION pos;
	CString csFuncName;
	//Удаляем определения всех функций:
	for( pos = cFunctions.GetStartPosition(); pos != NULL; )
	{
		cFunctions.GetNextAssoc( pos, csFuncName, (void*&)pFunction );
		if(pFunction)
			delete pFunction;
	}
	cFunctions.RemoveAll();

}














//////////////////////////////////////////////////////////////////////
// CCompileModule CCompileModule CCompileModule CCompileModule
//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Construction/Destruction CCompileModule
//////////////////////////////////////////////////////////////////////

CCompileModule::CCompileModule()
{
	pParent=0;
	pStopParent=0;
	pContinueParent=0;
	bExpressionOnly=0;
	nCommonModule=0;
	bIsObject=0;
	bExtendMode=0;

	cContext.nFindLocalInParent=0;//у родительских контекстов локальные переменные не ищем!

	ZeroMemory(aPriority,sizeof(aPriority));
	aPriority['+']=10;
	aPriority['-']=10;
	aPriority['*']=30;
	aPriority['/']=30;
	aPriority['%']=30;
	aPriority['!']=50;

	aPriority[KEY_OR]=1;
	aPriority[KEY_AND]=2;


	aPriority['>']=3;
	aPriority['<']=3;
	aPriority['=']=3;

	cTypeList["ЧИСЛО"]="NUMBER";
	cTypeList["NUMBER"]="NUMBER";
	cTypeList["СТРОКА"]="STRING";
	cTypeList["STRING"]="STRING";
	cTypeList["ДАТА"]="DATE";
	cTypeList["DATE"]="DATE";

	pContext=GetContext();
}

CCompileModule::~CCompileModule()
{
	Clear();
	
}
void CCompileModule::Clear()
{
	for(int i=0;i<apCallFunctions.GetSize();i++)
		delete apCallFunctions[i];
	apCallFunctions.RemoveAll();
	cByteCode.Clear();
}

//____________________________________________________________________
//SetError
//Назначение:
//Запомнить ошибку трансляции и вызвать исключение
//Возвращаемое значение:
//Метод не возвращает управление!
void CCompileModule::SetError(int nErr,CString s)
{
	int nStringNumber=0;
	if(nCurrentCompile>=LexemList.GetSize())
		nCurrentCompile=LexemList.GetSize()-1;
	if(nCurrentCompile<0)
		nCurrentCompile=0;

	if(nCurrentCompile>=0)
	if(nCurrentCompile<LexemList.GetSize())
		nStringNumber=LexemList.GetAt(nCurrentCompile).nStringNumber;


	//Clear();
	CTranslateModule::SetError(nErr,nStringNumber,s);
}
//другой вариант функции
void CCompileModule::SetError(int nErr,char c)
{
	CString Str;
	Str.Format("%c",c);
	SetError(nErr,Str);
}


//////////////////////////////////////////////////////////////////////
// Compiling
//////////////////////////////////////////////////////////////////////

//добавление в байт код информации о текущей строке
void CCompileModule::AddLineInfo(CByte &code)
{
	if(nCurrentCompile>=0)
	if(nCurrentCompile<LexemList.GetSize())
	{
		code.nStringNumber=LexemList[nCurrentCompile].nStringNumber;
		code.nNumberLine=LexemList[nCurrentCompile].nNumberLine;
		code.csModuleName=LexemList[nCurrentCompile].csModuleName;
	}

#ifdef TIME_DEBUG
//	if(bUseDebugTime)
//		code.nNumberLine=GetDelta();
#endif
}

//____________________________________________________________________
//GetLexem
//Назначение:
//Получить следующую лексему из списка байт кода и увеличть счетчик текущей позиции на 1
//Возвращаемое значение:
//0 или указатель на лексему
CLexem CCompileModule::GetLexem()
{
	CLexem lex;
	if(nCurrentCompile+1<LexemList.GetSize())
	{
		nCurrentCompile++;
		lex=LexemList[nCurrentCompile];
	}
	return lex;
}

//Получить следующую лексему из списка байт кода без увеличения счетчика текущей позиции
CLexem CCompileModule::PreviewGetLexem()
{
	CLexem lex;
	while(true)
	{
		lex=GetLexem();
		if(!(lex.nType==DELIMETER&&lex.nData==';'))
			break;
	}
	nCurrentCompile--;
	return lex;
}

//____________________________________________________________________
//GETLexem
//Назначение:
//Получить следующую лексему из списка байт кода и увеличть счетчик текущей позиции на 1
//Возвращаемое значение:
//нет (в случае неудачи генерится исключение)
CLexem CCompileModule::GETLexem()
{
	CLexem lex=GetLexem();
	if(lex.nType==ERRORTYPE)
		SetError(ERROR_CODE_DEFINE);
	return lex;
}
//____________________________________________________________________
//GETDelimeter
//Назначение:
//Получить следующую лексему как заданный разделитель
//Возвращаемое значение:
//нет (в случае неудачи генерится исключение)
void CCompileModule::GETDelimeter(char c)
{
	CLexem lex=GETLexem();
	if(!(lex.nType==DELIMETER&&lex.nData==c))
		SetError(ERROR_DELIMETER,c);
}
//____________________________________________________________________
//IsNextDelimeter
//Назначение:
//Проверить является ли следующая лексема байт-кода заданным разделителем
//Возвращаемое значение:
//true,false
bool CCompileModule::IsNextDelimeter(char c)
{
	if(nCurrentCompile+1<LexemList.GetSize())
	{
		CLexem lex=LexemList[nCurrentCompile+1];
		if(lex.nType==DELIMETER&&lex.nData==c)
			return true;
	}
	return false;
}

//____________________________________________________________________
//IsNextKeyWord
//Назначение:
//Проверить является ли следующая лексема байт-кода заданным ключевым словом
//Возвращаемое значение:
//true,false
bool CCompileModule::IsNextKeyWord(int nKey)
{
	if(nCurrentCompile+1<LexemList.GetSize())
	{
		CLexem lex=LexemList[nCurrentCompile+1];
		if(lex.nType==KEYWORD&&lex.nData==nKey)
			return true;
	}
	return false;
}

//____________________________________________________________________
//GETKeyWord
//Получить следующую лексему как заданное ключевое слово
//Возвращаемое значение:
//нет (в случае неудачи генерится исключение)
void CCompileModule::GETKeyWord(int nKey)
{
	CLexem lex=GETLexem();
	if(!(lex.nType==KEYWORD&&lex.nData==nKey))
	{
		CString Str;
		Str.Format("%s (%s)",aKeyWords[nKey].Eng,aKeyWords[nKey].Rus);
		SetError(ERROR_KEYWORD,Str);
	}
}

//____________________________________________________________________
//GETIdentifier
//Получить следующую лексему как заданное ключевое слово
//Возвращаемое значение:
//строка-идентификатор
CString CCompileModule::GETIdentifier(int nOrig)
{
	CLexem lex=GETLexem();
	if(lex.nType!=IDENTIFIER)
	{
		if(nOrig&&lex.nType==KEYWORD)
			return lex.sData;

		SetError(ERROR_IDENTIFIER_DEFINE);
	}
	if(nOrig)
		return lex.vData.sData;
	else
		return lex.sData;
}

//____________________________________________________________________
//GETConstant
//Получить следующую лексему как константу
//Возвращаемое значение:
//константа
CValue CCompileModule::GETConstant()
{
	CLexem lex;
	int bNumRequire=0;
	if(IsNextDelimeter('-')||IsNextDelimeter('+'))
	{
		bNumRequire=1;
		if(IsNextDelimeter('-'))
			bNumRequire=-1;
		lex=GETLexem();
	}

	lex=GETLexem();
	if(lex.nType!=CONSTANT)
		SetError(ERROR_CONST_DEFINE);
	if(bNumRequire)
	{//проверка на то чтобы константа имела числовой тип
		if(lex.vData.nType!=TYPE_NUMBER)
			SetError(ERROR_CONST_DEFINE);
		//меняем знак при минусе
		if(bNumRequire==-1)
			lex.vData.fData=-lex.vData.fData;
	}
	return lex.vData;
}

//получение номера константой строки (для определения номера метода)
int CCompileModule::GetConstString(CString csMethod)
{
	int nNumber;
	if(!HashConstList[csMethod])
	{
		nNumber=cByteCode.ConstList.Add(String(csMethod));
		HashConstList[csMethod]=(void*)(nNumber+1);
	}
	nNumber=((int)HashConstList[csMethod])-1;
	return nNumber;
}

//удаление списка системных функций
void CCompileModule::ClearSystemFunctions()
{
	//Удаляем определения системных функций:
	CFunction *pFunction;
	POSITION pos;
	CString csFuncName;
	for( pos = cSysFunctions.GetStartPosition(); pos != NULL; )
	{
		cSysFunctions.GetNextAssoc( pos, csFuncName, (void*&)pFunction );
		if(pFunction)
			delete pFunction;
	}
	cSysFunctions.RemoveAll();

}

//загрузка системных функций (в основном вызывается автоматически из загрузчика CAutoLoader)
void AddSystemFunctions(SCallFunction *pSysArray)
{
	int i=0;
	SCallFunction data;
	
	DEFFUNCTION prevFunc=0;
	char *prevDesc;
	while((data=pSysArray[i]).Func)
	{
		CString csFuncName=data.sName;

		//добавляем в массив для синтаксического анализатора
		aHashKeywordList[mUpper(csFuncName)]=(void*)1;

		if(!aSysFunctionAddr[(void*)data.Func])
		{
			aSysFunctionAddr[(void*)data.Func]=1;
			aSysFunction.Add(csFuncName);
		}
		csFuncName.MakeUpper();
		if(data.sShortDescription)
		{
			prevFunc=data.Func;
			prevDesc=data.sShortDescription;
			aHelpDescription[csFuncName]=prevDesc;
		}
		else
		{
			if(prevFunc==data.Func)
			{   //уже было описание
				aHelpDescription[csFuncName]=prevDesc;
			}
			else
			{   //создаем описание сами - для подсказки числа параметров
				CString &Str=*(new CString(data.sName));//todo: утечка памяти
				Str=Str+"(";
				for(int i=0;i<data.nCountParams;i++)
				{
					CString csParam;
					csParam.Format("Знач%d,",i+1);
					Str+=csParam;
				}
				Str.TrimRight(',');
				Str+=")";

				prevFunc=data.Func;
				prevDesc=Str.GetBuffer(0);
				aHelpDescription[csFuncName]=prevDesc;
			}
		}


		int nCountParams=data.nCountParams;

		CFunction *pFunction=new CFunction(csFuncName,0);
		pFunction->nStart=(int)(void*)data.Func;
		CCompileModule::cSysFunctions[csFuncName]=pFunction;
		pFunction->bSysFunction=1;

		CParamVariable Variable;
		Variable.vData.nArray=DEF_VAR_DEFAULT;
		Variable.vData.nIndex=DEF_VAR_DEFAULT;
		for(int n=0;n<nCountParams;n++)
		{
			pFunction->aParamList.Add(Variable);
		}

		i++;
	}
}
void CCompileModule::LoadSystemFunctions()
{
	ClearSystemFunctions();
	AddSystemFunctions(DefSystemFunctionsArray);
}


//____________________________________________________________________
//AddVariable
//Назначение:
//Добавить имя и адрес внешней перменной в специальный массив для дальнейшего использования
void CCompileModule::AddVariable(CString csName,CValue *pValue)
{
	if(cByteCode.aExternValue.GetSize()==0)
	{
		cContext.GetVariable("КОНТЕКСТ",0,0);//контекст самого модуля
		cByteCode.aExternValue.Add(0);

		if(csModuleName.Right(ENTERPRISEMODULENAME.GetLength())==ENTERPRISEMODULENAME)
			AddVariable("БазовыйОбъект",0);
	}

	if(csName.IsEmpty())
		return;

	csName.MakeUpper();
	//учитываем внешние переменные при компиляции
	cContext.GetVariable(csName,0,0);
	//в дальнейшем учтем их при выполнении - они проецируются на начало списка локальных переменных начиная с 0
	cByteCode.aExternValue.Add(pValue);
}
//____________________________________________________________________
//Compile
//Назначение:
//Трасляция и компиляция исходного кода в байт-код (объектный код)
//Возвращаемое значение:
//true,false
bool CCompileModule::Compile(CString csStrModule)
{
	pContext=GetContext();//контекст самого модуля
	cByteCode.csModuleName=csModuleName;
	cByteCode.csModule=csStrModule;
	if(pParent)
		cContext.pParent=&pParent->cContext;

	Load(csStrModule);
	PrepareLexem();

	cByteCode.bOptimize1=HasDef(CString("_ОптимизацияМетодов"));
	cByteCode.bOptimize2=HasDef(CString("_РазрешитьДублирование"));
	cByteCode.bOptimize3=HasDef(CString("_Оптимизация3"));

/*	if(csStrModule.GetLength()>20000) Баг - диалог не работает
	{
		cByteCode.ConstList.SetSize(cByteCode.ConstList.GetSize(),300);
		cByteCode.CodeList.SetSize(cByteCode.CodeList.GetSize(),1000);
		cByteCode.FuncList.InitHashTable(100);
		cContext.cVariables.InitHashTable(200);
		cContext.cFunctions.InitHashTable(100);
		HashConstList.InitHashTable(300);
	}
*/
	if(csStrModule.GetLength()>20000)
	{
		cByteCode.ConstList.SetSize(cByteCode.ConstList.GetSize(),300);
		cByteCode.CodeList.SetSize(cByteCode.CodeList.GetSize(),1000);
	}


	//загрузка системных функций
	if(cSysFunctions.GetCount()==0)
		LoadSystemFunctions();


	//проверка и добавление нулевой переменной - контекста модуля
	AddVariable("",0);

	//Добавление глобальных констант
	aValueGlVariables.SetSize(aGlVariables.GetSize());
	for(int i=0;i<aGlVariables.GetSize();i++)
		AddVariable(aGlVariables[i],&aValueGlVariables[i]);
	

#ifdef TIME_DEBUG
	InitTime();
	if(HasDef("UseDebugTime"))
		bUseDebugTime=1;
#endif

	bool bRes=CompileModule();


#ifdef MYDEBUGOUT
	ToDebug();
#else
	if(HasDef(CString("_БайтКод")))
		ToDebug();
#endif


	return bRes;
}

//____________________________________________________________________
int CCompileModule::IsTypeVar(CString Str)
{
	if(Str.IsEmpty())
	{
		CLexem lex=PreviewGetLexem();
		Str=lex.sData;
	}
	Str=cTypeList[mUpper(Str)];
	return !Str.IsEmpty();
}
//____________________________________________________________________
CString CCompileModule::GetTypeVar(CString Str)
{
	if(Str.IsEmpty())
	{
		CLexem lex=GETLexem();
		Str=lex.sData;
	}
	Str=cTypeList[mUpper(Str)];
	if(Str.IsEmpty())
		SetError(ERROR_TYPE_DEF);

	return Str;
}


//____________________________________________________________________
//CompileDeclaration
//Назначение:
//Компиляция явного объявления переменных
//Возвращаемое значение:
//true,false
bool CCompileModule::CompileDeclaration()
{
	CString csType;
	CLexem lex=PreviewGetLexem();
	if(IDENTIFIER==lex.nType)//типизированное задание переменных
	{
		csType=GetTypeVar();
	}
	else
	{
		GETKeyWord(KEY_VAR);
	}

	while(true)
	{
		CString csName0=GETIdentifier(1);
		CString csName=mUpper(csName0);

		int nArrayCount=-1;
		if(IsNextDelimeter('['))//это объявление массива
		{
			nArrayCount=0;
			GETDelimeter('[');
			if(!IsNextDelimeter(']'))
			{
				CValue vConst=GETConstant();
				if(vConst.nType!=TYPE_NUMBER||vConst.fData<0)
				{
					SetError(ERROR_ARRAY_SIZE_CONST);
				}
				nArrayCount=(int)vConst.fData;

			}
			GETDelimeter(']');
		}

		BOOL bExport=0;
		BOOL bStatic=0;
		for(int n=1;n<=2;n++)
		{
			if(IsNextKeyWord(KEY_EXPORT))
			{
				if(bExport)//было объявление Экспорт
					break;
				GETKeyWord(KEY_EXPORT);
				bExport=1;
			}
			if(IsNextKeyWord(KEY_STATIC))
			{
				if(bStatic)//было объявление Статично
					break;
				GETKeyWord(KEY_STATIC);
				bStatic=1;
			}
			else
				break;
		}

		cByteCode.aExportList.Add(csName0);//для intelli-sense

		//не было еще объявления переменной - добавляем
		SParam Variable;
		if(pContext->bStaticVariable||bStatic)//статические переменные
		{
			CString csKey;
			csKey.Format("%s:%s:%d",csModuleName,csName,nCurrentCompile);
			CValue *pValue=(CValue*)StaticValList[csKey];
			if(!pValue)
			{
				pValue=new CValue();//удаляются только вместе с программой...
				StaticValList[csKey]=pValue;
			}
			cByteCode.aExternValue.Add(pValue);
			
			//добавляем в основной контекст модуля
			Variable=cContext.AddVariable(csName,csType,0);
		}
		else
		{
			Variable=pContext->AddVariable(csName,csType,bExport);
		}


		if(nArrayCount>=0)//записываем информацию о массивах
		{
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_SET_ARRAY_SIZE;
			code.Param1=Variable;
			code.Param2.nArray=nArrayCount;//число элементов в массиве
			cByteCode.CodeList.Add(code);
		}

		AddTypeSet(Variable);

		if(IsNextDelimeter('='))//начальная инициализация - работает только внутри текста модулей (но не пере объявл. процедур и функций)
		{
			if(nArrayCount>=0)
				GETDelimeter(',');//Error!

			GETDelimeter('=');


			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_LET;
			code.Param1=Variable;
			code.Param2=GetExpression();
			cByteCode.CodeList.Add(code);

		}

		if(!IsNextDelimeter(','))
			break;
		GETDelimeter(',');
	}
	return true;
}


//____________________________________________________________________
//CompileModule
//Назначение:
//Компиляция всего байт-кода (создание из набора лексем объектного кода)
//Возвращаемое значение:
//true,false
bool CCompileModule::CompileModule()
{
	//устанавливаем курсор на начало массива лексем
	nCurrentCompile=-1;

	pContext=GetContext();//контекст самого модуля

	CLexem lex;

/*
	BOOL bWasImport=0;
	while((lex=PreviewGetLexem()).nType!=ERRORTYPE)
		if(lex.nType==KEYWORD&&lex.nData==KEY_IMPORT)
		{
			if(bWasImport)
				SetError(ERROR_DUBLICATE_IMPORT);

			bWasImport=1;
			SetImport();//устанавливаем родительский модуль
		}
		else
			break;
*/


	while((lex=PreviewGetLexem()).nType!=ERRORTYPE)
		if((KEYWORD==lex.nType&&KEY_VAR==lex.nData)||(IDENTIFIER==lex.nType&&IsTypeVar(lex.sData)))
		{
				pContext=GetContext();
				CompileDeclaration();//загружаем объявление переменных
		}
		else
		if(KEYWORD==lex.nType&&(KEY_PROCEDURE==lex.nData||KEY_FUNCTION==lex.nData))
		{
				CompileFunction();//загружаем объявление функций
				//не забываем восстанавливать текущий контекст модуля (если это нужно)...
		}
		else
			break;

	//загружаем исполняемое тело модуля
	pContext=GetContext();//контекст самого модуля
	cByteCode.nStartModule=0;//cByteCode.CodeList.GetSize();
	CompileBlock();
	pContext->DoLabels();


	//ставим признак конца программы
	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_END;
	cByteCode.CodeList.Add(code);
	cByteCode.nVarCount=pContext->cVariables.GetCount();

	pContext=GetContext();
	//дообрабатываем процедуры и функции, вызовы которых были до их объявления
	//для это в конце массива байт-кодов добавляем новый код по вызову таких функций,
	//а для корректной работы в места раннего вызова вставляем операторы GOTO
	for(int i=0;i<apCallFunctions.GetSize();i++)
	{
		cByteCode.CodeList[apCallFunctions[i]->nAddLine].Param1.nIndex=cByteCode.CodeList.GetSize();//переход на вызов функции

		AddCallFunction(apCallFunctions[i]);

		//корректрируем переходы
		CByte code;
		AddLineInfo(code);
		code.nOper=OPER_GOTO;
		code.Param1.nIndex=apCallFunctions[i]->nAddLine+1;//поссле вызова функции возвращаемся назад
		cByteCode.CodeList.Add(code);
	}


	pContext=GetContext();


	//Получаем список переменных
	CVariable cVar;
	POSITION pos;
	CStringExt csKey;
	for( pos = pContext->cVariables.GetStartPosition(); pos != NULL; )
	{
		pContext->cVariables.GetNextAssoc( pos, csKey, cVar );
		cByteCode.VarList[CString(csKey)]=(void*)(1+cVar.nNumber);
		if(cVar.bExport)
		{
			cByteCode.ExportVarList[CString(csKey)]=(void*)(1+cVar.nNumber);
		}
	}


	if(nCurrentCompile+1<LexemList.GetSize())
		SetError(ERROR_END_PROGRAM);


	cByteCode.SetModule(this);
	//компиляция завершена успешно
	cByteCode.bCompile=1;
	return true;
}

//поиск определения функции в текущем модуле и во всех родительских
CFunction *CCompileModule::GetFunction(CString csName,int *pNumber)
{
	int nCanUseLocalInParent=cContext.nFindLocalInParent-1;
	int nNumber=0;
	CFunction *pDefFunction;

//	//ищем в системных функциях
//	pDefFunction=(CFunction *)cSysFunctions[csName];
//	if(!pDefFunction)//ищем в текущем модуле
//		pDefFunction=(CFunction *)GetContext()->cFunctions[csName];

	//ищем в текущем модуле
	pDefFunction=(CFunction *)GetContext()->cFunctions[csName];
	if(!pDefFunction)//ищем в системных функциях
		pDefFunction=(CFunction *)cSysFunctions[csName];
	

	if(!pDefFunction)
	{
		CCompileModule *pCurModule=pParent;
		CCompileModule *pNotParent=pStopParent;
		while(pCurModule)
		{
			nNumber++;
			if(pCurModule==pNotParent)//текущий модуль != запрещенный прародитель
			{
				//проверяем следующий родитель
				pNotParent=pCurModule->pParent;
				if(pNotParent==pContinueParent)//начиная со следующего - нет запрещенных родителей
					pNotParent=0;
			}
			else
			{
				pDefFunction=(CFunction *)pCurModule->pContext->cFunctions[csName];
				if(pDefFunction)//нашли
				{
					//смотрим это экспортная функция или нет
					if(nCanUseLocalInParent>0||pDefFunction->bExport)
						break;//ок
					pDefFunction=0;
				}
			}
			nCanUseLocalInParent--;
			pCurModule=pCurModule->pParent;
		}
	}

	if(pNumber)
	{
		*pNumber=nNumber;
	}
	return pDefFunction;
}

//Добавление в массив байт-кода вызова функции
bool CCompileModule::AddCallFunction(CCallFunction* pRealCall)
{
		CString csName=pRealCall->csName;
		int nModuleNumber;

		//находим определение функции
		CFunction *pDefFunction=GetFunction(csName,&nModuleNumber);
		if(!pDefFunction)
		{
			nCurrentCompile=pRealCall->nError;
			SetError(ERROR_CALL_FUNCTION,csName);//нет такой функции в модуле
		}

		//проверяем соответствие количество переданных и объявленных параметров
		int nRealCount=pRealCall->aParamList.GetSize();
		int nDefCount=pDefFunction->aParamList.GetSize();
		if(nRealCount>nDefCount)
		{
			nCurrentCompile=pRealCall->nError;
			SetError(ERROR_MANY_PARAMS);//Слишком много фактических параметров
		}

		CByte code;
		AddLineInfo(code);
		code.nStringNumber=pRealCall->nStringNumber;
		code.nNumberLine=pRealCall->nNumberLine;
		code.csModuleName=pRealCall->csModuleName;

		if(pDefFunction->bVirtaul)//виртуальная функция - вызов заменям на конструкцию Контекст.ИмяФункции(...)
		{
			code.nOper=OPER_CALL_M;
			code.Param1=pRealCall->sRetValue;//переменная, в которую возвращается значение
			code.Param2=pRealCall->sContextVal;//переменная у которой вызывается метод
			code.Param3.nIndex=GetConstString(pRealCall->csName);//номер вызываемого метода из списка встретившихся методов
			code.Param3.nArray=nDefCount;//число параметров
		}
		else
		{
			code.nOper=OPER_CALL;
			code.Param1=pRealCall->sRetValue;//переменная, в которую возвращается значение
			code.Param2.nArray=nModuleNumber;//номер модуля
			code.Param2.nIndex=pDefFunction->nStart;//стартовая позиция
			code.Param3.nArray=nDefCount;//число параметров
			code.Param3.nIndex=pDefFunction->nVarCount;//число локальных переменных
			if(pDefFunction->bSysFunction==1)
				code.nOper=OPER_SYSCALL;
		}
		int nAddrLine=cByteCode.CodeList.Add(code);

		for(int i=0;i<nDefCount;i++)
		{
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_SET;//идет передача параметров
			bool bDefaultValue=0;
			if(i<nRealCount)
			{
				code.Param1=pRealCall->aParamList[i];
				if(code.Param1.nArray==DEF_VAR_SKIP)//нужно подставить значение по умолчанию
					bDefaultValue=1;
				else
					code.Param2.nIndex=pDefFunction->aParamList[i].bByRef;//тип передачи значений

			}
			else
				bDefaultValue=1;

			if(bDefaultValue)
			{
				if(pDefFunction->aParamList[i].vData.nArray==DEF_VAR_SKIP)
				{
					nCurrentCompile=pRealCall->nError;
					SetError(ERROR_FEW_PARAMS);//Недостаточно фактических параметров
				}

				if(pDefFunction->bSysFunction==0)
					code.nOper=OPER_SETCONST;//значения по умолчанию
				code.Param1=pDefFunction->aParamList[i].vData;
			}

			cByteCode.CodeList.Add(code);
		}
	return true;
}

//____________________________________________________________________
//CompileFunction
//Назначение:
//Создание объектного кода для одной функции (процедуры)
//Алгоритм:
//-Определить число формальных параметров
//-Определить способы вызова формальных параметров (по ссылке или по значению)
//-Определить значения по умолчанию
//-Определить число локальных переменных
//-Определить возвращает ли функция значение
//
//Возвращаемое значение:
//true,false
bool CCompileModule::CompileFunction()
{
	//сейчас мы на уровне лексемы, где задано ключевое слово FUNCTION или PROCEDURE
	CLexem lex;
	if(IsNextKeyWord(KEY_FUNCTION))
	{
		GETKeyWord(KEY_FUNCTION);
		pContext=new CCompileContext(GetContext());//создаем новый контекст, в котором будем компилировать тело функции
		pContext->SetModule(this);
		pContext->nReturn=RETURN_FUNCTION;
	}
	else
	if(IsNextKeyWord(KEY_PROCEDURE))
	{
		GETKeyWord(KEY_PROCEDURE);
		pContext=new CCompileContext(GetContext());//создаем новый контекст, в котором будем компилировать тело процедуры
		pContext->SetModule(this);
		pContext->nReturn=RETURN_PROCEDURE;
	}
	else
		SetError(ERROR_FUNC_DEFINE);


	//вытаскиваем текст объявления функции
	lex=PreviewGetLexem();
	CString csShortDescription;
	int nNumberLine=lex.nNumberLine;
	int nRes=Buffer.Find('\n',lex.nStringNumber);
	if(nRes>=0)
	{
		csShortDescription=Buffer.Mid(lex.nStringNumber,nRes-lex.nStringNumber-1);
		nRes=csShortDescription.ReverseFind('/');
		if(nRes>0)
		{
			if(csShortDescription.GetAt(nRes-1)=='/')//итак - это комментарий
			{
				csShortDescription=csShortDescription.Mid(nRes+1);
			}
		}
		else
		{
			nRes=csShortDescription.ReverseFind(')');
			csShortDescription=csShortDescription.Left(nRes+1);
		}
	}

	//получаем имя функции
	CString csFuncName0=GETIdentifier(1);
	CString csFuncName=mUpper(csFuncName0);
	int nError=nCurrentCompile;

//	//ищем в системных функциях
//	if(cSysFunctions[csFuncName])
//		SetError(ERROR_IDENTIFIER_DUPLICATE,csFuncName0);


	CFunction *pFunction=new CFunction(csFuncName,pContext);
	pFunction->csRealName=csFuncName0;
	pFunction->csShortDescription=csShortDescription;
	pFunction->nNumberLine=nNumberLine;

	//компилируем список формальных параметров + регистрируем их как локальные
	GETDelimeter('(');
	if(!IsNextDelimeter(')'))
	while(true)
	{
		CString csType;
		//проверка на типизированность
		if(IsTypeVar())
		{
			csType=GetTypeVar();
		}

		CParamVariable Variable;
		if(IsNextKeyWord(KEY_VAL))
		{
			GETKeyWord(KEY_VAL);
			Variable.bByRef=0;
		}

		Variable.csName=GETIdentifier();
		//регистрируем эту переменную как локальную
		if(pContext->FindVariable(Variable.csName))//было объявление + повторное объявление = ошибка
			SetError(ERROR_IDENTIFIER_DUPLICATE,Variable.csName);

		if(IsNextDelimeter('['))//это массив
		{
			GETDelimeter('[');
			GETDelimeter(']');
		}
		else
		if(IsNextDelimeter('='))
		{
			GETDelimeter('=');
			Variable.vData=FindConst(GETConstant());
		}

		pContext->AddVariable(Variable.csName,csType,0);
		Variable.csType=csType;

		pFunction->aParamList.Add(Variable);

		if(IsNextDelimeter(')'))
			break;

		GETDelimeter(',');
	}
	GETDelimeter(')');

	for(int n=1;n<=3;n++)
	{
		if(IsNextKeyWord(KEY_EXPORT))
		{
			if(pFunction->bExport)//было объявление Экспорт
				break;
			GETKeyWord(KEY_EXPORT);
			pFunction->bExport=1;
		}
		if(IsNextKeyWord(KEY_VIRTUAL))
		{
			if(pFunction->bVirtaul)//было объявление Виртуально
				break;
			GETKeyWord(KEY_VIRTUAL);
			pFunction->bVirtaul=1;
		}
		else
		if(IsNextKeyWord(KEY_FORWARD))
		{
			GETKeyWord(KEY_FORWARD);

			//предварительные объявления пропускаем (т.к. они лишние)
			delete pFunction;
			return true;
		}
		else
			break;
	}

	if(!cByteCode.bOptimize2)
	if(GetContext()->cFunctions[csFuncName])
	{
		nCurrentCompile=nError;
		SetError(ERROR_DEF_FUNCTION,csFuncName);
	}

	//проверка на типизированность
/*	if(RETURN_FUNCTION==pContext->nReturn)
	if(IsTypeVar())
	{
		pFunction->csType=GetTypeVar();
	}
*/
	GetContext()->cFunctions[csFuncName]=pFunction;

	//вставляем информацию о функции в массив байт-кодов:
	CByte code0;
	AddLineInfo(code0);
	code0.nOper=OPER_FUNC;
	code0.Param1.nArray=(int)pContext;
	int nAddres=pFunction->nStart=cByteCode.CodeList.Add(code0);
	cByteCode.FuncList[csFuncName]=(void*)(nAddres+1);
	if(pFunction->bExport)
	{
		cByteCode.ExportFuncList[csFuncName]=(void*)(nAddres+1);
		cByteCode.aExportList.Add(csFuncName0);//для intelli-sense
	}
	
	for(int i=0;i<pFunction->aParamList.GetSize();i++)
	{
		SParam Var;
		Var.csType=pFunction->aParamList[i].csType;
		Var.nArray=0;
		Var.nIndex=i;//индекс совпадает с номером
		AddTypeSet(Var);
	}


	GetContext()->csCurFuncName=csFuncName;
	CompileBlock();
	pContext->DoLabels();
	GetContext()->csCurFuncName="";

	if(pContext->nReturn==RETURN_FUNCTION)
		GETKeyWord(KEY_ENDFUNCTION);
	else
		GETKeyWord(KEY_ENDPROCEDURE);

	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_ENDFUNC;
	pFunction->nFinish=cByteCode.CodeList.Add(code);
	pFunction->nVarCount=pContext->cVariables.GetCount();

	cByteCode.CodeList[nAddres].Param3.nIndex=pFunction->nVarCount;//число локальных переменных
	cByteCode.CodeList[nAddres].Param3.nArray=pFunction->aParamList.GetSize();//число формальных параметров
	return true;
}

//____________________________________________________________________
//записываем информацию о типе переменной
void CCompileModule::AddTypeSet(SParam Var)
{
	if(!Var.csType.IsEmpty())
	{
		CByte code;
		AddLineInfo(code);
		code.nOper=OPER_SET_TYPE;
		code.Param1=Var;

		code.Param2.nArray=0;
		if("NUMBER"==Var.csType)
			code.Param2.nArray=1;
		else
		if("STRING"==Var.csType)
			code.Param2.nArray=2;
		else
		if("DATE"==Var.csType)
			code.Param2.nArray=3;
		cByteCode.CodeList.Add(code);
	}
}


//Макрос проверки переменной Var типу Str
#define CheckTypeDef(Var,Str)\
	if(!CString(Str).IsEmpty())\
	{\
		if(Var.csType!=Str)\
		{\
			if("NUMBER"==Str)\
				SetError(ERROR_BAD_TYPE_EXPRESSION_N);\
			else\
			if("STRING"==Str)\
				SetError(ERROR_BAD_TYPE_EXPRESSION_S);\
			else\
			if("DATE"==Str)\
				SetError(ERROR_BAD_TYPE_EXPRESSION_D);\
			else\
				SetError(ERROR_BAD_TYPE_EXPRESSION);\
		}\
		if("NUMBER"==Str)\
			code.nOper+=TYPE_DELTA1;\
		else\
		if("STRING"==Str)\
			code.nOper+=TYPE_DELTA2;\
		else\
		if("DATE"==Str)\
			code.nOper+=TYPE_DELTA3;\
	}


//Макрос корректировки операции по типу переменной
//если она типизированна, то будет выполняться типизированная операция
#define CorrectTypeDef(sKey)\
if(!sKey.csType.IsEmpty())\
{\
	if(sKey.csType=="NUMBER")\
		code.nOper+=TYPE_DELTA1;\
	else\
	if(sKey.csType=="STRING")\
		code.nOper+=TYPE_DELTA2;\
	else\
	if(sKey.csType=="DATE")\
		code.nOper+=TYPE_DELTA3;\
	else\
		SetError(ERROR_BAD_TYPE_EXPRESSION);\
}


//____________________________________________________________________
//CompileBlock
//Назначение:
//Создание объектного кода для одного блока (куска кода между какими-либо 
//операторными скобками типа ЦИКЛ...КОНЕЦИКЛА, ЕСЛИ...КОНЕЦЕСЛИ и т.п.
//nIterNumber - номер вложенного блока
//Возвращаемое значение:
//true,false
bool CCompileModule::CompileBlock()
{
	CLexem lex;
	while((lex=PreviewGetLexem()).nType!=ERRORTYPE)
	{
		if(IDENTIFIER==lex.nType&&IsTypeVar(lex.sData))
		{
			CompileDeclaration();
		}
		else
		if(KEYWORD==lex.nType)
		switch(lex.nData)
		{
			case KEY_VAR://задание переменных и массивов
				CompileDeclaration();
				break;
			case KEY_IF:
				CompileIf();
				break;
			case KEY_WHILE:
				CompileWhile();
				break;
			case KEY_FOR:
				CompileFor();
				break;
			case KEY_GOTO:
				CompileGoto();
				break;
			case KEY_RETURN:
			{
				GETKeyWord(KEY_RETURN);

				if(pContext->nReturn==RETURN_NONE)
					SetError(ERROR_USE_RETURN);//Оператор Return (Возврат) не может употребляться вне процедуры или функции
					
				CByte code;
				AddLineInfo(code);
				code.nOper=OPER_RET;

				if(pContext->nReturn==RETURN_FUNCTION)//возвращается какое-то значение
				{
					if(IsNextDelimeter(';'))
						SetError(ERROR_EXPRESSION_REQUIRE);
					code.Param1=GetExpression();
				}
				else
				{
					code.Param1.nArray=DEF_VAR_NORET;
					code.Param1.nIndex=DEF_VAR_NORET;
				}

				cByteCode.CodeList.Add(code);
				break;
			}
			case KEY_TRY:
			{
				GETKeyWord(KEY_TRY);
				CByte code;
				AddLineInfo(code);
				code.nOper=OPER_TRY;
				int nLineTry=cByteCode.CodeList.Add(code);

				CompileBlock();
				code.nOper=OPER_GOTO;//OPER_ENDTRY
				int nAddrLine=cByteCode.CodeList.Add(code);

				cByteCode.CodeList[nLineTry].Param1.nIndex=cByteCode.CodeList.GetSize();

				GETKeyWord(KEY_EXCEPT);
				CompileBlock();
				GETKeyWord(KEY_ENDTRY);

				cByteCode.CodeList[nAddrLine].Param1.nIndex=cByteCode.CodeList.GetSize();
				break;
			}
			case KEY_RAISE:
			{
				GETKeyWord(KEY_RAISE);
				CByte code;
				AddLineInfo(code);
				code.nOper=OPER_RAISE;
				cByteCode.CodeList.Add(code);
				break;
			}
			case KEY_CONTINUE:
			{
				GETKeyWord(KEY_CONTINUE);
				if(pContext->aContinueList[pContext->nDoNumber])
				{
					CByte code;
					AddLineInfo(code);
					code.nOper=OPER_GOTO;
					int nAddrLine=cByteCode.CodeList.Add(code);
					CDefIntList *pList=(CDefIntList *)pContext->aContinueList[pContext->nDoNumber];
					pList->Add(nAddrLine);
				}
				else
					SetError(ERROR_USE_CONTINUE);//Опереатор Continue (Продолжить)  может употребляться только внутри цикла
				break;
			}
			case KEY_BREAK:
			{
				GETKeyWord(KEY_BREAK);
				if(pContext->aBreakList[pContext->nDoNumber])
				{
					CByte code;
					AddLineInfo(code);
					code.nOper=OPER_GOTO;
					int nAddrLine=cByteCode.CodeList.Add(code);
					CDefIntList *pList=(CDefIntList *)pContext->aBreakList[pContext->nDoNumber];
					pList->Add(nAddrLine);
				}
				else
					SetError(ERROR_USE_BREAK);//Опереатор Break(Прервать)  может употребляться только внутри цикла
				break;
			}
			case KEY_PRINT:
			{
				GETKeyWord(KEY_PRINT);
				CByte code;
				AddLineInfo(code);
				code.nOper=OPER_MSG;
				//code.Param1=GetExpression();
				cByteCode.CodeList.Add(code);
				break;
			}
			case KEY_FUNCTION:
			case KEY_PROCEDURE:
			{
//				CompileFunction();//загружаем объявление функций
				GetLexem();
				SetError(ERROR_USE_BLOCK);
				break;
			}
/*			
			case KEY_IMPORT:
			{
				nCurrentCompile++;
				SetError(ERROR_USE_IMPORT);
				break;
			}*/
			default://значит встретилась завершающая данный блок операторная скобка (например КОНЕЦЕСЛИ, КОНЕЦЦИКЛА, КОНЕЦФУНКЦИИ и т.п.)
				return true;
		}
		else
		{
			lex=GetLexem();
			if(IDENTIFIER==lex.nType)
			{
				pContext->nTempVar=0;
				if(IsNextDelimeter(':'))//это встретилось задание метки
				{
					void *p=pContext->cLabelsDef[lex.sData];
					if(p)
						SetError(ERROR_IDENTIFIER_DUPLICATE,lex.sData);//произошло дублированное определения меток
					//записываем адрес перехода:
					pContext->cLabelsDef[lex.sData]=(void*)cByteCode.CodeList.GetSize();
					GETDelimeter(':');
				}
				else//здесь обрабатываются вызовы функций, методов, присваиваение выражений
				{
					nCurrentCompile--;//шаг назад
					int nSet=1;
					SParam Variable=GetCurrentIdentifier(nSet);//получаем левую часть выражения (до знака '=')
					if(nSet)//если есть правая часть, т.е. знак '='
					{
						GETDelimeter('=');//это присваивание переменной какого-то выражения
						SParam sExpression=GetExpression();
						CByte code;
						code.nOper=OPER_LET;
						AddLineInfo(code);

						CheckTypeDef(sExpression,Variable.csType);
						Variable.csType=sExpression.csType;

						BOOL bShortLet=0;
						int n;
						if(DEF_VAR_TEMP==sExpression.nArray)//сокращаем только временные переменные
						{
							n=cByteCode.CodeList.GetSize()-1;
							if(n>=0)
							{
								int nOperation=cByteCode.CodeList[n].nOper;
								nOperation=nOperation%TYPE_DELTA1;
								if(	OPER_MULT	==	nOperation||
									OPER_DIV	==	nOperation||
									OPER_ADD	==	nOperation||
									OPER_SUB	==	nOperation||
									OPER_MOD	==	nOperation||
									OPER_GT		==	nOperation||
									OPER_GE		==	nOperation||
									OPER_LS		==	nOperation||
									OPER_LE		==	nOperation||
									OPER_NE		==	nOperation||
									OPER_EQ		==	nOperation
									)
								{
									bShortLet=1;//сокращаем одно присваивание
								}
							}
						}
						
						if(bShortLet)
						{
							cByteCode.CodeList[n].Param1=Variable;
						}
						else
						{
							code.Param1=Variable;
							code.Param2=sExpression;
							cByteCode.CodeList.Add(code);
						}
					}
				}
			}
			else
			if(DELIMETER==lex.nType&&';'==lex.nData)
			{
			}
			else
			if(ENDPROGRAM==lex.nType)
				break;
			else
			{
				SetError(ERROR_CODE);
			}
		}
	}//while
	return true;
}//CompileBlock


//____________________________________________________________________
//CompileGoto
//Назначение:
//Компилирование оператора GOTO (определение расположение метки перехода 
//для последующей ее замены на адрес и тип = LABEL)
//Возвращаемое значение:
//true,false
bool CCompileModule::CompileGoto()
{
	GETKeyWord(KEY_GOTO);
	SLabel data;
	data.csName=GETIdentifier();
	data.nLine=cByteCode.CodeList.GetSize();//запоминаем те переходы, которые потом надо будет обработать
	data.nError=nCurrentCompile;
	pContext->cLabels.Add(data);

	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_GOTO;
	cByteCode.CodeList.Add(code);
	return true;
}//CompileGoto


//____________________________________________________________________
//GetCurrentIdentifier
//Назначение:
//Компилирование идентификатора (определение его типа как переменной,атрибута или функции,метода)
//nIsSet - на входе:  1 - признак того что возможно ожидается присваивание выражения (если встретится знак '=')
//Возвращаемое значение:
//nIsSet - на выходе: 1 - признак того что точно ожидается присваивание выражения (т.е. должен встретиться знак '=')
//номер идекса переменной, где лежит значение идентификатора
SParam CCompileModule::GetCurrentIdentifier(int &nIsSet)
{
	int nPrevSet=nIsSet;

	SParam Variable;
	CString csName=GETIdentifier();
	if(IsNextDelimeter('('))//это вызов функции
	{
		Variable=GetCallFunction(csName);
		if(IsTypeVar(csName))//это приведение типов
		{
			CString csType=GetTypeVar(csName);
			Variable.csType=csType;
		}
		nIsSet=0;
	}
	else//это вызов переменной
	{
		bool bCheckError=!nPrevSet;
		if(IsNextDelimeter('.'))//эта переменная содержит вызов метода
			bCheckError=1;

		Variable=GetVariable(csName,bCheckError);
		nIsSet=1;
	}


MLabel:
	if(IsNextDelimeter('['))//это массив
	{
		GETDelimeter('[');
		SParam sKey=GetExpression();
		GETDelimeter(']');
		//определяем тип вызова (т.е. это установка значения массива или получение)
		//Пример:
		//Мас[10]=12; - Set
		//А=Мас[10]; - Get
		//Мас[10][2]=12; - Get,Set
		nIsSet=0;

		if(IsNextDelimeter('['))//проверки типа переменной массива (поддержка многомерных массивов)
		{
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_CHECK_ARRAY;
			code.Param1=Variable;//переменная - массив
			code.Param2=sKey;//индекс массива
			cByteCode.CodeList.Add(code);
		}

		if(IsNextDelimeter('=')&&nPrevSet==1)
		{
			GETDelimeter('=');
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_SET_ARRAY;
			code.Param1=Variable;//переменная - массив
			code.Param2=sKey;//индекс массива (точнее ключ т.к. используется ассоциативный массив)
			code.Param3=GetExpression();

			CorrectTypeDef(sKey);//проверка типа значения индексной переменной

			cByteCode.CodeList.Add(code);
			return Variable;
		}
		else
		{
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_GET_ARRAY;

			code.Param2=Variable;//переменная - массив
			code.Param3=sKey;//индекс массива (точнее ключ т.к. используется ассоциативный массив)
			Variable=GetVariable();
			code.Param1=Variable;//переменная, в которую возвращается значение

			CorrectTypeDef(sKey);//проверка типа значения индексной переменной

			cByteCode.CodeList.Add(code);
		}


		goto MLabel;
	}
	if(IsNextDelimeter('.'))//это вызов метода или атрибута агрегатного объекта
	{
		GETDelimeter('.');
		CString csMethod=GETIdentifier(1);
		int nNumber=GetConstString(csMethod);

		if(IsNextDelimeter('('))//это вызов метода
		{
			CArray <SParam,SParam> aParamList;
			GETDelimeter('(');
			while(!IsNextDelimeter(')'))
			{
				if(IsNextDelimeter(','))
				{
					SParam data;
					data.nArray=DEF_VAR_SKIP;//пропущенный параметр
					data.nIndex=DEF_VAR_SKIP;
					aParamList.Add(data);
				}
				else
				{
					aParamList.Add(GetExpression());
					if(IsNextDelimeter(')'))
						break;
				}
				GETDelimeter(',');
			}
			GETDelimeter(')');


			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_CALL_M;
			code.Param2=Variable;//переменная у которой вызывается метод
			code.Param3.nIndex=nNumber;//номер вызываемого метода из списка встретившихся методов
			code.Param3.nArray=aParamList.GetSize();//число параметров
			Variable=GetVariable();
			code.Param1=Variable;//переменная, в которую возвращается значение
			cByteCode.CodeList.Add(code);

			for(int i=0;i<aParamList.GetSize();i++)
			{
				CByte code;
				AddLineInfo(code);
				code.nOper=OPER_SET;
				code.Param1=aParamList[i];
				cByteCode.CodeList.Add(code);
			}

			nIsSet=0;
		}
		else//иначе - вызов атрибута
		{
			//определяем тип вызова (т.е. это установка атрибута или получение)
			//Пример:
			//А=Спр.Товар; - Get
			//Спр.Товар=0; - Set
			//Спр.Товар.Код=0;  - Get,Set
			nIsSet=0;
			CByte code;
			AddLineInfo(code);
			if(IsNextDelimeter('=')&&nPrevSet==1)
			{
				GETDelimeter('=');
				code.nOper=OPER_SET_A;
				code.Param1=Variable;//переменная у которой вызывается атрибут
				code.Param2.nIndex=nNumber;//номер вызываемого метода из списка встретившихся атрибутов и методов
				code.Param3=GetExpression();
				cByteCode.CodeList.Add(code);
				return Variable;
			}
			else
			{
				code.nOper=OPER_GET_A;
				code.Param2=Variable;//переменная у которой вызывается атрибут
				code.Param3.nIndex=nNumber;//номер вызываемого атрибута из списка встретившихся атрибутов и методов
				Variable=GetVariable();
				code.Param1=Variable;//переменная, в которую возвращается значение
				cByteCode.CodeList.Add(code);
			}

		}
		goto MLabel;
	}

	return Variable;
}//GetCurrentIdentifier

bool CCompileModule::CompileIf()
{
	CArray <int,int>aAddrLine;


	GETKeyWord(KEY_IF);

	SParam sParam;
	int nLastIFLine=0;
	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_IF;

	sParam=GetExpression();
	code.Param1=sParam;
	CorrectTypeDef(sParam);//проверка типа значения

	nLastIFLine=cByteCode.CodeList.Add(code);

	GETKeyWord(KEY_THEN);
	CompileBlock();

	while(IsNextKeyWord(KEY_ELSEIF))
	{
		//Записываем выход из всех проверок для предыдущего блока
		code.nOper=OPER_GOTO;
		aAddrLine.Add(cByteCode.CodeList.Add(code));//параметр для оператора GOTO будет известен потом

		//для предыдущего условия устанавливаем адрес прехода при несовпадении условия
		cByteCode.CodeList[nLastIFLine].Param2.nIndex=cByteCode.CodeList.GetSize();

		GETKeyWord(KEY_ELSEIF);
		AddLineInfo(code);
		code.nOper=OPER_IF;

		sParam=GetExpression();
		code.Param1=sParam;
		CorrectTypeDef(sParam);//проверка типа значения

		nLastIFLine=cByteCode.CodeList.Add(code);

		GETKeyWord(KEY_THEN);
		CompileBlock();
	}

	if(IsNextKeyWord(KEY_ELSE))
	{
		//Записываем выход из всех проверок для предыдущего блока
		AddLineInfo(code);
		code.nOper=OPER_GOTO;
		aAddrLine.Add(cByteCode.CodeList.Add(code));//параметр для оператора GOTO будет известен потом

		//для предыдущего условия устанавливаем адрес прехода при несовпадении условия
		cByteCode.CodeList[nLastIFLine].Param2.nIndex=cByteCode.CodeList.GetSize();
		nLastIFLine=0;

		GETKeyWord(KEY_ELSE);
		CompileBlock();
	}

	GETKeyWord(KEY_ENDIF);


	int nCurCompile=cByteCode.CodeList.GetSize();
	//для последнего условия устанавливаем адрес прехода при несовпадении условия
	if(nLastIFLine>0)
		cByteCode.CodeList[nLastIFLine].Param2.nIndex=nCurCompile;

	//Устанавливаем параметр для оператора GOTO - выход из всех локальных условий
	for(int i=0;i<aAddrLine.GetSize();i++)
	{
		cByteCode.CodeList[aAddrLine[i]].Param1.nIndex=nCurCompile;

	}
	return true;
}

bool CCompileModule::CompileWhile()
{
	pContext->StartDoList();


	SParam sParam;
	int nStartWhile=cByteCode.CodeList.GetSize();
	GETKeyWord(KEY_WHILE);
	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_IF;

	sParam=GetExpression();
	code.Param1=sParam;
	CorrectTypeDef(sParam);//проверка типа значения

	int nAddrLine=cByteCode.CodeList.Add(code);

	GETKeyWord(KEY_DO);
	CompileBlock();
	GETKeyWord(KEY_ENDDO);


	CByte code2;
	AddLineInfo(code2);
	code2.nOper=OPER_GOTO;
	code2.Param1.nIndex=nStartWhile;
	cByteCode.CodeList.Add(code2);

	cByteCode.CodeList[nAddrLine].Param2.nIndex=cByteCode.CodeList.GetSize();

	//запоминаем адреса переходов для команд Continue и Break
	pContext->FinishDoList(cByteCode,nStartWhile,cByteCode.CodeList.GetSize());
	return true;
}

//____________________________________________________________________
bool CCompileModule::CompileFor()
{
	pContext->StartDoList();


	int nStartFOR=0;

	GETKeyWord(KEY_FOR);
	CString csName=GETIdentifier();
	SParam Variable=GetVariable(csName);

	//проверка типа переменной
	if(!Variable.csType.IsEmpty())
	{
		if(Variable.csType!="NUMBER")
			SetError(ERROR_NUMBER_TYPE);
	}

	GETDelimeter('=');
	SParam Variable2=GetExpression();

	CByte code0;
	AddLineInfo(code0);
	code0.nOper=OPER_LET;
	code0.Param1=Variable;
	code0.Param2=Variable2;
	cByteCode.CodeList.Add(code0);

	//проверка типа значения
	if(!Variable.csType.IsEmpty())
	{
		if(Variable2.csType!="NUMBER")
			SetError(ERROR_BAD_TYPE_EXPRESSION);
	}

	GETKeyWord(KEY_TO);
	SParam VariableTo=GetVariable(csName+"@");//переменная цикла

	CByte code1;
	AddLineInfo(code1);
	code1.nOper=OPER_LET;
	code1.Param1=VariableTo;
	code1.Param2=GetExpression();
	cByteCode.CodeList.Add(code1);

	CByte code;
	AddLineInfo(code);
	code.nOper=OPER_FOR;
	code.Param1=Variable;
	code.Param2=VariableTo;
	nStartFOR=cByteCode.CodeList.Add(code);

	GETKeyWord(KEY_DO);
	CompileBlock();
	GETKeyWord(KEY_ENDDO);

	CByte code2;
	AddLineInfo(code2);
	code2.nOper=OPER_NEXT;
	code2.Param1=Variable;
	code2.Param2.nIndex=nStartFOR;
	cByteCode.CodeList.Add(code2);

	cByteCode.CodeList[nStartFOR].Param3.nIndex=cByteCode.CodeList.GetSize();

	//запоминаем адреса переходов для команд Continue и Break
	pContext->FinishDoList(cByteCode,cByteCode.CodeList.GetSize()-1,cByteCode.CodeList.GetSize());
	return true;
}

//____________________________________________________________________
//обработка вызова функции или процедуры
SParam CCompileModule::GetCallFunction(CString csName)
{
	CCallFunction *pRealCall=new CCallFunction();
	pRealCall->nError=nCurrentCompile;//для выдачи сообщений при ошибках
	pRealCall->csName=csName;

	GETDelimeter('(');
	while(!IsNextDelimeter(')'))
	{
		if(IsNextDelimeter(','))
		{
			SParam data;
			data.nArray=DEF_VAR_SKIP;//пропущенный параметр
			data.nIndex=DEF_VAR_SKIP;
			pRealCall->aParamList.Add(data);
		}
		else
		{
			pRealCall->aParamList.Add(GetExpression());
			if(IsNextDelimeter(')'))
				break;
		}
		GETDelimeter(',');
	}
	GETDelimeter(')');

	SParam Variable=GetVariable();

	CByte code;
	AddLineInfo(code);
	pRealCall->nStringNumber=code.nStringNumber;
	pRealCall->nNumberLine=code.nNumberLine;
	pRealCall->csModuleName=code.csModuleName;
	pRealCall->sRetValue=Variable;
	pRealCall->sContextVal=GetVariable("КОНТЕКСТ");


	CFunction *pDefFunction=0;
	if(bExpressionOnly)
	{
		pDefFunction=GetFunction(csName);
	}
	else
	{
//		pDefFunction=(CFunction *)cSysFunctions[csName];//ищем в системных функциях
//		if(!pDefFunction)
//			pDefFunction=(CFunction *)GetContext()->cFunctions[csName];//ищем в текущем модуле
		pDefFunction=(CFunction *)GetContext()->cFunctions[csName];//ищем в текущем модуле
		if(!pDefFunction)
			pDefFunction=(CFunction *)cSysFunctions[csName];//ищем в системных функциях

	}

	if(GetContext()->csCurFuncName!=csName&&pDefFunction)
	{
		AddCallFunction(pRealCall);
		delete pRealCall;
	}
	else
	{
		if(bExpressionOnly)
			SetError(ERROR_CALL_FUNCTION,csName);

		code.nOper=OPER_GOTO;//переход в конец байт-кода, где будет производиться развернутый вызов
		pRealCall->nAddLine=cByteCode.CodeList.Add(code);
		apCallFunctions.Add(pRealCall);
	}
	return Variable;
}


//____________________________________________________________________
//Функция возвращает номер переменной по строковому имени
SParam CCompileModule::GetVariable(CString csName,bool bCheckError)
{
	return pContext->GetVariable(csName,1,bCheckError);

}

//____________________________________________________________________
//Cоздаем новый идентификатор переменной
SParam CCompileModule::GetVariable()
{
	CString csName;
	csName.Format("@%d",pContext->nTempVar);//@ - для гарантии уникальности имени
	SParam Variable=pContext->GetVariable(csName,0);//временную переменную ищем только в локальном контексте
	Variable.nArray=DEF_VAR_TEMP;//признак временной локальной переменной
	pContext->nTempVar++;
	return Variable;
}

//____________________________________________________________________
//Получает номер константы из уникального списка значений
//(если такого значения в списке нет, то оно создается)
SParam CCompileModule::FindConst(CValue &vData)
{
	SParam Const;
	Const.nArray=DEF_VAR_CONST;

	CString Str;
	Str.Format("%d:%s",vData.nType,vData.GetString());
	if(HashConstList[Str])
	{
		Const.nIndex=((int)HashConstList[Str])-1;
	} 
	else
	{
		Const.nIndex=cByteCode.ConstList.GetSize();
		cByteCode.ConstList.Add(vData);
		HashConstList[Str]=(void*)(Const.nIndex+1);
	}
//	vData.bReadOnly=1;
	CString csType=vData.GetTypeString();
	Const.csType=GetTypeVar(csType);
	return Const;
}

/*
//Вызов по дефолту - т.е. начальная точка компиляции произвольного выражения
//____________________________________________________________________
SParam CCompileModule::GetExpression(void)
{
	pContext->nTempVar=0;
	return GetExpression();
}
*/
#define SetOper(x)	code.nOper=x;


//____________________________________________________________________
//Компиляция произвольного выражения (служебные вызовы из самой функции)
SParam CCompileModule::GetExpression(int nPriority)
{
	SParam Variable;
	CLexem lex=GETLexem();

	//Сначала обрабатываем Левые операторы
	if((lex.nType==KEYWORD&&lex.nData==KEY_NOT)||(lex.nType==DELIMETER&&lex.nData=='!'))
	{
		Variable=GetVariable();
		SParam Variable2=GetExpression(aPriority['!']);
		CByte code;
		code.nOper=OPER_NOT;
		AddLineInfo(code);

		if(!Variable2.csType.IsEmpty())
			CheckTypeDef(Variable2,"NUMBER");
		Variable.csType="NUMBER";

		code.Param1=Variable;
		code.Param2=Variable2;
		cByteCode.CodeList.Add(code);
	}
	else
	if(lex.nType==DELIMETER&&lex.nData=='(')
	{
		Variable=GetExpression();
		GETDelimeter(')');
	}
	else
	if(lex.nType==DELIMETER&&lex.nData=='?')
	{
		Variable=GetVariable();
		CByte code;
		AddLineInfo(code);
		code.nOper=OPER_ITER;
		code.Param1=Variable;
		GETDelimeter('(');
		code.Param2=GetExpression();
		GETDelimeter(',');
		code.Param3=GetExpression();
		GETDelimeter(',');
		code.Param4=GetExpression();
		GETDelimeter(')');
		cByteCode.CodeList.Add(code);
/*
		CByte code2;
		AddLineInfo(code2);
		code2.nOper=OPER_SET;
		code2.Param1=Variable;
		code2.Param2=sParam4;
		cByteCode.CodeList.Add(code2);
*/
	}
	else
	if(lex.nType==IDENTIFIER)
	{
		nCurrentCompile--;//шаг назад
		int nSet=0;
		Variable=GetCurrentIdentifier(nSet);
	}
	else
	if(lex.nType==CONSTANT)
	{
		Variable=FindConst(lex.vData);
	}
	else
	if((lex.nType==DELIMETER&&lex.nData=='+')||(lex.nType==DELIMETER&&lex.nData=='-'))
	{
		//проверяем допустимость такого задания
		int nCurPriority=aPriority[lex.nData];
		if(nPriority>=nCurPriority)//сравниваем приоритеты левой (предыдущей операции) и текущей выполняемой операции
			SetError(ERROR_EXPRESSION);

		//Это задание пользователем знака выражения
		if(lex.nData=='+')//ничего не делаем (игнорируем)
		{
			CByte code;
			Variable=GetExpression(nPriority);
			if(!Variable.csType.IsEmpty())
				CheckTypeDef(Variable,"NUMBER");
			Variable.csType="NUMBER";
			return Variable;
		}
		else
		{
			Variable=GetExpression(100);//сверх высокий приоритет!
			CByte code;
			AddLineInfo(code);
			code.nOper=OPER_INVERT;

			if(!Variable.csType.IsEmpty())
				CheckTypeDef(Variable,"NUMBER");

			code.Param2=Variable;
			Variable=GetVariable();
			Variable.csType="NUMBER";
			code.Param1=Variable;
			cByteCode.CodeList.Add(code);
		}
	}
	else
	{
		SetError(ERROR_EXPRESSION);
	}

	//Теперь обрабатываем Правые операторы
	
	//итак в Variable имеем первый индекс переменной выражения
MOperation:
	lex=PreviewGetLexem();
	if(lex.nType==DELIMETER&&lex.nData==')')
		return Variable;

	//смотрим есть ли далее операторы выполнения действий над данной переменной
	if((lex.nType==DELIMETER&&lex.nData!=';')||(lex.nType==KEYWORD&&lex.nData==KEY_AND)||(lex.nType==KEYWORD&&lex.nData==KEY_OR))
	if(lex.nData>=0&&lex.nData<=255)
	{
		int nCurPriority=aPriority[lex.nData];
		if(nPriority<nCurPriority)//сравниваем приоритеты левой (предыдущей операции) и текущей выполняемой операции
		{
			CByte code;
			AddLineInfo(code);
			lex=GetLexem();

			if(lex.nData=='*')
			{
				SetOper(OPER_MULT);
			}
			else
			if(lex.nData=='/')
			{
				SetOper(OPER_DIV);
			}
			else
			if(lex.nData=='+')
			{
				SetOper(OPER_ADD);
			}
			else
			if(lex.nData=='-')
			{
				SetOper(OPER_SUB);
			}
			else
			if(lex.nData=='%')
			{
				SetOper(OPER_MOD);
			}
			else
			if(lex.nData==KEY_AND)
			{
				code.nOper=OPER_AND;
			}
			else
			if(lex.nData==KEY_OR)
			{
				code.nOper=OPER_OR;
			}
			else
			if(lex.nData=='>')
			{
				SetOper(OPER_GT);
				if(IsNextDelimeter('='))
				{
					GETDelimeter('=');
					SetOper(OPER_GE);
				}
			}
			else
			if(lex.nData=='<')
			{
				SetOper(OPER_LS);
				if(IsNextDelimeter('='))
				{
					GETDelimeter('=');
					SetOper(OPER_LE);
				}
				else
				if(IsNextDelimeter('>'))
				{
					GETDelimeter('>');
					SetOper(OPER_NE);
				}

			}
			else
			if(lex.nData=='=')
			{
				SetOper(OPER_EQ);
			}
			else
				SetError(ERROR_EXPRESSION);

			SParam Variable1=GetVariable();
			SParam Variable2=Variable;
			SParam Variable3=GetExpression(nCurPriority);

			//доп. проверка на запрещенные операции
			if(DEF_VAR_TEMP!=Variable3.nArray&&DEF_VAR_CONST!=Variable3.nArray)
			if(Variable2.csType=="STRING")
			{
				if(	OPER_DIV==code.nOper||
					OPER_MOD==code.nOper||
					OPER_MULT==code.nOper||
					OPER_AND==code.nOper||
					OPER_OR==code.nOper
					)
					SetError(ERROR_TYPE_OPERATION);

			}
 
			if(DEF_VAR_CONST!=Variable2.nArray&&DEF_VAR_TEMP!=Variable2.nArray)//константы не проверяем  - т.к. они типизированы по дефолту
			{
				CheckTypeDef(Variable3,Variable2.csType);
			}

			Variable1.csType=Variable2.csType;
			if(code.nOper>=OPER_GT && code.nOper<=OPER_NE)
				Variable1.csType="NUMBER";


			code.Param1=Variable1;
			code.Param2=Variable2;
			code.Param3=Variable3;
			cByteCode.CodeList.Add(code);


			Variable=Variable1;
			goto MOperation;
		}
	}
	
	return Variable;
}
void CCompileModule::SetParent(CCompileModule *pSetParent,CCompileModule *pSetStopParent,CCompileModule *pSetContinueParent)
{
	cByteCode.pParent=0;

	pParent=pSetParent;
	cContext.pParent=0;
	if(pParent)
	{
		cByteCode.pParent=&pParent->cByteCode;
		cContext.pParent=&pParent->cContext;
	}
 
	pStopParent=pSetStopParent;
	cContext.pStopParent=0;
	if(pStopParent)
		cContext.pStopParent=&pStopParent->cContext;

	pContinueParent=pSetContinueParent;
	cContext.pContinueParent=0;
	if(pContinueParent)
		cContext.pContinueParent=&pContinueParent->cContext;

	OnSetParent(pParent);
};


void CCompileModule::SetImport()
{
	//GETKeyWord(KEY_IMPORT);
	//CString csPath=GETConstant().GetString();

}

void ToDebugParam(/*int nOpCode,*/SParamRun Param)
{

	CString Str;
	Str.Format("%d,%d",Param.nArray,Param.nIndex);

/*	if(	(nOpCode>=OPER_ADD&&nOpCode<=OPER_SUB)||
		(nOpCode>=OPER_ADD&&nOpCode<=OPER_SUB)||
*/
	if(Param.nArray==DEF_VAR_TEMP)
		Str.Format("Temp%d",Param.nIndex);
	else
	if(Param.nArray==DEF_VAR_CONST)
		Str.Format("Const%d",Param.nIndex);
	else
	if(Param.nArray==DEF_VAR_DEFAULT)
		Str="Defualt";
/*	else
	if(Param.nArray==0)
		Str.Format("Local%d",Param.nIndex);
	else
	if(Param.nArray>0&&Param.nArray<DEF_VAR_CONST)
		Str.Format("Clobal%d",Param.nIndex);
		*/
	OutToDebug("%10s ",Str);
}
void CCompileModule::ToDebug()
{
	int i;
	OutToDebug("\n\n%s байт-код:\n",csModuleName);
	OutToDebug("CodeList (size=%d):\n",cByteCode.CodeList.GetSize());
	int nPrevnNumberLine=-1;
	for(i=0;i<cByteCode.CodeList.GetSize();i++)
	{
		CString Str;
		CByte byte=cByteCode.CodeList[i];
		int nOperation=byte.nOper;
		if(nOperation>=TYPE_DELTA3)
			nOperation-=TYPE_DELTA3;
		else
		if(nOperation>=TYPE_DELTA2)
			nOperation-=TYPE_DELTA2;
		else
		if(nOperation>=TYPE_DELTA1)
			nOperation-=TYPE_DELTA1;

		Str=aOperNames[nOperation];
		if(byte.nOper!=nOperation)
			Str=Str+"!";//признак типизированной (ускоренной) операции

		//строка исходного кода
		CString csSource;
		if(nPrevnNumberLine!=byte.nNumberLine)
		{
			if(byte.nNumberLine>=0)
				csSource=FindErrorCodeLine(Buffer,byte.nStringNumber,2);
			nPrevnNumberLine=byte.nNumberLine;
		}

		OutToDebug("%4d.",i);
		OutToDebug("%4d  ",byte.nNumberLine);
		OutToDebug("%10s ",Str);
		ToDebugParam(byte.Param1);
		ToDebugParam(byte.Param2);
		ToDebugParam(byte.Param3);
		OutToDebug("  %s\n",csSource);
	}
	
	if(cByteCode.ConstList.GetSize()>0)
	{
		OutToDebug("\nConstList: %d\n",cByteCode.ConstList.GetSize());
		for(i=0;i<cByteCode.ConstList.GetSize();i++)
		{
			OutToDebug("%d.\t",i);
			CString Str=cByteCode.ConstList[i].GetString();
			OutToDebug("%s\n",Str);
		}
	}
}


CMapStringToPtr	CTranslateModule::HashKeyWords;//список ключевых слов
CMapStringToPtr	CCompileModule::cSysFunctions;//список системных функций

class CAutoLoader
{
public:
	CAutoLoader()
	{
		//старт программы
		CTranslateModule::LoadKeyWords();
		CCompileModule::LoadSystemFunctions();

/*		void *p;
		POSITION pos;
		CString csKey;
		//добавляем в массив для синтаксического анализатора
		for( pos = CTranslateModule::HashKeyWords.GetStartPosition(); pos != NULL; )
		{
			CTranslateModule::HashKeyWords.GetNextAssoc( pos, csKey, (void*&)p );
			aHashKeywordList[csKey]=(void*)1;
		}
		for( pos = CCompileModule::cSysFunctions.GetStartPosition(); pos != NULL; )
		{
			CCompileModule::cSysFunctions.GetNextAssoc( pos, csKey, (void*&)p );
			aHashKeywordList[csKey]=(void*)1;
		}
*/
		
		aHashKeywordList["КОНТЕКСТ"]=(void*)1;
		aHashKeywordList["БАЗОВЫЙОБЪЕКТ"]=(void*)1;
		//allWords.Add("Форма");
	};

	~CAutoLoader()//завершение программы
	{
		//OutToDebug("Удаляем статические перменные:\n");
		POSITION pos;
		CObject* pValue;
		CString csKey;
		//добавляем в массив для синтаксического анализатора
		for( pos = StaticValList.GetStartPosition(); pos != NULL; )
		{
			StaticValList.GetNextAssoc( pos, csKey, pValue);
			ASSERT(pValue);
			if(pValue)
			{
				//OutToDebug("%s\n",csKey);
				delete (CValue*)pValue;
			}
		}
	}

}m_Auto;


