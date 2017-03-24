// SECToolBarMgr.h
#ifndef SECToolBarMgr_H
#define SECToolBarMgr_H

#include "hackvirtual.h"

// Класс обертка для управления тулбарами
class SECToolBarMgr : public SECToolBarManager
{
public:
	struct SCustomCmd{
		void Load();
		void Save();
		bool IsEmpty();
		void Empty()
		{
			modul=macros=descr="";
			Save();
		}
		void Copy(SCustomCmd* pCmd)
		{
			modul=pCmd->modul;
			macros=pCmd->macros;
			descr=pCmd->descr;
			IsScript=pCmd->IsScript;
			Save();
		}
		void Swap(SCustomCmd* pCmd)
		{
			CString temp;
			temp=modul;
			modul=pCmd->modul;
			pCmd->modul=temp;

			temp=macros;
			macros=pCmd->macros;
			pCmd->macros=temp;

			temp=descr;
			descr=pCmd->descr;
			pCmd->descr=temp;

			bool s=IsScript;
			IsScript=pCmd->IsScript;
			pCmd->IsScript=s;
			Save();
			pCmd->Save();
		}
		void Run();
		bool Select();
		CString descr;
		CString macros;
		CString modul;
		bool IsScript;
		UINT id;
	};

	struct SCustomCmdGroup
	{
		CString name;
		int count;
		SCustomCmd* pCmds;
		UINT* ids;
	};

	static CTrapSwap	m_TrapCmdPage;
	static CPtrArray	m_CustomCmd;
	static CMapPtrToPtr	m_mapIDs;
	static void Init();
	static void Done();

	static void LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count);
	static void MakeToolBar(CString strName,CString cmds,IDispatch* pDisp,LPCTSTR bmpRes,LPCTSTR modName);
	static SCustomCmd* GetCustomCmd(UINT id);
	static BOOL OnToolTipText(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);

	void OnInitCmdPage();

	// SECControlBarManager
	SECMDIFrameWnd* m_pFrame;

	// SECToolBarManager
	CDWordArray m_enabledList;			// List of windows which have been
											// disabled (EnableMainFrame)
	CPtrArray m_defaultBars;			// Array of default toolbars
	CPtrArray m_notifyWnds;				// Wnd's to pass toolbar notifications
	CWnd* m_pNoDropWnd;					// Wnd that won't accept dropped btns
	BOOL m_bMainFrameEnabled;			// TRUE if EnableMainFrame called
	BOOL m_bConfig;						// TRUE if in toolbar customize mode
	BOOL m_bToolTips;					// TRUE if tooltips enabled
	BOOL m_bFlyBy;						// TRUE if flyby help enabled
	BOOL m_bCoolLook;					// TRUE if "Cool" look enabled
	BOOL m_bLargeBmp;					// TRUE if using large bitmaps
	const void/*SECBtnMapEntry*/* m_pBtnMap;	// Maps ID's to button types.
	CObject* m_pObj;					// Aux version of button map.
	SECCustomToolBar* m_pConfigFocus;	// Toolbar with current config focus
	void/*SECCurrentCommand*/* m_pCmd;			// Holds status about current command
	UINT* m_lpMenuBarBtnIDs;			// IDs for default menu bar layout
	UINT m_nMenuBarBtnCount;			// No. of btns in m_lpMenuBarBtnIDs
	void* m_hz;
	CBitmap m_bmp1;						// Manages all our bitmap resources
	int m_count1;
	int m_resIDSmall;
	int m_resIDBig;
	UINT* m_cmdIDs;
	int m_height;
	int m_width;
	CDC m_DC1;
	CDC m_DC2;
	CDC m_DC3;
	CBrush m_brush;
	CBitmap	m_bmp2;
	int m_count2;
	CBitmap	m_bmp3;
	int m_count3;
};

#endif