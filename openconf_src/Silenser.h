// Silenser.h: interface for the CSilenser class.
#ifndef AFX_SILENSER_H
#define AFX_SILENSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSilenser:public CFileDialog
{
public:
	static void Done();
	static void SetSilense(bool bSilence);
	static bool IsSilence();
	// Перехват вывода диалогов
	typedef int (CSilenser::*PDoModal)();
	int DoModalDialog();
	int OnInitDlgTrap();
	// Перехват вывода файловых диалогов
	int DoModalFileDialog();

	// Реальные адреса функций
	static PDoModal	 m_pFDlgDoModal;
	static PDoModal  m_pDlgDoModal;
	// Перехват AfxMessageBox
	typedef int (AFXAPI *PAfxMessageBoxT)(LPCTSTR,UINT,UINT);
	static int AFXAPI AfxMessageBoxT(LPCTSTR,UINT,UINT);
	// Адреса (реальные) этих функций
	static PAfxMessageBoxT m_pAfxMsgBoxT;

	static void Init();
};

#endif
