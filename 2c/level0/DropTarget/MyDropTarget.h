//////////////////////////////////////////////////////////////////////
//
// MyDropTarget.h: interface for the CMyDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_)
#define AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
// CMyDropTarget implements a drop target for the MultiFiler dialog.
// I was also messing around with the IDropTargetHelper stuff in 
// Win2K, which lets the shell draw the nifty shaded drag image when
// you drag into the dialog.  If you're using 9x or NT 4, that stuff
// is disabled.
 
struct IDropTargetHelper;   // forward reference, in case the latest PSDK isn't installed.

class CMyDropTarget : public COleDropTarget  
{
public:
	DROPEFFECT OnDragEnter ( CWnd* pWnd, COleDataObject* pDataObject,
		                     DWORD dwKeyState, CPoint point );

	DROPEFFECT OnDragOver ( CWnd* pWnd, COleDataObject* pDataObject,
		                    DWORD dwKeyState, CPoint point );

	BOOL OnDrop ( CWnd* pWnd, COleDataObject* pDataObject,
		          DROPEFFECT dropEffect, CPoint point );

	void OnDragLeave ( CWnd* pWnd );

    CMyDropTarget ();
	virtual ~CMyDropTarget();

protected:
    IDropTargetHelper* m_piDropHelper;
    bool               m_bUseDnDHelper;

    BOOL DoDropText ( COleDataObject* pDataObject,CWnd* pWnd);
};

#endif // !defined(AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_)
