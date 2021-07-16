// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// CValueGrid.cpp: implementation of the CValueGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueGrid.h"
#include "ValueArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CValueGrid::Methods;

#define pGrid ((CGridCtrl *)pWnd)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueGrid,CValue);

CValueGrid::CValueGrid()
{
	pWnd=0;
	nType=100;
	nCurRow=-1;
}

CValueGrid::~CValueGrid()
{

}

DWORD GetFormatValue(CString Str)
{
	DWORD nFormat=0;
	Str.MakeUpper();
	Str.Replace(" ","");
	Str.Replace(",","|");
	Str.Replace(";","|");
	Str.Replace("+","|");
	Str=CString("|")+Str+"|";

#define ADD_FORMAT(x,y)\
if(Str.Find(CString("|")+CString(x).Mid(3)+"|")>=0)\
nFormat=nFormat|##y;\
else if(Str.Find(CString("|")+CString(x)+"|")>=0)\
nFormat=nFormat|##y;

	ADD_FORMAT("DT_BOTTOM",DT_BOTTOM);
	ADD_FORMAT("DT_CALCRECT",DT_CALCRECT);
	ADD_FORMAT("DT_CENTER",DT_CENTER);
	ADD_FORMAT("DT_END_ELLIPSIS",DT_END_ELLIPSIS);
	ADD_FORMAT("DT_PATH_ELLIPSIS",DT_PATH_ELLIPSIS);
	ADD_FORMAT("DT_END_ELLIPSIS",DT_END_ELLIPSIS);
	ADD_FORMAT("DT_EXPANDTABS",DT_EXPANDTABS);
	ADD_FORMAT("DT_EXTERNALLEADING",DT_EXTERNALLEADING);
	ADD_FORMAT("DT_LEFT",DT_LEFT);
	ADD_FORMAT("DT_MODIFYSTRING",DT_MODIFYSTRING);
	ADD_FORMAT("DT_NOCLIP",DT_NOCLIP);
	ADD_FORMAT("DT_NOPREFIX",DT_NOPREFIX);
	ADD_FORMAT("DT_RIGHT",DT_RIGHT);
	ADD_FORMAT("DT_SINGLELINE",DT_SINGLELINE);
	ADD_FORMAT("DT_TABSTOP",DT_TABSTOP);
	ADD_FORMAT("DT_TOP",DT_TOP);
	ADD_FORMAT("DT_VCENTER",DT_VCENTER);
	ADD_FORMAT("DT_WORDBREAK",DT_WORDBREAK);

	return nFormat;
}

//******************************
//������ ��� ����������� �������
//******************************
enum
{
	enSetEditable,
	enGetEditable,
	enGetRowCount,
	enGetColumnCount,
	enGetFixedRowCount,
	enGetFixedColumnCount,
	enSetRowCount,
	enSetColumnCount,
	enSetFixedRowCount,
	enSetFixedColumnCount,
	enSetItemText,
	enGetItemText,
	enSetItemData,
	enGetItemData,
	enSetItemImage,
	enGetItemImage,
	enInsertColumn,
	enInsertRow,
	enDeleteColumn,
	enDeleteRow,
	enDeleteNonFixedRows,
	enDeleteAllItems,
	enAutoSizeColumns,
	enAutoSizeRows,
	enSetColumnWidth,
	enGetColumnWidth,
	enGetRowHeight,
	enSetRowHeight,
	enSetHeaderSort,

	enCurrentRow,
	enCurrentColumn,
	enRow,
	enColumn,
	enRedrawWindow,
	enInsertItem,
	enDeleteItem,
	enPrint,
	enGetSelectedCount,
	enEnableSelection,
	enIsCellSelected,
	enLoadImage,
	enSetItemFgColour,
	enSetItemBkColour,
	enVirtualMode,
	enValue,
	enSetDefCellHeight,
	enSetDefCellWidth,
	enSetItemFormat,
	enSetDefItemFormat,
	enCurrentCell,
	enGetItem,
	enEditText,
	enSelectString,
	enSort,
	enRedrawWindow2,
	enEditable,
	enRowCount,
	enColumnCount,
	enFixedRowCount,
	enFixedColumnCount,
	enItemText,
	enItemData,
	enItemImage,
	enColumnWidth,
	enRowHeight,
	enFindValue,
	enFindText,
	enFocus,
	enFormat,
	enFastFind,
	enView,
	enColumnIdentifier,
	enColumnType,
	enCellType,
	enSelectRows,
	enGetRow,
	enCurrentRowNumber,
	enCurrentRowNumber2,
	enGetAttribute,
	enSetAttribute,
	enColumnFormule,
	enColumnTitle,
	enColumnVisible,
	enColumnEnable,
	enColumnTab,
	enDrawRow,
	enDrawCol,
	enEditValue,
	enModified,
	enLoad,
	enLength,
	enPrec,
	enSetControlRect,
	enGetControlRect,
	enSetCoveredCells,
	enSetUnCoveredCells,
};
void CValueGrid::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"SetEditable","������������������������"},//enSetEditable  (1)
		{"GetEditable","����������������������"},
		{"GetRowCount","�����������������������"},//enGetRowCount  (3)
		{"GetColumnCount","�������������������������"},//enGetColumnCount  (4)
		{"GetFixedRowCount","���������������������������"},//enGetFixedRowCount  (5)
		{"GetFixedColumnCount","�����������������������������"},//enGetFixedColumnCount  (6)
		{"SetRowCount","�������������������������"},//enSetRowCount  (7)
		{"SetColumnCount","���������������������������"},//enSetColumnCount  (8)
		{"SetFixedRowCount","�����������������������������"},//enSetFixedRowCount  (9)
		{"SetFixedColumnCount","�������������������������������"},//enSetFixedColumnCount  (10)
		{"SetItemText","�����������"},//enSetItemText  (11)
		{"GetItemText","�������������"},//enGetItemText  (12)
		{"SetItemData","������������"},//enSetItemData  (13)
		{"GetItemData","��������������"},//enGetItemData  (14)
		{"SetItemImage","��������������"},//enSetItemImage  (15)
		{"GetItemImage","����������������"},//enGetItemImage  (16)
		{"InsertColumn","���������������"},//enInsertColumn  (17)
		{"InsertRow","��������������"},//enInsertRow  (18)
		{"DeleteColumn","��������������"},//enDeleteColumn  (19)
		{"DeleteRow","�������������"},//enDeleteRow  (20)
		{"DeleteNonFixedRows","����������������������������"},//enDeleteNonFixedRows  (21)
		{"DeleteAllItems","����������"},//enDeleteAllItems  (22)
		{"AutoSizeColumns","�������������������������"},//enAutoSizeColumns  (23)
		{"AutoSizeRows","�����������������������"},//enAutoSizeRows  (24)
		{"SetColumnWidth","�����������������������"},//enSetColumnWidth  (25)
		{"GetColumnWidth","���������������������"},//enGetColumnWidth  (26)
		{"GetRowHeight","��������������������"},
		{"SetRowHeight","����������������������"},//enSetRowHeight  (28)
		{"SetHeaderSort","���������������������������"},//enSetHeaderSort  (29)
		{"CurrentRow","�������������"},//enCurrentRow  (30)
		{"CurrentColumn","��������������"},//enCurrentColumn  (31)
		{"Row","������"},//enRow  (32)
		{"Column","�����"},//enColumn  (33)
		{"RedrawWindow","����������������"},//enRedrawWindow  (35)
		{"InsertItem","���������������"},//enInsertItem  (36)
		{"DeleteItem","��������������"},//enDeleteItem  (37)
		{"Print","����������"},//enPrint  (39)
		{"GetSelectedCount","���������������������������������"},//enGetSelectedCount  (40)
		{"EnableSelection","��������������������������"},//enEnableSelection  (41)
		{"IsCellSelected","��������������"},//enIsCellSelected  (42)
		{"LoadImage","�����������������"},//enLoadImage  (43)
		{"FgColour","����������"},//enSetItemFgColour  (46)
		{"BkColour","��������"},//enSetItemBkColour  (47)
		{"VirtualMode","����������������"},//enVirtualMode  (48)
		{"Value","��������"},//enValue  (52)
		{"SetDefCellHeight","���������������������������������"},//enSetDefCellHeight  (53)
		{"SetDefCellWidth","����������������������������������"},//enSetDefCellWidth  (54)
		{"SetItemFormat","����������������������"},//enSetItemFormat  (55)
		{"SetDefItemFormat","���������������������������������"},//enSetDefItemFormat  (56)
		{"CurrentCell","�������������"},//enCurrentCell  (57)
		{"GetItem","���������������"},//enGetItem  (59)
		{"Activate","��������������"},//enEditText  (60)
		{"SelectString","��������������"},//enSelectString  (62)
		{"Sort","�����������"},//enSort  (63)
		{"Refresh","��������"},//enRedrawWindow2
		{"Editable","��������������"},//enEditable  (65)
		{"RowCount","���������������"},
		{"ColumnCount","�����������������"},//enColumnCount  (67)
		{"FixedRowCount","�������������������"},
		{"FixedColumnCount","���������������������"},
		{"Text","�����"},//
		{"Data","������"},//
		{"Image","��������"},//
		{"ColumnWidth","�������������"},//enColumnWidth  (73)
		{"RowHeight","������������"},//enRowHeight  (74)
		{"FindValue","�������������"},//enFindValue  (75)
		{"FindText","����������"},//enFindText  (76)
		{"Focus","�����"},//enFocus  (77)
		{"Format","������"},//enFormat  (78)
		{"FastFind","������������"},//enFastFind  (79)
		{"View","���������"},//enView  (80)	
		{"ColumnIdentifier","��������������������"},//enColumnIdentifier
		{"ColumnType","����������"},
		{"CellType","���������"},
		{"SelectRows","�������������"},
		{"GetRow","��������������"},
		{"CurrentRowNumber","������������������"},
		{"GetRowByNumber","����������������������"},
		{"GetAttribut","���������������"},
		{"SetAttribut","�����������������"},
		{"ColumnFormule","��������������"},
		{"ColumnTitle","����������������"},
		{"ColumnVisible","����������������"},
		{"ColumnEnable","������������������"},
		{"ColumnTab","��������������"},
		{"DrawRow","�����������������"},
		{"DrawCol","������������������"},
		{"EditValue","���������������������"},//enEditValue
		{"Modified","������������������"},//enModified
		{"Load","���������"},//enLoad
		{"ColumnLength","������������"},//enLength
		{"ColumnPrecision","���������������"},//enPrec
		{"SetControlRect","��������������������"},
		{"GetControlRect","������������������"},
		{"SetCoveredCells","����������������"}, //enSetCoveredCells,
		{"SetUncoveredCells","�����������������"}, //enSetUncoveredCells,
		
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}


CValue CValueGrid::Method(int iName,CValue **p)
{
	if(!pGrid)
		Error("������ �� ���������������!");



#define SET_NEW_CASE(x,nMinParam)\
		case en##x:\
			{\
				if(p[nMinParam]->nType!=0)\
					iName=enSet##x;\
				else\
					iName=enGet##x;\
				break;\
			}

	switch(iName)
	{  
		SET_NEW_CASE(Editable,0)
		SET_NEW_CASE(RowCount,0)
		SET_NEW_CASE(ColumnCount,0)
		SET_NEW_CASE(FixedRowCount,0)
		SET_NEW_CASE(FixedColumnCount,0)

		SET_NEW_CASE(ColumnWidth,1)
		SET_NEW_CASE(RowHeight,1)

		SET_NEW_CASE(ItemText,2)
		SET_NEW_CASE(ItemData,2)
		SET_NEW_CASE(ItemImage,2)
	}


	CValue Ret;
	switch(iName)
	{
	case enSetEditable:
		{
			if(pGrid)
			{
				pGrid->SetEditable(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enGetEditable:
		{
			if(pGrid)
			{
				return (pGrid->IsEditable());
			}
			break;
			
		}
	case enGetRowCount:
		{
			if(pGrid)
			{
				return (pGrid->GetRowCount());
			}
			break;
			
		}
	case enGetColumnCount:
		{
			if(pGrid)
			{
				return (pGrid->GetColumnCount());
			}
			break;
			
		}
	case enGetFixedRowCount:
		{
			if(pGrid)
			{
				return (pGrid->GetFixedRowCount());
			}
			break;
			
		}
	case enGetFixedColumnCount:
		{
			if(pGrid)
			{
				return (pGrid->GetFixedColumnCount());
			}
			break;
			
		}
	case enSetRowCount:
		{
			if(pGrid)
			{
				int nCount=p[0]->GetNumber();
				if(nCount<0)
					Error("���������� ����� <0");
				pGrid->SetRowCount(nCount);
				return (1);
			}
			break;
			
		}
	case enSetColumnCount:
		{
			if(pGrid)
			{
				pGrid->SetColumnCount(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enSetFixedRowCount:
		{
			if(pGrid)
			{
				pGrid->SetFixedRowCount(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enSetFixedColumnCount:
		{
			if(pGrid)
			{
				pGrid->SetFixedColumnCount(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enSetItemText:
		{
			if(pGrid)
			{
					pGrid->SetItemText(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetString());
					return (1);
			}
			break;
			
		}
	case enGetItemText:
		{
			if(pGrid)
			{
				Ret=String(pGrid->GetItemText(p[0]->GetNumber(),p[1]->GetNumber()));
			}
			break;
			
		}
	case enSetItemData:
		{
			if(pGrid)
			{
				pGrid->SetItemData(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enGetItemData:
		{
			if(pGrid)
			{
				return (pGrid->GetItemData(p[0]->GetNumber(),p[1]->GetNumber()));
			}
			break;
			
		}
	case enSetItemImage:
		{
			if(pGrid)
			{
					pGrid->SetItemImage(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetNumber());
					return (1);
			}
			break;
			
		}
	case enGetItemImage:
		{
			if(pGrid)
			{
				return (pGrid->GetItemImage(p[0]->GetNumber(),p[1]->GetNumber()));
			}
			break;
			
		}
		
		
		
		
		
	case enInsertColumn:
		{
			if(pGrid)
			{
				int nDefualt=DT_CENTER|DT_VCENTER|DT_SINGLELINE;
				CGridCellBase* cDefaultCell;
				cDefaultCell=pGrid->GetDefaultCell(false,false);
				if(cDefaultCell)
					nDefualt=cDefaultCell->GetFormat();
				
				pGrid->InsertColumn(p[0]->GetString(),nDefualt,p[1]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enInsertRow:
		{
			if(pGrid)
			{
				pGrid->InsertRow(p[0]->GetString(),p[1]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enDeleteColumn:
		{
			if(pGrid)
			{
				pGrid->DeleteColumn(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enDeleteRow:
		{
			if(pGrid)
			{
				pGrid->DeleteRow(p[0]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enDeleteNonFixedRows:
		{
			if(pGrid)
			{
				pGrid->DeleteNonFixedRows();
				return (1);
			}
			break;
			
		}
	case enDeleteAllItems:
		{
			if(pGrid)
			{
				pGrid->DeleteAllItems();
				return (1);
			}
			break;
			
		}
	case enAutoSizeColumns:
		{
			if(pGrid)
			{
				pGrid->AutoSizeColumns();
				return (1);
			}
			break;
			
		}
	case enAutoSizeRows:
		{
			if(pGrid)
			{
				pGrid->AutoSizeRows();
				return (1);
			}
			break;
			
		}
	case enSetColumnWidth:
		{
			if(pGrid)
			{
				pGrid->SetColumnWidth(p[0]->GetNumber(),p[1]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enGetColumnWidth:
		{
			if(pGrid)
			{
				return (pGrid->GetColumnWidth(p[0]->GetNumber()));
			}
			break;
			
		}
	case enGetRowHeight:
		{
			if(pGrid)
			{
				return (pGrid->GetRowHeight(p[0]->GetNumber()));
			}
			break;
			
		}
	case enSetRowHeight:
		{
			if(pGrid)
			{
				pGrid->SetRowHeight(p[0]->GetNumber(),p[1]->GetNumber());
				return (1);
			}
			break;
			
		}
	case enSetHeaderSort:
		{
			if(pGrid)
				{
					int n=p[0]->GetNumber();
					if(n==1)
						pGrid->SetCompareFunction(CGridCtrl::pfnCellTextCompare);//��������� �����
					else if(n==2)
						pGrid->SetCompareFunction(CGridCtrl::pfnCellNumericCompare);//��������� �����
					
					pGrid->SetHeaderSort(n);
					return (1);
				}
				break;
				
		}
		
	case enSort:
		{
			if(pGrid)
				{
					int column=p[0]->GetNumber();
					int bIncrese=1-((BOOL)p[1]->GetNumber());
					int mode=p[2]->GetNumber();
					int n1=p[3]->GetNumber();
					int n2=p[4]->GetNumber();
					if(n2==0)
						n2=-1;
					if(mode)
						pGrid->SortItems(CGridCtrl::pfnCellTextCompare, column, bIncrese, 0, n1, n2);
					else
						pGrid->SortItems(CGridCtrl::pfnCellNumericCompare, column, bIncrese, 0, n1, n2);
					
					
					return (1);
				}
				break;
				
		}
		
	case enView:
		{
			if(pGrid)
			{
				HWND newTable;
				newTable=pGrid->m_hWnd;
			
				int nVisible;
				if(p[0]->GetNumber())
					nVisible=SW_NORMAL;
				else
					nVisible=SW_HIDE;
				pGrid->ShowWindow(nVisible);
				return (1);
			}
			break;
			
		}
		
	case enCurrentCell:
		{
			if(pGrid)
			{
				int i=p[0]->GetNumber();
				int j=p[1]->GetNumber();
				pGrid->SetCurrentCell(i,j);
			}
			break;
		}
		
	case enCurrentRow:
		{
			if(pGrid)
			{
				CCellID curCell=pGrid->GetFocusCell();
				if(p[0]->nType!=0)
				{
					pGrid->SetCurrentCell(p[0]->GetNumber(),curCell.col);
				}
				if(curCell.row>=pGrid->GetRowCount())
					curCell.row=pGrid->GetRowCount()-1;

				return (curCell.row);
			}
			break;
			
		}
	case enCurrentColumn:
		{
			if(pGrid)
			{
				CCellID curCell=pGrid->GetFocusCell();
				if(p[0]->nType!=0)
				{
					pGrid->SetCurrentCell(curCell.row,p[0]->GetNumber());
				}
				return (curCell.col);
			}
			break;
			
		}
	case enRow:
		{
			if(pGrid)
			{
				//return (pGrid->Currentpoint.x);
			}
			break;
			
		}
	case enColumn:
		{
			
			if(pGrid)
			{
				//return (pGrid->Currentpoint.y);
			}
			break;
			
		}

		
	case enRedrawWindow2:
	case enRedrawWindow:
		{
			if(pGrid)
			{
				pGrid->RedrawWindow();
				return (1);
			}
			break;
			
		}
		
	case enPrint:
		{
			if(pGrid)
			{
				pGrid->Print();
				return (1);
			}
			break;
			
		}
		
		
	case enGetSelectedCount:
		{
			if(pGrid)
			{
				return (pGrid->GetSelectedCount());
			}
			break;
			
		}
	case enEnableSelection:
		{
				if(pGrid)
				{
					pGrid->EnableSelection(p[0]->GetNumber());
				}
				break;
				
		}
	case enIsCellSelected:
		{
			if(pGrid)
			{
				return (pGrid->IsCellSelected(p[0]->GetNumber(),p[1]->GetNumber()));
			}
			break;
			
		}
		
	case enLoadImage:
		{
			if(pGrid)
			{
				CImageList *hPictureForIcon=new CImageList();
				hPictureForIcon->Create( p[1]->GetNumber(), p[2]->GetNumber(), ILC_COLOR8, 1, 10);
				HBITMAP h=(HBITMAP)LoadImage(NULL,CString(p[0]->GetString()), IMAGE_BITMAP, 0,
					0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
				if(!h)
					SetError("������ �������� ��������!");
				else
				{
					hPictureForIcon->Add(CBitmap::FromHandle(h),RGB(0,0,0));
					
					pGrid->SetImageList(hPictureForIcon);
					return (1);
				}
			}
			break;
			
		}
		
	
	case enSetItemFgColour:
		{
			if(pGrid)
			{
				int nRes=pGrid->GetItemFgColour(p[0]->GetNumber(),p[1]->GetNumber());
				if(p[2]->nType!=0)//���� ���-�� ��������
				{
					int c=p[2]->GetNumber();
					if(abs(c)<256)
						c=RGB(p[2]->GetNumber(),p[3]->GetNumber(),p[4]->GetNumber());
					pGrid->SetItemFgColour(p[0]->GetNumber(),p[1]->GetNumber(),c);
				}
				return (nRes);
			}
			break;
			
		}
		
	case enSetItemBkColour:
		{
			if(pGrid)
			{
				int nRes=pGrid->GetItemBkColour(p[0]->GetNumber(),p[1]->GetNumber());
				if(p[2]->nType!=0)//���� ���-�� ��������
				{
					int c=p[2]->GetNumber();
					if(abs(c)<256)
						c=RGB(p[2]->GetNumber(),p[3]->GetNumber(),p[4]->GetNumber());
					pGrid->SetItemBkColour(p[0]->GetNumber(),p[1]->GetNumber(),c);
				}
				return (nRes);
			}
			break;
			
		}
		
	case enVirtualMode:
		{
			if(pGrid)
			{
				Ret=pGrid->GetVirtualMode();
				if(p[0]->nType!=0)//���� ���-�� ��������
				{
					pGrid->SetVirtualMode(p[0]->GetNumber());
				}
			}
			break;
			
		}
		
		
	case enValue:
		{
			
			if(pGrid)
			{
				Ret=pGrid->GetItemValue(p[0]->GetNumber(),p[1]->GetNumber());
				if(p[2]->nType!=0)//���� ���-�� ��������
				{
					pGrid->SetItemValue(p[0]->GetNumber(),p[1]->GetNumber(),*p[2]);
				}
			}
			
			break;
		}
		
	case enSetDefCellHeight:
		{
			if(pGrid)
			{
				pGrid->SetDefCellHeight(p[0]->GetNumber());
			}
			break;
			
		}
	case enSetDefCellWidth:
		{
			if(pGrid)
			{
				pGrid->SetDefCellWidth(p[0]->GetNumber());
			}
			break;
			
		}
	case enSetItemFormat:
		{
			if(pGrid)
			{
				DWORD nFormat=GetFormatValue(p[2]->GetString());
				pGrid->SetItemFormat(p[0]->GetNumber(),p[1]->GetNumber(),nFormat);
			}
			break;
			
		}
	case enSetDefItemFormat:
		{
			if(pGrid)
			{
				DWORD nFormat=GetFormatValue(p[0]->GetString());
				
				CGridCellBase* cDefaultCell;
				cDefaultCell=pGrid->GetDefaultCell(false,false);
				if(cDefaultCell)
					cDefaultCell->SetFormat(nFormat);
				cDefaultCell=pGrid->GetDefaultCell(false,true);
				if(cDefaultCell)
					cDefaultCell->SetFormat(nFormat);
				cDefaultCell=pGrid->GetDefaultCell(true,false);
				if(cDefaultCell)
					cDefaultCell->SetFormat(nFormat);
				cDefaultCell=pGrid->GetDefaultCell(true,true);
				if(cDefaultCell)
					cDefaultCell->SetFormat(nFormat);
			}
			break;
			
		}
		
		
		
	case enSelectString:
		{
			if(pGrid)
			{
				BOOL bListMode=(int)p[0]->GetNumber();
				pGrid->SetSingleRowSelection(bListMode);
				pGrid->SetListMode(bListMode);
			}
			break;
			
		}
		
		
	case enFindText:
	case enFindValue:
		{
			if(pGrid)
			{
				CString csFind;
				if(iName==enFindText)
					csFind=p[0]->GetString();
				if(p[1]->IsEmpty())//���� � �������� ������� (� ������ ���������� ���������)
				{
					int n=p[2]->GetNumber();
					for(int i=0;i<pGrid->GetRowCount();i++)
					{
						if(iName==enFindText)
						{
							if(csFind==pGrid->GetItemText(i,n))
							{
								*p[1]=i;
								return 1;
							}
						}
						else
						{
							if(*p[0]==pGrid->GetItemValue(i,n))
							{
								*p[1]=i;
								return 1;
							}
						}
					}
				}
				else//���� � �������� ������ (� ������� ���������� ���������)
				{
					int n=p[1]->GetNumber();
					for(int j=0;j<pGrid->GetColumnCount();j++)
					{
						if(iName==enFindText)
						{
							if(csFind==pGrid->GetItemText(n,j))
							{
								*p[2]=j;
								return 1;
							}
						}
						else
						{
							if(*p[0]==pGrid->GetItemValue(n,j))
							{
								*p[2]=j;
								return 1;
							}
						}
					}
				}
			}
			Ret=0;
			break;
		}
	case enFocus:
		{
			if(pGrid)
			{
				int nRes=0;
				nRes=pGrid->m_hWnd==GetFocus();
				if(p[0]->nType!=0)//���� ���-�� ��������
					if(p[0]->GetNumber())
					{
						pGrid->SetFocus();
					}
					Ret=nRes;
			}
			break;
		}
		
	case enFastFind:
		{
			if(pGrid)
			{
				Ret=pGrid->m_FastFind;
				if(p[0]->nType!=0)//���� ���-�� ��������
				{
					pGrid->m_FastFind=p[0]->GetNumber();
					if (::IsWindow(pGrid->m_TitleFind.GetSafeHwnd())) 
						pGrid->m_TitleFind.Hide();
				}
			}
			break;
		}

	case enColumnIdentifier:
		{
			if(pGrid)
			{
				int nCol=p[0]->GetNumber();
				if(p[0]->GetType()==TYPE_STRING)
				{//���� ������� ��������� ��������, �� ������� ���������� ����� �������
					return FindColumn(p[0]->GetString());
				}

				if(nCol<0||nCol>=pGrid->GetColumnCount())
					Error("������ ������� �� �������");

				Ret=String(pGrid->aColumns[nCol].csName);
				if(p[1]->nType!=0)//���� ���-�� ��������
				{
					pGrid->aColumns[nCol].csName=p[1]->GetString();
				}
			}
			break;
		}
	case enColumnVisible:
		{
			if(pGrid)
			{
				int nCol=GetColumnNumber(p[0]);
				Ret=pGrid->GetColumnVisible(nCol);
				if(p[1]->nType!=0)//���� ���-�� ��������
				{
					pGrid->SetColumnVisible(nCol,p[1]->GetNumber());
				}
			}
			break;
		}
	case enColumnType:
		{
			if(pGrid)
			{
				int nCol=GetColumnNumber(p[0]);
				Ret=String(pGrid->aColumns[nCol].csType);

				if(p[1]->nType!=0)//���� ���-�� ��������
				{
					pGrid->aColumns[nCol].csType=p[1]->GetString();
				}
			}
			break;
		}
	case enColumnFormule:
		{
			if(pGrid)
			{
				int nCol=GetColumnNumber(p[0]);
				Ret=String(pGrid->aColumns[nCol].csFormule);

				if(p[1]->nType!=0)//���� ���-�� ��������
				{
					pGrid->aColumns[nCol].csFormule=p[1]->GetString();
				}
			}
			break;
		}
	case enColumnTitle:
		{
			if(pGrid)
			{
				int nCol=GetColumnNumber(p[0]);
				Ret=String(pGrid->aColumns[nCol].csTitle);

				if(p[1]->nType!=0)//���� ���-�� ��������
				{
					pGrid->aColumns[nCol].csTitle=p[1]->GetString();
					pGrid->SetItemText(0,nCol,p[1]->GetString());
				}
			}
			break;
		}

	case enSelectRows:
		{
			if(pGrid)
			{
				int nCount=pGrid->GetRowCount()-pGrid->GetFixedRowCount();
				if(nCount>0)
				{
					nCurRow=pGrid->GetFixedRowCount()-1;
					return 1;
				}
			}
			return 0;
		}
	case enGetRow:
		{
			if(pGrid)
			{
				if(nCurRow+1<pGrid->GetRowCount())
				{
					nCurRow++;
					return 1;
				}
				else
				{
					nCurRow=pGrid->GetFixedRowCount()-1;
				}

			}
			return 0;
		}
	case enCurrentRowNumber:
	case enCurrentRowNumber2:
		Ret=nCurRow;
		if(p[0]->nType!=0)//���� ���-�� ��������
		{
			nCurRow=p[0]->GetNumber();
		}
		break;
	case enGetAttribute:
		{
			int n=FindAttribute(p[0]->GetString());
			if(n<0)
				SetError("������� �� ������!");
			Ret=GetAttribute(n);
			break;
		}
	case enSetAttribute:
		{
			int n=FindAttribute(p[0]->GetString());
			if(n<0)
				SetError("������� �� ������!");
			SetAttribute(n,*p[1]);
			break;
		}

	case enEditText://��������������
		{
			int nRow=p[0]->GetNumber();
			int nCol=p[1]->GetNumber();
			pGrid->SetCurrentCell(nRow,nCol);

			CGridCellBase* pCell=pGrid->GetCell(nRow,nCol);
			if(pCell)
			{
				pGrid->PostMessage(WM_ONEDITCELL,nRow,nCol);
/*				CPoint point;
				pGrid->GetCellOrigin(nRow,nCol,&point);
				pGrid->OnEditCell(nRow,nCol,point,VK_LBUTTON);*/

			}
			break;
		}
	case enDrawRow:
		{
			return pGrid->nDrawRow;
		}
	case enDrawCol:
		{
			return pGrid->nDrawCol;
		}
	case enEditValue:
		{
			return pGrid->cEditValue;
		}
	case enColumnEnable:
		{
			int nCol=GetColumnNumber(p[0]);
			Ret=pGrid->aColumns[nCol].bEnable;
			if(p[1]->nType)//���� ���-�� ��������
			{
				pGrid->aColumns[nCol].bEnable=p[1]->GetNumber()?1:0;
			}
			break;
		}
	case enColumnTab:
		{
			int nCol=GetColumnNumber(p[0]);
			Ret=pGrid->aColumns[nCol].bTab;
			if(p[1]->nType!=0)//���� ���-�� ��������
			{
				pGrid->aColumns[nCol].bTab=p[1]->GetNumber()?1:0;
			}
			break;
		}
	case enLength:
		{
			int nCol=GetColumnNumber(p[0]);
			Ret=pGrid->aColumns[nCol].nLength;
			if(p[1]->nType!=0)//���� ���-�� ��������
			{
				pGrid->aColumns[nCol].nLength=p[1]->GetNumber();
			}
			break;
		}
	case enPrec:
		{
			int nCol=GetColumnNumber(p[0]);
			Ret=pGrid->aColumns[nCol].nPrec;
			if(p[1]->nType!=0)//���� ���-�� ��������
			{
				pGrid->aColumns[nCol].nPrec=p[1]->GetNumber();
			}
			break;
		}
	case enModified:
		{
			Ret=pGrid->GetModified();
			if(p[0]->nType!=0)//���� ���-�� ��������
			{
				pGrid->SetModified(p[0]->GetNumber()?1:0);
			}
			break;
		}
	case enLoad://������� ������� �� ������� ��������
		{
			CValue &vSource=*p[0];
			if(vSource.GetType()!=TYPE_ARRAY)
				SetError("������ �� �������� ��������");


			int *aColList=0;
			int nColSize=0;


			CValueArray *pSource=(CValueArray *)vSource.pRef;
			for(int nKey=0;nKey<pSource->aValueArray.GetSize();nKey++)
			{
				CValue &vRow=pSource->aValueArray.GetAt(nKey);
				if(vRow.GetType()!=TYPE_ARRAY)
					continue;
				CValueArray *pRow=(CValueArray *)vRow.pRef;
				if(!pRow->pStringValueList)
					continue;
				nColSize=pRow->pStringValueList->GetCount();
				if(!nColSize)
					continue;

				if(!aColList)
				{
					aColList=new int [nColSize];
					for(int i=0;i<nColSize;i++)
						aColList[i]=-1;
				}

				//CGridCtrl *pGrid;
				int nRow=pGrid->GetRowCount();

				//����� ������
				pGrid->SetRowCount(nRow+1);


				CValue Val;
				POSITION pos;
				CStringExt csKey;
				int i=-1;
				for( pos = pRow->pStringValueList->GetStartPosition(); pos != NULL; )
				{
					i++;
					pRow->pStringValueList->GetNextAssoc( pos, csKey, Val);
					if(i>=nColSize)
						SetError("������ ��������� ������� �����������");

					int nCol=aColList[i];
					if(nCol<0)
					{
						nCol=FindColumn(csKey);
						aColList[i]=nCol;
					}
					if(nCol>=0)
					{
						pGrid->SetItemValue(nRow,nCol,Val);
					}
				}
			}

			if(aColList)
				delete aColList;


			break;
		}
/*
			��� ���=1 �� �������������(��������������) ����
				�������������������.�����������();
				��� ���=1 �� ������������������� ����
					���=�������������[���];
					//����� ��������� �������� �� �����, �.�. �������������� ������� ����� ���� ������
					�������������������.�����������������(���,��������������[���][���]);
				����������
			����������
*/		
		case enSetControlRect:
			{
				int x, y, nWidth, nHeight;
				x=p[0]->GetNumber();
				y=p[1]->GetNumber();
				nWidth=p[2]->GetNumber();
				nHeight=p[3]->GetNumber();
				ASSERT(pWnd);
				((CWnd*)pWnd)->MoveWindow(x, y, nWidth, nHeight,1);
				((CWnd*)pWnd)->RedrawWindow();
				break;
			}
		case enGetControlRect:
			{
				CRect mRect(0,0,0,0);
				ASSERT(pWnd);
				((CWnd*)pWnd)->GetWindowRect(&mRect);
				CWnd* pParentWnd=((CWnd*)pWnd)->GetParent();
				pParentWnd->ScreenToClient(&mRect);
				*p[0] = CValue(mRect.left);
				*p[1] = CValue(mRect.top);
				*p[2] = CValue(mRect.right - mRect.left);
				*p[3] = CValue(mRect.bottom - mRect.top);
				break;
			}
		case enSetCoveredCells:
			{
				if(p[0]->GetType()!=0&&p[1]->GetType()!=0&&p[2]->GetType()!=0&&p[3]->GetType()!=0)
				{
					pGrid->SetCoveredCells(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetNumber(),p[3]->GetNumber());
				}
				break;
			}
		case enSetUnCoveredCells:
			{
				if(p[0]->GetType()!=0&&p[1]->GetType()!=0)
				{
					pGrid->SetUnCoveredCells(p[0]->GetNumber(),p[1]->GetNumber(),0);
				}
				break;
			}
		
			

	}
	
	return Ret;
}

void CValueGrid::SetAttribute(int iName,CValue &Val)
{
	if(nCurRow>=0&&nCurRow<pGrid->GetRowCount())
	{
		pGrid->SetItemValue(nCurRow,iName,Val);
	}
}
CValue CValueGrid::GetAttribute(int iName)
{
	if(nCurRow>=0&&nCurRow<pGrid->GetRowCount())
	{
		return pGrid->GetItemValue(nCurRow,iName);
	}
	return CValue();
}
int  CValueGrid::FindColumn(CString csName)
{
	csName.MakeUpper();
	csName.TrimRight();
	for(int i=0;i<pGrid->aColumns.GetSize();i++)
	{
		CString csCurName=pGrid->aColumns[i].csName;
		csCurName.MakeUpper();
		csCurName.TrimRight();
		if(csCurName==csName)
			return i;
	}
	return -1;
}
int  CValueGrid::FindAttribute(CString csName)
{
	if(nCurRow>=0&&nCurRow<pGrid->GetRowCount())
	{
		return FindColumn(csName);
	}
	else
	{
		Error("�� ������� ������� ������");
	}
	return -1;
}
CString CValueGrid::GetAttributeName(int nNumber,int nAlias)
{
	if(nCurRow>=0&&nCurRow<pGrid->GetRowCount())
	{
		if(nNumber>=0&&nNumber<pGrid->aColumns.GetSize())
			return pGrid->aColumns[nNumber].csName;
		else
			Error("������ ������� �� ������� ������ ���������������");
	}
	else
	{
		Error("�� ������� ������� ������");
	}
	return "";
}
int  CValueGrid::GetNAttributes(void)
{
	return pGrid->aColumns.GetSize();
}

CString CValueGrid::GetString(void)const
{
	if(pWnd)
		return csObjectName;
	else
		return NOT_DEFINED;
};

int CValueGrid::GetColumnNumber(CValue *pVal)
{
	int nCol;
	if(pVal->GetType()==TYPE_STRING)
	{//���� ������� ��������� ��������...
		nCol=FindColumn(pVal->GetString());
	}
	else
	{
		nCol=pVal->GetNumber();
	}

	if(nCol<0||nCol>=pGrid->GetColumnCount())
		Error("������ ������� �� �������");
	return nCol;
}
