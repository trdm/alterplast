// SelectFrm.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SelectDialog.h"
#include "SelectFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectFrm dialog


SelectFrm::SelectFrm(CWnd* pParent /*=NULL*/)
	: CDialog(SelectFrm::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectFrm)
	//}}AFX_DATA_INIT
}


void SelectFrm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectFrm)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_Wrd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectFrm, CDialog)
	//{{AFX_MSG_MAP(SelectFrm)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkListBox)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectFrm message handlers

CString SelectFrm::DoChoice(CStringArray *AllStrRef)
{

	for(int i = 0; i < AllStrRef->GetSize(); i++)
	{
		AllStrings.Add(AllStrRef->GetAt(i));
	}

	this->DoModal();
	return SelectedItem; 
	return "";
}

void SelectFrm::SortList(CString WrdBegin)
{
	int i;
	
	m_List.DeleteAllItems();

	if(WrdBegin == "")
	{
		for(i = 0; i < AllStrings.GetSize(); i++)
		{
			CString MethodName = AllStrings.GetAt(i).Mid(3);
		
			if(MethodName == "")continue;

			CString iconNumStr = AllStrings.GetAt(i).Left(2);
			if(!(FilterValue == "") && !(FilterValue == iconNumStr))continue;  
			m_List.InsertItem(0, MethodName, atoi(iconNumStr) - 1);
		}
	}
	else if(WrdBegin.Find(" ") == -1)
	{
		for(i = 0; i < AllStrings.GetSize(); i++)
		{
			CString MethodNameView = AllStrings.GetAt(i);
			MethodNameView.MakeUpper();
			if(MethodNameView.Find(WrdBegin) == 3)
			{
				CString MethodName = AllStrings.GetAt(i).Mid(3);
				CString iconNumStr = AllStrings.GetAt(i).Left(2);
				if(!(FilterValue == "") && !(FilterValue == iconNumStr))continue;
				m_List.InsertItem(0, MethodName, atoi(iconNumStr) - 1);
			}
		}
	}
	else
	{

		CString WrdMid = WrdBegin.Mid(WrdBegin.Find(" ") + 1);
		WrdBegin = WrdBegin.Left(WrdBegin.Find(" "));

		for(i = 0; i < AllStrings.GetSize(); i++)
		{
			CString MethodNameView = AllStrings.GetAt(i);
			MethodNameView.MakeUpper();

			if(MethodNameView.Find(WrdBegin) == 3 && MethodNameView.Find(WrdMid) >= WrdBegin.GetLength() + 3)
			{
				CString MethodName = AllStrings.GetAt(i).Mid(3);
				CString iconNumStr = AllStrings.GetAt(i).Left(2);
				if(!(FilterValue == "") && !(FilterValue == iconNumStr))continue;
				m_List.InsertItem(0, MethodName, atoi(iconNumStr) - 1);
			}
		}
	}

	if(m_List.GetItemCount() == 0)
	{
		m_Wrd.GetWindowText(SelectedItem);
		CDialog::OnOK();
	}
	
	m_List.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	SelectFrm::ShowSelDlg();

}


//При открытии формы
BOOL SelectFrm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitImagesList();
	
	if(ModuleDescription)
	{
		CPoint Pts = CWnd::GetCaretPos();
		
		CWnd* ConfModule = CWnd::FromHandle(HWND(ModuleDescription));
		ConfModule->ClientToScreen(&Pts);
	
		
		CRect WndRect, ConfClientRect;
		this->GetWindowRect(WndRect);
		this->GetParent()->GetClientRect(ConfClientRect);
		this->GetParent()->ClientToScreen(ConfClientRect);
		
		int WndHeight = WndRect.bottom - WndRect.top;
		int WndWidth = WndRect.right - WndRect.left;

		int PosX = Pts.x + 8;
		int PosY = Pts.y + 12;

		if(WndHeight > ConfClientRect.bottom - PosY)
		{
			ListBottom = - (PosY - 16);
		}
		else
		{
			ListBottom = PosY;
		}

		if(WndWidth > ConfClientRect.right - PosX)
		{
			ListRight = - (PosX);
		}
		else
		{
			ListRight = PosX;
		}

	}
	
	FilterValue = "";
	SortList("");

	m_Wrd.SetWindowText(InitialText);

	SelectFrm::ShowSelDlg(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void SelectFrm::OnChangeEdit1() 
{
	CString CurrWrd;
	m_Wrd.GetWindowText(CurrWrd);
	CurrWrd.MakeUpper();
	CurrWrd.TrimRight();
	SortList(CurrWrd);
}


BOOL SelectFrm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	if(pMsg->message == WM_KEYDOWN)
	{
		
        if(pMsg->wParam == VK_RETURN)
		{
			ExitDialog();
			return TRUE;       // запрет дальнейшей обработки
		}
		else if(pMsg->wParam == 0x00000026) // стрелка вверх
		{
			int SelPos = GetSelectedPos();
			if(SelPos > 0)
			{
				m_List.SetItemState(SelPos - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				if(SelPos == m_List.GetTopIndex())
				{
					CSize MoveSz;
					MoveSz.cy = - 12; MoveSz.cx = 0;
					BOOL m = m_List.Scroll(MoveSz);
				}

			}
			return TRUE;
		}
		else if(pMsg->wParam == 0x00000028) // стрелка вниз
		{
			int SelPos = GetSelectedPos();
			if(SelPos < m_List.GetItemCount() - 1)
			{
				m_List.SetItemState(SelPos + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				if(SelPos == m_List.GetTopIndex() + 7)
				{
					CSize MoveSz;
					MoveSz.cy = 12; MoveSz.cx = 0;
					BOOL m = m_List.Scroll(MoveSz);
				}
			}
			return TRUE;
		}
		else if(pMsg->wParam == 0x00000020) // пробел
		{
			if(::GetKeyState(VK_CONTROL) < 120)
			{
				if(FilterValue == "")
				{
					SetFilter();
				}
				else 
				{
					FilterValue = "";
					OnChangeEdit1();
				}

				return TRUE;
			}
		}


    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void SelectFrm::ShowSelDlg()
{

	int PosX, PosY;
	
	//Уменьшение размера списка
	int ListCnt = m_List.GetItemCount();
	
	CRect WndRect;
	this->GetWindowRect(WndRect);
	
	double Q = 6 - ListCnt;
	if(Q < - 7)Q = -7;

	int WndHeight = 12 + ListCnt * 20 + Q * 2.5;
	if(WndHeight > 162)WndHeight = 162;
	int WndWidth = WndRect.right - WndRect.left;
	
	PosX = ListRight;
	if((PosX = ListRight) < 0) PosX = - ListRight - WndWidth; 
	if((PosY = ListBottom) < 0) PosY = - ListBottom - WndHeight; 

	this->SetWindowPos(NULL, PosX, PosY, WndWidth, WndHeight, 0); 


}

void SelectFrm::InitImagesList()
{
	//Определим цвет заднего фона, чтобы фон был белым прокатывает
	// почему-то черный цвет
	COLORREF Pal = RGB(0, 0, 0);
	
	//Активизируем список и заполняем его иконками
	LstImages.Create(16, 16, ILC_COLOR, 0, 10);

	CBitmap Pic1;
	Pic1.LoadBitmap(IDB_BITMAP1);
	LstImages.Add(&Pic1, Pal);
	
	CBitmap Pic2;
	Pic2.LoadBitmap(IDB_BITMAP2);
	LstImages.Add(&Pic2, Pal);
	
	CBitmap Pic3;
	Pic3.LoadBitmap(IDB_BITMAP3);
	LstImages.Add(&Pic3, Pal);
	
	CBitmap Pic4;
	Pic4.LoadBitmap(IDB_BITMAP4);
	LstImages.Add(&Pic4, Pal);
	
	CBitmap Pic5;
	Pic5.LoadBitmap(IDB_BITMAP5);
	LstImages.Add(&Pic5, Pal);
	
	CBitmap Pic6;
	Pic6.LoadBitmap(IDB_BITMAP6);
	LstImages.Add(&Pic6, Pal);
	
	CBitmap Pic7;
	Pic7.LoadBitmap(IDB_BITMAP7);
	LstImages.Add(&Pic7, Pal);

	CBitmap Pic8;
	Pic8.LoadBitmap(IDB_BITMAP8);
	LstImages.Add(&Pic8, Pal);

	CBitmap Pic9;
	Pic9.LoadBitmap(IDB_BITMAP9);
	LstImages.Add(&Pic9, Pal);

	CBitmap Pic10;
	Pic10.LoadBitmap(IDB_BITMAP10);
	LstImages.Add(&Pic10, Pal);
	
	//Определяем список картинок для списка выбора
	m_List.SetImageList(&LstImages, LVSIL_SMALL);

	//Добавляем одну единственную колонку
	m_List.InsertColumn (0, "", LVCFMT_LEFT, 194);
	
	//В набор флагов добавляем флаг полного выделения строки
	DWORD ExStyle = m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT;
	m_List.SetExtendedStyle(ExStyle);
	

}

//Функция возвращает номер текущей выделенной строки списка
int SelectFrm::GetSelectedPos()
{
	POSITION posStruct = m_List.GetFirstSelectedItemPosition();
	return m_List.GetNextSelectedItem(posStruct);
}



void SelectFrm::OnDblclkListBox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ExitDialog();
	*pResult = -1;
}

void SelectFrm::ExitDialog()
{
	int num = GetSelectedPos();
	SelectedItem = m_List.GetItemText(num, 0);
	CDialog::OnOK();
}

void SelectFrm::SetFilter()
{
	int num = GetSelectedPos();
	SelectedItem = m_List.GetItemText(num, 0);

	for(int i = 0; i < AllStrings.GetSize(); i++)
	{
		if(SelectedItem == AllStrings.GetAt(i).Mid(3))
		{
			FilterValue = AllStrings.GetAt(i).Left(2);
			OnChangeEdit1();
			break;
		}
	}
}

void SelectFrm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	CString str;
	m_Wrd.GetWindowText(str);
	m_Wrd.SetSel(str.GetLength(), str.GetLength());
}
