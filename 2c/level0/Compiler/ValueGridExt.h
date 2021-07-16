// Авторские права: VTOOLS.RU (info@vtools.ru) 2002-2004г.
// ValueGridExt.h: interface for the CValueGridExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueGridExt_H__2F1ABF64_4DA3_41A0_9C7B_8D08ED2B1FE7__INCLUDED_)
#define AFX_ValueGridExt_H__2F1ABF64_4DA3_41A0_9C7B_8D08ED2B1FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Value.h"
#include "../GridCtrl_src/View/GridView.h"

class CValueGridExt : public CValue  
{
DECLARE_DYNCREATE(CValueGridExt);
	//конструкторы
public:
	CValueGridExt();
	virtual ~CValueGridExt();


private:
	//Атрибуты:
	CString csSourceTable;

	//CMoxcel m_SourceTable;
	//CMoxcel m_DestTable;
	int m_nCurCol;
	int m_nCurRow,m_nCurRow0;
	int m_nMaxCol;

	//атрибуты метода Options
	BOOL m_bGrid;
	BOOL m_bHeader;
	BOOL m_bFixRow;
	BOOL m_bFixCol;
	CString m_csNamePrint;
	CString m_csNameSize;
	BOOL m_bBW;
	int m_nMode;

	BOOL m_bWasLoad;

	CGridDoc *m_pDoc;
	CGridCtrlExt	*m_pGrid;
	CGridCtrlExt	*m_pSourceGrid;
	//Методы:
public:
	void LoadSourceTable();
	//void CopyRange(CPoint,CRect,int);
	void CopyRange(int nStartRow,int nStartCol,int nSourceRow1,int nSourceCol1,int nSourceRow2,int nSourceCol2,int nResizeSize);
private:
	void GetRange(CString csName,CPoint &cHorizRange, CPoint &cVertRange);


public:
	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//Эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода
	virtual CString GetString(void);
	virtual CString GetTypeString(void)const;
	//virtual CValue GetAttribute(int iName);//значение атрибута

	//////////////////////////////////////////////////////////////////////
	//Пользовательские методы
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CValueGridExt ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL SourceTable(CValue &rez,CValue**ppV);
	BOOL Open(CValue &rez,CValue**ppV);
	BOOL Put(CValue &rez,CValue**ppV);
	BOOL Show(CValue &rez,CValue**ppV);
	BOOL PutSection(CValue &rez,CValue**ppV);
	BOOL AttachSection(CValue &rez,CValue**ppV);

	BOOL Options(CValue &rez,CValue**ppV);
	BOOL Write(CValue &rez,CValue**ppV);
	BOOL ReadOnly(CValue &rez,CValue**ppV);
	BOOL Clear(CValue &rez,CValue**ppV);

	BOOL SectionHeight(CValue &rez,CValue**ppV);
	BOOL SectionWidth(CValue &rez,CValue**ppV);
	BOOL TableHeight(CValue &rez,CValue**ppV);
	BOOL TableWidth(CValue &rez,CValue**ppV);

/*	CMOXCELDlg *NewGrid(CMOXCELDlg *p);
	void CheckTables();
	CSection GetSectionRange(CArray <CSection,CSection> &aSections,CString csSectionName);
	void CopyHWC(CMyGridView *pGrid1,CMyGridView *pGrid2,RECT rect);
	void CopyRange(CMyGridView *pGrid1,CMyGridView *pGrid2,RECT rect);
	RECT GetSectionsRange(CMyGridView *pGrid1,CString csName);

public:
	void SourceTable(CString csName);
	void Put();
	void Open(CString csName);
	void PutSection(CString csName,int nMode=0);//0-PutSection,1-AttachSection
	void AttachSection(CString csName);
	int TableHeight();
	int TableWidth();
	int SectionHeight(CString csName);
	int SectionWidth(CString csName);
	BOOL ReadOnly(BOOL bReadOnly);
	void Options(BOOL bGrid=1,BOOL bHeader=1,BOOL bFixRow=0,BOOL bFixCol=0,CString csNamePrint="",CString csNameSize="",BOOL bBW=0,int nMode=1);//Опции(<ВыводСетки>,<ВыводЗаголовков>,<ФиксСтрок>,<ФиксСтолбцов>,<ИмяОпцийПечати>,<ИмяСохрРазмОкна>,<ФлагЧБПросмотра>,<НаправлПерехода>)
	void Show(CString csTitle="",CString csFileName="",BOOL bActive=TRUE);
	void Clear();



private:
	//Атрибуты:
	//CMOXCELDlg *m_pSourceTable;
	CString csSourceTable;
	//CMOXCELDlg *m_pDestTable;
	int m_nCurCol;
	int m_nCurRow;

	//атрибуты метода Options
	BOOL m_bGrid;
	BOOL m_bHeader;
	BOOL m_bFixRow;
	BOOL m_bFixCol;
	CString m_csNamePrint;
	CString m_csNameSize;
	BOOL m_bBW;
	int m_nMode;

*/



};

#endif // !defined(AFX_ValueGridExt_H__2F1ABF64_4DA3_41A0_9C7B_8D08ED2B1FE7__INCLUDED_)
