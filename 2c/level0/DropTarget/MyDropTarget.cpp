// MyDropTarget.cpp: implementation of the CMyDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDropTarget.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//********************************************************************
// If you don't have a recent Platform SDK installed, you'll get linker
// errors on CLSID_DragDropHelper and IID_IDropTargetHelper, and you
// won't have the definition of IDropTargetHelper.  Uncomment the
// following stuff to fix that.

struct IDropTargetHelper : public IUnknown
{
    // IUnknown methods
    STDMETHOD (QueryInterface)(THIS_ REFIID riid, void **ppv) PURE;
    STDMETHOD_(ULONG, AddRef) ( THIS ) PURE;
    STDMETHOD_(ULONG, Release) ( THIS ) PURE;
	
    // IDropTargetHelper
    STDMETHOD (DragEnter)(THIS_ HWND hwndTarget, IDataObject* pDataObject,
		POINT* ppt, DWORD dwEffect) PURE;
    STDMETHOD (DragLeave)(THIS) PURE;
    STDMETHOD (DragOver)(THIS_ POINT* ppt, DWORD dwEffect) PURE;
    STDMETHOD (Drop)(THIS_ IDataObject* pDataObject, POINT* ppt,
		DWORD dwEffect) PURE;
    STDMETHOD (Show)(THIS_ BOOL fShow) PURE;
};

// {4657278A-411B-11d2-839A-00C04FD918D0}
extern "C" const GUID __declspec(selectany) CLSID_DragDropHelper = 
{ 0x4657278a, 0x411b, 0x11d2, { 0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0 }};

// {4657278B-411B-11d2-839A-00C04FD918D0}
extern "C" const GUID __declspec(selectany) IID_IDropTargetHelper = 
{ 0x4657278b, 0x411b, 0x11d2, { 0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0 }};

//********************************************************************


//////////////////////////////////////////////////////////////////////
// Construction/destruction

CMyDropTarget::CMyDropTarget  ( ) : 
m_piDropHelper(NULL),
m_bUseDnDHelper(false)
{
    // Create an instance of the shell DnD helper object.
	
    if ( SUCCEEDED( CoCreateInstance ( CLSID_DragDropHelper, NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IDropTargetHelper, 
		(void**) &m_piDropHelper ) ))
	{
        m_bUseDnDHelper = 0;//true;
	}
}

CMyDropTarget::~CMyDropTarget()
{
    if ( NULL != m_piDropHelper )
        m_piDropHelper->Release();
}


//////////////////////////////////////////////////////////////////////
// IDropTarget methods

DROPEFFECT CMyDropTarget::OnDragEnter ( CWnd* pWnd, COleDataObject* pDataObject,
									   DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;
	if (pDataObject->IsDataAvailable(CF_TEXT))			    // If Text Available
	{
		dwEffect = DROPEFFECT_COPY;
	}
	return dwEffect;
}

DROPEFFECT CMyDropTarget::OnDragOver ( CWnd* pWnd, COleDataObject* pDataObject,
									  DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;
	if (pDataObject->IsDataAvailable(CF_TEXT))			    // If Text Available
	{
         dwEffect = DROPEFFECT_COPY;
	}
    return dwEffect;
}

BOOL CMyDropTarget::OnDrop ( CWnd* pWnd, COleDataObject* pDataObject,
							DROPEFFECT dropEffect, CPoint point )
{
	BOOL bRet;
    bRet = DoDropText ( pDataObject, pWnd);
    return bRet;
}

void CMyDropTarget::OnDragLeave ( CWnd* pWnd )
{
    if ( m_bUseDnDHelper )
	{
        m_piDropHelper->DragLeave();
	}
}

BOOL CMyDropTarget::DoDropText(COleDataObject *pDataObject,CWnd* pWnd)
{
	HGLOBAL hData = pDataObject->GetGlobalData(CF_TEXT);
	if (hData == NULL)
		return FALSE;

	LPSTR pszText = (LPSTR) ::GlobalLock(hData);
	if (pszText == NULL)
		return FALSE;

	SendMessage(pWnd->m_hWnd,WM_SETTEXT,0,LPARAM(pszText));
	//pWnd->SetWindowText(pszText);

	::GlobalUnlock(hData);
	return TRUE;
}
