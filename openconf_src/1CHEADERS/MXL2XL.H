// Begin forward declarations
class  CSheet;
class  CSheetCell;
class  CSheetDoc;
class  CSheetDrawing;
class  CSheetFormat;
class  CSheetGDI;
class  CSheetNamedItem;
class  CSheetNames;
class  CSheetOutline;
class  CSheetPoint;
class  CSheetRect;
class  CSheetRow;
class  CSheetSelection;
class  CSheetView;
// End forward declarations

// Begin class descriptions
class CSheet:public CObject
{
DECLARE_DYNCREATE(CSheet)

public:
	 CSheet(class CSheet const &);	//1
	 CSheet(void);	//2
	virtual  ~CSheet(void);	//37
	void  ApplyFormatting(class CSheetSelection const &,class CSheetCell const &,unsigned long);	//90
	void  Clear(class CSheetSelection const &,unsigned long);	//100
	void  ClearCellContents(class CSheetSelection const &);	//101
	void  ClearCells(class CSheetSelection const &,unsigned long);	//102
	class CSheet &  Copy(class CSheet const &);	//108
	void  CopyCell(class CSheetCell &,class CSheet const &,class CSheetCell const &);	//109
	void  CopyCells(class CSheet &,class CSheetSelection const &)const;	//110
	void  CopySheet(class CSheet &,class CSheetSelection const &)const;	//111
	void  Delete(class CSheetSelection const &,int);	//121
	void  DeleteCells(class CSheetSelection const &,int);	//123
	void  DeleteDrawing(class CSheetDrawing *);	//125
	void  DeleteDrawings(class CList<class CSheetDrawing *,class CSheetDrawing *> *);	//126
	void  DeleteName(class CString const &);	//127
	int  FindCell(char const *,unsigned long,struct tagPOINT *,struct tagRECT const *);	//138
	class CSheetDrawing *  FindDrawingByID(unsigned long);	//139
	int  FindGroup(enum CSheetDirection,int,int,class CSheetSelection *);	//140
	int  FindGroup(enum CSheetDirection,char const *,class CSheetSelection *);	//141
	int  FindPageBreak(enum CSheetDirection,int,int *)const;	//142
	void  Fire_OnClose(void);	//143
	class CRect   GetAcrossFormattingRect(int,int);	//144
	class CSheetCell &  GetCell(int,int);	//154
	void  GetCellAttributes(int,int,class CSheetFormat &)const;	//155
	void  GetCellFontAttributes(int,int,struct tagLOGFONTA &)const;	//156
	class CSheetFormat &  GetCol(int);	//159
	class CSheetFormat &  GetColAt(int,int *)const;	//160
	int  GetColCount(void)const;	//161
	void  GetColFormat(int,class CSheetFormat &)const;	//162
	int  GetColKeyAt(int)const;	//163
	class CArray<class CSheetOutline,class CSheetOutline const &> &  GetColOutline(void);	//164
	int  GetColWidth(int);	//165
	class CSortArray<int,int,class CSheetFormat *,class CSheetFormat *> const &  GetColumns(void)const;	//169
	void  GetDefFormat(class CSheetFormat &)const;	//172
	class CSheetDoc *  GetDocument(void);	//174
	class CList<class CSheetDrawing *,class CSheetDrawing *> const &  GetDrawings(void)const;	//177
	void  GetDrawingsInRect(class CRect const &,class CList<class CSheetDrawing *,class CSheetDrawing *> &,int)const;	//178
	void  GetFontAttributes(class CSheetFormat const *,struct tagLOGFONTA &)const;	//182
	int  GetFontKey(struct tagLOGFONTA const &);	//187
	class CSortArray<int,int,struct tagLOGFONTA,struct tagLOGFONTA &> const &  GetFonts(void)const;	//191
	class CSheetCell &  GetFooter(void);	//192
	unsigned long  GetFormat(class CList<class CSheetDrawing *,class CSheetDrawing *> const &,class CSheetCell &);	//193
	unsigned long  GetFormat(class CSheetSelection const &,class CSheetCell &);	//194
	int  GetGroupPart(char const *,class CSheetSelection &,enum CSheetGroupPart);	//197
	int  GetGroupPart(enum CSheetDirection,int,class CSheetSelection &,enum CSheetGroupPart);	//198
	int  GetGroupPart(enum CSheetDirection,char const *,class CSheetSelection &,enum CSheetGroupPart);	//199
	void  GetGroupSelection(enum CSheetDirection,int,class CSheetSelection &);	//200
	class CSheetCell &  GetHeader(void);	//201
	static int  GetLineCountOfString(char const *);	//205
	class CSortArray<int,int,class CString,char const *> const &  GetMasks(void)const;	//209
	class CRect   GetMerge(int,int)const;	//210
	class CList<class CRect,struct tagRECT const *> &  GetMerges(void);	//211
	class CSheetNames const &  GetNames(void)const;	//214
	class CArray<class CSheetOutline,class CSheetOutline const &> &  GetOutline(enum CSheetDirection);	//220
	int  GetPageBreakAt(enum CSheetDirection,int)const;	//221
	int  GetPageBreakCount(enum CSheetDirection)const;	//222
	class CArray<int,int> &  GetPageBreaks(enum CSheetDirection);	//223
	class CSheetRow &  GetRow(int);	//232
	class CSheetRow &  GetRowAt(int,int *)const;	//233
	int  GetRowCount(void)const;	//234
	void  GetRowFormat(int,class CSheetFormat &)const;	//235
	int  GetRowKeyAt(int)const;	//238
	class CArray<class CSheetOutline,class CSheetOutline const &> &  GetRowOutline(void);	//239
	class CSortArray<int,int,class CSheetRow *,class CSheetRow *> const &  GetRows(void)const;	//241
	void  GetSelectionRects(class CSheetSelection const &,int,class CRect &,class CRect &,class CRect &)const;	//255
	struct ISheetHook *  GetSheetHook(void);	//257
	class CSize   GetSize(void)const;	//258
	static void  GetStandardFont(struct tagLOGFONTA &);	//259
	int  Group(class CSheetSelection const &,enum CSheetDirection);	//276
	void  Insert(class CSheetSelection const &,int);	//279
	void  InsertCells(class CSheetSelection const &,int);	//281
	void  InsertDrawing(class CSheetDrawing *,class CSheetDrawing *);	//282
	class CSheetDrawing *  InsertOleObject(class CSheetRect const &,struct _GUID const &);	//283
	static int  IsCellMatch(class CSheetCell *,char const *,unsigned long);	//287
	static int  IsStringMatch(char const *,char const *,unsigned long);	//300
	void  LoadFromString(char const *);	//304
	int  LookupCell(int,int,class CSheetCell * &)const;	//308
	int  LookupCol(int,class CSheetFormat * &,int *)const;	//309
	int  LookupColKey(int,int *)const;	//310
	int  LookupMerge(int,int,class CRect &)const;	//312
	int  LookupMerge(class CRect,class CRect &)const;	//313
	int  LookupName(class CString const &,class CSheetNamedItem &)const;	//314
	int  LookupRow(int,class CSheetRow * &,int *)const;	//318
	int  LookupRowKey(int,int *)const;	//319
	void  Merge(struct tagRECT const *);	//320
protected:
	void  MergeRect(class CRect const &);	//321
public:
	void  ModifyColKey(int,int);	//322
	void  ModifyRowKey(int,int);	//324
	void  MoveDrawing(class CSheetDrawing *,class CSheetRect const &);	//325
	void  OutlineSelection(class CSheetSelection const &,class CSheetFormat const &);	//344
	void  PasteCells(class CSheet const &,class CSheetSelection const &,unsigned long);	//347
	void  PasteCells(class CSheet const &,int,int,enum CSheetSelectType,unsigned long);	//348
	void  PasteSheet(class CSheet const &,class CSheetSelection const &,unsigned long);	//349
	void  PasteSheet(class CSheet const &,int,int,enum CSheetSelectType,unsigned long);	//350
	static char const *  ReadClipboardString(char const *,class CString &);	//355
	void  RemoveAll(void);	//357
	void  RemoveAllCols(void);	//359
	void  RemoveAllDrawings(void);	//360
	void  RemoveAllRows(void);	//363
	int  RemoveCol(int);	//365
	void  RemovePageBreak(enum CSheetDirection,int);	//367
	int  RemoveRow(int);	//371
	static void  ReplaceCellText(class CSheetCell *,char const *,unsigned long,char const *);	//372
	static class CString   ReplaceString(char const *,char const *,unsigned long,char const *);	//373
	void  SaveAsString(class CArchive &);	//375
	virtual void  Serialize(class CArchive &);	//388
	void  SetColFormat(int,class CSheetFormat const &);	//397
	void  SetDefFormat(class CSheetFormat const &);	//398
	void  SetName(class CString const &,class CSheetNamedItem const &);	//405
	void  SetPageBreak(enum CSheetDirection,int);	//408
	void  SetRowFormat(int,class CSheetFormat const &);	//425
	long  SetSheetHook(struct ISheetHook *);	//427
	void  SetSize(class CSize const &);	//428
	static void  SetStandardFont(struct tagLOGFONTA &);	//429
	void  UnGroup(class CSheetSelection const &,enum CSheetDirection);	//438
	int  UnMerge(int,int);	//439
	void  UnMerge(class CSheetSelection const &,int);	//440
protected:
	void  UnMergeRect(class CRect const &);	//441
public:
	static void  WriteClipboardString(class CArchive &,char const *);	//445
	static unsigned short const  m_cfDrawing;	//485
	static unsigned short const  m_cfEmbeddedObject;	//486
	static unsigned short const  m_cfObjectDescriptor;	//487
	static unsigned short const  m_cfSheet;	//488
	static unsigned short const  m_cfSheetDescriptor;	//489
	static unsigned short const  m_cfSheetOld;	//490
	static struct _GUID const  m_guidCol;	//491
	static struct _GUID const  m_guidDrawing;	//492
	static struct _GUID const  m_guidRect;	//493
	static struct _GUID const  m_guidRow;	//494
	static struct _GUID const  m_guidSheet;	//495
	static struct tagLOGFONTA  m_lfStandardFont;	//496
};


class CSheetCell:public CObject
{
DECLARE_DYNCREATE(CSheetCell)

public:
	 CSheetCell(class CSheetCell const &);	//4
	 CSheetCell(void);	//5
	virtual  ~CSheetCell(void);	//38
	class CSheetCell &  operator=(class CSheetCell const &);	//48
	void  Apply(class CSheetCell const &,unsigned long);	//87
	void  Apply(class CSheetFormat const &,unsigned long);	//88
	class CSheetCell &  Assign(class CSheetCell const &);	//91
	char const *  GetFormula(void);	//195
	int  GetPropertiesCount(void)const;	//226
	char const *  GetText(void);	//264
protected:
	void  Init(void);	//278
public:
	int  IsPropertySet(enum CSheetDWord)const;	//295
	int  IsPropertySet(enum CSheetString)const;	//296
	long  LoadValue(struct ISheetValue *);	//306
	int  LookupProperty(enum CSheetDWord,unsigned long &)const;	//315
	int  LookupProperty(enum CSheetString,class CString &)const;	//316
	void  RemoveAllProperties(void);	//361
	int  RemoveProperty(enum CSheetDWord);	//368
	int  RemoveProperty(enum CSheetString);	//369
	long  SaveValue(struct ISheetValue *);	//378
	virtual void  Serialize(class CArchive &);	//389
	void  SetProperty(enum CSheetDWord,unsigned long);	//416
	void  SetProperty(enum CSheetString,char const *);	//417
};


class CSheetDoc:public CObject
{
DECLARE_DYNCREATE(CSheetDoc)

public:
	 CSheetDoc(void);	//6
	virtual  ~CSheetDoc(void);	//39
	virtual void  DeleteContents(void);	//124
	virtual int  DoFileSave(void);	//131
protected:
	virtual int  DoSave(char const *,int);	//132
public:
	static unsigned short  GetArchiveVersionField(class CArchive &,unsigned int);	//145
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//180
public:
	int  GetNoSavePrompt(void)const;	//215
	int  GetReadOnly(void)const;	//229
	class CSheetUndo &  GetUndo(void);	//269
	int  GetViewCount(void)const;	//272
	class CString const &  GetWindowKey(void)const;	//274
	void  LoadProfile(void);	//305
protected:
	void  OnAppFontChange(class CMessage7 *,long *);	//331
	void  OnAppPageChange(class CMessage7 *,long *);	//332
public:
	virtual void  OnCloseDocument(void);	//333
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//334
protected:
	void  OnEditConvert(void);	//336
	void  OnFileSave(void);	//337
	void  OnFileSaveAs(void);	//338
	virtual int  OnNewDocument(void);	//339
public:
	virtual int  OnSaveDocument(char const *);	//340
protected:
	void  OnUpdateFileSave(class CCmdUI *);	//341
	void  OnUpdateFileSaveAs(class CCmdUI *);	//342
	void  OnUpdateObjectVerbMenu(class CCmdUI *);	//343
	virtual void  ReleaseFile(class CFile *,int);	//356
public:
	int  SaveAs(char const *,enum CSheetSaveAsType);	//374
protected:
	virtual int  SaveModified(void);	//376
public:
	void  SaveProfile(void);	//377
	virtual void  Serialize(class CArchive &);	//390
	virtual void  SetModifiedFlag(int);	//404
	void  SetNoSavePrompt(int);	//407
	virtual void  SetPathName(char const *,int);	//409
	void  SetProfileKey(char const *);	//415
	void  SetReadOnly(int);	//420
	void  SetUserTitle(char const *);	//430
	void  SetWindowKey(char const *);	//431

	DECLARE_MESSAGE_MAP()
};


class CSheetDrawing:public CObject
{
DECLARE_DYNCREATE(CSheetDrawing)

public:
	 CSheetDrawing(class CSheetDrawing const &,class COleDocument *);	//7
	 CSheetDrawing(enum CSheetDrawType);	//8
	 CSheetDrawing(void);	//9
	virtual  ~CSheetDrawing(void);	//40
	class CSheetDrawing &  operator=(class CSheetDrawing const &);	//49
	void  `copy constructor closure'(class CSheetDrawing const &);	//86
	class CSheetDrawing &  Assign(class CSheetDrawing const &);	//92
	class CRect   GetDisplRect(void)const;	//173
	unsigned long  GetID(void)const;	//204
	struct IDispatch *  GetObjectDispatch(void);	//219
	class CSheetRect const &  GetRect(void)const;	//230
	enum CSheetDrawType   GetType(void)const;	//266
	virtual void  Serialize(class CArchive &);	//391
	void  SetDisplRect(class CRect const &);	//399
	void  SetRect(class CSheetRect const &);	//421
};


class CSheetFormat:public CObject
{
DECLARE_DYNCREATE(CSheetFormat)

public:
	 CSheetFormat(class CSheetFormat const &);	//10
	 CSheetFormat(void);	//11
	virtual  ~CSheetFormat(void);	//41
	class CSheetFormat &  operator=(class CSheetFormat const &);	//50
	void  Apply(class CSheetFormat const &,unsigned long);	//89
	class CSheetFormat &  Assign(class CSheetFormat const &);	//93
protected:
	void  ClearFlag(enum CSheetDWord);	//103
	void  ClearFlag(enum CSheetString);	//104
public:
	unsigned long  Compare(class CSheetFormat const &);	//107
	unsigned char  GetBkColor(void)const;	//147
	unsigned char  GetBorderBottom(void)const;	//148
	unsigned char  GetBorderColor(void)const;	//149
	unsigned char  GetBorderLeft(void)const;	//150
	unsigned char  GetBorderRight(void)const;	//151
	unsigned char  GetBorderTop(void)const;	//152
	unsigned char  GetBorders(void)const;	//153
	unsigned char  GetDataFormat(void)const;	//171
	unsigned char  GetDontPrint(void)const;	//175
	unsigned char  GetFontBold(void)const;	//183
	unsigned char  GetFontItalic(void)const;	//186
	unsigned short  GetFontName(void)const;	//188
	unsigned short  GetFontSize(void)const;	//189
	unsigned char  GetFontUnderline(void)const;	//190
	unsigned short  GetHeight(void)const;	//202
	unsigned char  GetHorzAlign(void)const;	//203
	unsigned char  GetLineStyle(void)const;	//206
	unsigned char  GetLineWeight(void)const;	//207
	unsigned char  GetPattern(void)const;	//224
	unsigned char  GetPatternColor(void)const;	//225
	int  GetPropertiesCount(void)const;	//227
	unsigned char  GetTextColor(void)const;	//265
	int  GetUnprotected(void)const;	//270
	unsigned char  GetVertAlign(void)const;	//271
	unsigned short  GetWidth(void)const;	//273
	unsigned char  GetWrapText(void)const;	//275
	int  HasProperties(void)const;	//277
protected:
	int  IsFlagSet(enum CSheetDWord)const;	//292
	int  IsFlagSet(enum CSheetString)const;	//293
public:
	int  IsPropertySet(enum CSheetDWord)const;	//297
	int  LookupProperty(enum CSheetDWord,unsigned long &)const;	//317
	void  RemoveAllProperties(void);	//362
	int  RemoveProperty(enum CSheetDWord);	//370
	virtual void  Serialize(class CArchive &);	//392
protected:
	void  SetFlag(enum CSheetDWord);	//401
	void  SetFlag(enum CSheetString);	//402
public:
	void  SetProperty(enum CSheetDWord,unsigned long);	//418
};


class CSheetGDI
{
public:
	 CSheetGDI(void);	//12
	virtual  ~CSheetGDI(void);	//42
	void  Attach(class CSheetView *);	//97
	void  ClearFontCache(void);	//105
	void  ClearHeightCache(void);	//106
	void  CreateDisplayInfo(void);	//113
	int  CreatePrinterInfo(void);	//120
protected:
	void  Destroy(void);	//129
public:
	void  DrawBorder(class CDC *,class CRect,int,unsigned long,unsigned long);	//133
	void  DrawLine(class CDC *,class CRect,int,int,unsigned long);	//134
	static void  DrawPageBreak(class CDC *,class CRect,unsigned long,unsigned long,enum CSheetBreakStyle);	//135
	static void  FillRect(class CDC *,class CRect const &,unsigned long);	//137
	class CRect   GetCellRect(int,int);	//157
	static int  GetClipRgn(class CDC *,class CRgn &);	//158
	int  GetColumnStart(int);	//166
	int  GetColumnWidth(int);	//167
protected:
	int  GetColumnWidthUni(int);	//168
public:
	class CFont *  GetFont(struct tagLOGFONTA const &);	//181
protected:
	int  GetFontCachePos(struct tagLOGFONTA const &);	//184
public:
	int  GetFontHeight(struct tagLOGFONTA const &);	//185
	class CSize   GetGapSize(void)const;	//196
	class CSize   GetLogPixels(void);	//208
	int  GetRowHeight(int);	//236
protected:
	int  GetRowHeightUni(int);	//237
public:
	int  GetRowStart(int);	//240
	class CSize   GetRulerSize(void)const;	//242
	int  GetScale(void)const;	//252
	class CSize   GetSectionSize(void)const;	//253
	class CSize   GetStdCharSize(void);	//261
	unsigned long  GetSysColor(int);	//262
	struct tagTEXTMETRICA const &  GetTM(struct tagLOGFONTA const &);	//263
	int  IsEast(void);	//289
	int  IsNorth(void);	//294
	int  IsSouth(void);	//299
	int  IsWest(void);	//301
	static class CRect   Justify(class CDC *,char const *,class CRect,int,int,int,struct tagRECT const *);	//302
	void  PTtoRC(class CPoint,int &,int &,struct tagRECT *);	//345
	static void  PatFillRect(class CDC *,class CRect const &,unsigned long,unsigned long,int,class CPoint);	//351
	void  SetPrintHeaders(int);	//414
	void  SetScale(int);	//426
protected:
	void  UpdateDeviceMetrics(void);	//444
public:
	int  XCharToPixel(int);	//446
	int  XMillimetersToPixel(int);	//447
	int  XPixelToChar(int);	//448
	int  XPixelToMillimeters(int);	//449
	int  XPixelToUnit(int);	//450
	int  XUnitToPixel(int);	//451
	int  YCharToPixel(int);	//452
	int  YMillimetersToPixel(int);	//453
	int  YPixelToChar(int);	//454
	int  YPixelToMillimeters(int);	//455
	int  YPixelToUnit(int);	//456
	int  YUnitToPixel(int);	//457
};


class CSheetNamedItem:public CObject
{
DECLARE_DYNAMIC(CSheetNamedItem)

public:
	 CSheetNamedItem(class CSheetNamedItem const &);	//13
	 CSheetNamedItem(class CSheetSelection const &);	//14
	 CSheetNamedItem(class CSheetDrawing const *);	//15
	 CSheetNamedItem(void);	//16
	virtual  ~CSheetNamedItem(void);	//43
	class CSheetNamedItem &  operator=(class CSheetNamedItem const &);	//51
	bool  operator==(class CSheetNamedItem const &)const;	//60
	bool  operator!=(class CSheetNamedItem const &)const;	//64
	class CSheetNamedItem &  Assign(class CSheetNamedItem const &);	//94
	class CString   DisplayString(void)const;	//130
	unsigned long  GetDrawID(void)const;	//176
	class CSheetSelection const &  GetRange(void)const;	//228
	enum CSheetNameType   GetType(void)const;	//267
	bool  IsEqual(class CSheetNamedItem const &)const;	//290
	int  ParseString(char const *);	//346
	virtual void  Serialize(class CArchive &);	//393
	void  SetDrawID(unsigned long);	//400
	void  SetRange(class CSheetSelection const &);	//419
};


class CSheetNames:public CObject
{
DECLARE_DYNAMIC(CSheetNames)

public:
	 CSheetNames(void);	//17
	virtual  ~CSheetNames(void);	//44
	class CSheetNames &  operator=(class CSheetNames const &);	//52
	virtual void  Serialize(class CArchive &);	//394
	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   begin(void)const;	//469
	void  clear(void);	//479
	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   end(void)const;	//480
	void  erase(class CString const &);	//481
	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   find(class CString const &)const;	//482
	struct std::__hashtable_const_iterator<struct std::pair<class CString const ,class CSheetNamedItem>,class CString,struct CSheetNames::strihash,struct std::select1st<struct std::pair<class CString const ,class CSheetNamedItem> >,struct CSheetNames::strieq,class std::__alloc<1,0> >   find_item(class CSheetNamedItem const &)const;	//483
	void  insert(class CString const &,class CSheetNamedItem const &);	//484
};


class CSheetOutline
{
public:
	 CSheetOutline(class CSheetOutline const &);	//18
	 CSheetOutline(int,int,int,char const *);	//19
	 CSheetOutline(void);	//20
	virtual  ~CSheetOutline(void);	//45
	class CSheetOutline &  operator=(class CSheetOutline const &);	//53
	class CSheetOutline &  Assign(class CSheetOutline const &);	//95
	class CString const &  GetName(void)const;	//213
	void  SetName(char const *);	//406
};


class CSheetPoint
{
public:
	 CSheetPoint(struct SHEETPOINT const &);	//21
	 CSheetPoint(class CSheetPoint const &);	//22
	 CSheetPoint(int,int,int,int);	//23
	 CSheetPoint(class CPoint,class CPoint);	//24
	 CSheetPoint(void);	//25
	class CSheetPoint &  operator=(struct SHEETPOINT const &);	//54
	class CSheetPoint &  operator=(class CSheetPoint const &);	//55
	int  operator==(class CSheetPoint const &)const;	//61
	int  operator!=(class CSheetPoint const &)const;	//65
	class CSheetPoint   operator-(class CSheetPoint const &)const;	//69
	class CSheetPoint   operator-(void)const;	//70
	class CSheetPoint   operator+(class CSheetPoint const &)const;	//71
	void  operator+=(class CSheetPoint const &);	//72
	void  operator-=(class CSheetPoint const &);	//73
	void  Offset(class CSheetPoint const &);	//327
	void  Offset(class CPoint,class CPoint);	//328
	class CSheetPoint &  SetPoint(struct SHEETPOINT const &);	//410
	class CSheetPoint &  SetPoint(class CSheetPoint const &);	//411
	class CSheetPoint &  SetPoint(int,int,int,int);	//412
	class CSheetPoint &  SetPoint(class CPoint,class CPoint);	//413
};


class CSheetRect
{
public:
	 CSheetRect(class CSheetRect const &);	//26
	 CSheetRect(class CSheetPoint const &,class CSheetPoint const &);	//27
	 CSheetRect(class CRect);	//28
	 CSheetRect(void);	//29
	class CSheetRect &  operator=(class CSheetRect const &);	//56
	int  operator==(class CSheetRect const &)const;	//62
	int  operator!=(class CSheetRect const &)const;	//66
	 operator struct SHEETRECT &(void);	//68
	class CSheetPoint   BottomLeft(void)const;	//98
	class CSheetPoint   BottomRight(void)const;	//99
	int  DeleteRect(class CRect const &,enum CSheetDirection);	//128
	int  InsertRect(class CRect const &,enum CSheetDirection);	//284
	int  IntersectRect(class CSheetRect const &,class CSheetRect const &);	//286
	int  IsRectNull(void)const;	//298
	void  NormalizeRect(void);	//326
	void  OffsetRect(class CSheetPoint const &);	//330
	int  PtInRect(class CSheetPoint const &);	//354
	class CSheetRect &  SetRect(class CSheetRect const &);	//422
	class CSheetRect &  SetRect(class CSheetPoint const &,class CSheetPoint const &);	//423
	void  SetRectEmpty(void);	//424
	int  SubtractRect(class CSheetRect const &,class CSheetRect const &);	//435
	class CSheetPoint   TopLeft(void)const;	//436
	class CSheetPoint   TopRight(void)const;	//437
	int  UnionRect(class CSheetRect const &,class CSheetRect const &);	//443
};


class CSheetRow:public CObject
{
DECLARE_DYNCREATE(CSheetRow)

public:
	 CSheetRow(class CSheetRow const &);	//30
	 CSheetRow(void);	//31
	virtual  ~CSheetRow(void);	//46
	class CSheetRow &  operator=(class CSheetRow const &);	//57
	class CSheetRow &  Assign(class CSheetRow const &);	//96
	class CSheetCell &  ElementAt(int,int *);	//136
	class CSheetCell *  GetAt(int,int *)const;	//146
	int  GetCount(void)const;	//170
	int  KeyAt(int)const;	//303
	int  Lookup(int,class CSheetCell * &,int *)const;	//307
	int  LookupKey(int,int *)const;	//311
	void  ModifyKey(int,int);	//323
	void  RemoveAll(void);	//358
	void  RemoveAt(int);	//364
	int  RemoveKey(int);	//366
	virtual void  Serialize(class CArchive &);	//395
	int  SetKey(int,class CSheetCell *);	//403
};


class CSheetSelection:public CObject
{
DECLARE_DYNAMIC(CSheetSelection)

public:
	 CSheetSelection(class CSheetSelection const &);	//32
	 CSheetSelection(void);	//33
	virtual  ~CSheetSelection(void);	//47
	class CSheetSelection &  operator=(class CSheetSelection const &);	//58
	int  operator==(class CSheetSelection const &)const;	//63
	int  operator!=(class CSheetSelection const &)const;	//67
	int  Delete(class CSheetSelection const &,enum CSheetDirection);	//122
	int  GetEnd(void)const;	//179
	int  GetNormalEnd(void)const;	//216
	class CRect   GetNormalRect(void)const;	//217
	int  GetNormalStart(void)const;	//218
	class CRect const &  GetRect(void)const;	//231
	int  GetStart(void)const;	//260
	enum CSheetSelectType   GetType(void)const;	//268
	int  Insert(class CSheetSelection const &,enum CSheetDirection);	//280
	int  Intersect(class CSheetSelection const &,class CSheetSelection const &);	//285
	int  IsCellSelected(int,int)const;	//288
	int  IsEqual(class CSheetSelection const &)const;	//291
	void  Offset(int,int);	//329
	void  SelectCell(int,int);	//382
	void  SelectColumns(int,int);	//383
	void  SelectRect(class CRect const &);	//384
	void  SelectRect(int,int,int,int);	//385
	void  SelectRows(int,int);	//386
	void  SelectTable(void);	//387
	virtual void  Serialize(class CArchive &);	//396
	int  Subtract(class CSheetSelection const &,class CSheetSelection const &);	//434
	void  Union(class CSheetSelection const &,class CSheetSelection const &);	//442
};


class CSheetView
{
private:
	 CSheetView(void);	//34
public:
	 CSheetView(class CSheetView const &);	//35
	 CSheetView(class CSheetDoc *);	//36
	class CSheetView &  operator=(class CSheetView const &);	//59
protected:
	int  CreateDeviceMode(class CPrintDialog *);	//112
public:
	class CRect   GetSectorRect(enum CSheetSector,unsigned int);	//254
	class CSheet &  GetSheet(void)const;	//256
private:
	virtual void  OnDraw(class CDC *);	//335
protected:
	class CSheetPoint   PixelToSheet(class CSheetGDI &,class CPoint const &);	//352
	class CSheetRect   PixelToSheet(class CSheetGDI &,class CRect const &);	//353
public:
	void  ScrollBy(int,int);	//379
	void  ScrollToEndPosition(int,int);	//380
	void  ScrollToPosition(int,int);	//381
protected:
	class CPoint   SheetToPixel(class CSheetGDI &,class CSheetPoint const &);	//432
	class CRect   SheetToPixel(class CSheetGDI &,class CSheetRect const &);	//433
};

// End class descriptions
// Begin free functions definitions
// End free functions definitions
extern int XlConverter;

