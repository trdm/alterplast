//
//	����� CFListCtrl		(���� FListCtrl.h)
//	������-������
//
//	������:		2.0
//	����:		12/04/2002
//	�����:		����� ����������
//	e-mail:		2Los@land.ru
//
//
//
//


#if !defined(AFX_FLISTCTRL_H__27FB9EBC_DCA4_4E86_847F_8DB878AA000C__INCLUDED_)
#define AFX_FLISTCTRL_H__27FB9EBC_DCA4_4E86_847F_8DB878AA000C__INCLUDED_

#if (_MSC_VER > 1000) && defined(HIDE)
#pragma once
#endif // _MSC_VER > 1000

#include "FListCtrl.h"		// � ����� ����� ������� ������ HIDE

////	���������������� ��������� ������
////		����� ���������� ������� ����������/������� � ���� ClassView
//#if !defined(HIDE)
//#define HIDE /##*
//#endif


#include <afxtempl.h>


//	��������� ������
typedef POSITION HFROW;		//��������� ������ ������-������

#define FL_ROOT		((HFROW) 0x00000001L)	//
#define FL_LAST		((HFROW) 0x00000002L)	//��������� ������
#define FL_FIRST	((HFROW) 0x00000003L)	//������ ������
#define FL_SORTPOS	((HFROW) 0x00000004L)	//

#define FL_UNDEFINED	-1

// ������������ ������	(������������ � InsertFColumn
#define FL_LEFT		LVCFMT_LEFT		
#define FL_RIGHT	LVCFMT_RIGHT
#define FL_CENTER	LVCFMT_CENTER

//	�������� ����������		(������� AddControl/GetControlType/SetControlType)
#define FL_BUTTON			2					//������
#define FL_CHECKBOX			4					//������
#define FL_EDIT				8					//���� �����
#define FL_COMBOBOX			(16|FL_BUTTON)		//���������� ������
	//�������������� ����� ��������� ����������
#define FL_SORT				32		//���������� ��������� ����������� ������
#define FL_AUTOCHECK		64		//�������������� ������������ ������
#define FL_PLUSBUTTON		128		//������ � ������������ +

//	��������� CHECKBOX'�	(������� GetFItemCheck/SetFItemCheck)
#define FL_UNCHECKED		0		//�� ����������
#define FL_CHECKED			1		//����������
#define FL_INDETERMINATE	2		//�������������� ���������



//	�����		(������� GetFStyle/SetFStyle)
#define FL_READONLY		1			//������ ������
#define FL_LINEAR_TREE	2			//���� ������ ������������� � �����


//	���������� ������
#define FL_NOCOLOR			(0x0100000)	//���� �� �����
#define FL_DEFCOLOR			(0x0200000)	//���� �� ���������
#define FL_DONTSET			(0x0300000)	//���� �� ���������

#define FL_BACKGROUND		101			// ���
#define FL_TEXT				102			//�����

//	�������, ��� ������� ��������������� ����
#define FL_FULL				128			//�� ���������
#define FL_FCOLUMN			256			//�������
#define FL_FROW				512			//������
#define FL_SELECTED_FROW	1024		//���������� ������
#define FL_SELECTED_FITEM	2048		//���������� �������


// �������			(������� GetFRect)
#define FLRC_FITEM				201		// �������
#define FLRC_FITEM_CLIENT		202
#define FLRC_FITEM_TREEBUTTON	203
#define FLRC_FITEM_CHECKBOX		204
#define FLRC_FITEM_IMAGE		205
#define FLRC_FITEM_COMBOBOX		206
#define FLRC_FITEM_BUTTON		207
#define FLRC_FITEM_TEXT			208
#define FLRC_FROW				209		//������
#define FLRC_FROW_TOP			210
#define FLRC_VISIBLE_FROWS		211
#define FLRC_FCOLUMN			212		//�������

//����������� ������ ������� ��� �������������� ����������� ������
#define FL_MIN_FCOLUMN_WIDTH	30



//	��������� ��� ������� GetNextFRow
#define FL_FIRST_CHILD		301
#define FL_LAST_CHILD		302
#define FL_NEXT				303
#define FL_NEXT_VISIBLE		304 
#define FL_PREV				305
#define FL_PREV_VISIBLE		306
#define FL_PARENT			307
#define FL_PARENT_VISIBLE	308
#define FL_PARENT_ROOT		309
#define FL_TOP				311
#define FL_BOTTOM			312


//	��������� ���������
	#define FLNM_USER_MSG(x)		(WM_USER+200+x)

//������������ �������� ����������� ���������
#define FL_OK			0	//	
#define FL_CANCEL		1	//
#define FL_CONTINUE		2	//���������� ��������������, ����������� ��������� � FL_OK ��� FL_CANCEL

#define FLNM_COLLAPSE		FLNM_USER_MSG(1)	//	������/������� ���� ������
#define FLNM_CHECK			FLNM_USER_MSG(2)	//	���������� ��������� ������
#define FLNM_DELETE			FLNM_USER_MSG(3)	//	�������� ������
#define FLNM_SELECT			FLNM_USER_MSG(4)	//	���������� ���������

#define FLNM_EDIT_BEGIN			FLNM_USER_MSG(5)	//	������������ �������� ��������������
#define FLNM_EDIT_END			FLNM_USER_MSG(6)	//	������������ �������� ��������������
#define FLNM_BUTTON_CLICK		FLNM_USER_MSG(7)	//	������������ ����� �� ������
#define FLNM_COMBOBOX_EXPAND	FLNM_USER_MSG(8)	//	������������ ��������� ���������� ������


//��������� � �����������, ������� ���������� ����������� �������
struct FL_NOTIFY{
	NMHDR hdr;
	HFROW	hFRow;				
	int		iFColumn;
	CString strText;
	DWORD	dwUserData;	
	BOOL	bCollapsed;
	int		nCheckState;
	int		iControlProfile;
};


// ������� ����������� ������
HIDE*/ struct FL_COMBOITEM{			
	CString strText;
	DWORD dwUserData;
};
typedef CArray<FL_COMBOITEM, FL_COMBOITEM> FL_COMBOITEMS;


//	����������� �������
class CFListCtrl;
typedef int (CompareFRows)(CFListCtrl* pThis, int iFColumn, HFROW hFRow1, HFROW hFRow2);


//
//		CFListCtrl										CFListCtrl
//					CFListCtrl				CFListCtrl
//								CFListCtrl
//

class CFListCtrl : public CListCtrl
{
public:

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
		{return CListCtrl::Create(dwStyle, rect, pParentWnd, nID);}

/**/		//
/**/		// �������� � ��������� ��������� ��������������
	
	//	��������� ����� ������� ��������� �������������� � ���������� ��� ������
	int AddControl(int nControlType=FL_EDIT);
	//	���������� ����� ��������
	int GetControlCount() {return m_pRefControls->GetSize();}
	//	������� ��� ������� (������ ����, ��. AttachControls)
	void RemoveAllControls();
	//	���������� ��� ��������� �������������� ��� ������� �������
	int GetControlType(int iControlProfile);
	//	������������� ��� ��������� �������������� ��� ������� �������
	void SetControlType(int iControlProfile, int nControlType);
	//	���������� �������� �������� checkbox ���������� �������
	void GetControlCheckImages(int iControlProfile, int *piChecked=NULL, int *piUnchecked=NULL, int *piIndeterminate=NULL);
	//	������������� �������� �������� checkbox ���������� �������
	void SetControlCheckImages(int iControlProfile, int iChecked=-1, int iUnchecked=-1, int iIndeterminate=-1);
	//	��������� ������� � ���������� ������ combobox'� ���  ���������� �������
	void AddControlComboItem(int iControlProfile, CString strText, DWORD dwUserData=0);
	//	���������� ������ ��������� ����������� ������ combobox'�
	FL_COMBOITEMS* GetControlCombo(int iControlProfile);
	//	���������� ���������� ������� ����������� ������ combobox'�
	FL_COMBOITEM GetControlComboSel(int iControlProfile);
	//	������������� ���������� ������� ����������� ������ combobox'�
	void SetControlComboSel(int iControlProfile, CString strText, DWORD dwUserData);
	//  ������������ � ��������� ������� flist'�
	void AttachControls(CFListCtrl* pFList){m_pRefControls=pFList->m_pRefControls;}
	//  ��������� ����� ��������
	void DetachControls(){m_pRefControls=&m_aControlProfiles;}
	
/**/		//													
/**/		// �������� � ��������� ������-�������			

	//	�������� ����� ������� � ������-������
	int InsertFColumn(int iIndex, CString strText, int nWidth, int nFormat=FL_LEFT, int iDefaultControlProfile=FL_UNDEFINED);
	//	������� �������
	void DeleteFColumn(int iIndex);
	//	���������� ����� ������� 
	int GetFColumnCount() {return m_aFColumns.GetSize();}
	//	���������� ������ ������� � �������
	int GetTree();
	//	������������� �������, � ������� ����� ������������ ������
	void SetTree(int iFColumn=-1);	
	//	���������� ��� �������
	CString GetFColumnCaption(int iFColumn);
	//	������������� ��� �������
	void SetFColumnCaption(int iFColumn, CString strText);
	//	���������� ������ �������
	int GetFColumnWidth(int iFColumn) {return GetColumnWidth(iFColumn);}
	//	������������� ������ �������
	void SetFColumnWidth(int iFColumn, int nWidth){SetColumnWidth(iFColumn, nWidth);}

/**/	//
/**/	// �������� �� �������� �������
	
	//	��������� ����� ������ � ������-������
	HFROW InsertFRow(HFROW hParent, HFROW hPosition, CString strText, int iImageIndex=-1, int iControlProfile=-1,BOOL bFast=0);
	//	������� ������
	void DeleteFRow(HFROW hFRow);	
	//	������� ��� ������
	void DeleteAllFRows()	{DeleteFRow(FL_ROOT);}
	//	���������� ����� �����
	int GetFRowCount(HFROW hParent=FL_ROOT);
	//	���������� ��������� ������ �� � �������
	HFROW FindIndex(HFROW hParent, int iIndex);
	//	���������� ����� ������ � ����� �� � ���������
	int GetFRowIndex(HFROW hFRow);
	//	���������� ��������� "���������" ������ 
	HFROW GetNextFRow(HFROW hFRow, int nfrCode=FL_NEXT);
	//	���������/���������� ���� ������
	void Collapse(HFROW hFRow, BOOL bCollapse=TRUE);
	//	���������� TRUE ���� ���� ������ ��� �� ����� �������� �����
	BOOL IsCollapsed(HFROW hFRow);	
	//	��������� ������
	void UpdateFRow(HFROW hFRow);
	//	������ ��� ������ �������
	void ExchangeFRows(HFROW hFRow1, HFROW hFRow2);
	//���������� ������
	void MoveFRows(HFROW hFRow, HFROW hFParent);
	//�������� ������� ���� ���������
	void SetAllDeep(HFROW hFRow1,int nDeep);
	//	���������� TRUE, ���� ��� ������ ���������� ����� bold (������ �����)
	BOOL IsFRowBold(HFROW hFRow);
	//	������������� ����� ������ bold ��� ��������� ������
	void SetFRowBold(HFROW hFRow, BOOL bBoldText=TRUE);
	//	���������� ������� ����������� ��� ������ ������� � �������
	int GetFRowDeep(HFROW hFRow);
	//	������������� ������� ��������� ���������� ��� ������� �������� ������
	void SetFRowControls(HFROW hFRow, int iControlProfile1, ...);
	//	����� ������
	HFROW FindFRow(HFROW hParent, HFROW hFirst, int iFColumn, CString text, BOOL bCompareNoCase=TRUE);
	//	���������� TRUE, ���� ��� ������������ ���� ��������
	BOOL IsFRowVisible(HFROW hFRow);

/**/	//
/**/	// �������� � ���������� �����
	
	//	���������� �����
	CString GetFItemText(HFROW hFRow, int iFColumn);
	//	������������� ����� 
	void SetFItemText(HFROW hFRow, int iFColumn, CString strText);	
	//	���������� ������ ��������
	int GetFItemImage(HFROW hFRow, int iFColumn);	
	//	������������� ��������
	void SetFItemImage(HFROW hFRow, int iFColumn, int iImageIndex);	
	//	���������� ���������������� ������
	DWORD GetFItemData(HFROW hFRow, int iFColumn);
	//	������������� ���������������� ������
	void SetFItemData(HFROW hFRow, int iFColumn, DWORD dwUserData);
	//	������������ ������� ���, ����� ��������� ������� ���� �������
	BOOL EnsureVisible(HFROW hFRow, int iFColumn);
	//	���������� ��������� checkbox'�
	int GetFItemCheck(HFROW hFRow, int iFColumn);
	//	������������� ��������� checkbox'a
	void SetFItemCheck(HFROW hFRow, int iFColumn, int nState=FL_CHECKED);	
	//	���������� ������ ������� ��������� �������������� ���������� �������� ������
	int GetFItemControl(HFROW hFRow, int iFColumn);
	//	������������� ������� ��������� �������������� ��� ���������� �������� ������
	void SetFItemControl(HFROW hFRow, int iFColumn, int iControlProfile);	

/**/	//
/**/	// ���������

	//	�������� ��������� ������/������� ������-������
	void Select(HFROW hFRow, int iFColumn=-1, BOOL bEnsureVisible=TRUE);
	//	����������� ��������� ���������� ������
	inline HFROW GetSelectedFRow()	{return GetSel().m_hSelectedFRow;}
	//	���������� ������ ���������� �������
	inline int GetSelectedFColumn() {return IsReadOnly() ? -1 : GetSel().m_iSelectedFColumn;}

/**/	//
/**/	// ��������

	//	���������� ����� ������-������
	int GetFStyle() {return GetOptions().m_nStyle;}
	//	������������ ����� ������-������
	void SetFStyle(int nDeleteStyle, int nAddStyle, int nMask=0);	
	//	������������� ����� readonly
	void SetReadOnly(BOOL bReadOnly=TRUE);
	//	���������� TRUE, ���� �������������� ���������
	BOOL IsReadOnly()	{return m_opt.m_nStyle&FL_READONLY;}
	//	���������� ������ ����������� ��� ������ ��������/������� ����
	void GetTreeButton(int *piExpandImage, int *piCollapseImage);
	//	������������� ����������� ��� ������� ��������/������� ����
	void SetTreeButton(int iExpandImage, int iCollapseImage);
	//	���������� ����� ����� ������ � ����� ������ �������	
	int GetTextLineCount()	{return m_opt.m_nTextLineCount;}
	//	������������� ����� ����� ������ � ����� ������ �������	
	void SetTextLineCount(int nTextLineCount);

/**/	//
/**/	// ������
	
	//	���������� ������������� ��������� �������
	BOOL GetFRect(HFROW hFRow, int iFColumn, int nArea, CRect &rect, BOOL bFItemOrigin=FALSE);
	//	���������� ���� ������/���� ��� ��������/�������/�������/����
	COLORREF GetColor(int nType=FL_TEXT, HFROW hFRow=NULL, int iFColumn=-1);
	//	�������������� ���� ��� ��������� �������
	void SetColor(int nArea, HFROW hFRow, int iFColumn, COLORREF crText=FL_DONTSET, COLORREF crBackground=FL_DONTSET);

	//	��������� ������ �� ������ � ��������� �������
	void Sort(int iFColumn, BOOL bReverseSort=TRUE);
	//	��������� ��������� ��� ��������� ������
	void Sort(int iFColumn, HFROW hParent, BOOL bSortChild=TRUE);	
	//	����������� �������
	CompareFRows *m_pCompareFRowsFunc;		//��������� �� ������� ��������� ����� �������		
	//	������������� ������ ������� ���, ����� ��� ������� � ���� ����� � ������ ������ �������
	void UpdateFColumnWidth(int iFColumn);
	//  
	void SetImageList(CImageList *pImageList){CListCtrl::SetImageList(pImageList, LVSIL_SMALL);}
	
	//����, �������� ����� ���������� ���������. �� ��������� ��� ������������ ����
	CWnd* m_pNotifyWnd;


	void SetRedraw(BOOL bRedraw=TRUE, BOOL bClear=FALSE);	
	BOOL IsRedraw();	

//				go high
//
//
//
//				RESTRICTED  AREA
//
//
//
//

//	IMPLEMENTATION
protected:
	int m_nRedrawCounter;

	HIDE*/ struct XFList_Color{	
		COLORREF crBackground;
		COLORREF crText;
	};

		//CONTROL PROFILE
	HIDE*/class XFList_ControlProfile{
	public:
		int m_nControlType;	
		struct XFList_CheckImages{
			int m_iChecked;
			int m_iUnchecked;
			int m_iIndeterminate;
		}m_checkImages;
			// COMBOBOX
		FL_COMBOITEM m_ciSelect;
		FL_COMBOITEMS m_aComboItems;	

		//
		XFList_ControlProfile()
		{
			m_nControlType=0;			
			m_checkImages.m_iChecked=-1;
			m_checkImages.m_iUnchecked=-1;
			m_checkImages.m_iIndeterminate=-1;			
			m_ciSelect.strText="";
			m_ciSelect.dwUserData=0;
		}
	};	
	HIDE*/ XFList_ControlProfile m_defaultControlProfile;
	HIDE*/ CArray<XFList_ControlProfile*, XFList_ControlProfile*> m_aControlProfiles;
	HIDE*/ CArray<XFList_ControlProfile*, XFList_ControlProfile*> *m_pRefControls;
	HIDE*/ CWnd* m_pInplaceControl;
	HIDE*/ HFROW m_hFRow;
	HIDE*/ int m_iFColumn;
	HIDE*/ UINT m_nChar;
	HIDE*/ XFList_ControlProfile* GetCP(int iControlProfile);		
	HIDE*/ XFList_ControlProfile* GetCP(HFROW hFRow, int iFColumn)
		{return GetCP(GetFItem(hFRow, iFColumn).m_iControlProfile);}	

protected:		//	IMPLEMENTATION
	
		//COLUMN
	HIDE*/ struct XFList_Column{	
		int m_iDefaultControlProfile;		//������� �� ���������		
		int m_nFormat;						//������������ ������
		BOOL m_bReverseSort;				//���� �������� ����������						
		XFList_Color m_color;
		int m_nTmpWidth;
	};
	HIDE*/ CArray<XFList_Column, XFList_Column> m_aFColumns;	
	HIDE*/ void InitFColumn(XFList_Column &col)
		{
			col.m_iDefaultControlProfile=FL_UNDEFINED;
			col.m_nFormat=DT_LEFT;
			col.m_bReverseSort=FALSE;					
			col.m_color.crText=FL_NOCOLOR;
			col.m_color.crBackground=FL_NOCOLOR;
			col.m_nTmpWidth=-1;
		}	
	HIDE*/ static int DefaultCompareFRows(CFListCtrl* pThis, int iFColumn, HFROW hFRow1, HFROW hFRow2)
		{
			return pThis->GetFItemText(hFRow1, iFColumn).CompareNoCase(
				pThis->GetFItemText(hFRow2, iFColumn));
		}
	HIDE*/ XFList_Column& GetFColumn(int iFColumn)
		{ 
			ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());
			return m_aFColumns[iFColumn];
		}


//
//					DEEPER
//

protected:		//	IMPLEMENTATION
																	//IMPLEMENTATION
		//ROW
	HIDE*/ class XFList_Row{
	public:
		struct XFList_Item{	
			CString m_strText;
			int		m_iImageIndex;
			int		m_iControlProfile;
			DWORD	m_dwUserData;
			//
			int		m_nCheckState;
		};	
		HIDE*/ static void InitFItem(XFList_Item &item)
		{
			item.m_dwUserData=0;
			item.m_iImageIndex=-1;
			item.m_iControlProfile=-1;			
			item.m_nCheckState=0;
		}

		HIDE*/ CArray<XFList_Item, XFList_Item> m_aFItems;		//�������� ������

		HIDE*/ CList<HFROW, HFROW>	m_lstChildFRows;			//�������� ������
		
		HIDE*/ BOOL m_bCollapsed;								//�������� ����� �������
		HIDE*/ int	m_nDeep;									//������� �����������
		HIDE*/ int m_iListIndex;								//������ � ������ ������� �����

		HIDE*/ BOOL m_bBoldText;								//������ �����
		XFList_Color m_color;

		HIDE*/ HFROW	m_hFRow;								//��������� � ������ m_lstFRows
		HIDE*/ HFROW	m_hParentFRow;							//������������ ���������
		HIDE*/ POSITION m_posParentList;						//������� � ������ ��������

//	IMPLEMENTATION

		HIDE*/ XFList_Row()
		{
			m_bCollapsed=TRUE;
			m_nDeep=0;
			m_iListIndex=FL_UNDEFINED;
			m_bBoldText=FALSE;
			m_hFRow=m_hParentFRow=m_posParentList=NULL;			
			m_color.crText=FL_NOCOLOR;
			m_color.crBackground=FL_NOCOLOR;
		}
		HIDE*/ ~XFList_Row(){}
		
		friend CFListCtrl;
	
	#ifdef _DEBUG
		void AssertValid() const
		{			
			if(m_bCollapsed!=FALSE && m_bCollapsed!=TRUE)
				return;
			ASSERT(m_bCollapsed==FALSE || m_bCollapsed==TRUE);
			ASSERT(m_nDeep>=0);
			ASSERT(m_iListIndex>=-1);
			m_aFItems.AssertValid();
			m_lstChildFRows.AssertValid();			
		}	
	#endif
	};
	friend XFList_Row;
	HIDE*/ CList<XFList_Row*, XFList_Row*> m_lstFRows;
	HIDE*/ XFList_Row m_rootFRow;
	HIDE*/ XFList_Row* GetFRow(HFROW hFRow)			
	{
		XFList_Row* pFRow = NULL;
		if(hFRow!=NULL)
		{
			pFRow= hFRow==FL_ROOT ? &m_rootFRow : m_lstFRows.GetAt(hFRow);
			ASSERT((pFRow->AssertValid(), TRUE));
		}
		return pFRow;
	}	

protected:		//	IMPLEMENTATION
	
	HIDE*/ void ShowFRow(HFROW hFRow, BOOL bShow=TRUE, BOOL bUpdateIndexes=TRUE);	
	HIDE*/ inline XFList_Row::XFList_Item& GetFItem(HFROW hFRow, int iFColumn)
		{			
		#ifdef _DEBUG
			static XFList_Row::XFList_Item s_debugFItem;			
			if(iFColumn>=0 && iFColumn<GetFColumnCount())
			{
				XFList_Row* pFRow=GetFRow(hFRow);
				if(iFColumn>=0 && iFColumn<pFRow->m_aFItems.GetSize())
					return pFRow->m_aFItems.ElementAt(iFColumn);
			}
			ASSERT(FALSE);
			return s_debugFItem;
		#else
			return GetFRow(hFRow)->m_aFItems.ElementAt(iFColumn);			
		#endif
		}	

//
//					MORE DEEPER
//

		//SELECTION
	HIDE*/ class XFList_Selection{
	public:				
		HFROW	m_hSelectedFRow;
		int m_iSelectedFColumn;
		XFList_Color m_colorFRow;
		XFList_Color m_colorFItem;
		//
		XFList_Selection()
		{
			m_hSelectedFRow=NULL;
			m_iSelectedFColumn=-1;
			//
			m_colorFRow.crText=FL_DEFCOLOR;
			m_colorFRow.crBackground=FL_DEFCOLOR;
			m_colorFItem.crText=FL_DEFCOLOR;
			m_colorFItem.crBackground=FL_DEFCOLOR;			
		}		
	}m_selection;
	friend XFList_Selection;
	HIDE*/ XFList_Selection &GetSel()
		{return m_selection;}

protected:		//	IMPLEMENTATION
																	//IMPLEMENTATION
		//DRAW SERVICE	
	HIDE*/ CDC m_dcFItem;
	HIDE*/ CBitmap m_bmpFItem;
	HIDE*/ CSize m_szFItem;	
	HIDE*/ int	m_nTextLineHeight;
	HIDE*/ int GetFRowHeight(BOOL bReal=TRUE)
	{
		if(bReal && GetItemCount()){
			CRect rc;
			GetItemRect(0, &rc, LVIR_BOUNDS);
			return rc.Height();
		}
		return m_nTextLineHeight*GetOptions().m_nTextLineCount+4;
	}

	HIDE*/ void UpdateMemDC(CDC *pDC, CRect rcFItem);
	HIDE*/ void DrawFItem(CDC* pDC, HFROW hFRow, int iFColumn);	
	HIDE*/ BOOL DrawElement(int nType, CDC *pDC, HFROW hFRow, int iFColumn, CRect *prcElement);		
	HIDE*/ void DrawItem(LPDRAWITEMSTRUCT lpDIS);		
																	//IMPLEMENTATION
		//OPTIONS
	HIDE*/ struct XFList_Options{
		int m_iExpandButton;  // [+]
		int m_iCollapseButton;// [-]
		int m_nStyle;				
		int m_nTextLineCount;
		int m_iTreeColumn;
	};
	HIDE*/ XFList_Options m_opt;	
	HIDE*/ const XFList_Options &GetOptions() {return m_opt;}	

//CONSTRUCTION
public:
	CFListCtrl();
	~CFListCtrl();	

//
//
//						VERY DANGEROUS
//
//	
//====================================================================================
//						IMPLEMENTATION
protected:				

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFListCtrl)	
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	
protected:		//	IMPLEMENTATION

#ifdef _DEBUG
	HIDE*/ virtual void AssertValid() const
	{		
		CListCtrl::AssertValid();		
	}
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();		
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);	
	afx_msg void OnSysColorChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);		
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDividerDblClk(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnFontChange();
	//}}AFX_MSG
	afx_msg void OnInplaceControlNotify(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg LRESULT	OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
	
	HIDE*/ void TranslateMouse(UINT nFlags, CPoint point, BOOL bDoubleClick);	
	HIDE*/ BOOL HitTestMouseClick(CPoint point, HFROW &hFRow, int &iFColumn);

		// NOTIFY
	HIDE*/ void NotifyInit(FL_NOTIFY &notify, HFROW hFRow, int iFColumn, int code);
	HIDE*/ LRESULT NotifySend(FL_NOTIFY &notify);	

	virtual void OnEvent(FL_NOTIFY &notify, HFROW hFRow, int iFColumn, int code){};

protected:	//	IMPLEMENTATION

		// EDIT 	
	HIDE*/struct FL_INPLACE_NOTIFY{
		NMHDR hdr;
		CString strText;
		DWORD	dwUserData;
		HFROW	hFRow;
		int		iFColumn;
		UINT	nChar;
	};
	HIDE*/ LRESULT InplaceControl(UINT code, UINT nChar, FL_INPLACE_NOTIFY* pInplaceNotify=NULL);
		
	//
	friend class CFList_Edit;
	friend class CFList_ComboBox;
};
typedef CFListCtrl flist;

/////////////////////////////////////////////////////////////////////////////
//
//	CFList_Edit
//
HIDE*/class CFList_Edit : public CEdit
{
	UINT m_nChar;
	HFROW m_hFRow;
	int m_iFColumn;

// Construction
public:	
	CFList_Edit(HFROW hFRow, int iFColumn);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFList_Edit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFList_Edit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFList_Edit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//
//		CFList_ComboBox
//
HIDE*/ class CFList_ComboBox : public CComboBox
{	
	FL_COMBOITEMS* m_paComboItems;
	FL_COMBOITEM m_ciSelect;	
	HFROW m_hFRow;
	int m_iFColumn;
	UINT	m_nChar;
	CFListCtrl* m_pParent;
// Construction
public:	
	CFList_ComboBox(CFListCtrl* pParent, HFROW hFRow, int iFColumn, FL_COMBOITEMS* paComboItems, FL_COMBOITEM ciSelect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFList_ComboBox)
	public:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFList_ComboBox();
	// Generated message map functions
protected:
	//{{AFX_MSG(CFList_ComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus();//CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnCloseup();
	afx_msg void OnEditChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#elif !defined(HIDE)
//		���� ������ �������� ����������/�������/����� � ���� ClassView
	#define HIDE /##*

//
#endif // !defined(AFX_FLISTCTRL_H__27FB9EBC_DCA4_4E86_847F_8DB878AA000C__INCLUDED_)
