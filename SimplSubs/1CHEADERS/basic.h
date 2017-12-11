
// Begin forward declarations
class CGetDoc;
class CDocBoost;
// Begin class descriptions
/*
; const CGetDoc7::`vftable'
??_7CGetDoc7@@6B@ dd offset ?GetRuntimeClass@CGetDoc7@@UBEPAUCRuntimeClass@@XZ
; DATA XREF: CGetDoc7::CGetDoc7(void)+1B6.o
; CGetDoc7::~CGetDoc7(void)+20.o
; CGetDoc7::GetRuntimeClass(void)
*/
struct SNotifyActivateOperForm
{
public:
	 SNotifyActivateOperForm(struct SNotifyActivateOperForm const &);	//52
	 SNotifyActivateOperForm(void);	//53
	 ~SNotifyActivateOperForm(void);	//99
	struct SNotifyActivateOperForm &  operator=(struct SNotifyActivateOperForm const &);	//105
};
struct SNotifyUpdateDoc
{
public:
	 SNotifyUpdateDoc(struct SNotifyUpdateDoc const &);	//54
	 SNotifyUpdateDoc(void);	//55
	struct SNotifyUpdateDoc &  operator=(struct SNotifyUpdateDoc const &);	//106
};

class IMPORT_1C CAccContext:public CBLContext
{
DECLARE_DYNCREATE(CAccContext)

public:
	 CAccContext(void);	//1
	virtual  ~CAccContext(void);	//56
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//173
	virtual int  CallAsProc(int,class CValue * *);	//182
	class CAccSet *  GetAccSet(void);	//278
	virtual char const *  GetCode(void)const;	//289
	int  GetEnableChangeParent(void)const;	//324
	virtual void  GetExactValue(class CValue &);	//326
	int  GetFlagChoiseGroup(void)const;	//338
	virtual class CObjID   GetID(void)const;	//349
private:
	class CTypedValue *  GetPValue(int)const;	//410
public:
	virtual int  GetPropVal(int,class CValue &)const;	//424
	virtual long  GetTypeID(void)const;	//471
	class CObjID   GetUseParent(void)const;	//480
	virtual class CType   GetValueType(void)const;	//481
	virtual void  InitObject(class CType const &);	//505
	virtual void  InitObject(char const *);	//506
	void  InitOnModify(int *);	//514
	virtual int  IsExactValue(void)const;	//529
	int  IsReadOnly(void)const;	//539
	void  OnModify(void);	//685
	static void  Register(void);	//820
	virtual void  SelectByID(class CObjID,long);	//835
private:
	void  SendNotify(void);	//840
public:
	void  SetAccSet(class CAccSet *);	//842
	void  SetEnableChangeParent(int);	//872
	void  SetParent(class CObjID);	//909
	virtual int  SetPropVal(int,class CValue const &);	//914
	void  SetReadOnly(int);	//922
	void  SetStartFlagChoiseGroup(int);	//932
	void  SetUseParent(class CObjID);	//947
	static void  UnRegister(void);	//972
};


class IMPORT_1C CAllRegsContext:public CBLContext
{
DECLARE_DYNCREATE(CAllRegsContext);

//	DECLARE_DYNAMIC(CAllRegsContext)
//public:
//	static const AFX_DATA CRuntimeClass classCAllRegsContext;
//	virtual CRuntimeClass* GetRuntimeClass() const;

	
//	static CObject* PASCAL CreateObject();


public:
	 CAllRegsContext(void);	//2
	virtual  ~CAllRegsContext(void);	//57
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//174
	virtual int  CallAsProc(int,class CValue * *);	//183
	virtual char const *  GetMethodName(int,int)const;	//382
	virtual int  GetNMethods(void)const;	//388
	virtual int  GetNParams(int)const;	//393
	virtual int  GetNProps(void)const;	//398
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//415
	virtual char const *  GetPropName(int,int)const;	//421
	virtual int  GetPropVal(int,class CValue &)const;	//425
	virtual int  HasRetVal(int)const;	//486
	void  InitContext(void);	//496
	virtual void  InitObject(char const *);	//507
	virtual int  IsPropReadable(int)const;	//534
	virtual int  IsPropWritable(int)const;	//536
	static void  Register(void);	//821
	void  SetPostState(struct SDocPostState *);	//912
	virtual int  SetPropVal(int,class CValue const &);	//915
	int  TranPropPos(int &)const;	//966
	static void  UnRegister(void);	//973
};


class IMPORT_1C CBasicProfile
{
public:
	 CBasicProfile(void);	//3
	virtual  ~CBasicProfile(void);	//58
	class CDate   GetJournalBeg(void);	//357
	class CDate   GetJournalEnd(void);	//358
	int  GetRangeVarBeg(void);	//432
	int  GetRangeVarEnd(void);	//433
protected:
	static struct CProfileEntry7 *  s_Entries;	//1092
};


class IMPORT_1C CBrowseView7:public CBrowseView //CWnd //CObject
{
DECLARE_DYNCREATE(CBrowseView7)

protected:
	 CBrowseView7(int,unsigned int);	//4
	 CBrowseView7(void);	//5
public:
	virtual  ~CBrowseView7(void);	//59
	class CGetDoc7 *  GetDocument(void)const;	//320
protected:
	void  OnControlWhatIsIt(void);	//619
	int  OnCreate(struct tagCREATESTRUCTA *);	//623
	void  OnDestroy(void);	//638
	int  OnEraseBkgnd(class CDC *);	//654
	virtual void  OnInitialUpdate(void);	//678
	long  OnPrivateSetFocus(unsigned int,long);	//723
	void  OnSStringNeedText(struct tagNMHDR *,long *);	//727
	long  OnSayAboutControl(unsigned int,long);	//728
	void  OnSetFocus(class CWnd *);	//737
	void  OnSize(unsigned int,int,int);	//744
	void  OnToolBarNeedText(struct tagNMHDR *,long *);	//763
	long  OnWMUpdateUI(unsigned int,long);	//778
	void  OnWinIniChange(char const *);	//779
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//780
public:
	void  UpdateValue(class CRect const &);	//985
protected:
	void  Update_F4_Key(class CCmdUI *);	//988

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCalcVarSelDlg:public CDialog
{
public:
	 CCalcVarSelDlg(class CWnd *);	//6
	virtual  ~CCalcVarSelDlg(void);	//60
//	void  `default constructor closure'(void);	//144
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//236
	virtual int  OnInitDialog(void);	//665
	virtual void  OnOK(void);	//697
public:
	void  SetStartType(long);	//933

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCorrectProvContext:public CObject
{
DECLARE_DYNCREATE(CCorrectProvContext)

public:
	 CCorrectProvContext(void);	//7
	virtual  ~CCorrectProvContext(void);	//61
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//175
	virtual int  CallAsProc(int,class CValue * *);	//184
	virtual char const *  GetMethodName(int,int)const;	//383
	virtual int  GetNMethods(void)const;	//389
	virtual int  GetNParams(int)const;	//394
	virtual int  GetNProps(void)const;	//399
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//416
	virtual char const *  GetPropName(int,int)const;	//422
	virtual int  GetPropVal(int,class CValue &)const;	//426
	virtual char const *  GetTypeString(void)const;	//473
	virtual int  HasRetVal(int)const;	//487
	virtual int  IsPropReadable(int)const;	//535
	virtual int  IsPropWritable(int)const;	//537
private:
	void  P_Delete(class CValue &,class CValue * *);	//784
	void  P_Get(class CValue &,class CValue * *);	//785
	void  P_IsOK(class CValue &,class CValue * *);	//787
	void  P_New(class CValue * *);	//789
	void  P_Select(class CValue &,class CValue * *);	//792
	void  P_SelectByAcc(class CValue &,class CValue * *);	//793
	void  P_Write(class CValue &,class CValue * *);	//799
public:
	static void  Register(void);	//822
	virtual int  SetPropVal(int,class CValue const &);	//916
	static void  UnRegister(void);	//974
};


class IMPORT_1C CDocActModule
{
public:
	 CDocActModule(void);	//8
	virtual  ~CDocActModule(void);	//62
	class CDocContext *  GetDocContext(void);	//315
	int  TestIsPostingProc(void);	//964
};


class IMPORT_1C CDocContext:public CBLContext //CObject
{
DECLARE_DYNCREATE(CDocContext)

public:
friend CDocBoost;
	CDocDef* pDocDef;
	DWORD buf[100];

	 CDocContext(void);	//9
	virtual  ~CDocContext(void);	//63
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//176
	virtual int  CallAsProc(int,class CValue * *);	//185
protected:
	virtual class CDocDef *  CurDocDef(void)const;	//219
public:
	virtual char const *  GetCode(void)const;	//290
	int  GetCurLineIdx(void)const;	//300
	virtual void  GetExactValue(class CValue &);	//327
	int  GetFlagAutoTime(void);	//337
	virtual class CObjID   GetID(void)const;	//350
private:
	class CTypedValue *  GetPTypedValue(int,int)const;	//408
public:
	virtual int  GetPropVal(int,class CValue &)const;	//427
	int  GetSelectType(void);	//455
	virtual long  GetTypeID(void)const;	//472
	class CString   GetUseKindsForChoise(void);	//478
	class CString   GetUseNumPrefix(void);	//479
	virtual class CType   GetValueType(void)const;	//482
	void  InitContext(void);	//497
	virtual void  InitObject(class CType const &);	//508
	virtual void  InitObject(char const *);	//509
	void  InitOnModify(int *,int *);	//515
	void  InitOperVal(class COperSet *);	//516
private:
	void  InitTransacter(void);	//519
protected:
	int  IsDocAvailable(void)const;	//527
public:
	virtual int  IsExactValue(void)const;	//530
	int  IsNewNumSetted(void)const;	//533
	int  IsReadOnly(void)const;	//540
	void  OnModify(int)const;	//686
	static void  Register(void);	//823
	virtual void  SelectByID(class CObjID,long);	//836
private:
	void  SendNotify(void);	//841
public:
	void  SetCalcTotals(int);	//846
	void  SetCurLineIdx(int);	//858
	void  SetDocSet(class CDocSet *);	//868
	void  SetEnableDateChanging(int);	//874
	void  SetEnableNewLine(int);	//876
	void  SetFlagAutoTime(int);	//882
	void  SetOper(class CBLContext *,class COperSet *);	//905
	virtual int  SetPropVal(int,class CValue const &);	//917
	void  SetReadOnly(int);	//923
	void  SetSelectType(int);	//927
private:
	int  SkipByFilter(void);	//953
public:
	void  SyncWithOperPos(void)const;	//958
	static void  UnRegister(void);	//975
};


class IMPORT_1C CDocEnvironContext:public CBLContext
{
DECLARE_DYNCREATE(CDocEnvironContext)

public:
	 CDocEnvironContext(void);	//10
	virtual  ~CDocEnvironContext(void);	//64
	void  AfterPost(void);	//167
	void  BeforePost(void);	//169
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//177
	virtual int  CallAsProc(int,class CValue * *);	//186
	class CAllRegsContext *  GetAllRegs(void);	//282
	int  GetFlagNotCloseDoc(void);	//340
	struct SDocPostState *  GetPostState(void);	//420
	virtual int  GetPropVal(int,class CValue &)const;	//428
	void  InitContext(void);	//498
	void  SetFlagGroupProcess(int);	//884
	void  SetFlagTotalsActual(int);	//887
	void  SetRepeatToTM(int);	//926
};


class IMPORT_1C CDocFindDlg: public CDialog
{
public:
	 CDocFindDlg(class CWnd *);	//11
	virtual  ~CDocFindDlg(void);	//65
//	void  `default constructor closure'(void);	//145
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//237
private:
	void  EnableCtrls(void);	//255
protected:
	void  OnChangeDocNum(void);	//600
	void  OnDblclkDoclist(void);	//628
	void  OnDblclkDoclistFinded(void);	//629
	virtual int  OnInitDialog(void);	//666
	virtual void  OnOK(void);	//698
	void  OnOnlyInJournalRange(void);	//710
	void  OnSelchangeDoclist(void);	//731
	void  OnSelectAll(void);	//734
	void  OnStartFind(void);	//752
	void  OnUnSelectAll(void);	//766
public:
	void  SetEnv(long,class CDate,class CDate,long,class CString,long);	//877
	void  SetOnlyBookKeeping(int);	//904
	void  SetSpecTitle(class CString);	//929

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CGetDoc7:public CGetDoc //CCmdTarget //CObject //CGetDoc //CGetDoc77
{
DECLARE_DYNCREATE(CGetDoc7)
public:
	enum MsgType {msgtOne};
//	CString mm_strTitle;
//	CString mm_strPathName;
//	CDocTemplate* mm_pDocTemplate;
//	CPtrList mm_viewList;
//	BOOL mm_bModified;
//	BOOL mm_bAutoDelete;
//	BOOL mm_bEmbedded;
//	int m_buf1;
//	CObArray mm_ctrlList;

	DWORD buf[0x11B]; //(50Ch - A0h) / 4
/*
+0F8h = buf[22] = CImageList
+***h = buf[34] = CFormContext*
+***h = buf[35] = CGroupContext*
+134h = buf[37] = CToolTip7
+19Ch = buf[63] = COnLineHelp
+1FCh = buf[87] = CFntHolder7
+21Ch = buf[95] = CAccelTable7
+228h = buf[98] = CValue
+2A8h = buf[130] = CControlID
+354h = buf[173] = CControlID
+404h = buf[217] = CPtrList
+424h = buf[225] = CFont
+42Ch = buf[227] = CGetFieldsArray
+480h = buf[248] = CApp7::GetToolBarPos()
+48Ch = buf[251] = CString
+4A4h = buf[257] = CString
+4C4h = buf[265] = CString
+4E0h = buf[272] = CBLModule7

+***h = buf[53]-[59] = CMapStringToPtr
+***h = buf[88]-[94] = CPtrList
+***h = buf[127] = CTypedCont*
+***h = buf[138] = размер по горизонтали во внутренних единицах
+***h = buf[139] = размер по вертикали во внутренних единицах
+***h = buf[148] = заголовок формы
+***h = buf[168] = CPictureHolder7
+***h = buf[211] = CPictureHolder7
+***h = buf[224] = CEditDoc*
+***h = buf[225] = CFont
+***h = buf[227] = CObArray
+***h = buf[249] = CWorkBookDoc*
+***h = buf[250] = id объекта
+***h = buf[257] = наименование потока в md
+***h = buf[282] = CBLModule7*
*/
protected:
	virtual  ~CGetDoc7(void);
//dd offset ?OnCmdMsg@CGetDoc@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z ; CGetDoc::OnCmdMsg(uint,int,void *,AFX_CMDHANDLERINFO *)
//dd offset ?OnFinalRelease@CDocument@@UAEXXZ ; CDocument::OnFinalRelease(void)
//dd offset ?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z ; CCmdTarget::IsInvokeAllowed(long)
//dd offset ?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z ; CCmdTarget::GetDispatchIID(_GUID *)
//dd offset ?GetTypeInfoCount@CCmdTarget@@UAEIXZ ; CCmdTarget::GetTypeInfoCount(void)
//dd offset ?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ ; CCmdTarget::GetTypeLibCache(void)
//dd offset ?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z ; CCmdTarget::GetTypeLib(ulong,ITypeLib * *)
//dd offset ?GetMessageMap@CGetDoc7@@MBEPBUAFX_MSGMAP@@XZ ; CGetDoc7::GetMessageMap(void)
//dd offset ?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ ; CCmdTarget::GetCommandMap(void)
//dd offset ?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ ; CCmdTarget::GetDispatchMap(void)
//dd offset ?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ ; CCmdTarget::GetConnectionMap(void)
//dd offset ?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ ; CCmdTarget::GetInterfaceMap(void)
//dd offset ?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ ; CCmdTarget::GetEventSinkMap(void)
//dd offset ?OnCreateAggregates@CCmdTarget@@UAEHXZ ; CCmdTarget::OnCreateAggregates(void)
//dd offset ?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z ; CCmdTarget::GetInterfaceHook(void const *)
//dd offset ?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z ; CCmdTarget::GetExtraConnectionPoints(CPtrArray *)
//dd offset ?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z ; CCmdTarget::GetConnectionHook(_GUID const &)
//dd offset ?SetTitle@CDocument@@UAEXPBD@Z ; CDocument::SetTitle(char const *)
//dd offset ?SetPathName@CGetDoc7@@MAEXPBDH@Z ; CGetDoc7::SetPathName(char const *,int)
protected:	virtual void  SetPathName(char const *,int);	//910
//dd offset ?GetFirstViewPosition@CDocument@@UBEPAU__POSITION@@XZ ; CDocument::GetFirstViewPosition(void)
//dd offset ?GetNextView@CDocument@@UBEPAVCView@@AAPAU__POSITION@@@Z ; CDocument::GetNextView(__POSITION * &)
//dd offset ?OnChangedViewList@CDocument@@UAEXXZ ; CDocument::OnChangedViewList(void)
//dd offset ?DeleteContents@CDocument@@UAEXXZ ; CDocument::DeleteContents(void)
//dd offset ?OnNewDocument@CGetDoc7@@UAEHXZ ; CGetDoc7::OnNewDocument(void)
public:	virtual int  OnNewDocument(void);	//692
//dd offset ?OnOpenDocument@CGetDoc7@@UAEHPBD@Z ; CGetDoc7::OnOpenDocument(char const *)
public:	virtual int  OnOpenDocument(char const *);	//715
//dd offset ?OnSaveDocument@CDocument@@UAEHPBD@Z ; CDocument::OnSaveDocument(char const *)
//dd offset ?OnCloseDocument@CGetDoc7@@UAEXXZ ; CGetDoc7::OnCloseDocument(void)
public:	virtual void  OnCloseDocument(void);	//613
//dd offset ?ReportSaveLoadException@CDocument@@UAEXPBDPAVCException@@HI@Z ; CDocument::ReportSaveLoadException(char const *,CException *,int,uint)
//dd offset ?GetFile@CDocument@@UAEPAVCFile@@PBDIPAVCFileException@@@Z ; CDocument::GetFile(char const *,uint,CFileException *)
//dd offset ?ReleaseFile@CDocument@@UAEXPAVCFile@@H@Z ; CDocument::ReleaseFile(CFile *,int)
//dd offset ?CanCloseFrame@CGetDoc7@@UAEHPAVCFrameWnd@@@Z ; CGetDoc7::CanCloseFrame(CFrameWnd *)
public:	virtual int  CanCloseFrame(class CFrameWnd *);	//189
//dd offset ?SaveModified@CGetDoc7@@UAEHXZ ; CGetDoc7::SaveModified(void)
public:	virtual int  SaveModified(void);	//829
//dd offset ?PreCloseFrame@CGetDoc@@MAEXPAVCFrameWnd@@@Z ; CGetDoc::PreCloseFrame(CFrameWnd *)
//dd offset ?DoSave@CDocument@@UAEHPBDH@Z ; CDocument::DoSave(char const *,int)
//dd offset ?DoFileSave@CDocument@@UAEHXZ ; CDocument::DoFileSave(void)
//dd offset ?UpdateFrameCounts@CDocument@@UAEXXZ ; CDocument::UpdateFrameCounts(void)
//dd offset ?GetDefaultMenu@CDocument@@UAEPAUHMENU__@@XZ ; CDocument::GetDefaultMenu(void)
//dd offset ?GetDefaultAccelerator@CGetDoc7@@MAEPAUHACCEL__@@XZ ; CGetDoc7::GetDefaultAccelerator(void)
protected:	virtual struct HACCEL__ *  GetDefaultAccelerator(void);	//310
//dd offset ?OnIdle@CGetDoc7@@MAEXXZ ; CGetDoc7::OnIdle(void)
protected:	virtual void  OnIdle(void);	//662
//dd offset ?GetForm@CGetDoc7@@UAEPAVCWnd@@XZ ; CGetDoc7::GetForm(void)
public:	virtual class CWnd *  GetForm(void);	//343
//dd offset ?OnMakeUse@CGetDoc7@@UAEII@Z ; CGetDoc7::OnMakeUse(uint)
public:	virtual unsigned int  OnMakeUse(unsigned int);	//683
//dd offset ?OnAbortUse@CGetDoc7@@UAEXXZ ; CGetDoc7::OnAbortUse(void)
public:	virtual void  OnAbortUse(void);	//570
//dd offset ?UpdateData@CGetDoc7@@UAEHHI@Z ; CGetDoc7::UpdateData(int,uint)
public:	virtual int  UpdateData(int,unsigned int);	//980
//dd offset ?OnActivateCtrl@CGetDoc7@@MAEXI@Z ; CGetDoc7::OnActivateCtrl(uint)
protected:	virtual void  OnActivateCtrl(unsigned int);	//573
//dd offset ?GetStringData@CGetDoc@@UAEXAAVCString@@I@Z ; CGetDoc::GetStringData(CString &,uint)

//dd offset ?SetModify@CGetDoc7@@UAEXH@Z ; CGetDoc7::SetModify(int)
public:	virtual void  SetModify(int);	//901
//dd offset ?PreTranslateMessage@CGetDoc7@@UAEHPAUtagMSG@@@Z ; CGetDoc7::PreTranslateMessage(tagMSG *)
public:	virtual int  PreTranslateMessage(struct tagMSG *);	//806
//dd offset ?DeleteCurrent@CGetDoc@@UAEHXZ ; CGetDoc::DeleteCurrent(void)
//dd offset ?GetSavePrompt@CGetDoc@@UAEXAAVCString@@@Z ; CGetDoc::GetSavePrompt(CString &)
//dd offset ?GetDeletePrompt@CGetDoc@@UAEXAAVCString@@@Z ; CGetDoc::GetDeletePrompt(CString &)
//dd offset ?DataExchange@CGetDoc7@@UAEHPAVCGetExchange@@@Z ; CGetDoc7::DataExchange(CGetExchange *)
public:	virtual int  DataExchange(class CGetExchange *);	//221
//---------------------------------
//dd offset ?UpdateDefCommand@CGetDoc7@@UAEXXZ ; CGetDoc7::UpdateDefCommand(void)
public:	virtual void  UpdateDefCommand(void);	//981
//dd offset ?ReallyRefresh@CGetDoc7@@MAEXXZ ; CGetDoc7::ReallyRefresh(void)
protected:	virtual void  ReallyRefresh(void);	//814
//dd offset ?DescribeToolBar@CGetDoc7@@UAEXPAVCWnd@@PAVCToolBar7@@PBIH@Z ; CGetDoc7::DescribeToolBar(CWnd *,CToolBar7 *,uint const *,int)
public:	virtual void  DescribeToolBar(class CWnd *,class CToolBar7 *,unsigned int const *,int);	//229
//dd offset ?OnStartBatch@CGetDoc7@@UAEXXZ ; CGetDoc7::OnStartBatch(void)
/**/public:	virtual void  OnStartBatch(void);	//751
//dd offset ?OnEndBatch@CGetDoc7@@UAEXH@Z ; CGetDoc7::OnEndBatch(int)
/**/public:	virtual void  OnEndBatch(int);	//645
//dd offset ?OnStartRunModule@CGetDoc7@@UAEXPAVCGetField@@@Z ; CGetDoc7::OnStartRunModule(CGetField *)
public:	virtual void  OnStartRunModule(class CGetField *);	//756
//dd offset ?OnEndRunModule@CGetDoc7@@UAEXPAVCGetField@@@Z ; CGetDoc7::OnEndRunModule(CGetField *)
public:	virtual void  OnEndRunModule(class CGetField *);	//648
//dd offset ?MakeUseHandling@CGetDoc7@@UAEHXZ ; CGetDoc7::MakeUseHandling(void)
public:	virtual int  MakeUseHandling(void);	//565
//dd offset ?GetUseHandling@CGetDoc7@@UAEHPAV1@PAVCValue@@@Z ; CGetDoc7::GetUseHandling(CGetDoc7 *,CValue *)
public:	virtual int  GetUseHandling(class CGetDoc7 *,class CValue *);	//477
//dd offset ?OnSelectLine@CGetDoc7@@UAEHXZ ; CGetDoc7::OnSelectLine(void)
public:	virtual int  OnSelectLine(void);	//736
//dd offset ?DetermineDefCommand@CGetDoc7@@UAEXXZ ; CGetDoc7::DetermineDefCommand(void)
public:	virtual void  DetermineDefCommand(void);	//231
//dd offset ?ExternEventProcess@CGetDoc7@@UAEXPBD@Z ; CGetDoc7::ExternEventProcess(char const *)
public:	virtual void  ExternEventProcess(char const *);	//263
//dd offset ?FillUseValue@CGetDoc7@@UAEHPAVCValue@@@Z ; CGetDoc7::FillUseValue(CValue *)
public:	virtual int  FillUseValue(class CValue *);	//274
//dd offset ?GetPDate@CGetDoc7@@UAEPAVCDate@@XZ ; CGetDoc7::GetPDate(void)
public:	virtual class CDate *  GetPDate(void);	//406
//dd offset ?OnGetUse@CGetDoc7@@UAEHIIPAVCValue@@@Z ; CGetDoc7::OnGetUse(uint,uint,CValue *)
public:	virtual int  OnGetUse(unsigned int,unsigned int,class CValue *);	//658
//dd offset ?OnPermanentChoise@CGetDoc7@@UAEHPAV1@PAVCValue@@@Z ; CGetDoc7::OnPermanentChoise(CGetDoc7 *,CValue *)
public:	virtual int  OnPermanentChoise(class CGetDoc7 *,class CValue *);	//722
//dd offset ?SetDocTitle@CGetDoc7@@UAEXXZ ; CGetDoc7::SetDocTitle(void)
public:	virtual void  SetDocTitle(void);	//870
//dd offset ?LoadAsExternalForm@CGetDoc7@@UAEHPAVCWorkBookDoc@@PAVCTemplate7@@HH@Z ; CGetDoc7::LoadAsExternalForm(CWorkBookDoc *,CTemplate7 *,int,int)
public:	virtual int  LoadAsExternalForm(class CWorkBookDoc *,class CTemplate7 *,int,int);	//553
//dd offset ?ValidateMenu@CGetDoc7@@MAEXPAVCMenu@@@Z ; CGetDoc7::ValidateMenu(CMenu *)
protected:	virtual void  ValidateMenu(class CMenu *);	//991
//dd offset ?PrepareModule@CGetDoc7@@MAEHPAVCTxtModuleReader@@@Z ; CGetDoc7::PrepareModule(CTxtModuleReader *)
protected:	virtual int  PrepareModule(class CTxtModuleReader *);	//807
//dd offset ?SetDocModify@CGetDoc7@@UAEXH@Z ; CGetDoc7::SetDocModify(int)
public:	virtual void  SetDocModify(int);	//865
//dd offset ?GetField@CGetDoc7@@UAEPAVCGetField@@I@Z ; CGetDoc7::GetField(uint)
public:	virtual class CGetField *  GetField(unsigned int);	//328
//dd offset ?GetField@CGetDoc7@@UAEPAVCGetField@@PBD@Z ; CGetDoc7::GetField(char const *)
public:	virtual class CGetField *  GetField(char const *);	//329
//dd offset ?GetFieldWnd@CGetDoc7@@UAEPAVCWnd@@PAVCGetField@@@Z ; CGetDoc7::GetFieldWnd(CGetField *)
public:	virtual class CWnd *  GetFieldWnd(class CGetField *);	//332
//dd offset ?UpdateFieldValue@CGetDoc7@@UAEHIHH@Z ; CGetDoc7::UpdateFieldValue(uint,int,int)
public:	virtual int  UpdateFieldValue(unsigned int,int,int);	//982
//dd offset ?OnCreateFrame@CGetDoc7@@UAEHPAVCFrameWnd@@PAUCCreateContext@@@Z ; CGetDoc7::OnCreateFrame(CFrameWnd *,CCreateContext *)
public:	virtual int  OnCreateFrame(class CFrameWnd *,struct CCreateContext *);	//626
//dd offset ?GetEditedValueField@CGetDoc7@@UAEPAVCGetField@@XZ ; CGetDoc7::GetEditedValueField(void)
public:	virtual class CGetField *  GetEditedValueField(void);	//322
//dd offset ?PostClearField@CGetDoc7@@UAEXXZ ; CGetDoc7::PostClearField(void)
public:	virtual void  PostClearField(void);	//800
//dd offset ?OnButton@CGetDoc7@@UAEXI@Z ; CGetDoc7::OnButton(uint)
public:	virtual void  OnButton(unsigned int);	//588
//dd offset ?OnEnyChanged@CGetDoc7@@UAEXIH@Z ; CGetDoc7::OnEnyChanged(uint,int)
public:	virtual void  OnEnyChanged(unsigned int,int);	//653
//dd offset ?PreCloseDoc@CGetDoc7@@UAEHH@Z ; CGetDoc7::PreCloseDoc(int)
public:	virtual int  PreCloseDoc(int);	//803
//dd offset ?OnCloseForm@CGetDoc7@@UAEHXZ ; CGetDoc7::OnCloseForm(void)
public:	virtual int  OnCloseForm(void);	//614
//dd offset ?ActivateObj@CGetDoc7@@UAEHVCValue@@HHH@Z ; CGetDoc7::ActivateObj(CValue,int,int,int)
public:	virtual int  ActivateObj(class CValue,int,int,int);	//160
//dd offset ?UpdateWatchs@CGetDoc7@@UAEXHHHPAVCGetField@@@Z ; CGetDoc7::UpdateWatchs(int,int,int,CGetField *)
public:	virtual void  UpdateWatchs(int,int,int,class CGetField *);	//987
//dd offset ?LinkAllFormEdits@CGetDoc7@@UAEXXZ ; CGetDoc7::LinkAllFormEdits(void)
public:	virtual void  LinkAllFormEdits(void);	//551
//dd offset ?EvalFormula@CGetDoc7@@UAEHPAVCGetField@@@Z ; CGetDoc7::EvalFormula(CGetField *)
public:	virtual int  EvalFormula(class CGetField *);	//261
//dd offset ?EvalFormula@CGetDoc7@@UAEHI@Z ; CGetDoc7::EvalFormula(uint)
public:	virtual int  EvalFormula(unsigned int);	//260
//dd offset ?LayoutForm@CGetDoc7@@UAEHPAVCFormView@@@Z ; CGetDoc7::LayoutForm(CFormView *)
public:	virtual int  LayoutForm(class CFormView *);	//550
//dd offset ?OnMessageInModalMode@CGetDoc7@@UAEHPAUtagMSG@@@Z ; CGetDoc7::OnMessageInModalMode(tagMSG *)
public:	virtual int  OnMessageInModalMode(struct tagMSG *);	//684
//dd offset ?OnToolTipNeedText@CGetDoc7@@UAEHIAAVCString@@@Z ; CGetDoc7::OnToolTipNeedText(uint,CString &)
public:	virtual int  OnToolTipNeedText(unsigned int,class CString &);	//764
//dd offset ?OnStatusBarNeedText@CGetDoc7@@UAEHIAAVCString@@@Z ; CGetDoc7::OnStatusBarNeedText(uint,CString &)
public:	virtual int  OnStatusBarNeedText(unsigned int,class CString &);	//757
//dd offset ?OnAdjusteMenu@CGetDoc7@@UAEXIPAVCMenu@@@Z ; CGetDoc7::OnAdjusteMenu(uint,CMenu *)
public:	virtual void  OnAdjusteMenu(unsigned int,class CMenu *);	//574
//dd offset ?ChangeDeleteStr@CGetDoc7@@UAEHAAH@Z ; CGetDoc7::ChangeDeleteStr(int &)
public:	virtual int  ChangeDeleteStr(int &);	//190

protected:
	 CGetDoc7(void);	//21
//	virtual  ~CGetDoc7(void);	//73
public:
//	virtual int  ActivateObj(class CValue,int,int,int);	//160
//	virtual int  CanCloseFrame(class CFrameWnd *);	//189
//	virtual int  ChangeDeleteStr(int &);	//190
	void  CorrectActiveCtrl(void);	//201
	class CWnd *  CreateControl(class CGetField *,unsigned int,class CRect const &,class CWnd *,unsigned long,unsigned long);	//203
//**/	virtual void  OnStartBatch(void);	//751
//**/	virtual void  OnEndBatch(int);	//645
//	virtual int  DataExchange(class CGetExchange *);	//221
//	virtual void  DescribeToolBar(class CWnd *,class CToolBar7 *,unsigned int const *,int);	//229
//	virtual void  DetermineDefCommand(void);	//231
	int  DoCloseForm(void);	//235
	int  DoUse(class CValue *);	//250
protected:
	void  EnableRefresh(int);	//259
public:
//	virtual int  EvalFormula(unsigned int);	//260
//	virtual int  EvalFormula(class CGetField *);	//261
	int  ExistWindow(void);	//262
//	virtual void  ExternEventProcess(char const *);	//263
//	virtual int  FillUseValue(class CValue *);	//274
	class CGetField *  GetActiveField(void);	//280
	int  GetDefButtonIndex(void);	//309
protected:
//	virtual struct HACCEL__ *  GetDefaultAccelerator(void);	//310
public:
	unsigned int  GetDocID(void);	//317
	unsigned int  GetDocOwner(void);	//318
	unsigned int  GetDocOwnerCtrl(void);	//319
	class CGetField *  GetEditedField(void);	//321
//	virtual class CGetField *  GetEditedValueField(void);	//322
//	virtual class CGetField *  GetField(unsigned int);	//328
//	virtual class CGetField *  GetField(char const *);	//329
	static class CGetField *  GetFieldInMakeUseHadling(void);	//331
//	virtual class CWnd *  GetFieldWnd(class CGetField *);	//332
	int  GetFlagAutoClose(void);	//336
//	virtual class CWnd *  GetForm(void);	//343
	class CFormContext *  GetFormContext(void);	//344
	class CValue *  GetFormParam(void);	//345
	class CWnd *  GetFrame(void);	//346
	class CGroupContext *  GetGroupContext(void);	//347
	int  GetHardReadOnly(void);	//348
	class CIdleHandler *  GetIdleHandler(void);	//353
	class CImageList const &  GetImageList(void)const;	//354
protected:
	void  GetMsgString(enum CGetDoc7::MsgType,unsigned int,class CString &);	//387
public:
	class COnLineHelp &  GetOnLineHelp(void);	//401
	int  GetOpenForChoise(void);	//402
	int  GetOpenForPermanentChoise(void);	//403
//	virtual class CDate *  GetPDate(void);	//406
protected:
	long  GetParam(void);	//414
public:
	unsigned int  GetPermanentChoise(void);	//419
	int  GetTabCtrlUsed(void);	//467
	class CToolTip7 &  GetToolTipCtrl(void);	//468
//	virtual int  GetUseHandling(class CGetDoc7 *,class CValue *);	//477
	int  HasBrowse(void)const;	//485
	void  InitControlsState(void);	//500
	void  InitFieldsState(void);	//504
protected:
	int  InitStandartFields(class CGetField *,class CControlID *);	//518
public:
	int  IsAutoSize(void);	//522
	int  IsBatchStarted(void);	//523
	int  IsBigForm(void)const;	//524
	int  IsBrowseVisible(void)const;	//526
	int  IsDocLayouted(void);	//528
	int  IsFunctionRun(void);	//531
protected:
	int  IsRefreshEnabled(void);	//541
	int  IsRefreshNeeded(void)const;	//542
public:
	int  LayoutControl(class CFormView *,class CGetField *);	//547 // добавляет реквизит на форму
	int  LayoutDialog(class CFormView *);	//548
//	virtual int  LayoutForm(class CFormView *);	//550
//	virtual void  LinkAllFormEdits(void);	//551
//	virtual int  LoadAsExternalForm(class CWorkBookDoc *,class CTemplate7 *,int,int);	//553
protected:
	int  LoadDialog(long,char const *);	//554
	int  LoadForm(long,char const *);	//555
public:
//	virtual int  MakeUseHandling(void);	//565
//	virtual void  OnAbortUse(void);	//570
protected:
	void  OnAccelControl(unsigned int);	//571
//	virtual void  OnActivateCtrl(unsigned int);	//573
public:
//	virtual void  OnAdjusteMenu(unsigned int,class CMenu *);	//574
protected:
	void  OnBrContextMenu(void);	//582
	void  OnBrowseWantChPos(void);	//586
public:
//	virtual void  OnButton(unsigned int);	//588
protected:
	void  OnCancel(void);	//595
	void  OnChangeFont(class CMessage7 *,long *);	//601
	void  OnClearField(void);	//609
public:
//	virtual void  OnCloseDocument(void);	//613
//	virtual int  OnCloseForm(void);	//614
protected:
	void  OnCmdDoUse(void);	//615
	void  OnCmdGetUse(class CMessage7 *,long *);	//616
public:
//	virtual int  OnCreateFrame(class CFrameWnd *,struct CCreateContext *);	//626
protected:
	void  OnEditCopy(void);	//641
	void  OnEditCut(void);	//642
	void  OnEditPaste(void);	//643
public:
//	virtual void  OnEndBatch(int);	//645
//	virtual void  OnEndRunModule(class CGetField *);	//648
protected:
	void  OnEnter(void);	//651
public:
//	virtual void  OnEnyChanged(unsigned int,int);	//653
//	virtual int  OnGetUse(unsigned int,unsigned int,class CValue *);	//658
protected:
	void  OnHelpTopic(void);	//661
//	virtual void  OnIdle(void);	//662
public:
//	virtual unsigned int  OnMakeUse(unsigned int);	//683
//	virtual int  OnMessageInModalMode(struct tagMSG *);	//684
//	virtual int  OnNewDocument(void);	//692
protected:
	void  OnNotify(class CMessage7 *,long *);	//696
public:
//	virtual int  OnOpenDocument(char const *);	//715
protected:
	void  OnOtherDocClosed(class CMessage7 *,long *);	//720
public:
//	virtual int  OnPermanentChoise(class CGetDoc7 *,class CValue *);	//722
//	virtual int  OnSelectLine(void);	//736
protected:
	void  OnSpecCloseDocument(void);	//749
public:
//	virtual void  OnStartBatch(void);	//751
//	virtual void  OnStartRunModule(class CGetField *);	//756
//	virtual int  OnStatusBarNeedText(unsigned int,class CString &);	//757
protected:
	void  OnTabPageChanged(struct tagNMHDR *,long *);	//760
	void  OnTabPageChanging(struct tagNMHDR *,long *);	//761
public:
//	virtual int  OnToolTipNeedText(unsigned int,class CString &);	//764
protected:
	void  OnUpdateCmdButtons(class CCmdUI *);	//770
public:
	void  OnUseLayerCalled(void);	//775
//	virtual void  PostClearField(void);	//800
//	virtual int  PreCloseDoc(int);	//803
//	virtual int  PreTranslateMessage(struct tagMSG *);	//806
protected:
//	virtual int  PrepareModule(class CTxtModuleReader *);	//807
public:
	void  ProcessSecondOpen(class CValue *);	//808
protected:
//	virtual void  ReallyRefresh(void);	//814
private:
	void  RedirectMessageToParentFrame(struct tagMSG *);	//816
public:
	void  Refresh(void);	//817
protected:
	void  RefreshStdList(void);	//819
public:
//	virtual int  SaveModified(void);	//829
	void  SetBrowseVisible(int,int);	//845
	void  SetDefButtonIndex(int);	//861
//	virtual void  SetDocModify(int);	//865
	void  SetDocOwner(unsigned int,unsigned int);	//866
//	virtual void  SetDocTitle(void);	//870
	void  SetFlagAutoClose(int);	//881
	void  SetFlipFlop(void);	//889
	void  SetFormParam(class CValue *);	//891
	void  SetHardReadOnly(int);	//892
//	virtual void  SetModify(int);	//901
	void  SetModifyFlagsByCtrl(unsigned int);	//902
protected:
//	virtual void  SetPathName(char const *,int);	//910
public:
	void  SetPermanentChoise(unsigned int);	//911
	void  SetTabCtrlUsed(int);	//936
	void  SetTabCtrlVisible(int);	//938
protected:
	void  SetTableSourceForms(class CWorkBookDoc *,unsigned int);	//940
public:
	void  SetWantActivate(int);	//948
	void  SetWantClose(int,int);	//949
	void  SetWndCaption(char const *);	//950
	int  TestAskOnClose(void);	//960
	int  TestCanActivate(unsigned int);	//961
protected:
	int  TestInFormulaForm(unsigned int,int &,int &);	//963
public:
	void  TrySetFocusOnCtrl(class CGetField *,int);	//969
	void  TrySetFocusOnCtrl(class CString,int);	//970
	void  UDFCaption(class CString &);	//971
//	virtual int  UpdateData(int,unsigned int);	//980
//	virtual void  UpdateDefCommand(void);	//981
//	virtual int  UpdateFieldValue(unsigned int,int,int);	//982
//	virtual void  UpdateWatchs(int,int,int,class CGetField *);	//987
	void  VTSetCurrentRow(void);	//989
protected:
//	virtual void  ValidateMenu(class CMenu *);	//991

	DECLARE_MESSAGE_MAP()
//public:
//	int buffer[246];
//	CObArray m_Array; // массив указателей атрибуты формы

};


class IMPORT_1C CDocForm:public CGetDoc7 //CObject
{
DECLARE_DYNCREATE(CDocForm)

protected:
	 CDocForm(void);	//12
	virtual  ~CDocForm(void);	//66
	virtual int  BufferExchange(int);	//171
public:
	virtual int  DataExchange(class CGetExchange *);	//220
	virtual void  DescribeToolBar(class CWnd *,class CToolBar7 *,unsigned int const *,int);	//228
	int  DoWriteDoc(int,int,int,class CValue *,class CString *);	//251
	int  GetChangeLineOrder(void)const;	//288
protected:
	virtual struct HMENU__ *  GetDefaultMenu(void);	//311
	virtual class CDate *  GetPDate(void);	//405
	class CDocSkipper *  GetSkipper(void);	//458
public:
	void  InitTransacter(void);	//520
	virtual int  LayoutForm(class CFormView *);	//549
protected:
	virtual void  OnActivateCtrl(unsigned int);	//572
	void  OnBrChangePos(void);	//581
	void  OnBrowseAppendStart(void);	//583
	void  OnBrowseScan(void);	//584
	void  OnBrowseSetFocus(void);	//585
	virtual void  OnButton(unsigned int);	//587
	void  OnCancel(void);	//593
	virtual int  OnCancelEdit(void);	//598
	void  OnChildDocs(void);	//607
	void  OnCopy(void);	//620
	void  OnCopyOtherKind(void);	//621
	virtual void  OnCopyRec(long);	//622
	virtual int  OnDeleteRec(void);	//635
	void  OnEdit(void);	//640
public:
	virtual void  OnEndBatch(int);	//644
	virtual void  OnEndRunModule(class CGetField *);	//647
protected:
	void  OnEnter(void);	//650
public:
	virtual void  OnEnyChanged(unsigned int,int);	//652
protected:
	virtual int  OnGetUse(unsigned int,unsigned int,class CValue *);	//657
	void  OnGotoOper(void);	//659
	void  OnGotoProv(void);	//660
	void  OnInsertRec(void);	//679
	virtual unsigned int  OnMakeUse(unsigned int);	//682
	void  OnMoveLineDown(void);	//688
	void  OnMoveLineUp(void);	//689
	void  OnNewCommand(void);	//690
	virtual int  OnNewDocument(void);	//691
	virtual void  OnNewRec(void);	//694
	void  OnNotify(class CMessage7 *,long *);	//695
	void  OnOpenActs(void);	//712
public:
	virtual int  OnOpenDocument(char const *);	//714
protected:
	void  OnOpenOper(void);	//717
	void  OnOpenProvs(void);	//718
	virtual void  OnRead(int);	//725
	void  OnRenumber(void);	//726
	void  OnSortDescLines(void);	//747
	void  OnSortLines(void);	//748
public:
	virtual void  OnStartBatch(void);	//750
	virtual void  OnStartRunModule(class CGetField *);	//755
protected:
	virtual int  OnStoreRec(void);	//758
	void  OnUpdateCmdButtons(class CCmdUI *);	//769
	void  OnWrite(void);	//781
public:
	virtual int  PreCloseDoc(int);	//802
	virtual void  ReallyRefresh(void);	//813
	void  SetChangeLineOrder(int);	//848
	void  SetChanges(class CGetField *);	//849
	virtual void  SetDocTitle(void);	//869
protected:
	void  SortLines(int);	//954
	void  SpecNewRec(void);	//955
public:
	virtual void  UpdateWatchs(int,int,int,class CGetField *);	//986
protected:
	virtual void  ValidateMenu(class CMenu *);	//990

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CDocListPos
{
public:
	 CDocListPos(class CDocListPos const &);	//13
	 CDocListPos(void);	//14
	 ~CDocListPos(void);	//67
	class CDocListPos &  operator=(class CDocListPos const &);	//100
};


class IMPORT_1C CDocSetSelectDlg:public CDialog
{
public:
	 CDocSetSelectDlg(class CWnd *);	//15
	virtual  ~CDocSetSelectDlg(void);	//68
//	void  `default constructor closure'(void);	//146
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//238
	void  EnableCtrls(void);	//256
public:
	class CSelState *  GetState(long);	//460
protected:
	virtual void  InitDialog(int &,class CString);	//501
	void  OnAutoScan(void);	//575
	void  OnButtonClickedValue(void);	//589
	void  OnButtonClickedValueBeg(void);	//591
	void  OnButtonClickedValueEnd(void);	//592
	virtual void  OnCancel(void);	//594
	void  OnClearSelect(void);	//610
	void  OnDblclkRekvList(void);	//631
	void  OnDblclkValueList(void);	//633
	virtual int  OnInitDialog(void);	//667
	virtual void  OnOK(void);	//699
	void  OnOkValue(void);	//708
	void  OnOnlyInJournalRange(void);	//711
	virtual void  OnSelSelectType(void);	//729
	void  OnSelchangeRekvList(void);	//732
	void  OnStartFind(void);	//753
	void  OnUseRange(void);	//776
	void  OnUseValue(void);	//777
public:
	void  SetEnv(long,class CValue,long *,class CValue *,class CDate,class CDate,class CString);	//878
protected:
	virtual int  StartFind(void);	//956
public:
	static class CSelStateArray  m_StateArray;	//1070

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CDocTransacter
{
public:
	 CDocTransacter(class CDocSet *,class COperSet *);	//16
	 ~CDocTransacter(void);	//69
//	void  `default constructor closure'(void);	//147
	int  ClearDocPosting(int);	//198
	int  DeleteDocActs(void);	//224
	int  DoMakeTransactions(int,class CValue *,int);	//249
	class CDocActModule *  GetAt(int);	//283
	static struct SDocPostState *  GetCurrentPostState(void);	//303
	int  GetLastPostingType(void);	//360
	int  Init(int);	//491
	int  InitAll(void);	//495
	void  InitContexts(void);	//499
private:
	int  MakeActionsDocAddToStream(int);	//560
	int  MakeActionsDocBeforeTM(int);	//561
public:
	int  MakePosting(void);	//562
	int  MakeUnPosting(void);	//563
private:
	void  Message(class CString);	//566
public:
	void  SetDocFormID(unsigned int);	//864
	void  SetDocOwnerID(unsigned int);	//867
	void  SetFlagNewDoc(int);	//885
	void  SetInteractive(int);	//899
	void  SetPostingInStream(int);	//913
	void  SetRePostingInStream(int);	//921
	static int  flPosting;	//1068
	static class CObjID  sCurrentDocIDForCalc;	//1091
private:
	static struct SDocPostState *  s_pCurrentPostState;	//1093
};


class IMPORT_1C CFormContext:public CBLContext
{
DECLARE_DYNCREATE(CFormContext)

public:
	 CFormContext(void);	//17
	virtual  ~CFormContext(void);	//70
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//178
	virtual int  CallAsProc(int,class CValue * *);	//187
	int  GetEnableActivateField(void);	//323
	virtual int  GetNProps(void)const;	//400
	virtual char const *  GetPropName(int,int)const;	//423
	virtual int  GetPropVal(int,class CValue &)const;	//429
	class CValueListContext *  GetTabCtrlListContext(void);	//466
	int  InitExecProcedure(void);	//503
	virtual int  IsPropWritable(int)const;	//538
	void  OnInitForm(void);	//677
	int  SetEnableActivateField(int);	//871
	void  SetFlagsRefreshContext(int);	//888
	virtual void  SetForm(class CGetDoc7 *);	//890
	virtual int  SetPropVal(int,class CValue const &);	//918
protected:
	virtual int  TranPropPos(int &)const;	//967
};


class IMPORT_1C CGenericTAControl
{
public:
	 CGenericTAControl(class CGenericTAControl const &);	//18
	 CGenericTAControl(void);	//19
	virtual  ~CGenericTAControl(void);	//71
	class CGenericTAControl &  operator=(class CGenericTAControl const &);	//101
	virtual int  ChangeTA(class CDate,class CEventTime,int,int *,int *,int,int *);	//192
	static class CGenericTAControl *  GetTAControl(void);	//463
	static void  SetTAControl(class CGenericTAControl *);	//935
private:
	static class CGenericTAControl *  s_pTAControl;	//1094
};


class /*IMPORT_1C*/ CGetDlg:public CDialog //CObject
{
DECLARE_DYNAMIC(CGetDlg)

public:
	DWORD bufGetDlg[0xFFF];
	virtual  ~CGetDlg(void);	//72
/*
.rdata:2A0F4C44                 dd offset sub_2A06A7C0
.rdata:2A0F4C48                 dd offset nullsub_15
.rdata:2A0F4C4C                 dd offset nullsub_16
.rdata:2A0F4C50                 dd offset nullsub_17
*/
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//617
/*
.rdata:2A0F4C54                 dd offset ?OnCmdMsg@CGetDlg@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z ; CGetDlg::OnCmdMsg(uint,int,void *,AFX_CMDHANDLERINFO *)
.rdata:2A0F4C58                 dd offset ?OnFinalRelease@CWnd@@UAEXXZ ; CWnd::OnFinalRelease(void)
.rdata:2A0F4C5C                 dd offset ?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z ; CCmdTarget::IsInvokeAllowed(long)
.rdata:2A0F4C60                 dd offset ?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z ; CCmdTarget::GetDispatchIID(_GUID *)
.rdata:2A0F4C64                 dd offset ?GetTypeInfoCount@CCmdTarget@@UAEIXZ ; CCmdTarget::GetTypeInfoCount(void)
.rdata:2A0F4C68                 dd offset ?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ ; CCmdTarget::GetTypeLibCache(void)
.rdata:2A0F4C6C                 dd offset ?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z ; CCmdTarget::GetTypeLib(ulong,ITypeLib * *)
.rdata:2A0F4C70                 dd offset ?GetMessageMap@CGetDlg@@MBEPBUAFX_MSGMAP@@XZ ; CGetDlg::GetMessageMap(void)
.rdata:2A0F4C74                 dd offset ?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ ; CCmdTarget::GetCommandMap(void)
.rdata:2A0F4C78                 dd offset ?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ ; CCmdTarget::GetDispatchMap(void)
.rdata:2A0F4C7C                 dd offset ?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ ; CCmdTarget::GetConnectionMap(void)
.rdata:2A0F4C80                 dd offset ?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ ; CCmdTarget::GetInterfaceMap(void)
.rdata:2A0F4C84                 dd offset ?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ ; CCmdTarget::GetEventSinkMap(void)
.rdata:2A0F4C88                 dd offset ?OnCreateAggregates@CCmdTarget@@UAEHXZ ; CCmdTarget::OnCreateAggregates(void)
.rdata:2A0F4C8C                 dd offset ?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z ; CCmdTarget::GetInterfaceHook(void const *)
.rdata:2A0F4C90                 dd offset ?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z ; CCmdTarget::GetExtraConnectionPoints(CPtrArray *)
.rdata:2A0F4C94                 dd offset ?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z ; CCmdTarget::GetConnectionHook(_GUID const &)
.rdata:2A0F4C98                 dd offset ?PreSubclassWindow@CWnd@@UAEXXZ ; CWnd::PreSubclassWindow(void)
.rdata:2A0F4C9C                 dd offset ?Create@CWnd@@UAEHPBD0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z ; CWnd::Create(char const *,char const *,ulong,tagRECT const &,CWnd *,uint,CCreateContext *)
.rdata:2A0F4CA0                 dd offset ?DestroyWindow@CWnd@@UAEHXZ ; CWnd::DestroyWindow(void)
.rdata:2A0F4CA4                 dd offset ?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTA@@@Z ; CWnd::PreCreateWindow(tagCREATESTRUCTA &)
.rdata:2A0F4CA8                 dd offset ?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z ; CWnd::CalcWindowRect(tagRECT *,uint)
.rdata:2A0F4CAC                 dd offset ?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOA@@@Z ; CWnd::OnToolHitTest(CPoint,tagTOOLINFOA *)
.rdata:2A0F4CB0                 dd offset ?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z ; CWnd::GetScrollBarCtrl(int)
.rdata:2A0F4CB4                 dd offset ?WinHelpA@CWnd@@UAEXKI@Z ; CWnd::WinHelpA(ulong,uint)
.rdata:2A0F4CB8                 dd offset ?ContinueModal@CWnd@@UAEHXZ ; CWnd::ContinueModal(void)
.rdata:2A0F4CBC                 dd offset ?EndModalLoop@CWnd@@UAEXH@Z ; CWnd::EndModalLoop(int)
.rdata:2A0F4CC0                 dd offset ?OnCommand@CWnd@@MAEHIJ@Z ; CWnd::OnCommand(uint,long)
.rdata:2A0F4CC4                 dd offset ?OnNotify@CWnd@@MAEHIJPAJ@Z ; CWnd::OnNotify(uint,long,long *)
.rdata:2A0F4CC8                 dd offset ?GetSuperWndProcAddr@CWnd@@MAEPAP6GJPAUHWND__@@IIJ@ZXZ ; CWnd::GetSuperWndProcAddr(void)
*/
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//239
/*
.rdata:2A0F4CCC                 dd offset ?DoDataExchange@CGetDlg@@MAEXPAVCDataExchange@@@Z ; CGetDlg::DoDataExchange(CDataExchange *)
.rdata:2A0F4CD0                 dd offset sub_2A002700
.rdata:2A0F4CD4                 dd offset sub_2A002710
*/
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//805
/*
.rdata:2A0F4CD8                 dd offset ?PreTranslateMessage@CGetDlg@@UAEHPAUtagMSG@@@Z ; CGetDlg::PreTranslateMessage(tagMSG *)
.rdata:2A0F4CDC                 dd offset ?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z ; CWnd::OnAmbientProperty(COleControlSite *,long,tagVARIANT *)
.rdata:2A0F4CE0                 dd offset ?WindowProc@CWnd@@MAEJIIJ@Z ; CWnd::WindowProc(uint,uint,long)
.rdata:2A0F4CE4                 dd offset ?OnWndMsg@CWnd@@MAEHIIJPAJ@Z ; CWnd::OnWndMsg(uint,uint,long,long *)
.rdata:2A0F4CE8                 dd offset ?DefWindowProcA@CWnd@@MAEJIIJ@Z ; CWnd::DefWindowProcA(uint,uint,long)
.rdata:2A0F4CEC                 dd offset ?PostNcDestroy@CWnd@@MAEXXZ ; CWnd::PostNcDestroy(void)
.rdata:2A0F4CF0                 dd offset ?OnChildNotify@CWnd@@MAEHIIJPAJ@Z ; CWnd::OnChildNotify(uint,uint,long,long *)
.rdata:2A0F4CF4                 dd offset ?CheckAutoCenter@CDialog@@UAEHXZ ; CDialog::CheckAutoCenter(void)
.rdata:2A0F4CF8                 dd offset ?IsFrameWnd@CWnd@@UBEHXZ ; CWnd::IsFrameWnd(void)
.rdata:2A0F4CFC                 dd offset ?SetOccDialogInfo@CDialog@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z ; CDialog::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO *)
.rdata:2A0F4D00                 dd offset ?DoModal@CDialog@@UAEHXZ ; CDialog::DoModal(void)
*/
protected:
	virtual int  OnInitDialog(void);	//668
/*
.rdata:2A0F4D04                 dd offset ?OnInitDialog@CGetDlg@@MAEHXZ ; CGetDlg::OnInitDialog(void)
.rdata:2A0F4D08                 dd offset ?OnSetFont@CDialog@@UAEXPAVCFont@@@Z ; CDialog::OnSetFont(CFont *)
.rdata:2A0F4D0C                 dd offset ?OnOK@CDialog@@MAEXXZ ; CDialog::OnOK(void)
.rdata:2A0F4D10                 dd offset ?OnCancel@CDialog@@MAEXXZ ; CDialog::OnCancel(void)
.rdata:2A0F4D14                 dd offset ?PreInitDialog@CDialog@@MAEXXZ ; CDialog::PreInitDialog(void)
*/
public:
	 CGetDlg(unsigned int,class CWnd *);	//20
//	virtual  ~CGetDlg(void);	//72
//	void  `default constructor closure'(void);	//148
protected:
//	virtual void  DoDataExchange(class CDataExchange *);	//239
public:
	void  FlipToolBar(int);	//275
	static long  GetLParam(void);	//359
	class CTabCtrl *  GetTabControl(void);	//464
	static class CGetDoc7 *  GetTopmostDocunent(void);	//469
	int  IsTabCtrlVisible(void);	//545
protected:
	void  OnClose(void);	//612
public:
//	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//617
protected:
	void  OnDestroy(void);	//639
//	virtual int  OnInitDialog(void);	//668
	long  OnKickIdle(unsigned int,long);	//681
	void  OnSetFocus(class CWnd *);	//738
	void  OnSize(unsigned int,int,int);	//745
	void  OnTabPageBack(void);	//759
	void  OnTabPageForward(void);	//762
public:
	static void  OpenDocumentFileEx(unsigned int,char const *,long,class CValue *,unsigned int,unsigned int,unsigned int,unsigned int);	//782
//	virtual int  PreTranslateMessage(struct tagMSG *);	//805
	void  SetTabCtrlVisible(int);	//937
	static void  SetWrap(void);
	LRESULT myWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	static long  m_lParam;	//1072

	DECLARE_MESSAGE_MAP()
};

/*
class CGetDlg:public CDialog //CObject
{
DECLARE_DYNAMIC(CGetDlg)

public:
	 CGetDlg(unsigned int,class CWnd *);	//20
	virtual  ~CGetDlg(void);	//72
//	void  `default constructor closure'(void);	//148
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//239
public:
	void  FlipToolBar(int);	//275
	static long  GetLParam(void);	//359
	class CTabCtrl *  GetTabControl(void);	//464
	static class CGetDoc7 *  GetTopmostDocunent(void);	//469
	int  IsTabCtrlVisible(void);	//545
protected:
	void  OnClose(void);	//612
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//617
protected:
	void  OnDestroy(void);	//639
	virtual int  OnInitDialog(void);	//668
	long  OnKickIdle(unsigned int,long);	//681
	void  OnSetFocus(class CWnd *);	//738
	void  OnSize(unsigned int,int,int);	//745
	void  OnTabPageBack(void);	//759
	void  OnTabPageForward(void);	//762
public:
	static void  OpenDocumentFileEx(unsigned int,char const *,long,class CValue *,unsigned int,unsigned int,unsigned int,unsigned int);	//782
	virtual int  PreTranslateMessage(struct tagMSG *);	//805
	void  SetTabCtrlVisible(int);	//937
protected:
	static long  m_lParam;	//1072

	DECLARE_MESSAGE_MAP()
};
*/


class IMPORT_1C CGetField: public CObject
{
public:
	int buf[0x0A8];
	 CGetField(void);	//22
	virtual  ~CGetField(void);	//74
	int  AvtoActivate(void);	//168
	int  CheckLinkValue(class CString,int);	//194
	int  Clear(void);	//197
	class CBLContext *  GetBLContext(void);	//285
	unsigned int  GetCtrlID(void);	//294
	class CControlID *  GetCtrlInfo(void);	//296
	class CType   GetCtrlType(void)const;	//298
	class CDate   GetDate(void);	//305
	class CMetaDataTypedObj *  GetDef(void);	//308
	class CGetDoc7 *  GetDoc(void);	//314
	int  GetEnableChoiseFolder(void);	//325
	int  GetFixKind(void);	//335
	int  GetFlagSkipping(void);	//342
	int  GetInTable(void);	//355
	class CDate *  GetPDate(void);	//407
	class CTypedValue *  GetPTypedValue(void);	//409
	class CValue *  GetPValue(void);	//411
	int  GetParamNo(void);	//418
	int  GetQuickChoice(void)const;	//430
	virtual int  GetReadOnly(void);	//434
	int  GetUse(unsigned int,class CValue *);	//475
	int  IsBrOldStateEnable(void);	//525
	int  IsHideStateEnable(void);	//532
	int  IsStatic(void);	//544
	int  MakeUse(class CValue *,class CValue *);	//564
	void  ReCreateValue(class CType const &);	//810
	void  SetBrOldStateEnable(int);	//844
	void  SetCellReadOnly(int);	//847
	void  SetCtrlID(unsigned int);	//854
	void  SetCtrlInfo(class CControlID *);	//855
	void  SetCtrlType(class CType &);	//857
	void  SetDef(class CMetaDataTypedObj *);	//860
	void  SetDoc(class CGetDoc7 *);	//863
	void  SetEnableChoiseFolder(int);	//873
	void  SetFixKind(int);	//880
	void  SetFlagSkipping(int);	//886
	void  SetHideStateEnable(int);	//895
	void  SetInTable(int);	//898
	void  SetInternalValue(class CType const &);	//900
	void  SetPDate(class CDate *);	//906
	void  SetPValue(class CValue *);	//907
	void  SetParamNo(int);	//908
	void  SetQuickChoice(int);	//920
	void  SetReadOnly(int);	//924
	void  SetSkipMessageNotFindAcc(int);	//928
	int  TestCanClear(void);	//962
	int  TranslateType(void);	//968
	int  UpdateMask(void);	//983
	void  UpdateState(void);	//984
	int  WhantCheckLinkValue(void);	//992
};


class IMPORT_1C CGetFieldsArray
{
public:
	 CGetFieldsArray(void);	//23
	 ~CGetFieldsArray(void);	//75
	class CGetField *  operator[](int)const;	//107
	int  AddItem(class CGetField *);	//166
	class CGetField *  GetAt(int)const;	//284
	class CGetField *  GetFieldByCtrl(unsigned int);	//330
	int  GetSize(void)const;	//456
	class CGetField *  NewItem(void);	//569
	void  RemoveAt(int);	//827
};


class IMPORT_1C CGroupColumn
{
public:
	 CGroupColumn(void);	//24
	 ~CGroupColumn(void);	//76
	class CGroupColumn &  operator=(class CGroupColumn const &);	//102
	class CTypedValue *  Connect(class CDocSet *);	//199
	int  GetAlign(void)const;	//281
	unsigned int  GetCtrlID(void)const;	//295
	class CTypedValue *  GetPValue(void)const;	//412
	void  Init(class CMetaDataRefObj *,unsigned int);	//492
};


class IMPORT_1C CGroupColumns
{
public:
	 CGroupColumns(void);	//25
	virtual  ~CGroupColumns(void);	//77
	void  Init(class CJournalDef *);	//493
};


class IMPORT_1C CInputSbKindDlg
{
public:
	 CInputSbKindDlg(class CWnd *);	//26
	virtual  ~CInputSbKindDlg(void);	//78
//	void  `default constructor closure'(void);	//149
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//240
	virtual int  OnInitDialog(void);	//669
	virtual void  OnOK(void);	//700
public:
	void  SetStart(class CObjID,long,class CString);	//930

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CJournalOptDlg: public CDialog
{
public:
	 CJournalOptDlg(class CWnd *);	//27
	virtual  ~CJournalOptDlg(void);	//79
//	void  `default constructor closure'(void);	//150
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//241
	void  OnBegdateButton(void);	//578
	void  OnEnddateButton(void);	//649
	virtual int  OnInitDialog(void);	//670
	virtual void  OnOK(void);	//701

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CJournalSelDlg
{
public:
	 CJournalSelDlg(class CWnd *);	//28
	virtual  ~CJournalSelDlg(void);	//80
//	void  `default constructor closure'(void);	//151
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//242
	virtual int  OnInitDialog(void);	//671
	virtual void  OnOK(void);	//702
public:
	void  SetStartType(long);	//934

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CPictureColumn:public CObject
{
DECLARE_DYNAMIC(CPictureColumn)

public:
	 CPictureColumn(class CGetDoc7 *,unsigned int,char const *,int);	//29
	virtual  ~CPictureColumn(void);	//81
	virtual int  DrawItem(class CDC &,class CRect &,class CString &,int);	//253
};


class IMPORT_1C CPlanKindContext:public CObject
{
DECLARE_DYNCREATE(CPlanKindContext)

public:
	 CPlanKindContext(void);	//30
	virtual  ~CPlanKindContext(void);	//82
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//179
	virtual char const *  GetCode(void)const;	//291
	virtual class CObjID   GetID(void)const;	//351
	virtual char const *  GetMethodName(int,int)const;	//384
	virtual int  GetNMethods(void)const;	//390
	virtual int  GetNParams(int)const;	//395
	virtual class CType   GetValueType(void)const;	//483
	virtual int  HasRetVal(int)const;	//488
	virtual void  InitObject(class CType const &);	//510
	virtual void  InitObject(char const *);	//511
	static void  Register(void);	//824
	virtual void  SelectByID(class CObjID,long);	//837
	static void  UnRegister(void);	//976
};


class IMPORT_1C CPlanSelDlg
{
public:
	 CPlanSelDlg(class CWnd *);	//31
	virtual  ~CPlanSelDlg(void);	//83
//	void  `default constructor closure'(void);	//152
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//243
	virtual int  OnInitDialog(void);	//672
	virtual void  OnOK(void);	//703
public:
	void  SetStart(long,int,class CString);	//931

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CRefIntegDlg
{
public:
	 CRefIntegDlg(class CRefIntegrity *);	//32
	virtual  ~CRefIntegDlg(void);	//84
	virtual int  DestroyWindow(void);	//230
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//244
	virtual void  OnCancel(void);	//596
	void  OnCheckRef(void);	//606
	void  OnChkChangedDelDoc(void);	//608
	void  OnDblclkDelDocList(struct tagNMHDR *,long *);	//627
	void  OnDblclkRefdocList(struct tagNMHDR *,long *);	//630
	void  OnGetMinMaxInfo(struct tagMINMAXINFO *);	//656
	virtual int  OnInitDialog(void);	//673
	void  OnItemchangedDeldocList(struct tagNMHDR *,long *);	//680
	virtual void  OnOK(void);	//704
	void  OnOpenDel(void);	//713
	void  OnOpenRef(void);	//719
	void  OnSelectAll(void);	//735
	void  OnShowDelCheck(void);	//741
	void  OnShowNondelCheck(void);	//742
	void  OnSize(unsigned int,int,int);	//746
	int  OnToolTipNotify(unsigned int,struct tagNMHDR *,long *);	//765
	void  OnUnSelectAll(void);	//767
	virtual void  PostNcDestroy(void);	//801

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CRepHistory
{
public:
	 CRepHistory(long,class CGetDoc7 *);	//33
	 CRepHistory(void);	//34
	 ~CRepHistory(void);	//85
	int  EditName(void);	//254
protected:
	virtual class CString   GetFileName(void);	//333
public:
	int  Init(long,class CGetDoc7 *);	//494
	int  Load(void);	//552
	int  Read(void);	//812
	int  Save(void);	//828
	int  SelectName(void);	//839
	int  Write(void);	//993
};


class IMPORT_1C CSbCntSetSelectDlg
{
public:
	 CSbCntSetSelectDlg(class CWnd *);	//35
	virtual  ~CSbCntSetSelectDlg(void);	//86
//	void  `default constructor closure'(void);	//153
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//245
	void  EnableCtrls(void);	//257
public:
	class CSelState *  GetState(long);	//461
protected:
	virtual void  InitDialog(int &,class CString);	//502
	void  OnAutoScan(void);	//576
	void  OnButtonClickedValue(void);	//590
	virtual void  OnCancel(void);	//597
	void  OnClearSelect(void);	//611
	void  OnDblclkRekvList(void);	//632
	void  OnDblclkValueList(void);	//634
	void  OnInParent(void);	//664
	virtual int  OnInitDialog(void);	//674
	virtual void  OnOK(void);	//705
	void  OnOkValue(void);	//709
	virtual void  OnSelSelectType(void);	//730
	void  OnSelchangeRekvList(void);	//733
	void  OnStartFind(void);	//754
public:
	void  SetEnv(class CSbCntSet *,long,class CValue,long *,class CValue *,class CString);	//879
protected:
	virtual int  StartFind(void);	//957
public:
	static class CSelStateArray  m_StateArray;	//1071

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSbKindContext:public CBLContext
{
DECLARE_DYNCREATE(CSbKindContext)

public:
	 CSbKindContext(void);	//36
	virtual  ~CSbKindContext(void);	//87
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//180
	virtual char const *  GetCode(void)const;	//292
	virtual class CObjID   GetID(void)const;	//352
	virtual char const *  GetMethodName(int,int)const;	//385
	virtual int  GetNMethods(void)const;	//391
	virtual int  GetNParams(int)const;	//396
	class CObjID   GetUseAccount(void);	//476
	virtual class CType   GetValueType(void)const;	//484
	virtual int  HasRetVal(int)const;	//489
	virtual void  InitObject(class CType const &);	//512
	virtual void  InitObject(char const *);	//513
	static void  Register(void);	//825
	virtual void  SelectByID(class CObjID,long);	//838
	void  SetUseAccount(class CObjID);	//946
	static void  UnRegister(void);	//977
};


class IMPORT_1C CSelHistDlg
{
public:
	 CSelHistDlg(class PSelHistItemArray *,class CWnd *);	//37
	virtual  ~CSelHistDlg(void);	//88
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//246
public:
	int  GetSelIdx(void);	//454
protected:
	virtual int  OnInitDialog(void);	//675
	virtual void  OnOK(void);	//706

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSelHistHolder
{
public:
	 CSelHistHolder(int,class CProfile7 *);	//38
	 CSelHistHolder(class CString);	//39
	 ~CSelHistHolder(void);	//89
	void  Add(long,class CValue &,class CString);	//165
	class CSelHistItem *  Choise(void);	//196
	void  DeleteAt(int);	//222
	int  GetSize(void);	//457
	void  LoadFromString(char const *);	//557
	void  SaveToString(class CString &);	//832
};


class IMPORT_1C CSelHistItem
{
public:
	 CSelHistItem(class CSelHistItem const &);	//40
	 CSelHistItem(void);	//41
	 ~CSelHistItem(void);	//90
	class CSelHistItem &  operator=(class CSelHistItem const &);	//103
};


class IMPORT_1C CSelState
{
public:
	 CSelState(class CSelState const &);	//42
	 CSelState(long,class CType);	//43
	 ~CSelState(void);	//91
	class CSelState &  operator=(class CSelState const &);	//104
//	void  `default constructor closure'(void);	//154
	void  SetType(class CType,int,int,int);	//944
};


class IMPORT_1C CSelStateArray
{
public:
	 CSelStateArray(void);	//44
	 ~CSelStateArray(void);	//92
	class CSelState *  GetState(long);	//462
};


class IMPORT_1C CSetDateRangeDlg:public CObject
{
DECLARE_DYNCREATE(CSetDateRangeDlg)

public:
	 CSetDateRangeDlg(class CDate &,class CDate &,int,class CWnd *);	//45
	 CSetDateRangeDlg(class CWnd *);	//46
	virtual  ~CSetDateRangeDlg(void);	//93
//	void  `default constructor closure'(void);	//155
protected:
	void  CalcRealDate(void);	//172
	void  ChangeRange(int);	//191
	virtual void  DoDataExchange(class CDataExchange *);	//247
public:
	void  GetRange(class CDate &,class CDate &);	//431
	void  InitRange(class CDate &,class CDate &);	//517
protected:
	int  MonthChange(void);	//568
	void  OnBegDateButton(void);	//577
	void  OnChangeData(void);	//599
	void  OnChangeRange(void);	//602
	void  OnCheckBegMonth(void);	//603
	void  OnCheckBegQuart(void);	//604
	void  OnCheckBegYear(void);	//605
	void  OnDeltaposSpinMr(struct tagNMHDR *,long *);	//636
	void  OnDeltaposSpinQr(struct tagNMHDR *,long *);	//637
	void  OnEndDateButton(void);	//646
	virtual int  OnInitDialog(void);	//676
	void  OnMonthChange(void);	//687
	virtual void  OnOK(void);	//707
	void  OnQuartChange(void);	//724
	void  OnSingleDateButton(void);	//743
	int  QuartChange(void);	//809
public:
	void  SetCheckLevel(int);	//850
protected:
	void  SetMonthPresent(void);	//903
	void  SetQuartPresent(void);	//919
	void  SetRealPresent(void);	//925
public:
	void  SetTitle(class CString const &);	//941

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTabFrame:public CChildFrame7//CObject
{
DECLARE_DYNCREATE(CTabFrame)
public:
	DWORD buff[0x2E]; // (338h - 280h) / 4
protected:
	 CTabFrame(void);	//47
	virtual  ~CTabFrame(void);	//94
public:
	virtual class CDocument *  GetActiveDocument(void);	//279
	class CTabCtrl *  GetTabControl(void);	//465
	int  IsTabCtrlVisible(void);	//546
protected:
	void  OnBookToPageBack(void);	//579
	void  OnBookToPageForward(void);	//580
public:
	virtual int  OnCmdMsg(unsigned int,int,void *,struct AFX_CMDHANDLERINFO *);	//618
protected:
	int  OnCreate(struct tagCREATESTRUCTA *);	//624
	virtual int  OnCreateClient(struct tagCREATESTRUCTA *,struct CCreateContext *);	//625
	long  OnIdleUpdateCmdUI(unsigned int,long);	//663
	void  OnUpdateBookToPage(class CCmdUI *);	//768
public:
	virtual void  RecalcLayout(int);	//815
	void  SetActiveDocument(class CDocument *);	//843
	void  SetTabCtrlVisible(int);	//939

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CUserHelpTitle:public CObject
{
DECLARE_DYNAMIC(CUserHelpTitle)

public:
	 CUserHelpTitle(void);	//48
	virtual  ~CUserHelpTitle(void);	//95
	virtual int  Create(unsigned long,class CRect const &,class CWnd *);	//202
protected:
	void  OnPaint(void);	//721
	long  OnSetFontToHider(unsigned int,long);	//739
	long  OnSetText(unsigned int,long);	//740
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//804
public:
	void  SetDark(int,int);	//859
	void  SetImage(int,int);	//896
	void  SetImageList(unsigned int,int);	//897
	void  SetTitle(class CString const &,int);	//942

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CUsrHelpDoc:public CObject
{
DECLARE_DYNCREATE(CUsrHelpDoc)

protected:
	 CUsrHelpDoc(void);	//49
public:
	virtual  ~CUsrHelpDoc(void);	//96
	virtual int  DoFileSave(void);	//248
	static unsigned int  DocID;	//252
	class CCycleStack *  GetStack(void);	//459
	static int  InitUserHelp(long,int);	//521
protected:
	void  OnFileClose(void);	//655
	virtual int  OnNewDocument(void);	//693
public:
	virtual int  OnOpenDocument(char const *);	//716
protected:
	void  OnUpdateFileClose(class CCmdUI *);	//771
	void  OnUpdateFileSave(class CCmdUI *);	//772
	void  OnUpdateFileSaveAs(class CCmdUI *);	//773
	void  OnUpdateModified(class CCmdUI *);	//774
public:
	virtual int  SaveModified(void);	//830
	int  SetHelpItem(long);	//893
	void  SetHelpTitle(char const *,class CWnd *);	//894
	virtual void  SetTitle(char const *);	//943
	long  ShowDocument(class CDocTemplate *,int,long);	//951

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CValueListContext:public CBLContext //CObject
{
DECLARE_DYNCREATE(CValueListContext)

public:
	char buf1[4];
	HWND m_hListWnd;
	char buf2[8];
	CPtrArray* m_pValueItemList;
	char buf3[12];
	
	 CValueListContext(void);	//50
	virtual  ~CValueListContext(void);	//97
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//181
	virtual int  CallAsProc(int,class CValue * *);	//188
protected:
	class CValue   F_Belong(class CValue * *);	//264
	class CValue   F_Check(class CValue * *);	//265
	class CValue   F_CheckValues(class CValue * *);	//266
	class CValue   F_ChoiseValue(class CValue * *);	//267
	class CValue   F_FindElement(class CValue * *);	//268
	class CValue   F_FromDelimStr(class CValue * *);	//269
	class CValue   F_Get(class CValue * *);	//270
	class CValue   F_GetByTitle(class CValue * *,class CValue &);	//271
	class CValue   F_GetSize(class CValue * *);	//272
	class CValue   F_ToDelimStr(class CValue * *);	//273
public:
	void  FromList(class CItemList const &);	//276
	int  GetCtrlSel(void);	//297
	int  GetCurPosAndValue(class CValue &,class CValue &);	//301
	virtual class CBLContextInternalData *  GetInternalData(void);	//356
	static char const *  GetListFunctor(void);	//361
	virtual char const *  GetMethodName(int,int)const;	//386
	virtual int  GetNMethods(void)const;	//392
	virtual int  GetNParams(int)const;	//397
	class CArray<class CValueItem *,class CValueItem *> *  GetPValueList(void);	//413
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//417
	virtual char const *  GetTypeString(void)const;	//474
	virtual int  HasRetVal(int)const;	//490
	virtual int  IsSerializable(void);	//543
	int  LoadFromList(class CItemList *,int);	//556
protected:
	void  P_Add(class CValue * *);	//783
	void  P_Insert(class CValue * *);	//786
	void  P_MoveValue(class CValue * *);	//788
	void  P_Remove(class CValue * *);	//790
	void  P_RemoveAll(class CValue * *);	//791
	void  P_Set(class CValue * *);	//794
	void  P_SetByTitle(class CValue * *);	//795
	void  P_Sort(class CValue * *);	//796
	void  P_SortByPresent(class CValue * *);	//797
	void  P_Unload(class CValue * *);	//798
public:
	void  RefreshCtrl(void);	//818
	static void  Register(void);	//826
	int  SaveToList(class CItemList *);	//831
	virtual int  SaveToString(class CString &);	//833
	void  SetCtrl(struct HWND__ *);	//852
	void  SetCtrl(class CWnd *);	//853
	int  SetCtrlSel(int);	//856
	void  SetUpdateChecks(int);	//945
	void  ToList(class CItemList &);	//965
	static void  UnRegister(void);	//978
	void  UpdateChecks(void);	//979

protected:
	class CValue   get_set_CurSel(class CValue * *);	//1069
};


class IMPORT_1C CVarHistory
{
public:
	 CVarHistory(void);	//51
	 ~CVarHistory(void);	//98
protected:
	virtual class CString   GetFileName(void);	//334
};

//this class not in export
class CPictureContext : public CBLContext
{
    DECLARE_DYNCREATE(CPictureContext);
public:
    CPictureHolder7 m_PictureHolder7;
};

// End class descriptions
// Begin free functions definitions
IMPORT_1C class CAccDoc * __cdecl ActivateAccList(long,long,unsigned int,unsigned int,class CObjID,class CDate,int,int,long,int,class CValue *,class CString,int,int,class CString,class CString,class CObjID,int,int,class CObjID const &,int,class CValue *);
IMPORT_1C class CGetDoc7 * __cdecl ActivateDocForm(long,class CObjID,long,unsigned int,class CObjID,long,int,long,int,int,int,int,class CValue *,class CValue *);
IMPORT_1C class CJournalDoc * __cdecl ActivateDocList(long,long,class CObjID,unsigned int,unsigned int,class CObjID,class CString,class CDate,int,int,int,class CValue *,class CString,int,long,long,int,int,class CString,class CValue *);
IMPORT_1C class CGetDoc7 * __cdecl ActivateObj(class CValue *);
IMPORT_1C int __cdecl ActivateRegActDoc(long,class CObjID,int,int);
IMPORT_1C class CReportForm * __cdecl ActivateReportForm(long,class CString,int,unsigned int,unsigned int,int,int,class CString,class CValue *,int *,class CValue *);
IMPORT_1C int __cdecl ActivateSubcActDoc(class CObjID,int,int);
IMPORT_1C class CSbCntDoc * __cdecl ActivateSubkList(long,class CObjID,unsigned int,unsigned int,class CObjID,class CString,class CDate,int,class CObjID,int,long,int,class CValue *,class CString,int,int,int,class CValue *);
IMPORT_1C int __cdecl BrowseScan(class CGetDoc7 *,unsigned int);
IMPORT_1C int __cdecl ChangeTimeDoc(class CDocSet *,long);
IMPORT_1C int __cdecl CheckOnDelete(class CObjID);
IMPORT_1C void __cdecl ConvertDate(class CString &,class CDate,class CDate);
IMPORT_1C int __cdecl DeleteDoc(class CDocSet *,long);
IMPORT_1C int __cdecl DeleteDocActsFromContext(class CDocSet *,class COperSet *);
IMPORT_1C int __cdecl DeleteDocFromContext(class CDocSet *,class COperSet *,int);
IMPORT_1C int __cdecl DeleteDocTransactions(class CDocSet *,long);
IMPORT_1C void __cdecl DlgDXValue(class CDialog *,class CDataExchange *,unsigned int,class CTypedValue *,class CString *);
IMPORT_1C void __cdecl DlgOnButtonClickedValue(class CDialog *,unsigned int,class CTypedValue *,class CString *);
IMPORT_1C void __cdecl DlgSetTypeCtrlValue(class CDialog *,unsigned int,class CTypedValue *);
IMPORT_1C int __cdecl EnableMDIActivate(void);
IMPORT_1C class CString  __cdecl GetAccMaskEdit(class CPlanDef *,int);
IMPORT_1C class CBasicProfile * __cdecl GetBasicProfile(void);
IMPORT_1C int __cdecl GetBrSearchBySort(void);
IMPORT_1C int __cdecl GetContextPropExt(class CBLContext *,class CValue &,class CValue * *);
IMPORT_1C class CString  __cdecl GetCurAccEditMask(long,int,int);
IMPORT_1C class CObjID  __cdecl GetCurrentDocObjID(void);
IMPORT_1C class CObjID  __cdecl GetCurrentSbCntObjID(long);
IMPORT_1C class CString  __cdecl GetDateFldDescr(void);
IMPORT_1C class CString  __cdecl GetDateFldMask(void);
IMPORT_1C class CPlanDef * __cdecl GetDefaultPlanDef(void);
IMPORT_1C int __cdecl GetDeleteMode(void);
IMPORT_1C class CObjID  __cdecl GetDocCurrentID(void);
IMPORT_1C int __cdecl GetFlagHardFinishSystem(void);
IMPORT_1C int __cdecl GetFlagScanOneCol(void);
IMPORT_1C class CString  __cdecl GetMDCodeFromValue(class CValue *);
IMPORT_1C class CMetaDataObj * __cdecl GetMDDefFromValue(class CValue *);
IMPORT_1C int __cdecl GetOpenMode(void);
IMPORT_1C class CObjID  __cdecl GetRefCurrentID(long);
IMPORT_1C class CType  __cdecl GetTypeFromValueExt(class CValue *,int,int,int *,int);
IMPORT_1C int __cdecl LoadValueFromString(class CValue *,char const *);
IMPORT_1C int __cdecl LockWithMessage(class CObjectSet *);
IMPORT_1C int __cdecl ModalInputTypedValue(class CTypedValue *,class CString,int);
IMPORT_1C int __cdecl ReMakeDocsTransactions(class CDate,class CEventTime,class CDate,class CEventTime,int *,int *,int,int,int &,int *);
IMPORT_1C int __cdecl SaveValueToString(class CValue *,class CString &);
IMPORT_1C int __cdecl SetContextPropExt(class CBLContext *,class CValue * *);
IMPORT_1C void __cdecl SetDefaultPlanDef(class CPlanDef *);
IMPORT_1C int __cdecl SetEnableMDIActivate(int);
IMPORT_1C int __cdecl SetFlagFinishSystem(int);
IMPORT_1C void __cdecl ShowObject(class CMetaDataObj *,class CObjID,long);
IMPORT_1C int __cdecl TestAccess(unsigned int,long,unsigned int,int,int);
IMPORT_1C int __cdecl WriteDocFromContext(class CDocSet *,class COperSet *,int,class CDocContext *);
IMPORT_1C int __cdecl WriteDocFromForm(class CDocSet *,class COperSet *,int,int,class CDocTransacter *,class CValue *,int);
// End free functions definitions
// ??_C@_02OMNH@VL?$AA@



