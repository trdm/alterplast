// Begin forward declarations
class  CBodyTextDoc;
class  CBodyTextView;
class  CEditPoint;
class  CMainMessageHandler;
class  CMainWatch;
class  CMessageView;
class  CTextDocument;
class  CTextEditor;
//class  ;
// End forward declarations
// Begin class descriptions
//class
//{
//private:
//	static  ??  ?? :: ?? ::A  ??  `string';	//27
//};
//private: static  ?? :: ?? :: ?? ::A  ??  `string'

class IMPORT_1C CTextDocument:public CDocument
{
DECLARE_DYNCREATE(CTextDocument)
public:
	char m_docbuf[268];		// Точный размер
	virtual void  SetReadOnly(int);	//348
	virtual void  SetModule(int);	//343
protected:
	virtual int  IsSCGlobalEnable(void)const;	//183

public:
	CTextDocument(void);	//9
	virtual  ~CTextDocument(void);	//16
	int  CalcOffset(int)const;	//41
protected:
	void  CalcPos(void);	//45
public:
	int  CanRedo(void)const;	//46
	int  CanUndo(void)const;	//47
	void  ChangeBookMark(int);	//52
protected:
	void  ChangeGlobalParams(void);	//54
public:
	int  ChangeMaxLineLen(int);	//55
protected:
	void  ChangeModuleParams(void);	//56
public:
	void  Clear(void);	//58
	void  ClearAllBookMarks(void);	//61
protected:
	void  CopyBack(class CString &,int)const;	//64
public:
	void  CreateLine(int,char *,int)const;	//65
protected:
	void  CutEndSpaces(class CString &);	//75
public:
	void  DecorateSelection(int,char const *);	//77
	int  DeleteSymb(char,class CPoint);	//79
	virtual int  DoFileSave(void);	//83
protected:
	virtual int  DoSave(char const *,int);	//85
public:
	void  EmptyHistory(void);	//88
protected:
	void  EmptyStrInfo(void);	//89
public:
	void  EnableTemplate(int);	//90
	void  EndDragDrop(void);	//91
	int  FastCreateLine(int,char *,int,int)const;	//92
	int  FastGetLine(int,class CString &,int)const;	//93
	int  FastGetLine(int,char *,int,int)const;	//94
	void  FillProperty(class CPropertyPage *);	//95
	int  FindBk(char const *,int,int);	//96
	int  FindBkInSel(char const *,int &)const;	//97
	int  FindBkvBrance(int,class CPoint &,class CPoint &,int &,int);	//98
	int  FindFrv(char const *,int,int);	//99
	int  FindFrvBrance(int,class CPoint &,class CPoint &,int &,int);	//100
	int  FindFrvInSel(char const *,int &)const;	//101
	int  FindLoopBk(char const *,int,int);	//102
	int  FindLoopFrv(char const *,int,int);	//103
	void  FindNewProcPlace(class CPoint *,class CPoint *);	//104
	void  GetCurrentWord(class CPoint,class CPoint &,class CPoint &)const;	//110
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//115
	class CAbsBuf *  GetFirstPart(void)const;	//116
	class CStrInfo   GetInfo(int)const;	//118
public:
	char  GetLeftChar(class CPoint)const;	//119
	void  GetLine(int,class CString &)const;	//121
	void  GetLine(int,char *,int)const;	//122
	int  GetLineCount(void)const;	//124
	int  GetLineLength(int)const;	//125
	int  GetLinesForPrint(unsigned int,unsigned int,class CString &);	//126
	int  GetMaxLineLen(void)const;	//127
	int  GetNextBookMark(int);	//136
	void  GetNextWord(class CPoint,class CPoint &,class CPoint &)const;	//137
	int  GetPrevBookMark(int);	//139
	void  GetPrevWord(class CPoint,class CPoint &,class CPoint &)const;	//140
protected:
	class CStrInfo &  GetRefInfo(int);	//141
public:
	char  GetRightChar(class CPoint)const;	//142
	void  GetSel(class CPoint &,class CPoint &)const;	//150
	int  GetSelExt(void)const;	//151
	int  GetSelLength(void)const;	//152
	void  GetSelectedText(class CString &)const;	//154
	void  GetSelectedText(char *,int)const;	//155
	unsigned int  GetStoreType(void);	//156
	void  GetTabParams(int,int &,int &)const;	//158
	int  GetTabSize(void)const;	//159
	void  GetText(class CString &);	//160
	void  GetText(char *,int);	//161
	int  GetTextLen(void)const;	//162
	int  GetVLineLength(int)const;	//163
	void  GetViewInstance(class CPoint &,class CPoint &,int &,int &)const;	//166
	void  GetWord(class CPoint,class CPoint &,class CPoint &,int)const;	//168
	void  InsertPageBreak(void);	//173
	void  InsertStr(char const *,class CPoint,int);	//174
	void  InsertWithReturn(class CPoint,char const *);	//175
	void  InvertOverWrite(void);	//176
	int  IsEmpty(void)const;	//177
	int  IsModule(void)const;	//179
	int  IsReadOnly(void)const;	//181
public:
	int  IsSetBookMark(int)const;	//184
	int  IsSyntaxColoring(void)const;	//185
	int  IsTransferOnly(void);	//186
protected:
	void  LoadTemplateInfo(void);	//187
	void  MoveHistHead(void);	//189
public:
	int  OnChangeTemplate(char);	//200
protected:
	void  OnFilePageSetup(void);	//241
public:
	virtual int  OnNewDocument(void);	//263
	virtual int  OnOpenDocument(char const *);	//264
protected:
	void  OnTxtModule(void);	//284
	void  OnUpdateModified(class CCmdUI *);	//307
	void  OnUpdateOVR(class CCmdUI *);	//309
	void  OnUpdateTxtModule(class CCmdUI *);	//314
public:
	void  Redo(void);	//319
protected:
	virtual void  ReleaseFile(class CFile *,int);	//321
public:
	void  Replace(char const *);	//323
	int  ReplaceAll(char const *,char const *,int,int,int);	//324
	int  ReplaceTempl(char const *,char const *);	//326
	virtual void  Serialize(class CArchive &);	//332
	void  SetBookMark(int,int);	//335
protected:
	void  SetInfo(int,class CStrInfo);	//338
public:
	void  SetMarkState(int);	//340
	virtual void  SetModifiedFlag(int);	//341
	void  SetSel(class CPoint,class CPoint);	//349
	void  SetSelForFind(class CPoint,class CPoint)const;	//350
	void  SetStoreType(unsigned int);	//352
	int  SetText(char const *);	//353
	void  SetViewInstance(class CPoint,class CPoint,int,int);	//355
	void  TxtEnumproc(class CArray<class CProcInfo,class CProcInfo &> &,int);	//363
	void  UnSel(void);	//364
	void  Undo(void);	//365
	void  UpdateProperty(class CPropertyPage *);	//367
protected:
	void  UpdateView(unsigned int);	//368
	void  ValidPos(void);	//370
	void  _CalcStrInfo(char const *);	//375
	void  _CreateFullText(class CString &)const;	//376
	void  _Redo(struct HistoryStep *);	//391
	void  _Replace(char const *);	//392
	void  _Undo(struct HistoryStep *);	//393
	static int  m_OverWrite;	//412

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTextEditor:public CView
{
DECLARE_DYNCREATE(CTextEditor)

public:
	//char m_viewbuf[460];	// Точный размер
	char m_vbuf1[28*4];		// Не знаю
	CPoint m_ptStartSel;	//28,29 (смещение в DWORD'ах)
	CPoint m_ptEndSel;		//30,31
	CPoint m_ptNewStartSel;	//32,33  Пока не знаю точно. Перед WM_CHAR одинаковы, после обработки
	CPoint m_ptNewEndSel;	//34,35  WM_CHAR m_ptNew содержит новую позицию, а m_pt - старую
	char m_vbuf2[9*4];		// Не знаю
	BOOL m_caretInEndOfSel;	// Курсор находится в конце выделенного блока
	char m_vbufEnd[460-28*4-4*sizeof(CPoint)-9*4-sizeof(BOOL)];	// Остаток размера

	 CTextEditor(void);	//10
	virtual  ~CTextEditor(void);	//17
protected:
	void  AddTemplateMenu(class CMenu *);	//32
	int  AnalizeStrForCurr(class CString &)const;	//33
	int  AnalizeStrForNext(class CString &)const;	//34
	class CEditPoint   CalcCaretJump(class CPoint);	//35
	void  CalcError(class CEditPoint &);	//36
public:
	class CSize   CalcExtent(char const *,int,int,class CDC *);	//38
protected:
	class CSize   CalcFullSize(void);	//39
	class CSize   CalcLineSize(int)const;	//40
	class CSize   CalcPage(void)const;	//42
	class CSize   CalcPageSize(void)const;	//43
public:
	class CSize   CalcPartExtent(char const *,int,class CDC *);	//44
protected:
	void  CaretGoHomeEnd(int);	//48
	void  CaretJump(class CPoint);	//49
	void  CaretPageStep(int);	//50
	void  CaretStep(int);	//51
	int  ChangeWidth(char const *);	//57
	virtual void  Comment(int);	//62
	void  CursMove(class CPoint);	//73
	void  CursScroll(void);	//74
	void  Del(int);	//78
public:
	virtual unsigned long  DoDragDrop(class CPoint);	//80
protected:
	int  DoMouseWheel(unsigned int,short,class CPoint);	//84
	void  DrawFullLine(char const *,class CDC *,int &,int,int,int,int,int,int);	//86
	void  DrawLine(char const *,class CDC *,int &,int,int,int);	//87
	virtual void  FindReplace(int,int);	//105
	void  FormatSel(class CString *,class CString *,int)const;	//106
public:
	void  GetCurrentWord(class CString &);	//111
	class CTextDocument *  GetDocument(void)const;	//114
protected:
	int  GetLeftPos(void);	//120
public:
	int  GetMaxLineLen(void);	//128
	void  GetSelText(class CString &);	//153
	void  GetViewInstance(class CPoint &,class CPoint &,int &,int &);	//167
protected:
	void  GetWorkRect(class CRect *)const;	//169
	int  Insert(int);	//172
public:
	void  MoveCaret(class CPoint,class CPoint,int);	//188
protected:
	virtual void  MoveSection(int);	//190
	class CDC *  MyGetDC(void);	//191
	void  OnAssOk(struct tagNMHDR *,long *);	//192
	virtual void  OnBeginPrinting(class CDC *,struct CPrintInfo *);	//193
	void  OnBkwBrance2(void);	//194
	void  OnBkwBrance(void);	//195
	void  OnBookmark(void);	//196
	void  OnBookmarkClearall(void);	//197
	void  OnBookmarkFindBack(void);	//198
	void  OnBookmarkFindForward(void);	//199
	void  OnChar(unsigned int,unsigned int,unsigned int);	//202
	void  OnCommSection(void);	//205
	void  OnContextMenu(class CWnd *,class CPoint);	//207
	void  OnControlProperty(class CCmdUI *);	//208
	int  OnCreate(struct tagCREATESTRUCTA *);	//209
	void  OnDestroy(void);	//211
public:
	virtual unsigned long  OnDragEnter(class COleDataObject *,unsigned long,class CPoint);	//214
	virtual void  OnDragLeave(void);	//215
	virtual unsigned long  OnDragOver(class COleDataObject *,unsigned long,class CPoint);	//216
	virtual void  OnDraw(class CDC *);	//218
	virtual int  OnDrop(class COleDataObject *,unsigned long,class CPoint);	//219
	virtual unsigned long  OnDropEx(class COleDataObject *,unsigned long,unsigned long,class CPoint);	//220
protected:
	void  OnEditClear(void);	//221
	void  OnEditCopy(void);	//222
	void  OnEditCut(void);	//224
	void  OnEditDelStr(void);	//225
	void  OnEditFind(void);	//226
	void  OnEditFindnext(void);	//227
	void  OnEditFindprev(void);	//228
	void  OnEditPaste(void);	//230
	void  OnEditRedo(void);	//232
	void  OnEditRepeat(void);	//233
	void  OnEditReplace(void);	//235
	void  OnEditSelall(void);	//236
	void  OnEditUndo(void);	//238
	virtual void  OnEndPrinting(class CDC *,struct CPrintInfo *);	//239
	long  OnFindReplace(unsigned int,long);	//242
	void  OnFormatsel(void);	//244
	void  OnFrwBrance2(void);	//245
	void  OnFrwBrance(void);	//246
	void  OnHScroll(unsigned int,unsigned int,class CScrollBar *);	//247
public:
	virtual void  OnInitialUpdate(void);	//249
protected:
	void  OnInsertPageBreak(void);	//250
	void  OnInsertTemplate(unsigned int);	//251
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//252
	void  OnKillFocus(class CWnd *);	//253
	void  OnLButtonDblClk(unsigned int,class CPoint);	//255
	void  OnLButtonDown(unsigned int,class CPoint);	//256
	void  OnLButtonUp(unsigned int,class CPoint);	//257
	void  OnMouseMove(unsigned int,class CPoint);	//258
	int  OnMouseWheel(unsigned int,short,class CPoint);	//259
	long  OnMoveCaret(unsigned int,long);	//260
	void  OnMoveSectionL(void);	//261
	void  OnMoveSectionR(void);	//262
public:
	virtual void  OnPrepareDC(class CDC *,struct CPrintInfo *);	//265
protected:
	virtual int  OnPreparePrinting(struct CPrintInfo *);	//266
	virtual void  OnPrint(class CDC *,struct CPrintInfo *);	//267
	void  OnRButtonDown(unsigned int,class CPoint);	//268
	void  OnRButtonUp(unsigned int,class CPoint);	//269
	void  OnReplaceAll(class CString,class CString,int,int,int);	//270
	void  OnReplaceSel(class CString,class CString,int,int);	//271
public:
	virtual int  OnScroll(unsigned int,unsigned int,int);	//273
protected:
	void  OnSetFocus(class CWnd *);	//274
	long  OnSetFont(unsigned int,long);	//275
	void  OnSize(unsigned int,int,int);	//276
	void  OnTimer(unsigned int);	//281
	void  OnTxtEnumproc(void);	//282
	void  OnTxtGoto(void);	//283
	void  OnTxtProp(void);	//285
	void  OnUncommSection(void);	//286
	virtual void  OnUpdate(class CView *,long,class CObject *);	//289
	void  OnUpdateCaretPos(class CCmdUI *);	//290
	void  OnUpdateEditClear(class CCmdUI *);	//291
	void  OnUpdateEditCopy(class CCmdUI *);	//292
	void  OnUpdateEditCut(class CCmdUI *);	//294
	void  OnUpdateEditFind(class CCmdUI *);	//295
	void  OnUpdateEditPaste(class CCmdUI *);	//297
	void  OnUpdateEditRedo(class CCmdUI *);	//299
	void  OnUpdateEditReplace(class CCmdUI *);	//301
	void  OnUpdateEditUndo(class CCmdUI *);	//303
	void  OnUpdateFormatsel(class CCmdUI *);	//304
	void  OnUpdateInsertPageBreak(class CCmdUI *);	//305
	void  OnUpdateInsertTemplate(class CCmdUI *);	//306
	void  OnUpdateMoveSection(class CCmdUI *);	//308
	void  OnUpdateReadOnly(class CCmdUI *);	//310
	void  OnUpdateToolBar(class CCmdUI *);	//312
	void  OnUpdateTxtEnumproc(class CCmdUI *);	//313
	void  OnVScroll(unsigned int,unsigned int,class CScrollBar *);	//315
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//316
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//317
protected:
	void  PrintPage(class CDC *,unsigned int,class CRect &);	//318
	int  Repaginate(struct CPrintInfo *);	//322
	void  ReplaceLoop(class CString,int,int);	//325
	void  ReplaceTemplate(char const *,char const *,class CPoint,class CPoint,int);	//327
	virtual void  ScrollToPosition(class CPoint);	//331
	virtual void  SetCaret(void);	//336
	void  SetDDMarker(class CPoint *);	//337
	void  SetLeftPos(int,int);	//339
	virtual void  SetShowCaret(int);	//351
public:
	void  SetViewInstance(class CPoint,class CPoint,int,int);	//356
	void  SetWatchMode(int);	//357
protected:
	int  SmartRepaginate(struct CPrintInfo *);	//362
	void  UpdateBars(int);	//366
	int  ValidPoints(void);	//369
	static int  bCounted;	//401
public:
	static unsigned short  cf_template;	//402
protected:
	static long  count;	//410
	static int  m_CurWidthCaretPos;	//411
	static class CUIntArray  m_WidthAbs;	//413
	static class CUIntArray  m_WidthRel;	//414
	static class CTextFRDlg *  pFindReplaceDlg;	//422
	static struct SearchTempl  searchTemplate;	//423
	static class CString  wndClass;	//424

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CBodyTextDoc:public CTextDocument
{
DECLARE_DYNCREATE(CBodyTextDoc)

public:
	char m_docbuf1[24];	// Точный размер
	 CBodyTextDoc(void);	//1
	virtual  ~CBodyTextDoc(void);	//11
	int  AddLine(char const *);	//31
	void  ChangeFont(void);	//53
protected:
	virtual int  DebugModule(void);	//76
public:
	int  GetFontIndex(void);	//117
	class CString   GetLineAsString(int);	//123
	int  IsOnlyForOutput(void);	//180
	int  IsReplaced(void);	//182
protected:
	void  OnColorChange(class CMessage7 *,long *);	//204
	void  OnFontChange(class CMessage7 *,long *);	//243
	void  OnSTChange(class CMessage7 *,long *);	//272
	void  OnTextParamChange(class CMessage7 *,long *);	//280
	virtual int  SaveModified(void);	//328
public:
	virtual void  SetModule(int);	//342
	void  SetNewFontIndex(int);	//344
	virtual void  SetPathName(char const *,int);	//346
	virtual void  SetReadOnly(int);	//347
	void  SetUserTitle(char const *);	//354

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CBodyTextView:public CTextEditor
{
DECLARE_DYNCREATE(CBodyTextView)

public:
	 CBodyTextView(void);	//2
	virtual  ~CBodyTextView(void);	//12
	class CBodyTextDoc *  GetDocument(void)const;	//112
	void  OnDestroy(void);	//210
	void  OnEditCut(void);	//223
	void  OnEditPaste(void);	//229
	void  OnEditRedo(void);	//231
	void  OnEditReplace(void);	//234
	void  OnEditUndo(void);	//237
	virtual void  OnInitialUpdate(void);	//248
	virtual void  OnUpdate(class CView *,long,class CObject *);	//287
	void  OnUpdateEditCut(class CCmdUI *);	//293
	void  OnUpdateEditPaste(class CCmdUI *);	//296
	void  OnUpdateEditRedo(class CCmdUI *);	//298
	void  OnUpdateEditReplace(class CCmdUI *);	//300
	void  OnUpdateEditUndo(class CCmdUI *);	//302

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CEditPoint
{
public:
	 CEditPoint(int,int);	//3
	 CEditPoint(class CPoint);	//4
	 CEditPoint(void);	//5
	class CEditPoint &  operator=(class CEditPoint const &);	//18
	class CPoint &  operator=(class CPoint);	//19
	class CPoint   GetAbsPoint(class CTextEditor *)const;	//107
	class CPoint   GetNormPoint(class CTextEditor *)const;	//138
	void  SetAbsPoint(class CPoint,class CTextEditor *);	//333
	void  SetNormPoint(class CPoint,class CTextEditor *);	//345
};


class IMPORT_1C CMainMessageHandler:public CBodyTextDoc //CObject
{
DECLARE_DYNCREATE(CMainMessageHandler)

public:
	 CMainMessageHandler(void);	//6
	virtual  ~CMainMessageHandler(void);	//13
	void  Activate(int);	//28
	void  ActivateError(int);	//30
	void  ClearAll(void);	//59
protected:
	virtual int  DoFileSave(void);	//81
public:
	int  GetActivate(void);	//108
	int  GetCurrErrorPos(void);	//109
	int  GetSymbol(unsigned int);	//157
protected:
	class CBodyTextView *  GetView(void);	//164
public:
	void  Hide(void);	//170
	static int  IsError(enum MessageMarker);	//178
protected:
	void  OnClear(void);	//203
	void  OnDisable(class CCmdUI *);	//212
	void  OnStubed(void);	//278
	virtual int  SaveModified(void);	//329
public:
	void  SetActivate(int);	//334
	int  ShowError(int);	//358
	int  ShowError(char const *,enum MessageMarker,char const *,long,int);	//359
	int  ShowNextError(void);	//360
	int  ShowPrevError(void);	//361
	void  WriteError(char const *,enum MessageMarker,char const *,long,int,int);	//371
protected:
	void  WriteMessage(char const *,class CMessageInfo &,int);	//372
public:
	void  WriteMessage(char const *,enum MessageMarker,char const *,long,int,int);	//373
	void  WriteMessage(char const *,enum MessageMarker,int);	//374

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMainWatch:public CBodyTextDoc //CObject
{
DECLARE_DYNCREATE(CMainWatch)

public:
	 CMainWatch(void);	//7
	virtual  ~CMainWatch(void);	//14
	void  Activate(void);	//29
protected:
	virtual int  CalcExpr(char const *,class CString &);	//37
public:
	void  ClearAll(void);	//60
protected:
	virtual int  DoFileSave(void);	//82
public:
	class CBodyTextView *  GetView(void);	//165
	void  Hide(void);	//171
protected:
	void  OnDisable(class CCmdUI *);	//213
	void  OnStubed(void);	//279
public:
	void  Refresh(int);	//320
protected:
	virtual int  SaveModified(void);	//330

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMessageView:public CBodyTextView //CObject
{
DECLARE_DYNCREATE(CMessageView)

protected:
	 CMessageView(void);	//8
	virtual  ~CMessageView(void);	//15
	class CMainMessageHandler *  GetDocument(void)const;	//113
	void  OnChar(unsigned int,unsigned int,unsigned int);	//201
	void  OnContextMenu(class CWnd *,class CPoint);	//206
	virtual void  OnDraw(class CDC *);	//217
	int  OnError(int);	//240
	void  OnLButtonDblClk(unsigned int,class CPoint);	//254
	void  OnStubProperty(class CCmdUI *);	//277
	virtual void  OnUpdate(class CView *,long,class CObject *);	//288
	void  OnUpdateToolBar(class CCmdUI *);	//311

	DECLARE_MESSAGE_MAP()
};


// End class descriptions
// Begin free functions definitions
IMPORT_1C int __cdecl ConvertModules(class CDocument *,class CDocument *,class CDocument *,class CMetaDataObj *);
// End free functions definitions
