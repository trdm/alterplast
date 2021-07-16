#if !defined(AFX_LAYERSECLECT_H__0D292451_9CD7_4309_8F16_0A7ABE186E2A__INCLUDED_)
#define AFX_LAYERSECLECT_H__0D292451_9CD7_4309_8F16_0A7ABE186E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerSeclect.h : header file
//
#include "../GridCtrl_src/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CLayerSelect dialog
struct CLayerItem
{
public:
	CLayerItem()
	{
		bVisible=0;
	};
	CString csName;
	BOOL bVisible;
};

class CLayerSelect : public CDialog
{
// Construction
public:
	CLayerSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayerSelect)
	enum { IDD = IDD_LAYERSELECT_FORM };
	CButton	m_Help;
	CButton	m_Cancel;
	//}}AFX_DATA


	class CFormEditor *pEditor;//обратная связь с редактором форм для выдачи сообщений по изменению состава слоев


	int nCurrentEdit;
	CArray <CLayerItem,CLayerItem&> aList;


	CToolBar m_ToolBar;
	CGridCtrl m_Grid;


	CString GetStrLayers();
	BOOL IsVisibleLayer(CString csName);
	void AttachEditor(class CFormEditor *);
	void AddLayer(CString csName,BOOL bVisible,BOOL bActivate);
	CString GetCurrentName();
	void ReLoadGrid();


	void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLayerSelect)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnNewLayer();
	afx_msg void OnEditLayer();
	afx_msg void OnDelLayer();
	afx_msg void OnUpLayer();
	afx_msg void OnDownLayer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERSECLECT_H__0D292451_9CD7_4309_8F16_0A7ABE186E2A__INCLUDED_)
