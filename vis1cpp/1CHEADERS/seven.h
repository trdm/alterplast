// Begin forward declarations
enum CAppRunMode
{
	armNONE = 0,
	armCONFIG,
	armDEBUG,
	armMONITOR,
	armENTERPRISE
	/*enum CAppRunMode{		// См. CApp7::StartNewProcess
	runEnterprise=-1,
	runConfig=1,
	runDebug=2,
	runMonitor=4
	*/
};

//class CAppSupport
//{
//private:
//	 CAppSupport(void);	//8
//public:
//	class CAppSupport &  operator=(class CAppSupport const &);	//211
//	static int  GetCommandState(class CFrameWnd *,unsigned int);	//685
//	static int  GetSubString(class CString &,char const *,int);	//1102
//	static int  GetSubStringCount(char const *);	//1103
//	static void  UpdateFloatMenu(class CWnd *,class CMenu *);	//1894
//};
class CModuleContext;
class CComponentClass;
typedef class CArray<CBLModule7*,CBLModule7*> CBLModuleArray;
enum PageType {ptModule, ptHelp, ptDialog, pt1, pt2, pt3};
//class CStubParamStruct
//{
//public:
//	enum STUB_ACTION {saOne};
//	 CStubParamStruct(enum CStubParamStruct::STUB_ACTION,long);	//93
//	class CStubParamStruct &  operator=(class CStubParamStruct const &);	//226
//};
//class std
//{
//public:
//	static class vector<class CLogDBCrypter *,class std::__alloc<1,0> >  CTableCSV::m_cryptReads;	//2165
//};
// End forward declarations
// Begin class descriptions

class IMPORT_1C CAboutDlg: public CDialog
{
public:
	DWORD buf[0x18C];
	 CAboutDlg(int);	//1
	virtual  ~CAboutDlg(void);	//116

	DECLARE_MESSAGE_MAP()

protected:
	virtual void  DoDataExchange(class CDataExchange *);	//546
	void  OnDestroy(void);	//1392
	virtual int  OnInitDialog(void);	//1434
};


class IMPORT_1C CAccContextDef:public CBLContext
{
DECLARE_DYNCREATE(CAccContextDef)

public:
	 CAccContextDef(void);	//2
	virtual  ~CAccContextDef(void);	//117
	virtual char const *  GetMethodName(int,int)const;	//830
	virtual int  GetNMethods(void)const;	//865
	virtual int  GetNParams(int)const;	//891
	virtual int  GetNProps(void)const;	//917
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//951
	virtual char const *  GetPropName(int,int)const;	//992
	virtual int  HasRetVal(int)const;	//1149
	virtual int  IsPropReadable(int)const;	//1237
	virtual int  IsPropWritable(int)const;	//1257
protected:
	int  PropPosToParamIdx(int)const;	//1635
};


class IMPORT_1C CAccFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CAccFormContextDef)

public:
	 CAccFormContextDef(void);	//3
	virtual  ~CAccFormContextDef(void);	//118
	virtual char const *  GetMethodName(int,int)const;	//831
	virtual int  GetNMethods(void)const;	//866
	virtual int  GetNParams(int)const;	//892
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//952
	virtual int  HasRetVal(int)const;	//1150
};


class IMPORT_1C CAccListFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CAccListFormContextDef)

public:
	 CAccListFormContextDef(void);	//4
	virtual  ~CAccListFormContextDef(void);	//119
	virtual char const *  GetMethodName(int,int)const;	//832
	virtual int  GetNMethods(void)const;	//867
	virtual int  GetNParams(int)const;	//893
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//953
	virtual int  HasRetVal(int)const;	//1151
};


class IMPORT_1C CAccountContextDef:public CBLContext
{
DECLARE_DYNCREATE(CAccountContextDef)

public:
	 CAccountContextDef(void);	//5
	virtual  ~CAccountContextDef(void);	//120
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//388
	virtual int  CallAsProc(int,class CValue * *);	//396
	virtual char const *  GetMethodName(int,int)const;	//833
	virtual int  GetNMethods(void)const;	//868
	virtual int  GetNParams(int)const;	//894
	virtual int  GetNProps(void)const;	//918
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//954
	virtual char const *  GetPropName(int,int)const;	//993
	virtual int  GetPropVal(int,class CValue &)const;	//1014
	virtual int  HasRetVal(int)const;	//1152
	virtual int  IsPropReadable(int)const;	//1238
};


class IMPORT_1C CAdminService
{
public:
	 CAdminService(class CString);	//6
	 ~CAdminService(void);	//121
protected:
	void  AddUserEvent(class CString,class CString);	//362
public:
	void  Archive(void);	//369
	void  CopyFilterInterval(class CLogFilter *,class CLogFilter *);	//427
	void  CreateCommentFilter(class CLogFilter *,class CString);	//444
	void  CreateDateFilter(class CLogFilter *,class CDate);	//445
	void  CreateDefaultFilter(class CLogFilter *);	//446
	void  CreateEventFilter(class CLogFilter *,class CString,class CString);	//448
	class CLogFetch *  CreateFetch(class CLogFilter *,char const *,int);	//449
	void  CreateNewWindow(char const *);	//455
	void  CreateNullFilter(class CLogFilter *);	//456
	void  CreateObjViewFilter(class CLogFilter *,class CString);	//457
	void  CreateObjectFilter(class CLogFilter *,class CValue *);	//510
	void  CreateRunmodeFilter(class CLogFilter *,class CString);	//513
protected:
	class CString   CreateTempLogDB(int,class CLogDB *,class CExpression *,class CDate,long *);	//514
public:
	void  CreateUserFilter(class CLogFilter *,class CString);	//515
	void  DisposeFetch(class CLogFetch *);	//543
protected:
	void  FillLink(void);	//634
	void  FillZipCatalog(void);	//638
public:
	char  GetLogDidvider(void);	//775
	void  GetLogParams(int &,class CString &);	//776
	class CLogDB *  GetMainLog(void);	//780
	class CLogModule *  GetModule(class CString);	//858
	class CLogModule *  GetModuleTerm(class CString);	//860
	int  IsLogging(void);	//1222
	int  IsRealFilter(class CLogFilter *);	//1273
protected:
	void  LoadLogParams(void);	//1297
public:
	int  LockExclusive(void);	//1304
	void  RecallArchive(void);	//1646
	int  Refresh(void);	//1648
	int  RegisterEventSourceA(class CLogModule *);	//1656
	int  ReportEventA(class CString EventCategory,class CString Event,enum EventType EvType,class CString comment,class CString object,class CString ObjDescr);	//1683
	int  ReportUserEvent(class CString,class CString,enum EventType,class CString,class CValue *,class CString);	//1684
protected:
	int  SaveLogParams(int,char const *);	//1693
public:
	void  SetExclMode(int);	//1760
	void  SetLogParams(int,char const *);	//1782
	void  ShowUserLog(class CString);	//1830
	long  StartLogging(void);	//1838
	int  StartSecondLog(class CString);	//1841
	long  StopLogging(void);	//1846
	void  StopSecondLog(void);	//1848
	int  UnlockExclusive(void);	//1883
	void  UpdateFetch(class CLogFetch *);	//1893
protected:
	void  UpdateLink(void);	//1898
	void  UploadUserMsgDB(void);	//1901
public:
	void  ViewAnyFile(class CString);	//1905
	void  _CreateFilter(class CLogFilter *,int);	//1919
};


class IMPORT_1C CApp7:public CAppFrame //CWinApp
{
public:
	DWORD buf[0x300];
public:
	 CApp7(void);	//7
	virtual  ~CApp7(void);	//122
	void  AdjustDirectories(void);	//367
	int  AttachAddInDLL(char const *);	//373
	void  BindAddInContext(char const *,class CAddInContext *);	//382
	int  CheckDirectories(void);	//406
	void  CloseDataBase(void);	//420
	void  DoPageSetupDialog(void);	//557
	int  FindSpecialFile(char const *,class CString &,char const *)const;	//652
	class CUserCommandContaner *  GetCommandManager(void);	//684

	virtual char const *  GetDDFName(void);	//705
	virtual char const *  GetDDSQLFName(void);	//706
	virtual char const *  GetSQLAddrFName(void);	//1088
	virtual char const *  GetMDFName(void);	//779
	virtual char const *  GetConfigFName(void);	//686
	virtual char const *  GetLockFName(void);	//773
	virtual char const *  GetDefSpellingFName(void);	//716

	virtual void  ActivateMessageDevice(int,int);	//338
	virtual void  ClearMessageDevice(void);	//413

	int  GetExitCode(void)const;	//727
	class CAdminService *  GetLog(void);	//774
	class CRect   GetMarginsRect(void);	//781
	class CMetaDataCont *  GetMetaData(void);	//829
	unsigned int  GetModeMask(void)const;	//857
	unsigned int  GetMouseScrollLines(int);	//861
	struct HFONT__ *  GetProfileFont(int);	//990
	void  GetProfileFont(int,struct tagLOGFONTA &)const;	//991
	class CProfile7 *  GetProps(void);	//1022
	class CString   GetRegisteredUserCompany(void);	//1030
	class CString   GetRegisteredUserName(void);	//1031
	class CRightsContainer *  GetRightsManager(void);	//1034
	unsigned int  GetToolBarPos(void)const;	//1122
	void  GetUserDefContainers(class CDocument * *,class CDocument * *);	//1132
	class CString   GetUserSystemCaption(void);	//1133
	void  InitAddInService(void);	//1184
	int  InitDataBase(void);	//1186
	void  InitParamsPages(void);	//1189
	int  InitProps(void);	//1190
	int  IsExclusiveMode(void);	//1216
	int  IsGrantedToRecentList(void);	//1219
	int  IsModeEnabled(unsigned int)const;	//1223
	int  LoadAddInDLL(char const *);	//1293
	int  LoadMetaData(char const *);	//1298
	virtual void  LockMessageDevice(int);	//1305
	void  OnDBClosing(void);	//1388
	void  OnDBOpened(void);	//1389
	virtual int  OnIdle(long);	//1433
	int  OpenDataBase(int,int,int);	//1602
	void  ProcessAddinEvents(void);	//1626
protected:
	virtual long  ProcessWndProcException(class CException *,struct tagMSG const *);	//1632
public:
	class CString   ReadStatusLine(void)const;	//1641
	void  Register_UserDefContainers(class CDocument *,class CDocument *);	//1657
	int  Register_zlibEngine(void);	//1658
	int  SaveProps(void);	//1709
	void  SetExclusiveMode(int);	//1761
	void  SetExitCode(int);	//1762
	void  SetGrantedToRecentList(int);	//1771
	void  SetProfileFont(int,struct tagLOGFONTA const &);	//1799
	void  SetProfileFont(int,struct HFONT__ *);	//1800
	void  SetUserSystemCaption(class CString,int);	//1825
	virtual void  ShowError(char const *,enum MessageMarker,char const *,long,int);	//1829
	int  StartNewProcess(enum CAppRunMode);	//1839
	void  StartTimer(void);	//1842
	void  StopTimer(void);	//1849
	void  TranslateCommandLine(char const *);	//1869
	void  UndoParamsPages(void);	//1880
	void  UnloadAddIns(void);	//1881
	void  Unregister_UserDefContainers(void);	//1884
	void  Unregister_zlibEngine(void);	//1885
	void  UpdateAppTitle(void);	//1891
	virtual void  WriteError(char const *,enum MessageMarker,char const *,long,int);	//1906
	virtual void  WriteMessageString(char const *,enum MessageMarker,char const *,long,int);	//1908
	virtual void  WriteMessageString(char const *,enum MessageMarker);	//1909
	void  WriteStatusLine(unsigned int);	//1912
	void  WriteStatusLine(char const *);	//1913
	struct IzlibEngine *  get_zlibEngine(void);	//2142
protected:
	static int  m_bAddToRecentGranted;	//2161
public:
	static unsigned int  m_uiExtCopyDataID;	//2172

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CAssistantDoc:public CDocument
{
DECLARE_DYNCREATE(CAssistantDoc)

protected:
	 CAssistantDoc(void);	//9
public:
	virtual  ~CAssistantDoc(void);	//123
protected:
	void  ActivateItem(int,int);	//336
	void  ActivateItem(char const *);	//337
	void  AddPermDctItem(char const *,char const *);	//354
public:
	void  AddWildCard(char const *,char const *);	//364
protected:
	void  BadStruct(class CItemList *,int);	//377
public:
	void  ClearHistory(void);	//412
	void *  CopyItem(struct _TREEITEM *,unsigned long *);	//428
	void  CreateHistory(char const *);	//453
	void  CreateWizardGroup(void);	//517
	void  DelPermStub(void);	//531
	struct _TREEITEM *  GetContextRoot(void);	//693
	void  GetFromHisrory(class CStringArray &);	//742
protected:
	void  GetItemData(struct _TREEITEM *,class CItemList *);	//762
	char const *  GetPermData(class CItemList *,int);	//978
	char const *  GetPermDescr(class CItemList *);	//979
	char const *  GetPermName(class CItemList *,int);	//980
public:
	int  GetPermStrins(class CItemList *,class CString &,class CString &);	//981
	int  GetPermType(class CString);	//982
	char const *  GetPermType(class CItemList *);	//983
protected:
	char const *  GetPermValidate(class CItemList *);	//984
public:
	struct _TREEITEM *  GetPermanentRoot(void);	//985
	class CSrcTemplDocParent *  GetSourceTemplate(void);	//1097
	struct _TREEITEM *  GetTemplateRoot(void);	//1112
	class CView *  GetView(void);	//1134
	struct _TREEITEM *  GetWizardRoot(void);	//1136
protected:
	int  IsParent(struct _TREEITEM *,struct _TREEITEM *);	//1231
public:
	int  IsPermanent(struct _TREEITEM *);	//1234
	int  IsTemplate(struct _TREEITEM *);	//1277
	int  IsWizard(struct _TREEITEM *);	//1286
protected:
	void  LoadPermanentGroup(int);	//1301
	void  OnAssAppl(void);	//1338
	void  OnAssOk(void);	//1339
	void  OnAssSearch(void);	//1341
	void  OnChangeMD(class CMessage7 *,long *);	//1361
	void  OnChangeParams(class CMessage7 *,long *);	//1363
	void  OnDisable(class CCmdUI *);	//1402
	void  OnHistory(void);	//1432
	void  OnReload(void);	//1531
	void  OnSTChange(class CMessage7 *,long *);	//1533
	void  OnSTEdit(class CMessage7 *,long *);	//1534
	void  OnStActivate(void);	//1569
	void  OnStubed(void);	//1572
	void  PreparePermDic(void);	//1619
public:
	void  RefreshPermanent(void);	//1653
	void  RefreshTemplates(void);	//1655
protected:
	void  SearchDlg(char const *);	//1710
	long  SendNotifyMessageA(unsigned int,long,long);	//1716
public:
	int  SendTextBlock(void);	//1718
protected:
	void  SetPermGroup(class CItemList *,struct _TREEITEM *);	//1798
public:
	void  SetSourceTemplate(class CDocument *);	//1809
	void  SetTreeCtrl(class CTreeCtrl *);	//1822
	void  SmartInsertString(struct _TREEITEM *);	//1833
	void  StartSearch(char const *);	//1840
	void  StubPermanentGroup(void);	//1850
protected:
	void  _PreparePermDic(struct _TREEITEM *,char const *);	//2029

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CAssistantView:public CFormView
{
DECLARE_DYNCREATE(CAssistantView)

protected:
	 CAssistantView(void);	//10
	virtual  ~CAssistantView(void);	//124
	virtual void  DoDataExchange(class CDataExchange *);	//547
	unsigned long  DoDragDrop(class CPoint,struct _TREEITEM *);	//553
public:
	struct _TREEITEM *  GetCurrentItem(void);	//702
protected:
	class CAssistantDoc *  GetDocument(void);	//723
	void  OnAssReturn(void);	//1340
	void  OnAssShReturn(void);	//1342
	void  OnBegindragAssTree(struct tagNMHDR *,long *);	//1343
	void  OnDblclkAssTree(struct tagNMHDR *,long *);	//1390
	void  OnDestroy(void);	//1393
	void  OnEditCopy(void);	//1405
	void  OnGetdispinfoAssTree(struct tagNMHDR *,long *);	//1431
public:
	virtual void  OnInitialUpdate(void);	//1441
protected:
	void  OnItemexpandingAssTree(struct tagNMHDR *,long *);	//1444
	void  OnLButtonDown(unsigned int,class CPoint);	//1456
	void  OnSelChangeAssTree(struct tagNMHDR *,long *);	//1541
	int  OnSetCursor(class CWnd *,unsigned int,unsigned int);	//1546
	void  OnSize(unsigned int,int,int);	//1563
	virtual void  OnUpdate(class CView *,long,class CObject *);	//1574
	void  OnUpdateAssOk(class CCmdUI *);	//1577
public:
	void  SetTextItem(struct _TREEITEM *);	//1817
protected:
	static unsigned short  cf_template;	//2083

	DECLARE_MESSAGE_MAP()
};

class IMPORT_1C CBLModule7: public CBLModule
{
public:
	//char buff [0x400];
	DWORD m_Flag1;
	DWORD m_Flag2;
	DWORD m_Flag3;
	DWORD m_Flag4;
	DWORD m_Flag5;
	DWORD m_Flag6;
	DWORD m_Flag7;
	DWORD m_Flag8;
	friend class CModuleContext;
friend class CComponentClass;
	 CBLModule7(class CBLModule7 const &);	//11
	 CBLModule7(class CBLContext *,char const *);	//12
	virtual  ~CBLModule7(void);	//125
	class CBLModule7 &  operator=(class CBLModule7 const &);	//212
	virtual int  GetKind(void)const;	//769
	virtual int  OnSyntaxError(void);	//1573
	virtual int  OnRuntimeError(void);	//1532
	virtual int  OnStartExecution(void);	//1570
	virtual int  OnNextLine(void);	//1502
	virtual void  OnEnterProc(int);	//1408
	virtual void  OnExitProc(int);	//1414
	virtual void  OnStopExecution(void);	//1571
	virtual void  OnErrorMessage(char const *);	//1411
	virtual void  OnSetSpeller(class CValue const &);	//1557
	virtual void  OnGetErrorDescription(class CString &);	//1428
	virtual int  OnDoMessageBox(char const *,unsigned int);	//1403

	// artbear показать ошибку в окно сообщений с указанием пути к ошибке
	// при двойном щелчке на таком сообщении будет переход к нужному файлу в Конфигураторе
	// неважно, где появилась ошибка, в Предприятии или Конфигураторе
	//
	// функция принимает следующие параметры
	//		iMetaNum -  номер конкретного метаданного среди прочих одинаковых, 
	//			например 41 если путь обработки в storage-файле g:\\~mdD122.tmp\\CalcVar\\CalcVar_Number41\\WorkBook\\MD Programm text
	//		szMainMetaPath - наименование типа метаданного в storage-файле (для обработок CalcVar)
	//
	// остальные параметры понятны из названий
	//
	virtual void  OnErrorMessageEx(char const * szErrorMessage, long iMetaNum, char const * szMainMetaPath, int iLineNum);	//1412

	int  EvalDebugExpr(char const *,class CValue &);	//590
	int  EvalWatchExpr(char const *,class CValue &);	//591
	int  ExecProc(unsigned int,int,class CValue *,int,class CValue * *,int,int);	//592
	class CString   GetFullName(void)const;	//743
	void  GetID(class CString &,long &)const;	//752
	int  GetProcNotFound(void)const;	//988
	int  GetStatusCode(void)const;	//1099
	int  IsConditionSucc(unsigned int);	//1211
	int  IsProcessBroken(void)const;	//1235
	int  ModifyBreakPoints(void)const;	//1318
	void  OnGetCallStackInfo(class CTraceDataMap *);	//1425
	void  SetID(char const *,long);	//1772
	void  SetStatusCode(int);	//1810
	void  StartLevelProfile(void);	//1836
	void  StartLineProfile(void);	//1837
	void  StopLevelProfile(void);	//1844
	void  StopLineProfile(void);	//1845
	int  TryEvalExpr(char const *,class CValue &,class CValue * *);	//1870
	int  TryExecuteBatch(char const *,class CValue * *,int,int);	//1871

	static int  BeginTrans(void);	//381
	static int  Commit(void);	//421
	static void  DecrNTransStarted(void);	//525
	static void  IncrNTransStarted(void);	//1181
	static int  GetNTransStarted(void);	//940
	static void  ProcessDBException7(class CDBException7 *);	//1627
	static void  ProcessOnCommandDBException7(class CException *,int &);	//1630
	static int  Rollback(void);	//1688
	static void  RollbackAll(void);	//1689
	static void  StopProfileOnExit(void);	//1847
private:
//	static class CArray<class CBLModule7 *,class CBLModule7 *>  m_BLMod7Stack;	//2154
	static class CBLModuleArray m_BLMod7Stack;	//2154
public:
	static class CStringArray  m_BreakPointConditions;	//2155
	static class CBLProfileInfo  m_ProfileInfo;	//2159
private:
	static class CDWordArray  m_TransStartedStack;	//2160
	static int  m_bDbgExprEvaluating;	//2162
};


class IMPORT_1C CBMPHolder
{
public:
	 CBMPHolder(class CBMPHolder const &);	//13
	 CBMPHolder(void);	//14
	 ~CBMPHolder(void);	//126
	class CBMPHolder const &  operator=(class CBMPHolder const &);	//213
	int  CreateFromData(struct tagBITMAPINFO *);	//452
	struct tagBITMAPINFO *  GetBitmap(void)const;	//670
	unsigned long  GetSize(void)const;	//1093
	int  LoadBitmapA(char const *);	//1294
};


class IMPORT_1C CBigCalendar
{
public:
	 CBigCalendar(void);	//15
	virtual  ~CBigCalendar(void);	//127
	int  Create(char const *,struct tagPOINT const &,class CWnd *,int,int);	//435
protected:
	void  OnActivate(unsigned int,class CWnd *,int);	//1335
	int  OnCreate(struct tagCREATESTRUCTA *);	//1382
	void  OnDestroy(void);	//1394
	long  OnFloatStatus(unsigned int,long);	//1421
public:
	void  Open(void);	//1595

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCJContextDef:public CBLContext
{
DECLARE_DYNCREATE(CCJContextDef)

public:
	 CCJContextDef(class CCJDef *,int);	//16
	virtual  ~CCJContextDef(void);	//128
//	void  `default constructor closure'(void);	//323
	class CCJDef *  GetCJDef(void)const;	//674
	virtual char const *  GetMethodName(int,int)const;	//834
	virtual int  GetNMethods(void)const;	//869
	virtual int  GetNParams(int)const;	//895
	virtual int  GetNProps(void)const;	//919
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//955
	virtual char const *  GetPropName(int,int)const;	//994
	virtual int  HasRetVal(int)const;	//1153
	virtual int  IsPropReadable(int)const;	//1239
	virtual int  IsPropWritable(int)const;	//1258
	void  SetCJDef(class CCJDef *);	//1734
};


class IMPORT_1C CCJFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CCJFormContextDef)

public:
	 CCJFormContextDef(class CCJDef *);	//17
	virtual  ~CCJFormContextDef(void);	//129
//	void  `default constructor closure'(void);	//324
	virtual char const *  GetMethodName(int,int)const;	//835
	virtual int  GetNMethods(void)const;	//870
	virtual int  GetNParams(int)const;	//896
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//956
	virtual int  HasRetVal(int)const;	//1154
};


class IMPORT_1C CCJPeriodContext:public CBLContext
{
DECLARE_DYNCREATE(CCJPeriodContext)

public:
	 CCJPeriodContext(class CString);	//18
	virtual  ~CCJPeriodContext(void);	//130
//	void  `default constructor closure'(void);	//325
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//389
	class CCalcPeriod   GetCalcPeriod(void);	//676
	virtual char const *  GetCode(void)const;	//681
	virtual char const *  GetMethodName(int,int)const;	//836
	virtual int  GetNMethods(void)const;	//871
	virtual int  GetNParams(int)const;	//897
	virtual int  GetNProps(void)const;	//920
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//957
	virtual char const *  GetPropName(int,int)const;	//995
	virtual int  GetPropVal(int,class CValue &)const;	//1015
	virtual char const *  GetTypeString(void)const;	//1125
	virtual int  HasRetVal(int)const;	//1155
	virtual int  IsPropWritable(int)const;	//1259
	void  SetPeriodValue(class CCalcPeriod);	//1796
	void  SetPeriodValue(class CString);	//1797
};


class IMPORT_1C CCJRecordContext:public CBLContext
{
DECLARE_DYNCREATE(CCJRecordContext)

public:
char buf[200];
	 CCJRecordContext(class CCJDef *);	//19
	virtual  ~CCJRecordContext(void);	//131
//	void  `default constructor closure'(void);	//326
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//390
	virtual int  CallAsProc(int,class CValue * *);	//397
	virtual char const *  GetMethodName(int,int)const;	//837
	virtual int  GetNMethods(void)const;	//872
	virtual int  GetNParams(int)const;	//898
	virtual int  GetNProps(void)const;	//921
	class CObjID   GetObjID(void);	//945
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//958
	virtual char const *  GetPropName(int,int)const;	//996
	virtual int  GetPropVal(int,class CValue &)const;	//1016
	virtual char const *  GetTypeString(void)const;	//1126
	virtual int  HasRetVal(int)const;	//1156
	virtual int  IsPropWritable(int)const;	//1260
	void  SetObjID(class CObjID);	//1790
	virtual int  SetPropVal(int,class CValue const &);	//1801
};


class IMPORT_1C CCalcContextDef:public CBLContext
{
DECLARE_DYNCREATE(CCalcContextDef)

public:
	 CCalcContextDef(void);	//20
	virtual  ~CCalcContextDef(void);	//132
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//391
	virtual char const *  GetMethodName(int,int)const;	//838
	virtual int  GetNMethods(void)const;	//873
	virtual int  GetNParams(int)const;	//899
	virtual int  GetNProps(void)const;	//922
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//959
	virtual char const *  GetPropName(int,int)const;	//997
	virtual int  GetPropVal(int,class CValue &)const;	//1017
	virtual int  HasRetVal(int)const;	//1157
	virtual int  IsPropReadable(int)const;	//1240
};


class IMPORT_1C CCalculator
{
public:
	 CCalculator(void);	//21
	virtual  ~CCalculator(void);	//133
	int  Create(char const *,struct tagPOINT const &,class CWnd *,int,int);	//436
	int  GetCurrentNumber(class CNumeric &);	//703
	int  GetNumber(class CWnd *,class CNumeric &,int,int);	//944
protected:
	void  MakeAction(void);	//1312
	class CString   NumericToString(class CNumeric &);	//1331
	void  OnActivate(unsigned int,class CWnd *,int);	//1336
	void  OnBtnsClicked(unsigned int);	//1350
	void  OnClose(void);	//1371
	int  OnCreate(struct tagCREATESTRUCTA *);	//1383
	void  OnDestroy(void);	//1395
	void  OnEditCopy(void);	//1406
	void  OnEditPaste(void);	//1407
	long  OnFloatStatus(unsigned int,long);	//1422
	unsigned int  OnGetDlgCode(void);	//1426
	void  OnLButtonDown(unsigned int,class CPoint);	//1457
public:
	void  Open(void);	//1596
	virtual int  PreTranslateMessage(struct tagMSG *);	//1614
protected:
	class CString   SetErrorState(class CNumeric);	//1758
public:
	void  SetMaxLen(int);	//1783
protected:
	int  TranslateChar(unsigned int);	//1868

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCalendar:public CWnd
{
public:
	char buf[0x100];
	 CCalendar(void);	//22
	virtual  ~CCalendar(void);	//134
	void  BegOfMonth(void);	//378
	void  BegOfQuart(void);	//379
	void  BegOfYear(void);	//380
	int  Create(unsigned long,unsigned long,struct tagPOINT const &,class CWnd *,unsigned int);	//437
	void  EndOfMonth(void);	//586
	void  EndOfQuart(void);	//587
	void  EndOfYear(void);	//588
protected:
	void  FillDrawStruct(void);	//633
public:
	class CDate   GetCurDate(void)const;	//697
	int  GetDate(class CWnd *,class CDate &,class CDate);	//715
protected:
	void  GetItemRect(int,class CRect &);	//765
	void  GetRgnFromHit(int,class CRgn *);	//1033
public:
	class CString const &  GetTitle(void);	//1121
protected:
	void  GoToNMonth(int);	//1139
public:
	int  IsExForm(void);	//1215
protected:
	void  MoveCursorTo(int,int);	//1321
	int  NcHitTest(struct tagPOINT const &,class CRgn *);	//1323
public:
	void  NextMonth(void);	//1328
	void  NextYear(void);	//1330
protected:
	void  OnClose(void);	//1372
	void  OnDestroy(void);	//1396
	long  OnFloatStatus(unsigned int,long);	//1423
	unsigned int  OnGetDlgCode(void);	//1427
	long  OnGetFont(unsigned int,long);	//1429
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1445
	void  OnKillFocus(class CWnd *);	//1448
	void  OnLButtonDown(unsigned int,class CPoint);	//1458
	void  OnLButtonUp(unsigned int,class CPoint);	//1464
	void  OnMenuButton(void);	//1478
	void  OnMouseMove(unsigned int,class CPoint);	//1479
	void  OnNcCalcSize(int,struct tagNCCALCSIZE_PARAMS *);	//1484
	unsigned int  OnNcHitTest(class CPoint);	//1485
	void  OnNcLButtonDblClk(unsigned int,class CPoint);	//1486
	void  OnNcLButtonDown(unsigned int,class CPoint);	//1487
	void  OnNcPaint(void);	//1488
	void  OnPaint(void);	//1518
	void  OnSetFocus(class CWnd *);	//1547
	long  OnSetFont(unsigned int,long);	//1551
public:
	void  PrevMonth(void);	//1623
	void  PrevYear(void);	//1625
protected:
	void  SendNotifyMessageA(unsigned int,long);	//1717
public:
	void  SetCallBack(void (__cdecl*)(struct DRAWDATESTRUCT *,class CDate,long),long);	//1735
	void  SetCurDate(int,int,int);	//1744
	void  SetCurDate(class CDate);	//1745
	void  SetCurDate(void);	//1746
	void  SetExForm(int);	//1759
	void  SetTitle(class CString const &);	//1818
	void  SetTitle(char const *);	//1819
	void  Update(void);	//1886

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCheckListBox7:public CCheckListBox //CListBox
{
DECLARE_DYNAMIC(CCheckListBox7)

public:
	DWORD m_Flag;
	int m_szImage;

	 CCheckListBox7(void);	//23
	virtual  ~CCheckListBox7(void);	//135
	int  CalcRealWidth(int);	//387
protected:
	int  MyCheckFromPoint(class CPoint,int &);	//1322
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1452
	void  OnLButtonDown(unsigned int,class CPoint);	//1459
	virtual void  PostNcDestroy(void);	//1607

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCheckListCtrl
{
public:
	 CCheckListCtrl(void);	//24
	virtual  ~CCheckListCtrl(void);	//136
protected:
	int  CheckFromPoint(class CPoint,int &);	//407
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//574
public:
	void  EnableAll(int);	//584
	void  EnableItem(int,int);	//585
protected:
	int  GetCaretIndex(void);	//677
public:
	int  GetCheck(int);	//678
	int  GetCurSel(void);	//700
	int  GetSelCount(void);	//1089
	int  GetSelItems(int,int *);	//1090
protected:
	int  IsEnabled(int);	//1213
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1446
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1453
	void  OnLButtonDown(unsigned int,class CPoint);	//1460
	void  OnPaint(void);	//1519
	long  OnSetImageList(unsigned int,long);	//1552
	void  OnSize(unsigned int,int,int);	//1564
	virtual void  PreSubclassWindow(void);	//1612
public:
	void  SetCheck(int,int);	//1737
	int  SetCurSel(int);	//1747
	int  SetSel(int,int);	//1805
	void  UpdateItem(int);	//1895
	void  UpdateItems(int,int *);	//1897

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CConfigCont:public CContainer
{
DECLARE_DYNCREATE(CConfigCont)
public:
	char buf[20];
	
	enum ConvertMode {cmOne};
protected:
	 CConfigCont(void);	//25
public:
	virtual  ~CConfigCont(void);	//137
private:
	static int  AddTxtPage(struct IStorage *,long,char const *);	//361
	static int  AddWBPage(struct IStorage *,long,char const *);	//363
public:
	int  CompressAll(int);	//423
	int  ConvertAllCJForms(void);	//424
	int  ConvertAllDocModule(int (__cdecl*)(class CDocument *,class CDocument *,class CDocument *,class CMetaDataObj *));	//425
	static int  CopyTxtPage(struct IStorage *,long,long,char const *);	//431
	static int  CopyWBPage(struct IStorage *,long,long,char const *,char const *);	//432
	static int  DeleteTxtPage(long,char const *);	//537
	static int  DeleteWBPage(long,char const *);	//538
	virtual int  DoFileSave(void);	//554
	int  DoFileSaveToPath(char const *,int);	//556
	static unsigned int  DocID;	//560
	static long  DocumentToID(class CDocument *,class CString &);	//569
	static void  DocumentToPath(class CDocument *,class CString &);	//570
	static struct IStorage *  ExtractFolders(class CMetaDataObj *);	//595
	static int  FastSave(class CContainer *,int);	//628
	int  FirstInit(int);	//656
	static void  FreeFacedPage(void);	//659
	static void  GetAllDescr(int (__cdecl*)(class CString &,long,long,enum PageType),long,int);	//667
	static void  GetAllTypedItem(int (__cdecl*)(class CString &,long,long,enum PageType),long,enum PageType,int);	//668
	static class CConfigCont *  GetContainer(void);	//687
	enum CConfigCont::ConvertMode   GetConvertMode(void);	//694
	int  GetEncriptVer(void);	//726
	class CContainer *  GetFolder(char const *);	//740
	static void  GetLastUUID(struct _GUID &);	//771
	static class CDocument *  GetOpenDocument(char const *,long,char const *,int,int &);	//946
	static int  IDToPath(long,char const *,class CString &,class CTypedCont * *,int);	//1178
	static int  IDToTextModule(long,char const *,class CString &,class CModuleCont * *,int);	//1179
private:
	static class CContainer *  InsertDummyTxtPage(long,char const *);	//1195
	static class CContainer *  InsertDummyWBPage(long,char const *);	//1196
public:
//!	static int  InsertNewAndConvert(struct IStorage *,class CMetaDataObj *,class CMap<class CMetaDataObj *,class CMetaDataObj *,long,long> *,int);	//1198
//!	static int  InsertNewFolders(struct IStorage *,class CMetaDataObj *,class CMap<class CMetaDataObj *,class CMetaDataObj *,long,long> *);	//1199
	static int  IsFolderPresent(char const *,long);	//1217
	int  IsNeedEncrypt(void);	//1224
	int  IsNeenFullRevert(void);	//1225
	int  IsPasswordCrypt(void);	//1232
	int  IsSpecialCrypt(void);	//1276
	static int  LoadConfigContainer(void);	//1295
	static int  LoadMetaData(void);	//1299
	static class CDocument *  PathToDocument(class CString const &);	//1604
	static long  PathToID(class CString const &);	//1605
	static unsigned char *  ReadStream(char const *,long,unsigned long &,int);	//1642
	static int  ReplaceWBFolder(struct IStorage *,int,char const *,long,int);	//1680
	virtual int  SaveModified(void);	//1694
	void  SetEncriptVer(int);	//1757
	void  SetNeedEncrypt(int);	//1788
	void  SetNeedFullRevert(int);	//1789
	static int  TestCanExist(char const *,long,long &);	//1860
private:
	static class CContainerPage *  TryToMakeConvertion(long,char const *,class CContainer *);	//1872
public:
	static void  ValidateOleFile(class CMetaDataCont *);	//1903
	static class CString  m_FullTempPath;	//2158

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CConfigMutex
{
public:
	 CConfigMutex(void);	//26
	 ~CConfigMutex(void);	//138
	class CConfigMutex &  operator=(class CConfigMutex const &);	//215
protected:
	int  InitRealMutex(void);	//1191
public:
	int  Lock(unsigned long);	//1302
	int  Unlock(void);	//1882
};


class IMPORT_1C CContainerHistory:public CDocument
{
DECLARE_DYNCREATE(CContainerHistory)

protected:
	 CContainerHistory(void);	//27
public:
	virtual  ~CContainerHistory(void);	//139
	static unsigned int  DocID;	//561
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//734
public:
	static class CContainerHistory *  GetGUIDData(void);	//747
protected:
	virtual int  OnNewDocument(void);	//1489
public:
	virtual int  OnOpenDocument(char const *);	//1507
protected:
	virtual void  ReleaseFile(class CFile *,int);	//1664
	virtual int  SaveModified(void);	//1695
public:
	virtual void  Serialize(class CArchive &);	//1719

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CCursorCSV
{
public:
	 CCursorCSV(class CCursorCSV const &);	//28
	 CCursorCSV(class CTableCSV *);	//29
	virtual  ~CCursorCSV(void);	//140
	class CCursorCSV &  operator=(class CCursorCSV const &);	//216
//	void  `default constructor closure'(void);	//327
	void  FX_Char(int,char &,int);	//596
	void  FX_Char(class CFieldCSV *,char &,int);	//597
	void  FX_Date(int,class CDate &,int);	//600
	void  FX_Date(class CFieldCSV *,class CDate &,int);	//601
	void  FX_Double(int,double &,int);	//604
	void  FX_Double(class CFieldCSV *,double &,int);	//605
	void  FX_Int(int,int &,int);	//608
	void  FX_Int(class CFieldCSV *,int &,int);	//609
	void  FX_Long(int,long &,int);	//612
	void  FX_Long(class CFieldCSV *,long &,int);	//613
	void  FX_LongDouble(int,long double &,int);	//616
	void  FX_LongDouble(class CFieldCSV *,long double &,int);	//617
	void  FX_String(int,class CString &,int);	//620
	void  FX_String(class CFieldCSV *,class CString &,int);	//621
	void  FX_Time(int,class CTime &,int);	//624
	void  FX_Time(class CFieldCSV *,class CTime &,int);	//625
	char const *  GetRawData(void);	//1024
	class CRecAddrCSV   GetRecAddr(void)const;	//1028
	class CTableCSV *  GetSourceTable(void);	//1096
	int  Goto(class CRecAddrCSV &);	//1140
	int  IsAttached(void)const;	//1207
	int  IsOK(void)const;	//1226
	int  New(void);	//1324
	void  SetRawData(char const *);	//1804
	int  SetSourceTable(class CTableCSV *);	//1808
};


class IMPORT_1C CDBEngDB7CodePageDlg
{
public:
	 CDBEngDB7CodePageDlg(class CWnd *);	//30
	virtual  ~CDBEngDB7CodePageDlg(void);	//141
//	void  `default constructor closure'(void);	//328
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//548
	virtual int  OnInitDialog(void);	//1435
	virtual void  OnOK(void);	//1504

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CDebugBreak
{
public:
	 CDebugBreak(class CDebugBreak const &);	//31
	 CDebugBreak(unsigned int);	//32
	 CDebugBreak(void);	//33
	 ~CDebugBreak(void);	//142
	class CDebugBreak &  operator=(class CDebugBreak const &);	//217
	void  Modify(class CBreakPoint const &);	//1317
protected:
	static int  nextKey;	//2221
};


class IMPORT_1C CDialogTB
{
public:
	 CDialogTB(void);	//34
	virtual  ~CDialogTB(void);	//143
	void  DoSize(class CRect const &);	//559
protected:
	void  OnLButtonDown(unsigned int,class CPoint);	//1461
	void  OnLButtonUp(unsigned int,class CPoint);	//1465
public:
	void  UpdateAllButtons(void);	//1890
protected:
	void  UpdateButton(int);	//1892

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CDocContextDef:public CBLContext
{
DECLARE_DYNCREATE(CDocContextDef)

public:
	 CDocContextDef(class CDocDef *,int);	//35
	virtual  ~CDocContextDef(void);	//144
//	void  `default constructor closure'(void);	//329
protected:
	virtual class CDocDef *  CurDocDef(void)const;	//521
public:
	virtual char const *  GetMethodName(int,int)const;	//839
	virtual int  GetNMethods(void)const;	//874
	virtual int  GetNParams(int)const;	//900
	virtual int  GetNProps(void)const;	//923
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//960
	virtual char const *  GetPropName(int,int)const;	//998
	virtual int  HasRetVal(int)const;	//1158
	virtual int  IsPropReadable(int)const;	//1241
	virtual int  IsPropWritable(int)const;	//1261
	void  SetDocDef(class CDocDef *);	//1754
protected:
	int  TranPropPos(int &)const;	//1863
};


class IMPORT_1C CDocEnvironContextDef:public CBLContext
{
DECLARE_DYNCREATE(CDocEnvironContextDef)

public:
	 CDocEnvironContextDef(void);	//36
	virtual  ~CDocEnvironContextDef(void);	//145
	virtual char const *  GetMethodName(int,int)const;	//840
	virtual int  GetNMethods(void)const;	//875
	virtual int  GetNParams(int)const;	//901
	virtual int  GetNProps(void)const;	//924
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//961
	virtual char const *  GetPropName(int,int)const;	//999
	virtual int  HasRetVal(int)const;	//1159
	virtual int  IsPropReadable(int)const;	//1242
};


class IMPORT_1C CDocFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CDocFormContextDef)

public:
	 CDocFormContextDef(void);	//37
	virtual  ~CDocFormContextDef(void);	//146
	virtual char const *  GetMethodName(int,int)const;	//841
	virtual int  GetNMethods(void)const;	//876
	virtual int  GetNParams(int)const;	//902
	virtual int  GetNProps(void)const;	//925
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//962
	virtual char const *  GetPropName(int,int)const;	//1000
	virtual int  HasRetVal(int)const;	//1160
	virtual int  IsPropReadable(int)const;	//1243
};


class IMPORT_1C CDocType
{
public:
	 CDocType(void);	//38
	class CDocType &  operator=(class CDocType const &);	//218
	unsigned int  GetCurrentType(void);	//704
	void  SetCurrentType(unsigned int);	//1749
};

class IMPORT_1C CDrawTrasparentCtrl: public CButton
{
public:
	DWORD buf[0x10];
	CDrawTrasparentCtrl(void);
	virtual ~CDrawTrasparentCtrl(void);
	DECLARE_MESSAGE_MAP()

protected:
	struct HBRUSH__ * CtlColor(class CDC *,unsigned int);
	void OnEnable(int);
	virtual void PostNcDestroy(void);
};

class IMPORT_1C CDrawStatic: public CWnd
{
public:
	DWORD buf[0x10];
	 CDrawStatic(void);	//39
	virtual  ~CDrawStatic(void);	//147

	DECLARE_MESSAGE_MAP()

protected:
	virtual void  PostNcDestroy(void);	//1608
	struct HBRUSH__ *  CtlColor(class CDC *,unsigned int);	//520
public:
	int  IsTransparent(void);	//1282
	void  SetTransparent(int);	//1821

};


class IMPORT_1C CFetchCSV
{
public:
	 CFetchCSV(class CFetchCSV const &);	//40
	 CFetchCSV(int);	//41
	virtual  ~CFetchCSV(void);	//148
	class CFetchCSV &  operator=(class CFetchCSV const &);	//219
	class CRecAddrCSV   operator[](int);	//229
//	void  `default constructor closure'(void);	//330
	void  AddInBack(class CRecAddrCSV);	//346
	void  AddInFront(class CRecAddrCSV);	//347
	void  AddInPos(long,class CRecAddrCSV);	//348
	void  AttachToTable(class CTableCSV *);	//376
	void  Clear(void);	//411
	int  Create(class CExpression *);	//438
	void  DetachFromTable(void);	//541
	long  FindNearDate(class CDate);	//647
	long  FindNearPos(long);	//649
	long  First(void);	//653
	long  GetBookmark(void);	//671
	class CExpression *  GetExpression(void);	//728
	long  GetSize(void);	//1094
	long  GetTableSize(void);	//1106
	long  GetType(void);	//1123
	int  Goto(long);	//1141
	long  GotoPos(long);	//1144
	int  IsRefreshable(void);	//1274
	long  Last(void);	//1287
	long  Next(long);	//1325
	long  Prev(long);	//1620
	void  SetBookmark(long);	//1733
	void  SetExpression(class CExpression *);	//1763
	void  SetType(long);	//1823
};


class IMPORT_1C CFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CFormContextDef)

public:
	 CFormContextDef(void);	//42
	virtual  ~CFormContextDef(void);	//149
	void  AddProp(char const *,int);	//356
	virtual char const *  GetMethodName(int,int)const;	//842
	virtual int  GetNMethods(void)const;	//877
	virtual int  GetNParams(int)const;	//903
	virtual int  GetNProps(void)const;	//926
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//963
	virtual char const *  GetPropName(int,int)const;	//1001
	virtual int  HasRetVal(int)const;	//1161
	virtual int  IsPropReadable(int)const;	//1244
	virtual int  IsPropWritable(int)const;	//1262
protected:
	virtual int  TranPropPos(int &)const;	//1864
};


class IMPORT_1C CFormatWizard
{
public:
	 CFormatWizard(int,class CWnd *);	//43
	virtual  ~CFormatWizard(void);	//150
protected:
	void  DecodeResult(void);	//524
	virtual void  DoDataExchange(class CDataExchange *);	//549
	void  ModifyResult(int);	//1319
	void  OnChange(void);	//1355
	void  OnChangeDivDec(void);	//1357
	void  OnChangeDivTriad(void);	//1358
	void  OnCheckDashzero(void);	//1366
	void  OnCheckKillzero(void);	//1367
	void  OnCheckLeadzero(void);	//1368
	void  OnCurr(void);	//1387
	virtual int  OnInitDialog(void);	//1436
	void  OnSelchangeTypelist(void);	//1543
	void  OnSkip(void);	//1568

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CGalleryHolder:public CDocument
{
DECLARE_DYNCREATE(CGalleryHolder)

protected:
	 CGalleryHolder(void);	//44
public:
	virtual  ~CGalleryHolder(void);	//151
	unsigned long  AddPicture(class CPictureHolder7 &,int);	//355
	int  AssignAlias(unsigned long,char const *);	//371
	int  DeletePicture(unsigned long);	//535
	static unsigned int  DocID;	//562
	unsigned long  FindByAlias(char const *);	//642
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//735
public:
	static class CGalleryHolder *  GetGallery(void);	//748
	class CImageList const *  GetImageList(void)const;	//754
	class CDWordArray const *  GetImageMap(void)const;	//756
	class CItemList const *  GetNameList(void)const;	//943
	class CPictureHolder7   GetPicture(unsigned long);	//987
	int  ID2Position(unsigned long);	//1177
protected:
	virtual int  OnNewDocument(void);	//1490
public:
	virtual int  OnOpenDocument(char const *);	//1508
	unsigned long  Position2ID(int);	//1606
	void  ReValidate(void);	//1638
protected:
	virtual void  ReleaseFile(class CFile *,int);	//1665
	virtual int  SaveModified(void);	//1696
public:
	int  SavePictHldr(unsigned long,class CPictureHolder7 &);	//1708
	virtual void  Serialize(class CArchive &);	//1720
protected:
	void  SetExtRepMode(int);	//1764

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CGeneralContextDef:public CBLContext
{
DECLARE_DYNCREATE(CGeneralContextDef)

public:
	 CGeneralContextDef(void);	//45
	virtual  ~CGeneralContextDef(void);	//152
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//392
	virtual int  CallAsProc(int,class CValue * *);	//398
	virtual char const *  GetMethodName(int,int)const;	//843
	virtual int  GetNMethods(void)const;	//878
	virtual int  GetNParams(int)const;	//904
	virtual int  GetNProps(void)const;	//927
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//964
	virtual char const *  GetPropName(int,int)const;	//1002
	virtual int  GetPropVal(int,class CValue &)const;	//1018
	virtual int  HasRetVal(int)const;	//1162
	virtual int  IsPropReadable(int)const;	//1245
};


class IMPORT_1C CImageCol: public CColumn
{
public:
	 CImageCol(unsigned int,int *,char const *,int,unsigned int);	//46
	virtual  ~CImageCol(void);	//153
	virtual int  DrawItem(class CDC &,class CRect &,class CString &,int);	//575
	virtual void  GetData(class CString &);	//710
};


class IMPORT_1C CImageColComplex: public CColumn
{
public:
	 CImageColComplex(unsigned int,int *,int *,int *,char const *,int,unsigned int);	//47
	virtual  ~CImageColComplex(void);	//154
	virtual int  DrawItem(class CDC &,class CRect &,class CString &,int);	//576
	virtual void  GetData(class CString &);	//711
};


class IMPORT_1C CJournalContextDef:public CBLContext
{
DECLARE_DYNCREATE(CJournalContextDef)

public:
	 CJournalContextDef(class CJournalDef *);	//48
	virtual  ~CJournalContextDef(void);	//155
//	void  `default constructor closure'(void);	//331
	virtual char const *  GetMethodName(int,int)const;	//844
	virtual int  GetNMethods(void)const;	//879
	virtual int  GetNParams(int)const;	//905
	virtual int  GetNProps(void)const;	//928
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//965
	virtual char const *  GetPropName(int,int)const;	//1003
	virtual int  HasRetVal(int)const;	//1163
	virtual int  IsPropReadable(int)const;	//1246
	virtual int  IsPropWritable(int)const;	//1263
	void  SetJournalDef(class CJournalDef *);	//1780
protected:
	int  TranPropPos(int &)const;	//1865
};


class IMPORT_1C CListCtrlEx:public CListCtrl
{
DECLARE_DYNCREATE(CListCtrlEx)

public:
	 CListCtrlEx(void);	//49
	virtual  ~CListCtrlEx(void);	//156
protected:
	virtual void  DrawColumnItem(int,int,class CDC *,class CRect &,struct tagLVCOLUMNA &);	//572
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//577
public:
	int  GetCurSel(void);	//701
	int  GetFullRowSel(void);	//745
protected:
	static char const *  MakeShortString(class CDC *,char const *,int,int);	//1313
	void  OnKillFocus(class CWnd *);	//1449
	void  OnPaint(void);	//1520
	long  OnSetBkColor(unsigned int,long);	//1544
	void  OnSetFocus(class CWnd *);	//1548
	long  OnSetImageList(unsigned int,long);	//1553
	long  OnSetTextBkColor(unsigned int,long);	//1558
	long  OnSetTextColor(unsigned int,long);	//1560
	void  OnSize(unsigned int,int,int);	//1565
public:
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1609
protected:
	void  RepaintSelectedItems(void);	//1678
public:
	int  SetCurSel(int);	//1748
	int  SetFullRowSel(int);	//1769
	void  UpdateItem(int);	//1896

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CListTreeCtrl
{
public:
	 CListTreeCtrl(void);	//50
	virtual  ~CListTreeCtrl(void);	//157
	void  CheckAll(int);	//404
protected:
//***	virtual void  DrawHierarchyLines(class SECListCtrl::LvPaintContext *);	//573
	virtual void  DrawHierarchyLines(class SECListPaintContext *);
//***	virtual void  DrawSubItem(class SECListCtrl::LvPaintContext *);	//582
	virtual void  DrawSubItem(class SECListPaintContext *);
public:
	int  GetCheck(struct _TREEITEM *);	//679
protected:
	void  GetCheckBoxRect(struct _TREEITEM *,class CRect &);	//680
	struct _TREEITEM *  HitTest(class CPoint,unsigned int *);	//1176
//***	virtual int  LoadPaintContext(class SECListCtrl::LvPaintContext *);	//1300
	virtual int  LoadPaintContext(class SECListPaintContext *);
//***	virtual void  MeasureSubItem(class SECListCtrl::LvPaintContext *,unsigned int &);	//1315
	virtual void  MeasureSubItem(class SECListPaintContext *,unsigned int &);
	void  OnKeyDown(unsigned int,unsigned int,unsigned int);	//1447
	void  OnKillFocus(class CWnd *);	//1450
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1454
	void  OnLButtonDown(unsigned int,class CPoint);	//1462
	int  OnMouseWheel(unsigned int,short,class CPoint);	//1480
	void  OnSetFocus(class CWnd *);	//1549
public:
	void  SetCheck(struct _TREEITEM *,int);	//1738
protected:
//***	void  SetCheckInternal(class SECTreeCtrl::Node *,int);	//1739
	void  SetCheckInternal(class _Node *,int);	//1739
public:
	void  SetFocusedIndex(int);	//1768
	virtual class CImageList *  SetImageList(class CImageList *,int);	//1773

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CListViewEx:public CListView
{
DECLARE_DYNCREATE(CListViewEx)

public:
	 CListViewEx(void);	//51
	virtual  ~CListViewEx(void);	//158
protected:
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//578
public:
	int  GetFullRowSel(void);	//746
protected:
	static char const *  MakeShortString(class CDC *,char const *,int,int);	//1314
	void  OnKillFocus(class CWnd *);	//1451
	void  OnPaint(void);	//1521
	long  OnSetBkColor(unsigned int,long);	//1545
	void  OnSetFocus(class CWnd *);	//1550
	long  OnSetImageList(unsigned int,long);	//1554
	long  OnSetTextBkColor(unsigned int,long);	//1559
	long  OnSetTextColor(unsigned int,long);	//1561
	void  OnSize(unsigned int,int,int);	//1566
public:
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1610
protected:
	void  RepaintSelectedItems(void);	//1679
public:
	int  SetFullRowSel(int);	//1770

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLiveList
{
public:
	 CLiveList(void);	//52
	virtual  ~CLiveList(void);	//159
	int  Add(class CString const &);	//339
	int  Add(char const *);	//340
	int  DoSelectFromList(int,class CRect,class CFont *,int,int);	//558
	char const *  GetText(int)const;	//1113
protected:
	void  OnClose(void);	//1373
	int  OnCompareItem(int,struct tagCOMPAREITEMSTRUCT *);	//1381
	void  OnDestroy(void);	//1397
	void  OnDrawItem(int,struct tagDRAWITEMSTRUCT *);	//1404
	long  OnFloatStatus(unsigned int,long);	//1424
	void  OnMeasureItem(int,struct tagMEASUREITEMSTRUCT *);	//1477
public:
	int  SelectFromList(int,class CWnd *,int,int);	//1711
	int  SelectFromList(int,class CRect,int,int);	//1712

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLogDoc:public CGetDoc
{
DECLARE_DYNCREATE(CLogDoc)

protected:
	 CLogDoc(void);	//53
public:
	virtual  ~CLogDoc(void);	//160
	void  ComposeTitle(class CString &,class CString &);	//422
protected:
	virtual int  DataExchange(class CGetExchange *);	//522
private:
	int  Init(void);	//1182
protected:
	void  OnBrContextMenu(void);	//1344
	void  OnBrSearch(void);	//1346
	void  OnBrowseScan(void);	//1347
	void  OnBrowseWantVScroll(void);	//1349
public:
	virtual void  OnCloseDocument(void);	//1375
protected:
	void  OnFileClose(void);	//1415
	void  OnFileSaveAs(void);	//1418
	void  OnLogDetail(void);	//1470
	void  OnLogFilterChange(void);	//1471
	void  OnLogFilterDel(void);	//1472
	void  OnLogFilterSel(void);	//1473
	void  OnLogViewObj(void);	//1474
	void  OnLogViewRec(void);	//1475
public:
	virtual int  OnNewDocument(void);	//1491
	virtual int  OnOpenDocument(char const *);	//1509
	virtual void  OnRead(int);	//1527
protected:
	void  OnRefresh(void);	//1529
	void  OnUpdateDblClk(class CCmdUI *);	//1578
	void  OnUpdateEditRepeat(class CCmdUI *);	//1580
	void  OnUpdateLogDetail(class CCmdUI *);	//1584
	void  OnUpdateRefresh(class CCmdUI *);	//1585
	void  OnUpdateSave(class CCmdUI *);	//1586
	void  OnUpdateSaveAs(class CCmdUI *);	//1587
	void  OnUpdateSelFilter(class CCmdUI *);	//1590
	void  OnUpdateViewRec(class CCmdUI *);	//1591
public:
	void  Refresh(void);	//1649
	virtual void  Serialize(class CArchive &);	//1721
private:
	int  _AcceptNewFetch(class CLogFetch *);	//1915
	static int  m_iDocCount;	//2168

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLogFrame:public CChildFrame7
{
DECLARE_DYNCREATE(CLogFrame)

public:
	 CLogFrame(void);	//54
	virtual  ~CLogFrame(void);	//161
protected:
	int  OnCreate(struct tagCREATESTRUCTA *);	//1384
public:
	virtual void  OnUpdateFrameTitle(int);	//1582
	virtual int  PreCreateWindow(struct tagCREATESTRUCTA &);	//1611

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLogSkipper
{
public:
	 CLogSkipper(void);	//55
	virtual  ~CLogSkipper(void);	//162
	int  CheckScroll(void);	//409
	void  First(void);	//654
	long  GetCurPos(void);	//698
	virtual long  GetIndex(void);	//757
	int  IsAtBottom(void);	//1206
	void  Last(void);	//1288
	int  Next(void);	//1326
	virtual int  Ok(void);	//1333
	int  Prev(void);	//1621
	void  ProcessException(void);	//1628
	void  Refresh(void);	//1650
	void  SetDoc(class CLogDoc *);	//1752
	void  SetFetch(class CLogFetch *);	//1765
	virtual void  SetIndex(long);	//1774
	virtual void  SetMinMaxIndex(void);	//1784
	void  Update(void);	//1887
	virtual long  getdba(void);	//2143
	virtual long  getpos(void);	//2145
	virtual void  gobot(void);	//2147
	virtual void  godba(long);	//2149
	virtual void  gotop(void);	//2151
	virtual int  next(void);	//2219
	virtual int  prev(void);	//2223
	virtual long  setpos(void);	//2234
	virtual int  skip(int);	//2236
};


class IMPORT_1C CLogView:public CBrowseView
{
DECLARE_DYNCREATE(CLogView)

protected:
	 CLogView(void);	//56
public:
	virtual  ~CLogView(void);	//163
	class CLogDoc *  GetDocument(void);	//724
protected:
	void  OnDestroy(void);	//1398
public:
	virtual void  OnInitialUpdate(void);	//1442
protected:
	void  OnSize(unsigned int,int,int);	//1567
	virtual void  OnUpdate(class CView *,long,class CObject *);	//1575
	virtual int  PreTranslateMessage(struct tagMSG *);	//1615

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CLogoPan: public CButton
{
public:

	DWORD buf[0x15];
	 CLogoPan(void);	//57
	virtual  ~CLogoPan(void);	//164

	DECLARE_MESSAGE_MAP()

protected:
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//579
	int  OnEraseBkgnd(class CDC *);	//1409
public:
	int  SetContent(struct HBITMAP__ *);	//1741
	int  SetContent(struct HINSTANCE__ *,char *);	//1742
	void  SizeToContent(void);	//1831
};

class IMPORT_1C CMD5Hashing
{
public:
	 CMD5Hashing(void);	//58
	 ~CMD5Hashing(void);	//165
	class CMD5Hashing &  operator=(class CMD5Hashing const &);	//220
protected:
	void  MD5Decode(unsigned long *,unsigned char *,unsigned int);	//1306
	void  MD5Encode(unsigned char *,unsigned long *,unsigned int);	//1307
public:
	void  MD5HashValue(unsigned char * const);	//1308
	void  MD5Initialize(void);	//1309
protected:
	void  MD5Transform(unsigned long * const,unsigned char * const);	//1310
public:
	void  MD5Update(void const *,unsigned int);	//1311
};


class IMPORT_1C CMDataCont:public CContainer
{
DECLARE_DYNCREATE(CMDataCont)

protected:
	 CMDataCont(void);	//59
public:
	virtual  ~CMDataCont(void);	//166
	static unsigned int  DocID;	//563
	static class CMDataCont *  GetContainer(void);	//688
	virtual int  OnNewDocument(void);	//1492
	virtual int  OnOpenDocument(char const *);	//1510
	virtual int  SaveModified(void);	//1697

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMenuButton
{
public:
	 CMenuButton(void);	//60
	virtual  ~CMenuButton(void);	//167
	void  AttachMenu(unsigned int);	//375
	int  Create(char const *,unsigned int,struct tagRECT const &,class CWnd *,unsigned int);	//439
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//580
protected:
	long  OnButtonClick(unsigned int,long);	//1351
	void  OnButtonPushed(void);	//1352
	void  OnLButtonDblClk(unsigned int,class CPoint);	//1455
	void  OnLButtonDown(unsigned int,class CPoint);	//1463
	virtual void  PreSubclassWindow(void);	//1613
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//1616
	void  SetCallBack(void (__cdecl*)(class CMenu *,long),long);	//1736

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMetaDataStream:public CDocument
{
DECLARE_DYNCREATE(CMetaDataStream)

protected:
	 CMetaDataStream(void);	//61
public:
	virtual  ~CMetaDataStream(void);	//168
	static unsigned int  DocID;	//564
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//736
public:
	static class CMetaDataStream *  GetStream(void);	//1101
	class CMetaDataCont *  GetWorkMDCont(void);	//1138
	virtual int  OnNewDocument(void);	//1493
protected:
	virtual void  ReleaseFile(class CFile *,int);	//1666
	virtual int  SaveModified(void);	//1698
public:
	virtual void  Serialize(class CArchive &);	//1722
	void  SetWorkMDCont(class CMetaDataCont *);	//1826

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CModuleCompileParam
{
public:
	 CModuleCompileParam(void);	//62
	class CModuleCompileParam &  operator=(class CModuleCompileParam const &);	//221
};


class IMPORT_1C CModuleCont:public CContainer
{
DECLARE_DYNCREATE(CModuleCont)
protected:
	 CModuleCont(void);	//63
public:
	virtual  ~CModuleCont(void);	//169
	class CDocument *  FindDocument(char const *,int *);	//644
	class CDocument *  GetTextDocument(void);	//1115
	virtual int  OnNewDocument(void);	//1494
	virtual int  OnOpenDocument(char const *);	//1511
	virtual int  SaveModified(void);	//1699
	void  ShowDocument(char const *);	//1827

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMonitorDoc:public CGetDoc
{
DECLARE_DYNCREATE(CMonitorDoc)

protected:
	 CMonitorDoc(void);	//64
public:
	virtual  ~CMonitorDoc(void);	//170
protected:
	void  AddNewInfo(char const *);	//352
	virtual int  DataExchange(class CGetExchange *);	//523
public:
	void  Load(void);	//1291
protected:
	void  OnBrContextMenu(void);	//1345
	void  OnBrowseScan(void);	//1348
public:
	virtual void  OnCloseDocument(void);	//1376
	virtual int  OnNewDocument(void);	//1495
protected:
	virtual void  OnRead(int);	//1528
	void  OnRefresh(void);	//1530
	void  OnUpdateDblClk(class CCmdUI *);	//1579
	void  OnUpdateEditRepeat(class CCmdUI *);	//1581
	void  OnUpdateSaveAs(class CCmdUI *);	//1588
	void  OnUserLog(void);	//1592
public:
	void  Refresh(void);	//1651
	void  ReloadData(void);	//1670

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMonitorFrame:public CChildFrame7
{
DECLARE_DYNCREATE(CMonitorFrame)

public:
	 CMonitorFrame(void);	//65
	virtual  ~CMonitorFrame(void);	//171
protected:
	int  OnCreate(struct tagCREATESTRUCTA *);	//1385
	void  OnDestroy(void);	//1399
public:
	virtual void  OnUpdateFrameTitle(int);	//1583

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMonitorView:public CBrowseView
{
DECLARE_DYNCREATE(CMonitorView)

protected:
	 CMonitorView(void);	//66
public:
	virtual  ~CMonitorView(void);	//172
	class CMonitorDoc *  GetDocument(void);	//725
protected:
	void  OnDestroy(void);	//1400
	virtual void  OnInitialUpdate(void);	//1443
	virtual void  OnUpdate(class CView *,long,class CObject *);	//1576
	virtual int  PreTranslateMessage(struct tagMSG *);	//1617

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CMoxelVarsContextDef
{
public:
	 CMoxelVarsContextDef(void);	//67
	virtual  ~CMoxelVarsContextDef(void);	//173
	virtual int  GetNProps(void)const;	//929
	virtual char const *  GetPropName(int,int)const;	//1004
	virtual int  IsPropReadable(int)const;	//1247
	virtual int  IsPropWritable(int)const;	//1264
	void  SetObject(struct ISheetObject *);	//1791
};


class IMPORT_1C COperContextDef:public CBLContext
{
DECLARE_DYNCREATE(COperContextDef)

public:
	 COperContextDef(void);	//68
	virtual  ~COperContextDef(void);	//174
	virtual char const *  GetMethodName(int,int)const;	//845
	virtual int  GetNMethods(void)const;	//880
	virtual int  GetNParams(int)const;	//906
	virtual int  GetNProps(void)const;	//930
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//966
	virtual char const *  GetPropName(int,int)const;	//1005
	virtual int  HasRetVal(int)const;	//1164
	virtual int  IsPropReadable(int)const;	//1248
	virtual int  IsPropWritable(int)const;	//1265
};


class IMPORT_1C COperFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(COperFormContextDef)

public:
	 COperFormContextDef(void);	//69
	virtual  ~COperFormContextDef(void);	//175
	virtual char const *  GetMethodName(int,int)const;	//846
	virtual int  GetNMethods(void)const;	//881
	virtual int  GetNParams(int)const;	//907
	virtual int  GetNProps(void)const;	//931
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//967
	virtual char const *  GetPropName(int,int)const;	//1006
	virtual int  HasRetVal(int)const;	//1165
	virtual int  IsPropReadable(int)const;	//1249
	virtual int  IsPropWritable(int)const;	//1266
};


class IMPORT_1C COperJournalContextDef:public CBLContext
{
DECLARE_DYNCREATE(COperJournalContextDef)

public:
	 COperJournalContextDef(void);	//70
	virtual  ~COperJournalContextDef(void);	//176
	virtual char const *  GetMethodName(int,int)const;	//847
	virtual int  GetNMethods(void)const;	//882
	virtual int  GetNParams(int)const;	//908
	virtual int  GetNProps(void)const;	//932
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//968
	virtual char const *  GetPropName(int,int)const;	//1007
	virtual int  HasRetVal(int)const;	//1166
	virtual int  IsPropReadable(int)const;	//1250
	virtual int  IsPropWritable(int)const;	//1267
protected:
	int  TranPropPos(int &)const;	//1866
};


class IMPORT_1C CPictureCont:public CContainer
{
DECLARE_DYNCREATE(CPictureCont)

protected:
	 CPictureCont(void);	//71
public:
	virtual  ~CPictureCont(void);	//177
	static unsigned int  DocID;	//565
	static class CPictureCont *  GetContainer(void);	//689
	class CGalleryHolder *  GetGallery(void);	//749
protected:
	virtual int  OnNewDocument(void);	//1496
public:
	virtual int  OnOpenDocument(char const *);	//1512
protected:
	virtual int  OnSaveInternal(void);	//1537
	virtual int  SaveModified(void);	//1700

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CProvJournalContextDef:public CBLContext
{
DECLARE_DYNCREATE(CProvJournalContextDef)

public:
	 CProvJournalContextDef(void);	//72
	virtual  ~CProvJournalContextDef(void);	//178
	virtual char const *  GetMethodName(int,int)const;	//848
	virtual int  GetNMethods(void)const;	//883
	virtual int  GetNParams(int)const;	//909
	virtual int  GetNProps(void)const;	//933
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//969
	virtual char const *  GetPropName(int,int)const;	//1008
	virtual int  HasRetVal(int)const;	//1167
	virtual int  IsPropReadable(int)const;	//1251
	virtual int  IsPropWritable(int)const;	//1268
protected:
	int  TranPropPos(int &)const;	//1867
};


class IMPORT_1C CRepFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CRepFormContextDef)

public:
	 CRepFormContextDef(void);	//73
	virtual  ~CRepFormContextDef(void);	//179
	virtual char const *  GetMethodName(int,int)const;	//849
	virtual int  GetNMethods(void)const;	//884
	virtual int  GetNParams(int)const;	//910
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//970
	virtual int  HasRetVal(int)const;	//1168
};


class IMPORT_1C CRightAtom
{
public:
	 CRightAtom(class CRightAtom &);	//74
	 CRightAtom(unsigned long,int);	//75
	 CRightAtom(void);	//76
	class CRightAtom &  operator=(class CRightAtom const &);	//222
};


class IMPORT_1C CRightInfo
{
public:
	 CRightInfo(class CRightInfo &);	//77
	 CRightInfo(unsigned int,long,class CDWordArray *);	//78
	 CRightInfo(void);	//79
	 ~CRightInfo(void);	//180
	void  operator=(class CRightInfo &);	//223
	void  GetAtribytes(unsigned int &,long &)const;	//669
	int  IsAccess(unsigned long)const;	//1202
	void  RefreshRights(class CRightInfo *,class CDWordArray *);	//1654
	virtual void  Serialize(class CArchive &,int);	//1723
	void  Set(class CRightInfo &);	//1729
	int  SetAccess(unsigned int,int,int);	//1731
	void  SetAll(int);	//1732
};


class IMPORT_1C CRightsContainer:public CObject
{
public:
	 CRightsContainer(void);	//80
	virtual  ~CRightsContainer(void);	//181
	void  AddCompareStrings(int,class CRightInfo *,class CRightInfo *,class CString &,class CString &,class CString);	//342
	void  AddInfo(class CRightsContainer *);	//351
	int  CheckAccess(unsigned int,long,unsigned int);	//403
protected:
	class CRightInfo *  Find(unsigned int,long)const;	//639
public:
	void  FullDifferens(class CRightsContainer *,class CString &,class CString &,class CMetaDataCont *,class CMetaDataCont *,int);	//660
	int  GetCount(void)const;	//695
	class CString   GetDescription(void);	//718
	void  GetDifferens(class CRightsContainer *,int,class CMetaDataObj *,class CMetaDataObj *,class CString &,class CString);	//720
	class CStringArray *  GetFullNameList(void);	//744
	virtual class CRightInfo *  GetRInfo(unsigned int,long,int,class CDWordArray *,class CDocument *);	//1023
	int  Has(unsigned int,long)const;	//1147
	int  IsAccess(unsigned int,long,unsigned int)const;	//1203
	int  IsEnabled(void);	//1214
	int  IsInstalled(void);	//1221
	int  IsObjectsDiff(class CRightsContainer *,int,class CMetaDataObj *,class CMetaDataObj *);	//1227
	void  ObjectDifferens(class CRightsContainer *,int,class CMetaDataObj *,class CMetaDataObj *,class CString &,class CString &,class CString);	//1332
protected:
	virtual int  RemoveInfo(unsigned int,long);	//1676
public:
	virtual void  Serialize(class CArchive &,int);	//1724
	void  SetDescription(class CString);	//1750
	void  SetEnabled(int);	//1756
	void  SetInstallFlag(void);	//1777
	int  Test(void);	//1857
};


class IMPORT_1C CSTInsrtType: public CDialog
{
public:
	char buf[1000];
	 CSTInsrtType(class CWnd *);	//81
	virtual  ~CSTInsrtType(void);	//182
//	void  `default constructor closure'(void);	//332
	static class CString   ConvertTemplate(char const *,char const *,class CPoint &,int &);	//426
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//550
	void  OnCaretRadio(void);	//1354
	virtual int  OnInitDialog(void);	//1437
	void  OnMDRadio(void);	//1476
	virtual void  OnOK(void);	//1505
	void  OnOtherRadio(void);	//1517
	static int  m_selType;	//2171

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSheetPluginDT
{
public:
	 CSheetPluginDT(class CSheetPluginDT const &);	//82
	 CSheetPluginDT(void);	//83
	 ~CSheetPluginDT(void);	//183
	class CSheetPluginDT &  operator=(class CSheetPluginDT const &);	//224
	virtual long  CreateEdit(struct ISheetEditSite *,struct ISheetEdit * *);	//447
	static long  CreatePluginDT(struct ISheetHook * *);	//511
	virtual long  CreatePropertyPage(struct IPropertyPage * *);	//512
	virtual long  CreateValue(struct ISheetValue * *);	//516
	struct IUnknown *  GetUnknown(void);	//1129
	virtual long  OnChange(unsigned short *);	//1356
	virtual long  OnClick(unsigned short *,unsigned short *);	//1370
	virtual long  OnClose(void);	//1374
	virtual long  OnEvaluate(unsigned short *);	//1413
	virtual long  OnLoad(void);	//1466
	static long  _Cache(void *,struct _GUID const &,void * *,unsigned long);	//1916
//	static struct ATL::_ATL_INTMAP_ENTRY const *  _GetEntries(void);	//2020
	struct IUnknown *  _GetRawUnknown(void);	//2023
	long  _InternalQueryInterface(struct _GUID const &,void * *);	//2026
};


class IMPORT_1C CSheetPluginPage
{
public:
	 CSheetPluginPage(void);	//84
	virtual  ~CSheetPluginPage(void);	//184
	virtual long  Apply(void);	//368
	struct HWND__ *  Create(struct HWND__ *);	//440
protected:
	void  DisableControls(void);	//542
	virtual void  DoDataExchange(class CDataExchange *);	//551
	void  FillTypeBox(void);	//637
public:
	struct IUnknown *  GetUnknown(void);	//1130
protected:
	void  OnChangeFmt(void);	//1359
	void  OnChangeLen(void);	//1360
	void  OnChangeMxlMask(void);	//1362
	void  OnChangePrec(void);	//1364
	virtual int  OnCommand(unsigned int,long);	//1379
	void  OnDeltaposMxlSpinLen(struct tagNMHDR *,long *);	//1391
	virtual int  OnInitDialog(void);	//1438
	void  OnMxlDisable(void);	//1481
	void  OnMxlPedal(void);	//1482
	void  OnMxlReadonly(void);	//1483
	void  OnNumsep(void);	//1503
	void  OnPosonly(void);	//1525
	void  OnSelchangeType(void);	//1542
	void  OnSetObjects(void);	//1555
public:
	virtual long  SetObjects(unsigned long,struct IUnknown * *);	//1792
	static long  _Cache(void *,struct _GUID const &,void * *,unsigned long);	//1917
//	static struct ATL::_ATL_INTMAP_ENTRY const *  _GetEntries(void);	//2021
	struct IUnknown *  _GetRawUnknown(void);	//2024
	long  _InternalQueryInterface(struct _GUID const &,void * *);	//2027

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSheetValue
{
public:
	 CSheetValue(class CSheetValue const &);	//85
	 CSheetValue(void);	//86
	 ~CSheetValue(void);	//185
	class CSheetValue &  operator=(class CSheetValue const &);	//225
	struct IUnknown *  GetUnknown(void);	//1131
	virtual long  InitNew(void);	//1188
	virtual long  Load(char const *,unsigned char *,int);	//1292
	virtual long  Save(char * *,unsigned char * *,int *);	//1690
	static long  _Cache(void *,struct _GUID const &,void * *,unsigned long);	//1918
//	static struct ATL::_ATL_INTMAP_ENTRY const *  _GetEntries(void);	//2022
	struct IUnknown *  _GetRawUnknown(void);	//2025
	long  _InternalQueryInterface(struct _GUID const &,void * *);	//2028
	virtual long  getType(class CType &);	//2137
	virtual long  get_Format(unsigned short * *);	//2138
	virtual long  get_Text(unsigned short * *);	//2139
	virtual long  get_Type(struct tagVARIANT *);	//2140
	virtual long  get_Value(struct tagVARIANT *);	//2141
	virtual long  putType(class CType const &);	//2225
	virtual long  put_Format(unsigned short *);	//2226
	virtual long  put_Text(unsigned short *);	//2227
	virtual long  put_Type(struct tagVARIANT);	//2228
	virtual long  put_Value(struct tagVARIANT,unsigned long);	//2229
};


class IMPORT_1C CSpecObjectFactory
{
public:
	 CSpecObjectFactory(struct _GUID const &,struct CRuntimeClass *,int,char const *);	//87
	virtual  ~CSpecObjectFactory(void);	//186
	virtual int  UpdateRegistry(int);	//1900
};


class IMPORT_1C CSpecPropertyPage:public COlePropertyPage
{
DECLARE_DYNCREATE(CSpecPropertyPage)

protected:
	 CSpecPropertyPage(void);	//88
public:
	 CSpecPropertyPage(unsigned int,unsigned int);	//89
	virtual  ~CSpecPropertyPage(void);	//187
protected:
	virtual struct AFX_INTERFACEMAP const *  GetInterfaceMap(void)const;	//761
	virtual long  OnApply(void);	//1337
	virtual int  OnCommand(unsigned int,long);	//1380
	virtual int  OnInitDialog(void);	//1439
	virtual int  PreTranslateMessage(struct tagMSG *);	//1618
	static struct AFX_INTERFACEMAP const *  _GetBaseInterfaceMap(void);	//1973
private:
	static struct AFX_INTERFACEMAP_ENTRY const * const  _interfaceEntries;	//2033
protected:
	static int  boolIsCentered;	//2082
	static struct AFX_INTERFACEMAP const  interfaceMap;	//2153
};


class IMPORT_1C CSplash
{
public:
	 CSplash(class CWnd *);	//90
	virtual  ~CSplash(void);	//188
//	void  `default constructor closure'(void);	//333
	int  Create(class CWnd *);	//441
	virtual int  DestroyWindow(void);	//539
protected:
	virtual void  DoDataExchange(class CDataExchange *);	//552
public:
	class CString   GetStatusLine(void)const;	//1100
protected:
	virtual void  OnCancel(void);	//1353
	int  OnCreate(struct tagCREATESTRUCTA *);	//1386
	void  OnDestroy(void);	//1401
	int  OnEraseBkgnd(class CDC *);	//1410
	virtual int  OnInitDialog(void);	//1440
	virtual void  OnOK(void);	//1506
	void  OnPaint(void);	//1522
	void  OnPaletteChanged(class CWnd *);	//1524
	int  OnQueryNewPalette(void);	//1526
public:
	void  SetStatusLine(char const *);	//1811
	void  Start(class CWnd *,unsigned long,int);	//1835
	void  StopIfTimeExceed(void);	//1843
	void  Update(void);	//1888

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSrcTemplDocParent:public CDocument
{
DECLARE_DYNCREATE(CSrcTemplDocParent)

public:
	 CSrcTemplDocParent(void);	//91
	virtual  ~CSrcTemplDocParent(void);	//189
	int  EditInWindow(class CItemList *);	//583
	void  FillTreeCtrl(class CTreeCtrl &,struct _TREEITEM *,int);	//635
protected:
	void  FillTreeLine(class CTreeCtrl &,class CItemList *,struct _TREEITEM *,int);	//636
public:
	char const *  GetData(class CItemList *);	//712
protected:
	class CItem *  GetDataItem(class CItemList *);	//714
public:
	unsigned long  GetID(class CItemList *);	//753
	char const *  GetKey(class CItemList *);	//766
protected:
	class CItem *  GetKeyItem(class CItemList *);	//767
	class CItem *  GetMemuItem(class CItemList *);	//782
public:
	int  GetMode(void);	//856
	char const *  GetName(class CItemList *);	//941
protected:
	class CItem *  GetNameItem(class CItemList *);	//942
	class CItem *  GetTOItem(class CItemList *);	//1104
	class CItem *  GetWindowItem(class CItemList *);	//1135
public:
	int  InMenu(class CItemList *);	//1180
	int  Initialize(int);	//1193
	int  IsGroup(class CItemList *);	//1220
protected:
	int  Lock(void);	//1303
public:
	virtual void  OnCloseDocument(void);	//1377
protected:
	void  RemoveEmpty(class CItemList *);	//1674
	void  RemoveExtra(class CItemList *);	//1675
public:
	virtual void  Serialize(class CArchive &);	//1725
protected:
	int  UnLock(void);	//1873
public:
	int  UseInTO(class CItemList *);	//1902
	int  VerifyNewList(class CItemList *);	//1904

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CStubContainerDocument
{
public:
	 CStubContainerDocument(void);	//92
	virtual  ~CStubContainerDocument(void);	//190
protected:
	static unsigned int  DocID;	//566
public:
	static unsigned int  GetDocID(void);	//722
protected:
	void  OnGetParam(class CMessage7 *,long *);	//1430
	void  OnSetParam(class CMessage7 *,long *);	//1556

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSubcContextDef:public CBLContext
{
DECLARE_DYNCREATE(CSubcContextDef)

public:
	 CSubcContextDef(class CSbCntTypeDef *);	//94
	virtual  ~CSubcContextDef(void);	//191
//	void  `default constructor closure'(void);	//334
	virtual char const *  GetMethodName(int,int)const;	//850
	virtual int  GetNMethods(void)const;	//885
	virtual int  GetNParams(int)const;	//911
	virtual int  GetNProps(void)const;	//934
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//971
	virtual char const *  GetPropName(int,int)const;	//1009
	virtual int  HasRetVal(int)const;	//1169
	virtual int  IsPropReadable(int)const;	//1252
	virtual int  IsPropWritable(int)const;	//1269
protected:
	int  PropPosToParamIdx(int)const;	//1636
public:
	void  SetSubcDef(class CSbCntTypeDef *);	//1814
};


class IMPORT_1C CSubcFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CSubcFormContextDef)

public:
	 CSubcFormContextDef(void);	//95
	virtual  ~CSubcFormContextDef(void);	//192
	virtual char const *  GetMethodName(int,int)const;	//851
	virtual int  GetNMethods(void)const;	//886
	virtual int  GetNParams(int)const;	//912
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//972
	virtual int  HasRetVal(int)const;	//1170
};


class IMPORT_1C CSubcItemFormContextDef:public CBLContext
{
DECLARE_DYNCREATE(CSubcItemFormContextDef)

public:
	 CSubcItemFormContextDef(void);	//96
	virtual  ~CSubcItemFormContextDef(void);	//193
	virtual char const *  GetMethodName(int,int)const;	//852
	virtual int  GetNMethods(void)const;	//887
	virtual int  GetNParams(int)const;	//913
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//973
	virtual int  HasRetVal(int)const;	//1171
};


class IMPORT_1C CTabbedCheckListBox
{
public:
	 CTabbedCheckListBox(void);	//97
	virtual  ~CTabbedCheckListBox(void);	//194
	int  AddString(char const *);	//359
	int  DeleteString(unsigned int);	//536
	virtual void  DrawItem(struct tagDRAWITEMSTRUCT *);	//581
	unsigned long  GetItemData(int);	//763
	void *  GetItemDataPtr(int);	//764
	class CString   GetTextFull(int);	//1118
	void  GetTextFull(int,class CString &);	//1119
	int  InsertString(int,char const *);	//1200
	void  ResetContent(void);	//1685
	void  SetItemData(int,unsigned long);	//1778
	void  SetItemDataPtr(int,void *);	//1779
	int  SetTabStops(int,int *);	//1815
protected:
	void  SplitString(class CString,class CString &,class CString &);	//1834

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTableCSV
{
public:
	 CTableCSV(int,class CString);	//98
	 ~CTableCSV(void);	//195
//	void  `default constructor closure'(void);	//335
	class CFieldCSV *  AddField(class CString,long,int);	//343
	int  AddRecord(void);	//358
	unsigned long  AddToFilteredFile(class CString,class CExpression *,long *,long);	//360
	int  Archive(class CString,class CDate,class CString);	//370
protected:
	int  AttachFetch(class CFetchCSV *);	//374
	void  CleanUp(void);	//410
public:
	int  CloseAccess(void);	//417
	unsigned long  CreateFetch(class CFetchCSV *);	//450
	unsigned long  CreateFilteredFile(class CString,class CExpression *,long *);	//451
protected:
	void  DecryptRawData(void);	//526
public:
	int  DeleteField(int);	//533
	int  DeleteField(class CFieldCSV *);	//534
protected:
	int  DetachFetch(class CFetchCSV *);	//540
	int  ExpandLineMap(void);	//593
public:
	virtual void  FX_Char(int,char &,int);	//598
	virtual void  FX_Char(class CFieldCSV *,char &,int);	//599
	virtual void  FX_Date(int,class CDate &,int);	//602
	virtual void  FX_Date(class CFieldCSV *,class CDate &,int);	//603
	virtual void  FX_Double(int,double &,int);	//606
	virtual void  FX_Double(class CFieldCSV *,double &,int);	//607
	virtual void  FX_Int(int,int &,int);	//610
	virtual void  FX_Int(class CFieldCSV *,int &,int);	//611
	virtual void  FX_Long(int,long &,int);	//614
	virtual void  FX_Long(class CFieldCSV *,long &,int);	//615
	virtual void  FX_LongDouble(int,long double &,int);	//618
	virtual void  FX_LongDouble(class CFieldCSV *,long double &,int);	//619
	virtual void  FX_String(int,class CString &,int);	//622
	virtual void  FX_String(class CFieldCSV *,class CString &,int);	//623
	virtual void  FX_Time(int,class CTime &,int);	//626
	virtual void  FX_Time(class CFieldCSV *,class CTime &,int);	//627
protected:
	void  FillCacheSegmentBackw(void);	//630
	void  FillCacheSegmentForw(unsigned long);	//631
public:
	long  FindNearDate(class CDate,class CFetchCSV *);	//648
	long  FindNearPos(long,class CFetchCSV *);	//650
	long  First(class CFetchCSV *);	//655
	long  GetBookmark(void);	//672
protected:
	void  GetCacheSegmentDate(class CDate *,class CDate *);	//675
public:
	static char  GetDivider(void);	//721
	class CFieldCSV *  GetField(int);	//729
protected:
	void  GetFieldBounds(int,int *,int *);	//731
public:
	class CString   GetFieldValue(int);	//733
	unsigned long  GetNFetches(void);	//863
	int  GetNFields(void)const;	//864
	long  GetNRecs(void);	//939
protected:
	void  GetRawFieldBounds(int,int *,int *);	//1025
	void  GetRecDate(char const *,class CDate *);	//1029
public:
	class CString   GetTableFile(void);	//1105
	unsigned long  GetTableSize(void);	//1107
	void  GetTimeInterval(class CDate *,class CDate *);	//1120
	int  Goto(class CCursorCSV &);	//1142
	long  GotoPos(long,class CFetchCSV *);	//1145
	void  GrowFetch(class CFetchCSV *);	//1146
	void  InitRecord(void);	//1192
	int  IsOpen(void)const;	//1229
	long  Last(class CFetchCSV *);	//1289
	long  Next(long,class CFetchCSV *);	//1327
	long  OpenAccess(class CString,int);	//1599
	long  Prev(long,class CFetchCSV *);	//1622
protected:
	void  ProcessFieldBounds(void);	//1629
public:
	static void  ProcessQuotes(class CString &);	//1631
	int  Refresh(void);	//1652
	int  RetrieveData(class CRecAddrCSV const *);	//1686
	int  RetrieveRecord(class CRecAddrCSV const *);	//1687
	void  SetNRecs(long);	//1787
	void  SetPassword(class CString);	//1793
protected:
	int  TestStringIntegrity(char const *);	//1862
private:
	static char  m_chDivider;	//2164
};


class IMPORT_1C CTagDocument:public CDocument
{
DECLARE_DYNCREATE(CTagDocument)

protected:
	 CTagDocument(void);	//99
public:
	virtual  ~CTagDocument(void);	//196
protected:
	void  AdditionalCript(void);	//365
	void  AdditionalDeCript(void);	//366
public:
	static unsigned int  DocID;	//567
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//737
public:
	static class CTagDocument *  GetTagDocument(void);	//1110
	class CString   GetTagString(char const *,char const *);	//1111
protected:
	virtual int  OnNewDocument(void);	//1497
	virtual void  ReleaseFile(class CFile *,int);	//1667
	virtual int  SaveModified(void);	//1701
public:
	virtual void  Serialize(class CArchive &);	//1726
	int  SetTagString(char const *,char const *);	//1816

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CToolTip7
{
public:
	 CToolTip7(void);	//100
	virtual  ~CToolTip7(void);	//197
	int  Create(class CWnd *,unsigned long);	//442
private:
	static class CPoint  MousePoint;	//1320
public:
	int buf[7];
	HWND m_ToolTipCtrl;
	virtual int  OnChildNotify(unsigned int,unsigned int,long,long *);	//1369
protected:
	void  OnPaint(void);	//1523
	void  OnShowWindow(int,unsigned int);	//1562
	long  OnWindowFromPoint(unsigned int,long);	//1593
	void  OnWindowPosChanging(struct tagWINDOWPOS *);	//1594
public:
	void  RelayEvent(struct tagMSG *);	//1660
	static struct HWND__ *  TT7ChildFromPoint(struct HWND__ *,struct tagPOINT);	//1852
	static int  TT7IsBrowser(struct HWND__ *);	//1853
	static int  TT7IsComboBox(struct HWND__ *,unsigned int);	//1854
	static int  TT7IsMaskEdit(struct HWND__ *);	//1855
	static int  TT7IsStatic(struct HWND__ *);	//1856

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTopLevelMoxelContextDef
{
public:
	 CTopLevelMoxelContextDef(void);	//101
	virtual  ~CTopLevelMoxelContextDef(void);	//198
	virtual int  GetNProps(void)const;	//935
	virtual char const *  GetPropName(int,int)const;	//1010
	virtual int  IsPropReadable(int)const;	//1253
	virtual int  IsPropWritable(int)const;	//1270
};


class IMPORT_1C CTradeContextDef:public CBLContext
{
DECLARE_DYNCREATE(CTradeContextDef)

public:
	 CTradeContextDef(void);	//102
	virtual  ~CTradeContextDef(void);	//199
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//393
	virtual int  CallAsProc(int,class CValue * *);	//399
	virtual char const *  GetMethodName(int,int)const;	//853
	virtual int  GetNMethods(void)const;	//888
	virtual int  GetNParams(int)const;	//914
	virtual int  GetNProps(void)const;	//936
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//974
	virtual char const *  GetPropName(int,int)const;	//1011
	virtual int  GetPropVal(int,class CValue &)const;	//1019
	virtual int  HasRetVal(int)const;	//1172
	virtual int  IsPropReadable(int)const;	//1254
};


class IMPORT_1C CTxtModuleReader:public CDocument
{
DECLARE_DYNCREATE(CTxtModuleReader)

protected:
	 CTxtModuleReader(void);	//103
public:
	virtual  ~CTxtModuleReader(void);	//200
	int  AssignAndCompile(class CBLModule *);	//372
CString module;
CBLModule7 pPreCompileModule;
protected:
	virtual class CFile *  GetFile(char const *,unsigned int,class CFileException *);	//738
public:
	void  GetText(class CString &);	//1114
	int  IsCasheEmpty(void);	//1210
protected:
	virtual void  ReleaseFile(class CFile *,int);	//1668
	virtual int  SaveModified(void);	//1702
public:
	virtual void  Serialize(class CArchive &);	//1727

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTypedCont:public CContainer
{
DECLARE_DYNCREATE(CTypedCont)

protected:
	 CTypedCont(void);	//104
public:

	DWORD dwBuf[0x8C];

	virtual  ~CTypedCont(void);	//201
	class CDocument *  FindDocument(char const *,int *);	//645
	class CDocument *  GetDEditDocument(void);	//707
	class CDocument *  GetMoxelDocument(char const *,int *);	//862
	int  GetTablesName(class CStringArray &);	//1108
	class CDocument *  GetTextDocument(void);	//1116
	class CWorkBookDoc *  GetWorkBook(void);	//1137
	virtual int  OnLoadInternal(void);	//1467
	virtual int  OnNewDocument(void);	//1498
	virtual int  OnOpenDocument(char const *);	//1513
	virtual int  OnSaveDocument(char const *);	//1535
	virtual int  OnSaveInternal(void);	//1538
	virtual int  SaveModified(void);	//1703
	void  ShowDocument(int);	//1828

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTypedDataCont:public CContainer
{
DECLARE_DYNCREATE(CTypedDataCont)

protected:
	 CTypedDataCont(void);	//105
public:
	virtual  ~CTypedDataCont(void);	//202
	virtual int  OnLoadInternal(void);	//1468
	virtual int  OnOpenDocument(char const *);	//1514
	virtual int  OnSaveDocument(char const *);	//1536
	virtual int  OnSaveInternal(void);	//1539
	virtual int  SaveModified(void);	//1704

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CTypedTextCont:public CContainer
{
DECLARE_DYNCREATE(CTypedTextCont)

protected:
	 CTypedTextCont(void);	//106
public:
	virtual  ~CTypedTextCont(void);	//203
	virtual int  OnNewDocument(void);	//1499
	virtual int  OnOpenDocument(char const *);	//1515
	virtual int  SaveModified(void);	//1705

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CUserCommandContaner
{
public:
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


class IMPORT_1C CUserCommandInfo
{
public:
	 CUserCommandInfo(class CUserCommandInfo const &);	//108
	 CUserCommandInfo(unsigned int,char const *);	//109
	 CUserCommandInfo(void);	//110
	 ~CUserCommandInfo(void);	//205
	class CUserCommandInfo &  operator=(class CUserCommandInfo const &);	//228
	struct tagACCEL   GetAccel(void)const;	//662
	int  GetParamCode(class CString &,int)const;	//949
	int  GetParamCode(char *,int,int)const;	//950
};


class IMPORT_1C CUserDefCont:public CContainer
{
DECLARE_DYNCREATE(CUserDefCont)

protected:
	 CUserDefCont(void);	//111
public:
	virtual  ~CUserDefCont(void);	//206
	static unsigned int  DocID;	//568
	static class CUserDefCont *  GetContainer(void);	//690
protected:
	virtual int  OnNewDocument(void);	//1500
	virtual int  SaveModified(void);	//1706

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CUserSkipper
{
public:
	 CUserSkipper(void);	//112
	virtual  ~CUserSkipper(void);	//207
	long  GetCurPos(void);	//699
	virtual long  GetIndex(void);	//758
	virtual int  Ok(void);	//1334
	void  SetDoc(class CMonitorDoc *);	//1753
	virtual void  SetIndex(long);	//1775
	virtual void  SetMinMaxIndex(void);	//1785
	void  Update(void);	//1889
	virtual long  getdba(void);	//2144
	virtual long  getpos(void);	//2146
	virtual void  gobot(void);	//2148
	virtual void  godba(long);	//2150
	virtual void  gotop(void);	//2152
	virtual int  next(void);	//2220
	virtual int  prev(void);	//2224
	virtual long  setpos(void);	//2235
	virtual int  skip(int);	//2237
};


class IMPORT_1C CWorkBookDoc:public CContainer
{
DECLARE_DYNCREATE(CWorkBookDoc)
public:
	friend class CICfgDocuments;
	virtual void  OnChangedViewList(void);	//1365
	virtual void  OnCloseDocument(void);	//1378
	virtual int  OnNewDocument(void);	//1501
	virtual int  OnOpenDocument(char const *);	//1516
	virtual int  SaveModified(void);	//1707
	virtual void  SetPathName(char const *,int);	//1795
	virtual void  SetTitle(char const *);	//1820
	virtual struct HMENU__ *  GetDefaultMenu(void);	//717
	virtual int  OnLoadInternal(void);	//1469
	virtual int  OnSaveInternal(void);	//1540

	virtual  ~CWorkBookDoc(void);	//208
	class CDocument *  AddPage(unsigned int,char const *);	//353
	int  CopyStreamFrom(class CWorkBookDoc *,int,int);	//430
	int  CreateWorkBookFrame(class CTemplate7 *,int);	//518
	virtual int  DoFileSave(void);	//555
	int  ExportDescription(char const *,long);	//594
	class CDocument *  FindDocument(char const *,int *);	//646
	class CDocument *  GetActiveDocument(void);	//666
	class CDocument *  GetDEditDocument(void);	//708
	static struct HICON__ *  GetFrameIcon(unsigned int);	//741
	class CImageList *  GetImageList(void);	//755
	void  GetInfo(long &,class CString &)const;	//760
	class CPictureCont *  GetPictCont(void);	//986
	class CString const &  GetSourcePathName(void)const;	//1095
	int  GetTablesName(class CStringArray &);	//1109
	class CDocument *  GetTextDocument(void);	//1117
	int  InsertDescription(void);	//1194
	int  InsertMDHolder(void);	//1197
	static int  IsAddToMRUEnable(void);	//1205
	int  IsPasswordDisable(void)const;	//1233
	int  IsSingleMode(void)const;	//1275
	class CProfile7 &  Profile(void);	//1633
	class CProfile7 const &  Profile(void)const;	//1634
	int  RemoveDescription(void);	//1673
	int  RemoveMDHolder(void);	//1677
	static void  SetEnableAddToMRU(int);	//1755
	void  SetInfo(long const &,class CString const &);	//1776
	void  SetPasswordDisable(int);	//1794
	void  SetSingleMode(int);	//1806
	void  SetSourcePathName(char const *);	//1807
	int  SwitchToPage(int);	//1851
	void  UpdateMoxelHook(void);	//1899

protected:
	 CWorkBookDoc(void);	//113
	void  LoadExtRepPictGallery(void);	//1296
	void  OnFileClose(void);	//1416
	int  OnFileSaveAs(void);	//1419
	void  OnFileSaveAsHandler(void);	//1420
	void  OnUpdateSaveAs(class CCmdUI *);	//1589
	void  SaveExtRepPictGallery(void);	//1692
	static int  m_bEnableAddToMRU;	//2163

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CXBaseContext:public CBLContext
{
DECLARE_DYNCREATE(CXBaseContext)

public:
	 CXBaseContext(void);	//114
	virtual  ~CXBaseContext(void);	//209
protected:
	void  AddField(class CValue * *);	//344
private:
	void  AddFld(char const *,int,int,int);	//345
	void  AddInd(char const *,char const *,int,int,char const *);	//349
protected:
	void  AddIndex(class CValue * *);	//350
	void  AddRec(class CValue * *);	//357
	void  Bof(class CValue &,class CValue * *);	//383
public:
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//394
	virtual int  CallAsProc(int,class CValue * *);	//400
private:
	void  Cancel(void);	//402
	void  CheckCBError(void);	//405
protected:
	void  ClearRec(class CValue * *);	//414
	void  CloseBase(class CValue * *);	//418
private:
	void  CloseDB(void);	//419
protected:
	void  CopyRec(class CValue * *);	//429
	void  CreateBase(class CValue * *);	//443
private:
	void  CreateIndex(class CValue &,class CValue * *);	//454
protected:
	void  DelRec(class CValue * *);	//532
	void  Eof(class CValue &,class CValue * *);	//589
	void  FieldNumber(class CValue &,class CValue * *);	//629
	void  FindByKey(class CValue &,class CValue * *);	//643
	void  FindRec(class CValue &,class CValue * *);	//651
	void  FirstRec(class CValue &,class CValue * *);	//657
public:
	int  GetCodePage(void)const;	//682
protected:
	void  GetField(class CValue &,class CValue * *);	//730
	void  GetFieldInfo(class CValue * *);	//732
private:
	void  GetFld(int,class CValue &)const;	//739
protected:
	void  GetIndexInfo(class CValue * *);	//759
public:
	void  GetKeyVal(int,class CValue &);	//768
	virtual char const *  GetMethodName(int,int)const;	//854
	virtual int  GetNMethods(void)const;	//889
	virtual int  GetNParams(int)const;	//915
	virtual int  GetNProps(void)const;	//937
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//975
	virtual char const *  GetPropName(int,int)const;	//1012
	virtual int  GetPropVal(int,class CValue &)const;	//1020
	virtual char const *  GetTypeString(void)const;	//1127
protected:
	void  Goto(class CValue * *);	//1143
public:
	virtual int  HasRetVal(int)const;	//1173
	virtual int  IsPropReadable(int)const;	//1255
	virtual int  IsPropWritable(int)const;	//1271
protected:
	void  LastRec(class CValue &,class CValue * *);	//1290
	void  NextRec(class CValue &,class CValue * *);	//1329
	void  OpenBase(class CValue * *);	//1600
private:
	void  OpenDB(char *,char *,int);	//1601
protected:
	void  Pack(class CValue * *);	//1603
	void  PrevRec(class CValue &,class CValue * *);	//1624
private:
	void  ReadFld(int,class CValue &,int)const;	//1639
protected:
	void  RecCount(class CValue &,class CValue * *);	//1643
	void  RecDeleted(class CValue &,class CValue * *);	//1644
	void  RecNo(class CValue &,class CValue * *);	//1645
	void  RecallRec(class CValue * *);	//1647
	void  Reindex(class CValue * *);	//1659
private:
	void  ReleaseFieldInfo(void);	//1663
	void  ReleaseIndexInfo(void);	//1669
	void  Save(void);	//1691
protected:
	void  SelectIndex(class CValue &,class CValue * *);	//1713
	void  SetCodePage(class CValue &,class CValue * *);	//1740
	void  SetField(class CValue * *);	//1766
private:
	void  SetFld(int,class CValue const &);	//1767
public:
	void  SetKeyVal(int,class CValue);	//1781
	virtual int  SetPropVal(int,class CValue const &);	//1802
private:
	int  SkipDeleted(int);	//1832
	void  WriteFld(int,class CValue const &,int);	//1907
protected:
	void  Zap(class CValue * *);	//1914
private:
	static int  s_DBCount;	//2230
	static char const * const  s_DBaseTypes;	//2231
	static struct SMethodDescr *  s_MethodDescr;	//2232
	static class CODE4 *  s_pCodeBase;	//2233
};


class IMPORT_1C CXBaseKeyContext:public CBLContext
{
DECLARE_DYNCREATE(CXBaseKeyContext)

public:
	 CXBaseKeyContext(void);	//115
	virtual  ~CXBaseKeyContext(void);	//210
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//395
	virtual int  CallAsProc(int,class CValue * *);	//401
	void  Done(void);	//571
	virtual char const *  GetMethodName(int,int)const;	//855
	virtual int  GetNMethods(void)const;	//890
	virtual int  GetNParams(int)const;	//916
	virtual int  GetNProps(void)const;	//938
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//976
	virtual char const *  GetPropName(int,int)const;	//1013
	virtual int  GetPropVal(int,class CValue &)const;	//1021
	virtual int  HasRetVal(int)const;	//1174
	void  Init(class CXBaseContext *);	//1183
	virtual int  IsPropReadable(int)const;	//1256
	virtual int  IsPropWritable(int)const;	//1272
	virtual int  SetPropVal(int,class CValue const &);	//1803
};


class IMPORT_1C CConfigInterface
{
public:
	class CConfigInterface &  operator=(class CConfigInterface const &);	//214
	static void  Close(void);	//415
	static class CConfigDataMap *  GetData(void);	//709
	static int  IsConfigActive(void);	//1212
	static int  IsOpen(void);	//1228
	static class CConfigDataMap *  Open(void);	//1597
	static void  ReleaseData(void);	//1661
	static void  SendCommandToConfig(void);	//1714
private:
	static class CConfigMutex  m_DataMutex;	//2156
	static void *  m_hFileMap;	//2166
	static class CConfigDataMap *  m_pData;	//2169
};


class IMPORT_1C CTraceInterface
{
public:
	class CTraceInterface &  operator=(class CTraceInterface const &);	//227
	static void  Close(void);	//416
	static void  GetBreakPoints(class CBLModule7 const *);	//673
	static class CTraceDataMap *  GetData(void);	//713
	static unsigned int  GetLineStoped(void);	//772
	static char const *  GetProfileDefFileName(void);	//989
	static int  IsBreakPointOnLine(unsigned int,unsigned int &);	//1208
	static int  IsOpen(void);	//1230
	static int  IsProfilerOn(void);	//1236
	static int  IsTraced(void);	//1278
	static int  IsTraceeActive(void);	//1279
	static int  IsTracerActive(void);	//1280
	static class CTraceDataMap *  Open(void);	//1598
	static void  ReleaseData(void);	//1662
	static void  SendCommandToTracer(void);	//1715
private:
	static class CMutex  m_DataMutex;	//2157
	static void *  m_hFileMap;	//2167
	static class CTraceDataMap *  m_pData;	//2170
};

// End class descriptions
// Begin free functions definitions
IMPORT_1C void __cdecl BookdocsFillOleFileException(class CFileException *,long);
IMPORT_1C void __cdecl BookdocsThrowOleFileException(long,char const *);
IMPORT_1C int __cdecl CheckPasswordString(class CString,int,int,char const *);
IMPORT_1C void __cdecl CorrectCommand(class CString &,class CString &,class CString &);
IMPORT_1C void __cdecl CorrectCommand(class CString &,class CString &);
IMPORT_1C class CString  __cdecl CriptPasswordString(char const *);
IMPORT_1C void __cdecl DefineAccountContext(void);
IMPORT_1C void __cdecl DefineCalcContext(void);
IMPORT_1C void __cdecl DefineGeneralContext(void);
IMPORT_1C void __cdecl DefineTradeContext(void);
IMPORT_1C int __cdecl DlgDateButton(class CWnd *,unsigned int,class CDate *);
IMPORT_1C int __cdecl DlgNumericButton(class CWnd *,unsigned int,class CNumeric &,int,int);
IMPORT_1C void __cdecl FillCombo(class CComboBox *);
IMPORT_1C void __cdecl ForAll(int (__cdecl*)(class CMetaDataObj *,void *,long &),int (__cdecl*)(unsigned int,void *,long &),void *,long &);
IMPORT_1C class CString  __cdecl GetContainerDescription(char const *);
IMPORT_1C int __cdecl GetContextProp(class CBLContext *,class CValue &,class CValue * *);
IMPORT_1C class CDocType * __cdecl GetGlobalDocType(void);
IMPORT_1C class CBLModule7 * __cdecl GetGlobalModule(void);
IMPORT_1C class CString  __cdecl GetLastPPPString(void);
IMPORT_1C class CString  __cdecl GetModuleFullName(long,char const *,int,class CMetaDataCont *);
IMPORT_1C int __cdecl GetPasswordString(class CString &,char const *,int);
IMPORT_1C class CSearchHistoryProfile * __cdecl GetReplaceHistory(void);
IMPORT_1C class CSplash * __cdecl GetSplashWnd(void);
IMPORT_1C class CType  __cdecl GetTypeFromValue(class CValue *,int,int,int *,int);
IMPORT_1C class CString  __cdecl HashPasswordString(char const *);
IMPORT_1C void __cdecl InitBasicExtContexts(void);
IMPORT_1C int __cdecl InitGlobalModule(void);
IMPORT_1C int __cdecl IsAccountContextDefined(void);
IMPORT_1C int __cdecl IsCalcContextDefined(void);
IMPORT_1C int __cdecl IsGeneralContextDefined(void);
IMPORT_1C int __cdecl IsTradeContextDefined(void);
IMPORT_1C int __cdecl IsTypeAvailable(class CType const &,unsigned int);
IMPORT_1C int __cdecl IsValid(char const *,char const *);
IMPORT_1C int __cdecl ModalMode(void);
IMPORT_1C int __cdecl OnFileCommand(unsigned int,int (__cdecl*)(class CDocument *,long),long);
IMPORT_1C void __cdecl RT_ActiveBarcodeLicense(void);
IMPORT_1C class CString  __cdecl ReadStatusLine(void);
IMPORT_1C void __cdecl ReportEventA(class CString,long,enum EventType,class CString,class CString,class CString);
IMPORT_1C void __cdecl ReportEventA(class CString,struct LogEventInfo *);
IMPORT_1C int __cdecl SetContextProp(class CBLContext *,class CValue * *);
IMPORT_1C int __cdecl SetModalMode(int);
IMPORT_1C int __cdecl SetTypedValueWithTest(class CTypedValue *,class CValue const *);
IMPORT_1C int __cdecl TestSetTypedValue(class CType const *,class CValue const *);
IMPORT_1C void __cdecl UndefineAccountContext(void);
IMPORT_1C void __cdecl UndefineCalcContext(void);
IMPORT_1C void __cdecl UndefineGeneralContext(void);
IMPORT_1C void __cdecl UndefineTradeContext(void);
IMPORT_1C void __cdecl UndoBasicExtContexts(void);
IMPORT_1C void __cdecl UndoGlobalModule(void);
IMPORT_1C void __cdecl WriteStatusLine(unsigned int);
IMPORT_1C void __cdecl WriteStatusLine(char const *);
IMPORT_1C long __cdecl atox(char const *);
IMPORT_1C int __cdecl bin2uue(void const *,int,void *,int);
IMPORT_1C int __cdecl uue2bin(void const *,int,void *,int);
// End free functions definitions
//const  CSheetPluginPage::`vftable'{for `CMFCRoot<class CDialog,class ATL::CComObjectRootEx<class ATL::CComMultiThreadModel> >'}
//const  CSheetPluginPage::`vftable'{for `ATL::IPropertyPageImpl<class CSheetPluginPage>'}
//const  CSheetValue::`vftable'{for `IPersistCell'}
//const  CSheetValue::`vftable'{for `ISheetValue'}
//??_C@_05JNEO@XBASE?$AA@
IMPORT_1C extern struct _GUID  CLSID_AdminPropPage;
//struct ATL::_ATL_INTMAP_ENTRY const * const  `public: static struct ATL::_ATL_INTMAP_ENTRY const * __stdcall CSheetPluginDT::_GetEntries(void)'::`2'::_entries
//struct ATL::_ATL_INTMAP_ENTRY const * const  `public: static struct ATL::_ATL_INTMAP_ENTRY const * __stdcall CSheetPluginPage::_GetEntries(void)'::`2'::_entries
//struct ATL::_ATL_INTMAP_ENTRY const * const  `public: static struct ATL::_ATL_INTMAP_ENTRY const * __stdcall CSheetValue::_GetEntries(void)'::`2'::_entries
IMPORT_1C extern int  numberCodes;
IMPORT_1C extern unsigned int *  vkCodes;
IMPORT_1C extern char const * *  vkNames;
