// amsWnd.h : header file for the CAMSWnd class
// Created by: Alvaro Mendez - 04/09/2002

#ifndef __AMS_WND_H__
#define __AMS_WND_H__

#include <afxtempl.h>


///////////////////////////////////////////////////////////////////////////////
// Window namespace class

// Class CAMSWnd wraps several static functions and nested classes that work with CWnd objects.
//
class CAMSWnd
{
public:
	enum Flags	// Bit values passed to the uFlags parameter of the functions below.
	{ 
		Left			= 0x0001,	// left coordinate
		Right			= 0x0002,	// right coordinate
		Top				= 0x0004,	// top coordinate
		Bottom			= 0x0008,	// bottom coordinate
		X 				= 0x0003,	// horizontal coordinates (left and right)
		Y 				= 0x000C,	// vertical coordinates (top and bottom)
		Position 		= 0x000F,	// both the horizontal and vertical coordinates
		Width 			= 0x0010,	// the window's width 
		Height			= 0x0020,	// the window's height
		Size			= 0x0030,	// the window's width and height
		Both			= 0x003F,	// the window's position and size
		State			= 0x0040,	// the window's state (minimized, maximized, etc.)
		All				= 0x007F,	// the window's position, size, and state		
		NoRedraw		= 0x1000	// the window is not repainted
	};

	static void Save(CWnd* pWnd, LPCTSTR szWindowName = NULL);
	static void Restore(CWnd* pWnd, LPCTSTR szWindowName = NULL, unsigned uFlags = Position);
	
	static void ChangeBy(CWnd* pWnd, int nPixels, unsigned uFlags);
	static void AlignControl(CWnd* pWnd, unsigned uCtrlToAlign, unsigned uCtrlToAlignAgainst, unsigned uFlags, int nOffset = 0);
	static void OrganizeSequentialControls(CWnd* pWnd, unsigned uFirstCtrl, unsigned uLastCtrl, int nDistance, unsigned uFlags = CAMSWnd::Y);

	static int	GetTextExtent(CWnd* pWnd, const CString& strText, CFont* pFont = NULL);


	// Class FocusHolder allows you to save a window object in construction
	// whose focus will be set on destruction.
	//
	class FocusHolder
	{
	public:
		FocusHolder(CWnd* pWnd);
		FocusHolder(CWnd& wnd);
		~FocusHolder();

	private:
		CWnd* m_pWnd;
	};


	// Class Disabler allows you to disable a window in construction and reenable it on destruction.
	//
	class Disabler
	{
	public:
		Disabler(CWnd* pWnd);
		Disabler(CWnd& wnd);
		~Disabler();

	private:
		CWnd* m_pWnd;
	};


	// Class Hook is an abstract class used for intercepting a given window's messages 
	// before they're processed.
	//
	class Hook
	{
	public:
		Hook(CWnd* pWnd = NULL);
		virtual ~Hook();
		
		virtual BOOL Open(CWnd* pWnd);
		virtual void Close();

		BOOL IsOpen() const;
		CWnd* GetWindow() const;
		HWND GetHwnd() const;
		
		static LRESULT CALLBACK HookedWindowProc(HWND, UINT, WPARAM, LPARAM);

	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;
		LRESULT Default();			// call this at the end of handler fns
				
	private:
		HWND m_hWnd;				// the window being hooked
		WNDPROC	m_pWndProcOld;		// the original window proc

	private:
		static CMap<HWND, HWND&, Hook*, Hook*&> m_mapHookedWindows;  // map of windows to hooks
	};


	// Class PlacementHook is used to easily save and restore given window's position and size.
	// Simply derive from it and call one of the Set functions inside OnCreate or OnInitDialog.
	//
	class PlacementHook : private Hook
	{
	public:
		PlacementHook();

		void SetLastPositionAndSize(CWnd* pWnd, LPCTSTR szWindowName = NULL);
		void SetLastPosition(CWnd* pWnd, LPCTSTR szWindowName = NULL);
		void SetLastSize(CWnd* pWnd, LPCTSTR szWindowName = NULL);

	protected:
		void Open(CWnd* pWnd, LPCTSTR szWindowName);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		CString m_strWindowName;	// the name of the window (to be saved in the registry)
		unsigned m_uFlags;			// how the window should be restored
	};
};

#endif	// __AMS_WND_H__
