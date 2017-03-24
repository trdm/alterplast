// Begin forward declarations
typedef CArray<class CSheetOutline,class CSheetOutline const &> type_1;
typedef CArray<int,int> type_2;
enum CSheetDWord{
	shdFontNum=0,
	shdFontSize=1,		// -4*size
	shdFontBold=2,		// 7 - bold
	shdFontItalic=3,	// 1|0
	shdFontUnderline=4,	// 1|0
	shdBorderLeft=5,	// 1|0
	shdBorderRight=6,	// 1|0
	shdBorderTop=7,		// 1|0
	shdBorderBottom=8,	// 1|0
	shdBorderColor=9,	// num in palette
	shdHAlign=12,		// 0-left, 2-right, 4-width, 6-center +32-in sel column
	shdVAlign=13,		// 0 - верх 8 - низ 24-центр
	shdTextColorNum=14,	// num in palette
	shdBackColorNum=15,	// num in palette
	shdPatternNum=16,	// num
	shdPattColorNum=17,	// num in palette
	shdWrap=18,			// 0 auto, 1-обрезать, 2-забивать, 3-переносить, 4-красный,5-заб+кр
	shdDataFormat=19,	// 0 текст, 1-выражение, 2 - шаблон, 3-фшаблон
	shdUnprotect=20,	// 1|0
};
enum CSheetString{
	shStrText,
	shStrFormula,
};
enum CSheetDirection{
	shDirH=0,
	shDirV=1
};
enum CSheetSaveAsType{
	shMxl,
	shExc,
	shHtml,
	shText,
};
// End forward declarations

// Begin class descriptions


class CSheetFormat:public CObject
{
DECLARE_DYNCREATE(CSheetFormat)

public:
	char buf[32];
	 CSheetFormat(class CSheetFormat const &);	//9
	 CSheetFormat(void);	//10
	virtual  ~CSheetFormat(void);	//38
	class CSheetFormat &  operator=(class CSheetFormat const &);	//48
	void  Apply(class CSheetFormat const &,unsigned long);	//91
	class CSheetFormat &  Assign(class CSheetFormat const &);	//97
protected:
	void  ClearFlag(enum CSheetDWord);	//111
	void  ClearFlag(enum CSheetString);	//112
public:
	unsigned long  Compare(class CSheetFormat const &);	//118
	unsigned char  GetBkColor(void)const;	//184			+
	unsigned char  GetBorderBottom(void)const;	//185		+
	unsigned char  GetBorderColor(void)const;	//186		+
	unsigned char  GetBorderLeft(void)const;	//187		+
	unsigned char  GetBorderRight(void)const;	//188		+
	unsigned char  GetBorderTop(void)const;	//189			+
	unsigned char  GetBorders(void)const;	//190			?
	unsigned char  GetDataFormat(void)const;	//216		+
	unsigned char  GetDontPrint(void)const;	//223
	unsigned char  GetFontBold(void)const;	//236			+
	unsigned char  GetFontItalic(void)const;	//239		+
	unsigned short  GetFontName(void)const;	//241			+?
	unsigned short  GetFontSize(void)const;	//242			+
	unsigned char  GetFontUnderline(void)const;	//243		+
	unsigned short  GetHeight(void)const;	//255			
	unsigned char  GetHorzAlign(void)const;	//256			+
	unsigned char  GetLineStyle(void)const;	//259
	unsigned char  GetLineWeight(void)const;	//260
	unsigned char  GetPattern(void)const;	//281			+
	unsigned char  GetPatternColor(void)const;	//282		+
	int  GetPropertiesCount(void)const;	//285
	unsigned char  GetTextColor(void)const;	//332			+
	int  GetUnprotected(void)const;	//338					+
	unsigned char  GetVertAlign(void)const;	//339			+
	unsigned short  GetWidth(void)const;	//343
	unsigned char  GetWrapText(void)const;	//345			+
	int  HasProperties(void)const;	//347
protected:
	int  IsFlagSet(enum CSheetDWord)const;	//377
	int  IsFlagSet(enum CSheetString)const;	//378
public:
	int  IsPropertySet(enum CSheetDWord)const;	//386
	int  LookupProperty(enum CSheetDWord,unsigned long &)const;	//410
	void  RemoveAllProperties(void);	//618
	int  RemoveProperty(enum CSheetDWord);	//626
	virtual void  Serialize(class CArchive &);	//663
protected:
	void  SetFlag(enum CSheetDWord);	//676
	void  SetFlag(enum CSheetString);	//677
public:
	void  SetProperty(enum CSheetDWord,unsigned long);	//696
};


class CSheet:public CSheetFormat
{
DECLARE_DYNCREATE(CSheet)

public:
	char buf[508];
	 CSheet(class CSheet const &);	//1
	 CSheet(void);	//2
	virtual  ~CSheet(void);	//34
	void  ApplyFormatting(class CSheetSelection const &,class CSheetCell const &,unsigned long);	//92
	void  Clear(class CSheetSelection const &,unsigned long);	//107
	void  ClearCellContents(class CSheetSelection const &);	//108
	void  ClearCells(class CSheetSelection const &,unsigned long);	//109
	class CSheet &  Copy(class CSheet const &);	//119
	void  CopyCell(class CSheetCell &,class CSheet const &,class CSheetCell const &);	//120
	void  CopyCells(class CSheet &,class CSheetSelection const &)const;	//121
	void  CopySheet(class CSheet &,class CSheetSelection const &)const;	//123
	long  CreateValue(struct ISheetValue * *);	//134
	void  Delete(class CSheetSelection const &,int);	//135
	void  DeleteCells(class CSheetSelection const &,int);	//137
	void  DeleteDrawing(class CSheetDrawing *);	//139
	void  DeleteDrawings(class CList<class CSheetDrawing *,class CSheetDrawing *> *);	//140
	void  DeleteName(class CString const &);	//141
	int  FindCell(char const *,unsigned long,struct tagPOINT *,struct tagRECT const *);	//169
	class CSheetDrawing *  FindDrawingByID(unsigned long);	//170
	int  FindGroup(enum CSheetDirection,int,int,class CSheetSelection *);	//171
	int  FindGroup(enum CSheetDirection,char const *,class CSheetSelection *);	//172
	int  FindPageBreak(enum CSheetDirection,int,int *)const;	//174
	void  Fire_OnChange(class CSheetNamedItem const &);	//175
	void  Fire_OnClick(class CSheetNamedItem const &,char const *);	//176
	void  Fire_OnClose(void);	//177
	void  Fire_OnLoad(void);	//178
	class CRect   GetAcrossFormattingRect(int,int);	//181
	class CSheetCell &  GetCell(int,int);	//191
	void  GetCellAttributes(int,int,class CSheetFormat &)const;	//192
	void  GetCellFontAttributes(int,int,struct tagLOGFONTA &)const;	//193
	class CSheetFormat &  GetCol(int);	//200
	class CSheetFormat &  GetColAt(int,int *)const;	//201
	int  GetColCount(void)const;	//202
	void  GetColFormat(int,class CSheetFormat &)const;	//203
	int  GetColKeyAt(int)const;	//204
	class CArray<class CSheetOutline,class CSheetOutline const &> &  GetColOutline(void);	//205
	int  GetColWidth(int);	//206
	class CSortArray<int,int,class CSheetFormat *,class CSheetFormat *> const &  GetColumns(void)const;	//210
	void  GetDefFormat(class CSheetFormat &)const;	//217
	class CSheetDoc *  GetDocument(void);	//221
	class CList<class CSheetDrawing *,class CSheetDrawing *> const &  GetDrawings(void)const;	//227
	void  GetDrawingsInRect(class CRect const &,class CList<class CSheetDrawing *,class CSheetDrawing *> &,int)const;	//228
	void  GetFontAttributes(class CSheetFormat const *,struct tagLOGFONTA &)const;	//235
	int  GetFontKey(struct tagLOGFONTA const &);	//240
	class CSortArray<int,int,struct tagLOGFONTA,struct tagLOGFONTA &> const &  GetFonts(void)const;	//244
	class CSheetCell &  GetFooter(void);	//245
	unsigned long  GetFormat(class CList<class CSheetDrawing *,class CSheetDrawing *> const &,class CSheetCell &);	//246
	unsigned long  GetFormat(class CSheetSelection const &,class CSheetCell &);	//247
	int  GetGroupPart(char const *,class CSheetSelection &,enum CSheetGroupPart);	//250
	int  GetGroupPart(enum CSheetDirection,int,class CSheetSelection &,enum CSheetGroupPart);	//251
	int  GetGroupPart(enum CSheetDirection,char const *,class CSheetSelection &,enum CSheetGroupPart);	//252
	void  GetGroupSelection(enum CSheetDirection,int,class CSheetSelection &);	//253
	class CSheetCell &  GetHeader(void);	//254
	static int  GetLineCountOfString(char const *);	//258
	class CSortArray<int,int,class CString,char const *> const &  GetMasks(void)const;	//263
	class CRect   GetMerge(int,int)const;	//264
	class CList<class CRect,struct tagRECT const *> &  GetMerges(void);	//265
	class CSheetNames const &  GetNames(void)const;	//269
	class CArray<class CSheetOutline,class CSheetOutline const &>& GetOutline(enum CSheetDirection);	//277
	int  GetPageBreakAt(enum CSheetDirection,int)const;	//278
	int  GetPageBreakCount(enum CSheetDirection)const;	//279
	class CArray<int,int> &  GetPageBreaks(enum CSheetDirection);	//280
	class CSheetRow &  GetRow(int);	//290
	class CSheetRow &  GetRowAt(int,int *)const;	//291
	int  GetRowCount(void)const;	//292
	void  GetRowFormat(int,class CSheetFormat &)const;	//293
	int  GetRowKeyAt(int)const;	//296
	class type_1 &  GetRowOutline(void);	//297
	class CSortArray<int,int,class CSheetRow *,class CSheetRow *> const &  GetRows(void)const;	//299
	void  GetSelectionRects(class CSheetSelection const &,int,class CRect &,class CRect &,class CRect &)const;	//318
	struct ISheetHook *  GetSheetHook(void);	//322
	long  GetSheetObject(struct ISheetObject * *);	//323
	class CSize   GetSize(void)const;	//324
	static void  GetStandardFont(struct tagLOGFONTA &);	//325
	int  Group(class CSheetSelection const &,enum CSheetDirection);	//346
	void  Insert(class CSheetSelection const &,int);	//358
	void  InsertCells(class CSheetSelection const &,int);	//360
	void  InsertDrawing(class CSheetDrawing *,class CSheetDrawing *);	//361
	class CSheetDrawing *  InsertOleObject(class CSheetRect const &,struct _GUID const &);	//363
	static int  IsCellMatch(class CSheetCell *,char const *,unsigned long);	//369
	static int  IsStringMatch(char const *,char const *,unsigned long);	//390
	void  LoadFromString(char const *);	//396
	int  LookupCell(int,int,class CSheetCell * &)const;	//401
	int  LookupCol(int,class CSheetFormat * &,int *)const;	//402
	int  LookupColKey(int,int *)const;	//403
	int  LookupMerge(int,int,class CRect &)const;	//405
	int  LookupMerge(class CRect,class CRect &)const;	//406
	int  LookupName(class CString const &,class CSheetNamedItem &)const;	//407
	int  LookupRow(int,class CSheetRow * &,int *)const;	//411
	int  LookupRowKey(int,int *)const;	//412
	void  Merge(struct tagRECT const *);	//414
protected:
	void  MergeRect(class CRect const &);	//415
public:
	void  ModifyColKey(int,int);	//416
	void  ModifyRowKey(int,int);	//418
	void  MoveDrawing(class CSheetDrawing *,class CSheetRect const &);	//419
	void  OutlineSelection(class CSheetSelection const &,class CSheetFormat const &);	//582
	void  PasteCells(class CSheet const &,class CSheetSelection const &,unsigned long);	//595
	void  PasteCells(class CSheet const &,int,int,enum CSheetSelectType,unsigned long);	//596
	void  PasteSheet(class CSheet const &,class CSheetSelection const &,unsigned long);	//599
	void  PasteSheet(class CSheet const &,int,int,enum CSheetSelectType,unsigned long);	//600
	static char const *  ReadClipboardString(char const *,class CString &);	//610
	void  RemoveAll(void);	//613
	void  RemoveAllCols(void);	//615
	void  RemoveAllDrawings(void);	//616
	void  RemoveAllRows(void);	//619
	int  RemoveCol(int);	//621
	void  RemovePageBreak(enum CSheetDirection,int);	//623
	int  RemoveRow(int);	//627
	static void  ReplaceCellText(class CSheetCell *,char const *,unsigned long,char const *);	//630
	static class CString   ReplaceString(char const *,char const *,unsigned long,char const *);	//631
	void  SaveAsString(class CArchive &);	//633
	virtual void  Serialize(class CArchive &);	//659
	void  SetColFormat(int,class CSheetFormat const &);	//668
	void  SetDefFormat(class CSheetFormat const &);	//672
	void  SetName(class CString const &,class CSheetNamedItem const &);	//681
	void  SetPageBreak(enum CSheetDirection,int);	//686
	void  SetRowFormat(int,class CSheetFormat const &);	//704
	long  SetSheetHook(struct ISheetHook *);	//710
	void  SetSize(class CSize const &);	//711
	static void  SetStandardFont(struct tagLOGFONTA &);	//712
	void  UnGroup(class CSheetSelection const &,enum CSheetDirection);	//731
	int  UnMerge(int,int);	//732
	void  UnMerge(class CSheetSelection const &,int);	//733
protected:
	void  UnMergeRect(class CRect const &);	//734
public:
	static void  WriteClipboardString(class CArchive &,char const *);	//739
	static unsigned short const  m_cfDrawing;	//793
	static unsigned short const  m_cfEmbeddedObject;	//794
	static unsigned short const  m_cfObjectDescriptor;	//795
	static unsigned short const  m_cfSheet;	//796
	static unsigned short const  m_cfSheetDescriptor;	//797
	static unsigned short const  m_cfSheetOld;	//798
	static struct _GUID const  m_clsidChart;	//799
	static struct _GUID const  m_guidCol;	//800
	static struct _GUID const  m_guidDrawing;	//801
	static struct _GUID const  m_guidRect;	//802
	static struct _GUID const  m_guidRow;	//803
	static struct _GUID const  m_guidSheet;	//804
	static struct tagLOGFONTA  m_lfStandardFont;	//805
};


class CSheetCell:public CSheetFormat
{
DECLARE_DYNCREATE(CSheetCell)

public:
	char buf[32];
	 CSheetCell(class CSheetCell const &);	//3
	 CSheetCell(void);	//4
	virtual  ~CSheetCell(void);	//35
	class CSheetCell &  operator=(class CSheetCell const &);	//46
	void  Apply(class CSheetCell const &,unsigned long);	//89
	void  Apply(class CSheetFormat const &,unsigned long);	//90
	class CSheetCell &  Assign(class CSheetCell const &);	//95
	char const *  GetFormula(void);	//248
	int  GetPropertiesCount(void)const;	//284
	char const *  GetText(void);	//331
protected:
	void  Init(void);	//356
public:
	int  IsPropertySet(enum CSheetDWord)const;	//384
	int  IsPropertySet(enum CSheetString)const;	//385
	long  LoadValue(struct ISheetValue *);	//398
	int  LookupProperty(enum CSheetDWord,unsigned long &)const;	//408
	int  LookupProperty(enum CSheetString,class CString &)const;	//409
	void  RemoveAllProperties(void);	//617
	int  RemoveProperty(enum CSheetDWord);	//624
	int  RemoveProperty(enum CSheetString);	//625
	long  SaveValue(struct ISheetValue *);	//636
	virtual void  Serialize(class CArchive &);	//660
	void  SetProperty(enum CSheetDWord,unsigned long);	//694
	void  SetProperty(enum CSheetString,char const *);	//695
};


class CSheetDoc:public COleLinkingDoc
{
DECLARE_DYNCREATE(CSheetDoc)

public:
	CSheet m_sheet;
	char buf[280];
	 CSheetDoc(void);	//5
	virtual  ~CSheetDoc(void);	//36
	void  BroadcastSheetClicked(char const *,class CSheetNamedItem const &);	//104
	virtual void  DeleteContents(void);	//138
	virtual int  DoFileSave(void);	//145
protected:
	virtual int  DoSave(char const *,int);	//150
public:
	static unsigned short  GetArchiveVersionField(class CArchive &,unsigned int);	//182
	int  GetCopiesPerPage(void);	//211
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//230
public:
	class CRect   GetMarginsRect(void);	//262
	int  GetNoSavePrompt(void)const;	//270
	int  GetReadOnly(void)const;	//287
	class CSheetUndo &  GetUndo(void);	//336
	int  GetViewCount(void)const;	//340
	class CString const &  GetWindowKey(void)const;	//344
	void  LoadProfile(void);	//397
protected:
	void  OnAppFontChange(class CMessage7 *,long *);	//427
	void  OnAppPageChange(class CMessage7 *,long *);	//428
public:
	virtual void  OnCloseDocument(void);	//435
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//437
protected:
	void  OnEditConvert(void);	//452
	void  OnFileSave(void);	//473
	void  OnFileSaveAs(void);	//474
	virtual int  OnNewDocument(void);	//494
public:
	virtual int  OnSaveDocument(char const *);	//505
protected:
	void  OnUpdateFileSave(class CCmdUI *);	//541
	void  OnUpdateFileSaveAs(class CCmdUI *);	//542
	void  OnUpdateObjectVerbMenu(class CCmdUI *);	//547
	virtual void  ReleaseFile(class CFile *,int);	//612
public:
	int  SaveAs(char const *,enum CSheetSaveAsType);	//632
protected:
	virtual int  SaveModified(void);	//634
public:
	void  SaveProfile(void);	//635
	virtual void  Serialize(class CArchive &);	//661
	virtual void  SetModifiedFlag(int);	//680
	void  SetNoSavePrompt(int);	//683
	virtual void  SetPathName(char const *,int);	//687
	void  SetProfileKey(char const *);	//693
	void  SetReadOnly(int);	//698
	void  SetUserTitle(char const *);	//713
	void  SetWindowKey(char const *);	//714

	DECLARE_MESSAGE_MAP()
};


class CSheetDrawing:public CObject
{
DECLARE_DYNCREATE(CSheetDrawing)

public:
	 CSheetDrawing(class CSheetDrawing const &,class COleDocument *);	//6
	 CSheetDrawing(enum CSheetDrawType);	//7
	 CSheetDrawing(void);	//8
	virtual  ~CSheetDrawing(void);	//37
	class CSheetDrawing &  operator=(class CSheetDrawing const &);	//47
//	void  `copy constructor closure'(class CSheetDrawing const &);	//83
	class CSheetDrawing &  Assign(class CSheetDrawing const &);	//96
	class CRect   GetDisplRect(void)const;	//220
	unsigned long  GetID(void)const;	//257
	struct IDispatch *  GetObjectDispatch(void);	//274
	class CSheetRect const &  GetRect(void)const;	//288
	enum CSheetDrawType   GetType(void)const;	//333
	virtual void  Serialize(class CArchive &);	//662
	void  SetDisplRect(class CRect const &);	//674
	void  SetRect(class CSheetRect const &);	//699
};


class CSheetGDI
{
public:
	 CSheetGDI(void);	//11
	virtual  ~CSheetGDI(void);	//39
	void  Attach(class CSheetView *);	//101
	void  ClearFontCache(void);	//113
	void  ClearHeightCache(void);	//114
	void  CreateDisplayInfo(void);	//125
	int  CreatePrinterInfo(void);	//133
protected:
	void  Destroy(void);	//143
public:
	void  DrawBorder(class CDC *,class CRect,int,unsigned long,unsigned long);	//151
	void  DrawLine(class CDC *,class CRect,int,int,unsigned long);	//154
	static void  DrawPageBreak(class CDC *,class CRect,unsigned long,unsigned long,enum CSheetBreakStyle);	//159
	static void  FillRect(class CDC *,class CRect const &,unsigned long);	//168
	class CRect   GetCellRect(int,int);	//194
	static int  GetClipRgn(class CDC *,class CRgn &);	//198
	int  GetColumnStart(int);	//207
	int  GetColumnWidth(int);	//208
protected:
	int  GetColumnWidthUni(int);	//209
public:
	class CFont *  GetFont(struct tagLOGFONTA const &);	//234
protected:
	int  GetFontCachePos(struct tagLOGFONTA const &);	//237
public:
	int  GetFontHeight(struct tagLOGFONTA const &);	//238
	class CSize   GetGapSize(void)const;	//249
	class CSize   GetLogPixels(void);	//261
	int  GetRowHeight(int);	//294
protected:
	int  GetRowHeightUni(int);	//295
public:
	int  GetRowStart(int);	//298
	class CSize   GetRulerSize(void)const;	//300
	int  GetScale(void)const;	//311
	class CSize   GetSectionSize(void)const;	//313
	class CSize   GetStdCharSize(void);	//327
	unsigned long  GetSysColor(int);	//328
	struct tagTEXTMETRICA const &  GetTM(struct tagLOGFONTA const &);	//329
	int  IsEast(void);	//373
	int  IsNorth(void);	//383
	int  IsSouth(void);	//389
	int  IsWest(void);	//393
	static class CRect   Justify(class CDC *,char const *,class CRect,int,int,int,struct tagRECT const *);	//394
	void  PTtoRC(class CPoint,int &,int &,struct tagRECT *);	//584
	static void  PatFillRect(class CDC *,class CRect const &,unsigned long,unsigned long,int,class CPoint);	//601
	void  SetPrintHeaders(int);	//692
	void  SetScale(int);	//706
protected:
	void  UpdateDeviceMetrics(void);	//738
public:
	int  XCharToPixel(int);	//740
	int  XMillimetersToPixel(int);	//742
	int  XPixelToChar(int);	//744
	int  XPixelToMillimeters(int);	//745
	int  XPixelToUnit(int);	//747
	int  XUnitToPixel(int);	//749
	int  YCharToPixel(int);	//751
	int  YMillimetersToPixel(int);	//753
	int  YPixelToChar(int);	//755
	int  YPixelToMillimeters(int);	//756
	int  YPixelToUnit(int);	//758
	int  YUnitToPixel(int);	//760
};


class CSheetNamedItem:public CObject
{
DECLARE_DYNAMIC(CSheetNamedItem)

public:
	DWORD buf[10];
	 CSheetNamedItem(class CSheetNamedItem const &);	//12
	 CSheetNamedItem(class CSheetSelection const &);	//13
	 CSheetNamedItem(class CSheetDrawing const *);	//14
	 CSheetNamedItem(void);	//15
	virtual  ~CSheetNamedItem(void);	//40
	class CSheetNamedItem &  operator=(class CSheetNamedItem const &);	//49
	bool  operator==(class CSheetNamedItem const &)const;	//57
	bool  operator!=(class CSheetNamedItem const &)const;	//61
	class CSheetNamedItem &  Assign(class CSheetNamedItem const &);	//98
	class CString   DisplayString(void)const;	//144
	unsigned long  GetDrawID(void)const;	//224
	class CSheetSelection const &  GetRange(void)const;	//286
	enum CSheetNameType   GetType(void)const;	//334
	bool  IsEqual(class CSheetNamedItem const &)const;	//375
	int  ParseString(char const *);	//594
	virtual void  Serialize(class CArchive &);	//664
	void  SetDrawID(unsigned long);	//675
	void  SetRange(class CSheetSelection const &);	//697
};


class CSheetNames:public CObject
{
DECLARE_DYNAMIC(CSheetNames)

public:
	DWORD buf[100];
	struct strihash{};
	struct strieq{};
	CSheetNames(void);	//16
	virtual  ~CSheetNames(void);	//41
	class CSheetNames &  operator=(class CSheetNames const &);	//50
	virtual void  Serialize(class CArchive &);	//665
//	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   begin(void)const;	//776
	void  clear(void);	//787
//	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   end(void)const;	//788
	void  erase(class CString const &);	//789
//	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   find(class CString const &)const;	//790
//	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   find_item(class CSheetNamedItem const &)const;	//791
	void  insert(class CString const &,class CSheetNamedItem const &);	//792
};


class CSheetOutline
{
public:
	char buf[12];
	CString name;
	 CSheetOutline(class CSheetOutline const &);	//17
	 CSheetOutline(int,int,int,char const *);	//18
	 CSheetOutline(void);	//19
	virtual  ~CSheetOutline(void);	//42
	class CSheetOutline &  operator=(class CSheetOutline const &);	//51
	class CSheetOutline &  Assign(class CSheetOutline const &);	//99
	class CString const &  GetName(void)const;	//268
	void  SetName(char const *);	//682
};


class CSheetPoint
{
public:
	 CSheetPoint(struct SHEETPOINT const &);	//20
	 CSheetPoint(class CSheetPoint const &);	//21
	 CSheetPoint(int,int,int,int);	//22
	 CSheetPoint(class CPoint,class CPoint);	//23
	 CSheetPoint(void);	//24
	class CSheetPoint &  operator=(struct SHEETPOINT const &);	//52
	class CSheetPoint &  operator=(class CSheetPoint const &);	//53
	int  operator==(class CSheetPoint const &)const;	//58
	int  operator!=(class CSheetPoint const &)const;	//62
	class CSheetPoint   operator-(class CSheetPoint const &)const;	//66
	class CSheetPoint   operator-(void)const;	//67
	class CSheetPoint   operator+(class CSheetPoint const &)const;	//68
	void  operator+=(class CSheetPoint const &);	//69
	void  operator-=(class CSheetPoint const &);	//70
	void  Offset(class CSheetPoint const &);	//422
	void  Offset(class CPoint,class CPoint);	//423
	class CSheetPoint &  SetPoint(struct SHEETPOINT const &);	//688
	class CSheetPoint &  SetPoint(class CSheetPoint const &);	//689
	class CSheetPoint &  SetPoint(int,int,int,int);	//690
	class CSheetPoint &  SetPoint(class CPoint,class CPoint);	//691
};


class CSheetRect
{
public:
	 CSheetRect(class CSheetRect const &);	//25
	 CSheetRect(class CSheetPoint const &,class CSheetPoint const &);	//26
	 CSheetRect(class CRect);	//27
	 CSheetRect(void);	//28
	class CSheetRect &  operator=(class CSheetRect const &);	//54
	int  operator==(class CSheetRect const &)const;	//59
	int  operator!=(class CSheetRect const &)const;	//63
	 operator struct SHEETRECT &(void);	//65
	class CSheetPoint   BottomLeft(void)const;	//102
	class CSheetPoint   BottomRight(void)const;	//103
	int  DeleteRect(class CRect const &,enum CSheetDirection);	//142
	int  InsertRect(class CRect const &,enum CSheetDirection);	//364
	int  IntersectRect(class CSheetRect const &,class CSheetRect const &);	//366
	int  IsRectNull(void)const;	//387
	void  NormalizeRect(void);	//421
	void  OffsetRect(class CSheetPoint const &);	//425
	int  PtInRect(class CSheetPoint const &);	//609
	class CSheetRect &  SetRect(class CSheetRect const &);	//700
	class CSheetRect &  SetRect(class CSheetPoint const &,class CSheetPoint const &);	//701
	void  SetRectEmpty(void);	//702
	int  SubtractRect(class CSheetRect const &,class CSheetRect const &);	//727
	class CSheetPoint   TopLeft(void)const;	//728
	class CSheetPoint   TopRight(void)const;	//729
	int  UnionRect(class CSheetRect const &,class CSheetRect const &);	//736
};


class CSheetRow:public CSheetFormat
{
DECLARE_DYNCREATE(CSheetRow)

public:
	char buf[44];
	 CSheetRow(class CSheetRow const &);	//29
	 CSheetRow(void);	//30
	virtual  ~CSheetRow(void);	//43
	class CSheetRow &  operator=(class CSheetRow const &);	//55
	class CSheetRow &  Assign(class CSheetRow const &);	//100
	class CSheetCell &  ElementAt(int,int *);	//164
	class CSheetCell *  GetAt(int,int *)const;	//183
	int  GetCount(void)const;	//212
	int  KeyAt(int)const;	//395
	int  Lookup(int,class CSheetCell * &,int *)const;	//400
	int  LookupKey(int,int *)const;	//404
	void  ModifyKey(int,int);	//417
	void  RemoveAll(void);	//614
	void  RemoveAt(int);	//620
	int  RemoveKey(int);	//622
	virtual void  Serialize(class CArchive &);	//666
	int  SetKey(int,class CSheetCell *);	//678
};


class CSheetSelection:public CObject
{
DECLARE_DYNAMIC(CSheetSelection)

public:
	char buf[0x14];
	 CSheetSelection(class CSheetSelection const &);	//31
	 CSheetSelection(void);	//32
	virtual  ~CSheetSelection(void);	//44
	class CSheetSelection &  operator=(class CSheetSelection const &);	//56
	int  operator==(class CSheetSelection const &)const;	//60
	int  operator!=(class CSheetSelection const &)const;	//64
	int  Delete(class CSheetSelection const &,enum CSheetDirection);	//136
	int  GetEnd(void)const;	//229
	int  GetNormalEnd(void)const;	//271
	class CRect   GetNormalRect(void)const;	//272
	int  GetNormalStart(void)const;	//273
	class CRect const &  GetRect(void)const;	//289
	int  GetStart(void)const;	//326
	enum CSheetSelectType   GetType(void)const;	//335
	int  Insert(class CSheetSelection const &,enum CSheetDirection);	//359
	int  Intersect(class CSheetSelection const &,class CSheetSelection const &);	//365
	int  IsCellSelected(int,int)const;	//370
	int  IsEqual(class CSheetSelection const &)const;	//376
	void  Offset(int,int);	//424
	void  SelectCell(int,int);	//645
	void  SelectColumns(int,int);	//647
	void  SelectRect(class CRect const &);	//652
	void  SelectRect(int,int,int,int);	//653
	void  SelectRows(int,int);	//656
	void  SelectTable(void);	//657
	virtual void  Serialize(class CArchive &);	//667
	int  Subtract(class CSheetSelection const &,class CSheetSelection const &);	//726
	void  Union(class CSheetSelection const &,class CSheetSelection const &);	//735
};


class CSheetView:public CView
{
DECLARE_DYNCREATE(CSheetView)
public:
	enum BarTextState  {btsOne};
protected:
	 CSheetView(void);	//33
	virtual  ~CSheetView(void);	//45
public:
	void  ActivateObject(class CSheetDrawing *);	//84
	void  AddSelectionToTail(class CSheetSelection const &);	//85
protected:
	int  AdjustDeviceMode(class CPrintDialog *);	//86
	int  AdjustScaleToFitToPage(void);	//87
	void  AjustDragPoint(class CPoint &);	//88
public:
	void  ApplyFormatting(class CSheetCell const &,unsigned long);	//93
	void  ApplyTyping(class CSheetCell const &);	//94
	int  CanScrollLeft(void);	//105
	int  CanScrollUp(void);	//106
	void  ClearDrawingSelection(void);	//110
	void  ClearSelectedData(void);	//115
	void  ClearSelection(void);	//116
protected:
	void  ClosePopups(void);	//117
public:
	void  CopyDrawings(class CSheet &,class CList<class CSheetDrawing *,class CSheetDrawing *> &,class CPoint,class CSize &,class CPoint &);	//122
protected:
	int  CreateDeviceMode(class CPrintDialog *);	//124
	void  DoGroupDialog(enum CSheetDirection,int);	//146
	int  DoPaste(unsigned short);	//147
	unsigned short  DoPasteSpecialDialog(class COleDataObject *);	//148
	void  DoRenameGroup(enum CSheetDirection,int);	//149
	void  DrawDragger(class CDC *,class CPoint);	//152
	void  DrawGrid(class CDC *,struct tagRECT const *);	//153
	void  DrawObjectDragger(class CDC *,class CPoint);	//155
	void  DrawObjectHandles(class CSheetGDI &,class CDC *,struct tagRECT const *);	//156
public:
	void  DrawObjectTracker(class CDC *,int,class CPoint);	//157
protected:
	void  DrawObjects(class CSheetGDI &,class CDC *,struct tagRECT const *);	//158
	void  DrawRuler(class CDC *);	//160
	void  DrawSections(class CDC *,struct tagRECT const *);	//161
	void  DrawSelection(class CDC *);	//162
	void  DrawSheet(class CSheetGDI &,class CDC *,struct tagRECT const *);	//163
public:
	void  EnableGrid(int);	//165
	void  EnableHeaders(int);	//166
	void  EnableNames(int);	//167
protected:
	int  FindNext(char const *,unsigned long);	//173
public:
	class CFont *  FormatToFont(class CSheetFormat &);	//179
	void  FormatToLogFont(class CSheetFormat &,struct tagLOGFONTA &);	//180
	class CRect   GetCellRect(int,int);	//195
	enum CSheetSector   GetCellSector(int,int);	//196
	void  GetClipRect(class CRect &);	//197
protected:
	int  GetClipboardSheet(class COleDataObject *,class CSheet &,unsigned short);	//199
public:
	int  GetCurrentCol(void)const;	//213
	int  GetCurrentRow(void)const;	//214
	enum CSheetSector   GetCurrentSector(void);	//215
	class CRect   GetDefSectorRect(void);	//218
	class CRect   GetDisplDrawingRect(class CSheetDrawing *);	//219
	class CSheetDoc *  GetDocument(void)const;	//222
	class CList<class CSheetDrawing *,class CSheetDrawing *> *  GetDrawingSelection(void);	//225
	void  GetDrawingSelectionRect(class CSheetRect &);	//226
protected:
	class CRect   GetFindReplaceRect(int &,unsigned long);	//231
public:
	int  GetFirstCol(void)const;	//232
	int  GetFirstRow(void)const;	//233
protected:
	int  GetObjectInfo(class COleDataObject *,unsigned short,struct _GUID *,class CSize *,class CPoint *);	//275
	int  GetObjectSelection(struct _GUID,class CSize,class CPoint,class CSheetSelection &);	//276
public:
	class CProfile7 &  GetProfile(void)const;	//283
	class CPoint   GetScrollPosition(void)const;	//312
	class CRect   GetSectorRect(enum CSheetSector,unsigned int);	//314
	unsigned long  GetSelectionFormat(class CSheetCell &);	//315
	class CString   GetSelectionName(void);	//316
	class CRect   GetSelectionRect(class CSheetSelection *);	//317
	void  GetSelectionRgn(class CRgn &,class CList<class CSheetSelection,class CSheetSelection const &> *);	//319
	void  GetSelectionRgn(class CRgn &,class CSheetSelection *,int);	//320
	class CSheet &  GetSheet(void)const;	//321
	class CSheetSelection *  GetTailSelection(void);	//330
	class CSheetUndo &  GetUndo(void)const;	//337
	void  GetVisibleCells(class CRect &);	//341
protected:
	void  GetVisibleDrawings(class CSheetGDI &,class CRect const &,class CList<class CSheetDrawing *,class CSheetDrawing *> &);	//342
public:
	class CHitInfo   HitTest(class CPoint);	//348
	unsigned int  HitTestDragScroll(class CPoint);	//349
	class CSheetDrawing *  HitTestDrawing(class CPoint,class CHitInfo const &);	//350
	int  HitTestDrawingSelection(class CPoint,class CHitInfo const &);	//351
	int  HitTestObject(class CSheetDrawing *,class CRect const &,class CPoint);	//352
	int  HitTestSelection(class CPoint,class CHitInfo const &);	//353
	int  HitTestSelectionBorder(class CPoint,class CHitInfo const &);	//354
	int  HitTestTool(class CPoint,class CHitInfo const &);	//355
	int  InitObject(class CSheetDrawing *);	//357
	void  InsertDrawing(class CSheetDrawing *);	//362
	void  InvalidateDrawing(class CSheetDrawing *);	//367
	void  InvalidateSelection(class CList<class CSheetSelection,class CSheetSelection const &> *);	//368
	int  IsDrawingSelected(class CSheetDrawing *)const;	//371
	int  IsDrawingSelected(void)const;	//372
protected:
	int  IsEditMode(void);	//374
public:
	int  IsGridEnable(void);	//379
	int  IsHeadersEnable(void);	//380
protected:
	int  IsInPlaceActive(void);	//381
public:
	int  IsNamesEnable(void);	//382
	virtual int  IsSelected(class CObject const *)const;	//388
protected:
	int  IsTextBoxEditMode(void);	//391
	int  IsTextOnBar(class CString &,enum CSheetView::BarTextState &);	//392
	void  LoadViewState(class CSheetViewState &);	//399
public:
	void  MakeCellVisible(int,int,int);	//413
protected:
	void  MoveDrawings(class CPoint);	//420
	virtual void  OnActivateView(int,class CView *,class CView *);	//426
public:
	void  OnApplyBarText(class CString);	//429
protected:
	void  OnApplyProperties(struct tagNMHDR *,long *);	//430
	virtual void  OnBeginPrinting(class CDC *,struct CPrintInfo *);	//431
	void  OnCancelEditCntr(void);	//432
	void  OnCancelProperties(struct tagNMHDR *,long *);	//433
	void  OnClickRuler(unsigned int,class CHitInfo const &,class CPoint);	//434
	void  OnCloseupProperties(struct tagNMHDR *,long *);	//436
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//438
protected:
	void  OnContextMenu(class CWnd *,class CPoint);	//439
	int  OnCreate(struct tagCREATESTRUCTA *);	//440
	void  OnDestroy(void);	//441
public:
	virtual unsigned long  OnDragEnter(class COleDataObject *,unsigned long,class CPoint);	//442
	virtual void  OnDragLeave(void);	//443
	virtual unsigned long  OnDragOver(class COleDataObject *,unsigned long,class CPoint);	//444
	virtual unsigned long  OnDragScroll(unsigned long,class CPoint);	//445
protected:
	virtual void  OnDraw(class CDC *);	//446
public:
	virtual int  OnDrop(class COleDataObject *,unsigned long,class CPoint);	//447
protected:
	virtual unsigned long  OnDropEx(class COleDataObject *,unsigned long,unsigned long,class CPoint);	//448
	void  OnEditActivate(void);	//449
	void  OnEditClear(void);	//450
	void  OnEditClipMath(unsigned int);	//451
	void  OnEditCopy(void);	//453
	void  OnEditCut(void);	//454
	void  OnEditDelete(void);	//455
	void  OnEditDeleteContents(void);	//456
	void  OnEditFind(void);	//457
	void  OnEditFindBack(void);	//458
	void  OnEditFindForward(void);	//459
	void  OnEditFindReplace(int);	//460
	void  OnEditInsert(void);	//461
	void  OnEditMerge(void);	//462
	int  OnEditObjectPos(unsigned int);	//463
	void  OnEditPaste(void);	//464
	void  OnEditPasteSpecial(void);	//465
	void  OnEditProperties(void);	//466
	void  OnEditRepeat(void);	//467
	void  OnEditReplace(void);	//468
	int  OnEditSheetSize(unsigned int);	//469
	int  OnEditUndo(unsigned int);	//470
	virtual void  OnEndPrinting(class CDC *,struct CPrintInfo *);	//471
	void  OnFilePageSetup(void);	//472
	void  OnFindNext(char const *,unsigned long);	//475
	long  OnFindReplaceMsg(unsigned int,long);	//476
	void  OnHScroll(unsigned int,unsigned int,class CScrollBar *);	//477
	int  OnInPlaceEditorCmd(void);	//478
	int  OnInactiveMouse(class CSheetDrawing *,class CString &);	//479
	int  OnInactiveSetCursor(class CSheetDrawing *);	//480
	virtual void  OnInitialUpdate(void);	//481
	int  OnInsertHeader(unsigned int);	//482
	void  OnInsertLine(void);	//483
	void  OnInsertPicture(void);	//484
	void  OnInsertRect(void);	//485
	void  OnInsertTextBox(void);	//486
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//487
	void  OnKeyNext(int,int,int);	//488
	void  OnLButtonDblClk(unsigned int,class CPoint);	//489
	void  OnLButtonDown(unsigned int,class CPoint);	//490
	void  OnLButtonUp(unsigned int,class CPoint);	//491
	void  OnMouseMove(unsigned int,class CPoint);	//492
	int  OnMouseWheel(unsigned int,short,class CPoint);	//493
	void  OnOleInsertNew(void);	//495
	void  OnPaletteChanged(class CWnd *);	//496
public:
	virtual void  OnPrepareDC(class CDC *,struct CPrintInfo *);	//497
protected:
	virtual int  OnPreparePrinting(struct CPrintInfo *);	//498
	virtual void  OnPrint(class CDC *,struct CPrintInfo *);	//499
	int  OnQueryNewPalette(void);	//500
	void  OnRButtonDown(unsigned int,class CPoint);	//501
	void  OnRButtonUp(unsigned int,class CPoint);	//502
	void  OnReplaceAll(char const *,unsigned long,char const *);	//503
	void  OnReplaceSel(char const *,unsigned long,char const *);	//504
public:
	virtual int  OnScroll(unsigned int,unsigned int,int);	//506
protected:
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//507
	void  OnSetFocus(class CWnd *);	//508
	void  OnSheetBarEq(void);	//509
	int  OnSheetGroup(unsigned int);	//510
	int  OnSheetGroups(unsigned int);	//511
	void  OnSheetInsertChart(void);	//512
	void  OnSheetNames(void);	//513
	int  OnSheetPageBreak(unsigned int);	//514
	void  OnSheetParagraph(void);	//515
	void  OnSheetPrintClearRange(void);	//516
	void  OnSheetPrintClearRepeat(void);	//517
	void  OnSheetPrintRepeat(void);	//518
	void  OnSheetPrintSetRange(void);	//519
	int  OnSheetScale(unsigned int);	//520
	void  OnSize(unsigned int,int,int);	//521
	void  OnSysKeyDown(unsigned int,unsigned int,unsigned int);	//522
	void  OnTimer(unsigned int);	//523
	virtual void  OnUpdate(class CView *,long,class CObject *);	//524
	void  OnUpdateEditClear(class CCmdUI *);	//525
	void  OnUpdateEditClipMath(class CCmdUI *);	//526
	void  OnUpdateEditCopy(class CCmdUI *);	//527
	void  OnUpdateEditCut(class CCmdUI *);	//528
	void  OnUpdateEditDelete(class CCmdUI *);	//529
	void  OnUpdateEditDeleteContents(class CCmdUI *);	//530
	void  OnUpdateEditFind(class CCmdUI *);	//531
	void  OnUpdateEditInsert(class CCmdUI *);	//532
	void  OnUpdateEditMerge(class CCmdUI *);	//533
	void  OnUpdateEditObjectPos(class CCmdUI *);	//534
	void  OnUpdateEditPaste(class CCmdUI *);	//535
	void  OnUpdateEditProperties(class CCmdUI *);	//536
	void  OnUpdateEditRepeat(class CCmdUI *);	//537
	void  OnUpdateEditReplace(class CCmdUI *);	//538
	void  OnUpdateEditSheetSize(class CCmdUI *);	//539
	void  OnUpdateEditUndo(class CCmdUI *);	//540
	int  OnUpdateInPlaceEditorCmd(class CCmdUI *);	//543
	void  OnUpdateInsertDraw(class CCmdUI *);	//544
	void  OnUpdateInsertHeader(class CCmdUI *);	//545
	void  OnUpdateLayout(class CCmdUI *);	//546
	void  OnUpdateProperties(class CCmdUI *);	//548
	void  OnUpdateSheetBar(class CCmdUI *);	//549
	void  OnUpdateSheetBarEdit(class CCmdUI *);	//550
	void  OnUpdateSheetBarEq(class CCmdUI *);	//551
	void  OnUpdateSheetGroup(class CCmdUI *);	//552
	void  OnUpdateSheetNamesCombo(class CCmdUI *);	//553
	void  OnUpdateSheetPageBreak(class CCmdUI *);	//554
	void  OnUpdateSheetParagraph(class CCmdUI *);	//555
	void  OnUpdateSheetPrintRepeat(class CCmdUI *);	//556
	void  OnUpdateSheetPrintSetRange(class CCmdUI *);	//557
	void  OnUpdateSheetScale(class CCmdUI *);	//558
	void  OnUpdateViewAutoBreaks(class CCmdUI *);	//559
	void  OnUpdateViewGrid(class CCmdUI *);	//560
	void  OnUpdateViewHeaders(class CCmdUI *);	//561
	void  OnUpdateViewReadonly(class CCmdUI *);	//562
	void  OnUpdateViewRuler(class CCmdUI *);	//563
	void  OnUpdateViewSectionNames(class CCmdUI *);	//564
	void  OnUpdateViewSections(class CCmdUI *);	//565
	void  OnUpdateViewSelectDrawing(class CCmdUI *);	//566
	void  OnUpdateViewSheetBar(class CCmdUI *);	//567
	void  OnUpdateViewSheetBw(class CCmdUI *);	//568
	void  OnUpdateWindowFreezePanes(class CCmdUI *);	//569
	void  OnVScroll(unsigned int,unsigned int,class CScrollBar *);	//570
	void  OnViewAutoBreaks(void);	//571
	void  OnViewGrid(void);	//572
	void  OnViewHeaders(void);	//573
	void  OnViewReadonly(void);	//574
	void  OnViewRuler(void);	//575
	void  OnViewSectionNames(void);	//576
	void  OnViewSections(void);	//577
	void  OnViewSelectDrawing(void);	//578
	void  OnViewSheetBar(void);	//579
	void  OnViewSheetBw(void);	//580
	void  OnWindowFreezePanes(void);	//581
public:
	void  OutlineSelection(class CSheetFormat const &);	//583
	void  PTtoRC(class CSheetGDI &,class CPoint,int &,int &,struct tagRECT *);	//585
	void  PTtoRC(class CPoint,int &,int &,struct tagRECT *);	//586
protected:
	void  PaintBorder(class CSheetGDI &,class CDC *,struct tagRECT const *,class CSheetFormat const &,struct tagRECT const *);	//587
public:
	void  PaintObject(class CSheetGDI &,class CSheetDrawing *,class CDC *,class CRect);	//588
	void  PaintObjectHandles(class CSheetDrawing *,class CDC *,class CRect const &);	//589
	void  PaintObjectTracker(class CSheetDrawing *,class CDC *,class CRect const &);	//590
protected:
	void  PaintRect(class CSheetGDI &,class CDC *,struct tagRECT const *,class CSheetFormat const &,struct tagRECT const *);	//591
	void  PaintText(class CSheetGDI &,class CDC *,char const *,class CSheetFormat const &,struct tagRECT const *,int,struct tagRECT const *);	//592
	void  PaintText(class CDC *,char const *,class CSheetFormat const &,struct tagRECT const *,struct tagRECT const *);	//593
	void  PasteCells(class CSheet const &,struct _GUID,int,int);	//597
	void  PasteDrawings(class CSheet const &,class CPoint);	//598
	class CSheetPoint   PixelToSheet(class CSheetGDI &,class CPoint const &);	//602
	class CSheetRect   PixelToSheet(class CSheetGDI &,class CRect const &);	//603
public:
	class CSheetPoint   PixelToSheet(class CPoint const &);	//604
	class CSheetRect   PixelToSheet(class CRect const &);	//605
protected:
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//606
	int  PreTranslateEditMessage(struct tagMSG *);	//607
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//608
protected:
	void  RecalcFixedSize(void);	//611
	int  Repaginate(void);	//628
	int  RepaginateHorz(void);	//629
	void  SaveViewState(class CSheetViewState &);	//637
public:
	void  ScrollBy(int,int);	//638
	void  ScrollBy(struct tagSIZE);	//639
	void  ScrollToEndPosition(int,int);	//640
	void  ScrollToEndPosition(struct tagPOINT);	//641
	void  ScrollToPosition(int,int);	//642
	void  ScrollToPosition(struct tagPOINT);	//643
	void  SelectArea(class CSheetSelection const &);	//644
	class CSheetSelection *  SelectCol(int,int);	//646
	int  SelectCurrentGroup(void);	//648
	void  SelectDrawing(class CSheetDrawing *);	//649
	void  SelectDrawingsInRect(class CRect const &);	//650
	void  SelectName(class CString);	//651
	class CSheetSelection *  SelectRect(int,int,int,int);	//654
	class CSheetSelection *  SelectRow(int,int);	//655
	class CSheetSelection *  SelectTable(void);	//658
	void  SetColWidth(int,int);	//669
	void  SetCurrentPos(int,int);	//670
	enum CSheetSector   SetCurrentSector(enum CSheetSector);	//671
	void  SetDisplDrawingRect(class CSheetDrawing *,class CRect const &);	//673
protected:
	void  SetMessageText(void);	//679
public:
	void  SetObjectCursor(class CSheetDrawing *,int);	//684
	void  SetObjectRect(int,class CPoint);	//685
protected:
	void  SetRectStatus(class CRect const &);	//703
public:
	void  SetRowHeight(int,int);	//705
	void  SetScrollSizes(class CSize);	//707
	void  SetSelectionChanged(void);	//708
	void  SetSelectionName(class CString);	//709
protected:
	class CPoint   SheetToPixel(class CSheetGDI &,class CSheetPoint const &);	//715
	class CRect   SheetToPixel(class CSheetGDI &,class CSheetRect const &);	//716
public:
	class CPoint   SheetToPixel(class CSheetPoint const &);	//717
	class CRect   SheetToPixel(class CSheetRect const &);	//718
protected:
	int  SmartRepaginate(void);	//719
	void  StartEditMode(void);	//720
	void  StartEditTextBox(void);	//721
	void  StopEditMode(int);	//722
	void  StopEditTextBox(int);	//723
	void  StopInPlaceActivation(int);	//724
public:
	void  StretchSelection(int,int,int);	//725
protected:
	void  TranslateDeviceMode(class CPrintDialog *);	//730
public:
	void  UpdateBars(void);	//737
	int  XCharToPixel(int);	//741
	int  XMillimetersToPixel(int);	//743
	int  XPixelToMillimeters(int);	//746
	int  XPixelToUnit(int);	//748
	int  XUnitToPixel(int);	//750
	int  YCharToPixel(int);	//752
	int  YMillimetersToPixel(int);	//754
	int  YPixelToMillimeters(int);	//757
	int  YPixelToUnit(int);	//759
	int  YUnitToPixel(int);	//761

	DECLARE_MESSAGE_MAP()
};

// Секция таблицы
struct CNamedArrayItem
{
	DWORD flag;
	char * Name;
	CSheetNamedItem SheetNamedItem;
};

