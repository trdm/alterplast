//взаимодействие с глобальным поиском не удается сделать релизонезависимым,
//поэтому "точки" подключения к глобальным данным указываем явно
//поддерживаются релизы v7
//15,18,19,20,21,24,25,26,27

#pragma once

class CTFGlobalFindDataService
{
public:
	struct CGlobalFindData
	{
		CString str;
		DWORD dw1;
		CString str2;
		DWORD dw2;
		DWORD dw3;
		BOOL bBack;
		BOOL bCaseSensitive;
	};
	struct CGlobalFindData2
	{
		DWORD dw1;
		DWORD dw2;
		CString str;
		DWORD dw3;
		CTypedValue val;
	};

	static CGlobalFindData* pGlobalFindData;
	static CGlobalFindData2* pGlobalFindData2;
	static DWORD FindDialogVTAddress;
	static BOOL bGlobalFindDataResolved;
	static BOOL* pFirstFindFlag;

	static BOOL GetFlagScanOneCol() {return ::GetFlagScanOneCol();};
	static void SetFlagScanOneCol(BOOL bFlag)
	{
		CString str;
		str.Format("%d", bFlag);
		AfxGetApp()->WriteProfileString("SectionTools", "ScanOneCol", str);
	};

	static BOOL DefineGlobalFindDataAddr()
	{
		CString ver = GetModuleVersionInfo("basic.dll", "FileVersion");
		CString minorver = ver.Right(3);
		switch(atoi(minorver))
		{
		case 27:
		case 26:
			pGlobalFindData = (CGlobalFindData*)0x2A1348FC;
			pGlobalFindData2 = (CGlobalFindData2*)0x2A135398;
			FindDialogVTAddress = 0x2A0FD858;
			pFirstFindFlag = (BOOL*)0x2A13295C;
			break;
		case 25:
		case 24:
			pGlobalFindData = (CGlobalFindData*)0x2A1338FC;
			pGlobalFindData2 = (CGlobalFindData2*)0x2A134398;
			FindDialogVTAddress = 0x2A0FC848;
			pFirstFindFlag = (BOOL*)0x2A13195C;
			break;
		case 21:
		case 20:
		case 19:
		case 18:
			pGlobalFindData = (CGlobalFindData*)0x2A1338FC;
			pGlobalFindData2 = (CGlobalFindData2*)0x2A134390;
			FindDialogVTAddress = 0x2A0FC858;
			pFirstFindFlag = (BOOL*)0x2A13195C;
			break;
		case 15:
			pGlobalFindData = (CGlobalFindData*)0x2A13394C;
			pGlobalFindData2 = (CGlobalFindData2*)0x2A1343E0;
			FindDialogVTAddress = 0x2A0FC848;
			pFirstFindFlag = (BOOL*)0x2A13199C;
			break;
		default:
			pGlobalFindData = NULL;
			pGlobalFindData2 = NULL;
			FindDialogVTAddress = 0;
			pFirstFindFlag = NULL;
			return FALSE;
		}
		
		CWnd* pWnd = GetMainFrame();
		if (pWnd)
		{
			pWnd = pWnd->GetDescendantWindow(0xE128, FALSE);
			if (pWnd && IsWindow(pWnd->m_hWnd))
				pWnd->GetWindowText(pGlobalFindData2->str);
		}

		return TRUE;
	}
};

class CFindDialog : public CDialog
{
public:
	CWnd m_wnd;
	int m_direction;
	int m_1;
	int m_ScanOneCol;
	int m_CS;
	CTypedValue m_tval;
	CString m_str;
	
	CFindDialog(CWnd* pParentWnd = NULL)
		: CDialog(1097, pParentWnd)
	{
		*((DWORD*)this) = CTFGlobalFindDataService::FindDialogVTAddress;

		m_direction = 0;
		m_1 = 0;
		m_ScanOneCol = 0;
		m_CS = CTFGlobalFindDataService::pGlobalFindData->bCaseSensitive;
		m_str = CTFGlobalFindDataService::pGlobalFindData2->str;
		m_tval = CTFGlobalFindDataService::pGlobalFindData2->val;
	};
};
