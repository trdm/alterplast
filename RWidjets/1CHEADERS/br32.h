// Begin forward declarations
// End forward declarations
// Begin class descriptions
class IMPORT_1C CBmpButton:public CBitmapButton //CObject BitBlt
{
DECLARE_DYNAMIC(CBmpButton)

public:
	 CBmpButton(void);	//1
	virtual  ~CBmpButton(void);	//28
protected:
	void  CreateBitmaps(void);	//63
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//100
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(HWND,unsigned int,unsigned int,long);	//192
	long  OnBmSetStyle(unsigned int,long);	//225
	int  OnCreate(struct tagCREATESTRUCTA *);	//256
	unsigned int  OnGetDlgCode(void);	//272
	virtual void  PostNcDestroy(void);	//342
public:
	static int  RegisterControlClass(void);	//360
protected:
	static long  WndProcHook(HWND,unsigned int,unsigned int,long);	//413

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CBrowse:public CWnd //CView //CObject
{
DECLARE_DYNAMIC(CBrowse)

public:
	
	// + 04h
	CSkipper* pDoc; // [0x40]
	// + 08h
	int m_flag; // [0x44]
	// + 0C
	CObArray m_colArray; // массив колонок // [0x48]
	//void* vtable; // [0x48]
	//CObject** m_pData; // [0x4C]
	//int m_nSize; // [0x50]
	//int m_nMaxSize; // [0x54]
	//int m_nGrowBy; // [0x58]
	// + ??h
	CObArray m_strArray; // массив содержимого €чеек // [0x5C]
	//void* vtable; // [0x5C]
	//CObject** m_pData; // [0x60]
	//int m_nSize; // [0x64]
	//int m_nMaxSize; // [0x68]
	//int m_nGrowBy; // [0x6C]
	long m_numRow; // количество строк // [0x70]
	long m_numHeadRow; // количество строк в заголовке // [0x74]
	RECT m_wndrect; // [0x78]
//LONG    left; // [0x78]
//LONG    top; // [0x7C]
//LONG    right; // [0x80]
//LONG    bottom; // [0x84]
	CString m_str; // [0x88]
	DWORD m_param1; // [0x8C]
	CWnd* m_br_Edit; // [0x90]
	DWORD m_param3; // [0x94] // флаг редактируемости таблицы
	DWORD m_param4; // [0x98]
	DWORD m_param5; // [0x9C]
	DWORD m_param6; // [0xA0]
	DWORD m_br_AppendStyle; // [0xA4] // AppendStyle
	DWORD m_param8; // [0xA8] // перва€ строка дл€ перерисовки
	DWORD m_param9; // [0xAC] // последн€€ строка дл€ перерисовки
	DWORD m_param10; // [0xB0]
	DWORD m_br_Font; // [0xB4] // Font
	DWORD m_param12; // [0xB8]
	DWORD m_param13; // [0xBC] // высота одной строки текста
	int m_SelRow; // [0xC0] // текуща€ строка
	int m_SelCol; // [0xC4] // текуща€ колонка
	int m_FixCol; // [0xC8] // количество фиксированных колонок
	DWORD m_param17; // [0xCC]
	int m_FixRow; // [0xD0] // количество фиксированных строк
	DWORD m_param19; // [0xD4]
	//ULONG m_br_BkColor; // [0xD8]

	enum AppendStyle {asOne, asTwo};
	 CBrowse(HWND);	//2
	 CBrowse(class CWnd *);	//3
	 CBrowse(void);	//4
	virtual  ~CBrowse(void);	//29
	class CColumn &  operator[](int);	//45
	int  Add(class CColumn * pColumn,int nPosStyle);	//52
protected:
	int  AjustColBounds(int,int);	//54
public:
	void  BeginEdit(void);	//55
	void  Cancel(void);	//59
protected:
	int  CheckCancel(unsigned int,unsigned int);	//60
	int  ColWidth(int,int);	//61
public:
	int  Delete(int);	//86
	void  DeleteSearchWnd(void);	//89
protected:
	void  DrawFocusRect(void);	//99
public:
	static void  EnableMouseWheel(int);	//108
	void  EndAppend(int);	//109
	void  EndEdit(int);	//110
	void  FixCurRow(void);	//112
	void  FixHorz(int);	//113
	void  FixVert(int);	//114
	enum CBrowse::AppendStyle   GetAppendStyle(void);	//118
	void  GetBufferText(class CString &,int,int);	//123
	void  GetBufferText(char *,int,int,int);	//124
	void  GetCellRect(struct tagRECT *,int,int);	//126
	class CColumn &  GetColumn(unsigned int);	//127
	int  GetColumnIndex(unsigned int);	//128
	class CColumn &  GetColumnN(int);	//129
	int  GetColumnNum(void);	//130
	int  GetColumnWidth(unsigned int);	//131
	class CWnd *  GetEdit(void);	//148
protected:
	long  GetFont(unsigned int,long);	//152
	int  GetHiTrackBound(int);	//154
	int  GetLoTrackBound(int);	//156
public:
	int  GetMaxHeadLines(void);	//157
	void  GetTextRect(struct tagRECT *,int,int);	//197
	void  Goto(int,int);	//198
	int  GotoCol(unsigned int);	//199
	void  HideColumn(unsigned int);	//200
	int  HitTest(class CPoint,int *,int *);	//201
	int  Insert(int,class CColumn *,int);	//205
protected:
	virtual int  IsFocused(void);	//210
	int  IsPtOnSeparator(class CPoint,int *,int *);	//213
	int  LastCol(void);	//214
	int  NumCols(void);	//219
	long  OnBrCancel(unsigned int,long);	//226
	void  OnCancelMode(void);	//240
	void  OnChar(unsigned int,unsigned int,unsigned int);	//242
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//248
protected:
	void  OnContextMenu(class CWnd *,class CPoint);	//253
	int  OnCreate(struct tagCREATESTRUCTA *);	//257
	void  OnDestroy(void);	//263
	unsigned int  OnGetDlgCode(void);	//273
	void  OnHScroll(unsigned int,unsigned int,class CScrollBar *);	//275
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//279
	void  OnKillFocus(class CWnd *);	//281
	void  OnLButtonDblClk(unsigned int,class CPoint);	//283
	void  OnLButtonDown(unsigned int,class CPoint);	//284
	void  OnLButtonUp(unsigned int,class CPoint);	//286
	void  OnMouseMove(unsigned int,class CPoint);	//293
	int  OnMouseWheel(unsigned int,short,class CPoint);	//296
	void  OnMove(int,int);	//297
	void  OnPaint(void);	//313
	void  OnRButtonDown(unsigned int,class CPoint);	//316
	void  OnRButtonUp(unsigned int,class CPoint);	//317
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//319
	void  OnSetFocus(class CWnd *);	//320
	void  OnSize(unsigned int,int,int);	//327
	void  OnTimer(unsigned int);	//330
	void  OnVScroll(unsigned int,unsigned int,class CScrollBar *);	//336
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//348
	void  RefreshAll(void);	//358
	void  RefreshCurrent(void);	//359
	static int  RegisterControlClass(void);	//361
protected:
	virtual long  SendCommand(unsigned int,unsigned int);	//368
public:
	void  SetAppendStyle(enum CBrowse::AppendStyle);	//374
	void  SetAutoAdjust(int);	//375
	void  SetBkColor(unsigned long);	//376
	void  SetBufferText(char const *,int,int);	//378
protected:
	void  SetColWidth(void);	//379
public:
	void  SetColumnWidth(unsigned int,int);	//380
protected:
	long  SetFont(unsigned int,long);	//393
public:
	void  SetMaxHeadLines(int);	//395
	void  SetSkipper(class CSkipper *,int);	//404
	static void  SetWaitProc(int (__cdecl*)(int));	//405
	void  ShowColumn(unsigned int);	//407
	int  StartAppend(void);	//408
	void  bottom(void);	//445
	void  center(void);	//446
	void  down(int);	//464
	void  left(int);	//471
	void  pgDown(void);	//483
	void  pgUp(void);	//484
protected:
	void  refresh(void);	//486
public:
	void  right(int);	//487
protected:
	virtual void  scroll(int);	//488
	int  stable(void);	//490
public:
	void  top(void);	//491
	void  up(int);	//492

	// перехват OnPain дл€ раскрашивани€ таблицы

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CBrowseView:public CFormView //CObject
{
DECLARE_DYNCREATE(CBrowseView)

public:
	UINT m_BrowseID; //80h
	CBrowse* m_pBrowse; //84h
	DWORD _buf[0x04];
protected:
	 CBrowseView(int,unsigned int);	//5
	 CBrowseView(void);	//6
	virtual  ~CBrowseView(void);	//30
public:
	int  BroadcastCommand(unsigned int,HWND,unsigned int);	//56
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//90
public:
	class CBrowse *  GetBrowse(void);	//119
	unsigned int  GetBrowseID(void);	//121
protected:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//249
	virtual void  OnInitialUpdate(void);	//278
	void  OnNcDestroy(void);	//300
	void  OnSetFocus(class CWnd *);	//321
	void  OnSize(unsigned int,int,int);	//328
	long  OnWMUpdateUI(unsigned int,long);	//337
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//338
	virtual int  PreTranslateMessage(struct tagMSG *);	//349

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMaskEdit:public CWnd //CObject
{
DECLARE_DYNAMIC(CMaskEdit)

public:
	char buffer[512];
	 CMaskEdit(void);	//26
	virtual  ~CMaskEdit(void);	//43
	int  Create(DWORD dwStyle,RECT& rect,CWnd * pParentWnd,UINT nID,LPCTSTR lpszWindowName);	//62
	int  CreateEx(DWORD dwStyle,RECT& rect,CWnd * pParentWnd,UINT nID,LPCTSTR lpszWindowName);	//64
	static char  GetAccDelimeter(void);	//116
	static char  GetCommonFiller(void);	//132
	char  GetFiller(void);	//149
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(HWND,unsigned int,unsigned int,long);	//195
protected:
	virtual void  PostNcDestroy(void);	//345
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//353
	static void  RegisterControlClass(void);	//364
	static void  SetAccDelimeter(char);	//370
	static void  SetCommonFiller(char);	//381
	void  SetFiller(char);	//391
};


class IMPORT_1C CBtnEdit:public CMaskEdit //CObject
{
DECLARE_DYNAMIC(CBtnEdit)

public:
	char buffer[512];
	 CBtnEdit(void);	//7
	virtual  ~CBtnEdit(void);	//31

	DECLARE_MESSAGE_MAP()

protected:
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(HWND,unsigned int,unsigned int,long);	//193

	void  GetButtonRect(class CRect &);	//125
	void  OnLButtonUp(unsigned int,class CPoint);	//287
	void  OnMouseMove(unsigned int,class CPoint);	//294
	void  OnNcCalcSize(int,struct tagNCCALCSIZE_PARAMS *);	//298
	unsigned int  OnNcHitTest(class CPoint);	//301
	void  OnNcLButtonDblClk(unsigned int,class CPoint);	//304
	void  OnNcLButtonDown(unsigned int,class CPoint);	//305
	void  OnNcPaint(void);	//306
	void  OnUse(void);	//335
public:
	static int  RegisterControlClass(void);	//362
	void  ShowButton(int);	//406
protected:
	static long  WndProcHook(HWND,unsigned int,unsigned int,long);	//414
};


class IMPORT_1C CColumn:public CCmdTarget //CObject
{
DECLARE_DYNAMIC(CColumn)

public:
	CBrowse* m_Browse;
	DWORD m_ID;
	char* m_Caption;
	DWORD m_Flag1;
	DWORD m_Flag2;
	DWORD m_Flag3;
	DWORD m_Flag4;
	DWORD m_Flag5;
	CCellGet* m_CellGet;
	CImageList m_Image;
	//DWORD buffer[256];
	 CColumn(unsigned int,char const *,int);	//10
	virtual  ~CColumn(void);	//35
protected:
	virtual int  DrawItem(class CDC &,class CRect &,class CString &,int);	//101
public:
	virtual void  GetData(class CString &);	//141
	virtual int  SetData(class CString &);	//385
protected:
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//103
//public:
//	int PublicDrawItem(class CDC & DC,class CRect &Rect,class CString &st,int flag)
//		{return DrawItem(DC,Rect,st,flag);};
protected:
	virtual int  PreValid(void);	//354
	virtual int  PostValid(void);	//346
public:
	void  SetRedNegativeDisplay(int);	//402
};

class CColumnEdited:public CColumn
{
public:
	CGetField* m_pGetField;
};

class IMPORT_1C CComboBrowse:public CObject
{
DECLARE_DYNAMIC(CComboBrowse)

public:
	 CComboBrowse(HWND);	//11
	 CComboBrowse(void);	//12
	virtual  ~CComboBrowse(void);	//36
protected:
	void  DrawButton(class CDC &,int);	//97
	long  GetDlgCode(unsigned int,long);	//146
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(HWND,unsigned int,unsigned int,long);	//194
	void  OnChangePos(void);	//241
	void  OnChar(unsigned int,unsigned int,unsigned int);	//243
	int  OnCreate(struct tagCREATESTRUCTA *);	//258
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//280
	void  OnKillFocus(class CWnd *);	//282
	void  OnLButtonDown(unsigned int,class CPoint);	//285
	void  OnLButtonUp(unsigned int,class CPoint);	//289
	void  OnMouseMove(unsigned int,class CPoint);	//295
	int  OnNcCreate(struct tagCREATESTRUCTA *);	//299
	unsigned int  OnNcHitTest(class CPoint);	//303
	void  OnNcPaint(void);	//307
	void  OnPaint(void);	//315
	void  OnSetFocus(class CWnd *);	//323
	long  OnSetFont(unsigned int,long);	//325
	long  OnShowDropDown(unsigned int,long);	//326
	virtual void  PostNcDestroy(void);	//344
public:
	static int  RegisterControlClass(void);	//363
protected:
	static long  WndProcHook(HWND,unsigned int,unsigned int,long);	//415
public:
	void  godba(long);	//467

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CGetCtrl:public CCmdTarget //CObject
{
DECLARE_DYNAMIC(CGetCtrl)

public:
	//размер: 0x34
	CGetDoc7* m_pGetDoc;
	DWORD bufGetCtrl[0x03];
	UINT m_uiID;
	 CGetCtrl(unsigned int);	//22
	virtual  ~CGetCtrl(void);	//40
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//251

	DECLARE_MESSAGE_MAP()

public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//351
	virtual void  SetActive(void);	//371
	virtual int  Activate(void);	//48
	virtual int  DeActivate(void);	//85
	virtual void  SetCtrlText(char const *);	//384
	virtual void  GetCtrlText(char *,int);	//140
	virtual void  GetCtrlText(class CString &);	//139
	virtual void  GetStringData(char *,int);	//190
	virtual void  GetStringData(class CString &);	//189
	virtual void  SetReadOnly(int);	//400
protected:
	virtual int  DataExchange(int);	//82
public:
	virtual class CWnd *  GetDlgItem(void);	//147
	virtual void  Enable(int);	//107
	virtual int  IsEnable(void);	//209

	int  IsActive(void);	//206
protected:
	void  OnBeginUse(void);	//223
	int  OnCtrlClpBrd(unsigned int);	//259
};


class IMPORT_1C CCellGet:public CGetCtrl //CObject
{
DECLARE_DYNAMIC(CCellGet)

public:
	//размер: 0x54
	CBrowse* m_pBrowse;
	CGetColumn* m_pGetColumn;
	char* m_strColCaption;
	int m_iWidth;
	int m_iUnknow1;
	int m_iUnknow2;
	int m_iUnknow3;
	char* m_strEditCaption;
	 CCellGet(class CBrowse *	a_pBrowse,
			  unsigned int		a_uiID,
			  int				a_iWidth,
			  char const *		a_strColCaption,
			  int				a_iUnknow1,
			  char const *		a_strEditCaption,
			  int				a_iUnknow2);	//8

	 CCellGet(class CBrowse *	a_pBrowse,
			  unsigned int		a_uiID,
			  char const *		a_strEditCaption,
			  int				a_iUnknow1);	//9

	virtual  ~CCellGet(void);	//34
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//250

	DECLARE_MESSAGE_MAP()

protected:
	virtual int  Activate(void);	//46
	virtual int  DeActivate(void);	//84
public:
	virtual void  SetCtrlText(char const *);	//382
	virtual void  GetCtrlText(char *,int);	//136
	virtual void  GetCtrlText(class CString &);	//135
	virtual int  IsEnable(void);	//207
	//virtual class CWnd *  Edit(class CRect &);	//102

protected:
	void  OnBrEndEditCancel(void);	//227
	void  OnBrEndEditOk(void);	//228
	int  OnClipBrd(unsigned int);	//244
	void  OnEnChange(void);	//267
	void  OnEnKillFocus(void);	//269
	void  OnEnMaxText(void);	//270
	void  OnEnSetFocus(void);	//271
	void  OnIdcEdit(void);	//276
	void  OnUpdateClipBrd(class CCmdUI *);	//331
};


class IMPORT_1C CEditGet:public CGetCtrl //CObject
{

DECLARE_DYNAMIC(CEditGet)

public:
	 CEditGet(unsigned int,int);	//13
	virtual  ~CEditGet(void);	//37
	virtual int  PreTranslateMessage(struct tagMSG *);	//350

	DECLARE_MESSAGE_MAP()

public:
	virtual int  Activate(void);	//47
	virtual void  SetCtrlText(char const *);	//383
	virtual void  GetCtrlText(char *,int);	//138
	virtual void  GetCtrlText(class CString &);	//137
	virtual void  Enable(int);	//106
	virtual int  IsEnable(void);	//208

protected:
	int  OnClipBrd(unsigned int);	//245
	void  OnEnChange(void);	//268
	void  OnMaxText(void);	//291
	void  OnSetFocus(void);	//324
	void  OnUpdateClipBrd(class CCmdUI *);	//332
};


class IMPORT_1C CDocEditGet:public CEditGet //CObject
{
DECLARE_DYNCREATE(CDocEditGet)

public:
	int buf1[7];
	
	//CGetDoc77* m_GetDoc; // јль‘ - указатель на атрибуты класса GetDoc7
	CGetDoc7* m_GetDoc; // јль‘ - указатель на атрибуты класса GetDoc7 artbear

	int buf2[3];
	int m_ID;
	int buf3[3];
	
	//CGetDoc77* m_GetDoc1; // јль‘ - указатель на атрибуты класса GetDoc7
	CGetDoc7* m_GetDoc1; // јль‘ - указатель на атрибуты класса GetDoc7 artbear

	//CFormCtrl* m_FormCtrl;
	CGetField* m_FormCtrl; //јль‘
	int buf4[30];
};


class IMPORT_1C CFColumn:public CColumn //CObject
{
DECLARE_DYNAMIC(CFColumn)

public:
	DWORD m_dwFColumn1;
	DWORD m_dwFColumn2;
	DWORD m_dwFColumn3;
	DWORD m_dwFColumn4;
	DWORD m_dwFColumn5;
	DWORD m_dwFColumn6;
	DWORD m_dwFColumn7;
	 CFColumn(unsigned int,char *,char const *,int,int,int);	//14
	 CFColumn(unsigned int,int *,char const *,int,int);	//15
	 CFColumn(unsigned int,long *,char const *,int,int);	//16
	 CFColumn(unsigned int,double *,char const *,int,char const *,int);	//17
	 CFColumn(unsigned int,class CDate *,char const *,int,int);	//18
	 CFColumn(unsigned int,class CString *,char const *,int,int,int);	//19
	 //(DlgCtrlID, текущее значение, ???,,,длина);
	virtual  ~CFColumn(void);	//38
	virtual void  GetData(class CString &);	//142
	virtual int  SetData(class CString &);	//386
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//104
};


class IMPORT_1C CGetColumn:public CColumn //CObject
{
DECLARE_DYNAMIC(CGetColumn)

public:
	DWORD m_dwGetColumn1;
	DWORD m_dwGetColumn2;
	 CGetColumn(unsigned int,char const *,int,int);	//20
	 CGetColumn(class CCellGet *);	//21
	virtual  ~CGetColumn(void);	//39
protected:
	virtual void  GetData(class CString &);	//143
	virtual int  SetData(class CString &);	//387
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//105
	virtual int  PreValid(void);	//355
};


class IMPORT_1C CGetDoc:public CDocument // CObject //
{
DECLARE_DYNCREATE(CGetDoc)

public:
	//DWORD buf[0x013];//(A0h - 54h) / 4
	DWORD m_Attr_01h_54h;
	//DWORD m_Attr_02h_58h; CObArray
	//DWORD m_Attr_03h_5Ch;
	//DWORD m_Attr_04h_60h;
	//DWORD m_Attr_05h_64h;
	//DWORD m_Attr_06h_68h;
	CObArray m_CtrlArray;
	DWORD m_Attr_07h_6Ch;
	//DWORD m_Attr_08h_70h;
	CGetCtrl * m_ActiveCtrl;
	DWORD m_Attr_09h_74h;
	//DWORD m_Attr_0Ah_78h;
	UINT m_BrowseID;
	DWORD m_Attr_0Bh_7Ch;
	DWORD m_Attr_0Ch_80h;
	DWORD m_Attr_0Dh_84h;
	DWORD m_Attr_0Eh_88h;
	//DWORD m_Attr_0Fh_8Ch;
	UINT m_SavePromptResNum;
	//DWORD m_Attr_10h_90h;
	UINT m_DeletePromptResNum;
	DWORD m_Attr_11h_94h;
	DWORD m_Attr_12h_98h;
	DWORD m_Attr_13h_9Ch;

public:
	virtual  ~CGetDoc(void);	//41
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//252
	DECLARE_MESSAGE_MAP()
protected:
	virtual int  OnNewDocument(void);	//309
	virtual int  CanCloseFrame(class CFrameWnd *);	//58
public:
	virtual int  SaveModified(void);	//367
protected:
	virtual void  PreCloseFrame(class CFrameWnd *);	//347
	virtual void  OnIdle(void);	//277
public:
	virtual class CWnd *  GetForm(void);	//153
	virtual int  OnStoreRec(void);	//329
	virtual int  OnDeleteRec(void);	//262
	virtual void  OnNewRec(void);	//310
	virtual void  OnCopyRec(long);	//255
	virtual int  OnCancelEdit(void);	//239
	virtual void  OnRead(int);	//318
	virtual int  OnGetUse(unsigned int,unsigned int,long);	//274
	virtual unsigned int  OnMakeUse(unsigned int);	//290
	virtual void  OnAbortUse(void);	//221
	virtual int  UpdateData(int,unsigned int);	//411
	virtual void  OnActivateCtrl(unsigned int);	//222
	virtual void  GetStringData(class CString &,unsigned int);	//191
	virtual int  BufferExchange(int);	//57
	virtual void  SetModify(int);	//397
	virtual int  PreTranslateMessage(struct tagMSG *);	//352
	virtual int  DeleteCurrent(void);	//88
	virtual void  GetSavePrompt(class CString &);	//188
	virtual void  GetDeletePrompt(class CString &);	//145
protected:
	virtual int  DataExchange(class CGetExchange *);	//83
	virtual void  UpdateDefCommand(void);	//412

	 CGetDoc(void);	//23
public:
	void  AddGet(class CGetCtrl *);	//53
public:
	void  DeleteColumn(int);	//87
	class CGetCtrl *  GetActiveCtl(void);	//117
	class CBrowse *  GetBrowse(void);	//120
	unsigned int  GetBrowseID(void);	//122
	class CGetCtrl *  GetCtrl(unsigned int);	//133
	int  GetCtrlN(class CGetCtrl *);	//134
protected:
	unsigned int  GetDefFormCommand(void);	//144
public:
	static int  GetFlag(enum GetDocFlags);	//151
	int  IsLast(class CGetCtrl *);	//212
	static int  LoadColumnWidth(class CGetDoc *);	//215
protected:
	void  OnBeginUse(void);	//224
	void  OnBrowseAppendStart(void);	//229
	void  OnBrowseChPos(void);	//230
	void  OnBrowseDoubleClick(void);	//231
	void  OnBrowseFillCurrent(void);	//232
	void  OnBrowseFillRow(void);	//233
	void  OnBrowseWantChPos(void);	//234
	void  OnBrowseWantHScroll(void);	//235
	void  OnBrowseWantRefresh(void);	//236
	void  OnBrowseWantVScroll(void);	//237
	void  OnCancel(void);	//238
	void  OnCmdDoUse(void);	//246
	void  OnCmdGetUse(void);	//247
	void  OnCopy(void);	//254
	void  OnDefFileCommand(void);	//260
	void  OnDelete(void);	//261
	void  OnEdit(void);	//266
	void  OnNew(void);	//308
	void  OnNext(void);	//311
public:
	void  OnNextCtl(int);	//312
protected:
	void  OnUpdateCmdButtons(class CCmdUI *);	//333
	void  OnWrite(void);	//339
public:
	static int  SaveColumnWidth(class CGetDoc *);	//365
	void  SetActiveCtl(unsigned int);	//372
	void  SetActiveCtl(class CGetCtrl *);	//373
	void  SetBrowseID(unsigned int);	//377
	void  SetDefCommand(unsigned int);	//388
protected:
	void  SetDefFormCommand(unsigned int);	//389
public:
	static void  SetDeletePromptID(unsigned int);	//390
	static void  SetFlag(enum GetDocFlags,int);	//392
	void  SetReadOnly(int);	//401
	static void  SetSavePromptID(unsigned int);	//403
};


class IMPORT_1C CGetExchange
{
public:
	 CGetExchange(class CGetDoc *,int,unsigned int,class CString *);	//24
	void  Fail(void);	//111
	class CGetCtrl *  PrepareColumn(unsigned int);	//356
	class CGetCtrl *  PrepareControl(unsigned int);	//357
};


class IMPORT_1C CGetTemplate:public CObject
{
DECLARE_DYNAMIC(CGetTemplate)

public:
	 CGetTemplate(unsigned int,struct CRuntimeClass *,struct CRuntimeClass *,struct CRuntimeClass *,unsigned int,unsigned int,unsigned int);	//25
	virtual  ~CGetTemplate(void);	//42
	static class CDocument *  ActivateDocFrame(unsigned int);	//49
	static class CDocument *  GetFirstDoc(unsigned int);	//150
	static class CGetTemplate *  GetTemplate(unsigned int);	//196
	virtual void  InitialUpdateFrame(class CFrameWnd *,class CDocument *,int);	//204
	static int  LoadFramePosition(class CFrameWnd *,unsigned int);	//216
	class CDocument *  OpenDocumentFile(int);	//340
	static class CDocument *  OpenDocumentFile(unsigned int,int);	//341
	static int  SaveFramePosition(class CFrameWnd *,unsigned int);	//366
	static void __cdecl SetProfileInfo(void (__stdcall*)(int,char const *,char *,int)); //398
};


class IMPORT_1C CSkipper:public CObject
{
DECLARE_DYNAMIC(CSkipper)

public:
	DWORD m_dwMaxIndex;
	DWORD m_dwMinIndex;
	DWORD m_Unknow;
	DWORD m_dba; // текуща€ позици€
	DWORD m_Flag;
	 CSkipper(void);	//27
	virtual  ~CSkipper(void);	//44

	virtual int  next(void) = 0;
	virtual int  prev(void) = 0;
	virtual void  gotop(void) = 0;
	virtual void  gobot(void) = 0;
	virtual long  getpos(void) = 0;
	virtual long  setpos(void) = 0;

	virtual void  read(void);	//485
	virtual int  skip(int);	//489
	virtual int  Ok(void);	//220
	virtual int  ateof(void);	//444
	virtual void  godba(long);	//468
	virtual long  getdba(void);	//466
	virtual void  goeof(void);	//469
	virtual long  MaxIndex(void);	//217
	virtual long  MinIndex(void);	//218
	virtual long  GetIndex(void);	//155
	virtual void  SetIndex(long);	//394
	virtual void  SetMinMaxIndex(void);	//396
	virtual int  find(char const *);	//465
protected:
	virtual void  init(void);	//470
};

class IMPORT_1C CCBrowse:public CBrowse //CObject
{
DECLARE_DYNAMIC(CCBrowse)

public:
	virtual  ~CCBrowse(void);	//32
protected:
	virtual int  IsFocused(void);	//211
public:
	void  OnDestroy(void);	//264
	void  OnLButtonUp(unsigned int,class CPoint);	//288
	int  OnMouseActivate(class CWnd *,unsigned int,unsigned int);	//292
	unsigned int  OnNcHitTest(class CPoint);	//302
	void  OnSetFocus(class CWnd *);	//322
protected:
	virtual long  SendCommand(unsigned int);	//369

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCellEdit:public CEdit //CObject
{
DECLARE_DYNAMIC(CCellEdit)

public:
	virtual  ~CCellEdit(void);	//33
protected:
	int  OnEdit(unsigned int);	//265
	void  OnPaint(void);	//314
	void  OnUpdateEdit(class CCmdUI *);	//334
	virtual void  PostNcDestroy(void);	//343

	DECLARE_MESSAGE_MAP()
};


// End class descriptions
// Begin free functions definitions
IMPORT_1C class CDocument * __cdecl ActivateOpenDocFrame(class CDocTemplate *,char const *);
IMPORT_1C void __cdecl ActivateOpenDocFrame(class CDocument *);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,int &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,long &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,double &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,long double &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,class CDate &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,class CNumeric &);
IMPORT_1C void __stdcall DDX_MaskText(class CDataExchange *,int,class CString &,int);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,int &);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,long &);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,double &,int,int);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,long double &,int,int);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,class CDate &,enum CDateFormat);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,class CNumeric &,int,int);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,class CString &);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,unsigned int,char *,int);
IMPORT_1C void __cdecl Draw3DBox(class CDC &,struct tagRECT *,int);
IMPORT_1C void __cdecl Draw3dEllipse(HDC,struct tagRECT *,unsigned short,unsigned short,unsigned int);
IMPORT_1C void __cdecl Draw3dInsetEllipse(HDC,struct tagRECT *);
IMPORT_1C void __cdecl Draw3dInsetRect(HDC,struct tagRECT *,unsigned short);
IMPORT_1C void __cdecl Draw3dPopRect(HDC,struct tagRECT *,unsigned short);
IMPORT_1C void __cdecl Draw3dRec(HDC,struct tagRECT *,unsigned short,unsigned short,unsigned short);
IMPORT_1C void __cdecl DrawDlgFrame(HDC,struct tagRECT *);
IMPORT_1C void __cdecl Free3dDlg(void);
IMPORT_1C unsigned int __cdecl GetMouseScrollLines(int);
IMPORT_1C class CDocument * __cdecl GetOpenDocument(class CDocTemplate *,char const *);
IMPORT_1C int __cdecl Init3dDlg(void);
IMPORT_1C void __cdecl InitBrowseLibrary(void);
IMPORT_1C int __cdecl Subclass3dDlg(HWND);
IMPORT_1C void __cdecl SysColorChange3d(void);
// End free functions definitions
// _MyWordBreakProc@16
// _edit_GetAccDelimeter@0
// _edit_GetAlignment@4
// _edit_GetBackColor@4
// _edit_GetBorder@4
// _edit_GetClipStripping@4
// _edit_GetCommonFiller@0
// _edit_GetDecimalChar@4
// _edit_GetDouble@4
// _edit_GetFiller@4
// _edit_GetFont@4
// _edit_GetHighlightColor@4
// _edit_GetInt@4
// _edit_GetLeader@16
// _edit_GetLength@4
// _edit_GetLong@4
// _edit_GetLongDouble@4
// _edit_GetMask2@4
// _edit_GetMask@4
// _edit_GetNegativeColor@4
// _edit_GetPositiveOnly@4
// _edit_GetPrecision@4
// _edit_GetSeparator@4
// _edit_GetSeparatorChar@4
// _edit_GetText@16
// _edit_GetTextColor@4
// _edit_GetTextLen@8
// _edit_GetTrailer@16
// _edit_GetType@4
// _edit_Init@4
// _edit_SetAccDelimeter@4
// _edit_SetAlignment@8
// _edit_SetBackColor@8
// _edit_SetBorder@8
// _edit_SetClipStripping@8
// _edit_SetCommonFiller@4
// _edit_SetDecimalChar@8
// _edit_SetDouble@12
// _edit_SetEnabled@8
// _edit_SetFiller@8
// _edit_SetFont@8
// _edit_SetHScroll@8
// _edit_SetHighlightColor@8
// _edit_SetInt@8
// _edit_SetLeader@12
// _edit_SetLength@8
// _edit_SetLong@8
// _edit_SetLongDouble@12
// _edit_SetMask2@8
// _edit_SetMask@8
// _edit_SetMaskInfo@8
// _edit_SetNegativeColor@8
// _edit_SetPositiveOnly@8
// _edit_SetPrecision@8
// _edit_SetSeparator@8
// _edit_SetSeparatorChar@8
// _edit_SetText@12
// _edit_SetTextColor@8
// _edit_SetTrailer@12
// _edit_SetType@8
// _edit_SetVScroll@8


