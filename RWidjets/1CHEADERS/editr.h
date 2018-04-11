// Begin forward declarations
class  CAppSupport;
class  CControlID;
class  CControlList;
class  CEditDoc;
class  CFntHolder7;
class  CGallery;
class  CGalleryCtrl;
class  COnLineHelp;
// End forward declarations
// Begin class descriptions
struct ACCEL_LIGHT
{
	//DWORD alAccel; CAccelTable7
	BYTE m_ShiftState;
	WORD m_KeyCode;
};

class IMPORT_1C CAppSupport
{
private:
	 CAppSupport(void);	//1
public:
	class CAppSupport &  operator=(class CAppSupport const &);	//17
	static int  GetCommandState(class CFrameWnd *,unsigned int);	//84
	static int  GetSubString(class CString &,char const *,int);	//150
	static int  GetSubStringCount(char const *);	//151
	static void  UpdateFloatMenu(class CWnd *,class CMenu *);	//287
};


class IMPORT_1C CControlID
{
public:
//	char buf[0xAC];
// buf[0x00] = unsigned int CtrlID
// buf[0x04] = int CtrlType
// buf[0x08] = long LongID
// buf[0x0C - 0x17] = class CType & ValType
// buf[0x18 - 0x27] = class CRect & Place
// buf[0x28] = char const * Title
// buf[0x2C] = char const * CodeName
// buf[0x30] = char const * Formul
// buf[0x34] = unsigned long Style
// buf[0x38] = unsigned long ExStyle
// buf[0x3C] = struct ACCEL_LIGHT Accel
// buf[0x40] = long DependParam
// buf[0x44] = long ChooseForm
// buf[0x48] = char const * SDependParam
// buf[0x4C] = char const * ToolTip
// buf[0x50] = char const * HelpString
// buf[0x54 - 0x8F] = struct tagLOGFONTA Font
// buf[0x90] = unsigned long FontColor
// buf[0x94] = unsigned long BkColor
// buf[0x98 - 0xA7] = class CPictureHolder7 & Picture
// buf[0xA8] = char const * LayerName

	unsigned int m_CtrlID;
	int m_CtrlType;
	long m_LongID;
	class CType m_ValType;
	class CRect m_Place;
	char const * m_Title;
	char const * m_CodeName;
	char const * m_Formul;
	unsigned long m_Style;
	unsigned long m_ExStyle;
	struct ACCEL_LIGHT m_Accel;
	long m_DependParam;
	long m_ChooseForm;
	char const * m_SDependParam;
	char const * m_ToolTip;
	char const * m_HelpString;
	LOGFONTA m_Font;
	unsigned long m_FontColor;
	unsigned long m_BkColor;
	class CPictureHolder7 m_Picture;
	char const * m_LayerName;

	 CControlID(class CControlID const &);	//2
	 CControlID(void);	//3
	 ~CControlID(void);	//10
	class CControlID &  operator=(class CControlID &);	//18
	struct ACCEL_LIGHT   GetAccel(void);	//73
	unsigned long  GetBkColor(void);	//77
	long  GetChooseForm(void);	//80
	char const *  GetCodeName(void);	//81
	unsigned int  GetCtrlID(void);	//91
	int  GetCtrlType(void);	//92
	long  GetDependParam(void);	//100
	unsigned long  GetExStyle(void);	//105
	void  GetFont(struct tagLOGFONTA *);	//111
	unsigned long  GetFontColor(void);	//112
	char const *  GetFormul(void);	//113
	char const *  GetHelpString(void);	//120
	char const *  GetLayerName(void);	//127
	long  GetLongID(void);	//130
	class CPictureHolder7 &  GetPicture(void);	//140
	class CRect &  GetPlace(void);	//141
	char const *  GetSDependParam(void);	//146
	unsigned long  GetStyle(void);	//149
	char const *  GetTitle(void);	//155
	char const *  GetToolTip(void);	//156
	class CType &  GetValType(void);	//159
	void  InitAsBrowser(class CRect const &,long,class CPictureHolder7 const &);	//161
	void  InitAsFrame(class CRect const &,class CEditDoc *);	//162
	void  SetAccel(struct ACCEL_LIGHT const &);	//237
	void  SetBkColor(unsigned long);	//240
	void  SetChooseForm(long);	//242
	void  SetCodeName(class CString const &);	//243
	void  SetCtrlID(unsigned int);	//247
	void  SetCtrlType(int);	//248
	void  SetDependParam(long);	//251
	void  SetDependParam(char const *);	//252
	void  SetExStyle(unsigned long);	//255
	void  SetFont(struct tagLOGFONTA *);	//256
	void  SetFontColor(unsigned long);	//257
	void  SetFormul(class CString const &);	//258
	void  SetHelpString(char const *);	//261
	void  SetLayerName(char const *);	//267
	void  SetLongID(long);	//269
	void  SetPicture(class CPictureHolder7 &);	//273
	void  SetPlace(class CRect const &);	//274
	void  SetStyle(unsigned long);	//276
	void  SetTitle(class CString const &);	//278
	void  SetToolTip(char const *);	//282
	void  SetValType(class CType &);	//286
};


class IMPORT_1C CControlList
{
public:
	CPtrList m_List;

	 CControlList(void);	//4
	virtual  ~CControlList(void);	//11
	void  AddHead(class CControlID *);	//29
	void  AddTail(class CControlID *);	//31
	class CControlID *  GetAt(struct __POSITION *);	//76
	int  GetCount(void);	//89
	class CControlID *  GetHead(void);	//117
	struct __POSITION *  GetHeadPosition(void);	//119
	class CControlID *  GetNext(struct __POSITION * &);	//137
	class CControlID *  GetPrev(struct __POSITION * &);	//142
	class CControlID *  GetTail(void);	//152
	struct __POSITION *  GetTailPosition(void);	//153
	int  IsEmpty(void);	//167
	void  RemoveAll(void);	//221
	void  RemoveHead(void);	//223
	void  RemoveTail(void);	//225
};

//--------------------------------------
class CCtrlHolder
{
	//0xC4
public:
	DWORD m_Param1;
	RECT m_Rect;
	DWORD m_Param6;
	DWORD m_Param7;
	DWORD m_Param8;
	DWORD m_Param9;
	CString m_pFormula;
	CString m_pName;
	CString m_pCaption;
	DWORD m_ID;
	DWORD m_Param14;
	DWORD m_Param15;
	DWORD m_Param16;
	DWORD m_Param17;
	DWORD m_Param18;
	DWORD m_Param19;
	DWORD m_Param20;
	DWORD m_Param21;
	DWORD m_Param22;
	DWORD m_Param23;
	DWORD m_Param24;
	DWORD m_Param25;
	DWORD m_Param26;
	DWORD m_Param27;
	DWORD m_Param28;
	DWORD m_Param29;
	DWORD m_Param30;
	DWORD m_Param31;
	DWORD m_Param32;
	DWORD m_Param33;
	DWORD m_Param34;
	DWORD m_Param35;
	DWORD m_Param36;
	DWORD m_Param37;
	DWORD m_Param38;
	DWORD m_Param39;
	DWORD m_Param40;
	DWORD m_Param41;
	DWORD m_Param42;
	DWORD m_Param43;
	DWORD m_Param44;
	DWORD m_Param45;
	DWORD m_Param46;
	DWORD m_Param47;
	DWORD m_Param48;
	DWORD m_Param49;

};
//--------------------------------------

class CBrowseHolder
{
public:
	void* vtable;
	RECT rect;
};

class IMPORT_1C CEditDoc:public CDocument
{
DECLARE_DYNCREATE(CEditDoc)

protected:
	 CEditDoc(void);	//5
public:
	CString m_FileName;
	CString m_Profile;
	CString m_strToolTip;
	CString m_strHelpString;
	CString m_strDependParam;
	int m_isUseAutoFont;
	BOOL m_iTollBarEnable;
	BOOL m_iAutoSize;
	BOOL m_iPaletePopup;
	CString m_str1;
	CGalleryHolder* m_pGalery;
	class CDrawMaster *m_pDrawMaster;
	class CEditMaster *m_pEditMaster;
	CPtrArray m_ctrlArray;		// CCtrlType*
	CStringArray m_extListSA;	//char m_extList[0x28];		// CExtListSupport m_extList;	// (0x28 (40))
	CUIntArray m_extListUA;
	CStringArray m_extDocListSA;//char m_extDocList[0x28];	// CExtListSupport m_extDocList;	(0x28 (40))
	CUIntArray m_extDocListUA;
	CPtrArray m_extCtrlArray;	// CCtrlHolder*
	CPtrArray m_docCtrlArray;	// CBrowserSupport*
	CPtrList m_selList;			// CCtrlHolder*
	CPtrList m_objList;			// CCtrlHolder*
	class CMakeUndo *m_pUndo;
	class CMakeUndo *m_pRedo;
	enum  EdDocType m_TemplateType;
	class CBrowseHolder *m_brwsHolder;
	DWORD m_NextID;
	CStringArray m_layers;
	CDWordArray m_layersState;
	int m_iActiveLayer;
	CString m_title;
	CRect m_rect;
	LOGFONT m_font;
	CPictureHolder7 m_picture;
	DWORD m_bkColor;
	DWORD m_pictID;
	DWORD m_flags;
	long m_lDependParam;
	DWORD m_dw3;
	long m_iAutoLayout;
	
	
	virtual  ~CEditDoc(void);	//12
	void  AddLayer(LPCTSTR name,BOOL visible);	//30
protected:
	void  AdjustExtReportPicture(class CPictureHolder7 &,unsigned long &,class CCtrlHolder *);	//32
	void  AdjustExtReportPictureGallery(void);	//33
	void  AdjustTypedObj(class CBrowserSupport *,class CMetaDataTypedObj *);	//34
	void  AdjustTypedObj(class CCtrlHolder *,class CMetaDataTypedObj *);	//35
	void  AdjustTypedObj(class CCtrlHolder *,class CType *);	//36
public:
	int  CalcButtonWidth(int);	//37
	int  CalcColumnWidth(int);	//38
	int  CalcEditWidth(class CType const &);	//39
	int  CalcEditWidth(int);	//40
	int  CalcEditWidth(class CMetaDataTypedObj *);	//41
	void  CheckDefaultButton(class CCtrlHolder *,int);	//42
	void  ClearList(void);	//43
	void  CorrectSysMetrics(void);	//45
	void *  CreateButtonControl(class CRect const &,char const *,char const *,char const *);	//48
	HWND  CreateControl(class CWnd *,class CControlID *);	//49
protected:
	void  CreateGroupFrame(char const *,class CRect const &);	//50
public:
	int  CreateIndirect(class CString &);	//51
private:
	class CCtrlHolder *  CreatePermanentControl(class CRect const &,class CType const &,char const *,char const *,long,int,int,int);	//53
protected:
	class CCtrlHolder *  CreateStaticTitle(char const *,class CRect const &);	//54
private:
	void  DefineFixedControl(class CType const &,char const *,char const *,long,int);	//55
	void  DefineNewControl(class CType const &,char const *,char const *,long,int,int,int);	//56
public:
	void  DeleteLayer(int,char const *);	//59
	int  DialogToScreen(int,int);	//60
	void  DialogToScreen(class CPoint &);	//61
	void  DialogToScreen(class CRect &);	//62
protected:
	int  DoLoadExternal(char const *);	//64
	int  DoLoadLayout(char const *);	//65
public:
	void  FillContext(class CFormContextDef *);	//67
protected:
	class CBrowserSupport *  FindBrControl(class CBrowserSupport *);	//68
public:
	void *  FindByCode(char const *,class CType *);	//69
protected:
	class CCtrlHolder *  FindControl(class CCtrlHolder *);	//70
public:
	int  FindLayer(char const *);	//71
	void  FindTextA(char const *,int (__cdecl*)(class CString &,class CString &,long),long,int,int,int);	//72
	int  GetActiveLayer(void);	//75
	class CBrowseHolder *  GetBrowseHolder(void);	//78
	int  GetBrowser(class CControlID *);	//79
	int  GetColumnLen(class CType const &);	//82
	int  GetColumnLen(class CMetaDataTypedObj *);	//83
	int  GetControl(class CControlList *);	//85
	void  GetControlFont(void *,struct tagLOGFONTA *);	//86
	int  GetControlHAlign(void *);	//87
	void  GetControlLocation(void *,class CRect &);	//88
	class CTypedPtrArray<class CPtrArray,class CCtrlType *> *  GetCtrlArray(void);	//90
protected:
	virtual HMENU  GetDefaultMenu(void);	//99
public:
	long  GetDependParam(void);	//101
	class CTypedPtrArray<class CPtrArray,class CBrowserSupport *> *  GetDocCtrlArray(void);	//102
	class CDrawMaster *  GetDrawMaster(void);	//103
	class CEditMaster *  GetEditMaster(void);	//104
	class CTypedPtrArray<class CPtrArray,class CCtrlHolder *> *  GetExtCtrlArray(void);	//106
	class CExtListSupport *  GetExtDocList(void);	//107
	class CExtListSupport *  GetExtList(void);	//108
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//109
	char const *  GetFileName(void);	//110
	int  GetFrame(class CControlID *);	//114
	class CGalleryHolder *  GetGallery(void);	//115
	int  GetHeadControlList(int,class CString &,class CString &);	//118
	char const *  GetHelpString(void);	//121
	class CString   GetLayer(int);	//126
	int  GetLayerState(int);	//128
	int  GetLayersCount(void);	//129
	unsigned long  GetNextID(void);	//138
	class CTypedPtrList<class CPtrList,class CCtrlHolder *> *  GetObjList(void);	//139
	char const *  GetProfile(void);	//143
	class CMakeUndo *  GetRedo(void);	//144
	char const *  GetSDependParam(void);	//147
	class CTypedPtrList<class CPtrList,class CCtrlHolder *> *  GetSelList(void);	//148
	enum EdDocType   GetTemplateType(void);	//154
	char const *  GetToolTip(void);	//157
	class CMakeUndo *  GetUndo(void);	//158
	class CCtrlHolder *  Handle2Control(void *);	//160
protected:
	int  InitFromArchive(class CArchive &);	//164
public:
	int  IsAutoSize(void);	//166
	int  IsLayerActive(char const *);	//168
	int  IsLayerVisible(char const *);	//169
	int  IsPaletePopup(void);	//170
	int  IsToolBarEnable(void);	//171
	int  IsUseAutoFont(void);	//172
	int  LayoutControls(class CTypedPtrList<class CPtrList,class CCtrlHolder *> const &,int,int);	//173
	int  LoadFromStream(struct IStream *);	//174
	int  Merge2Way(class CEditDoc *,int);	//176
	int  ModifyContol(int,class CString,class CString);	//177
	void  MoveLayer(int,int);	//178
protected:
	void  OnAddColumn(class CMessage7 *,long *);	//180
public:
	virtual void  OnCloseDocument(void);	//183
protected:
	void  OnMergePictures(class CMessage7 *,long *);	//197
public:
	virtual int  OnNewDocument(void);	//199
protected:
	void  OnNotifyFontChange(class CMessage7 *,long *);	//200
	void  OnNotifyRecvisitChange(class CMessage7 *,long *);	//201
	void  OnNotifyRecvisitDelete(class CMessage7 *,long *);	//202
	void  OnNotifyRecvisitInsert(class CMessage7 *,long *);	//203
public:
	virtual int  OnOpenDocument(char const *);	//204
protected:
	void  OnRemoveColumn(class CMessage7 *,long *);	//207
public:
	void *  PlaceControl(int,void *,class CString &,class CType const &,char const *,char const *,int,int,int);	//213
	virtual void  ReleaseFile(class CFile *,int);	//220
	int  Replace(char const *,int,char const *,char const *);	//227
protected:
	virtual int  SaveModified(void);	//228
	int  SaveStringToArchive(class CArchive &,class CString const &);	//229
	int  SaveToArchive(class CArchive &);	//230
public:
	int  SaveToStream(struct IStream *);	//231
	int  ScreenToDialog(int,int);	//232
	void  ScreenToDialog(class CPoint &);	//233
	void  ScreenToDialog(class CRect &);	//234
	void  SelectFromUnicString(char const *);	//235
	virtual void  Serialize(class CArchive &);	//236
	void  SetActiveLayer(int);	//238
	void  SetAutoSize(int);	//239
	void  SetBrowseHolder(class CBrowseHolder *);	//241
	void  SetControlFont(void *,struct tagLOGFONTA *);	//244
	void  SetControlHAlign(void *,int);	//245
	void  SetControlLocation(void *,class CRect const &);	//246
	int  SetDWParam(void *,unsigned long);	//250
	void  SetDependParam(long);	//253
	void  SetDependParam(char const *);	//254
	int  SetFormula(void *,char const *);	//259
	void  SetGallery(class CGalleryHolder *);	//260
	void  SetHelpString(char const *);	//262
	void  SetLayer(int,char const *,int);	//266
	static void  SetLayoutStrategy(unsigned long);	//268
	virtual void  SetModifiedFlag(int);	//270
	void  SetPaletePopup(int);	//272
	int  SetStrParam(void *,char const *);	//275
	int  SetStyle(void *,unsigned long);	//277
	int  SetTitle(void *,char const *);	//279
	void  SetToolBarEnable(int);	//281
	void  SetToolTip(char const *);	//283
	int  SetType(void *,class CType const &);	//284
	void  SetUseFont(int);	//285
	int  ValidateAccelerator(struct ACCEL_LIGHT const &,class CCtrlHolder *);	//290
	int  ValidateCodeName(char const *,class CBrowserSupport *);	//291
	int  ValidateCodeName(char const *,class CCtrlHolder *);	//292
	static unsigned int  m_cfDialogEditor;	//303
protected:
	static unsigned long  m_dwLayoutMask;	//304

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CFntHolder7
{
public:
	CPtrList m_cPtrList;
	 CFntHolder7(void);	//6
	virtual  ~CFntHolder7(void);	//13
	void  AddFont(HFONT);	//26
	void  AddFont(class CFont *);	//27
	class CFont *  AddFontIndirect(struct tagLOGFONTA *);	//28
	void  RemoveAll(void);	//222
	void  RemoveHead(void);	//224
	void  RemoveTail(void);	//226
};


class IMPORT_1C CGallery:public CDialog
{
public:
	 CGallery(class CGalleryHolder *,unsigned long,class CWnd *);	//7
	virtual  ~CGallery(void);	//14
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//63
public:
	int  GetCurPosition(void)const;	//95
	class CGalleryHolder *  GetGalleryHolder(void)const;	//116
protected:
	virtual void  OnCancel(void);	//181
	void  OnClose(void);	//182
	void  OnDblclkEdGalleryPan(void);	//185
	void  OnEdChoosePicture(void);	//188
	void  OnEdDeletePicture(void);	//189
	void  OnEdIdendificator(void);	//190
	void  OnEdInsertPicture(void);	//191
	void  OnEdRemovePicture(void);	//192
	void  OnGetMinMaxInfo(struct tagMINMAXINFO *);	//194
	virtual int  OnInitDialog(void);	//195
	void  OnSavePict(void);	//208
	void  OnSize(unsigned int,int,int);	//210
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//216
protected:
	void  SetToFit(void);	//280

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CGalleryCtrl:public CWnd
{
public:
	 CGalleryCtrl(void);	//8
	virtual  ~CGalleryCtrl(void);	//15
	virtual int  CompareItem(struct tagCOMPAREITEMSTRUCT *);	//44
	int  Create(unsigned long,struct tagRECT const &,class CWnd *,unsigned int);	//46
	virtual void  DeleteItem(struct tagDELETEITEMSTRUCT *);	//58
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//66
	int  GetCurColumn(void);	//93
	int  GetCurHeight(void);	//94
	int  GetCurPosition(void);	//96
	int  GetCurWidth(void);	//97
protected:
	unsigned long  GetCurrentID(void);	//98
public:
	class CImageList const *  GetImageList(void)const;	//122
	class CDWordArray const *  GetImageMap(void)const;	//123
protected:
	char const *  GetNameByPos(int);	//135
public:
	class CItemList const *  GetNameList(void)const;	//136
	virtual void  MeasureItem(struct tagMEASUREITEMSTRUCT *);	//175
protected:
	void  OnContextMenu(class CWnd *,class CPoint);	//184
	void  OnDestroy(void);	//186
	void  OnDropFiles(HDROP);	//187
	void  OnLButtonDown(unsigned int,class CPoint);	//196
	void  OnMouseMove(unsigned int,class CPoint);	//198
	void  OnRButtonDown(unsigned int,class CPoint);	//206
	void  OnSelchange(void);	//209
	void  OnSize(unsigned int,int,int);	//211
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//212
	virtual void  PreSubclassWindow(void);	//215
public:
	void  Refresh(void);	//218
	void  SetCurPosition(int);	//249
	class CImageList const *  SetImageList(class CImageList const *);	//263
	class CDWordArray const *  SetImageMap(class CDWordArray const *);	//264
	class CItemList const *  SetNameList(class CItemList const *);	//271
	void  UpdateRemove(void);	//288
	virtual int  VKeyToItem(unsigned int,unsigned int);	//289

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C COnLineHelp:public CWnd
{
public:
	DWORD m_Flag1;
	DWORD m_Flag2;
	DWORD m_Flag3;
	DWORD m_Flag4;
	DWORD m_Flag5;
	DWORD m_Flag6;
	DWORD m_Flag7;
	 COnLineHelp(void);	//9
	virtual  ~COnLineHelp(void);	//16
	void  Activate(int);	//25
	virtual int  Create(class CWnd *);	//47
	int  DelItem(class CWnd *,unsigned int);	//57
	int  GetItemCount(class CWnd *);	//124
	int  GetItemText(class CWnd *,class CString &,unsigned int);	//125
	int  IsActive(void);	//165
protected:
	void  OnActivate(unsigned int,class CWnd *,int);	//179
	long  OnFloatStatus(unsigned int,long);	//193
	void  OnPaint(void);	//205
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//214
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//217
	int  RelayEventSayAbout(HWND,class CPoint const &);	//219
	void  SetItem(class CWnd *,char const *,unsigned int);	//265

	DECLARE_MESSAGE_MAP()
};

// End class descriptions
// Begin free functions definitions
IMPORT_1C struct ACCEL_LIGHT const & __cdecl GetAccelLightNil(void);
IMPORT_1C void __cdecl InitDialogEditor(void);
// End free functions definitions
