//
//	класс CFListCtrl		(файл FListCtrl.h)
//	дерево-список
//
//	версия:		2.0
//	дата:		12/04/2002
//	автор:		Денис Солоненков
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

#include "FListCtrl.h"		// в конце файла обявлен макрос HIDE

////	раскомментируйте следующие строки
////		чтобы отобразить скрытые переменные/функции в окне ClassView
//#if !defined(HIDE)
//#define HIDE /##*
//#endif


#include <afxtempl.h>


//	ОПИСАТЕЛЬ СТРОКИ
typedef POSITION HFROW;		//описатель строки дерева-скиска

#define FL_ROOT		((HFROW) 0x00000001L)	//
#define FL_LAST		((HFROW) 0x00000002L)	//последняя строка
#define FL_FIRST	((HFROW) 0x00000003L)	//первая строка
#define FL_SORTPOS	((HFROW) 0x00000004L)	//

#define FL_UNDEFINED	-1

// ВЫРАВНИВАНИЕ ТЕКСТА	(используется в InsertFColumn
#define FL_LEFT		LVCFMT_LEFT		
#define FL_RIGHT	LVCFMT_RIGHT
#define FL_CENTER	LVCFMT_CENTER

//	ЭЛЕМЕНТЫ УПРАВЛЕНИЯ		(функции AddControl/GetControlType/SetControlType)
#define FL_BUTTON			2					//кнопка
#define FL_CHECKBOX			4					//флажок
#define FL_EDIT				8					//поле ввода
#define FL_COMBOBOX			(16|FL_BUTTON)		//выпадающий список
	//дополнительные стили элементов управления
#define FL_SORT				32		//сортировка элементов выпадающего списка
#define FL_AUTOCHECK		64		//автоматическое переключение флажка
#define FL_PLUSBUTTON		128		//кнопка с изображением +

//	СОСТОЯНИЕ CHECKBOX'а	(функции GetFItemCheck/SetFItemCheck)
#define FL_UNCHECKED		0		//не установлен
#define FL_CHECKED			1		//установлен
#define FL_INDETERMINATE	2		//неопределенное состояние



//	СТИЛИ		(фукнция GetFStyle/SetFStyle)
#define FL_READONLY		1			//только чтение
#define FL_LINEAR_TREE	2			//узлы дерева располагаются в линию


//	УПРАВЛЕНИЕ ЦВЕТОМ
#define FL_NOCOLOR			(0x0100000)	//цвет не задан
#define FL_DEFCOLOR			(0x0200000)	//цвет по умолчанию
#define FL_DONTSET			(0x0300000)	//цвет по умолчанию

#define FL_BACKGROUND		101			// фон
#define FL_TEXT				102			//текст

//	область, для которой устанавливается цвет
#define FL_FULL				128			//по умолчанию
#define FL_FCOLUMN			256			//колонка
#define FL_FROW				512			//строка
#define FL_SELECTED_FROW	1024		//выделенная строка
#define FL_SELECTED_FITEM	2048		//выделенный элемент


// область			(функция GetFRect)
#define FLRC_FITEM				201		// элемент
#define FLRC_FITEM_CLIENT		202
#define FLRC_FITEM_TREEBUTTON	203
#define FLRC_FITEM_CHECKBOX		204
#define FLRC_FITEM_IMAGE		205
#define FLRC_FITEM_COMBOBOX		206
#define FLRC_FITEM_BUTTON		207
#define FLRC_FITEM_TEXT			208
#define FLRC_FROW				209		//строка
#define FLRC_FROW_TOP			210
#define FLRC_VISIBLE_FROWS		211
#define FLRC_FCOLUMN			212		//колонка

//минимальная ширина колонки при автоматическом определении ширины
#define FL_MIN_FCOLUMN_WIDTH	30



//	константы для функции GetNextFRow
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


//	КОНСТАНТЫ СООБЩЕНИЙ
	#define FLNM_USER_MSG(x)		(WM_USER+200+x)

//возвращаемое значение обработчика сообщения
#define FL_OK			0	//	
#define FL_CANCEL		1	//
#define FL_CONTINUE		2	//продолжить редактирование, применяется совместно с FL_OK или FL_CANCEL

#define FLNM_COLLAPSE		FLNM_USER_MSG(1)	//	закрыт/раскрыт узел дерева
#define FLNM_CHECK			FLNM_USER_MSG(2)	//	изменилось состояние флажка
#define FLNM_DELETE			FLNM_USER_MSG(3)	//	удаление строки
#define FLNM_SELECT			FLNM_USER_MSG(4)	//	изменилось выделение

#define FLNM_EDIT_BEGIN			FLNM_USER_MSG(5)	//	пользователь начинает редактирование
#define FLNM_EDIT_END			FLNM_USER_MSG(6)	//	пользователь закончил редактирование
#define FLNM_BUTTON_CLICK		FLNM_USER_MSG(7)	//	пользователь нажал на кнопку
#define FLNM_COMBOBOX_EXPAND	FLNM_USER_MSG(8)	//	пользователь открывает выпадающий список


//структура с информацией, которая передается обработчику события
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


// элемент выпадающего списка
HIDE*/ struct FL_COMBOITEM{			
	CString strText;
	DWORD dwUserData;
};
typedef CArray<FL_COMBOITEM, FL_COMBOITEM> FL_COMBOITEMS;


//	сортирующая функция
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
/**/		// операции с профилями контролов редактирования
	
	//	добавляет новый профиль контролов редактирования и возвращает его индекс
	int AddControl(int nControlType=FL_EDIT);
	//	возвращает число профилей
	int GetControlCount() {return m_pRefControls->GetSize();}
	//	удаляет все профили (только свои, см. AttachControls)
	void RemoveAllControls();
	//	возвращает тип контролов редактирования для данного профиля
	int GetControlType(int iControlProfile);
	//	устанавливает тип контролов редактирования для данного профиля
	void SetControlType(int iControlProfile, int nControlType);
	//	возвращает картинки элемента checkbox указанного профиля
	void GetControlCheckImages(int iControlProfile, int *piChecked=NULL, int *piUnchecked=NULL, int *piIndeterminate=NULL);
	//	устанавливает картинки элемента checkbox указанного профиля
	void SetControlCheckImages(int iControlProfile, int iChecked=-1, int iUnchecked=-1, int iIndeterminate=-1);
	//	добавляет элемент в выпадающий список combobox'а для  указанного профиля
	void AddControlComboItem(int iControlProfile, CString strText, DWORD dwUserData=0);
	//	возвращает массив элементов выпадающего списка combobox'а
	FL_COMBOITEMS* GetControlCombo(int iControlProfile);
	//	возвращает выделенный элемент выпадающего списка combobox'а
	FL_COMBOITEM GetControlComboSel(int iControlProfile);
	//	устанавливает выделенный элемент выпадающего списка combobox'а
	void SetControlComboSel(int iControlProfile, CString strText, DWORD dwUserData);
	//  подключается к контролам другого flist'а
	void AttachControls(CFListCtrl* pFList){m_pRefControls=pFList->m_pRefControls;}
	//  отключает чужые контролы
	void DetachControls(){m_pRefControls=&m_aControlProfiles;}
	
/**/		//													
/**/		// операции с колонками дерева-списока			

	//	втавляет новую колонку в дерево-список
	int InsertFColumn(int iIndex, CString strText, int nWidth, int nFormat=FL_LEFT, int iDefaultControlProfile=FL_UNDEFINED);
	//	удаляет колонку
	void DeleteFColumn(int iIndex);
	//	возвращает число колонок 
	int GetFColumnCount() {return m_aFColumns.GetSize();}
	//	возвращает индекс колонки с деревом
	int GetTree();
	//	устанавлисает колонку, в которой будет отображаться дерево
	void SetTree(int iFColumn=-1);	
	//	возвращает имя колонки
	CString GetFColumnCaption(int iFColumn);
	//	устанавливает имя колонки
	void SetFColumnCaption(int iFColumn, CString strText);
	//	возвращает ширину колонки
	int GetFColumnWidth(int iFColumn) {return GetColumnWidth(iFColumn);}
	//	устанавливает ширину колонки
	void SetFColumnWidth(int iFColumn, int nWidth){SetColumnWidth(iFColumn, nWidth);}

/**/	//
/**/	// операции со строками таблицы
	
	//	вставляет новую строку в дерево-список
	HFROW InsertFRow(HFROW hParent, HFROW hPosition, CString strText, int iImageIndex=-1, int iControlProfile=-1,BOOL bFast=0);
	//	удаляет строку
	void DeleteFRow(HFROW hFRow);	
	//	удаляет все строки
	void DeleteAllFRows()	{DeleteFRow(FL_ROOT);}
	//	возвращает число строк
	int GetFRowCount(HFROW hParent=FL_ROOT);
	//	возвращает описатель строки по её индексу
	HFROW FindIndex(HFROW hParent, int iIndex);
	//	возвращает номер строки в листе по её описателю
	int GetFRowIndex(HFROW hFRow);
	//	возвращает описатель "следующей" строки 
	HFROW GetNextFRow(HFROW hFRow, int nfrCode=FL_NEXT);
	//	закрывает/раскрывает узел дерева
	void Collapse(HFROW hFRow, BOOL bCollapse=TRUE);
	//	возвращает TRUE если узел закрыт или не имеет дочерних строк
	BOOL IsCollapsed(HFROW hFRow);	
	//	обновляет строку
	void UpdateFRow(HFROW hFRow);
	//	меняет две строки местами
	void ExchangeFRows(HFROW hFRow1, HFROW hFRow2);
	//перемещает строку
	void MoveFRows(HFROW hFRow, HFROW hFParent);
	//изменяет уровень всех элементов
	void SetAllDeep(HFROW hFRow1,int nDeep);
	//	возвращает TRUE, если для строки установлен стиль bold (жирный текст)
	BOOL IsFRowBold(HFROW hFRow);
	//	устанавливает стиль текста bold для указанной строки
	void SetFRowBold(HFROW hFRow, BOOL bBoldText=TRUE);
	//	возвращает уровень вложенности для строки начиная с единицы
	int GetFRowDeep(HFROW hFRow);
	//	устанавливает профиль элементов управления для каждого элемента строки
	void SetFRowControls(HFROW hFRow, int iControlProfile1, ...);
	//	поиск строки
	HFROW FindFRow(HFROW hParent, HFROW hFirst, int iFColumn, CString text, BOOL bCompareNoCase=TRUE);
	//	возвращает TRUE, если все родительские узлы раскрыты
	BOOL IsFRowVisible(HFROW hFRow);

/**/	//
/**/	// операции с элементами строк
	
	//	возвращает текст
	CString GetFItemText(HFROW hFRow, int iFColumn);
	//	устанавливает текст 
	void SetFItemText(HFROW hFRow, int iFColumn, CString strText);	
	//	возвращает индекс картинки
	int GetFItemImage(HFROW hFRow, int iFColumn);	
	//	устанавливает картинку
	void SetFItemImage(HFROW hFRow, int iFColumn, int iImageIndex);	
	//	возвращает пользовательские данные
	DWORD GetFItemData(HFROW hFRow, int iFColumn);
	//	устанавливает пользовательские данные
	void SetFItemData(HFROW hFRow, int iFColumn, DWORD dwUserData);
	//	прокручивает таблицу так, чтобы указанный элемент стал видимым
	BOOL EnsureVisible(HFROW hFRow, int iFColumn);
	//	возвращает состояние checkbox'а
	int GetFItemCheck(HFROW hFRow, int iFColumn);
	//	устанавливает состояние checkbox'a
	void SetFItemCheck(HFROW hFRow, int iFColumn, int nState=FL_CHECKED);	
	//	возвращает индекс профиля контролов редактирования указанного элемента строки
	int GetFItemControl(HFROW hFRow, int iFColumn);
	//	устанавливает профиль контролов редактирования для указанного элемента строки
	void SetFItemControl(HFROW hFRow, int iFColumn, int iControlProfile);	

/**/	//
/**/	// выделение

	//	выделяет указанную строку/элемент дерева-списка
	void Select(HFROW hFRow, int iFColumn=-1, BOOL bEnsureVisible=TRUE);
	//	возваращает описатель выделенной строки
	inline HFROW GetSelectedFRow()	{return GetSel().m_hSelectedFRow;}
	//	возвращает индекс выделенной колонки
	inline int GetSelectedFColumn() {return IsReadOnly() ? -1 : GetSel().m_iSelectedFColumn;}

/**/	//
/**/	// свойства

	//	возвращает стиль дерева-списка
	int GetFStyle() {return GetOptions().m_nStyle;}
	//	модифицирует стиль дерева-списка
	void SetFStyle(int nDeleteStyle, int nAddStyle, int nMask=0);	
	//	устанваливает стиль readonly
	void SetReadOnly(BOOL bReadOnly=TRUE);
	//	возвращает TRUE, если редактирование запрещено
	BOOL IsReadOnly()	{return m_opt.m_nStyle&FL_READONLY;}
	//	возвращает индекс изображения для кнопки раскрыть/закрыть узел
	void GetTreeButton(int *piExpandImage, int *piCollapseImage);
	//	устанавливает изображение для кнопоки раскрыть/закрыть узел
	void SetTreeButton(int iExpandImage, int iCollapseImage);
	//	возвращает число строк текста в одной строке таблицы	
	int GetTextLineCount()	{return m_opt.m_nTextLineCount;}
	//	устанавливает число строк текста в одной строке таблицы	
	void SetTextLineCount(int nTextLineCount);

/**/	//
/**/	// разное
	
	//	определяет прямоугольник требуемой области
	BOOL GetFRect(HFROW hFRow, int iFColumn, int nArea, CRect &rect, BOOL bFItemOrigin=FALSE);
	//	возвращает цвет текста/фона для элемента/стронки/колонки/фона
	COLORREF GetColor(int nType=FL_TEXT, HFROW hFRow=NULL, int iFColumn=-1);
	//	устанавлинвает цвет для указанной области
	void SetColor(int nArea, HFROW hFRow, int iFColumn, COLORREF crText=FL_DONTSET, COLORREF crBackground=FL_DONTSET);

	//	сортирует строки по тексту в указанной колонке
	void Sort(int iFColumn, BOOL bReverseSort=TRUE);
	//	сортирует подстроки для указанной строки
	void Sort(int iFColumn, HFROW hParent, BOOL bSortChild=TRUE);	
	//	сортирующая функция
	CompareFRows *m_pCompareFRowsFunc;		//указатель на функцию сравнения строк таблицы		
	//	устанавливает ширину колонки так, чтобы она вмещала в себя текст в каждой строке таблицы
	void UpdateFColumnWidth(int iFColumn);
	//  
	void SetImageList(CImageList *pImageList){CListCtrl::SetImageList(pImageList, LVSIL_SMALL);}
	
	//Окно, которому будут посылаться сообщения. По умолчанию это родительское окно
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
		int m_iDefaultControlProfile;		//контрол по умолчанию		
		int m_nFormat;						//выравнивание текста
		BOOL m_bReverseSort;				//флаг обратной сортировки						
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

		HIDE*/ CArray<XFList_Item, XFList_Item> m_aFItems;		//элементы строки

		HIDE*/ CList<HFROW, HFROW>	m_lstChildFRows;			//дочерние строки
		
		HIDE*/ BOOL m_bCollapsed;								//дочерняя ветвь закрыта
		HIDE*/ int	m_nDeep;									//уровень вложенности
		HIDE*/ int m_iListIndex;								//индекс в списке видимых строк

		HIDE*/ BOOL m_bBoldText;								//жирный текст
		XFList_Color m_color;

		HIDE*/ HFROW	m_hFRow;								//описатель в списке m_lstFRows
		HIDE*/ HFROW	m_hParentFRow;							//родительский описатель
		HIDE*/ POSITION m_posParentList;						//позиция в списке родителя

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
//		этот макрос скрывает переменную/функцию/класс в окне ClassView
	#define HIDE /##*

//
#endif // !defined(AFX_FLISTCTRL_H__27FB9EBC_DCA4_4E86_847F_8DB878AA000C__INCLUDED_)
