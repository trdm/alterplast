// Begin forward declarations
// End forward declarations
// Begin class descriptions

class IMPORT_1C SECControlBar:public CControlBar
{
	DECLARE_DYNCREATE(SECControlBar)

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_Buf[0x2F]; // размер объекта: 0x0134 байта
	// CControlBar virtual functions
	virtual  ~SECControlBar(void);	//106
//	virtual class CSize   CalcDynamicLayout(int,unsigned long);	//180
//	virtual class CSize   CalcFixedLayout(int,int);	//183
//	virtual void  DoPaint(class CDC *);	//330
//	virtual void  DrawBorders(class CDC *,class CRect &);	//337
//	virtual void  OnUpdateCmdUI(class CFrameWnd *,int);	//1166
	virtual int  OnToolHitTest(class CPoint,struct tagTOOLINFOA *)const;	//1156
	virtual class CSize   CalcDynamicLayout(int,unsigned long);	//180
	virtual class CSize   CalcFixedLayout(int,int);	//183
	virtual void  OnUpdateCmdUI(class CFrameWnd *,int);	//1166
	virtual void  DoPaint(class CDC *);	//330
protected:
	// CControlBar virtual functions
//	virtual void  DrawGripper(class CDC *,class CRect &);	//349
public:
	// SECControlBar virtual functions
/*58*/	virtual int  Create(class CWnd *,char const *,unsigned long,unsigned long,unsigned int,struct CCreateContext *);	//220
/*59*/	virtual int  Create(char const *,char const *,unsigned int,unsigned long,unsigned long,struct tagRECT const &,class CWnd *,struct CCreateContext *);	//221
/*60*/	virtual void  GetInsideRect(class CRect &)const;	//541
/*61*/	virtual void  SetExBarStyle(unsigned long,int);	//1343
/*62*/	virtual void  ModifyBarStyleEx(unsigned long,unsigned long,int);	//872

		virtual void  DrawBorders(class CDC *,class CRect &);	//337

/*64*/	virtual void  OnBarDock(void);	//936
/*65*/	virtual void  OnBarFloat(void);	//938
/*66*/	virtual void  OnBarMDIFloat(void);	//940
/*67*/	virtual void  GetBarInfoEx(class SECControlBarInfo *);	//455
/*68*/	virtual void  SetBarInfoEx(class SECControlBarInfo *,class CFrameWnd *);	//1310
protected:
	// SECControlBar virtual functions
/*69*/	virtual void  OnExtendContextMenu(class CMenu *);	//998
/*70*/	virtual void  AdjustInsideRectForGripper(class CRect &,int);	//158
		virtual void  DrawGripper(class CDC *,class CRect &);	//349
/*72*/	virtual void  DrawGripperCloseButton(class CDC *,class CRect &,int);	//350
/*73*/	virtual void  DrawGripperCloseButtonDepressed(class CDC *);	//351
/*74*/	virtual void  DrawGripperCloseButtonRaised(class CDC *);	//352
/*75*/	virtual void  DrawGripperExpandButton(class CDC *,class CRect &,int);	//353
/*76*/	virtual void  DrawGripperExpandButtonDepressed(class CDC *);	//354
/*77*/	virtual void  DrawGripperExpandButtonRaised(class CDC *);	//355
/*78*/	virtual void  SetGripperExpandButtonState(int);	//1352
public:
	 SECControlBar(void);	//46
	void  CalcInsideRect(class CRect &,int,int)const;	//186
	void  EnableDocking(unsigned long);	//392
	void  EraseNonClient(void);	//416
	void  GetBarInfo(class SECControlBarInfo *);	//454
	int  IsMDIChild(void)const;	//810
	void  SetBarInfo(class SECControlBarInfo *,class CFrameWnd *);	//1309

	static int  m_bOptimizedRedrawEnabled;	//1630
protected:
	void  DeleteLayoutInfo(void);	//304
	void  InitLayoutInfo(void);	//768
	void  OnContextMenu(class CWnd *,class CPoint);	//966
	void  OnDestroy(void);	//980
	int  OnEraseBkgnd(class CDC *);	//995
	void  OnFloatAsMDIChild(void);	//1003
	void  OnHide(void);	//1018
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1042
	void  OnLButtonDown(unsigned int,class CPoint);	//1049
	void  OnLButtonUp(unsigned int,class CPoint);	//1054
	void  OnSize(unsigned int,int,int);	//1138
	void  OnToggleAllowDocking(void);	//1153
public:
};

class IMPORT_1C SECCustomToolBar:public SECControlBar
{
DECLARE_DYNCREATE(SECCustomToolBar)

public:
	struct Wrapped{};
	enum BarNotifyCode{bncOne};
	 SECCustomToolBar(void);	//48
	virtual  ~SECCustomToolBar(void);	//108
protected:
	virtual int  AcceptDrop(void)const;	//125
public:
	void  AddButton(int,int,int,int);	//132
protected:
	virtual void  AdjustInsideRectForGripper(class CRect &,int);	//159
	virtual void  BalanceWrap(int,struct SECCustomToolBar::Wrapped *);	//171
public:
	virtual class CSize   CalcDynamicLayout(int,unsigned long);	//181
	virtual class CSize   CalcFixedLayout(int,int);	//184
protected:
	class CSize   CalcLayout(int,unsigned long);	//189
	class CSize   CalcSize(int,struct SECCustomToolBar::Wrapped *);	//191
	int  CanWrapRow(int,int,int,struct SECCustomToolBar::Wrapped *);	//192
public:
	int  CommandToIndex(unsigned int)const;	//203
	int  Create(class CWnd *,unsigned long,unsigned int,char const *);	//222
	virtual int  Create(char const *,char const *,unsigned int,unsigned long,unsigned long,struct tagRECT const &,class CWnd *,struct CCreateContext *);	//223
protected:
	class SECStdBtn *  CreateButton(unsigned int,class SECCustomToolBar *);	//236
public:
	int  CreateEx(unsigned long,class CWnd *,unsigned long,unsigned int,char const *);	//244
protected:
	class SECStdBtn *  CreateSeparator(class SECCustomToolBar *);	//279
	virtual void  DelayShow(int);	//293
	virtual void  DoPaint(class CDC *);	//331
	int  DragButton(int,class CPoint);	//335
public:
	virtual void  DrawBorders(class CDC *,class CRect &);	//338
protected:
	void  DropButton(class CRect &,class SECStdBtn *,int);	//380
public:
	void  EnableConfigMode(int);	//388
protected:
	void  FindRow(class CRect &,class CRect &,int &,int &);	//434
public:
	virtual void  GetBarInfoEx(class SECControlBarInfo *);	//456
	unsigned int  GetButtonStyle(int)const;	//467
	int  GetCurBtn(void)const;	//488
protected:
	virtual int  GetDragMode(void)const;	//502
public:
	struct SECBtnDrawData &  GetDrawData(void);	//503
	virtual void  GetInsideRect(class CRect &)const;	//542
	unsigned int  GetItemID(int)const;	//560
	void  GetItemRect(int,struct tagRECT *)const;	//563
	int  IDToBmpIndex(unsigned int);	//757
	void  InformBtns(unsigned int,unsigned int,void *,int);	//761
protected:
	void  InvalidateButton(int);	//791
	int  ItemFromPoint(class CPoint,class CRect &)const;	//832
public:
	int  LoadBitmapA(unsigned int,unsigned int const *,int);	//851
	int  LoadBitmapA(char const *,unsigned int const *,int);	//852
	int  LoadToolBar(unsigned int);	//859
	int  LoadToolBar(char const *);	//860
protected:
	void  NewToolBar(class CPoint,class SECStdBtn const &);	//882
	void  NotifyManager(enum SECCustomToolBar::BarNotifyCode);	//904
public:
	virtual void  OnBarStyleChange(unsigned long,unsigned long);	//944
protected:
	void  OnCancelMode(void);	//951
	void  OnDestroy(void);	//981
	void  OnLButtonDown(unsigned int,class CPoint);	//1050
	void  OnLButtonUp(unsigned int,class CPoint);	//1055
	int  OnMouseActivate(class CWnd *,unsigned int,unsigned int);	//1064
	void  OnMouseMove(unsigned int,class CPoint);	//1066
	long  OnNoDelayHide(unsigned int,long);	//1082
	void  OnShowWindow(int,unsigned int);	//1134
	void  OnSysColorChange(void);	//1145
	void  OnTimer(unsigned int);	//1151
public:
	virtual int  OnToolHitTest(class CPoint,struct tagTOOLINFOA *)const;	//1157
	virtual void  OnUpdateCmdUI(class CFrameWnd *,int);	//1167
protected:
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//1176
	int  PerformWrap(int,struct SECCustomToolBar::Wrapped *);	//1182
	void  RaiseButton(void);	//1213
public:
	int  RemoveButton(int,int);	//1239
protected:
	void  RemoveConfigFocus(int);	//1241
public:
	virtual void  SetBarInfoEx(class SECControlBarInfo *,class CFrameWnd *);	//1311
	void  SetBitmap(HBITMAP);	//1312
	void  SetButtonStyle(int,unsigned int);	//1318
	int  SetButtons(unsigned int const *,int);	//1319
protected:
	void  SetConfigFocus(int,int);	//1331
	void  SetMode(int);	//1390
public:
	void  SetToolBarInfo(HBITMAP,unsigned int *,unsigned int,int,int);	//1441
protected:
	class CSize   SizeToolBar(int,int);	//1455
	int  SmallDrag(class CRect &,int,int,int);	//1456
	class SECCustomToolBar *  ToolBarFromPoint(int,int);	//1477
	void  UpdateButton(int);	//1490

	DECLARE_MESSAGE_MAP()
};

class IMPORT_1C CToolBar7:public SECCustomToolBar
{
DECLARE_DYNCREATE(CToolBar7)

public:
	DWORD buf[0x6A];
	 CToolBar7(void);	//41
	virtual  ~CToolBar7(void);	//101

	DECLARE_MESSAGE_MAP()

protected:
	virtual class CSize   CalcFixedLayout(int,int);	//182
	virtual class CSize   CalcDynamicLayout(int,unsigned long);	//179
	virtual int  AcceptDrop(void)const;	//124
public:
	void  ClearStored(void);	//200
protected:
	void  OnLButtonDown(unsigned int,class CPoint);	//1047
};

struct IMPORT_1C SECBtnDrawData
{
public:
	 SECBtnDrawData(void);	//45
	 ~SECBtnDrawData(void);	//105
protected:
	HBRUSH  CreateDitherBrush(void);	//241
public:
	void  PostDrawButton(void);	//1189
	int  PreDrawButton(class CDC &,HBITMAP,int,class SECCustomToolBar *);	//1196
	void  SysColorChange(void);	//1470
};

class IMPORT_1C CAccelTable7
{
public:
	 CAccelTable7(void);	//1
	 ~CAccelTable7(void);	//68
	int  CreateAcceleratorTableA(struct tagACCEL *,int);	//233
	int  LoadAcceleratorsA(unsigned int);	//848
	int  LoadAcceleratorsA(char const *);	//849
	int  Merge(HACCEL,int);	//871
	int  TranslateAcceleratorA(class CWnd *,struct tagMSG *);	//1481
};


class IMPORT_1C CChooseDirDlg:public CDialog
{
public:
	 CChooseDirDlg(class CWnd *);	//3
	virtual  ~CChooseDirDlg(void);	//70
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//323
	void  OnBrowseOk(void);	//946
	virtual void  OnCancel(void);	//947
	virtual void  OnFolderChange(void);	//1007
	virtual void  OnInitDone(void);	//1029
	virtual void  OnLBSelChangedNotify(unsigned int,unsigned int,unsigned int);	//1038

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CColorComboBox:public CComboBox
{
public:
	 CColorComboBox(void);	//4
	virtual  ~CColorComboBox(void);	//71
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//362
	virtual void  DrawRect(class CDC *,int,class CRect);	//369
	virtual void  MeasureItem(struct tagMEASUREITEMSTRUCT *);	//864
protected:
	HBRUSH  OnCtlColor(class CDC *,class CWnd *,unsigned int);	//975
public:
	virtual void  PreSubclassWindow(void);	//1198
	void  SetColor(unsigned long);	//1324
	void  SetPos(int);	//1409

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CColorListBox:public CListBox
{
public:
	 CColorListBox(void);	//5
	virtual  ~CColorListBox(void);	//72
	void  DoPaint(class CDC *);	//329
	void  DrawBorder(class CDC *,class CRect);	//336
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//363
	virtual void  DrawRect(class CDC *,int,class CRect);	//370
	int  EnableOther(int);	//399
	unsigned long  GetColor(void)const;	//474
	class CRect   GetRect(int);	//673
protected:
	int  OnEraseBkgnd(class CDC *);	//990
	unsigned int  OnGetDlgCode(void);	//1009
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1031
	void  OnLButtonDown(unsigned int,class CPoint);	//1045
	void  OnPaint(void);	//1088
public:
	void  SetColor(unsigned long);	//1325
	void  SetDefaultString(char const *);	//1336
	void  SetPos(int);	//1410
	void  SetSize(class CSize);	//1422
protected:
	static void  StandartDrawRect(class CDC *,int,class CRect,class CColorListBox *);	//1460

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CContainer:public CDocument
{
DECLARE_DYNCREATE(CContainer)

protected:
	 CContainer(void);	//6
public:
	COleStorage* m_pStorage;
	CObArray m_objArray;
	char bufer[32];
	

public:
	virtual  ~CContainer(void);	//73
	virtual void  Serialize(class CArchive &);	//1302

	DECLARE_MESSAGE_MAP()

public:
	virtual int  IsModified(void);	//812
	virtual void  SetModifiedFlag(int);	//1393
	virtual void  DeleteContents(void);	//301
protected:
	virtual int  OnNewDocument(void);	//1076
public:
	virtual int  OnOpenDocument(char const *);	//1086
	virtual int  OnSaveDocument(char const *);	//1112
	virtual void  OnCloseDocument(void);	//960
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//509
	virtual void  ReleaseFile(class CFile *,int);	//1235
protected:
	virtual int  OnLoadInternal(void);	//1061
	virtual int  OnSaveInternal(void);	//1113

public:
	static char const *  GetContentsStreamName(void);	//483

	void  AddTemplate(unsigned int,char const *);	//155
	void  DeletePage(int);	//305
	int  FreePage(int,int);	//445
	int  GetPagePos(char const *)const;	//651
	int  GetPagePos(class CContainerPage const *)const;	//652
	int  GetPagePos(class CDocument const *)const;	//653
	int  GetPagePosFromFileName(char const *)const;	//654
	int  InsertNewPage(unsigned int,int,char const *,char const *);	//784
	int  IsValidType(unsigned int)const;	//826
	void  MovePage(int,int);	//880
	int  ReadPage(int);	//1219
	void  RenameFileName(int,char const *);	//1256
	void  RenamePage(int,char const *);	//1257
	void  SetReadOnlyMode(int);	//1413
	int  WritePage(int);	//1495
protected:
	class CString   GetPagePathName(int)const;	//650
	void  ClearContents(void);	//197
};


class IMPORT_1C CContainerPage:public CObject
{
DECLARE_DYNCREATE(CContainerPage)

public:
	DWORD m_ID;
	char* m_str1;
	char* m_str2;
	char* m_str3;
	CDocument* m_pDoc;

protected:
	 CContainerPage(unsigned int,char const *,char const *,class CDocument *,char const *);	//7
public:
	virtual  ~CContainerPage(void);	//74
	virtual void  Serialize(class CArchive &);	//1303
};


class IMPORT_1C CFontComboBox7
{
public:
	 CFontComboBox7(void);	//10
	virtual  ~CFontComboBox7(void);	//76
	void  AddFont(struct tagENUMLOGFONTA *,unsigned long,char const *);	//138
	virtual int  CompareItem(struct tagCOMPAREITEMSTRUCT *);	//206
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//364
	void  EmptyContents(void);	//386
	static int  EnumFamPrinterCallBack(struct tagENUMLOGFONTA *,struct tagNEWTEXTMETRICEXA *,int,void *);	//409
	static int  EnumFamPrinterCallBackEx(struct tagENUMLOGFONTEXA *,struct tagNEWTEXTMETRICEXA *,int,void *);	//410
	static int  EnumFamScreenCallBack(struct tagENUMLOGFONTA *,struct tagNEWTEXTMETRICEXA *,int,void *);	//411
	static int  EnumFamScreenCallBackEx(struct tagENUMLOGFONTEXA *,struct tagNEWTEXTMETRICEXA *,int,void *);	//412
	void  EnumFontFamiliesExA(class CDC &,unsigned char);	//413
	int  GetFontAttributes(char const *,struct tagLOGFONTA &,int &);	//525
	void  MatchFont(char const *,unsigned char);	//863
	virtual void  MeasureItem(struct tagMEASUREITEMSTRUCT *);	//865
	void  OnDestroy(void);	//976
	virtual void  PreSubclassWindow(void);	//1199

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLayout: public CObject
{
public:
	 CLayout(class CLayout const &);	//11
	 CLayout(unsigned int,class CLayoutItem *,int,unsigned int *,char const *,unsigned int);	//12
	 CLayout(void);	//13
	virtual  ~CLayout(void);	//77
	void  AddItems(unsigned int,class CLayoutItem *);	//141
	int  Attach(class CPtrList *);	//168
	int  Find(char const *)const;	//425
	void  Load(char const *,int);	//845
	void  Reset(class CLayout *);	//1266
	void  SaveState(class CPtrList *);	//1273
	virtual void  Serialize(class CArchive &);	//1304
	void  Store(char const *,int);	//1464
};


class IMPORT_1C CLayoutItem
{
public:
	 CLayoutItem(class CString,unsigned int);	//14
	 CLayoutItem(void);	//15
	virtual  ~CLayoutItem(void);	//78
	void  operator=(class CLayoutItem const &);	//123
	virtual void  Serialize(class CArchive &);	//1305
};


class IMPORT_1C CModule7:public CCmdTarget
{
DECLARE_DYNAMIC(CModule7)

protected:
	 CModule7(void);	//16
public:
	 CModule7(unsigned int);	//17
	virtual  ~CModule7(void);	//81
	virtual int  Attach(class CAppFrame *);	//169
	virtual int  Detach(class CAppFrame *);	//314
	static int  DispatchCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//322
	static class CModule7 *  FromID(unsigned int);	//448
protected:
	void  Register(void);	//1230

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C XPageSite:public IPropertyPageSite
{
public:
 virtual HRESULT STDMETHODCALLTYPE QueryInterface(struct _GUID const &,void * *); //1209
 virtual ULONG STDMETHODCALLTYPE AddRef(void); //149
 virtual ULONG STDMETHODCALLTYPE Release(void); //1234
 virtual HRESULT STDMETHODCALLTYPE GetLocaleID(unsigned long *); //585
 virtual HRESULT STDMETHODCALLTYPE GetPageContainer(struct IUnknown * *); //649
 virtual HRESULT STDMETHODCALLTYPE OnStatusChange(unsigned long); //1142
 virtual HRESULT STDMETHODCALLTYPE TranslateAcceleratorA(MSG __RPC_FAR *pMsg); //1482
};

class IMPORT_1C CMenu7:public CMenu
{
DECLARE_DYNCREATE(CMenu7)

public:
	virtual  ~CMenu7(void);	//80
	void	Add(class CMenu *,int);	//131
	void	FinalRemoveExtra(void);	//424
	int		FindSubMenu(char const *)const;	//436
	void	RemoveExtra(void);	//1244
	void	Sub(class CMenu *);	//1467

	static void		AddSMenu(class CMenu *,class CMenu *);	//150
	static void		AddTempString(long,class CString,int);	//154
	static void		AddToBlackList(char const *);	//156
	static void		CopySMenu(class CMenu *,class CMenu *,unsigned int);	//213
	static void		FreeTempContainer(void);	//446
	static int		GetPosition(class CMenu *,int);	//666
	static class	CString &  GetTempString(long,int);	//729
	static int		IsOwnerDrawFlag(void);	//814
	static void		SetOwnerDraw(class CMenu *,int,int);	//1396
	static void		SetOwnerDrawFlag(int);	//1397
	static void		SubSMenu(class CMenu *,class CMenu *);	//1468
protected:
	static class	CMap<unsigned long,unsigned long,class CString,class CString>  m_ContextStringMap;	//1627
	static int		m_OwnerDraw;	//1628
	static class	CMap<long,long,class CString,class CString>  m_PopUpStringMap;	//1629
	static class	CStringArray  m_blackList;	//1631
	static class	CMap<unsigned long,unsigned long,class CString,class CString>  m_stringMap;	//1635
};

//class IMPORT_1C COlePropertyPageSite:public CDialog,public XPageSite
class IMPORT_1C COlePropertyPageSite:public CDialog//,public XPageSite // АльФ
{
public:
	CTabCtrl m_TabCtrl;
	DWORD m_dwFlag1;
	DWORD m_dwFlag2;

	CArray<void*, void*> m_PropPages; //CArray<struct SPropPage, struct SPropPage> 

	DWORD m_dwFlag5;
	DWORD m_dwFlag6;
	DWORD m_dwFlag7;
	DWORD m_dwFlag8;
	DWORD m_dwFlag9;
	//DWORD m_dwFlag10; //
	XPageSite m_pageSite;
	CMenu7 m_Menu1;
	CMenu7 m_Menu2;

DECLARE_DYNAMIC(COlePropertyPageSite)

public:
	 COlePropertyPageSite(class CWnd *);	//18
	virtual  ~COlePropertyPageSite(void);	//82

	DECLARE_MESSAGE_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	virtual void  DoDataExchange(class CDataExchange *);	//324
	virtual int  OnInitDialog(void);	//1023
	virtual void  OnOK(void);	//1083
	virtual void  OnCancel(void);	//948
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID , void __RPC_FAR *__RPC_FAR *);	//1209
	virtual ULONG STDMETHODCALLTYPE AddRef(void);	//149
	virtual ULONG STDMETHODCALLTYPE Release(void);	//1234
	virtual long  OnStatusChange(unsigned long);	//1142
	virtual long  GetLocaleID(unsigned long *);	//585
	virtual long  GetPageContainer(struct IUnknown * *);	//649
	virtual HRESULT STDMETHODCALLTYPE  TranslateAcceleratorA(MSG __RPC_FAR *);	//1482

public:
	void  AddPropertyPage(struct _GUID const &,struct IUnknown *);	//147
	int  FindPropertyPage(struct _GUID const &);	//433
	void  RemovePropertyPage(struct _GUID const &);	//1251
	void  RemovePropertyPage(struct _GUID const &,struct IUnknown *);	//1252
protected:
	void  OnApply(void);	//924
	void  OnHelp(void);	//1016
	void  OnSelChangePageSite(struct tagNMHDR *,long *);	//1117

//	virtual struct AFX_INTERFACEMAP const *  GetInterfaceMap(void)const;	//546
//	static struct AFX_INTERFACEMAP const *  _GetBaseInterfaceMap(void);	//1528
//	static struct AFX_INTERFACEMAP const  interfaceMap;	//1626

//	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C COleStorage:public CObject
{
DECLARE_DYNAMIC(COleStorage)

public:
	IStorage*		m_pStorage;
	CString			m_strPath;
	COleStorage*	m_pParent;
	CPtrList		m_list1;
	CPtrList		m_list2;
	
	 COleStorage(class COleStorage *);	//19
	virtual  ~COleStorage(void);	//83
	void  Commit(void);	//205
	int  Create(char const *,unsigned long,class CFileException *);	//216
	class COleStreamFile *  CreateStream(char const *,unsigned long,class CFileException *);	//282
	int  DestroyElement(char const *);	//309
	class COleStorage *  FindSubStorage(char const *)const;	//437
	int  Open(char const *,unsigned long,class CFileException *);	//1178
	class COleStreamFile *  OpenStream(char const *,unsigned long,class CFileException *);	//1181
	void  Release(int);	//1232
	void  ReleaseStream(class COleStreamFile *,int);	//1237
	int  RenameElement(char const *,char const *);	//1255
	void  SetParentStorage(class COleStorage *);	//1403

	static class COleStorage *  FindStorage(char const *);	//435
	static int  ReleaseFile(class CFile *,int);	//1236
	static class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//510
};


class IMPORT_1C CPageSetupDialog7
{
public:
	 CPageSetupDialog7(unsigned long,class CWnd *);	//20
	virtual  ~CPageSetupDialog7(void);	//84
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//325
	void  OnTwoPerPage(void);	//1163

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CPictureHolder7:public CObject
{
DECLARE_DYNAMIC(CPictureHolder7)

public:
	struct IPicture *  m_IPicture;
    enum DrawMode {dmStretch = 1, dmOriginalSize = 2, dmKeepingOriginalParity} m_DrawMode;
	char buffer[0x04];

	enum BorderStyle{bsOne, bsTwo, bs3, bs4};
	enum RenderMode {rmOne, rmTwo};

	 CPictureHolder7(class CPictureHolder7 const &);	//23
	 CPictureHolder7(void);	//24
	virtual  ~CPictureHolder7(void);	//87
	void  AdjustRect(class CRect &)const;	//160
	class CPictureHolder7   Clone(void);	//201
	class CPictureHolder7 &  Copy(class CPictureHolder7 const &);	//211
	int  CreateEmpty(void);	//243
	int  CreateFromBitmap(HBITMAP,HPALETTE);	//246
	int  CreateFromBitmap(HINSTANCE,char *);	//247
	int  CreateFromDataObject(class COleDataObject *);	//248
	int  CreateFromFile(char const *);	//249
	int  CreateFromIcon(HICON);	//250
	int  CreateFromIcon(HINSTANCE,char *);	//251
	int  CreateFromMetafile(HENHMETAFILE);	//252
	struct IPictureDisp *  GetDispatch(void)const;	//494
	struct IPicture *  GetPicture(void)const;	//664
	class CSize   GetSize(void)const;	//715
	short  GetType(void)const;	//745
	int  IsEmpty(void)const;	//801
	virtual void  Render(HDC,struct tagRECT const &,int,struct tagRECT const *)const;	//1261
	virtual void  Serialize(class CArchive &);	//1306
	void  SetDispatch(struct IPictureDisp *);	//1339
	int  SetToDataSource(class COleDataSource *,int)const;	//1439
};


class IMPORT_1C CPictureButton7:public CButton
{
DECLARE_DYNAMIC(CPictureButton7)

public:
	enum DrawState {dsOne, dsTwo};
	 CPictureButton7(void);	//21
	virtual  ~CPictureButton7(void);	//85
	int  Create(char const *,unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//217
protected:
	HBRUSH  CtlColor(class CDC *,unsigned int);	//288
	void  DrawDisabled(HDC,int,int);	//345
public:
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//365
protected:
	void  DrawToDC(class CDC *,enum CPictureButton7::DrawState,class CRect const &);	//378
public:
	class CPictureHolder7 const &  GetPicture(void)const;	//663
protected:
	long  OnBMClick(unsigned int,long);	//925
	long  OnBMGetCheck(unsigned int,long);	//926
	long  OnBMGetImage(unsigned int,long);	//927
	long  OnBMGetState(unsigned int,long);	//928
	long  OnBMSetCheck(unsigned int,long);	//929
	long  OnBMSetImage(unsigned int,long);	//930
	long  OnBMSetState(unsigned int,long);	//931
	long  OnBMSetStyle(unsigned int,long);	//932
	int  OnClicked(void);	//957
	int  OnDoubleclicked(void);	//986
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1039
	long  OnPrintClient(unsigned int,long);	//1098
	virtual void  PostNcDestroy(void);	//1190
	virtual void  PreSubclassWindow(void);	//1200
public:
	void  SetBorderStyle(enum CPictureHolder7::BorderStyle);	//1314
	void  SetPicture(class CPictureHolder7 const &);	//1404
	void  SetPicture(HBITMAP,enum CPictureHolder7::RenderMode);	//1405
	void  SetPicture(HENHMETAFILE,enum CPictureHolder7::RenderMode);	//1406
	void  SetPicture(HICON,enum CPictureHolder7::RenderMode);	//1407

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CPictureDialog7:public CFileDialog
{
DECLARE_DYNAMIC(CPictureDialog7)

public:
	 CPictureDialog7(int);	//22
	virtual  ~CPictureDialog7(void);	//86
protected:
	void  OnClicked(void);	//958
	void  OnDestroy(void);	//977
	virtual void  OnFileNameChange(void);	//1001
	virtual void  OnInitDone(void);	//1030

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CPopupFrame:public CFrameWnd
{
DECLARE_DYNCREATE(CPopupFrame)

protected:
	 CPopupFrame(void);	//25
	virtual  ~CPopupFrame(void);	//88
public:
	virtual int  Create(char const *,char const *,unsigned long,struct tagRECT const &,struct CCreateContext *);	//218
protected:
	long  OnFloatStatus(unsigned int,long);	//1004
public:
	virtual void  OnUpdateFrameTitle(int);	//1172
	virtual int  PreTranslateMessage(struct tagMSG *);	//1201

	DECLARE_MESSAGE_MAP()
};

struct SHotStruct
{
	tagACCEL m_ACCEL;
	CString m_LongText;
	CString m_ShortText;
};

typedef CArray<SHotStruct, SHotStruct> SHotStructArray;

class IMPORT_1C CUserCommandContaner: public SHotStructArray
{
public:
	//CArray<SHotStruct, SHotStruct> m_Array;
//	SHotStruct* m_Array;
//	int m_nSize;
//	int m_nMaxSize;
//	int m_nGrowBy;
	CString m_Description;

	 CUserCommandContaner(void);	//107
	virtual  ~CUserCommandContaner(void);	//204
	void  AddCommands(class CUserCommandContaner *);	//341
	class CUserCommandInfo *  Find(unsigned int);	//640
	class CUserCommandInfo *  Find(char const *,char const *);	//641
	int  GetAccel(unsigned int,struct tagACCEL &)const;	//661
	int  GetAccelPresent(unsigned int,class CString &)const;	//663
	int  GetAccelPresent(unsigned int,char *,int)const;	//664
	void  GetAccelTable(struct tagACCEL * *,int &)const;	//665
	virtual class CUserCommandInfo *  GetCommand(char const *,char const *,int,class CDocument *);	//683
	int  GetCount(void)const;	//696
	class CString   GetDescription(void);	//719
	int  GetLongText(unsigned int,class CString &)const;	//777
	int  GetLongText(unsigned int,char *,int)const;	//778
	int  GetParamCode(unsigned int,class CString &,int)const;	//947
	int  GetParamCode(unsigned int,char *,int,int)const;	//948
	int  GetRealCode(unsigned int,class CString &,int)const;	//1026
	int  GetRealCode(unsigned int,char *,int,int)const;	//1027
	int  GetShortText(unsigned int,class CString &,int)const;	//1091
	int  GetShortText(unsigned int,char *,int,int)const;	//1092
protected:
	unsigned int  GetUnCode(void)const;	//1128
public:
	int  Has(unsigned int)const;	//1148
	int  IsAccelUsed(struct tagACCEL)const;	//1201
	int  IsValidProject(unsigned int)const;	//1285
protected:
	virtual int  RemoveCommand(char const *);	//1671
public:
	virtual int  RemoveCommand(unsigned int);	//1672
	virtual void  Serialize(class CArchive &,int);	//1728
	void  SetAccel(struct tagACCEL);	//1730
	void  SetDescription(class CString);	//1751
	void  SetStatusStr(unsigned int,class CString &)const;	//1812
	void  SetStatusStr(unsigned int,char *,int)const;	//1813
	void  Test(void);	//1858
	void  TestAcc(unsigned int);	//1859
};

struct SPropStruct
{
	DWORD m_Type;
	DWORD m_1;
	CString m_Name;
	CString m_Value;
	DWORD m_4;
};

class IMPORT_1C CProfile7
{
public:
	CString m_RegPath;
	DWORD m_1;
	CPtrList m_PropList;
	CItemList* m_pItemList;
	CString m_RegPath2;
	CStringArray m_PathArray;
	CArray<SPropStruct*, SPropStruct*> m_PropArray;
	CUserCommandContaner m_UserCommandContaner;

	DWORD buffer[0x50];

	 CProfile7(char const *,struct CProfileEntry7 const *);	//26
	virtual  ~CProfile7(void);	//89
	int  AddEntry(struct CProfileEntry7 const *);	//137
	virtual void  Attach(class CProfile7 *);	//170
protected:
	void  ConstructProp(int);	//207
	void  DestructProp(int);	//313
public:
	virtual void  Detach(class CProfile7 *);	//315
	int  FindEntry(char const *)const;	//427
	int  FindEntry(struct CProfileEntry7 const *)const;	//428
	class CProfile7 *  FindProfile(char const *);	//432
	class CNumeric const &  GetCNumericProp(int)const;	//468
protected:
	class CItemList *  GetChildList(class CItemList *,char const *);	//473
public:
	unsigned long  GetColorProp(int)const;	//475
	class CDate   GetDateProp(int)const;	//489
	double  GetDoubleProp(int)const;	//501
	struct CProfileEntry7 const *  GetEntryAt(int)const;	//507
	int  GetEntryCount(void)const;	//508
	struct __POSITION *  GetFirstProfile(void);	//519
	class CFont *  GetFontProp(int)const;	//527
	HFONT  GetHFONTProp(int)const;	//529
	int  GetIntProp(int)const;	//545
	char const *  GetKey(void)const;	//574
	int  GetLOGFONTProp(int,struct tagLOGFONTA &)const;	//575
	long  GetLongProp(int)const;	//587
	class CProfile7 *  GetNextProfile(struct __POSITION * &);	//638
	class CProfile7 *  GetParentProfile(void);	//662
	void *  GetPointerProp(int)const;	//665
	class CString const &  GetStringProp(int)const;	//718
protected:
	class CItem *  GetValueItem(class CItemList *,struct CProfileEntry7 const *);	//746
	static class CString   LOGFONTtoString(struct tagLOGFONTA const &);	//842
	void  Load(class CItemList *,char const *);	//846
public:
	int  Load(char const *,char const *);	//847
protected:
	static class CString   ProfileGetString(char const *,char const *,char const *);	//1205
	static int  ProfileWriteString(char const *,char const *,char const *);	//1206
	void  PropFromString(int,char const *);	//1207
	void  PropToString(int,class CString &);	//1208
public:
	void  RemoveEntry(int);	//1242
	void  RemoveEntry(struct CProfileEntry7 const *);	//1243
	void  Reset(void);	//1267
protected:
	void  Save(class CItemList *,char const *);	//1270
public:
	int  Save(char const *,char const *);	//1271
	void  Serialize(class CArchive &);	//1307
	void  SetCNumericProp(int,class CNumeric const &);	//1320
	void  SetColorProp(int,unsigned long);	//1326
	void  SetDateProp(int,class CDate);	//1334
	void  SetDoubleProp(int,double);	//1341
	void  SetFontProp(int,class CFont *);	//1350
	void  SetHFONTProp(int,HFONT);	//1353
	void  SetIntProp(int,int);	//1360
	void  SetKey(char const *);	//1379
	void  SetLOGFONTProp(int,struct tagLOGFONTA const &);	//1380
	void  SetLongProp(int,long);	//1386
	void  SetPointerProp(int,void *);	//1408
	void  SetStringProp(int,char const *);	//1424
protected:
	static int  StringtoLOGFONT(char const *,struct tagLOGFONTA &);	//1466
};


class IMPORT_1C CPropertyBox:public CPropertySheet
{
DECLARE_DYNAMIC(CPropertyBox)

public:
	 CPropertyBox(unsigned int,class CWnd *,unsigned int);	//27
	 CPropertyBox(char const *,class CWnd *,unsigned int);	//28
	virtual  ~CPropertyBox(void);	//90
	int  ApplayAndValidate(void);	//166
	void  CreateAllPage(int);	//234
	virtual int  DestroyWindow(void);	//312
protected:
	void  EnableBox(int);	//387
public:
	static class CPropertyBox *  GetPropertyBox(void);	//672
	void  Hide(void);	//751
protected:
	void  OnActivate(unsigned int,class CWnd *,int);	//916
	void  OnApplayClicked(void);	//923
	void  OnCancelClicked(void);	//950
	void  OnCheckClicked(void);	//952
	int  OnEraseBkgnd(class CDC *);	//992
	long  OnFloatStatus(unsigned int,long);	//1005
	long  OnIdleUpdateCmdUI(unsigned int,long);	//1022
public:
	virtual int  OnInitDialog(void);	//1024
protected:
	void  OnOkClicked(void);	//1085
	long  OnPostValidate(unsigned int,long);	//1096
	virtual void  PostNcDestroy(void);	//1191
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//1202
	void  RemoveAll(void);	//1238
	static void  RemovePropertyBox(void);	//1250
protected:
	long  SendNotifyMessageA(unsigned int,long);	//1301
public:
	void  SetSchema(class CPropertySchema *);	//1415
	void  Show(int);	//1451

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CPropertySchema
{
public:
	 CPropertySchema(int);	//29
	virtual  ~CPropertySchema(void);	//91
	void  AddPage(class CPropertyPage *);	//146
	int  GetIndex(char const *);	//539
	char const *  GetTitle(int);	//733
	char const *  GetTitle(class CPropertyPage *);	//734
};


class IMPORT_1C CPropertySheet7:public CPropertySheet
{
DECLARE_DYNAMIC(CPropertySheet7)

public:
	 CPropertySheet7(unsigned int,class CWnd *,unsigned int);	//30
	 CPropertySheet7(char const *,class CWnd *,unsigned int);	//31
	virtual  ~CPropertySheet7(void);	//92
protected:
	virtual int  OnInitDialog(void);	//1025

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSearchBox
{
public:
	 CSearchBox(int);	//32
	virtual  ~CSearchBox(void);	//93
	void  CreateHistory(char const *);	//253
protected:
	int  IsHistoryActual(void);	//809
public:
	void  LoadFromHisrory(void);	//854
protected:
	int  OnCreate(struct tagCREATESTRUCTA *);	//971
	void  OnDestroy(void);	//978
	void  OnKillfocus(void);	//1037
	long  OnSetText(unsigned int,long);	//1128
	void  OnSetfocus(void);	//1131
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//1203
protected:
	void  SaveToHistory(void);	//1276
public:
	void  SetCommand(unsigned int,int);	//1330
	void  SetSharedHistory(class CSearchHistoryProfile *);	//1420
protected:
	void  SmartInsertString(char const *);	//1457
public:
	static class CFont  searchFont;	//1679

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSearchHistoryProfile
{
public:
	 CSearchHistoryProfile(char const *);	//33
	virtual  ~CSearchHistoryProfile(void);	//94
	void  AddRef(void);	//148
	void  Release(void);	//1233
protected:
	static struct CProfileEntry7 *  s_Entries;	//1678
};


class IMPORT_1C CSerialBitmap:public CBitmap //CObject
{
public:
	DWORD bufSerialBitmap[0x07];
	 CSerialBitmap(int);	//34
	 CSerialBitmap(class CBitmap *);	//35
	virtual  ~CSerialBitmap(void);	//95
	virtual void  Serialize(class CArchive &);	//1308

public:
	int  AddImage(int,class CSerialBitmap &,class CSize);	//139
	int  AddImage(int,class CBitmap *);	//140
	void  Clear(void);	//196
	void  CreateCopy(class CBitmap *);	//239
	void  CreateSerBitmap(int,class CSize);	//281
	class CSize   GetFullSize(void)const;	//528
	int  GetImage(int,class CSerialBitmap &,int);	//532
	int  GetImage(int,int,class CSerialBitmap &,int);	//533
	int  GetImage(int,int,class CBitmap *);	//534
	int  GetImage(int,class CBitmap *);	//535
	class CSize   GetItemCount(void)const;	//553
	class CSize   GetSizeItem(void)const;	//716
	int  IsEdit(void)const;	//800
	int  LoadFromFile(char const *);	//853
	int  RemoveImage(int);	//1246
	int  ReplaceImage(int,class CSerialBitmap &,int);	//1262
	int  ReplaceImage(int,int,class CSerialBitmap &,int,int);	//1263
	int  ReplaceImage(int,int,class CBitmap *);	//1264
	int  ReplaceImage(int,class CBitmap *);	//1265
	int  SetEdit(int);	//1342
	void  SetSizeItem(class CSize);	//1423
	int  StoreToFile(char const *);	//1465
protected:
	void  CorrectRect(class CRect *);	//214
	void  CreateSerBitmap(struct tagBITMAPINFO *);	//280
};


class IMPORT_1C CSizeComboBox7
{
public:
	 CSizeComboBox7(void);	//36
	virtual  ~CSizeComboBox7(void);	//96
	void  EnumFontSizes(class CDC &,char const *);	//414
	static int  EnumSizeCallBack(struct tagLOGFONTA *,struct tagNEWTEXTMETRICA *,int,void *);	//415
	int  GetTwipSize(void);	//744
	void  InsertSize(int);	//785
	void  SetTwipSize(int);	//1448
	void  TwipsToPointString(char *,int);	//1483

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTabCtl7:public CTabCtrl
{
public:
	 CTabCtl7(void);	//39
	virtual  ~CTabCtl7(void);	//99
protected:
	void  OnPaint(void);	//1090
	void  OnSelchanging(struct tagNMHDR *,long *);	//1118
	long  OnSetCurSel(unsigned int,long);	//1121

	DECLARE_MESSAGE_MAP()
};

class IMPORT_1C CUnknown_1 : public CObject
{
    DWORD buf[6];
};

class IMPORT_1C CTemplate7:public CMultiDocTemplate
{
DECLARE_DYNAMIC(CTemplate7)
    CUnknown_1 unknown;

public:
	 CTemplate7(unsigned int aMenuResID,struct CRuntimeClass * aDocument,struct CRuntimeClass * aFrame,struct CRuntimeClass * aView);	//40
	virtual  ~CTemplate7(void);	//100
	static class CTemplate7 *  FromDocString(char const *,enum CDocTemplate::DocStringIndex);	//447
	static class CTemplate7 *  FromID(unsigned int);	//449
	static class CDocument *  GetDocument(unsigned int);	//497
	static unsigned int  GetDocumentID(class CDocument *);	//498
	static unsigned int  GetDocumentTemplateID(unsigned int);	//499
	static unsigned int  GetDocumentTemplateID(class CDocument *);	//500
	static unsigned int  GetParent(unsigned int);	//659
	virtual class CDocument *  OpenDocumentFile(char const *,int);	//1179
	class CDocument *  OpenDocumentFileEx(char const *,long,int);	//1180
	static unsigned int  RegisterDocument(class CDocument *);	//1231
	static long  SendMessage7(unsigned int,unsigned int,unsigned int,unsigned int,long);	//1296
	static long  SendMessage7(unsigned int,class CMessage7 *);	//1297
	static long  SendMessage7Direct(class CCmdTarget *,unsigned int,unsigned int,unsigned int,long);	//1298
	static long  SendMessage7Direct(class CCmdTarget *,class CMessage7 *);	//1299
	static void  SetParent(unsigned int,unsigned int);	//1402
	static void  UnRegisterDocument(unsigned int);	//1485
	static void  UnRegisterDocument(class CDocument *);	//1486

	//----------
	CRuntimeClass* GetFrameClass() {return m_pFrameClass;};
	CString GetFrameClassName() {if(m_pFrameClass) return m_pFrameClass->m_lpszClassName; return "";};
	CRuntimeClass* GetViewClass() {return m_pViewClass;};
	CString GetViewClassName() {if(m_pViewClass) return m_pViewClass->m_lpszClassName; return "";};
	CRuntimeClass* GetDocClass() {return m_pDocClass;};
	CString GetDocClassName() {if(m_pDocClass) return m_pDocClass->m_lpszClassName; return "";};
	
};


class IMPORT_1C SECListCtrl:public CWnd
{
DECLARE_DYNCREATE(SECListCtrl)

public:
	enum SelectAction {saOne, saTwo};
	class LvPaintContext {
	};
	class Item {
	};
	class Column {
	};
	 SECListCtrl(void);	//49
	virtual  ~SECListCtrl(void);	//109
	virtual int  AddColumn(class CString const &,int,int,int);	//135
	virtual int  AddColumn(class SECListCtrl::Column *);	//136
protected:
	int  AddToSelItemsArray(int);	//157
	virtual int  AllocateSubItems(class SECListCtrl::Item *)const;	//165
public:
	virtual int  AreToolTipsEnabled(void)const;	//167
	virtual int  CalcItemsInRange(int,int,int);	//188
	virtual int  CalcRangeHeight(int,int,int);	//190
	virtual int  ColumnExists(class CString const &)const;	//202
	virtual int  Create(unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//224
	virtual int  Create(unsigned long,unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//225
protected:
	virtual class SECListCtrl::Item *  CreateNewItem(void);	//256
	virtual class SECListCtrl::LvPaintContext *  CreatePaintContext(class CDC *);	//275
	virtual int  CreateScrollBarCtrl(unsigned long,unsigned int);	//277
	virtual int  DeallocateSubItems(class SECListCtrl::Item *)const;	//289
public:
	virtual int  DeleteAllColumns(void);	//295
	virtual int  DeleteAllItems(void);	//296
	virtual int  DeleteColumn(class CString const &);	//299
	virtual int  DeleteColumn(int);	//300
	virtual int  DeleteItem(int,int);	//302
	virtual int  DeselectAllItems(int);	//308
protected:
	virtual void  DrawColumnLines(class CDC *,int,class CRect const &);	//343
	virtual void  DrawHeader(class SECListCtrl::LvPaintContext *);	//356
	virtual void  DrawInvalidItems(class SECListCtrl::LvPaintContext *);	//361
	virtual void  DrawItem(class SECListCtrl::LvPaintContext *);	//366
	virtual void  DrawItemLines(class CDC *,int,class CRect const &);	//368
	virtual void  DrawSubItem(class SECListCtrl::LvPaintContext *);	//371
public:
	virtual class CEdit *  EditLabel(int,int);	//382
protected:
	void  EditLabel_OnKillFocus(void);	//384
	void  EditLabel_OnUpdate(void);	//385
public:
	virtual void  EnableHeaderCtrl(int);	//395
	virtual void  EnableMultiSelect(int);	//398
	virtual void  EnableToolTips(int);	//403
	virtual void  EnableWordWrap(int);	//405
	virtual int  EnsureRangeVisible(int,int,int);	//406
	virtual int  EnsureVisible(int,int);	//407
	virtual int  FindColumn(class CString const &,int &,int &)const;	//426
protected:
	int  FindInSelItemsArray(int,int &)const;	//429
public:
	virtual unsigned long  GetBkColor(void)const;	//458
	virtual unsigned int  GetCallbackMask(void)const;	//469
	virtual unsigned int  GetCallbackMaskEx(void)const;	//470
	virtual class SECListCtrl::Column *  GetColumnAt(int)const;	//476
	virtual int  GetColumnCount(void)const;	//477
	virtual int  GetColumnFormat(int)const;	//478
	virtual class CString   GetColumnHeading(int)const;	//479
	virtual int  GetColumnLeft(int)const;	//480
	virtual int  GetColumnRight(int)const;	//481
	virtual int  GetColumnWidth(int)const;	//482
	virtual unsigned long  GetDisabledTextColor(void)const;	//493
	virtual int  GetDropHilightIndex(void)const;	//504
	virtual class CEdit *  GetEditControl(void);	//506
	virtual int  GetFirstItemLastPage(void)const;	//516
	virtual int  GetFirstItemNextPage(void)const;	//517
	virtual int  GetFirstItemPrevPage(void);	//518
	virtual int  GetFocusIndex(void)const;	//523
	virtual class CFont *  GetHeaderFont(void)const;	//530
	virtual unsigned long  GetIconBkColor(void)const;	//531
	virtual class CImageList *  GetImageList(unsigned int)const;	//536
	virtual void  GetInsideRect(struct tagRECT *)const;	//543
	virtual int  GetItem(struct tagLVITEMA *,int,int)const;	//547
	class SECListCtrl::Item *  GetItemAt(int)const;	//550
	virtual int  GetItemAtPoint(class CPoint const &);	//552
	virtual int  GetItemCount(void)const;	//554
	virtual unsigned long  GetItemData(int)const;	//555
	virtual int  GetItemEx(struct tagLVITEMA *,struct tagLVITEMEX *,int)const;	//557
	virtual int  GetItemRect(int,int,struct tagRECT *,unsigned int)const;	//564
	virtual int  GetItemRect(int,struct tagRECT *,unsigned int)const;	//565
	virtual unsigned int  GetItemState(int,unsigned int)const;	//568
	virtual class CString   GetItemText(int,int)const;	//571
	virtual int  GetItemText(int,int,char *,int)const;	//572
	virtual class CFont *  GetLabelFont(void)const;	//576
	virtual int  GetLastFullyVisibleItem(void);	//579
	virtual unsigned long  GetListCtrlStyle(void)const;	//582
	virtual unsigned long  GetListCtrlStyleEx(void)const;	//583
	virtual int  GetListCtrlStyles(unsigned long &,unsigned long &)const;	//584
	virtual class CSize   GetLogicalSize(void);	//586
	virtual int  GetNextItem(int,int)const;	//635
	virtual int  GetOrigin(struct tagPOINT *)const;	//648
	virtual class CScrollBar *  GetScrollBarCtrl(int)const;	//709
	virtual unsigned long  GetSelIconBkColor(void)const;	//710
	virtual unsigned long  GetSelTextBkColor(void)const;	//711
	virtual unsigned long  GetSelTextColor(void)const;	//712
	virtual unsigned int  GetSelectedCount(void)const;	//713
	virtual int  GetSubItemCount(void)const;	//719
	virtual unsigned long  GetTextBkColor(void)const;	//730
	virtual unsigned long  GetTextColor(void)const;	//731
	virtual unsigned long  GetToolTipBkColor(void)const;	//738
	virtual unsigned long  GetToolTipTextColor(void)const;	//739
	virtual int  GetTopIndex(void)const;	//740
	virtual int  HitTest(class CPoint,unsigned int *,int &)const;	//754
protected:
	virtual int  InitNewItem(class SECListCtrl::Item *)const;	//770
public:
	virtual int  InsertColumn(int,class CString const &,int,int,int,int);	//776
	virtual int  InsertColumn(int,class SECListCtrl::Column *,int);	//777
	virtual int  InsertItem(int,class CString const &);	//778
	virtual int  InsertItem(int,class SECListCtrl::Item const *);	//779
	virtual int  IsCallbackItem(int)const;	//795
	virtual int  IsHeaderCtrlEnabled(void)const;	//805
	virtual int  IsMultiSelectEnabled(void)const;	//813
	virtual int  IsPaddingMode(void)const;	//815
	virtual int  IsSameHeightMode(void)const;	//816
	virtual int  IsSelected(int)const;	//817
	virtual int  IsShowGridMode(void)const;	//819
	virtual int  IsShowHeader(void)const;	//820
	virtual int  IsWordWrapEnabled(void)const;	//830
	virtual int  ItemsPrevPage(void);	//836
	virtual int  ItemsThisPage(void);	//837
	virtual int  KillEditControl(void);	//838
protected:
	virtual int  LoadPaintContext(class SECListCtrl::LvPaintContext *);	//855
	virtual void  MeasureItem(int);	//866
	virtual void  MeasureItem(class SECListCtrl::LvPaintContext *);	//867
	virtual void  MeasureSubItem(class SECListCtrl::LvPaintContext *,unsigned int &);	//868
public:
	virtual int  ModifyListCtrlStyle(unsigned long,unsigned long,int);	//873
	virtual int  ModifyListCtrlStyleEx(unsigned long,unsigned long,int);	//874
	virtual int  ModifyListCtrlStyles(unsigned long,unsigned long,unsigned long,unsigned long,int);	//875
protected:
	virtual void  NotifyBeginDrag(int,struct tagPOINT)const;	//885
	virtual int  NotifyBeginLabelEdit(int)const;	//887
	virtual void  NotifyBeginRDrag(int,struct tagPOINT)const;	//889
	virtual void  NotifyDeleteItem(int)const;	//891
	virtual int  NotifyEndLabelEdit(int,int,class CString &,int)const;	//893
	virtual void  NotifyGetDispInfo(struct tagLVITEMA *)const;	//895
	virtual void  NotifyGetDispInfoEx(struct tagLVITEMEX *)const;	//897
	virtual void  NotifyInsertItem(int)const;	//899
	virtual void  NotifyKeyDown(unsigned short)const;	//902
	virtual void  NotifyNMHDR(unsigned int)const;	//905
	virtual void  NotifySelChanged(int,int,enum SECListCtrl::SelectAction)const;	//907
	virtual int  NotifySelChanging(int,int,enum SECListCtrl::SelectAction)const;	//909
	virtual void  NotifySetDispInfo(struct tagLVITEMA const *)const;	//911
	virtual void  NotifySetDispInfoEx(struct tagLVITEMEX const *)const;	//913
	virtual void  OnBackKeyPressed(int,int,int,int,unsigned int,unsigned int);	//933
	int  OnCreate(struct tagCREATESTRUCTA *);	//972
	void  OnDestroy(void);	//982
	virtual void  OnDownKeyPressed(int,int,int,int,unsigned int,unsigned int);	//987
	virtual void  OnEndKeyPressed(int,int,int,int,unsigned int,unsigned int);	//988
	int  OnEraseBkgnd(class CDC *);	//996
	virtual void  OnF2KeyPressed(int,int,int,int,unsigned int,unsigned int);	//1000
	unsigned int  OnGetDlgCode(void);	//1010
	void  OnHScroll(unsigned int,unsigned int,class CScrollBar *);	//1015
	virtual void  OnHomeKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1019
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1032
	void  OnKillFocus(class CWnd *);	//1036
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1043
	void  OnLButtonDown(unsigned int,class CPoint);	//1051
	void  OnLButtonUp(unsigned int,class CPoint);	//1056
	virtual void  OnLeftKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1059
	void  OnMouseMove(unsigned int,class CPoint);	//1067
	void  OnPaint(void);	//1093
	void  OnRButtonDown(unsigned int,class CPoint);	//1101
	void  OnRButtonUp(unsigned int,class CPoint);	//1103
	virtual void  OnReturnKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1108
	virtual void  OnRightKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1110
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//1123
	void  OnSetFocus(class CWnd *);	//1125
	void  OnSize(unsigned int,int,int);	//1140
	void  OnTimer(unsigned int);	//1152
	virtual void  OnUpKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1164
	void  OnVScroll(unsigned int,unsigned int,class CScrollBar *);	//1174
	virtual void  PickColumnLinePen(class CPen * &);	//1183
	virtual void  PickIconBkgnd(class SECListCtrl::LvPaintContext *);	//1184
	virtual void  PickItemBkgnd(class SECListCtrl::LvPaintContext *);	//1185
	virtual void  PickItemLinePen(class CPen * &);	//1186
	virtual void  PickTextColors(class SECListCtrl::LvPaintContext *);	//1187
	virtual void  PickTextFont(class SECListCtrl::LvPaintContext *);	//1188
	virtual int  ProcessAndDestroyEditLabel(int);	//1204
public:
	virtual void  ReMeasureAllItems(void);	//1218
protected:
	virtual void  RecalcLayout(void);	//1223
	virtual void  RecalcScrollBars(void);	//1227
public:
	virtual int  RedrawItems(int,int);	//1228
protected:
	int  RemoveFromSelItemsArray(int);	//1245
public:
	virtual void  ScrollHorz(int);	//1278
	virtual void  ScrollVert(int,int);	//1282
	virtual int  SelectDropTarget(int);	//1287
	virtual int  SelectFocusItem(int,int);	//1289
	virtual int  SelectItem(int,int);	//1290
	virtual int  SelectItemRange(int,int,int);	//1292
	virtual int  SetBkColor(unsigned long);	//1313
	virtual int  SetCallbackMask(unsigned int);	//1321
	virtual int  SetCallbackMaskEx(unsigned int);	//1322
	virtual void  SetColumnFormat(int,int);	//1327
	virtual void  SetColumnHeading(int,class CString const &);	//1328
	virtual void  SetColumnWidth(int,int);	//1329
	virtual int  SetDisabledTextColor(unsigned long);	//1338
	virtual void  SetFirstVisible(int);	//1346
	virtual int  SetHeaderFont(class CFont *);	//1354
	virtual int  SetIconBkColor(unsigned long);	//1355
	virtual class CImageList *  SetImageList(class CImageList *,int);	//1356
	virtual int  SetItem(int,int,unsigned int,char const *,int,unsigned int,unsigned int,long);	//1361
	virtual int  SetItem(struct tagLVITEMA const *);	//1362
	virtual int  SetItemData(int,unsigned long);	//1366
	virtual int  SetItemEx(struct tagLVITEMA *,struct tagLVITEMEX const *);	//1368
	virtual int  SetItemHeight(int,unsigned int)const;	//1371
	virtual int  SetItemState(int,unsigned int,unsigned int);	//1373
	virtual int  SetItemText(int,int,char const *);	//1376
	virtual int  SetLabelFont(class CFont *);	//1381
	virtual int  SetListCtrlStyle(unsigned long,int);	//1383
	virtual int  SetListCtrlStyleEx(unsigned long,int);	//1384
	virtual int  SetListCtrlStyles(unsigned long,unsigned long,int);	//1385
	virtual void  SetPaddingMode(int);	//1398
	virtual void  SetSameHeightMode(int);	//1414
	virtual int  SetSelIconBkColor(unsigned long);	//1417
	virtual int  SetSelTextBkColor(unsigned long);	//1418
	virtual int  SetSelTextColor(unsigned long);	//1419
	virtual void  SetShowGridMode(int);	//1421
	virtual int  SetTextBkColor(unsigned long);	//1436
	virtual int  SetTextColor(unsigned long);	//1437
	virtual int  SetToolTipBkColor(unsigned long);	//1443
	virtual int  SetToolTipTextColor(unsigned long);	//1444
	virtual void  ShowHeader(int);	//1453
protected:
	virtual void  ShowScrollBarCtrl(int,int,struct tagSCROLLINFO *);	//1454
	virtual int  TrackColumnResize(class CPoint,int);	//1480
public:
	virtual int  Update(int,int,int,int);	//1487

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECTreeCtrl:public SECListCtrl
{
DECLARE_DYNCREATE(SECTreeCtrl)

public:
	class Node {
	public:
		Node(void);	//42
		virtual ~Node(void);	//102
		virtual void  Expand(int);	//418
		int  IsDisabled(void)const;	//798
		class Node *  GetNode(struct _TREEITEM *)const;	//644
		class Node *  GetNode(class SECListCtrl::Item *)const;	//645
		class Node *  GetNodeAt(int)const;	//646
		virtual int  IsExpanded(void)const;	//802
		int IsHidden(void)const;	//806
		int IsVisible(void);	//827
		virtual int OnNextSearchNode(unsigned short,class SECTreeNode *);	//1079
	};
	 SECTreeCtrl(void);	//66
	virtual  ~SECTreeCtrl(void);	//121
protected:
	void  AddChildrenToListCtrl(class SECTreeCtrl::Node *,int &);	//134
	int  AddNodeToListBox(class SECTreeCtrl::Node *);	//143
	void  CalcButtonRect(class CRect *,int,class CRect const &)const;	//178
public:
	virtual int  Create(unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//231
	virtual int  Create(unsigned long,unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//232
	class CImageList *  CreateDragImage(struct _TREEITEM *);	//242
protected:
	virtual class SECListCtrl::LvPaintContext *  CreatePaintContext(class CDC *);	//276
	int  DelNodeFromListBox(class SECTreeCtrl::Node *);	//292
public:
	virtual int  DeleteAllItems(void);	//297
	int  DeleteItem(struct _TREEITEM *);	//303
protected:
	int  DeleteUnChildedItem(struct _TREEITEM *,int);	//307
public:
	int  DisableAllItems(int);	//319
	int  DisableItem(struct _TREEITEM *,int);	//320
protected:
	virtual void  DrawButton(class SECListCtrl::LvPaintContext *);	//340
	virtual void  DrawHierarchyLines(class SECListCtrl::LvPaintContext *);	//357
	virtual void  DrawHorzHierarchyLine(class CDC *,int,int,int,int);	//358
	virtual void  DrawSubItem(class SECListCtrl::LvPaintContext *);	//372
	virtual void  DrawVertHierarchyLine(class CDC *,int,int,int,int);	//379
public:
	class CEdit *  EditLabel(struct _TREEITEM *);	//383
	int  EnsureVisible(struct _TREEITEM *,int);	//408
	int  Expand(struct _TREEITEM *,unsigned int,int);	//419
protected:
	struct _TREEITEM *  ExpandSelectedItem(unsigned int);	//421
	int  FilterListBox(void);	//423
public:
	struct _TREEITEM *  GetCaretItem(void)const;	//471
	struct _TREEITEM *  GetChildItem(struct _TREEITEM *)const;	//472
	unsigned int  GetCount(void)const;	//486
	struct _TREEITEM *  GetDropHilightItem(void)const;	//505
	unsigned short  GetFilterLevel(void)const;	//512
	struct _TREEITEM *  GetFirstDisabledItem(void)const;	//514
	struct _TREEITEM *  GetFirstHiddenItem(void)const;	//515
	struct _TREEITEM *  GetFirstSelectedItem(void)const;	//520
	struct _TREEITEM *  GetFirstVisibleItem(void)const;	//522
	unsigned int  GetIndent(void)const;	//538
	virtual int  GetItem(struct tagLVITEMA *,int,int)const;	//548
	virtual int  GetItem(struct tagTVITEMA *,int,int)const;	//549
	struct _TREEITEM *  GetItemAt(int)const;	//551
	unsigned long  GetItemData(struct _TREEITEM *)const;	//556
	virtual int  GetItemEx(struct tagLVITEMA *,struct tagLVITEMEX *,int)const;	//558
	virtual int  GetItemEx(struct tagTVITEMA *,struct tagTVITEMEX *,int)const;	//559
	int  GetItemImage(struct _TREEITEM *,int &,int &)const;	//562
	int  GetItemRect(struct _TREEITEM *,struct tagRECT *,unsigned int)const;	//567
	unsigned int  GetItemState(struct _TREEITEM *,unsigned int)const;	//569
	int  GetItemString(struct _TREEITEM *,int,class CString &);	//570
	class CString   GetItemText(struct _TREEITEM *,int)const;	//573
	struct _TREEITEM *  GetLastVisibleItem(void)const;	//581
	struct _TREEITEM *  GetNextDisabledItem(struct _TREEITEM *)const;	//632
	struct _TREEITEM *  GetNextHiddenItem(struct _TREEITEM *)const;	//633
	struct _TREEITEM *  GetNextItem(struct _TREEITEM *,unsigned int)const;	//636
	struct _TREEITEM *  GetNextItemInDisplayOrder(struct _TREEITEM *)const;	//637
	struct _TREEITEM *  GetNextSelectedItem(struct _TREEITEM *)const;	//639
	struct _TREEITEM *  GetNextSiblingItem(struct _TREEITEM *)const;	//641
	struct _TREEITEM *  GetNextVisibleItem(struct _TREEITEM *)const;	//642
	struct _TREEITEM *  GetParentItem(struct _TREEITEM *)const;	//661
	struct _TREEITEM *  GetPrevSelectedItem(struct _TREEITEM *)const;	//668
	struct _TREEITEM *  GetPrevSiblingItem(struct _TREEITEM *)const;	//670
	struct _TREEITEM *  GetPrevVisibleItem(struct _TREEITEM *)const;	//671
	struct _TREEITEM *  GetRootItem(struct _TREEITEM *)const;	//675
	struct _TREEITEM *  GetSelectedItem(void)const;	//714
	virtual unsigned long  GetTreeCtrlStyle(void)const;	//741
	virtual unsigned long  GetTreeCtrlStyleEx(void)const;	//742
	virtual int  GetTreeCtrlStyles(unsigned long &,unsigned long &)const;	//743
	unsigned int  GetVisibleCount(void)const;	//747
	void  HideDisabledItems(int);	//752
	int  HideItem(struct _TREEITEM *,int);	//753
	struct _TREEITEM *  HitTest(struct tagTVHITTESTINFO *);	//755
	struct _TREEITEM *  HitTest(class CPoint,unsigned int *);	//756
protected:
	void  InitRootNode(void);	//771
public:
	struct _TREEITEM *  InsertItem(unsigned int,char const *,int,int,unsigned int,unsigned int,long,struct _TREEITEM *,struct _TREEITEM *);	//780
	struct _TREEITEM *  InsertItem(struct tagTVINSERTSTRUCTA *);	//781
	struct _TREEITEM *  InsertItem(char const *,int,int,struct _TREEITEM *,struct _TREEITEM *);	//782
	struct _TREEITEM *  InsertItem(char const *,struct _TREEITEM *,struct _TREEITEM *);	//783
	virtual int  IsCallbackItem(int)const;	//796
	int  IsDisabled(struct _TREEITEM *)const;	//799
	int  IsExpanded(struct _TREEITEM *)const;	//803
	int  IsHidden(struct _TREEITEM *)const;	//807
	int  IsHideDisabledItems(void)const;	//808
	int  IsSelected(struct _TREEITEM *)const;	//818
	int  ItemExists(struct _TREEITEM *,int)const;	//831
	int  ItemHasChildren(struct _TREEITEM *)const;	//833
	int  ItemHasChildrenOnDemand(struct _TREEITEM *)const;	//834
protected:
	int  ItemIndex(struct _TREEITEM *,int)const;	//835
	virtual int  LoadPaintContext(class SECListCtrl::LvPaintContext *);	//856
	virtual void  MeasureSubItem(class SECListCtrl::LvPaintContext *,unsigned int &);	//869
	void  MeasureText(class CDC *,class SECTreeCtrl::Node *,class CString const &,class CRect &)const;	//870
public:
	virtual int  ModifyTreeCtrlStyle(unsigned long,unsigned long,int);	//877
	virtual int  ModifyTreeCtrlStyleEx(unsigned long,unsigned long,int);	//878
	virtual int  ModifyTreeCtrlStyles(unsigned long,unsigned long,unsigned long,unsigned long,int);	//879
protected:
	virtual class SECTreeCtrl::Node *  NewRefTreeCtrlNode(void)const;	//881
	int  NodeInListBox(class SECTreeCtrl::Node *)const;	//883
	int  NodeIndex(class SECTreeCtrl::Node *,int)const;	//884
	virtual void  NotifyBeginDrag(int,struct tagPOINT)const;	//886
	virtual int  NotifyBeginLabelEdit(int)const;	//888
	virtual void  NotifyBeginRDrag(int,struct tagPOINT)const;	//890
	virtual void  NotifyDeleteItem(int)const;	//892
	virtual int  NotifyEndLabelEdit(int,int,class CString &,int)const;	//894
	virtual void  NotifyGetDispInfo(struct tagTVITEMA *)const;	//896
	virtual void  NotifyGetDispInfoEx(struct tagTVITEMEX *)const;	//898
	virtual void  NotifyItemExpanded(struct _TREEITEM *,unsigned int)const;	//900
	virtual int  NotifyItemExpanding(struct _TREEITEM *,unsigned int)const;	//901
	virtual void  NotifyKeyDown(unsigned short)const;	//903
	virtual void  NotifyParent_DeleteItem(struct _TREEITEM *)const;	//906
	virtual void  NotifySelChanged(int,int,enum SECListCtrl::SelectAction)const;	//908
	virtual int  NotifySelChanging(int,int,enum SECListCtrl::SelectAction)const;	//910
	virtual void  NotifySetDispInfo(struct tagTVITEMA *)const;	//912
	virtual void  NotifySetDispInfoEx(struct tagTVITEMEX const *)const;	//914
	virtual void  OnBackKeyPressed(int,int,int,int,unsigned int,unsigned int);	//934
	void  OnDestroy(void);	//985
	virtual void  OnEndKeyPressed(int,int,int,int,unsigned int,unsigned int);	//989
	virtual void  OnHomeKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1020
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1033
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1044
	void  OnLButtonDown(unsigned int,class CPoint);	//1052
	virtual void  OnLeftKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1060
public:
protected:
	virtual void  OnReturnKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1109
	virtual void  OnRightKeyPressed(int,int,int,int,unsigned int,unsigned int);	//1111
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1195
	void  RemoveChildrenFromListCtrl(class SECTreeCtrl::Node *,int);	//1240
public:
	int  Select(struct _TREEITEM *,unsigned int);	//1284
	int  SelectAllVisibleChildren(struct _TREEITEM *);	//1285
protected:
	int  SelectCaretItem(struct _TREEITEM *);	//1286
public:
	int  SelectDropTarget(struct _TREEITEM *);	//1288
	int  SelectItem(struct _TREEITEM *);	//1291
	int  SelectItemRange(struct _TREEITEM *,struct _TREEITEM *,int);	//1293
	int  SelectSetFirstVisible(struct _TREEITEM *);	//1294
	void  SetFilterLevel(unsigned short);	//1344
	int  SetFirstVisible(struct _TREEITEM *);	//1347
	void  SetIndent(unsigned int);	//1358
	int  SetItem(struct _TREEITEM *,unsigned int,char const *,int,int,unsigned int,unsigned int,long);	//1363
	int  SetItem(struct tagTVITEMA *);	//1364
	virtual int  SetItem(struct tagLVITEMA const *);	//1365
	int  SetItemData(struct _TREEITEM *,unsigned long);	//1367
	virtual int  SetItemEx(struct tagLVITEMA *,struct tagLVITEMEX const *);	//1369
	virtual int  SetItemEx(struct tagTVITEMA *,struct tagTVITEMEX *);	//1370
	int  SetItemImage(struct _TREEITEM *,int,int);	//1372
	int  SetItemState(struct _TREEITEM *,unsigned int,unsigned int);	//1374
	int  SetItemString(struct _TREEITEM *,int,class CString const &);	//1375
	int  SetItemText(struct _TREEITEM *,int,char const *);	//1377
	int  SetItemText(struct _TREEITEM *,char const *);	//1378
	virtual int  SetTreeCtrlStyle(unsigned long,int);	//1445
	virtual int  SetTreeCtrlStyleEx(unsigned long,int);	//1446
	virtual int  SetTreeCtrlStyles(unsigned long,unsigned long,int);	//1447
protected:
	int  ShouldBeInListBox(struct _TREEITEM *)const;	//1450
public:
	int  SortChildren(struct _TREEITEM *);	//1458
	int  SortChildrenCB(struct tagTVSORTCB *);	//1459
	int  SubclassTreeCtrlId(unsigned int,class CWnd *);	//1469
	int  UnHideAllItems(void);	//1484
	virtual int  Update(struct _TREEITEM *,int,int,int);	//1488

	DECLARE_MESSAGE_MAP()
};

class IMPORT_1C SECMDIChildWnd:public CMDIChildWnd
{
DECLARE_DYNCREATE(SECMDIChildWnd)

protected:
	 SECMDIChildWnd(void);	//50
	virtual  ~SECMDIChildWnd(void);	//110
	void  OnChildActivate(void);	//953
	int  OnMouseActivate(class CWnd *,unsigned int,unsigned int);	//1065

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECMDIFrameWnd:public CMDIFrameWnd //CObject
{
DECLARE_DYNCREATE(SECMDIFrameWnd)
protected:
	SECMDIFrameWnd(void);	//51
	virtual  ~SECMDIFrameWnd(void);	//111
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//962

	DECLARE_MESSAGE_MAP()

protected:
	virtual int  OnCommand(unsigned int,long);	//963
public:
	virtual class CFrameWnd *  GetActiveFrame(void);	//450
	virtual void  RecalcLayout(int);	//1224
	virtual void  OnSetPreviewMode(int,struct CPrintPreviewState *);	//1127
	virtual void  OnUpdateFrameTitle(int);	//1173
	virtual void  OnUpdateFrameMenu(HMENU);	//1170
	virtual void  DockControlBarEx(class CControlBar *,unsigned int,int,int,float,int);	//334
	virtual class CMDIChildWnd *  CreateFloatingMDIChild(unsigned long,class CControlBar *);	//245
	virtual void  ActivateDockableFrame(class SECFrameWnd *);	//126
	virtual void  OnActivateDockableFrame(class SECFrameWnd *);	//920
	virtual void  LoadBarState(char const *);	//850
	virtual void  SaveBarState(char const *)const;	//1272
	virtual void  SetDockState(class SECDockState &);	//1340
	virtual void  GetDockState(class SECDockState &)const;	//496
	virtual class SECControlBarManager *  GetControlBarManager(void)const;	//485
	virtual void  SetControlBarManager(class SECControlBarManager *);	//1332

public:
	enum AlignCaption {acLeft, acCenter, acRight};

	void  DockControlBar(class CControlBar *,unsigned int,struct tagRECT const *);	//332
	void  DockControlBar(class CControlBar *,class CDockBar *,struct tagRECT const *);	//333
	int  EnableCustomCaption(int,int);	//391
	void  EnableDocking(unsigned long);	//393
	void  FloatControlBar(class CControlBar *,class CPoint,unsigned long);	//440
	void  FloatControlBarInMDIChild(class CControlBar *,class CPoint,unsigned long);	//441
	void  ForceCaptionRedraw(void);	//443
	void  ReDockControlBar(class CControlBar *,class CDockBar *,struct tagRECT const *);	//1214
	void  ReFloatControlBar(class CControlBar *,class CPoint,unsigned long);	//1215
	void  SetCaptionTextAlign(enum SECMDIFrameWnd::AlignCaption,int);	//1323
	void  ShowControlBar(class CControlBar *,int,int);	//1452
protected:
	void  GetWindowsVersion(void);	//748
	void  OnActivate(unsigned int,class CWnd *,int);	//917
	void  OnActivateApp(int,HTASK);	//919
	long  OnCommandHelp(unsigned int,long);	//964
	long  OnExtendContextMenu(unsigned int,long);	//999
	long  OnGetText(unsigned int,long);	//1012
	int  OnNcActivate(int);	//1070
	void  OnNcPaint(void);	//1074
	long  OnSetText(unsigned int,long);	//1129
	void  OnSysColorChange(void);	//1146

	static unsigned long const  (*  dwSECDockBarMap)[2];	//1624

};


class IMPORT_1C SECStatusBar:public SECControlBar
{
DECLARE_DYNAMIC(SECStatusBar)

public:
	 SECStatusBar(void);	//52
	virtual  ~SECStatusBar(void);	//112
	int  AllocElements(int,int);	//164
	virtual class CSize   CalcFixedLayout(int,int);	//185
	void  CalcInsideRect(class CRect &,int)const;	//187
	int  CommandToIndex(unsigned int)const;	//204
	int  Create(class CWnd *,unsigned long,unsigned int);	//226
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//367
	unsigned int  GetItemID(int)const;	//561
	void  GetItemRect(int,struct tagRECT *)const;	//566
	void  GetPaneInfo(int,unsigned int &,unsigned int &,int &)const;	//655
	unsigned int  GetPaneStyle(int)const;	//656
	class CString   GetPaneText(int)const;	//657
	void  GetPaneText(int,class CString &)const;	//658
	virtual void  OnBarStyleChange(unsigned long,unsigned long);	//945
protected:
	virtual int  OnChildNotify(unsigned int,unsigned int,long,long *);	//954
	long  OnGetText(unsigned int,long);	//1013
	long  OnGetTextLength(unsigned int,long);	//1014
	void  OnNcCalcSize(int,struct tagNCCALCSIZE_PARAMS *);	//1071
	unsigned int  OnNcHitTest(class CPoint);	//1073
	void  OnNcPaint(void);	//1075
	void  OnPaint(void);	//1094
	long  OnSetMinHeight(unsigned int,long);	//1126
	long  OnSetText(unsigned int,long);	//1130
	void  OnSize(unsigned int,int,int);	//1141
public:
	virtual void  OnUpdateCmdUI(class CFrameWnd *,int);	//1168
protected:
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//1177
public:
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1194
	int  SetIndicators(unsigned int const *,int);	//1359
	void  SetPaneInfo(int,unsigned int,unsigned int,int);	//1399
	void  SetPaneStyle(int,unsigned int);	//1400
	int  SetPaneText(int,char const *,int);	//1401
protected:
	void  UpdateAllPanes(int,int);	//1489

	DECLARE_MESSAGE_MAP()

};

// Классы для создания своих контролов в тулбарах
struct SECButtonClass
{
	SECStdBtn* (PASCAL* m_pfnCreateButton)();
};
struct SECBtnMapEntry
{
	SECButtonClass* m_pBtnClass;					// Class for this button
	UINT            m_nID;							// ID for this button
	UINT	 	    m_nStyle;						// Style for this button
	UINT            m_nData[5];	// Button specific info.
	/*
	m_nData[0]= id контрола
	m_nData[1]=	стили окна
	m_nData[2]= MAKELONG(maxwidth,minwidth);
	m_nData[3]= width;
	m_nData[4]=	height;
	*/
};

class IMPORT_1C SECStdBtn
{
public:
	UINT m_nID;				// Button ID
	UINT m_nImage;			// Index of image in toolbar bitmap
	UINT m_nStyle;			// Button styles

	int  m_x;				// x position in parent toolbars client rect
	int  m_y;				// y position in parent toolbars client rect
	int  m_cx;				// Width of button
	int  m_cy;				// Height of button
	SECCustomToolBar* m_pToolBar;			// Our parent toolbar
	int  m_nType;			// See BtnType enum
	char m_buf[100];
	 SECStdBtn(void);	//53

	virtual void  SetMode(int);	//1391											0
	virtual void  SetPos(int,int);	//1411										1
	virtual void  Invalidate(int)const;	//789									2
	virtual void  Init(class SECCustomToolBar *,unsigned int const *);	//765	3
	virtual void  DrawButton(class CDC &,struct SECBtnDrawData &);	//339		4
	virtual int  BtnPressDown(class CPoint);	//175							5
	virtual void  BtnPressMouseMove(class CPoint);	//176						6
	virtual void  BtnPressCancel(void);	//174									7
	virtual unsigned int  BtnPressUp(class CPoint);	//177						8
	virtual void  BarStyleChanged(unsigned long);	//173						9
	virtual int  OnToolHitTest(class CPoint,struct tagTOOLINFOA *)const;	//1158	10
	virtual void  InformBtn(unsigned int,void *);	//759						11
	virtual void  AdjustSize(void);	//162										12
	virtual void  GetBtnInfo(unsigned char *,unsigned char * *)const;	//461	13
	virtual void  SetBtnInfo(unsigned char,unsigned char * const);	//1315		14
protected:
	virtual void  DrawFace(struct SECBtnDrawData &,int,int &,int &,int &,int &,int);	//348	15
	virtual void  DrawDisabled(struct SECBtnDrawData &,int,int,int,int);	//347				16
	virtual void  DrawChecked(struct SECBtnDrawData &,int,int,int,int);	//342					17
	virtual void  DrawIndeterminate(struct SECBtnDrawData &,int,int,int,int);	//359			18
	virtual void  DrawConfigFocus(struct SECBtnDrawData &);	//344								19

public:
	virtual  ~SECStdBtn(void);	//113											20
	virtual struct SECButtonClass *  GetButtonClass(void)const;	//465			21
	static class SECStdBtn * __stdcall CreateButton(void);	//237				
	static struct SECButtonClass const  classSECStdBtn;	//1617
protected:
	void  CreateMask(struct SECBtnDrawData &,int,int,int,int);	//255
public:
	void  GetBtnRect(class CRect &)const;	//463
	void  SetToolBar(class SECCustomToolBar *);	//1440
};


class IMPORT_1C SECWndBtn: public SECStdBtn
{
public:
	 SECWndBtn(void);	//67
	virtual  ~SECWndBtn(void);	//122
	virtual void  AdjustSize(void);	//163
	virtual void  DrawButton(class CDC &,struct SECBtnDrawData &);	//341
	virtual void  GetBtnInfo(unsigned char *,unsigned char * *)const;	//462
	virtual void  InformBtn(unsigned int,void *);	//760
	virtual void  Init(class SECCustomToolBar *,unsigned int const *);	//766
	virtual void  Invalidate(int)const;	//790
	virtual int  OnToolHitTest(class CPoint,struct tagTOOLINFOA *)const;	//1159
	virtual void  SetBtnInfo(unsigned char,unsigned char * const);	//1316
	virtual void  SetMode(int);	//1392
	virtual void  SetPos(int,int);	//1412

	virtual int  LButtonDown(unsigned int,class CPoint);	//840				22
protected:
	// Returns pointer to our CWnd		23
	virtual CWnd* GetWnd() = 0;			
	// Creates our CWnd 24
	virtual BOOL CreateWnd(CWnd* pParentWnd, DWORD dwStyle, CRect& rect,int nID) = 0;
public:
	int  LButtonDblClk(unsigned int,class CPoint);	//839
	int  LButtonUp(unsigned int,class CPoint);	//841
	int  RButtonDblClk(unsigned int,class CPoint);	//1210
	int  RButtonDown(unsigned int,class CPoint);	//1211
	int  RButtonUp(unsigned int,class CPoint);	//1212
	int  SetCursor(void);	//1333
protected:
	void  ForwardMessage(unsigned int,unsigned int,long);	//444
	void  PostPaint(void);	//1192
	void  PrePaint(void);	//1197
	void  SendNotify(unsigned int);	//1300
	static HICON  hCurHorzDrag;	//1625
};


class IMPORT_1C CTBSearchBox:public SECWndBtn,CComboBox
{
public:
	 CTBSearchBox(void);	//38
	virtual  ~CTBSearchBox(void);	//98
	virtual void  AdjustSize(void);	//161
	static class SECStdBtn *  CreateButton(void);	//235
protected:
	virtual int  CreateWnd(class CWnd *,unsigned long,class CRect &,int);	//287
public:
	virtual struct SECButtonClass *  GetButtonClass(void)const;	//464
protected:
	virtual class CWnd *  GetWnd(void);	//749
public:
	virtual void  Init(class SECCustomToolBar *,unsigned int const *);	//764
	virtual void  OnFontCreateAndSet(void);	//1008
protected:
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1040
	void  OnLButtonDown(unsigned int,class CPoint);	//1046
	void  OnLButtonUp(unsigned int,class CPoint);	//1053
	void  OnPaint(void);	//1089
	void  OnRButtonDblClk(unsigned int,class CPoint);	//1099
	void  OnRButtonDown(unsigned int,class CPoint);	//1100
	void  OnRButtonUp(unsigned int,class CPoint);	//1102
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//1122
public:
	static struct SECButtonClass const  classCTBSearchBox;	//1605

	DECLARE_MESSAGE_MAP()
};
// Конец классов контролов в тулбарах

class IMPORT_1C SECTabControlBase:public CWnd
{
DECLARE_DYNAMIC(SECTabControlBase)

public:
	 SECTabControlBase(void);	//54
	virtual  ~SECTabControlBase(void);	//114
	virtual void  ActivateTab(int);	//127
	virtual class SECTab *  AddTab(char const *,class CObject *,HMENU,void *);	//151
	virtual void  ClearSelection(void);	//198
	virtual int  Create(unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//227
	virtual class SECTab *  CreateNewTab(void)const;	//258
	virtual void  DeleteTab(int);	//306
	virtual void  DestroyGDIObjects(void);	//311
	int  FindTab(class CObject const * const,int &)const;	//438
	int  GetActiveTab(int &)const;	//451
protected:
	class SECTab &  GetTab(int)const;	//721
public:
	int  GetTabCount(void)const;	//722
	int  GetTabInfo(int,char const * &,int &,class CObject * &,HMENU &,void * &)const;	//724
protected:
	class SECTab *  GetTabPtr(int)const;	//727
public:
	virtual void  Initialize(unsigned long);	//773
	virtual class SECTab *  InsertTab(int,char const *,class CObject *,HMENU,void *);	//786
	virtual void  InvalidateTab(int);	//793
	virtual void  OnActivateTab(int);	//922
protected:
	void  OnDestroy(void);	//983
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//1124
public:
	virtual void  RecalcLayout(void);	//1225
	virtual void  RenameTab(int,char const *);	//1258
	virtual void  ScrollToTab(int);	//1279
	virtual void  SelectTab(int);	//1295
	virtual void  SetTabIcon(int,unsigned int);	//1428
	virtual void  SetTabIcon(int,HICON);	//1429
	virtual void  SetTabIcon(int,char const *);	//1430
	int  TabExists(int)const;	//1472
	int  TabExists(class CObject *)const;	//1473

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECTabWndBase:public CWnd
{
DECLARE_DYNAMIC(SECTabWndBase)

public:
	 SECTabWndBase(void);	//55
	virtual  ~SECTabWndBase(void);	//115
	virtual void  ActivateTab(int);	//128
	virtual void  ActivateTab(class CWnd *);	//129
	virtual void  ActivateTab(class CWnd *,int);	//130
	virtual class SECTab *  AddTab(struct CRuntimeClass *,char const *,struct CCreateContext *,unsigned int);	//152
	virtual class SECTab *  AddTab(class CWnd *,char const *);	//153
	virtual void  ClearSelection(void);	//199
	virtual int  Create(class CWnd *,unsigned long,unsigned int);	//228
	virtual int  Create(char const *,char const *,unsigned long,struct tagRECT const &,class CWnd *,unsigned int,struct CCreateContext *);	//229
protected:
	virtual int  CreateScrollBarCtrl(unsigned long,unsigned int);	//278
	virtual int  CreateTabCtrl(unsigned long,unsigned int);	//284
	class CWnd *  CreateView(struct CRuntimeClass *,struct CCreateContext *,unsigned int);	//286
public:
	int  FindTab(class CWnd const * const,int &)const;	//439
	int  GetActiveTab(int &);	//452
	int  GetActiveTab(class CWnd * &);	//453
protected:
	virtual void  GetInsideRect(class CRect &)const;	//544
public:
	int  GetTabCount(void);	//723
	int  GetTabInfo(int,char const * &,int &,class CWnd * &,void * &);	//725
	virtual class SECTab *  InsertTab(struct CRuntimeClass *,int,char const *,struct CCreateContext *,unsigned int);	//787
	virtual class SECTab *  InsertTab(class CWnd *,int,char const *);	//788
protected:
	void  OnDestroy(void);	//984
	long  OnTabSelect(unsigned int,long);	//1150
	virtual void  RecalcLayout(void);	//1226
public:
	virtual void  RemoveTab(int);	//1253
	virtual void  RemoveTab(class CWnd *);	//1254
	virtual void  RenameTab(int,char const *);	//1259
	virtual void  RenameTab(class CWnd *,char const *);	//1260
	virtual void  ScrollToTab(int);	//1280
	virtual void  ScrollToTab(class CWnd *);	//1281
	virtual void  SetScrollStyle(int,unsigned long);	//1416
	virtual void  SetTabIcon(int,unsigned int);	//1431
	virtual void  SetTabIcon(int,HICON);	//1432
	virtual void  SetTabIcon(int,char const *);	//1433
	int  TabExists(int);	//1474
	int  TabExists(class CWnd *);	//1475

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECTipOfDay:public CDialog
{
DECLARE_DYNAMIC(SECTipOfDay)

public:
	 SECTipOfDay(char const *,int,int,class CWnd *);	//56
	virtual  ~SECTipOfDay(void);	//116
protected:
	void  CountTips(void);	//215
public:
	virtual int  Create(void);	//230
protected:
	void  LoadTip(int);	//858
public:
	virtual void  OnHelp(void);	//1017
	virtual int  OnInitDialog(void);	//1026
	void  OnNextTip(void);	//1081
	void  OnPaint(void);	//1095
	void  OnPreviousTips(void);	//1097
	void  OnShowButtonChanged(void);	//1132
protected:
	void  RedrawTip(void);	//1229
public:
	void  SetLeadInText(char const *);	//1382

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECToolBarCmdPage:public CPropertyPage
{
DECLARE_DYNCREATE(SECToolBarCmdPage)

public:
	 SECToolBarCmdPage(unsigned int,unsigned int);	//57
	 SECToolBarCmdPage(char const *,unsigned int);	//58
	 SECToolBarCmdPage(void);	//59
	virtual  ~SECToolBarCmdPage(void);	//117
	void  DefineBtnGroup(char const *,int,unsigned int *);	//290
protected:
	class SECCustomToolBar *  GetControlBar(unsigned int);	//484
	void  Initialize(void);	//774
	long  OnBarNotify(unsigned int,long);	//941
	virtual int  OnInitDialog(void);	//1027
	virtual int  OnKillActive(void);	//1034
	virtual void  OnReset(void);	//1104
	void  OnSelChange(void);	//1114
	virtual int  OnSetActive(void);	//1119
	long  OnToolBarStyleChanging(unsigned int,long);	//1154
	void  SetDescription(int);	//1337
public:
	void  SetManager(class SECToolBarManager *);	//1387

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECToolBarManager:public CCmdTarget	// На самом деле SECControlBarManager
{
DECLARE_DYNAMIC(SECToolBarManager)
public:
	 SECToolBarManager(class CFrameWnd *);	//60
	 SECToolBarManager(void);	//61
	virtual  ~SECToolBarManager(void);	//118
	void  AddNotify(class CWnd *);	//144
	void  BarNotify(int,enum SECCustomToolBar::BarNotifyCode);	//172
	void  ChangeBarStyle(int,unsigned long);	//193
protected:
	int  ChangeBitmap(void);	//194
public:
	void  ChangeExBarStyle(int,unsigned long);	//195
	int  CoolLookEnabled(void)const;	//210
	virtual class SECControlBarInfoEx *  CreateControlBarInfoEx(class SECControlBarInfo *)const;	//238
	class SECCustomToolBar *  CreateUserToolBar(char const *);	//285
	void  DefineDefaultToolBar(unsigned int,char const *,unsigned int,unsigned int *,unsigned long,unsigned int,unsigned int,int,int);	//291
	void  DisableMainFrame(void);	//321
	virtual class CControlBar *  DynCreateControlBar(class SECControlBarInfo *);	//381
	void  EnableConfigMode(int);	//389
	void  EnableCoolLook(int,unsigned long);	//390
	void  EnableFlyBy(int);	//394
	void  EnableLargeBtns(int);	//396
	void  EnableMainFrame(void);	//397
	void  EnableToolTips(int);	//404
	int  ExecViewToolBarsDlg(void);	//417
	int  FlyByEnabled(void)const;	//442 SECMDIFrameWnd
	virtual unsigned long  GetBarTypeID(class CControlBar *)const;	//457
	struct SECBtnMapEntry const *  GetButtonMap(void)const;	//466
	int  GetDefaultBtns(unsigned int,int &,unsigned int * &);	//491
	void  GetNoDropRect(class CRect &)const;	//643
	int  InConfigMode(void)const;	//758
	void  InformBtns(unsigned int,unsigned int,void *);	//762
	int  IsMainFrameEnabled(void);	//811
	int  IsToolBarCommand(class CRect &)const;	//825
	int  LargeBtnsEnabled(void)const;	//843
	virtual void  LoadState(char const *);	//857
	int  LoadToolBarResource(char const *,char const *);	//861
	void  RemoveNotify(class CWnd *);	//1247
	virtual void  SaveState(char const *)const;	//1274
	void  SetButtonMap(struct SECBtnMapEntry const *);	//1317
	virtual void  SetDefaultDockState(void);	//1335
	void  SetInToolBarCommand(int,class CRect &);	//1357
	void  SetNoDropWnd(class CWnd *);	//1394
	void  SetToolBarInfo(class SECCustomToolBar *);	//1442
	void  SysColorChange(void);	//1471
	class SECCustomToolBar *  ToolBarUnderRect(class CRect const &)const;	//1478
	int  ToolTipsEnabled(void)const;	//1479

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECToolBarsBase
{
protected:
	 SECToolBarsBase(void);	//62
public:
	virtual  ~SECToolBarsBase(void);	//119
protected:
	void  FillToolBarList(void);	//422
	int  FindIndex(unsigned int);	//430
	int  GetCurBar(class CControlBar * &,int *);	//487
	void  InitDialog(void);	//767
	long  OnBarNotify(unsigned int,long);	//942
	void  OnChkChange(void);	//955
	void  OnCoolLook(void);	//967
	void  OnLargeButtons(void);	//1057
	void  OnNameKillFocus(void);	//1068
	int  OnNewToolbar(void);	//1077
	void  OnResetBtn(void);	//1106
	void  OnSelChange(void);	//1115
	void  OnToolTips(void);	//1161
	void  RestoreState(void);	//1268
	void  SaveState(void);	//1275
public:
	void  SetManager(class SECToolBarManager *);	//1388
protected:
	void  SetWnd(class CWnd *);	//1449
};


class IMPORT_1C SECToolBarsPage
{
public:
	 SECToolBarsPage(unsigned int,unsigned int);	//63
	 SECToolBarsPage(char const *,unsigned int);	//64
	 SECToolBarsPage(void);	//65
	virtual  ~SECToolBarsPage(void);	//120
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//326
	long  OnBarNotify(unsigned int,long);	//943
	virtual void  OnCancel(void);	//949
	void  OnChkChange(void);	//956
	void  OnCoolLook(void);	//968
	virtual int  OnInitDialog(void);	//1028
	virtual int  OnKillActive(void);	//1035
	void  OnLargeButtons(void);	//1058
	void  OnNameKillFocus(void);	//1069
	void  OnNewToolbar(void);	//1078
	virtual void  OnOK(void);	//1084
	virtual void  OnReset(void);	//1105
	void  OnResetBtn(void);	//1107
	void  OnSelChange(void);	//1116
	virtual int  OnSetActive(void);	//1120
	void  OnStyleChanging(unsigned int);	//1143
	void  OnToolTips(void);	//1162

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLocalComboBox
{
public:
	virtual  ~CLocalComboBox(void);	//79
	void  GetTheText(class CString &);	//732
	int  LimitText(int);	//844
	int  OnCreate(struct tagCREATESTRUCTA *);	//970
	void  SetTheText(char const *,int);	//1438
	static int  m_nFontHeight;	//1633

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECTreeNode
{
public:
	int  AddChild(class SECTreeNode *,class SECTreeNode *);	//133
	void  DeleteAllChildren(void);	//294
	void  DeleteChildren(void);	//298
	void  DetachFromTree(void);	//316
	virtual void  Expand(int);	//420
	class SECTreeNode *  GetBottomLeftChild(void)const;	//459
	class SECTreeNode *  GetBottomRightChild(void)const;	//460
	unsigned short  GetDistanceFromRoot(void)const;	//495
	virtual class SECTreeNode *  GetFirstChild(void)const;	//513
	class SECTreeNode *  GetFirstSibling(void)const;	//521
	class SECTreeNode *  GetLastChild(void)const;	//578
	class SECTreeNode *  GetLastSibling(void)const;	//580
	class SECTreeNode *  GetNextInDisplayOrder(void)const;	//634
	virtual class SECTreeNode *  GetNextSibling(void)const;	//640
	unsigned int  GetNumDescendents(void)const;	//647
	virtual class SECTreeNode *  GetParent(void)const;	//660
	class SECTreeNode *  GetPrevInDisplayOrder(void)const;	//667
	virtual class SECTreeNode *  GetPrevSibling(void)const;	//669
	class SECTreeNode *  GetRoot(void)const;	//674
	int  IsAncestor(class SECTreeNode *)const;	//794
	int  IsDescendant(class SECTreeNode *)const;	//797
	virtual int  IsExpanded(void)const;	//804
	int  IsSibling(class SECTreeNode *)const;	//821
	virtual int  OnNextSearchNode(unsigned short,class SECTreeNode *)const;	//1080
	class SECTreeNode *  Search(unsigned short)const;	//1283
};


class IMPORT_1C CAppFrame:public CWinApp
{
public:
	DWORD bufAppFrame[0xFF];// 0 = +0C4h

	virtual int  OnIdle(long);	//1021
	virtual int  DoMessageBox(char const *,unsigned int,unsigned int);	//327

	void  AddMenu(HMENU,int);	//142
	void  AddOptionsPage(struct _GUID const &,struct IUnknown *);	//145
	long  DoOptionsDialog(void);	//328
	int  FindOptionsPage(struct _GUID const &);	//431
	class CTBManager *  GetToolBarManager(void);	//736
	void  InitMenus(unsigned int,unsigned int);	//769
	int  IsWaiting(void);	//828
	void  RemoveOptionsPage(struct _GUID const &);	//1248
	void  RemoveOptionsPage(struct _GUID const &,struct IUnknown *);	//1249
	int  SetNoSounds(int)const;	//1395
	int  StartWait(void);	//1461
	int  StopWait(unsigned int);	//1462
	int  StopWait(char const *);	//1463
	void  UpdateMainFrmMenu(void);	//1491
	int  WaitProc(unsigned int);	//1493
	int  WaitProc(char const *);	//1494

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECToolBarSheet
{
protected:
	void  OnActivate(unsigned int,class CWnd *,int);	//918
	long  OnFloatStatus(unsigned int,long);	//1006

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTBManager
{
private:
    SECToolBarManager SECManager;

public:
    virtual void FUNC0();
    virtual void FUNC1();
    virtual void FUNC2();
    virtual void FUNC3(int idr, char* name, int idr2, int idm, CDocument* doc, int DockID, int arg_6, int arg_7, int arg_8, int HorzDockCY);
public:
	DWORD buf[0x70];
	static class SECToolBarManager *  GetSECManager(void);	//708
	static class CTBManager *  GetToolBarManager(void);	//737
};


class IMPORT_1C CChildFrame7:public SECMDIChildWnd
{
DECLARE_DYNCREATE(CChildFrame7)

public:
	DWORD bufChildFrame7[0x02]; // (280h - C8h) / 4
	POINT ptMinSize;
	CToolBar7 cToolBar;
protected:
	 CChildFrame7(void);	//2
	virtual  ~CChildFrame7(void);	//69
public:
	virtual HACCEL  GetDefaultAccelerator(void);	//490
	virtual HMENU  GetInPlacedMenu(void);	//537
protected:
	virtual void  GetMessageString(unsigned int,class CString &)const;	//630
	void  OnGetMinMaxInfo(struct tagMINMAXINFO *);	//1011
	void  OnMDIActivate(int,class CWnd *,class CWnd *);	//1062
public:
	void  OnOperateSayAbout(void);	//1087
protected:
	void  OnTBPosChanged(struct tagNMHDR *,long *);	//1147
	void  OnTBSettChanged(struct tagNMHDR *,long *);	//1148
	void  OnToolTipNeedText(unsigned int,struct tagNMHDR *,long *);	//1160
	virtual void  OnUpdateFrameMenu(int,class CWnd *,HMENU);	//1169
	virtual void  OnUpdateFrameTitle(int);	//1171
public:
	virtual void  SetMinTrackSize(struct tagPOINT const &);	//1389
	static HMENU  m_hLastMenu;	//1632
	static int  m_nFrameCnt;	//1634

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CDockableWindow:public SECControlBar
{
	DECLARE_DYNAMIC(CDockableWindow)

protected:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//961

	DECLARE_MESSAGE_MAP()

public:
	//char buf[328];
	DWORD buf[0x06]; // размер объекта: 0x014С байта
	 CDockableWindow(unsigned int uTemplateID,unsigned int,class CDocument *);	//8
	 CDockableWindow(void);	//9

	virtual  ~CDockableWindow(void);	//75
protected:
	virtual void  OnUpdateCmdUI(class CFrameWnd *,int);	//1165
	virtual void  OnBarDock(void);	//935
	virtual void  OnBarFloat(void);	//937
	virtual void  OnBarMDIFloat(void);	//939
	virtual void  OnExtendContextMenu(class CMenu *);	//997

protected:
	void  OnContextMenu(class CWnd *,class CPoint);	//965
	int  OnCreate(struct tagCREATESTRUCTA *);	//969
	int  OnEraseBkgnd(class CDC *);	//991
	void  OnFloatAsMDIChild7(void);	//1002
	int  OnMouseActivate(class CWnd *,unsigned int,unsigned int);	//1063
	void  OnSize(unsigned int,int,int);	//1135
public:
	void  Init(unsigned int,unsigned int,class CDocument *);	//763
	void  SetFirstType(int);	//1345

};

class IMPORT_1C SEC3DTabControl:public SECTabControlBase
{
DECLARE_DYNAMIC(SEC3DTabControl)

public:
	 SEC3DTabControl(void);	//43
	virtual  ~SEC3DTabControl(void);	//103
	virtual class SECTab *  CreateNewTab(void)const;	//257
	virtual void  DestroyGDIObjects(void);	//310
protected:
	virtual void  DrawDisabled(class CDC *,int,int);	//346
	virtual void  DrawInterior(class CDC *,class SEC3DTab *,unsigned long);	//360
	virtual void  DrawTab(class CDC *,int);	//373
	virtual void  DrawTabBottom(class CDC *,class SEC3DTab *,unsigned long);	//374
	virtual void  DrawTabLeft(class CDC *,class SEC3DTab *,unsigned long);	//375
	virtual void  DrawTabRight(class CDC *,class SEC3DTab *,unsigned long);	//376
	virtual void  DrawTabTop(class CDC *,class SEC3DTab *,unsigned long);	//377
public:
	virtual void  EnableTab(int,int);	//400
protected:
	int  GetLabelWidth(int);	//577
	class SEC3DTab &  GetTab(int);	//720
	class SEC3DTab *  GetTabPtr(int)const;	//726
	virtual void  GetTabRect(int,class CRect &)const;	//728
public:
	virtual void  Initialize(unsigned long);	//772
	virtual void  InitializeFonts(unsigned long);	//775
	virtual void  InvalidateTab(int);	//792
	virtual int  IsTabEnabled(int);	//822
	virtual int  ModifyStyle(unsigned long,unsigned long,unsigned int);	//876
	virtual void  OnActivateTab(int);	//921
protected:
	void  OnDestroy(void);	//979
	int  OnEraseBkgnd(class CDC *);	//993
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1041
	void  OnLButtonDown(unsigned int,class CPoint);	//1048
	void  OnPaint(void);	//1091
	void  OnSize(unsigned int,int,int);	//1136
public:
	virtual int  OnToolHitTest(class CPoint,struct tagTOOLINFOA *)const;	//1155
	virtual void  RecalcLayout(void);	//1220
protected:
	virtual int  SetFontTab(class CFont *,int,int);	//1351
public:
	virtual void  SetTabIcon(int,unsigned int);	//1425
	virtual void  SetTabIcon(int,HICON);	//1426
	virtual void  SetTabIcon(int,char const *);	//1427
protected:
	void  SetTabPos(int,int,int,unsigned long);	//1434
	int  TabHit(int,class CPoint &)const;	//1476
	virtual void  rgbFill(class CDC *,int,int,int,int,unsigned long);	//1676

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SEC3DTabWnd:public SECTabWndBase
{
DECLARE_DYNCREATE(SEC3DTabWnd)

public:
	 SEC3DTabWnd(void);	//44
	virtual  ~SEC3DTabWnd(void);	//104
	virtual int  Create(class CWnd *,unsigned long,unsigned int);	//219
protected:
	virtual int  CreateTabCtrl(unsigned long,unsigned int);	//283
public:
	void  EnableTab(int,int);	//401
	void  EnableTab(class CWnd *,int);	//402
	class CFont *  GetFontActiveTab(void);	//524
	class CFont *  GetFontInactiveTab(void);	//526
protected:
	virtual void  GetInsideRect(class CRect &)const;	//540
private:
	void  HLine3D(class CDC *,int,int,int,int);	//750
public:
	int  IsTabEnabled(int);	//823
	int  IsTabEnabled(class CWnd *);	//824
protected:
	int  OnEraseBkgnd(class CDC *);	//994
	int  OnNcCreate(struct tagCREATESTRUCTA *);	//1072
	void  OnPaint(void);	//1092
	void  OnSize(unsigned int,int,int);	//1137
	void  OnSysColorChange(void);	//1144
	long  OnTabSelect(unsigned int,long);	//1149
	virtual void  RecalcLayout(void);	//1221
public:
	int  SetFontActiveTab(class CFont *,int);	//1348
	int  SetFontInactiveTab(class CFont *,int);	//1349
	unsigned long  SetTabStyle(unsigned long);	//1435
private:
	void  VLine3D(class CDC *,int,int,int);	//1492
protected:
	void  rgbFill(class CDC *,int,int,int,int,unsigned long);	//1677

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C SECControlBarMDIChildWnd:public SECMDIChildWnd
{
DECLARE_DYNCREATE(SECControlBarMDIChildWnd)

protected:
	 SECControlBarMDIChildWnd(void);	//47
	virtual  ~SECControlBarMDIChildWnd(void);	//107
	void  OnClose(void);	//959
	virtual int  OnCreateClient(struct tagCREATESTRUCTA *,struct CCreateContext *);	//974
	void  OnShowWindow(int,unsigned int);	//1133
	void  OnSize(unsigned int,int,int);	//1139
	void  OnWindowPosChanged(struct tagWINDOWPOS *);	//1175
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1193
public:
	virtual void  RecalcLayout(int);	//1222

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSplittFrame:public CChildFrame7
{
DECLARE_DYNCREATE(CSplittFrame)

protected:
	 CSplittFrame(void);	//37
	virtual  ~CSplittFrame(void);	//97
	virtual int  OnCreateClient(struct tagCREATESTRUCTA *,struct CCreateContext *);	//973

	DECLARE_MESSAGE_MAP()
};

// End class descriptions
// Begin free functions definitions
IMPORT_1C HMETAFILE __stdcall ConvertEMFtoWMF(HDC,HENHMETAFILE);
IMPORT_1C HENHMETAFILE __stdcall ConvertWMFtoEMF(HMETAFILE);
IMPORT_1C HBITMAP __stdcall CopyPaletteBitmap(HBITMAP,HPALETTE);
IMPORT_1C HPALETTE __stdcall CreateDIBPalette(struct tagBITMAPINFO *);
IMPORT_1C HPALETTE __stdcall CreateIdentityPalette(struct tagRGBQUAD * const,int);
IMPORT_1C void * __stdcall DibFormBitmap(HBITMAP,HPALETTE);
IMPORT_1C void * __stdcall DibFormBitmapEx(struct tagBITMAPINFOHEADER *,HBITMAP,HPALETTE);
IMPORT_1C unsigned char __cdecl GetDefaultUserCharSet(void);
IMPORT_1C int __cdecl GetFilePathAndName(char const *,class CString &,class CString &);
IMPORT_1C class CMDIFrameWnd * __cdecl GetMainFrame(void);
IMPORT_1C class CSearchHistoryProfile * __cdecl GetMainSHistory(void);
IMPORT_1C int __cdecl GetNearestStandardColorIndex(unsigned long,int);
IMPORT_1C unsigned long __cdecl GetStandardColor(int);
IMPORT_1C class CTBManager * __cdecl GetToolBarManager(void);
IMPORT_1C int __stdcall IsWinDib(struct tagBITMAPINFOHEADER *);
IMPORT_1C int __cdecl LoadWindowPosition(class CWnd *,char const *,int);
IMPORT_1C int __stdcall NumDIBColorEntries(struct tagBITMAPINFO *);
IMPORT_1C HBITMAP __stdcall ReMapBitmap2(HBITMAP,unsigned long,unsigned long);
IMPORT_1C HBITMAP __stdcall ReMapBitmap(HBITMAP,struct _COLORMAP *,int);
IMPORT_1C int __cdecl SECLoadSysColorBitmap(class CBitmap &,char const *);
IMPORT_1C int __cdecl SaveWindowPosition(class CWnd *,char const *);
// End free functions definitions

