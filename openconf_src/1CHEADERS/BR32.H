// Begin forward declarations
// End forward declarations
// Begin class descriptions
class CBmpButton:public CBitmapButton //CObject BitBlt
{
DECLARE_DYNAMIC(CBmpButton)

public:
	 CBmpButton(void);	//1
	virtual  ~CBmpButton(void);	//28
protected:
	void  CreateBitmaps(void);	//63
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//100
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(struct HWND__ *,unsigned int,unsigned int,long);	//192
	long  OnBmSetStyle(unsigned int,long);	//225
	int  OnCreate(struct tagCREATESTRUCTA *);	//256
	unsigned int  OnGetDlgCode(void);	//272
	virtual void  PostNcDestroy(void);	//342
public:
	static int  RegisterControlClass(void);	//360
protected:
	static long  WndProcHook(struct HWND__ *,unsigned int,unsigned int,long);	//413

	DECLARE_MESSAGE_MAP()
};


class CBrowse:public CWnd //CView //CObject
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
	CObArray m_strArray; // массив содержимого ячеек // [0x5C]
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
	DWORD m_param8; // [0xA8]
	DWORD m_param9; // [0xAC]
	DWORD m_param10; // [0xB0]
	DWORD m_br_Font; // [0xB4] // Font
	DWORD m_param12; // [0xB8]
	DWORD m_param13; // [0xBC]
	DWORD m_param14; // [0xC0]
	DWORD m_param15; // [0xC4]
	DWORD m_param16; // [0xC8]
	DWORD m_param17; // [0xCC]
	DWORD m_param18; // [0xD0]
	DWORD m_param19; // [0xD4]
	//ULONG m_br_BkColor; // [0xD8]

	enum AppendStyle {asOne, asTwo};
	 CBrowse(struct HWND__ *);	//2
	 CBrowse(class CWnd *);	//3
	 CBrowse(void);	//4
	virtual  ~CBrowse(void);	//29
	class CColumn &  operator[](int);	//45
	int  Add(class CColumn *,int);	//52
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

	// перехват OnPain для раскрашивания таблицы
public:
	static void SetWrap(void);
	static void OffWrap(void);
	void  OnPaintWrap(void);
	int  WrapDrawItem(class CDC & DC,class CRect & Rect,class CString & st,int flag);

	DECLARE_MESSAGE_MAP()
};


class CBrowseView:public CFormView //CObject
{
DECLARE_DYNCREATE(CBrowseView)

protected:
	 CBrowseView(int,unsigned int);	//5
	 CBrowseView(void);	//6
	virtual  ~CBrowseView(void);	//30
public:
	int  BroadcastCommand(unsigned int,struct HWND__ *,unsigned int);	//56
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


class CMaskEdit:public CWnd //CObject
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
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(struct HWND__ *,unsigned int,unsigned int,long);	//195
protected:
	virtual void  PostNcDestroy(void);	//345
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//353
	static void  RegisterControlClass(void);	//364
	static void  SetAccDelimeter(char);	//370
	static void  SetCommonFiller(char);	//381
	void  SetFiller(char);	//391
};


class CBtnEdit:public CMaskEdit //CObject
{
DECLARE_DYNAMIC(CBtnEdit)

public:
	char buffer[512];
	 CBtnEdit(void);	//7
	virtual  ~CBtnEdit(void);	//31
protected:
	void  GetButtonRect(class CRect &);	//125
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(struct HWND__ *,unsigned int,unsigned int,long);	//193
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
	static long  WndProcHook(struct HWND__ *,unsigned int,unsigned int,long);	//414

	DECLARE_MESSAGE_MAP()
};


class CColumn:public CCmdTarget //CObject
{
DECLARE_DYNAMIC(CColumn)

public:
	char buffer[256];
	 CColumn(unsigned int,char const *,int);	//10
	virtual  ~CColumn(void);	//35
protected:
	virtual int  DrawItem(class CDC &,class CRect &,class CString &,int);	//101
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//103
public:
	int PublicDrawItem(class CDC & DC,class CRect &Rect,class CString &st,int flag)
		{return DrawItem(DC,Rect,st,flag);};
	virtual void  GetData(class CString &);	//141
protected:
	virtual int  PostValid(void);	//346
	virtual int  PreValid(void);	//354
public:
	virtual int  SetData(class CString &);	//385
	void  SetRedNegativeDisplay(int);	//402
};


class CComboBrowse:public CObject
{
DECLARE_DYNAMIC(CComboBrowse)

public:
	 CComboBrowse(struct HWND__ *);	//11
	 CComboBrowse(void);	//12
	virtual  ~CComboBrowse(void);	//36
protected:
	void  DrawButton(class CDC &,int);	//97
	long  GetDlgCode(unsigned int,long);	//146
	virtual long (__stdcall**  GetSuperWndProcAddr(void))(struct HWND__ *,unsigned int,unsigned int,long);	//194
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
	static long  WndProcHook(struct HWND__ *,unsigned int,unsigned int,long);	//415
public:
	void  godba(long);	//467

	DECLARE_MESSAGE_MAP()
};


class CGetCtrl:public CCmdTarget //CObject
{
DECLARE_DYNAMIC(CGetCtrl)

public:
	char buffer[512];
	 CGetCtrl(unsigned int);	//22
	virtual  ~CGetCtrl(void);	//40
	virtual int  Activate(void);	//48
protected:
	virtual int  DataExchange(int);	//82
public:
	virtual int  DeActivate(void);	//85
	virtual void  Enable(int);	//107
	virtual void  GetCtrlText(class CString &);	//139
	virtual void  GetCtrlText(char *,int);	//140
	virtual class CWnd *  GetDlgItem(void);	//147
	virtual void  GetStringData(class CString &);	//189
	virtual void  GetStringData(char *,int);	//190
	int  IsActive(void);	//206
	virtual int  IsEnable(void);	//209
protected:
	void  OnBeginUse(void);	//223
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//251
protected:
	int  OnCtrlClpBrd(unsigned int);	//259
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//351
	virtual void  SetActive(void);	//371
	virtual void  SetCtrlText(char const *);	//384
	virtual void  SetReadOnly(int);	//400

	DECLARE_MESSAGE_MAP()
};


class CCellGet:public CGetCtrl //CObject
{
DECLARE_DYNAMIC(CCellGet)

public:
	char buffer[512];
	 CCellGet(class CBrowse *,unsigned int,int,char const *,int,char const *,int);	//8
	 CCellGet(class CBrowse *,unsigned int,char const *,int);	//9
	virtual  ~CCellGet(void);	//34
protected:
	virtual int  Activate(void);	//46
	virtual int  DeActivate(void);	//84
	virtual class CWnd *  Edit(class CRect &);	//102
public:
	virtual void  GetCtrlText(class CString &);	//135
	virtual void  GetCtrlText(char *,int);	//136
	virtual int  IsEnable(void);	//207
protected:
	void  OnBrEndEditCancel(void);	//227
	void  OnBrEndEditOk(void);	//228
	int  OnClipBrd(unsigned int);	//244
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//250
protected:
	void  OnEnChange(void);	//267
	void  OnEnKillFocus(void);	//269
	void  OnEnMaxText(void);	//270
	void  OnEnSetFocus(void);	//271
	void  OnIdcEdit(void);	//276
	void  OnUpdateClipBrd(class CCmdUI *);	//331
public:
	virtual void  SetCtrlText(char const *);	//382

	DECLARE_MESSAGE_MAP()
};


class CEditGet:public CGetCtrl //CObject
{

DECLARE_DYNAMIC(CEditGet)

public:
	 CEditGet(unsigned int,int);	//13
	virtual  ~CEditGet(void);	//37
	virtual int  Activate(void);	//47
	virtual void  Enable(int);	//106
	virtual void  GetCtrlText(class CString &);	//137
	virtual void  GetCtrlText(char *,int);	//138
	virtual int  IsEnable(void);	//208
protected:
	int  OnClipBrd(unsigned int);	//245
	void  OnEnChange(void);	//268
	void  OnMaxText(void);	//291
	void  OnSetFocus(void);	//324
	void  OnUpdateClipBrd(class CCmdUI *);	//332
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//350
	virtual void  SetCtrlText(char const *);	//383
	virtual void  SetReadOnly(int);	//399

	DECLARE_MESSAGE_MAP()
};


class CDocEditGet:public CEditGet //CObject
{
DECLARE_DYNCREATE(CDocEditGet)

public:
	int buf1[7];
	CGetDoc77* m_GetDoc;
	int buf2[3];
	int m_ID;
	int buf3[3];
	CGetDoc77* m_GetDoc1;
	CFormCtrl* m_FormCtrl;
	int buf4[30];
};


class CFColumn:public CColumn //CObject
{
DECLARE_DYNAMIC(CFColumn)

public:
	char buffer[512];
	 CFColumn(unsigned int,char *,char const *,int,int,int);	//14
	 CFColumn(unsigned int,int *,char const *,int,int);	//15
	 CFColumn(unsigned int,long *,char const *,int,int);	//16
	 CFColumn(unsigned int,double *,char const *,int,char const *,int);	//17
	 CFColumn(unsigned int,class CDate *,char const *,int,int);	//18
	 CFColumn(unsigned int,class CString *,char const *,int,int,int);	//19
	 //(DlgCtrlID, текущее значение, ???,,,длина);
	virtual  ~CFColumn(void);	//38
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//104
	virtual void  GetData(class CString &);	//142
	virtual int  SetData(class CString &);	//386
};


class CGetColumn:public CColumn //CObject
{
DECLARE_DYNAMIC(CGetColumn)

public:
	 CGetColumn(unsigned int,char const *,int,int);	//20
	 CGetColumn(class CCellGet *);	//21
	virtual  ~CGetColumn(void);	//39
protected:
	virtual class CWnd *  Edit(class CRect &,class CBrowse *);	//105
	virtual void  GetData(class CString &);	//143
	virtual int  PreValid(void);	//355
	virtual int  SetData(class CString &);	//387
};


class CGetDoc:public CDocument // CObject //
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

protected:
	 CGetDoc(void);	//23
public:
	virtual  ~CGetDoc(void);	//41
	void  AddGet(class CGetCtrl *);	//53
	virtual int  BufferExchange(int);	//57
protected:
	virtual int  CanCloseFrame(class CFrameWnd *);	//58
	virtual int  DataExchange(class CGetExchange *);	//83
public:
	void  DeleteColumn(int);	//87
	virtual int  DeleteCurrent(void);	//88
	class CGetCtrl *  GetActiveCtl(void);	//117
	class CBrowse *  GetBrowse(void);	//120
	unsigned int  GetBrowseID(void);	//122
	class CGetCtrl *  GetCtrl(unsigned int);	//133
	int  GetCtrlN(class CGetCtrl *);	//134
protected:
	unsigned int  GetDefFormCommand(void);	//144
public:
	virtual void  GetDeletePrompt(class CString &);	//145
	static int  GetFlag(enum GetDocFlags);	//151
	virtual class CWnd *  GetForm(void);	//153
	virtual void  GetSavePrompt(class CString &);	//188
	virtual void  GetStringData(class CString &,unsigned int);	//191
	int  IsLast(class CGetCtrl *);	//212
	static int  LoadColumnWidth(class CGetDoc *);	//215
	virtual void  OnAbortUse(void);	//221
	virtual void  OnActivateCtrl(unsigned int);	//222
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
public:
	virtual int  OnCancelEdit(void);	//239
protected:
	void  OnCmdDoUse(void);	//246
	void  OnCmdGetUse(void);	//247
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//252
protected:
	void  OnCopy(void);	//254
public:
	virtual void  OnCopyRec(long);	//255
protected:
	void  OnDefFileCommand(void);	//260
	void  OnDelete(void);	//261
public:
	virtual int  OnDeleteRec(void);	//262
protected:
	void  OnEdit(void);	//266
public:
	virtual int  OnGetUse(unsigned int,unsigned int,long);	//274
protected:
	virtual void  OnIdle(void);	//277
public:
	virtual unsigned int  OnMakeUse(unsigned int);	//290
protected:
	void  OnNew(void);	//308
	virtual int  OnNewDocument(void);	//309
public:
	virtual void  OnNewRec(void);	//310
protected:
	void  OnNext(void);	//311
public:
	void  OnNextCtl(int);	//312
	virtual void  OnRead(int);	//318
	virtual int  OnStoreRec(void);	//329
protected:
	void  OnUpdateCmdButtons(class CCmdUI *);	//333
	void  OnWrite(void);	//339
	virtual void  PreCloseFrame(class CFrameWnd *);	//347
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//352
	static int  SaveColumnWidth(class CGetDoc *);	//365
	virtual int  SaveModified(void);	//367
	void  SetActiveCtl(unsigned int);	//372
	void  SetActiveCtl(class CGetCtrl *);	//373
	void  SetBrowseID(unsigned int);	//377
	void  SetDefCommand(unsigned int);	//388
protected:
	void  SetDefFormCommand(unsigned int);	//389
public:
	static void  SetDeletePromptID(unsigned int);	//390
	static void  SetFlag(enum GetDocFlags,int);	//392
	virtual void  SetModify(int);	//397
	void  SetReadOnly(int);	//401
	static void  SetSavePromptID(unsigned int);	//403
	virtual int  UpdateData(int,unsigned int);	//411
protected:
	virtual void  UpdateDefCommand(void);	//412

	DECLARE_MESSAGE_MAP()
};


class CGetExchange
{
public:
	 CGetExchange(class CGetDoc *,int,unsigned int,class CString *);	//24
	void  Fail(void);	//111
	class CGetCtrl *  PrepareColumn(unsigned int);	//356
	class CGetCtrl *  PrepareControl(unsigned int);	//357
};


class CGetTemplate:public CObject
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


class CSkipper:public CObject
{
DECLARE_DYNAMIC(CSkipper)

public:
	DWORD m_dwMaxIndex;
	DWORD m_dwMinIndex;
	DWORD m_Unknow;
	DWORD m_dba; // текущая позиция
	DWORD m_Flag;
	 CSkipper(void);	//27
	virtual  ~CSkipper(void);	//44
	virtual long  GetIndex(void);	//155
	virtual long  MaxIndex(void);	//217
	virtual long  MinIndex(void);	//218
	virtual int  Ok(void);	//220
	virtual void  SetIndex(long);	//394
	virtual void  SetMinMaxIndex(void);	//396
	virtual int  ateof(void);	//444
	virtual int  find(char const *);	//465
	virtual long  getdba(void);	//466
	virtual void  godba(long);	//468
	virtual void  goeof(void);	//469
protected:
	virtual void  init(void);	//470
public:
	virtual void  read(void);	//485
	virtual int  skip(int);	//489
};


class CCBrowse:public CBrowse //CObject
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


class CCellEdit:public CEdit //CObject
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
class CDocument * __cdecl ActivateOpenDocFrame(class CDocTemplate *,char const *);
void __cdecl ActivateOpenDocFrame(class CDocument *);
void __stdcall DDX_MaskText(class CDataExchange *,int,int &);
void __stdcall DDX_MaskText(class CDataExchange *,int,long &);
void __stdcall DDX_MaskText(class CDataExchange *,int,double &);
void __stdcall DDX_MaskText(class CDataExchange *,int,long double &);
void __stdcall DDX_MaskText(class CDataExchange *,int,class CDate &);
void __stdcall DDX_MaskText(class CDataExchange *,int,class CNumeric &);
void __stdcall DDX_MaskText(class CDataExchange *,int,class CString &,int);
void __cdecl DX_Text(class CGetExchange *,unsigned int,int &);
void __cdecl DX_Text(class CGetExchange *,unsigned int,long &);
void __cdecl DX_Text(class CGetExchange *,unsigned int,double &,int,int);
void __cdecl DX_Text(class CGetExchange *,unsigned int,long double &,int,int);
void __cdecl DX_Text(class CGetExchange *,unsigned int,class CDate &,enum CDateFormat);
void __cdecl DX_Text(class CGetExchange *,unsigned int,class CNumeric &,int,int);
void __cdecl DX_Text(class CGetExchange *,unsigned int,class CString &);
void __cdecl DX_Text(class CGetExchange *,unsigned int,char *,int);
void __cdecl Draw3DBox(class CDC &,struct tagRECT *,int);
void __cdecl Draw3dEllipse(struct HDC__ *,struct tagRECT *,unsigned short,unsigned short,unsigned int);
void __cdecl Draw3dInsetEllipse(struct HDC__ *,struct tagRECT *);
void __cdecl Draw3dInsetRect(struct HDC__ *,struct tagRECT *,unsigned short);
void __cdecl Draw3dPopRect(struct HDC__ *,struct tagRECT *,unsigned short);
void __cdecl Draw3dRec(struct HDC__ *,struct tagRECT *,unsigned short,unsigned short,unsigned short);
void __cdecl DrawDlgFrame(struct HDC__ *,struct tagRECT *);
void __cdecl Free3dDlg(void);
unsigned int __cdecl GetMouseScrollLines(int);
class CDocument * __cdecl GetOpenDocument(class CDocTemplate *,char const *);
int __cdecl Init3dDlg(void);
void __cdecl InitBrowseLibrary(void);
int __cdecl Subclass3dDlg(struct HWND__ *);
void __cdecl SysColorChange3d(void);
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


