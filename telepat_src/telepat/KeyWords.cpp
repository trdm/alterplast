// KeyWords.cpp: implementation of the CKeyWords class.
#include "stdafx.h"
#include "KeyWords.h"
#include "resource.h"
#include "tentrydrawing.h"
#include "TeleGroup.h"
#include "TeleList.h"
#include "TxtTrap.h"
#include "parser.h"
#include "option.h"
#include "plugin.h"

static CTeleGroup gr;
static CTeleGroup templ;

class CKWEntry:public CTeleEntry
{
public:
	CKWEntry(CKeyWords::SKWInfo* pInfo,int lang):
	  CTeleEntry(pInfo->name[lang]),m_pInfo(pInfo){}
	CKeyWords::SKWInfo* m_pInfo;
	void GetHint(CString& buf)
	{
		buf=m_Name+"\nКлючевое слово";
		if(m_pInfo->descr && strlen(m_pInfo->descr))
			buf=buf+"\n"+m_pInfo->descr;
	}
	void GetInsert(CString& buf)
	{
		buf=m_Name+m_pInfo->delim;
		if(m_pInfo->name[0]=="Return")
		{
			if(!(CTxtTrap::GetParser()->m_flags & CParser::InProc))
				buf+=" !";
			buf+=';';
		}
	}
	bool MakeInsert()
	{
		CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
		CPoint ptStart,ptEnd;
		ptEnd=CTeleList::m_pOneList->Insert(this);
		if(IsWindow(pEditor->m_hWnd))
		{
			ptStart=ptEnd;
			ptStart.x=0;
			pEditor->MoveCaret(ptStart,ptEnd,0);
			pEditor->SendMessage(WM_COMMAND,33289);

			CTextDocument* pDoc=pEditor->GetDocument();
			CString txt;
			pDoc->GetSelectedText(txt);
			pDoc->GetSel(ptStart,ptEnd);
			pEditor->MoveCaret(ptEnd,ptEnd,0);
			if(m_pInfo->NeedCR)
				pEditor->OnChar('\r',0,0);
		}
		pEditor->GetMyParser()->m_NeedQuick=true;
		return true;
	}
};

class CKWTempl:public CTeleEntry
{
public:
	int m_lang;
	CKWTempl(CKeyWords::SKWTemplates* pInfo,int lang):
	  CTeleEntry(pInfo->name[lang]),m_pInfo(pInfo),m_lang(lang){}
	CKeyWords::SKWTemplates* m_pInfo;
	void GetHint(CString& buf)
	{
		buf=m_pInfo->insert[m_lang];
		buf.Replace("\r\n","\n");
		buf.Replace("\t","    ");
		buf.Replace("!","¦");
		if(m_pInfo->descr && strlen(m_pInfo->descr))
			buf=buf+"\n"+m_pInfo->descr;
	}
	bool MakeInsert()
	{
		CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
		CPoint ptStart=CTeleList::m_pOneList->m_CaretPoint,ptEnd;
		ptStart.x-=CTeleList::m_pOneList->m_curPosInBuf;
		ptEnd=ptStart;
		ptEnd.x+=CTeleList::m_pOneList->m_buf.GetLength();

		CString ins=m_pInfo->insert[m_lang];
		if(CPlugin::m_pPlugin)
			CPlugin::m_pPlugin->FireInsert(m_pParent->m_Image,m_pInfo->name[m_lang],ins);
		CString line,tab="\r\n",tab1="";
		
		// Получим текущую строку
		CTextDocument* pDoc=pEditor->GetDocument();
		pDoc->GetLine(ptStart.y,line);
		// Удалим перевод строки
		int ll=line.GetLength();
		if(line.Right(2)=="\r\n")
			line=line.Left(ll-=2);
		// Проверка на положение каретки за концом строки
		if(ll<ptStart.x)
		{
			for(int i=ll;i<ptStart.x;i++)
				tab1+=' ';
			ins=tab1+ins;
			ptStart.x=ll;
			ptEnd.x=ptStart.x+2;
		}
		// Сформируем отступ
		for(int i=0;i<ptStart.x;i++)
		{
			if(line[i]=='\t')
				tab+='\t';
			else
				tab+=' ';
		}
		// Сформируем текст для вставки
		ins.Replace("\r\n",tab+tab1);
		ins.Replace("!","¦");
		// Вставим текст
		pDoc->SetSel(ptStart,ptEnd);
		pDoc->Replace(ins);
		// Вычислим, докуда он вставился
		LPCTSTR ptr=ins;
		ptEnd=ptStart;
		while(*ptr)
		{
			if(*ptr=='\n')
			{
				ptEnd.y++;
				ptEnd.x=0;
			}
			else
				ptEnd.x++;
			ptr++;
		}
		// Выделим вставленный блок и отформатируем его
		pEditor->MoveCaret(ptStart,ptEnd,TRUE);
		pEditor->SendMessage(WM_COMMAND,33289);
		// Найдем место для каретки
		for(;ptStart.y<=ptEnd.y;ptStart.y++)
		{
			pDoc->GetLine(ptStart.y,line);
			ptStart.x=line.Find("¦")+1;
			if(ptStart.x>0)
			{
				pDoc->DeleteSymb(1,ptStart);
				ptStart.x--;
				pEditor->MoveCaret(ptStart,ptStart,1);
				break;
			}
		}
		// Скроем список		
		CTeleList::m_pOneList->Hide();
		pEditor->GetMyParser()->m_NeedQuick=true;
		return true;
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CKeyWords::SKWInfo CKeyWords::keyWords[kwLast]={
	{{"AND","И"},"Логическое И"," (!) ",false},
	{{"Break","Прервать"},"Прерывает выполнение цикла",";",false},
	{{"Context","Контекст"},"Передача локального контекста как параметра","",false},
	{{"Continue","Продолжить"},"Передает управление в начало цикла",";",false},
	{{"Do","Цикл"},"","",true},
	{{"Else","Иначе"},"","",true},
	{{"ElsIf","ИначеЕсли"},""," ",false},
	{{"EndDo","КонецЦикла"},"",";",false},
	{{"EndFunction","КонецФункции"},"","",false},
	{{"EndIf","КонецЕсли"},"Завершает структуру оператора условного выполнения",";",false},
	{{"EndProcedure","КонецПроцедуры"},"","",false},
	{{"EndTry","КонецПопытки"},"",";",false},
	{{"Except","Исключение"},"","",true},
	{{"Export","Экспорт"},"","",false},
	{{"For","Для"},""," ",false},
	{{"Forward","Далее"},"","",false},
	{{"Function","Функция"},"Объявление функции"," ",false},
	{{"GoTo","Перейти"},"Безусловная передача управления на другой оператор программы"," ~!;",false},
	{{"If","Если"},"Оператор условного выполнения"," ",false},
	{{"NOT","НЕ"},""," (!) ",false},
	{{"OR","ИЛИ"},""," (!) ",false},
	{{"Procedure","Процедура"},"Объявление процедуры"," ",false},
	{{"Return","Возврат"},"Завершение процедуры или функции","",false},
	{{"Then","Тогда"},"","",true},
	{{"To","По"},""," ",false},
	{{"Try","Попытка"},"","",true},
	{{"Val","Знач"},"Передача параметра метода по значению"," ",false},
	{{"Var","Перем"},"Объявление переменной"," !;",false},
	{{"While","Пока"},""," ",false},
};

CKeyWords::SKWTemplates CKeyWords::keyTemplates[]={
	{{"ElsIf_Then","ИначеЕсли_Тогда"},
		{
		"ElsIf ! Then",
		"ИначеЕсли ! Тогда"
		},
		"Вставка блока \"ИначеЕсли\""
	},
	{{"For_EndDo","Для_КонецЦикла"},
		{
		"For != To  Do\r\n"
		"EndDo;",
		"Для != По  Цикл\r\n"
		"КонецЦикла;"
		},
		"Вставка блока цикла \"Для\""
	},
	{{"Function_EndFunction","Функция_КонецФункции"},
		{
		"Function !()\r\n"
		"EndFunction",
		"Функция !()\r\n"
		"КонецФункции"
		},
		"Вставка блока функции"
	},
	{{"If_EndIf","Если_КонецЕсли"},
		{
		"If ! Then\r\n"
		"EndIf;",
		"Если ! Тогда\r\n"
		"КонецЕсли;"
		},
		"Вставка блока условия"
	},
	{{"Procedure_EndProcedure","Процедура_КонецПроцедуры"},
		{
			"Procedure !()\r\n"
			"EndProcedure",
			"Процедура !()\r\n"
			"КонецПроцедуры"
		},
		"Вставка блока процедуры"
	},
	{{"Try_EndTry","Попытка_КонецПопытки"},
		{
			"Try\r\n"
			"\t!\r\n"
			"Except\r\n"
			"EndTry;",
			"Попытка\r\n"
			"\t!\r\n"
			"Исключение\r\n"
			"КонецПопытки;"
		},
		"Вставка блока попытки"
	},
	{{"While_EndDo","Пока_КонецЦикла"},
		{
			"While ! Do\r\n"
			"EndDo;",
			"Пока ! Цикл\r\n"
			"КонецЦикла;",
		},
		"Вставка блока цикла \"Пока\""
	},
};

CKeyWords::CKeyWords():m_allowedWords(0)
{
}


void CKeyWords::FillGroup(CPtrArray& arr)
{
	gr.m_Image=IDI_KEYWORD_;
	templ.m_Image=IDI_TEMPLATES_;
	gr.ClearEntries();
	templ.ClearEntries();
	int lng=COption::m_pOption->GetLanguage();
	static const int tts[]={-1,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,1,-1,2,-1,3,-1,-1,4,-1,-1,-1,5,-1,-1,6,};
	for(int i=0;i<kwLast;i++)
	{
		if(m_allowedWords & (1<<i))
		{
			if(lng & 1)
			{
				CTeleEntry* pEntry=new CKWEntry(keyWords+i,0);
				gr.AddEntry(pEntry);
			}
			if(lng & 2)
			{
				CTeleEntry* pEntry=new CKWEntry(keyWords+i,1);
				gr.AddEntry(pEntry);
			}
			if(tts[i]>-1)
			{
				if(lng & 1)
				{
					CTeleEntry* pEntry=new CKWTempl(keyTemplates+tts[i],0);
					templ.AddEntry(pEntry);
				}
				if(lng & 2)
				{
					CTeleEntry* pEntry=new CKWTempl(keyTemplates+tts[i],1);
					templ.AddEntry(pEntry);
				}
			}
		}
	}
	if(gr.GetCount())
	{
		arr.Add(&gr);
		if(templ.GetCount())
			arr.Add(&templ);
	}
}
