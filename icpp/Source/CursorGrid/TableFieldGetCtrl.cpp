#include "stdafx.h"
#include "V7CursorGridCtrl.h"
#include "GlobalFindSupport.h"
#include "TableFieldGetCtrl.h"

CTFGlobalFindDataService::CGlobalFindData* CTFGlobalFindDataService::pGlobalFindData = NULL;
CTFGlobalFindDataService::CGlobalFindData2* CTFGlobalFindDataService::pGlobalFindData2 = NULL;
DWORD CTFGlobalFindDataService::FindDialogVTAddress = 0;
BOOL* CTFGlobalFindDataService::pFirstFindFlag = NULL;
BOOL CTFGlobalFindDataService::bGlobalFindDataResolved = CTFGlobalFindDataService::DefineGlobalFindDataAddr();

CV7GlobalFindData V7GlobalFindData;

BOOL CTableFieldGetCtrl::OnFindCommand(UINT dwID)
{
	CV7CursorGridCtrl* pV7Grid = (CV7CursorGridCtrl*)GetDlgItem();
	if (!pV7Grid)
		return TRUE;

	pV7Grid->m_Tip.Hide();

	BOOL bFromBegin = FALSE;

	switch(dwID)
	{
	case ID_EDIT_FIND:
		{
			if (!pV7Grid->OnFindDialog(&V7GlobalFindData))
				return TRUE;

			if (CTFGlobalFindDataService::bGlobalFindDataResolved)
			{
				CFindDialog dlg;
				if ( dlg.DoModal() != IDOK )
					return TRUE;

				CTFGlobalFindDataService::pGlobalFindData->bBack = dlg.m_direction == 1;
				CTFGlobalFindDataService::pGlobalFindData->bCaseSensitive = dlg.m_CS;
				
				bFromBegin = dlg.m_direction == 2;

				if ( dlg.m_tval.GetTypeCode() == STRING_TYPE_1C )
				{
					CWnd* pWnd = GetMainFrame()->GetDescendantWindow(0xE128, FALSE);
					if (pWnd && IsWindow(pWnd->m_hWnd))
						pWnd->SetWindowText(dlg.m_str);
				}
			}
		}
		break;
	case ID_FIND_BACKWARD:
	case ID_FIND_FORWARD:
	case ID_EDIT_REPEAT:
		{
			if (CTFGlobalFindDataService::bGlobalFindDataResolved)
			{
				if ( dwID == ID_FIND_BACKWARD )
					CTFGlobalFindDataService::pGlobalFindData->bBack = TRUE;
				else if ( dwID == ID_FIND_FORWARD )
					CTFGlobalFindDataService::pGlobalFindData->bBack = FALSE;

				CWnd* pWnd = GetMainFrame()->GetDescendantWindow(0xE128, FALSE);
				if (pWnd && IsWindow(pWnd->m_hWnd) /*&& IsWindow(::GetFocus())*/)
				{
					if ( *(CTFGlobalFindDataService::pFirstFindFlag) || CTFGlobalFindDataService::pGlobalFindData2->val.type == STRING_TYPE_1C
						|| pWnd->IsChild(CWnd::FromHandle(::GetFocus())) )
					{
						CString str;
						pWnd->GetWindowText(str);
						if (!str.IsEmpty())
						{
							CTFGlobalFindDataService::pGlobalFindData2->str = str;
							CTFGlobalFindDataService::pGlobalFindData2->val.SetTypeCode(STRING_TYPE_1C);
							CTFGlobalFindDataService::pGlobalFindData2->val = str;
						}
						*CTFGlobalFindDataService::pFirstFindFlag = FALSE;
					}
				}
			}
		}
		break;
	default:
		return TRUE;
	}

	if (!pV7Grid->OnFind(&V7GlobalFindData))
		return TRUE;

	if (!pV7Grid->IsFocused())
		pV7Grid->SetFocus();
	
	BOOL bOneCol = CTFGlobalFindDataService::GetFlagScanOneCol();

	CDataProvider* pProvider = pV7Grid->GetDataProvider(rtBody);
	if (pProvider)
	{
		CDataRowPtr ptrCurDataRow;
		if (!bFromBegin)
		{
			if (pV7Grid->m_nCurrentDataRowIndex == -1)
				ptrCurDataRow = pV7Grid->m_pCurrentDataRowOutOfWindow;
			else
				ptrCurDataRow = pV7Grid->m_rows[pV7Grid->m_nCurrentDataRowIndex]->GetDataRowPtr();
		}

		CDataProviderScaner scaner;
		scaner.Init(pV7Grid, pV7Grid->m_pV7RowFormat, pProvider, pV7Grid->GetColumns());
		
		if (bFromBegin && !bOneCol)
			scaner.SetStartPosition(ptrCurDataRow, -1);
		else
			scaner.SetStartPosition(ptrCurDataRow, pV7Grid->m_nCurrentColumn);

		if (CTFGlobalFindDataService::bGlobalFindDataResolved)
		{
			scaner.SetSearchConditions(
				CTFGlobalFindDataService::pGlobalFindData2->str,
				CTFGlobalFindDataService::pGlobalFindData2->val,
				CTFGlobalFindDataService::pGlobalFindData->bBack,
				CTFGlobalFindDataService::pGlobalFindData->bCaseSensitive,
				bOneCol
				);
		}

		int res = scaner.Find(bFromBegin);

		switch (res)
		{
		case 1:
			{
				CDataRow* pFindDataRow = scaner.GetFindRow();
				pProvider->SetCurrentRow(pFindDataRow);
				pV7Grid->SetCurrentColumn(scaner.GetCurrentColumn());
				//::MessageBeep(MB_ICONASTERISK); //пикалка при успешном поиске
			}
			break;
		case 0:
			{
				CString strMsg = ResString(0x12BA);
				TranStr(strMsg, "$1", scaner.GetSearchString());
				::AfxMessageBox(strMsg, MB_OK, 0);

			}
		    break;
		case -1:
			{
				//::AfxMessageBox("Поиск прерван", MB_ICONASTERISK, 0);
			}
			break;
		}
	}
	return TRUE;
}

CString CTableFieldGetCtrl::GetCurrentCellText()
{
	CString str;
	
	CV7CursorGridCtrl* pV7Grid = (CV7CursorGridCtrl*)GetDlgItem();
	if (pV7Grid)
	{
		if (CGridColumn *pColumn = pV7Grid->GetCurrentColumn())
		{
			CDataRowPtr ptrCurDataRow;
			if (pV7Grid->m_nCurrentDataRowIndex == -1)
				ptrCurDataRow = pV7Grid->m_pCurrentDataRowOutOfWindow;
			else
				ptrCurDataRow = pV7Grid->m_rows[pV7Grid->m_nCurrentDataRowIndex]->GetDataRowPtr();

			if (ptrCurDataRow)
			{
				pV7Grid->m_pV7RowFormat->Format(ptrCurDataRow);
				str = static_cast<CV7GridColumn*>(pColumn)->m_pCellFormat->GetText();
			}
		}
	}
		
	return str;
}

BOOL CTableFieldGetCtrl::OnBufferCommand(UINT dwID)
{
	CV7CursorGridCtrl* pV7Grid = (CV7CursorGridCtrl*)GetDlgItem();
	if (!pV7Grid)
		return TRUE;

	switch(dwID)
	{
	case ID_EDIT_COPY:
	case ID_MEM_SAV:
	case ID_MEM_ADD:
	case ID_MEM_SUB:
		{
			CString str = GetCurrentCellText();
			AllTrim(str);
			
			if (dwID != ID_EDIT_COPY)
			{
				str.Remove(' ');
				str.Replace(',','.');
				CNumeric num = ClipSumStrToNum(str);
				str = ClipSumNumToStr(num);
			}
			if (!pV7Grid->OnClipboard(dwID, str))
				return TRUE;

			if (pV7Grid->OpenClipboard())
			{
				HGLOBAL hGlobal;
				if (dwID != ID_EDIT_COPY)
				{
					CNumeric num = ClipSumStrToNum(str);
					if (dwID != ID_MEM_SAV)
					{
						CString strInClipboard;
						hGlobal = GetClipboardData(CF_TEXT);
						if (hGlobal)
						{
							strInClipboard = (LPSTR)GlobalLock(hGlobal);
							GlobalUnlock(hGlobal);
						}
						CNumeric numInClipboard = ClipSumStrToNum(strInClipboard);
						CNumeric numInCell(num);
						if (dwID == ID_MEM_ADD)
							num = numInClipboard + numInCell;
						else
							num = numInClipboard - numInCell;
					}
					str = ClipSumNumToStr(num);
				}

				hGlobal = GlobalAlloc(GMEM_MOVEABLE, str.GetLength() + 1);
				if (hGlobal)
				{
					lstrcpy((LPSTR)GlobalLock(hGlobal), str);
					GlobalUnlock(hGlobal);
				}
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hGlobal);
				CloseClipboard();
			}
		}
		break;
	case ID_EDIT_PASTE:
		{
			CString str;
			if (pV7Grid->OpenClipboard())
			{
				HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
				if (hGlobal)
				{
					str = (LPSTR)GlobalLock(hGlobal);
					GlobalUnlock(hGlobal);
				}
			}
			pV7Grid->OnClipboard(dwID, str);
		}
		break;
	default:
		return TRUE;
	}

	return TRUE;
}

void CTableFieldGetCtrl::OnUpdateCommandUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

BEGIN_MESSAGE_MAP(CTableFieldGetCtrl, CEditGet)
	//{{AFX_MSG_MAP(CTableFieldGetCtrl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND,    OnUpdateCommandUI) //ctrl + F3 - искать
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT,  OnUpdateCommandUI) //F3 - повторить поиск
	ON_UPDATE_COMMAND_UI(ID_FIND_FORWARD, OnUpdateCommandUI) //shift + F3 - искать вперед
	ON_UPDATE_COMMAND_UI(ID_FIND_BACKWARD,OnUpdateCommandUI) //alt + F3 - искать назад
	ON_COMMAND_EX(ID_EDIT_FIND,    OnFindCommand)
	ON_COMMAND_EX(ID_EDIT_REPEAT,  OnFindCommand)
	ON_COMMAND_EX(ID_FIND_FORWARD, OnFindCommand)
	ON_COMMAND_EX(ID_FIND_BACKWARD,OnFindCommand)

	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,   OnUpdateCommandUI)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,  OnUpdateCommandUI)
	ON_COMMAND_EX(ID_EDIT_COPY,  OnBufferCommand)
	ON_COMMAND_EX(ID_EDIT_PASTE, OnBufferCommand)
	ON_COMMAND_EX(ID_MEM_SAV,    OnBufferCommand)
	ON_COMMAND_EX(ID_MEM_ADD,    OnBufferCommand)
	ON_COMMAND_EX(ID_MEM_SUB,    OnBufferCommand)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTableFieldGetCtrl::PreTranslateMessage(struct tagMSG *msg)
{
	//переопределяем CGetCtrl::PreTranslateMessage() в котором своя, нам не нужная, обработка команд работы с буфером
	return FALSE;
}



void CTableFieldGetCtrl::SetCtrlText(char const *)
{
	//do nothing
}
void CTableFieldGetCtrl::GetCtrlText(char *pCh,int n)
{
	CEditGet::GetCtrlText(pCh, n);
}
void CTableFieldGetCtrl::GetCtrlText(class CString &str)
{
	CEditGet::GetCtrlText(str);
}
void CTableFieldGetCtrl::SetReadOnly(int)
{
	//do nothing
}
int CTableFieldGetCtrl::DataExchange(int)
{
	//do nothing
	return TRUE;
}
void CTableFieldGetCtrl::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;
	GetDlgItem()->EnableWindow(bEnable);
}


/////////////////////////////////////////////////////////////////////////////
// CDataProviderScaner
/////////////////////////////////////////////////////////////////////////////
CDataProviderScaner::CDataProviderScaner()
{
	m_pProvider = NULL;
	m_pCurDataRow.reset();
	m_pPosDataRow.reset();
	m_nCurColumnIndex = -1;
	m_bBack = FALSE;
	m_bCS = FALSE;
	m_bOneCol = FALSE;
	m_nScanCnt = 0;
}

CDataProviderScaner::~CDataProviderScaner()
{
	EscCheck(AFX_IDS_IDLEMESSAGE);
}

void CDataProviderScaner::Init(CV7CursorGridCtrl* pV7Grid, CRowFormat* pRowFormat, CDataProvider* pProvider, CGridColumns* pColumns)
{
	m_pV7Grid = pV7Grid;
	m_pRowFormat = pRowFormat;
	m_pColumns = pColumns;
	m_pProvider = pProvider;
	m_nCurColumnIndex = -1;
	m_bBack = FALSE;
	m_bCS = FALSE;
	m_bOneCol = FALSE;
}

void CDataProviderScaner::SetStartPosition(CDataRowPtr const& pCurDataRow, int nCurColIndex)
{
	m_pPosDataRow.reset();
	m_pCurDataRow = pCurDataRow;
	m_nCurColumnIndex = nCurColIndex;
}

void CDataProviderScaner::SetSearchConditions(CString str, CValue val, BOOL bBack, BOOL bCS, BOOL bOneCol)
{
	m_str = str;
	m_val = val;
	m_bBack = bBack;
	m_bCS = bCS;
	m_bOneCol = bOneCol;
}

CDataRow* CDataProviderScaner::GetFindRow()
{
	return m_pCurDataRow.get();
	//return m_pPosDataRow.get();
}

CDataRow* CDataProviderScaner::GetCurrentRow()
{
	return m_pCurDataRow.get();
}

CGridColumn* CDataProviderScaner::GetCurrentColumn()
{
	return m_pColumns->GetAt(m_nCurColumnIndex);
}

#define FIND_ROWS_FRAME_SIZE 22

BOOL CDataProviderScaner::EscCheck(CString strMsg)
{
	CApp7* pApp = (CApp7*)AfxGetApp();
	CWnd* pWnd = pApp->GetMainWnd();
	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, pWnd);
	if (pFrame)
		pFrame->SetMessageText(strMsg);

	MSG msg;
/*
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN)
		{
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			if (msg.wParam == VK_ESCAPE)
				return FALSE;
		}
		else
		{
			if (!pApp->PumpMessage())
			{
				PostQuitMessage(0);
				return FALSE;
			}
		}
	}
*/

	/*
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
			return FALSE;
	}
	*/

	PeekMessage(&msg, 0, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE);
	if (PeekMessage(&msg, 0, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
	{
		if (msg.wParam == VK_ESCAPE)
			return TRUE;
	}
	return FALSE;
}
BOOL CDataProviderScaner::EscCheck(UINT ResID)
{
	return EscCheck(ResString(ResID));
}

BOOL CDataProviderScaner::NextRow()
{
	if (m_rows.empty())
	{
		int nRowsCount = m_bBack ? -FIND_ROWS_FRAME_SIZE : FIND_ROWS_FRAME_SIZE;
		m_pProvider->QueryRows(GetCurrentRow(), nRowsCount, FIND_ROWS_FRAME_SIZE);

		CDataRow* pDataRow;
		CDataRowPtr ptrDataRow;
		while (pDataRow = m_pProvider->Fetch())
		{
			ptrDataRow.reset(pDataRow);
			m_rows.push_back(ptrDataRow);
		}
	}
	if (!m_rows.empty())
	{
		m_pCurDataRow = m_rows.front();
		m_rows.pop_front();
		return TRUE;
	}

	return FALSE;
};

int CDataProviderScaner::Find(BOOL bFromBegin)
{
	int nIndexStep = m_bBack ? -1 : 1;

	int nColumnCnt = m_pColumns->GetSize();
	if (m_bOneCol)
	{
		nIndexStep *= nColumnCnt;
		if (bFromBegin)
			m_nCurColumnIndex -= nIndexStep;
	}
	m_nCurColumnIndex += nIndexStep;

	m_bIsStringFind = m_val.GetTypeCode() == STRING_TYPE_1C;
	CV7DataProvider* pV7Provider = dynamic_cast<CV7DataProvider*>(m_pProvider);
	CV7DataRow* pV7DataRow = NULL;

	BOOL bIsFound = FALSE, bEsc = FALSE;

	CString strMsg = ResString(0x12B9);
	strMsg += m_str;

	
	//CApp7* pApp = (CApp7*)AfxGetApp();
	//pApp->StartWait();
	
	do 
	{
		if (m_bIsStringFind)
			m_pRowFormat->Format(m_pCurDataRow);
		else
			pV7DataRow = pV7Provider->GetV7DataRow(GetCurrentRow());

		while( m_nCurColumnIndex >= 0 && m_nCurColumnIndex < nColumnCnt )
		{
			CGridColumn* pColumn = m_pColumns->GetAt(m_nCurColumnIndex);
			if (pColumn->GetVisible())
			{
				if (m_bIsStringFind)
				{
					CString& strText = static_cast<CV7GridColumn*>(pColumn)->m_pCellFormat->GetText();
					if (m_bCS)
					{
						if (StrStr(strText, m_str))
						{
							bIsFound = TRUE;
							break;
						}
					}
					else
					{
						if (StrStrI(strText, m_str))
						{
							bIsFound = TRUE;
							break;
						}
					}
				}
				else
				{
					const CString& strField = pColumn->GetDataFieldStr();
					if (!strField.IsEmpty())
					{
						int nFldIndex = m_pCurDataRow->GetFieldIndexOf(strField);
						if (nFldIndex != -1)
						{
							if (pV7DataRow->GetValue(nFldIndex) == m_val)
							{
								bIsFound = TRUE;
								break;
							}
						}
					}
				}
			}
			m_nCurColumnIndex += nIndexStep;
		}

		if (!bIsFound)
		{
			if (m_bOneCol)
				m_nCurColumnIndex -= nIndexStep;
			else if (m_bBack)
				m_nCurColumnIndex = nColumnCnt - 1;
			else
				m_nCurColumnIndex = 0;
		}

		m_nScanCnt++;
		if ((m_nScanCnt % 100) == 0)
		{
			CString strMsgCnt;
			strMsgCnt.Format("%s %d", strMsg, m_nScanCnt);
			bEsc = EscCheck(strMsgCnt);
		}

	} while(!bIsFound && !bEsc && NextRow());
	
	//pApp->StopWait(AFX_IDS_IDLEMESSAGE);
	
	if (bIsFound)
		return 1;
	
	if (bEsc)
		return -1;

	return 0;
}

BEGIN_BL_METH_MAP(CV7GlobalFindData)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7GlobalFindData)
	BL_PROP("Value", "Значение", &GetPropValue, &SetPropValue)
	BL_PROP("String", "Строка", &GetPropString, &SetPropString)
	BL_PROP("Back", "Назад", &GetPropBack, &SetPropBack)
	BL_PROP("CS", "УчитыватьРегистр", &GetPropCS, &SetPropCS)
	BL_PROP("OneCol", "ПоОднойКолонке", &GetPropOneCol, &SetPropOneCol)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7GlobalFindData, "GlobalFindData", "ДанныеГлобальногоПоиска", 0, NULL, NULL, -1);

CV7GlobalFindData::CV7GlobalFindData()
{
	if (CTFGlobalFindDataService::bGlobalFindDataResolved)
	{
		m_pval = &CTFGlobalFindDataService::pGlobalFindData2->val;
		m_pstr = &CTFGlobalFindDataService::pGlobalFindData2->str;
		m_pbBack = &CTFGlobalFindDataService::pGlobalFindData->bBack;
		m_pbCS = &CTFGlobalFindDataService::pGlobalFindData->bCaseSensitive;
	}
	else
	{
		m_pval = &m_val;
		m_pstr = &m_str;
		m_pbBack = &m_bBack;
		m_pbCS = &m_bCS;
	}
}

CV7GlobalFindData::~CV7GlobalFindData()
{
}

BOOL CV7GlobalFindData::GetPropValue(CValue& Value)const
{
	Value = *m_pval;
	return TRUE;
}

BOOL CV7GlobalFindData::SetPropValue(CValue const& Value)
{
	m_pval->SetTypeCode(Value.GetTypeCode());
	*m_pval = Value;
	*m_pstr = Value.Format();
	return TRUE;
}

BOOL CV7GlobalFindData::GetPropString(CValue& Value)const
{
	Value = *m_pstr;
	return TRUE;
}

BOOL CV7GlobalFindData::SetPropString(CValue const& Value)
{
	*m_pstr = Value.Format();
	m_pval->SetTypeCode(STRING_TYPE_1C);
	*m_pval = CTFGlobalFindDataService::pGlobalFindData2->str;
	return TRUE;
}

BOOL CV7GlobalFindData::GetPropBack(CValue& Value)const
{
	Value = *m_pbBack;
	return TRUE;
}

BOOL CV7GlobalFindData::SetPropBack(CValue const& Value)
{
	*m_pbBack = Value.GetNumeric();
	return TRUE;
}

BOOL CV7GlobalFindData::GetPropCS(CValue& Value)const
{
	Value = *m_pbCS;
	return TRUE;
}

BOOL CV7GlobalFindData::SetPropCS(CValue const& Value)
{
	*m_pbCS = Value.GetNumeric();
	return TRUE;
}

BOOL CV7GlobalFindData::GetPropOneCol(CValue& Value)const
{
	Value = CTFGlobalFindDataService::GetFlagScanOneCol();
	return TRUE;
}

BOOL CV7GlobalFindData::SetPropOneCol(CValue const& Value)
{
	CTFGlobalFindDataService::SetFlagScanOneCol(Value.GetNumeric());
	return TRUE;
}
