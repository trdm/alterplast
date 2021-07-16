// ValueGridExt.cpp: implementation of the CValueGridExt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueGridExt.h"
#include "../enterprise.h"
#include "../ModuleManager.h"
#include "../OpenForm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void WritoToFile(CString csCFile,CString Str);
CString GetModuleText(CString csPath,BOOL &bError);
CGridCtrlExt *AfxNewGridDoc();
void AfxSetGridDoc(CGridCtrlExt *p);
CString Template(CString arg);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMethods CValueGridExt::Methods;

IMPLEMENT_DYNCREATE(CValueGridExt,CValue);
CValueGridExt::CValueGridExt()
{
	nType=100;
	csSourceTable="Таблица";

	m_nCurCol=1;
	m_nCurRow=0;
	m_nCurRow0=0;
	m_nMaxCol=0;

	m_bGrid=1;
	m_bHeader=1;
	m_bFixRow=0;
	m_bFixCol=0;
	m_bBW=0;
	m_nMode=1;

	m_bWasLoad=0;
	m_pDoc=0;
	m_pSourceGrid=AfxNewGridDoc();
	m_pGrid=AfxNewGridDoc();

}

CValueGridExt::~CValueGridExt()
{
	if(!m_pDoc)
		delete m_pGrid;
	m_pGrid=0;

	delete m_pSourceGrid;
	m_pSourceGrid=0;
}




//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CValueGridExt::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CValueGridExt::SourceTable,"SourceTable","ИсходнаяТаблица","ИсходнаяТаблица(СтрПуть)"},
	{CValueGridExt::Open,"Open","Открыть","Открыть(СтрИмяФайла)"},
	{CValueGridExt::Put,"Put","Вывести","Вывести()"},
	{CValueGridExt::PutSection,"PutSection","ВывестиСекцию","ВывестиСекцию(СтрИмяСекции)"},
	{CValueGridExt::AttachSection,"AttachSection","ПрисоединитьСекцию","ПрисоединитьСекцию(СтрИмяСекции)"},


	{CValueGridExt::Options,"Options","Опции","Опции(ВыводСетки,ВыводЗаголовков,ФиксСтрок,ФиксСтолбцов,ИмяОпцийПечати,ИмяСохрРазмОкна,ФлагЧБПросмотра,НаправлПерехода)"},

	{CValueGridExt::Show,"Show","Показать","Показать(СтрЗаголовок,СтрИмяФайла,НомАктивизировать)"},
	{CValueGridExt::Write,"Write","Записать","Записать(СтрИмяФайла)"},
	{CValueGridExt::ReadOnly,"ReadOnly","ТолькоПросмотр","ТолькоПросмотр(Флаг)"},
	{CValueGridExt::Clear,"Clear","Очистить","Очистить()"},

	{CValueGridExt::SectionHeight,"SectionHeight","ВысотаСекции","ВысотаСекции(ИмяСекции)"},
	{CValueGridExt::SectionWidth,"SectionWidth","ШиринаСекции","ШиринаСекции(ИмяСекции"},
	{CValueGridExt::TableHeight,"Clear","ВысотаТаблицы","ВысотаТаблицы()"},
	{CValueGridExt::TableWidth,"TableWidth","ШиринаТаблицы","ШиринаТаблицы()"},

	
};


#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CValueGridExt::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CValueGridExt::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CValueGridExt::GetString()
{
	return "Таблица";
}
CString CValueGridExt::GetTypeString(void)const
{
	return "Таблица";
}



//////////////////////////////////////////////////////////////////////
//Обычные методы
//////////////////////////////////////////////////////////////////////
void CValueGridExt::LoadSourceTable()
{
	if(m_bWasLoad)
		return;
	m_bWasLoad=1;
	CMetaObject *pObject=0;
	ASSERT(AfxGetModuleManager());
	if(!m_pSourceGrid->LoadMXL(AfxGetModuleManager()->GetTable(csSourceTable)))
	{
		Error(CString("Ошибка формата в таблице: \"")+csSourceTable+"\"");
	}
}

void CValueGridExt::CopyRange(int nStartRow,int nStartCol,int nSourceRow1,int nSourceCol1,int nSourceRow2,int nSourceCol2,int nResizeSize)
{
	int nMaxRow=nStartRow+(nSourceRow2-nSourceRow1);
	if(m_pGrid->GetRowCount()<nMaxRow+3)
		m_pGrid->SetRowCount(nMaxRow+5);
	int nMaxCol=nStartCol+(nSourceCol2-nSourceCol1);
	if(m_pGrid->GetColumnCount()<nMaxCol+3)
		m_pGrid->SetColumnCount(nMaxCol+5);

	m_pGrid->SetLastRowCol(nMaxRow,nMaxCol);

	for(int nRow=nSourceRow1;nRow<=nSourceRow2;nRow++)
	for(int nCol=nSourceCol1;nCol<=nSourceCol2;nCol++)
	{
		int nRow2=nStartRow+nRow-nSourceRow1;
		int nCol2=nStartCol+nCol-nSourceCol1;
		if(nResizeSize&&nRow==nSourceRow1)
		{
			int nWidth=m_pSourceGrid->GetColumnWidth(nCol);
			m_pGrid->SetColumnWidth(nCol2,nWidth);
		}
		if(nResizeSize&&nCol==nSourceCol1)
		{
			m_pGrid->SetRowHeight(nRow2,m_pSourceGrid->GetRowHeight(nRow));
		}

		CGridCellBase* pCell = m_pSourceGrid->GetCell(nRow, nCol);
		ASSERT(pCell);
		if (!pCell)
			continue;

		CGridCellBase* pCell2 = m_pGrid->GetCell(nRow2, nCol2);
		ASSERT(pCell2);
		if (!pCell2)
			continue;

		if(pCell->IsCovered())
		{
			if(!pCell->GetCoveredCell())
			{
				m_pGrid->SetCoveredCells(nRow2, nCol2,pCell->GetCoveredRow(),pCell->GetCoveredCol());
			}
			else
			{
				continue;
			}
		}

		CCell *pcell=pCell->GetCell();
		if(pcell)
		{
			CString Str=pCell->GetText();
			if(1==pcell->nMode)
			{
				Str=CProcUnit::Eval(Str);
			}
			else
			if(pcell->nMode>=2)
			{
				Str=Template(Str);
			}

			pCell2->SetCell(pcell);
			pCell2->SetText(Str);
		}
	}
}



//////////////////////////////////////////////////////////////////////
//Пользовательские методы
//////////////////////////////////////////////////////////////////////



BOOL CValueGridExt::SourceTable(CValue &rez,CValue**ppV)
{
	m_bWasLoad=0;
	csSourceTable=ppV[0]->GetString();
	LoadSourceTable();

	return TRUE;
}


BOOL CValueGridExt::Open(CValue &rez,CValue**ppV)
{
	CString csFileName=ppV[0]->GetString();

	CString csBuffer=AfxGetModuleManager()->GetTable(csFileName);

	AfxSetGridDoc(m_pGrid);
	m_pGrid->LoadMXL(csBuffer);

	m_pDoc=(CGridDoc*)OpenFormMDI("Таблица",1);
	if(m_pDoc)
	{
		m_pDoc->SetPathName(csFileName,0);
		m_pDoc->SetTitle(csFileName);
		//m_pDoc->GetView()->GetParent()->ShowWindow(1);
	}

	return TRUE;
}


BOOL CValueGridExt::Show(CValue &rez,CValue**ppV)
{
	CString csTitle=ppV[0]->GetString();
	CString csFileName=ppV[1]->GetString();
	int nActivate=ppV[2]->GetNumber();
	if(nActivate<0)
	{
		if(m_pDoc)
		{
			m_pDoc->OnCloseDocument();
		}
		m_pDoc=0;
		m_pGrid=AfxNewGridDoc();
		return 1;
	}

	LoadSourceTable();

	if(m_pDoc)
	{
		//m_pDoc->GetGrid()->m_Data=m_DestTable;
		//m_pDoc->GetGrid()->LoadMXL(m_pDoc->GetGrid()->m_Data);
	}
	else
	{

		AfxSetGridDoc(m_pGrid);

		m_pDoc=(CGridDoc*)OpenFormMDI("Таблица",1);
		if(m_pDoc)
		{
			//m_pDoc->GetGrid()->m_Data=m_DestTable;
			//m_pDoc->GetGrid()->LoadMXL();
			if(!csFileName.IsEmpty())
				m_pDoc->SetPathName(csFileName,0);
			m_pDoc->SetTitle(csTitle);
		} 
	}

	if(m_pDoc)
		if(nActivate>0)
			if(m_pDoc->GetView()->GetParent())
				m_pDoc->GetView()->GetParent()->BringWindowToTop();


	return TRUE;
}


BOOL CValueGridExt::Put(CValue &rez,CValue**ppV)
{
	LoadSourceTable();

	CopyRange(1,1,1,1,m_pSourceGrid->GetRowCount()-1,m_pSourceGrid->GetColumnCount()-1,1);

	return TRUE;
}
void CValueGridExt::GetRange(CString csStr,CPoint &cHorizRange, CPoint &cVertRange)
{
	cHorizRange=CPoint(1,1);
	cVertRange=CPoint(1,1);


	CString csHorizSection;
	CString csVertSection;
	int nIndex=csStr.Find("|");
	if(nIndex>=0)
	{
		csHorizSection=csStr.Left(nIndex);
		csVertSection=csStr.Mid(nIndex+1);
	}
	else
	{
		csHorizSection=csStr;
	}

	cHorizRange=m_pSourceGrid->GetHorizRange(csHorizSection);
	if(!csVertSection.IsEmpty())
		cVertRange=m_pSourceGrid->GetVertRange(csVertSection);
	else
		cVertRange=CPoint(1,m_pSourceGrid->GetColumnCount()-1);

}
BOOL CValueGridExt::PutSection(CValue &rez,CValue**ppV)
{
	LoadSourceTable();

	CPoint cHorizRange;
	CPoint cVertRange;
	GetRange(ppV[0]->GetString(),cHorizRange,cVertRange);

	m_nCurRow++;
	m_nCurCol=1;
	m_nCurRow0=m_nCurRow;//для команды ПрисоединитьСекцию


	CopyRange(m_nCurRow,m_nCurCol,cHorizRange.x,cVertRange.x,cHorizRange.y,cVertRange.y,1==m_nCurRow);

	m_nCurRow+=cHorizRange.y-cHorizRange.x;
	m_nCurCol+=cVertRange.y-cVertRange.x;

	m_nMaxCol=max(m_nMaxCol,m_nCurCol);
	return TRUE;
}

BOOL CValueGridExt::AttachSection(CValue &rez,CValue**ppV)
{
	LoadSourceTable();

	CPoint cHorizRange;
	CPoint cVertRange;
	GetRange(ppV[0]->GetString(),cHorizRange,cVertRange);

	m_nCurCol++;
	if(m_nCurRow0<1)
		m_nCurRow0=1;


	CopyRange(m_nCurRow0,m_nCurCol,cHorizRange.x,cVertRange.x,cHorizRange.y,cVertRange.y,1==m_nCurRow0);

	m_nCurCol+=cVertRange.y-cVertRange.x;

	m_nMaxCol=max(m_nMaxCol,m_nCurCol);
	return TRUE;
}


BOOL CValueGridExt::Options(CValue &rez,CValue**ppV)
{
/*
	m_bGrid=ppV[0]->GetNumber();
	m_bHeader=ppV[1]->GetNumber();
	m_bFixRow=ppV[2]->GetNumber();
	m_bFixCol=ppV[3]->GetNumber();
	m_csNamePrint=ppV[4]->GetString();
	m_csNameSize=ppV[5]->GetString();
	m_bBW=ppV[6]->GetNumber();
	m_nMode=ppV[7]->GetNumber();
*/
	LoadSourceTable();
	m_pGrid->ViewGridLines(ppV[0]->GetNumber());
	m_pGrid->ViewHeader(ppV[1]->GetNumber());
	return TRUE;
}

BOOL CValueGridExt::Write(CValue &rez,CValue**ppV)
{
	LoadSourceTable();
	CString Buffer;
	m_pGrid->SaveMXL(Buffer);
	WritoToFile(ppV[0]->GetString(),Buffer);
	return TRUE;
}

BOOL CValueGridExt::ReadOnly(CValue &rez,CValue**ppV)
{
	LoadSourceTable();
	rez=!m_pGrid->IsEditable();
	m_pGrid->SetEditable(!ppV[0]->GetNumber());
	return TRUE;
}

BOOL CValueGridExt::Clear(CValue &rez,CValue**ppV)
{
	LoadSourceTable();
	m_pGrid->Clear();
	return TRUE;
}

BOOL CValueGridExt::SectionHeight(CValue &rez,CValue**ppV)
{
	LoadSourceTable();

	CPoint cHorizRange;
	CPoint cVertRange;
	GetRange(ppV[0]->GetString(),cHorizRange,cVertRange);

	rez=cHorizRange.y-cHorizRange.x;
	return TRUE;
}
BOOL CValueGridExt::SectionWidth(CValue &rez,CValue**ppV)
{
	LoadSourceTable();
	CPoint cHorizRange;
	CPoint cVertRange;
	GetRange(ppV[0]->GetString(),cHorizRange,cVertRange);

	rez=cVertRange.y-cVertRange.x;
	return TRUE;
}
BOOL CValueGridExt::TableHeight(CValue &rez,CValue**ppV)
{
	rez=m_nCurRow;
	return TRUE;
}
BOOL CValueGridExt::TableWidth(CValue &rez,CValue**ppV)
{
	rez=m_nCurRow;
	return TRUE;
}
