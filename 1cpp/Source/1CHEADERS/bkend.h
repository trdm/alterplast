// Begin forward declarations

//typedef CTypedPtrArray<CPtrArray, CValue*> CValuePtrArray;
class CValuePtrArray : public CTypedPtrArray<CPtrArray, CValue*>
{
public:
	// Direct Access to the element data (may return NULL)
	const CValue** GetData() const
	{
		return reinterpret_cast<const CValue**>(CTypedPtrArray<CPtrArray, CValue*>::GetData());
	}
	CValue** GetData()
	{
		return reinterpret_cast<CValue**>(CTypedPtrArray<CPtrArray, CValue*>::GetData());
	}
	
};

enum MessageMarker
{
	mmNone = 0,
	mmBlueTriangle,
	mmExclamation,
	mmExclamation2,
	mmExclamation3,
	mmInformation,
	mmBlackErr,
	mmRedErr,
	mmMetaData,
	mmUnderlinedErr
};
enum PeriodType
{
eDay = 0,
eWeek,
eMonth,
eQuart,
eYear,
e_NotUsed,
eTenDays,
eFiveDays,
eFifteenDays,
e_Document,
};
class _std;
struct SAccSbKindInfo
{
public:
	 SAccSbKindInfo(struct SAccSbKindInfo const &);	//326
	 SAccSbKindInfo(long,int,int,int,int);	//327
	 SAccSbKindInfo(void);	//328
	struct SAccSbKindInfo &  operator=(struct SAccSbKindInfo const &);	//595
	struct SAccSbKindInfo const &  operator=(CAccSbCntDef const &);	//596
};
struct SAccountInfo
{
public:
	 SAccountInfo(struct SAccountInfo const &);	//329
	 SAccountInfo(CObjID const &,int,int,int,int);	//330
	 SAccountInfo(void);	//331
	struct SAccountInfo &  operator=(struct SAccountInfo const &);	//597
	struct SAccountInfo &  operator=(CAccSet const &);	//598
};
struct SClearActsRule
{
public:
	 SClearActsRule(void);	//332
	 ~SClearActsRule(void);	//495
	struct SClearActsRule &  operator=(struct SClearActsRule const &);	//599
};
struct SSbAccountInfo
{
public:
	 SSbAccountInfo(struct SSbAccountInfo const &);	//333
	 SSbAccountInfo(CObjID const &,int,int,int,int,struct SAccSbKindInfo *);	//334
	 SSbAccountInfo(void);	//335
	 ~SSbAccountInfo(void);	//496
	struct SSbAccountInfo &  operator=(struct SSbAccountInfo const &);	//600
	struct SSbAccountInfo &  operator=(CAccSet const &);	//601
	int  FreeSbCnts(void);	//1383
	int  SplitSbCnts(void);	//3854
};
struct SSbKindInfo
{
public:
	 SSbKindInfo(struct SSbKindInfo const &);	//336
	 SSbKindInfo(long,int);	//337
	 SSbKindInfo(void);	//338
	struct SSbKindInfo &  operator=(struct SSbKindInfo const &);	//602
	struct SSbKindInfo const &  operator=(CAccSbCntDef const &);	//603
};
// End forward declarations
// Begin class descriptions

// orefkov тип объекта метаданных (константа, справочник, документ и т.д.)
/*
		// Типы объектов метаданных
		[helpstring("Типы объектов метаданных")]
		typedef enum
		{
		[helpstring("Константа")]				tmdConstant=  1,
		[helpstring("Реквизит справочника")]	tmdRefRekv=  2,
		[helpstring("Справочник")]				tmdReference=  3,
		[helpstring("Измерение регистра")]		tmdRegIzm=  4,
		[helpstring("Ресурс регистра")]			tmdRegResurs=  5,
		[helpstring("Регистр")]					tmdRegister=  6,
		[helpstring("Реквизит шапки документа")]	tmdDocHeadRekv=  7,
		[helpstring("Реквизит ТЧ документа")]	tmdDocTableRekv=  8,
		[helpstring("Документ")]				tmdDocument=  9,
		[helpstring("Графа журнала документов")]	tmdJDGrafa= 10,
		[helpstring("Журнал документов")]		tmdJournalDoc= 11,
		[helpstring("Значение перечисления")]	tmdEnumVal= 12,
		[helpstring("Перечисление")]			tmdEnum= 13,
		[helpstring("Список ссылок")]			tmdListRefs= 14,
		[helpstring("Форма списка")]			tmdFormList= 15,
		[helpstring("Отчет")]					tmdReport= 16,
		[helpstring("Конфигурация")]			tmdTask= 17,
		[helpstring("Календарь")]				tmdKalendar= 18,
		[helpstring("Вид расчета")]				tmdCalcKind= 19,
		[helpstring("Группа расчетов")]			tmdCalcGroup= 20,
		[helpstring("Реквизит журнала расчетов")]	tmdCJRekv= 21,
		[helpstring("Журнал расчетов")]			tmdCalcJournal= 22,
		[helpstring("Обработка")]				tmdCalcVar= 23,
		[helpstring("Реквизит регистра")]		tmdRegRekv= 25,
		[helpstring("Общий реквизит документов")]	tmdCommonRekv= 26,
		[helpstring("План счетов")]				tmdAccountChart= 27,
		[helpstring("Реквизит счета")]			tmdAccRekv= 28,
		[helpstring("Счет")]					tmdAcount= 30,
		[helpstring("Субконто счета")]			tmdAccountSubc= 31,
		[helpstring("Реквизит проводки")]		tmdProvRekv= 32,
		[helpstring("Реквизит операции")]		tmdOperRekv= 33,
		[helpstring("Графа отбора")]			tmdSelGraf= 34,
		[helpstring("Нумератор")]				tmdNumbers= 35,
		[helpstring("Вид субконто")]			tmdSubconto= 36,
		[helpstring("Последовательность")]		tmdStream= 43,
		[helpstring("Правило перерасчета")]		tmdRecalcRule= 44,
		}TypeOfMetaDataObj;
*/
enum TypeOfMetaDataObj
{
	tmdConstant= 1,
	tmdRefRekv= 2,
	tmdReference= 3,
	tmdRegIzm= 4,
	tmdRegResurs= 5,
	tmdRegister= 6,
	tmdDocHeadRekv= 7,
	tmdDocTableRekv= 8,
	tmdDocument= 9,
	tmdJDGrafa= 10,
	tmdJournalDoc= 11,
	tmdEnumVal= 12,
	tmdEnum= 13,
	tmdListRefs= 14,
	tmdFormList= 15,
	tmdReport= 16,
	tmdTask= 17,
	tmdKalendar= 18,
	tmdCalcKind= 19,
	tmdCalcGroup= 20,
	tmdCJRekv= 21,
	tmdCalcJournal= 22,
	tmdCalcVar= 23,
	tmdRegRekv= 25,
	tmdCommonRekv= 26,
	tmdAccountChart= 27,
	tmdAccRekv= 28,
	tmdAcount= 30,
	tmdAccountSubc= 31,
	tmdProvRekv= 32,
	tmdOperRekv= 33,
	tmdSelGraf= 34,
	tmdNumbers= 35,
	tmdSubconto= 36,
	tmdTypOpers =  37,
    tmdOpers = 40,
    tmdProvs = 41,
	tmdStream= 43,
	tmdRecalcRule= 44
};

class IMPORT_1C CMetaDataObj
{
public:
long m_ID; // ИД метаданного
char * m_Code; // Вид метаданного, например, вид документа или справочника
char * m_Present;
char * m_Descr;
	CMetaDataObj(CMetaDataObj const &);
	CMetaDataObj(long,char const *,char const *);
	virtual  ~CMetaDataObj(void);									//1
	virtual void	LoadFromList(CItemList const&, int&);			//2
	virtual void	AddToList(CItemList&) const;					//3
	virtual int		WhatIsIt(void)const=0;							//4
	virtual int		IsTypedObj(void)const;							//5
	virtual int		IsRefObj(void)const;							//6
	virtual void	DescribeObject(CString&,CMetaDataCont*, CString,int);	//7
	virtual void	ScanMDObjects(SScanMDObjects*);					//8
	virtual int		GetNProps(void)const;							//9
	virtual LPCTSTR GetPropName(int,int)const;						//10
	virtual int		GetPropVal(int,CMetaDataCont*,CValue&,CMetaDataObj **)const;	//11
	virtual int		GetNChilds(void)const;							//12
	virtual LPCTSTR	GetChildName(int,int)const;						//13
	virtual CMetaDataObjArray*  GetChild(int,CMetaDataCont*);		//14

	CMetaDataObj &  operator=(CMetaDataObj const &);	//541
//	void  `default constructor closure'(void);	//768
	char const *  GetCode(void)const;	//1545
	char const *  GetDescr(void)const;	//1655
	long  GetID(void)const;	//1805
	char const *  GetPresent(void)const;	//2160
	char const *  GetRealPresent(void)const;	//2285
	void  SetCode(char const *);	//3499
	void  SetDescr(char const *);	//3555
	void  SetID(long);	//3609
	void  SetPresent(char const *);	//3690
	static int  ValidateCode(char const *,int);	//4033
};

class IMPORT_1C CFldDef:public CMetaDataObj
//DmitrO: определение класса перенесено для организации наследования
{
public:
	 CFldDef(CFldDef const &);	//137
	 CFldDef(CItemList const &);	//138
	 CFldDef(long,char const *,char const *,CType const &);	//139
	 CFldDef(CMetaDataTypedObj const *);	//140
	virtual  ~CFldDef(void);	//404
	CFldDef &  operator=(CFldDef const &);	//534
	virtual void  AddToList(CItemList &)const;	//873
	int  GetHasIndex(void)const;	//1786
	static char const *  GetListFunctor(void);	//1902
	virtual int  GetNProps(void)const;	//2027
	virtual char const *  GetPropName(int,int)const;	//2198
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2232
	void  SetHasIndex(int);	//3599
	virtual int  WhatIsIt(void)const;	//4093
};

class IMPORT_1C CMetaDataTypedObj:public CMetaDataObj
{
public:
	 CMetaDataTypedObj(CMetaDataTypedObj const &);	//180
	 CMetaDataTypedObj(long,char const *,char const *,CType const &);	//181
	 CMetaDataTypedObj(CMetaDataTypedObj const *);	//182
	virtual  ~CMetaDataTypedObj(void);	//426
	virtual void  AddToList(CItemList &)const;	//883
	virtual int  GetNProps(void)const;	//2034
	virtual char const *  GetPropName(int,int)const;	//2206
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2239
	virtual int  IsTypedObj(void)const;	//2884
	virtual void  LoadFromList(CItemList const &,int &);	//2942

	virtual char const *  GetFieldName(void)const;	//1732
	virtual char const *  GetTDFieldName(void)const;	//2459
	virtual int  IsDateDep(void)const;	//2795

	CMetaDataTypedObj &  operator=(CMetaDataTypedObj const &);	//544
//	void  `default constructor closure'(void);	//773
	CType const &  GetType(void)const;	//2498
	int  IsBlob(void)const;	//2785
	int  IsSeparated(void)const;	//2873
	void  SetType(CType const &);	//3781
};


class IMPORT_1C CMetaDataRefObj:public CMetaDataObj
{
public:
	CMetaDataRefObj(CItemList const &);	//177
	CMetaDataRefObj(long,char const *,char const *);	//178
	virtual  ~CMetaDataRefObj(void);	//424
	virtual void  AddToList(CItemList &)const;	//882
	virtual int  IsRefObj(void)const;	//2870
	virtual int  WhatIsIt(void)const;	//4100

	//	void  `default constructor closure'(void);	//772
	void  Accept(CMetaDataCont *);	//790
	int	  AddRefDef(CMetaDataRef *);	//845
	void  DelAllRefDefs(int);	//1112
	void  DelRefDef(int,int);	//1154
	void  DelRefDef(CMetaDataRef *,int);	//1155
	int  GetNRefDefs(void)const;	//2047
	CMetaDataRef *  GetRefDef(long)const;	//2297
	CMetaDataRef *  GetRefDefAt(int)const;	//2298
	int  GetRefDefIdx(long)const;	//2299
	int  GetRefDefIdx(CMetaDataRef *)const;	//2300
	CType   GetType(void);	//2497
	int  InsertRefDef(int,CMetaDataRef *);	//2768
	static char const *  GetListFunctor(void);	//1913
};

class IMPORT_1C CObjectSet:public CObject
{
DECLARE_DYNAMIC(CObjectSet)

public:
	DWORD m_dwFlag1;
	DWORD m_dwFlag2;
	CODBCDB7* m_pODBCDB7;
	DWORD m_dwFlag4;
	DWORD m_dwFlag5;
	CStoreObj* m_pCurObj;//CObjectSetCur* m_pCurObj; //_Unkn_2020C0E8;
	DWORD m_dwFlag7;
	DWORD m_dwFlag8;
	CMetaDataObj* m_pMetaDataObj;
	DWORD m_dwFlag10;
	DWORD* m_dwArray; //m_dwArray[4] = *CStoreObj
	DWORD dwObjSet [0x09];

	 CObjectSet(CDataBase7 *);	//195
	virtual  ~CObjectSet(void);	//434
//	void  `default constructor closure'(void);	//776
	void  EnableRegisterUpdate(int);	//1299
	int  GetCalcObjIDOnNew(void);	//1495
	int  GetCurOrder(void)const;	//1584
	int  GetCurSelection(void)const;	//1586
	CDataBase7 *  GetDB7(void)const;	//1604
	int  IsRegisterUpdateEnabled(void)const;	//2871
	int  IsUploadAvailable(long,CObjID const &,CDBSign,int);	//2886
	void  RegisterDelete(long,CObjID const &);	//3196
	void  RegisterUpdate(long,CObjID const &);	//3206
	void  SetCalcObjIDOnNew(int);	//3492
};


class IMPORT_1C CUniSet:public CObjectSet
{
DECLARE_DYNAMIC(CUniSet)

public:
	DWORD dwUniSet;
	//char buff[4000];

	 CUniSet(CDataBase7 *);	//307
	virtual  ~CUniSet(void);	//486

  CSQLRec *  GetMainSQLRec(void)const;  //1946
  CStoreObj *  GetMainRec(void)const;   //1945
  virtual int  Delete(enum MoveAfterDel);   //1187
  virtual int  GotoCurrent(void);   //2565
  virtual int  GotoFirst(void);   //2582
  virtual int  GotoLast(void);  //2599
  virtual int  GotoNext(void);  //2614
  virtual int  GotoPrev(void);  //2648
  virtual int  IsLocked(void);  //2817
  virtual int  IsMultSel(void) = 0;
  virtual int  IsOK(void);  //2844
  virtual int  Lock(void);  //2974    // +60h
  virtual int  New(void);   //3043
  virtual int  ReSelect(int);   //3144
  virtual int  SetSelectionFields(void) = 0;
  virtual int  TestSelectionFields(void) = 0;
  virtual int  Unlock(void);  //3970    // +64h
  virtual int  Update(void);  //3996
  virtual long  GetCurrentPos(void);  //1601
  virtual long  GetNearPos(enum MoveAfterDel);  //2059
  virtual long  GetRelPos(void);  //2327
  virtual long  GotoPos(long);  //2634
  virtual void  Download(CDB7Stream &);   //1278
  virtual void  EscalateTLock(enum TLockLevel);   //1310
  virtual void  GotoNull(void);   //2623
  virtual void  InitNew(void);  //2720
  virtual void  SetBufferSize(short);   //3489
  virtual void  SetRelPos(long,int);  //3726
  virtual void  UnSelect(void);   //3955
  virtual void  Upload(CDB7Stream &,CDBSign,int);   //4023
  void  SetMainRec(CStoreObj *);  //3634
  void  SetMainSQLRec(CSQLRec *);   //3635
//  void  `default constructor closure'(void);  //785
};


class IMPORT_1C C1SZReferInfo
{
public:
	 C1SZReferInfo(void);	//1
	 ~C1SZReferInfo(void);	//340
	int  FindFieldByID(long);	//1364
};


class IMPORT_1C CAccDef: public CMetaDataObj
{
public:
	 CAccDef(CItemList const &);	//2
	 CAccDef(long,char const *,char const *);	//3
	virtual  ~CAccDef(void);	//341
	virtual void  AddToList(CItemList &);	//855
	int  CanAddSbCnt(void);	//940
	int  CanEditActive(void);	//942
	int  CanEditIsAmount(void);	//943
	int  CanEditIsCurr(void);	//944
	int  CanEditIsSingle(void);	//945
	CString   GetAccCode(void)const;	//1400
	int  GetActive(void)const;	//1432
	virtual CMetaDataObjArray *  GetChild(int,CMetaDataCont *);	//1514
	virtual char const *  GetChildName(int,int)const;	//1529
	int  GetIsAmount(void)const;	//1827
	int  GetIsCurr(void)const;	//1828
	int  GetIsFolder(void)const;	//1829
	int  GetIsSingle(void)const;	//1830
	static char const *  GetListFunctor(void);	//1883
	virtual int  GetNChilds(void)const;	//1980
	virtual int  GetNProps(void)const;	//2012
	int  GetNSbCntDefs(void);	//2051
	virtual char const *  GetPropName(int,int)const;	//2184
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2218
	CAccSbCntDef *  GetSbCntDefAt(int);	//2398
	CTypedFldDefsArray<CAccSbCntDef> *  GetSbCntDefs(void);	//2399
	CAccSbCntsList *  GetSbCnts(void);	//2403
	virtual void  ScanMDObjects(SScanMDObjects *);	//3303
	void  SetAccCode(CString);	//3451
	void  SetActive(int);	//3462
	void  SetIsAmount(int);	//3614
	void  SetIsCurr(int);	//3615
	void  SetIsFolder(int);	//3616
	void  SetIsSingle(int);	//3617
	int  TestUniqueSbCntCode(CString,long);	//3924
	virtual int  WhatIsIt(void)const;	//4074
};


class IMPORT_1C CAccParamDef: public CMetaDataTypedObj
{
public:
	 CAccParamDef(CAccParamDef const &);	//4
	 CAccParamDef(CItemList const &);	//5
	 CAccParamDef(long,char const *,char const *,CType const &);	//6
	 CAccParamDef(CMetaDataTypedObj const *);	//7
	virtual  ~CAccParamDef(void);	//342
	CAccParamDef &  operator=(CAccParamDef const &);	//499
	virtual void  AddToList(CItemList &)const;	//856
	int  GetDateDep(void)const;	//1624
	static char const *  GetListFunctor(void);	//1884
	virtual int  IsDateDep(void)const;	//2793
	void  SetDateDep(int);	//3534
	virtual int  WhatIsIt(void)const;	//4075
};


class IMPORT_1C CAccSbCntDef
{
public:
	 CAccSbCntDef(CAccSbCntDef const &);	//8
	 CAccSbCntDef(CItemList const &);	//9
	 CAccSbCntDef(long,char const *,char const *,CType const &);	//10
	 CAccSbCntDef(CMetaDataTypedObj const *);	//11
	virtual  ~CAccSbCntDef(void);	//343
	CAccSbCntDef &  operator=(CAccSbCntDef const &);	//500
	virtual void  AddToList(CItemList &)const;	//857
	int  GetAmount(void)const;	//1448
	int  GetCurrency(void)const;	//1587
	int  GetEq(void)const;	//1701
	static char const *  GetListFunctor(void);	//1885
	virtual int  GetNProps(void)const;	//2013
	virtual char const *  GetPropName(int,int)const;	//2185
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2219
	long  GetSbKind(void)const;	//2404
	int  GetTurnOversOnly(void)const;	//2491
	void  SetAmount(int);	//3472
	void  SetCurrency(int);	//3516
	void  SetEq(int);	//3572
	void  SetSbKind(long);	//3739
	void  SetTurnOversOnly(int);	//3780
	virtual int  WhatIsIt(void)const;	//4076
};


class IMPORT_1C CAccSbCntsList
{
public:
	 CAccSbCntsList(void);	//12
	 ~CAccSbCntsList(void);	//344
};


class IMPORT_1C CAccSelSet:public CUniSet
{
DECLARE_DYNAMIC(CAccSelSet)

public:
	char buff[2000];
	 CAccSelSet(CDataBase7 *);	//13
	virtual  ~CAccSelSet(void);	//345
//	void  `default constructor closure'(void);	//756
	virtual int  Delete(enum MoveAfterDel);	//1167
	virtual int  GotoFirst(void);	//2566
	virtual int  GotoLast(void);	//2583
	virtual void  InitNew(void);	//2705
	virtual int  IsMultSel(void);	//2821
	virtual int  New(void);	//3026
	int  SelectPeriod(CObjID,CDate,CDate,int,CEventTime,CEventTime);	//3416
	int  SelectProv(CObjID,int,int);	//3427
protected:
	void  SetCurOrder(int);	//3510
	void  SetSearchTime(CEventTime const &,CEventTime const &);	//3743
public:
	virtual int  SetSelectionFields(void);	//3750
	virtual int  TestSelectionFields(void);	//3904
	virtual int  Update(void);	//3978
};


class IMPORT_1C CAccService
{
public:
	 CAccService(CAccService const &);	//14
	 CAccService(void);	//15
	 ~CAccService(void);	//346
	CAccService &  operator=(CAccService const &);	//501
	void  AddNewAcc(CObjID);	//828
	void  Done(void);	//1254
	struct SSbAccountInfo const &  GetAccInfoByCode(CString,long &);	//1414
	CAccSet *  GetAccSet(int);	//1421
	CObjID   GetAccountByCode(CString,long &);	//1424
	long  GetMDID(CObjID const &);	//1937
	int  GetNewAccIndex(CObjID);	//2060
	CString   GetPresent(CObjID);	//2154
	CAccSet *  Init(int);	//2672
	int  IsNewAcc(CObjID);	//2836
	void  RemoveNewAcc(CObjID);	//3224
};


class IMPORT_1C CAccSet:public CUniSet
{
DECLARE_DYNAMIC(CAccSet)

public:
	 CAccSet(CDataBase7 *);	//16
	virtual  ~CAccSet(void);	//347
//	void  `default constructor closure'(void);	//757
	void  BeginReadParams(void);	//913
	int  CanEdit(int &,int &,int &,int &,int &,int &);	//941
	int  CheckFilter(void);	//970
	int  CheckNew(CString &);	//971
	void  ClearAllParams(void);	//983
protected:
	void  ClearDirty(void);	//990
public:
	void  ClearParam(int);	//997
	void  ClearParam(CAccParamDef *);	//998
	virtual int  Delete(enum MoveAfterDel);	//1168
	virtual void  Download(CDB7Stream &);	//1263
	void  EnableDeleteChilds(int);	//1293
	void  EndReadParams(void);	//1301
	virtual void  EscalateTLock(enum TLockLevel);	//1304
	CString   GetCode(void)const;	//1543
	virtual long  GetCurrentPos(void);	//1594
	CString   GetDifCode(CString);	//1658
	CString   GetNewCode(long,CString);	//2061
	CPlanDef *  GetPlanDef(void)const;	//2143
	CString   GetPresent(void);	//2155
	static int  GetRecalcTotals(void);	//2294
	virtual long  GetRelPos(void);	//2322
	virtual int  GotoCurrent(void);	//2558
	virtual int  GotoFirst(void);	//2567
	virtual int  GotoLast(void);	//2584
	virtual int  GotoNext(void);	//2600
	virtual void  GotoNull(void);	//2615
	virtual long  GotoPos(long);	//2624
	virtual int  GotoPrev(void);	//2635
	virtual void  InitNew(void);	//2706
	virtual int  IsMultSel(void);	//2822
	int  IsParamDirty(int);	//2856
	int  IsParamReaded(int);	//2858
	void  LinkAllParams(int,int);	//2901
	void  LinkParam(int,int,int);	//2911
	void  LinkParam(CAccParamDef *,int,int);	//2912
	virtual int  Lock(void);	//2965
	int  MarkDelete(int,int);	//3000
	int  New(int);	//3027
	virtual int  New(void);	//3028
	void  ReadAllParams(void);	//3149
	void  ReadParam(int);	//3162
	void  ReadParam(CAccParamDef *);	//3163
	void  ReadParamWhithCheck(int);	//3168
	void  ResetFilter(void);	//3236
	int  SelectAll(CDate,int);	//3324
	int  SelectAllPlan(long,CDate);	//3342
	int  SelectAllSubAcc(long,CString,CDate,int);	//3343
	int  SelectCode(long,CString const &,CDate);	//3365
	int  SelectID(CObjID,CDate);	//3387
	int  SelectLikeCode(long,CString,CDate,int);	//3398
	int  SelectSubAcc(long,CString,CDate,int);	//3434
	void  SetAllParamDirty(void);	//3468
	virtual void  SetBufferSize(short);	//3484
	int  SetCode(CString const &);	//3498
protected:
	void  SetCurOrder(int);	//3511
public:
	void  SetFilter(long,std::set<CString,struct std::less<CString>,std::__alloc<1,0> > *);	//3580
	void  SetParamDirty(int);	//3670
	void  SetParamDirty(CAccParamDef *);	//3671
	void  SetParamReaded(int);	//3675
	void  SetPlanDef(CPlanDef *);	//3687
	virtual void  SetRelPos(long,int);	//3718
	virtual int  SetSelectionFields(void);	//3751
	int  TestCodeUnique(int);	//3893
	virtual int  TestSelectionFields(void);	//3905
private:
	int  TestSelectionFieldsPriv(CAccRec *);	//3919
public:
	virtual void  UnSelect(void);	//3946
	virtual int  Unlock(void);	//3963
	virtual int  Update(void);	//3979
	int  UpdateRec(void);	//4006
	virtual void  Upload(CDB7Stream &,CDBSign,int);	//4008
private:
	static int  s_bRecalcTotals;	//4248
public:
	static CString  s_sLastChangedCode;	//4257
};


class IMPORT_1C CAccTotalSet:public CUniSet
{
DECLARE_DYNAMIC(CAccTotalSet)

public:
	 CAccTotalSet(CDataBase7 *,char const *);	//17
	virtual  ~CAccTotalSet(void);	//348
//	void  `default constructor closure'(void);	//758
	int  CalcTurnoverAndBalanceByAccountByQuartWithTmpProvTbl(CDate);	//932
	int  ChangeTotal(CDate,int,CProvObj *,int,int,CNumeric,int);	//953
	int  ChangeTotals(CProvObj *,int,int,CDate);	//955
	virtual int  Delete(enum MoveAfterDel);	//1169
	int  DeleteByPeriod(CDate,CDate);	//1200
	int  DeleteProv(CProvObj *,CDate);	//1214
	void  DropTmpProvTbl(void);	//1284
	CMetaDataTypedObj *  GetAccountingDivisionDef(void)const;	//1425
	CValue *  GetSbCnt(long);	//2394
	virtual int  GotoFirst(void);	//2568
	virtual int  GotoLast(void);	//2585
	virtual int  GotoNext(void);	//2601
	virtual int  GotoPrev(void);	//2636
	virtual void  InitNew(void);	//2707
	virtual int  IsMultSel(void);	//2823
	virtual int  New(void);	//3029
	int  NewProv(CProvObj *,CDate);	//3045
	int  Select(CDate,int,CObjID,CObjID,CValue,CValue *,struct SSbKindInfo *,int);	//3318
	int  SelectAll(void);	//3325
	int  SelectPeriod(CDate,CDate);	//3417
	int  SelectWithFilter(CDate,int,CObjID,CObjID,CValue,CValue *,struct SAccSbKindInfo *,int);	//3446
	void  SetAccDiv(CValue const &,CValue &);	//3456
protected:
	void  SetCurOrder(int);	//3512
public:
	virtual int  SetSelectionFields(void);	//3752
	int  TestAllFields(void);	//3891
	virtual int  TestSelectionFields(void);	//3906
	int  Transfer(CDate,CDate);	//3939
	virtual int  Update(void);	//3980
	int  UpdateCorrespond(CProvObj *,CProvObj *,int,CDate);	//3999
	int  UpdateProv(CProvObj *,CProvObj *,CDate);	//4003
};


class IMPORT_1C CActualContextList
{
public:
	 CActualContextList(CActualContextList const &);	//18
	 CActualContextList(void);	//19
	virtual  ~CActualContextList(void);	//349
	CActualContextList &  operator=(CActualContextList const &);	//502
	static void  AddProvs(CProvObj *,int);	//839
	static void  AddRegs(long,CDate const &,CEventTime const &,CValue * *,CValue * *,int,int);	//848
	static void  CreateTempCalc(CDate,CEventTime);	//1069
	static void  DestroyTempCalc(void);	//1228
	static CActualContextList *  GetActualRegContexts(void);	//1434
	static CActualContextList *  GetActualTotalContexts(void);	//1435
	static int  GetInPosting(void);	//1817
	static int  NeedAddRegs(long,CDate const &,CEventTime const &,CValue * *);	//3025
	static void  RecalcTempCalc(CDate,CEventTime);	//3183
protected:
	static void  SetActualRegContexts(CActualContextList *);	//3463
	static void  SetActualTotalContexts(CActualContextList *);	//3464
public:
	static void  SetInPosting(int);	//3612
protected:
	virtual void  _AddProvs(CProvObj *,int);	//4141
	virtual void  _AddRegs(long,CDate const &,CEventTime const &,CValue * *,CValue * *,int,int);	//4142
	virtual void  _CreateTempCalc(CDate const &,CEventTime const &);	//4143
	virtual void  _DestroyTempCalc(void);	//4144
	virtual int  _NeedAddRegs(long,CDate const &,CEventTime const &,CValue * *);	//4179
	virtual void  _RecalcTempCalc(CDate const &,CEventTime const &);	//4180
	virtual void  _SetInPosting(int);	//4181
	static int  s_bInPosting;	//4246
	static CActualContextList *  s_pActualRegContexts;	//4251
	static CActualContextList *  s_pActualTotalContexts;	//4252
};


class IMPORT_1C CAlgorithmDef:public CMetaDataObj
{
public:
	 CAlgorithmDef(CItemList const &);	//20
	 CAlgorithmDef(long,char const *,char const *);	//21
	virtual  ~CAlgorithmDef(void);	//350
	virtual void  AddToList(CItemList &)const;	//858
	void  DelGroupID(long);	//1139
	int  Displ(CAlgorithmDef const *)const;	//1237
	virtual CMetaDataObjArray *  GetChild(int,CMetaDataCont *);	//1515
	virtual char const *  GetChildName(int,int)const;	//1530
	CMetaDataRefObj *  GetDisplAlgs(void);	//1659
	static char const *  GetListFunctor(void);	//1886
	virtual int  GetNChilds(void)const;	//1981
	virtual int  GetNProps(void)const;	//2014
	int  GetPriority(void)const;	//2176
	virtual char const *  GetPropName(int,int)const;	//2186
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2220
	int  GetSubst(void)const;	//2454
	int  GetSubstPriority(void)const;	//2455
	int  InGroup(long)const;	//2664
	void  SetGroupIDList(void);	//3595
	void  SetPriority(int);	//3692
	void  SetSubst(int);	//3772
	void  SetSubstPriority(int);	//3773
	virtual int  WhatIsIt(void)const;	//4077
};


class IMPORT_1C CAllRegsFilters
{
public:
	 CAllRegsFilters(CAllRegsFilters const &);	//22
	 CAllRegsFilters(void);	//23
	 ~CAllRegsFilters(void);	//351
	CAllRegsFilters &  operator=(CAllRegsFilters const &);	//503
	CRegFilter *  GetAt(int);	//1457
	void  ResetAt(int);	//3234
	void  SetAt(int,CRegFilter *);	//3478
};


class IMPORT_1C CBLContext:public CObject //32 Real - 4 VT = 28=0x1C
{
DECLARE_DYNCREATE(CBLContext)

public:
//Begin def
// +00 VTABLE
// +04 DWORD
	int m_RefCount;
// +08 DWORD param
	int m_FlagAutoDestroy;
// +0C CPtrArray
	//CPtrArray m_Array; // массив ссылок на CValue, которые ссылаются на этот CBLContext
	CValuePtrArray m_Array;  // массив ссылок на CValue, которые ссылаются на этот CBLContext

//	char dump [0x50];
	virtual  ~CBLContext(void);	//352
	CBLContext(int Param = 1);	//24
//	void  `default constructor closure'(void);	//759
protected:
//	CBLContext();
	void  					AddToValues(CValue const *);	//896
public:
	virtual void			IncrRef(void);
	virtual void			DecrRef(void);

	virtual int				GetDestroyUnRefd(void)const; // return m_FlagAutoDestroy; // в этом классе

	virtual int				IsOleContext(void)const;

	virtual CType			GetValueType(void)const;	//2547
	virtual long			GetTypeID(void)const;	//2505
	virtual CObjID			GetID(void)const;	//1804
	virtual char const *	GetCode(void)const;	//1544

	virtual int				IsExactValue(void)const;	//2808

	virtual void			InitObject(char const *);	//2722
	virtual void			InitObject(class CType const &);	//2721

	virtual void			SelectByID(CObjID,long);	//3350

	virtual char const *	GetTypeString(void)const;	//2513

	virtual int				GetNProps(void)const;	//2015
	virtual int				FindProp(char const *)const;	//1369
	virtual char const *	GetPropName(int,int)const;	//2187
	virtual int				GetPropVal(int,CValue &)const;	//2221
	virtual int				SetPropVal(int,CValue const &);	//3694
	virtual int				IsPropReadable(int)const;	//2863
	virtual int				IsPropWritable(int)const;	//2865

	virtual int				GetNMethods(void)const;	//2005
	virtual int				FindMethod(char const *)const;	//1366
	virtual char const *	GetMethodName(int,int)const;	//1964
	virtual int				GetNParams(int)const;	//2008
	virtual int				GetParamDefValue(int,int,CValue *)const;	//2122
	virtual int				HasRetVal(int)const;	//2657

	virtual int				CallAsProc(int,CValue * *);	//937
	virtual int				CallAsFunc(int,CValue &,CValue * *);	//935

	virtual int				IsSerializable(void);	//2874

	virtual int				SaveToString(CString &);	//3295
	virtual class			CBLContextInternalData *  GetInternalData(void);	//1826
	virtual void			GetExactValue(CValue &);	//1708

	static CBLContext *		CreateInstance(CType const &);	//1059
	static CBLContext *		CreateInstance(char const *);	//1060
	static unsigned long	GetFirstLoadedContextID(void);	//1751
	static CBLContext *		GetLoadedContext(unsigned long);	//1930
	static unsigned long	GetNextLoadedContextID(unsigned long);	//2072
	
	static void				RegisterContextClass(struct CRuntimeClass *,char const *,CType const &);	//3195
	static void				RegisterOleContextClass(struct CRuntimeClass *);	//3200
	static void				UnRegisterContextClass(struct CRuntimeClass *);	//3945

	unsigned long			GetLoadedID(void)const;	//1931
	char const *			GetPresentMethodName(int)const;	//2170
	char const *			GetPresentPropName(int)const;	//2171
	
	void					HashMethods(int);	//2661
	void					HashProperties(int);	//2662
	void					Load(void)const;	//2924
	void					SetHashID(char const *);	//3605
	void					Unload(void)const;	//3962

private:
	void					RemoveFromValues(CValue const *);	//3223
};

// теперь в basic.h
// class IMPORT_1C CFormCtrlContext:public CBLContext
// {
// DECLARE_DYNCREATE(CFormCtrlContext)
// 
// public:
// 	CGetField* m_pGetField; //CFormCtrl* m_Ctrl; //указатель на поименованный атрибут формы
// 
// 	//CGetDoc77* m_GetDoc; // АльФ - указатель на атрибуты класса GetDoc7
// 	CGetDoc7* m_GetDoc; // АльФ - указатель на атрибуты класса GetDoc7 artbear
// 
// 	int m_Flag;
// };


class IMPORT_1C CBLContextInternalData:public CObject
{
DECLARE_DYNAMIC(CBLContextInternalData)

public:
	 CBLContextInternalData(void);	//25
	virtual  ~CBLContextInternalData(void);	//353
};


class IMPORT_1C CBLExportContext:public IDispatch
{
public:
	DWORD m_RefCount;
	CBLContext* m_pCont;
	DWORD m_Flag2;
	 CBLExportContext(class CBLExportContext const &);	//26
	 CBLExportContext(class CBLContext *);	//27
	 CBLExportContext(void);	//28
	class CBLExportContext &  operator=(class CBLExportContext const &);	//504

	virtual HRESULT STDMETHODCALLTYPE  QueryInterface(struct _GUID const &,void * *);	//3136
	virtual ULONG STDMETHODCALLTYPE AddRef(void);	//843
	virtual ULONG STDMETHODCALLTYPE  Release(void);	//3217
	virtual HRESULT STDMETHODCALLTYPE  GetTypeInfoCount(unsigned int *);	//2509
	virtual HRESULT STDMETHODCALLTYPE  GetTypeInfo(unsigned int,unsigned long,struct ITypeInfo * *);	//2507
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID,LPOLESTR __RPC_FAR *,UINT,unsigned long,long *);	//1807
	virtual HRESULT STDMETHODCALLTYPE  Invoke(long,struct _GUID const &,unsigned long,unsigned short,struct tagDISPPARAMS *,struct tagVARIANT *,struct tagEXCEPINFO *,unsigned int *);	//2776
protected:
	long  DispParamToValue(class CValue * *,struct tagDISPPARAMS *,unsigned int *);	//1236
	int  ReplaceDefValue(class CValue * * *,int,class CBLContext *,long);	//3227
	int  TryReturnByRef(class CValue * *,struct tagDISPPARAMS *);	//3940
	int  ValueToVariant(class CValue &,struct tagVARIANT *);	//4041
//public:
//	long  pubDispParamToValue(class CValue ** ppValue,DISPPARAMS * pDispParam,UINT * uiFlags) {return DispParamToValue(ppValue,pDispParam,uiFlags);};
//	int  pubValueToVariant(class CValue & cValue,struct tagVARIANT * pVariant) {return ValueToVariant(cValue,pVariant);};
};


class IMPORT_1C CBLSpeller
{
public:
	 CBLSpeller(CBLSpeller const &);	//29
	 CBLSpeller(CItemList const &);	//30
	 CBLSpeller(char const *);	//31
	 CBLSpeller(void);	//32
	virtual  ~CBLSpeller(void);	//354
	CBLSpeller &  operator=(CBLSpeller const &);	//505
protected:
	void  AddHundred(char const *,CString &)const;	//821
	void  AddTen(char const *,int,CString &,int &)const;	//854
public:
	struct SDateFmtInfo const *  GetDateFmtInfo(void)const;	//1628
protected:
	virtual int  GetNProps(void)const;	//2016
	virtual struct SProperty *  GetProperty(int)const;	//2250
public:
	CString   GetSpelling(int)const;	//2443
	void  InitDefaults(void);	//2696
	int  InitFromList(CItemList const &);	//2697
	int  InitFromSpeller(CBLSpeller const &);	//2698
	int  LoadFromFile(char const *);	//2936
	int  LoadFromValue(CValue const &);	//2951
	int  SaveToFile(char const *)const;	//3283
	int  SaveToList(CItemList &)const;	//3286
	int  SetSpelling(int,char const *);	//3770
protected:
	void  SpellBillion(CThriads const &,CString &)const;	//3841
	void  SpellCopeek(CThriads const &,CString &)const;	//3842
public:
	void  SpellDate(CDate,CString &)const;	//3843
	void  SpellDateRange(CDate,CDate,CString &)const;	//3844
protected:
	void  SpellMillion(CThriads const &,CString &)const;	//3845
public:
	void  SpellMoney(CNumeric const &,CString &,int)const;	//3846
	void  SpellNumber(CNumeric const &,CString &)const;	//3847
protected:
	void  SpellOne(CThriads const &,CString &)const;	//3848
	void  SpellRuble(CThriads const &,CString &,int)const;	//3849
	void  SpellThousand(CThriads const &,CString &)const;	//3850
	void  SpellThrillion(CThriads const &,CString &)const;	//3851
public:
	void  SpellWeekDay(CDate,CString &)const;	//3852
};


class IMPORT_1C CBLValueRepresentContext
{
public:
	 CBLValueRepresentContext(CBLValueRepresentContext const &);	//33
	 CBLValueRepresentContext(void);	//34
	 ~CBLValueRepresentContext(void);	//355
	CBLValueRepresentContext &  operator=(CBLValueRepresentContext const &);	//506
	virtual unsigned long  AddRef(void);	//844
	virtual long  GetIDsOfNames(struct _GUID const &,unsigned short * *,unsigned int,unsigned long,long *);	//1808
	virtual long  GetTypeInfo(unsigned int,unsigned long,struct ITypeInfo * *);	//2508
	virtual long  GetTypeInfoCount(unsigned int *);	//2510
	virtual long  Invoke(long,struct _GUID const &,unsigned long,unsigned short,struct tagDISPPARAMS *,struct tagVARIANT *,struct tagEXCEPINFO *,unsigned int *);	//2777
	virtual long  QueryInterface(struct _GUID const &,void * *);	//3137
	virtual unsigned long  Release(void);	//3218
	int  SetValue(CValue &);	//3799
};


class IMPORT_1C CBelong
{
public:
	 CBelong(CBelong const &);	//35
	 CBelong(void);	//36
	 ~CBelong(void);	//356
	CBelong &  operator=(CBelong const &);	//507
	int  Belong(CValue const &);	//919
	int  GetBelongValue(CValue &,struct std::__hashtable_iterator<struct std::pair<CString const ,CBelongValueInfo>,CString,struct std::hash<char const *>,struct std::select1st<struct std::pair<CString const ,CBelongValueInfo> >,struct std::equal_to<CString>,std::__alloc<1,0> > &,int);	//1470
	int  GetGroup(CValue &,struct std::__hashtable_iterator<struct std::pair<CString const ,CBelongValueInfo>,CString,struct std::hash<char const *>,struct std::select1st<struct std::pair<CString const ,CBelongValueInfo> >,struct std::equal_to<CString>,std::__alloc<1,0> > &,int);	//1775
	CValue   GetParent(CValue const &);	//2127
	int  GetValue(CValue &);	//2539
	int  IsEmpty(void)const;	//2799
	int  IsPrepared(void)const;	//2862
	int  ItemsOnly(void)const;	//2889
private:
	CValue   KeyToValue(CString const &);	//2893
public:
	int  OneItemOnly(void)const;	//3074
	void  Prepare(PValueArray const &,CType const &,int);	//3091
	void  Prepare(CValue const &,CType const &,int);	//3092
private:
	void  PrepareGroups(void);	//3094
	void  PrepareValue(CValue const &,CType const &);	//3095
	void  Reorder(void);	//3226
public:
	void  Reset(void);	//3228
	int  SelectBelongValues(struct std::__hashtable_iterator<struct std::pair<CString const ,CBelongValueInfo>,CString,struct std::hash<char const *>,struct std::select1st<struct std::pair<CString const ,CBelongValueInfo> >,struct std::equal_to<CString>,std::__alloc<1,0> > &,int);	//3344
	int  SelectGroups(struct std::__hashtable_iterator<struct std::pair<CString const ,CBelongValueInfo>,CString,struct std::hash<char const *>,struct std::select1st<struct std::pair<CString const ,CBelongValueInfo> >,struct std::equal_to<CString>,std::__alloc<1,0> > &,int);	//3386
private:
	CString   ValueToKey(CValue const &);	//4040
};


class IMPORT_1C CBkEndUI
{
public:
	 CBkEndUI(CBkEndUI const &);	//37
	 CBkEndUI(void);	//38
	CBkEndUI &  operator=(CBkEndUI const &);	//508
	virtual int  DoMessageBox(unsigned int,unsigned int,unsigned int);	//1238
	virtual int  DoMessageBox(char const *,unsigned int,unsigned int);	//1239
	virtual void  DoStatusLine(char const *);	//1241
	virtual void  DoMessageLine(char const *,enum MessageMarker);	//1240
	virtual CString   GetStatusLine(void)const;	//2445
};


class IMPORT_1C CBuhDef:public CMetaDataObj
{
public:
	 CBuhDef(void);	//39
	virtual  ~CBuhDef(void);	//357
	int  AddPlanDef(CPlanDef *);	//831
	void  AddToList(CItemList &);	//859
	void  ClearSbKindToType(void);	//1005
	void  DelAll(void);	//1098
	void  DelPlanDef(int,int);	//1146
	void  DelPlanDef(CPlanDef *,int);	//1147
	virtual void  DescribeObject(CString &,CMetaDataCont *,CString,int);	//1222
	void  EnableBuhMigration(int);	//1292
	int  GetAccCodeMaxLen(void)const;	//1401
	int  GetAccCodeWithDelimMaxLen(void)const;	//1402
	int  GetAccDescrLen(void)const;	//1411
	int  GetAccEditType(void)const;	//1412
	CFormDefsArray *  GetAccFormDefsArray(void);	//1413
	int  GetAccMaxLevels(void)const;	//1415
	CTypedFldDefsArray<CAccParamDef> *  GetAccParamDefs(void);	//1416
	CString   GetAccSbCntFieldName(int);	//1418
	CMetaDataTypedObj *  GetAccountingDivisionDef(void);	//1426
	long  GetAccountingDivisionID(void)const;	//1428
	int  GetAllowComplexProvs(void)const;	//1447
	int  GetAmountByAccount(void)const;	//1449
	int  GetAmountSeparated(void)const;	//1450
	CCorrProvsDef *  GetCorrProvsDef(void);	//1578
	long  GetCurrencyID(void)const;	//1588
	long  GetCurrencyMultiplierID(void);	//1589
	int  GetCurrencySumSeparated(void)const;	//1592
	long  GetDefaultPlanID(void)const;	//1644
	int  GetEditDocOperFlag(void);	//1686
	int  GetExistAccPresent(void)const;	//1709
	int  GetKindCalcOperSum(void)const;	//1873
	static char const *  GetListFunctor(void);	//1887
	int  GetNPlanDefs(void)const;	//2010
	int  GetOperDescrHasIndex(void)const;	//2102
	int  GetOperDescrLen(void)const;	//2103
	long  GetOperDocID(void);	//2104
	COperJournalDef *  GetOperJournalDef(void);	//2105
	CTypedFldDefsArray<COperParamDef> *  GetOperParamDefs(void);	//2106
	int  GetOperSumHasIndex(void)const;	//2110
	int  GetOperSumLen(void)const;	//2111
	int  GetOperSumPrecision(void)const;	//2112
	int  GetOperSumSeparated(void)const;	//2113
	CPlanDef *  GetPlanDef(long)const;	//2144
	CPlanDef *  GetPlanDef(char const *)const;	//2145
	CPlanDef *  GetPlanDefAt(int)const;	//2146
	int  GetPlanDefIdx(long)const;	//2147
	int  GetPlanDefIdx(CPlanDef *)const;	//2148
	int  GetPlanDefIdx(char const *)const;	//2149
	CMetaDataObjArray *  GetPlanDefs(void);	//2150
	int  GetProvAmountLen(void)const;	//2253
	int  GetProvAmountPrecision(void)const;	//2254
	int  GetProvAmountSort(void)const;	//2255
	int  GetProvCurSumLen(void)const;	//2256
	int  GetProvCurSumPrecision(void)const;	//2257
	int  GetProvCurrencySort(void)const;	//2258
	int  GetProvCurrencySumSort(void)const;	//2259
	CFormDefsArray *  GetProvFormDefsArray(void);	//2260
	CProvHardFldDef *  GetProvHardFldCurDef(void);	//2261
	CProvHardFldDef *  GetProvHardFldPlanDef(void);	//2262
	CTypedFldDefsArray<CProvParamDef> *  GetProvParamDefs(void);	//2264
//*!*	class CMetaDataObjArrayTemplate<class CPresentItem> *  GetProvPresentFields(void)const;	//2265
	int  GetProvSumLen(void)const;	//2270
	int  GetProvSumPrecision(void)const;	//2271
	int  GetProvSumSeparated(void)const;	//2272
	int  GetProvSumSort(void)const;	//2273
	long  GetRateID(void)const;	//2279
	int  GetSbCntMaxCount(void)const;	//2400
	int  GetSbCntMaxLen(void)const;	//2401
	CTypedFldDefsArray<CSbKindDef> *  GetSbKindDefs(void);	//2405
	CSbKindToType *  GetSbKindToType(void);	//2407
	int  GetSelByAccountDepth(void)const;	//2410
	int  GetSelectByAccDtKt(void);	//2411
	int  GetSelectByAccount(void)const;	//2412
	int  GetSelectByPlanID(void)const;	//2413
	CTypOpersDef *  GetTypOpersDef(void);	//2495
	int  GetUserDefineKindCalcOperSum(void)const;	//2526
	int  InsertPlanDef(int,CPlanDef *);	//2764
	int  IsBuhMigrationEnabled(void)const;	//2787
	void  LoadFromList(CItemList const &);	//2938
	int  MovePlanDef(CPlanDef *,int);	//3017
	virtual void  ScanMDObjects(SScanMDObjects *);	//3304
	int  SetAccCodeMaxLen(int);	//3453
	void  SetAccDescrLen(int);	//3455
	void  SetAccEditType(int);	//3458
	void  SetAccountingDivisionID(long);	//3461
	void  SetAllowComplexProvs(int);	//3471
	void  SetAmountByAccount(int);	//3473
	void  SetAmountSeparated(int);	//3474
	void  SetCurrencyID(long);	//3517
	void  SetCurrencyMultiplierID(long);	//3518
	void  SetCurrencySumSeparated(int);	//3520
	void  SetDefaultPlanID(long);	//3550
	void  SetEditDocOperFlag(int);	//3563
	void  SetExistAccPresent(int);	//3574
	void  SetKindCalcOperSum(int);	//3622
	void  SetOperDescrHasIndex(int);	//3662
	void  SetOperDescrLen(int);	//3663
	void  SetOperDocID(long);	//3664
	void  SetOperSumHasIndex(int);	//3665
	void  SetOperSumLen(int);	//3666
	void  SetOperSumPrecision(int);	//3667
	void  SetOperSumSeparated(int);	//3668
	void  SetProvAmountLen(int);	//3698
	void  SetProvAmountPrecision(int);	//3699
	void  SetProvAmountSort(int);	//3700
	void  SetProvCurSumLen(int);	//3701
	void  SetProvCurSumPrecision(int);	//3702
	void  SetProvCurrencySort(int);	//3703
	void  SetProvCurrencySumSort(int);	//3704
	void  SetProvSumLen(int);	//3705
	void  SetProvSumPrecision(int);	//3706
	void  SetProvSumSeparated(int);	//3707
	void  SetProvSumSort(int);	//3708
	void  SetRateID(long);	//3711
	int  SetSbCntMaxCount(int);	//3737
	void  SetSelByAccountDepth(int);	//3746
	void  SetSelectByAccDtKt(int);	//3747
	void  SetSelectByAccount(int);	//3748
	void  SetSelectByPlanID(int);	//3749
	void  SetUserDefineKindCalcOperSum(int);	//3794
	int  SortPlanDefs(void);	//3831
	virtual int  WhatIsIt(void)const;	//4078
};


class IMPORT_1C CCJDef:public CMetaDataObj
{
public:
	 CCJDef(CItemList const &);	//40
	 CCJDef(long,char const *,char const *);	//41
	virtual  ~CCJDef(void);	//358
	int  AddCJParamDef(CCJParamDef *);	//806
	virtual void  AddToList(CItemList &)const;	//860
	void  DelCJParamDef(int,int);	//1119
	void  DelCJParamDef(CCJParamDef *,int);	//1120
	CCJParamDef *  GetCJParamDef(long)const;	//1484
	CCJParamDef *  GetCJParamDef(char const *)const;	//1485
	CCJParamDef *  GetCJParamDefAt(int)const;	//1486
	int  GetCJParamDefIdx(long)const;	//1487
	int  GetCJParamDefIdx(CCJParamDef *)const;	//1488
	int  GetCJParamDefIdx(char const *)const;	//1489
	CMetaDataObjArray *  GetCJParamDefs(void);	//1490
	virtual CMetaDataObjArray *  GetChild(int,CMetaDataCont *);	//1516
	virtual char const *  GetChildName(int,int)const;	//1531
	char const *  GetFFCode(long,CSbCntTypeDef *);	//1712
	char const *  GetFFFieldName(long)const;	//1713
	static char const *  GetFFFunctor(void);	//1714
	int  GetFFKind(void)const;	//1716
	CFFList *  GetFFList(void)const;	//1717
	CType   GetFFType(long,CSbCntTypeDef *);	//1719
	CFormDefsArray *  GetFormDefsArray(void);	//1762
	static char const *  GetListFunctor(void);	//1888
	int  GetNCJParamDefs(void)const;	//1977
	virtual int  GetNChilds(void)const;	//1982
	virtual int  GetNProps(void)const;	//2017
	CMetaDataObjArray *  GetParams(void);	//2125
	CDate   GetPeriodDate(void)const;	//2136
	enum PeriodType   GetPeriodType(void)const;	//2140
	virtual char const *  GetPropName(int,int)const;	//2188
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2222
	long  GetReferID(void)const;	//2303
	int  GetResDec(void)const;	//2335
	int  GetResLen(void)const;	//2336
	char const *  GetTDFFFieldName(long)const;	//2458
	char const *  GetTableName(void)const;	//2460
	int  InsertCJParamDef(int,CCJParamDef *);	//2748
	int  MoveParamDef(CCJParamDef *,int);	//3015
	virtual void  ScanMDObjects(SScanMDObjects *);	//3305
	void  SetFFKind(int);	//3577
	void  SetPeriodDate(CDate);	//3682
	void  SetPeriodType(enum PeriodType);	//3684
	void  SetReferID(int);	//3716
	void  SetResDec(int);	//3727
	void  SetResLen(int);	//3728
	int  SortParamDefs(void);	//3829
	virtual int  WhatIsIt(void)const;	//4079
};


class IMPORT_1C CCJParamDef: public CMetaDataTypedObj
{
public:
	 CCJParamDef(CCJParamDef const &);	//42
	 CCJParamDef(CItemList const &);	//43
	 CCJParamDef(long,char const *,char const *,CType const &);	//44
	 CCJParamDef(CMetaDataTypedObj const *);	//45
	virtual  ~CCJParamDef(void);	//359
	CCJParamDef &  operator=(CCJParamDef const &);	//509
	virtual void  AddToList(CItemList &)const;	//861
	static char const *  GetListFunctor(void);	//1889
	virtual int  WhatIsIt(void)const;	//4080
};


class IMPORT_1C CCJRecsArray
{
public:
	 CCJRecsArray(void);	//46
	 ~CCJRecsArray(void);	//360
	int  Add(CCJVirtualRec *);	//795
	void  ExternalAND(CCJRecsArray *);	//1318
	CCJVirtualRec *  GetAt(int);	//1458
	int  GetSize(void);	//2436
	void  InsertAt(int,CCJVirtualRec *);	//2746
	void  InsertRecs(CCJDef *,CObjID,int,CObjID);	//2767
	void  InternalXOR(void);	//2775
	void  SortByPeriod(void);	//3820
};


class IMPORT_1C CCJSet:public CUniSet//CObjectSet
{
DECLARE_DYNAMIC(CCJSet)

public:
	 CCJSet(CCJDef *,int,CDataBase7 *);	//47
	virtual  ~CCJSet(void);	//361
	int  CheckOnArch(void);	//973
	void  CopyRec(CObjID,CObjID);	//1049
	virtual int  Delete(enum MoveAfterDel);	//1170
	int  DeleteAll(CCJRecsArray *);	//1189
	int  DeleteIDS(CObjID);	//1208
	int  DeleteRec(int);	//1217
	virtual int  DocIsOK(void);	//1243
	virtual void  Download(CDB7Stream &);	//1264
	virtual void  EscalateTLock(enum TLockLevel);	//1305
	long  FillIDSArray(CObjID * *);	//1343
	CCJDef *  GetCJDef(void);	//1476
	//CCalcPeriod   GetCurrentPeriod(void);	//1593
	CCalcPeriod   GetCurrentPeriod(int);	//1593
	virtual long  GetCurrentPos(void);	//1595
	CDocDef *  GetDocDef(void);	//1663
	long  GetFFID(CString);	//1715
	CString   GetFFName(long);	//1718
	int  GetKeyValueList(PValueItemArray *,int,int);	//1867
	CCalcPeriod   GetPeriodByDate(CDate);	//2135
	CString   GetPresent(void);	//2156
	CSbCntTypeDef *  GetReferDef(void);	//2302
	virtual long  GetRelPos(void);	//2323
	virtual int  GotoCurrent(void);	//2559
	virtual int  GotoFirst(void);	//2569
	virtual int  GotoLast(void);	//2586
	virtual int  GotoNext(void);	//2602
	virtual void  GotoNull(void);	//2616
	virtual long  GotoPos(long);	//2625
	virtual int  GotoPrev(void);	//2637
	int  HasFixedRec(void);	//2654
	int  HasRecalcInPeriod(CString);	//2656
	virtual void  InitNew(void);	//2708
	virtual int  IsMultSel(void);	//2824
	virtual int  IsOK(void);	//2838
	void  LinkAllParams(void);	//2902
	void  LinkParam(int);	//2913
	void  LinkParam(CCJParamDef *);	//2914
	virtual int  Lock(void);	//2966
	virtual int  New(void);	//3030
	virtual int  ReSelect(int);	//3141
	int  ReSelectFF(CValue *);	//3145
	int  ReSelectFF(CCalcPeriod,CCalcPeriod);	//3146
	int  ReSelectFF(CObjID);	//3147
	virtual int  ReferIsOK(void);	//3188
	void  RefreshProps(void);	//3192
	int  ReversPeriod(CCalcPeriod,CCalcPeriod);	//3243
	int  SelectAllObjAllPer(void);	//3336
	int  SelectAllObjByDatesStrong(CDate,CDate);	//3337
	int  SelectAllObjFewPer(CCalcPeriod,CCalcPeriod);	//3338
	int  SelectAllObjOnePer(CCalcPeriod);	//3339
	int  SelectFF(long,CValue *,CCalcPeriod,CCalcPeriod);	//3382
	int  SelectFFOneObj(long,CValue *,CCalcPeriod,CCalcPeriod,CObjID);	//3383
	int  SelectID(CObjID);	//3388
	int  SelectIDDoc(CObjID,CCalcPeriod,CObjID);	//3391
	int  SelectIDParDoc(CObjID);	//3392
	int  SelectIDRecalc(CObjID);	//3393
	int  SelectOneObjAllPer(CObjID);	//3400
	int  SelectOneObjByDatesStrong(CObjID,CDate,CDate);	//3401
	int  SelectOneObjFewPer(CObjID,CCalcPeriod,CCalcPeriod);	//3402
	int  SelectOneObjFromPer(CObjID,CCalcPeriod);	//3403
	int  SelectOneObjOnePer(CObjID,CCalcPeriod);	//3404
	int  SetCurrentPeriod(CCalcPeriod);	//3521
	void  SetFFData(CDate,long);	//3576
	void  SetRecs(int);	//3715
	virtual void  SetRelPos(long,int);	//3719
	virtual int  SetSelectionFields(void);	//3753
	virtual int  TestSelectionFields(void);	//3907
	virtual void  UnSelect(void);	//3947
	virtual int  Unlock(void);	//3964
	virtual int  Update(void);	//3981
	int  UpdateFF(CObjID,enum FFUpdateType,CDate,long);	//4001
	virtual void  Upload(CDB7Stream &,CDBSign,int);	//4009
};


class IMPORT_1C CCJVirtualRec
{
public:
	 CCJVirtualRec(CCJVirtualRec const &);	//48
	 CCJVirtualRec(void);	//49
	 ~CCJVirtualRec(void);	//362
	CCJVirtualRec &  operator=(CCJVirtualRec const &);	//510
};


class IMPORT_1C CCalcPeriod
{
BYTE buf[100];
public:
	 CCalcPeriod(CCalcPeriod const &);	//50
	 CCalcPeriod(CDate,enum PeriodType,CDate);	//51
	 CCalcPeriod(CDate,enum PeriodType);	//52
	 CCalcPeriod(CString);	//53
	 CCalcPeriod(void);	//54
	 ~CCalcPeriod(void);	//363
	CCalcPeriod &  operator=(CCalcPeriod const &);	//511
	int  operator==(CCalcPeriod)const;	//604
	CCalcPeriod   AddPeriod(int);	//830
	void  ConstructByDateType(CDate,enum PeriodType);	//1034
	void  ConstructByString(CString);	//1035
	CString   GetDescr(void);	//1654
	CDate   GetLastDate(void);	//1874
	CDate   GetStartDate(void);	//2444
	CString   GetText(void);	//2478
	int  IsEmpty(void);	//2800
	void  SetText(CString);	//3775
};


class IMPORT_1C CCalcVarDef:public CMetaDataObj
{
public:
	 CCalcVarDef(CCalcVarDef const &);	//55
	 CCalcVarDef(CItemList const &);	//56
	 CCalcVarDef(long,char const *,char const *);	//57
	virtual  ~CCalcVarDef(void);	//364
	CCalcVarDef &  operator=(CCalcVarDef const &);	//512
	static char const *  GetListFunctor(void);	//1890
	virtual int  WhatIsIt(void)const;	//4081
};


class IMPORT_1C CCalendarSet:public CUniSet//CObjectSet
{
DECLARE_DYNAMIC(CCalendarSet)

public:
	 CCalendarSet(CMetaDataObj *,CDataBase7 *);	//58
	virtual  ~CCalendarSet(void);	//365
	virtual int  AutoLayout(CDate,CDate,int);	//910
	virtual CNumeric   Count(CDate,CDate);	//1052
	virtual int  Delete(enum MoveAfterDel);	//1171
	virtual void  Download(CDB7Stream &);	//1265
	virtual void  EscalateTLock(enum TLockLevel);	//1306
	virtual long  GetCurrentPos(void);	//1596
	CString   GetPresent(void);	//2157
	virtual long  GetRelPos(void);	//2324
	CMetaDataObj *  GetTypeDef(void)const;	//2501
	virtual int  GotoFirst(void);	//2570
	virtual int  GotoLast(void);	//2587
	virtual int  GotoNext(void);	//2603
	virtual void  GotoNull(void);	//2617
	virtual long  GotoPos(long);	//2626
	virtual int  GotoPrev(void);	//2638
	virtual int  IsMultSel(void);	//2825
	virtual int  IsOK(void);	//2839
	virtual int  Lock(void);	//2967
	virtual int  New(void);	//3031
	virtual int  ReSelect(int);	//3142
	int  SelectAll(void);	//3326
	int  SelectAllPeriod(void);	//3341
	int  SelectDate(CDate);	//3370
	int  SelectFromDate(CDate,CNumeric);	//3385
	int  SelectPeriod(CDate,CDate);	//3418
	virtual void  SetRelPos(long,int);	//3720
	virtual int  SetSelectionFields(void);	//3754
	virtual CNumeric   Summ(CDate,CDate);	//3884
	virtual int  TestSelectionFields(void);	//3908
	virtual void  UnSelect(void);	//3948
	virtual int  Unlock(void);	//3965
	virtual int  Update(void);	//3982
	virtual void  Upload(CDB7Stream &,CDBSign,int);	//4010
};


class IMPORT_1C CCalendarTypeDef: public CMetaDataObj
{
public:
	 CCalendarTypeDef(CItemList const &);	//59
	 CCalendarTypeDef(long,char const *,char const *);	//60
	virtual  ~CCalendarTypeDef(void);	//366
	virtual void  AddToList(CItemList &)const;	//862
	static char const *  GetListFunctor(void);	//1891
	virtual int  GetNProps(void)const;	//2018
	CMetaDataObjDistr *  GetObjDistr(void);	//2087
	CDate   GetPeriodDate(void)const;	//2137
	virtual char const *  GetPropName(int,int)const;	//2189
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2223
	char const *  GetTableName(void)const;	//2461
	void  SetPeriodDate(CDate);	//3683
	virtual int  WhatIsIt(void)const;	//4082
};


class IMPORT_1C CConstDef: public CMetaDataTypedObj
{
public:
	 CConstDef(CConstDef const &);	//61
	 CConstDef(CItemList const &);	//62
	 CConstDef(long,char const *,char const *,int,CType const &);	//63
	 CConstDef(CMetaDataTypedObj const *);	//64
	virtual  ~CConstDef(void);	//367
	virtual void  AddToList(CItemList &)const;	//863
	virtual int  GetNProps(void)const;	//2019
	virtual char const *  GetPropName(int,int)const;	//2190
	virtual int  GetPropVal(int,CMetaDataCont *,CValue &,CMetaDataObj * *)const;	//2224
	virtual int  WhatIsIt(void)const;	//4083

	virtual int  IsDateDep(void)const;	//2794

	CConstDef &  operator=(CConstDef const &);	//513
	static char const *  GetListFunctor(void);	//1892
	CMetaDataObjDistr *  GetObjDistr(void);	//2088
	void  SetDateDep(int);	//3535
};


class IMPORT_1C CConstService
{
public:
	 CConstService(void);	//65
	 ~CConstService(void);	//368
	CConstService &  operator=(CConstService const &);	//514
	void  Done(void);	//1255
	CConstSet *  GetConstSet(void);	//1575
	int  GetDateDepParam(CMetaDataTypedObj *,CObjID,CDate,CValue *,CTypedValue *);	//1625
	int  GetDateDepParamOnTime(CMetaDataTypedObj *,CObjID,CDate,long,CObjID,CValue *,CTypedValue *);	//1627
	void  MainInit(void);	//2986
};


class IMPORT_1C CConstSet:public CUniSet//CObjectSet
{
DECLARE_DYNAMIC(CConstSet)

public:
	 CConstSet(CDataBase7 *);	//66
	virtual  ~CConstSet(void);	//369
//	void  `default constructor closure'(void);	//760
protected:
	void  DecodeSQLValue(void);	//1084
	void  DecodeValue(void);	//1085
public:
	virtual int  Delete(enum MoveAfterDel);	//1172
	int  DeleteAll(void);	//1190
protected:
	void  DetectOwnerTypeDef(void);	//1234
public:
	virtual void  Download(CDB7Stream &);	//1266
	void  EnableRegisterOwnerUpdate(int);	//1298
	virtual void  EscalateTLock(enum TLockLevel);	//1307
	CDate   GetConstDate(void)const;	//1564
	CConstDef *  GetConstDef(void)const;	//1565
	int  GetConstIdx(void)const;	//1573
	virtual long  GetCurrentPos(void);	//1597
	CMetaDataTypedObj *  GetParamDef(void)const;	//2115
	virtual long  GetRelPos(void);	//2325
	CMetaDataObj *  GetTypeDef(void)const;	//2502
	virtual int  GotoCurrent(void);	//2560
	virtual int  GotoFirst(void);	//2571
	virtual int  GotoLast(void);	//2588
	virtual int  GotoNext(void);	//2604
	virtual void  GotoNull(void);	//2618
	virtual long  GotoPos(long);	//2627
	virtual int  GotoPrev(void);	//2639
	virtual void  InitNew(void);	//2709
	virtual int  IsLocked(void);	//2815
	virtual int  IsMultSel(void);	//2826
	virtual int  IsOK(void);	//2840
	virtual int  Lock(void);	//2968
	virtual int  New(void);	//3032
protected:
	int  NewConstRecs(CDate);	//3044
public:
	virtual int  ReSelect(int);	//3143
protected:
	void  RegisterOwnerUpdate(CObjID const &);	//3201
	void  SQLSetPosForDateSel(void);	//3262
public:
	int  Select(CObjID,CMetaDataTypedObj *);	//3319
	int  Select(CObjID,CMetaDataTypedObj *,CDate);	//3320
	int  Select(CObjID,CDate);	//3321
	int  SelectAllOfDoc(CObjID);	//3340
	int  SelectOnTime(CObjID,CMetaDataTypedObj *,CDate,long,CObjID);	//3399
	int  SelectOneOfDoc(CObjID,CMetaDataTypedObj *,CDate,long,CObjID);	//3405
	int  SelectRange(CObjID,CMetaDataTypedObj *,CDate,CDate);	//3429
	void  SetBufferSize(short);	//3485
	void  SetOwnerTypeDef(class CMetaDataObj *);	//3669
protected:
	void  SetPosForDateSel(void);	//3688
public:
	virtual void  SetRelPos(long,int);	//3721
	virtual int  SetSelectionFields(void);	//3755
protected:
	void  SetTypeDef(class CMetaDataTypedObj *);	//3786
public:
	void  SetUploadTypeID(long);	//3791
	int  TestSelectionFields2(class CConstRec *);	//3903
	virtual int  TestSelectionFields(void);	//3909
protected:
	void  TypeAnalis(void);	//3941
public:
	virtual void  UnSelect(void);	//3949
	virtual int  Unlock(void);	//3966
	virtual int  Update(void);	//3983
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4011
	void  UploadData(class CDB7Stream &);	//4028
};


class IMPORT_1C CConvService
{
public:
	 CConvService(void);	//67
	 ~CConvService(void);	//370
	int  ConvertAll(void);	//1039
	enum ConvResult   ConvertRefer(int);	//1040
	enum ConvResult   CreateSbCnt(class CString,int,class CAttribDat *,class CAttribDatKey *);	//1066
	void  DeleteFiles(void);	//1207
	enum ConvResult   ReadAttribDat(void);	//3152
	int  SetParams(class CString,class CString);	//3677
};


class IMPORT_1C CCorrProvsDef
{
public:
	 CCorrProvsDef(class CCorrProvsDef const &);	//68
	 CCorrProvsDef(class CItemList const &);	//69
	 CCorrProvsDef(long,char const *,char const *);	//70
	virtual  ~CCorrProvsDef(void);	//371
	class CCorrProvsDef &  operator=(class CCorrProvsDef const &);	//515
	virtual void  AddToList(class CItemList &)const;	//864
	static char const *  GetListFunctor(void);	//1893
	virtual int  GetNProps(void)const;	//2020
	class CMetaDataObjDistr *  GetObjDistr(void);	//2089
	virtual char const *  GetPropName(int,int)const;	//2191
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2225
	virtual int  WhatIsIt(void)const;	//4084
};


class IMPORT_1C CCorrectProvSet:public CUniSet
{
DECLARE_DYNAMIC(CCorrectProvSet)

public:
	 CCorrectProvSet(class CDataBase7 *);	//71
	virtual  ~CCorrectProvSet(void);	//372
//	void  `default constructor closure'(void);	//761
	virtual int  Delete(enum MoveAfterDel);	//1173
	virtual void  Download(class CDB7Stream &);	//1267
	virtual int  GotoFirst(void);	//2572
	virtual int  GotoLast(void);	//2589
	virtual void  InitNew(void);	//2710
	virtual int  IsMultSel(void);	//2827
	virtual int  New(void);	//3033
	int  SelectAll(void);	//3327
	int  SelectCode(long,class CString const &,int);	//3366
	int  SelectDt(class CObjID const &,class CObjID const &,int);	//3381
	int  SelectKt(class CObjID const &,class CObjID const &,int);	//3397
	int  SelectPlan(long,int);	//3424
	virtual int  SetSelectionFields(void);	//3756
	virtual int  TestSelectionFields(void);	//3910
	virtual int  Update(void);	//3984
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4012
};


class IMPORT_1C CCrossDocSet
{
public:
	 CCrossDocSet(class CDataBase7 *);	//72
	 ~CCrossDocSet(void);	//373
	class CCrossDocSet &  operator=(class CCrossDocSet const &);	//516
	int  ChangeTime(class CObjID *,long);	//950
	int  Delete(class CObjID *,int,int);	//1174
	int  Delete(class CObjID *,long,int,int);	//1175
	int  DeleteAll(int,int);	//1191
	void  InitCrossWrite(void);	//2694
	int  WriteCrossRef(class CDocSet *,long,class CValue *,class CDocSelRefDef *,int);	//4122
	int  WriteCrossRef(class CObjID *,class CDate,long,long,class CValue *,class CDocSelRefDef *,int);	//4123
};


class IMPORT_1C CDB7Info
{
public:
	 CDB7Info(class CDB7Info const &);	//73
	 CDB7Info(class CDBSign const &,char const *,int,struct _GUID const &,int,enum DIST_MODE_tag,class CString,class CString,int,int,class CString,class CString);	//74
	 CDB7Info(void);	//75
	 ~CDB7Info(void);	//374
	class CDB7Info const &  operator=(class CDB7Info const &);	//517
	static int  Char2DBStatus(char);	//958
	static char  DBStatus2Char(int);	//1076
	void  Download(class CDB7Stream &);	//1268
	void  FillDBRecordSet(class CDBSetRec *)const;	//1341
	void  FillMyselfFromDBRecordSet(class CDBSetRec *);	//1345
	char const *  GetDBDescr(void)const;	//1607
	void  GetDBDistInfo(enum DIST_MODE_tag &,class CString &,class CString &,class CString &,class CString &,int &,int &,class CString &,class CString &)const;	//1608
	class CDBSign const &  GetDBSign(void)const;	//1612
	int  GetDBStatus(void)const;	//1615
	struct _GUID const &  GetDBUUID(void)const;	//1616
	enum DIST_MODE_tag   GetDistMode(void)const;	//1660
	char const *  GetEMailAddr(void)const;	//1685
	char const *  GetPassword(void)const;	//2132
	int  IsEMailEnabled(void)const;	//2797
	int  IsReceiptorOnly(void)const;	//2868
	void  SetDBDescr(char const *);	//3523
	void  SetDBDistInfo(enum DIST_MODE_tag,class CString,class CString,class CString,class CString,int,int,class CString,class CString);	//3524
	void  SetDBStatus(int);	//3530
	void  SetDBUUID(struct _GUID const &);	//3531
	void  SetEMailAddr(char const *);	//3561
	void  SetEMailEnabled(int);	//3562
	void  SetPassword(char const *);	//3679
	void  SetReceiptorOnly(int);	//3714
	void  Upload(class CDB7Stream &);	//4013
};


class IMPORT_1C CDB7SetInfo
{
public:
	 CDB7SetInfo(void);	//76
	 ~CDB7SetInfo(void);	//375
	void  ClearAll(void);	//982
	void  Download(class CDB7Stream &);	//1269
	class CDB7InfoArray &  GetChildInfo(void);	//1528
	struct _GUID const &  GetDBSetUUID(void)const;	//1610
	class CDB7Info &  GetInfo(int);	//1825
	class CDB7Info &  GetMyInfo(void);	//1967
	class CDB7Info &  GetParentInfo(void);	//2131
	int  GetPos(class CDBSign const &);	//2152
	int  IsSetEmpty(void)const;	//2875
	int  Read(void);	//3148
	void  SetDBSetUUID(struct _GUID const &);	//3526
	void  Upload(class CDB7Stream &);	//4014
	int  Write(void);	//4118
};


class IMPORT_1C CDB7Stream:public CObject
{
DECLARE_DYNAMIC(CDB7Stream)

public:
	 CDB7Stream(void);	//77
	virtual  ~CDB7Stream(void);	//376
	virtual void  AppendLogMessage(struct FullEventInfo *);	//902
	void  Checkpoint(void);	//979
	virtual void  Close(void);	//1010
	void  DcrLogLevel(void);	//1081
	int  EnableLog(int);	//1297
	virtual void  FlushLogFile(void);	//1377
	void  GetBeginOfList(void);	//1469
	int  GetComma(void);	//1561
	void  GetEndOfList(void);	//1692
	void  GetItem(char &);	//1831
	void  GetItem(short &);	//1832
	void  GetItem(int &);	//1833
	void  GetItem(long &);	//1834
	void  GetItem(struct _GUID &);	//1835
	void  GetItem(class CDBSign &);	//1836
	void  GetItem(class CDate &);	//1837
	void  GetItem(class CNumeric &);	//1838
	void  GetItem(class CObjID &);	//1839
	void  GetItem(class CString &,char const *);	//1840
	void  GetItem(class CValue &,int);	//1841
	int  GetLogLevel(void)const;	//1933
	int  GetMode(void)const;	//1966
	long  GetSinceCheckpoint(void)const;	//2435
	void  IncLogLevel(void);	//2666
	int  IsLogEnabled(void)const;	//2818
	virtual int  IsOpen(void)const;	//2848
	int  IsSpace(void)const;	//2876
	virtual void  NextChar(void);	//3050
	virtual int  Open(char const *,int);	//3075
	void  PutBeginOfList(void);	//3122
	void  PutEndOfList(void);	//3123
	void  PutItem(struct _GUID const &);	//3124
	void  PutItem(class CDBSign const &);	//3125
	void  PutItem(class CNumeric const &);	//3126
	void  PutItem(class CObjID const &);	//3127
	void  PutItem(class CValue const &,int);	//3128
	void  PutItem(char);	//3129
	void  PutItem(short);	//3130
	void  PutItem(int);	//3131
	void  PutItem(long);	//3132
	void  PutItem(char const *);	//3133
	void  PutItem(class CDate);	//3134
	virtual void  RollbackLogMessages(void);	//3248
	int  SetLogLevel(int);	//3629
	void  SkipGap(void);	//3811
	void  SkipListContent(void);	//3812
	void  SkipSpaces(void);	//3813
	virtual void  Write(char);	//4119
	virtual void  Write(char const *);	//4120
};


class IMPORT_1C CDB7StreamException:public CException
{
DECLARE_DYNAMIC(CDB7StreamException)

public:
	 CDB7StreamException(unsigned int);	//78
	 CDB7StreamException(char const *,unsigned int);	//79
	virtual  ~CDB7StreamException(void);	//377
	class CString   GetErrorDescr(void)const;	//1703
	unsigned int  GetHelpID(void)const;	//1802
};


class IMPORT_1C CDBDoctor
{
public:
	 CDBDoctor(void);	//80
	virtual  ~CDBDoctor(void);	//378
	int  ChangesMade(void);	//957
	static int  CheckASCII(unsigned char *,int);	//963
	static int  CheckNumeric(unsigned char *,int,int);	//972
	int  PackDB(void);	//3087
	int  ReindexAll(void);	//3212
	void  SetDDName(class CString const &);	//3532
	int  VerifyDB(int);	//4045
};


class IMPORT_1C CDBDoctorDbfField
{
public:
	 CDBDoctorDbfField(unsigned char *);	//81
	unsigned long  GetDataAddr(void);	//1618
	int  GetDecimal(void);	//1636
	int  GetLength(void);	//1879
	class CString const   GetName(void);	//2055
	char  GetType(void);	//2496
	void  SetFieldPos(unsigned char *);	//3578
	int  VerifyName(void);	//4069
	int  VerifyType(void);	//4072
};


class IMPORT_1C CDBDoctorDbfFile
{
public:
	 CDBDoctorDbfFile(class CTable *,class CString const &,int);	//82
	virtual  ~CDBDoctorDbfFile(void);	//379
	int  CheckFieldContent(long);	//969
	unsigned char *  GetFieldDet(int);	//1728
	int  GetFieldsNum(void);	//1737
	unsigned int  GetHeaderLen(void);	//1800
	unsigned long  GetNRec(void);	//2045
	unsigned int  GetRecordLen(void);	//2295
	unsigned char  GetVersion(void);	//2551
	int  IsOpen(void);	//2849
	int  ReadRecord(long);	//3171
	int  VerDate(void);	//4042
	int  VerVersion(void);	//4044
	int  WriteRecord(long);	//4139
};


class IMPORT_1C CDBDoctorRec
{
public:
	 CDBDoctorRec(char const *,int);	//83
	 ~CDBDoctorRec(void);	//380
	int  Delete(void);	//1176
	static class CString   Get1cTableName(int);	//1393
	long  GetCurRecordNum(void);	//1585
	long  GetFieldAsMDID(int);	//1720
	class CObjID   GetFieldAsObjID(int);	//1721
	void  GetFieldContent(int,class CByteArray &);	//1722
	void  GetFieldContent(class CMetaDataTypedObj *,class CByteArray &);	//1723
	void  GetFieldContent(char const *,class CByteArray &,int);	//1724
	class CString   GetFieldContentAsString(int);	//1725
	class CString   GetFieldContentAsString(class CMetaDataTypedObj *);	//1726
	class CString   GetFieldContentAsString(char const *);	//1727
	int  GetFieldInfo(class CString const &,int &,long &,int &,int &);	//1729
	class CString const   GetFieldNameAt(int);	//1734
	int  GetFieldNum(int,int);	//1735
	int  GetFieldNum(char const *);	//1736
	class CString const   GetTableName(void);	//2462
	struct SFixFldDcr *  GetTableStdFldsDcr(int);	//2465
	int  GotoFirst(void);	//2573
	int  GotoLast(void);	//2590
	int  GotoNext(void);	//2605
	long  GotoPos(long);	//2628
	int  GotoPrev(void);	//2640
	int  IsAccTurnOnlyNeed(void);	//2778
	int  ReIndex(void);	//3140
	static void  SetEnvStrings(class CString const &,class CString const &,class CString const &,class CString const &,class CString const &);	//3571
	void  SetMetaDataObj(class CMetaDataObj *);	//3641
	int  Update(void);	//3985
	int  VerifyDBBool(int,enum DCTR_FIELD_ACT);	//4046
	int  VerifyDBChar(int,char const *,enum DCTR_FIELD_ACT);	//4047
	int  VerifyDBCharMark(int,enum DCTR_FIELD_ACT);	//4048
	int  VerifyDBDate(int,enum DCTR_FIELD_ACT);	//4049
	int  VerifyDBDouble(int,enum DCTR_FIELD_ACT);	//4050
	int  VerifyDBFixField(int,int,void *,enum DCTR_FIELD_ACT);	//4051
	int  VerifyDBInt(int,class CDWordArray *,enum DCTR_FIELD_ACT);	//4052
	int  VerifyDBIntBool(int,enum DCTR_FIELD_ACT);	//4053
	int  VerifyDBLong(int,enum DCTR_FIELD_ACT);	//4054
	int  VerifyDBLongDouble(int,enum DCTR_FIELD_ACT);	//4055
	int  VerifyDBMDID(int,enum DCTR_FIELD_ACT);	//4056
	int  VerifyDBNumeric(int,enum DCTR_FIELD_ACT);	//4057
	int  VerifyDBObjID(int,enum DCTR_FIELD_ACT);	//4058
	int  VerifyDBSign(int,enum DCTR_FIELD_ACT);	//4059
	int  VerifyDBString(int,enum DCTR_FIELD_ACT);	//4060
	int  VerifyDBTime(int,enum DCTR_FIELD_ACT);	//4061
	int  VerifyDBType(int,enum DCTR_FIELD_ACT);	//4062
	int  VerifyDBUUID(int,enum DCTR_FIELD_ACT);	//4063
	int  VerifyDBValue(int,class CType *,enum DCTR_FIELD_ACT);	//4064
	int  VerifyFieldContent(class CType const &,int,int);	//4066
	int  VerifyFieldContent(class CType const &,class CMetaDataTypedObj *);	//4067
	int  VerifyFieldContent(class CType const &,char const *);	//4068
};


class IMPORT_1C CDataBase7:public CObject // Size OK & Parent OK & VFTable OK?
{
DECLARE_DYNAMIC(CDataBase7)

	friend class CService;

public:
	char size_buff[0x1C];

	virtual  ~CDataBase7(void);	//385

	virtual class CDDB7Service *  CreateDDB7Service(void) = 0;
	virtual int  AttachDDB7Service(class CDDB7Service *);
	virtual int  DoesDBExist(void) = 0;
	virtual int  Open(int,int,int,int,int) =0;
	virtual int  Close(void) =0;
	virtual int  IsOpen(void) =0;
	virtual int  IsOpenExclusive(void) =0;
	virtual void  SetLockWaitTime(int) =0;
	virtual int  BeginTransaction(void) =0;
	virtual int  CommitTransaction(void) =0;
	virtual int  RollbackTransaction(void) =0;
	virtual int  RollbackAllTransactions(void) =0;
	virtual int  GetNActiveTransactions(void) =0;
	virtual void *  GetRealDB(void) =0;
	virtual int CanBeCommited(void) =0;

	//?

	CDataBase7(class CMetaDataCont *);	//90
	class CDDB7Service *  GetDDB7Service(void)const;	//1617
	int  GetDataSourceType(void)const;	//1619
	static class CDataBase7 *  GetDefault(void);	//1640
	class CMetaDataCont *  GetMD(void)const;	//1936
	class CObjID &  GetMaxObjID(long);	//1954
	class CUniqueIDControlSet *  GetUIDCtrlSet(void)const;	//2519
	static int  GetUserdefChecksum(unsigned long &, int);	//2527
	void  InitUIDCtrlSet(void);	//2736
	int  IsMDSaveAvailable(int);	//2819
	int  IsStoredSeparately(class CMetaDataTypedObj const *)const;	//2877
	int  IsUploadAvailable(long,class CObjID const &,class CDBSign,int);	//2885
	int  IsUploadDeletedAvailable(long,class CObjID const &,class CDBSign);	//2887
	int  LockToOpenData(void);	//2978
	void  RegisterMDUpdate(void);	//3198
	void  RegisterUpdate(long,class CObjID const &,int,class CDBSign);	//3204
	void  RegisterUpdateLow(class CDBSign const &,long,class CObjID const &,int);	//3208
	void  RejectUpdate(long,class CObjID const &,class CDBSign);	//3215
	static void  SetConnectParamsCallback(void (__cdecl*)(void));	//3507
	static void  SetDefault(class CDataBase7 *);	//3545
	void  SetMaxObjID(long,class CObjID const &);	//3639
	void  SetOpenParamString(char const *);	//3661
	static void  SetUserdefFileName(char const *);	//3796
	void  UndoUIDCtrlSet(void);	//3959
	void  UnlockToOpenData(void);	//3973
	void  UnregisterAllUpdates(long,class CDBSign);	//3974
	void  UnregisterUpdates(class CDBSign const &,long,class CDBSign);	//3976
	static int  UserdefFileExists(void);	//4030
	static void (__cdecl*__cdecl GetConnectParamsCallback(void))(void);
protected:
	static class CString  m_sUserdefFileName;	//4236
};


class IMPORT_1C CDBEngDB7:public CDataBase7 // VFTable OK, Parent OK, size OK
{
DECLARE_DYNAMIC(CDBEngDB7)


public:
	//	void  `default constructor closure'(void);	//762
	char buff_size[0x24];
	virtual  ~CDBEngDB7(void);	//381
	virtual class CDDB7Service *  CreateDDB7Service(void);
	virtual int  AttachDDB7Service(class CDDB7Service *);
	virtual int  DoesDBExist(void);
	virtual int  Open(int,int,int,int,int);
	virtual int  Close(void);
	virtual int  IsOpen(void);
	virtual int  IsOpenExclusive(void);
	virtual void  SetLockWaitTime(int);
	virtual int  BeginTransaction(void);
	virtual int  CommitTransaction(void);
	virtual int  RollbackTransaction(void);
	virtual int  RollbackAllTransactions(void);
	virtual int  GetNActiveTransactions(void);
virtual int CanBeCommited(void);
	virtual void *  GetRealDB(void);


	CDBEngDB7(class CMetaDataCont *);	//84
	int  ChangeCodePage(int);	//949
	void  CloseDD(void);	//1015
	static int  ExtractCodePage(char const *);	//1319
	static int  FindCodePage(char const *);	//1359
	class CBlobService *  GetBlobService(void)const;	//1472
	static struct SDBEngCodePage *  GetCodePageAt(int);	//1554
	static char const *  GetCustomSortOrderFileName(void);	//1602
	static int  GetNCodePages(void);	//1994
	static struct SDBEngCodePage *  GetSelectedCodePage(void);	//2430
	static int  GetSelectedCodePageIdx(void);	//2431
	static int  LoadCustomSortOrder(void);	//2931
	int  OpenDD(void);	//3081
	static void  SelectCodePage(int);	//3369
	static void  SetCustomSortOrderFilePath(char const *);	//3522
private:
	static struct SDBEngCodePage *  m_pSelectedCodePage;	//4234
	static class CString  m_sCustomSortOrderFName;	//4235
};


class IMPORT_1C CDBEngDDB7Service:public CObject
{
DECLARE_DYNAMIC(CDBEngDDB7Service)

public:
	 CDBEngDDB7Service(void);	//85
	virtual  ~CDBEngDDB7Service(void);	//382
protected:
	void  ClearDDB7Tables(void);	//988
	int  CreateDDB7Tables(void);	//1058
	void  DeleteDDB7Tables(void);	//1204
public:
	virtual int  DetectMDChanged(class CDBSign,class CObjID const &);	//1229
	virtual int  DetectObjChanged(long,class CObjID const &,class CDBSign);	//1232
	virtual void  Download(class CDB7Stream &);	//1270
	virtual void  DownloadAcknowledgements(class CDBSign,class CDB7Stream &);	//1279
	virtual void  GetLastUploadedID(class CDBSign,class CObjID &);	//1875
	virtual int  MakeDownload(class CDBSign const &,class CObjID &);	//2990
	virtual int  ManageDB7Set(int,class CDB7Info const &);	//2995
	virtual int  NextChangedObj(class CObjID &);	//3048
	virtual int  OnInit(void);	//3058
	virtual void  OnUndo(void);	//3067
protected:
	int  OpenDDB7Tables(void);	//3082
	int  OpenSystemTable(void);	//3085
public:
	virtual int  ReadDB7SetInfo(class CDB7SetInfo &);	//3153
	virtual void  RegisterMDUpdate(void);	//3197
	virtual void  RegisterUpdate(long,class CObjID const &,int,class CDBSign const &);	//3203
	virtual void  RegisterUpdateLow(class CDBSign const &,long,class CObjID const &,int);	//3207
	virtual void  RegisterUploaded(class CObjID const &,class CDBSign);	//3210
	virtual void  RejectUpdate(long,class CObjID const &,class CDBSign);	//3214
	virtual int  SelectChangedObj(long,class CObjID const &,class CDBSign,class CObjID const &,int);	//3360
	virtual void  SelectChangedObjs(long,class CDBSign,class CObjID const &,int);	//3362
	virtual void  UnregisterUpdates(class CDBSign const &,long,class CDBSign const &);	//3975
	virtual void  Upload(class CDB7Stream &);	//4015
	virtual void  UploadAcknowledgements(class CDB7Stream &);	//4024
	virtual int  WriteDB7SetInfo(class CDB7SetInfo &);	//4127
};


class IMPORT_1C CDBException7:public CException
{
DECLARE_DYNAMIC(CDBException7)

public:
	 CDBException7(int,char const *,int);	//86
	virtual  ~CDBException7(void);	//383
	int  GetErrorCode(void)const;	//1702
	class CString   GetErrorDescr(void)const;	//1704
	int  IsErrorRecoverable(void)const;	//2807
	int  RetryTransaction(void);	//3242
};


class IMPORT_1C CDBSign
{
public:
	char Sign[4];
	 CDBSign(char const *);	//87
	 CDBSign(char const *,int);	//88
	class CDBSign &  operator=(class CDBSign const &);	//518
	int  operator==(class CDBSign const &)const;	//605
	int  operator!=(class CDBSign const &)const;	//612
	 operator char const *(void)const;	//623
	int  operator<(class CDBSign const &)const;	//632
	int  operator>(class CDBSign const &)const;	//635
//	void  `default constructor closure'(void);	//763
	class CDBSign const &  Empty(void);	//1286
};


class IMPORT_1C CDNdata
{
public:
	enum EDN15_2 {ednOne,ednTwo};
	 CDNdata(void);	//89
	virtual  ~CDNdata(void);	//384
	class CDate const &  GetDate(void)const;	//1620
	class CNumeric   GetDivisor(void)const;	//1661
	class CDate &  GetRefDate(void);	//2296
	class CNumeric &  GetRefDivisor(void);	//2301
	enum EDN15_2   GetRoundMode(void)const;	//2340
	int  SetFigure(int,class CString const &);	//3579
	int  SetRegister(class CString const &);	//3717
	void  SetRoundMode(enum CDNdata::EDN15_2);	//3730
};


class IMPORT_1C CDeNomination
{
public:
	 CDeNomination(void);	//91
	 ~CDeNomination(void);	//386
	int  CheckPeriod(class CDate const &);	//974
	void  ClearSets(void);	//1006
	class CString *  ComentText(class CString *);	//1016
	void  DeInit(void);	//1082
protected:
	class CValue &  Denominate(class CValue &,int);	//1220
	class CString &  DigText(class CString const &,class CString &);	//1235
public:
	int  DoesFigureInUse(class CRegFigureDef *);	//1251
	int  DoesNeedDN(class CRegDef *,class CDate const &,class CDate const &);	//1252
	int  DoesRegisterInUse(class CRegDef *,class CDate const &);	//1253
protected:
	int  EvalCondition(class CDNCompItem &,class CValue * *,class CValue * *);	//1311
	int  FindRegFigure(void);	//1372
	class CDocSet *  GetDocSet(void);	//1679
public:
	void  GetFiguresBefore(class CValue * *);	//1745
protected:
	class CSbCntSet *  GetSbcSet(class CSbCntTypeDef *);	//2409
public:
	int  Init(class CString &);	//2673
protected:
	int  Parse(class CString const &);	//3088
public:
	class CValue * *  RecalcFiguresAfter(class CValue * *,class CValue * *,int,int);	//3180
	void  Recalculate(int,class CValue * *,class CValue * *);	//3187
	void  RemoveAll(void);	//3222
};


class IMPORT_1C CDelRecData
{
public:
	 CDelRecData(class CDelRecData const &);	//92
	 CDelRecData(long,class CObjID const &);	//93
	 CDelRecData(class CMetaDataObj *,class CObjID const &);	//94
	class CDelRecData &  operator=(class CDelRecData const &);	//519
};


class IMPORT_1C CDocActions
{
public:
	 CDocActions(class CDataBase7 *);	//95
	 ~CDocActions(void);	//387
	class CDocActions &  operator=(class CDocActions const &);	//520
//	void  `default constructor closure'(void);	//764
	int  ClearDocActs(class CObjID,int,class CDate,int *,int,int,int *);	//994
	int  GetAct(void);	//1430
	class CRegActSet *  GetActSelectSet(void);	//1431
	class CRegActSet *  GetRegActSet(int);	//2305
	class CRegSet *  GetRegSet(int);	//2317
	int  GetSelectCurDoc(class CValue &);	//2414
	int  GetTotal(void);	//2485
	void  Init(void);	//2674
private:
	int  InitReg(int);	//2733
public:
	int  ReCalcDocActs(class CObjID,class CDate,int *,int,class CDate,int);	//3138
	int  SelectActs(int,class CRegFilter *,class CString,int &,class CDate,class CDate,class CEventTime,class CEventTime,int);	//3323
	int  SelectTotals(int,class CRegFilter *,class CDate,int,class CRegSet *);	//3438
	void  SetBufferSize(short);	//3486
	int  WriteCrossRefs(int,class CDocSet *,class CValue * *,class CValue * *);	//4124
	int  WriteCrossRefs(class CDocSet *);	//4125
	int  WriteDocAct(long,class CDocSet *,long,int *,class CValue * *,class CValue * *,class CValue * *,int,int);	//4130
};


class IMPORT_1C CDocDataLine
{
public:
	 CDocDataLine(int);	//96
	 ~CDocDataLine(void);	//388
	class CDocDataLine &  operator=(class CDocDataLine const &);	//521
	class CTypedValue *  operator[](int)const;	//619
	class CTypedValue *  GetAt(int)const;	//1459
	void  GetValues(class CTypedValue * *,int,int &);	//2549
	int  LinkFld(int,int,int);	//2908
	int  SetAt(int,class CValue const *);	//3479
};


class IMPORT_1C CDocDataLineArray
{
public:
	 CDocDataLineArray(void);	//97
	 ~CDocDataLineArray(void);	//389
	class CDocDataLine *  operator[](int)const;	//620
	int  AddLine(int,class CTypedValue * *);	//827
	int  Delete(int);	//1177
	int  DeleteAll(void);	//1192
	class CDocDataLine *  GetAt(int)const;	//1460
	int  GetLineNo(int);	//1882
	int  GetNewLineNo(void);	//2067
	int  GetSize(void);	//2437
	int  GetValues(int,class CTypedValue * *,int &);	//2550
	void  Insert(int,class CDocDataLine *);	//2740
	int  LinkAll(int,int);	//2895
	void  Remove(int);	//3219
	void  Renumber(void);	//3225
	void  SetDocDef(class CDocDef *);	//3557
	int  SetLineInOrder(int);	//3624
	void  SetMaxItemsInLine(int);	//3638
	void  Sort(class std::vector<struct std::pair<int,int>,class std::__alloc<1,0> > &);	//3815
	int  Store(int,int,class CTypedValue * *);	//3881
	int  TestLineNo(int);	//3900
};


class IMPORT_1C CDocDef:public CMetaDataObj
{
public:
	 CDocDef(class CItemList const &);	//98
	 CDocDef(long,char const *,char const *);	//99
	virtual  ~CDocDef(void);	//390
	int  AddHeadFldDef(class CDocHeadFldDef *);	//820
	int  AddTblFldDef(class CDocTblFldDef *);	//853
	virtual void  AddToList(class CItemList &)const;	//865
	void  DelHeadFldDef(int,int);	//1140
	void  DelHeadFldDef(class CDocHeadFldDef *,int);	//1141
	void  DelTblFldDef(int,int);	//1163
	void  DelTblFldDef(class CDocTblFldDef *,int);	//1164
	int  GetApplicationCode(void)const;	//1452
	int  GetAutoClearActions(void)const;	//1467
	int  GetBookKeepingEnable(void)const;	//1473
	int  GetCalcEnable(void)const;	//1492
	int  GetCheckNumUnique(void)const;	//1512
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1517
	virtual char const *  GetChildName(int,int)const;	//1532
	class CMetaDataRefObj *  GetDerivedDocs(void);	//1651
	int  GetDerivedDocsAll(void)const;	//1652
	long  GetDocNumDefID(void)const;	//1674
	class CString   GetDocNumPrefix(class CDate);	//1675
	int  GetEnableEditOper(void)const;	//1688
	int  GetEnablePosting(void)const;	//1691
	enum EExistOper   GetExistOper(void)const;	//1711
	int  GetHasHeadSQLTable(void);	//1784
	int  GetHasHeadTable(void);	//1785
	class CDocHeadFldDef *  GetHeadFldDef(long)const;	//1793
	class CDocHeadFldDef *  GetHeadFldDef(char const *)const;	//1794
	class CDocHeadFldDef *  GetHeadFldDefAt(int)const;	//1795
	int  GetHeadFldDefIdx(long)const;	//1796
	int  GetHeadFldDefIdx(class CDocHeadFldDef *)const;	//1797
	int  GetHeadFldDefIdx(char const *)const;	//1798
	char const *  GetHeadTableName(void)const;	//1799
	class CMetaDataObjArray *  GetHeads(void);	//1801
	long  GetJournal(void)const;	//1845
	static char const *  GetListFunctor(void);	//1894
	int  GetManagementEnable(void)const;	//1947
	virtual int  GetNChilds(void)const;	//1983
	int  GetNHeadFldDefs(void)const;	//2001
	virtual int  GetNProps(void)const;	//2021
	int  GetNTblFldDefs(void)const;	//2053
	int  GetNumAuto(void)const;	//2075
	int  GetNumEditType(void)const;	//2077
	int  GetNumLen(void)const;	//2079
	int  GetNumLineAuto(void)const;	//2081
	int  GetNumUnique(void)const;	//2082
	class CMetaDataObjDistr *  GetObjDistr(void);	//2090
	virtual char const *  GetPropName(int,int)const;	//2192
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2226
	class CMetaDataObjArray *  GetTables(void);	//2466
	class CDocTblFldDef *  GetTblFldDef(long)const;	//2468
	class CDocTblFldDef *  GetTblFldDef(char const *)const;	//2469
	class CDocTblFldDef *  GetTblFldDefAt(int)const;	//2470
	int  GetTblFldDefIdx(long)const;	//2471
	int  GetTblFldDefIdx(class CDocTblFldDef *)const;	//2472
	int  GetTblFldDefIdx(char const *)const;	//2473
	char const *  GetTblTableName(void)const;	//2475
	int  InsertHeadFldDef(int,class CDocHeadFldDef *);	//2759
	int  InsertTblFldDef(int,class CDocTblFldDef *);	//2772
	int  MoveHeadFldDef(class CDocHeadFldDef *,int);	//3013
	int  MoveTblFldDef(class CDocTblFldDef *,int);	//3023
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3306
	void  SetApplicationCode(int);	//3476
	void  SetAutoClearActions(int);	//3480
	void  SetBookKeepingEnable(int);	//3483
	void  SetCalcEnable(int);	//3491
	void  SetCheckNumUnique(int);	//3496
	void  SetDerivedDocsAll(int);	//3553
	void  SetDocNumDefID(long);	//3559
	void  SetEnableEditOper(int);	//3565
	void  SetEnablePosting(int);	//3570
	void  SetExistOper(enum EExistOper);	//3575
	void  SetHasHeadSQLTable(int);	//3597
	void  SetHasHeadTable(int);	//3598
	void  SetJournal(long);	//3618
	void  SetManagementEnable(int);	//3636
	void  SetNumAuto(int);	//3647
	void  SetNumEditType(int);	//3649
	void  SetNumLen(int);	//3651
	void  SetNumLineAuto(int);	//3653
	void  SetNumUnique(int);	//3655
	int  SortHeadFldDefs(void);	//3827
	int  SortTblFldDefs(void);	//3838
	virtual int  WhatIsIt(void)const;	//4085
};


class IMPORT_1C CDocFldDef: public CMetaDataTypedObj
{
public:
	 CDocFldDef(class CDocFldDef const &);	//100
	 CDocFldDef(class CItemList const &);	//101
	 CDocFldDef(long,char const *,char const *,class CType const &);	//102
	 CDocFldDef(class CMetaDataTypedObj const *);	//103
	virtual  ~CDocFldDef(void);	//391
	class CDocFldDef &  operator=(class CDocFldDef const &);	//522
	virtual void  AddToList(class CItemList &)const;	//866
};


class IMPORT_1C CDocHeadFldDef: public CDocFldDef
{
public:
	 CDocHeadFldDef(class CDocHeadFldDef const &);	//104
	 CDocHeadFldDef(class CItemList const &);	//105
	 CDocHeadFldDef(long,char const *,char const *,class CType const &);	//106
	 CDocHeadFldDef(class CMetaDataTypedObj const *);	//107
	virtual  ~CDocHeadFldDef(void);	//392
	class CDocHeadFldDef &  operator=(class CDocHeadFldDef const &);	//523
	static char const *  GetListFunctor(void);	//1895
	virtual int  WhatIsIt(void)const;	//4086
};


class IMPORT_1C CDocNumDef
{
public:
	 CDocNumDef(class CDocNumDef const &);	//108
	 CDocNumDef(class CItemList const &);	//109
	 CDocNumDef(long,char const *,char const *);	//110
	virtual  ~CDocNumDef(void);	//393
	class CDocNumDef &  operator=(class CDocNumDef const &);	//524
	virtual void  AddToList(class CItemList &)const;	//867
	int  GetCheckNumUnique(void)const;	//1513
	class CString   GetDocNumPrefix(class CDate);	//1676
	static char const *  GetListFunctor(void);	//1896
	virtual int  GetNProps(void)const;	//2022
	int  GetNumAuto(void)const;	//2076
	int  GetNumEditType(void)const;	//2078
	int  GetNumLen(void)const;	//2080
	int  GetNumUnique(void)const;	//2083
	virtual char const *  GetPropName(int,int)const;	//2193
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2227
	void  SetCheckNumUnique(int);	//3497
	void  SetNumAuto(int);	//3648
	void  SetNumEditType(int);	//3650
	void  SetNumLen(int);	//3652
	void  SetNumUnique(int);	//3656
	virtual int  WhatIsIt(void)const;	//4087
};


class IMPORT_1C CDocSelRefDef:public CMetaDataObj
{
public:
	 CDocSelRefDef(class CItemList const &);	//111
	 CDocSelRefDef(long,char const *,char const *);	//112
	virtual  ~CDocSelRefDef(void);	//394
//	void  `default constructor closure'(void);	//765
	virtual void  AddToList(class CItemList &)const;	//868
	class CType   GetHardType(void)const;	//1783
	static char const *  GetListFunctor(void);	//1897
	virtual int  GetNProps(void)const;	//2023
	virtual char const *  GetPropName(int,int)const;	//2194
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2228
	int  GetSaveEmptyValue(void)const;	//2392
	int  IsAllTypesSame(class CMetaDataCont *,class CMetaDataObj * *);	//2781
	int  IsTypeSame(class CType);	//2882
	void  SetHardType(class CType);	//3596
	void  SetSaveEmptyValue(int);	//3736
	virtual int  WhatIsIt(void)const;	//4088
};


class IMPORT_1C CDocSelRefHolder
{
public:
	 CDocSelRefHolder(class CMetaDataCont *);	//113
	 ~CDocSelRefHolder(void);	//395
	class CDocSelRefHolder &  operator=(class CDocSelRefHolder const &);	//525
	void  Done(void);	//1256
	class CDocSelRefItemArray *  GetCurrencyRefs(void);	//1591
	class longArray *  GetJournalsAt(int);	//1855
	class CDocSelRefItemArray *  GetOperRefsAt(int);	//2107
	class CDocSelRefItemArray *  GetPlanRefs(void);	//2151
	class CDocSelRefItemArray *  GetProvRefsAt(int);	//2266
	class CDocSelRefItemArray *  GetRefsAt(int);	//2304
	class CDocSelRefItemArray *  GetRegRefsAt(int);	//2316
	class CDocSelRefItemArray *  GetSbKindRefsAt(int);	//2406
	void  Init(void);	//2675
};


class IMPORT_1C CDocService
{
public:
	 CDocService(void);	//114
	 ~CDocService(void);	//396
	class CDocService &  operator=(class CDocService const &);	//527
	void  Done(void);	//1257
	class CString   GetDefaultPrefix(int);	//1645
	class CDocSet *  GetDocSet(void);	//1680
	class CString   GetPresent(long,class CObjID);	//2158
	class CDocSet *  Init(void);	//2676
	void  SetDefaultPrefix(int,class CString);	//3551
};


class IMPORT_1C CDocSet:public CUniSet
{
DECLARE_DYNAMIC(CDocSet)

public:
	 CDocSet(class CDataBase7 *);	//115
	virtual  ~CDocSet(void);	//397
//	void  `default constructor closure'(void);	//766
	void  AcceptRange(class CDate,class CDate,class CEventTime &,class CEventTime &);	//794
protected:
	void  BeginParams(void);	//912
public:
	void  ClearChangedFlags(void);	//986
	void  ClearDirty(void);	//991
	void  ClearParams(void);	//1001
	virtual int  Delete(enum MoveAfterDel);	//1178
protected:
	int  DeleteDoc(void);	//1206
public:
	int  DocNumLock(char const *,char const *);	//1244
	int  DocNumUnLock(char const *,char const *);	//1246
	virtual void  Download(class CDB7Stream &);	//1271
	void  EnableAutoRegister(int);	//1290
protected:
	void  EndParams(void);	//1300
public:
	virtual void  EscalateTLock(enum TLockLevel);	//1308
	class CDocDef *  GetCurDocDef(void);	//1583
	class CDocDef *  GetDocDef(void)const;	//1664
	int  GetDocDefIdx(void)const;	//1668
	class CEventTime   GetEventTime(void)const;	//1706
	int  GetKeyValueList(class PValueItemArray *,int,int,class CDate,class CDate);	//1868
	virtual long  GetNearPos(enum MoveAfterDel);	//2057
	static class CDocDef *  GetNewDocDef(void);	//2063
	static class CObjID &  GetNewDocID(void);	//2064
	class CString   GetPresent(void);	//2159
	class CDocTblRec *  GetTblRec(void);	//2474
	virtual int  GotoCurrent(void);	//2561
	virtual int  GotoFirst(void);	//2574
	virtual int  GotoLast(void);	//2591
	virtual int  GotoNext(void);	//2606
	virtual void  GotoNull(void);	//2619
	virtual long  GotoPos(long);	//2629
	virtual int  GotoPrev(void);	//2641
	void  GraphChanged(class CMetaDataTypedObj *);	//2649
	int  IncrementDocNum(class CString &,class CDocDef *);	//2670
protected:
	void  InitCross(void);	//2691
	void  InitKey(int);	//2701
public:
	virtual void  InitNew(long);	//2711
	virtual void  InitNew(void);	//2712
	int  IsAutoRegister(void)const;	//2782
	int  IsBookKeeping(void)const;	//2786
	int  IsCalc(void)const;	//2788
	int  IsClosed(void)const;	//2792
	int  IsGenJrnlParamReaded(int);	//2811
	virtual int  IsLocked(void);	//2816
	int  IsManagement(void)const;	//2820
	virtual int  IsMultSel(void);	//2828
	virtual int  IsOK(void);	//2841
	int  IsParamReaded(int);	//2859
	int  IsTblReaded(void);	//2878
	void  LinkAll(int,int);	//2896
	void  LinkAllGenFlds(int,int);	//2899
	void  LinkAllHeadFlds(int,int);	//2900
	void  LinkAllTblFlds(int,int);	//2906
	void  LinkGenFld(int,int,int);	//2909
	void  LinkHeadFld(int,int,int);	//2910
protected:
	int  LinkToJournal(void);	//2923
public:
	virtual int  Lock(void);	//2969
	int  MarkDelete(int);	//3001
	virtual int  New(void);	//3034
	void  ReadAllParams(void);	//3150
	int  ReadDoc(void);	//3155
	int  ReadDocWithCheck(int,int);	//3156
	void  ReadGenJrnlParam(int);	//3157
	void  ReadGenJrnlParam(class CGenJrnlFldDef *);	//3158
	void  ReadGenJrnlParamWithCheck(int);	//3159
	int  ReadHead(void);	//3160
private:
	int  ReadHeadPrivate(void);	//3161
public:
	void  ReadParam(int);	//3164
	void  ReadParam(class CDocHeadFldDef *);	//3165
	void  ReadParamWithCheck(int);	//3170
	int  ReadTbl(void);	//3172
	int  ReadTblFirst(void);	//3173
	int  ReadTblNext(void);	//3174
private:
	int  ReadTblPrivate(void);	//3175
public:
	int  RecalcTotals(void);	//3185
	int  RefreshPostingFlags(void);	//3191
	void  ResetDocStreamFlags(void);	//3235
	class CObjID   SearchFirstDocAfter(class CDate,class CEventTime,int,int,class CEventTime);	//3315
	int  SearchTimePlace(void);	//3316
	int  SearchTimePlaceAfter(class CDate,class CEventTime,long &);	//3317
	int  SelectAll(class CDate,class CDate,class CEventTime,class CEventTime);	//3328
	int  SelectDocDefID(long,class CDate,class CDate,class CEventTime,class CEventTime);	//3374
	int  SelectDocID(class CObjID);	//3376
	int  SelectDocNum(long,char const *,class CDate);	//3379
	int  SelectDocStream(class CDate,class CDate,class CEventTime,class CEventTime,long);	//3380
	int  SelectJournal(long,class CDate,class CDate,class CEventTime,class CEventTime);	//3396
	int  SelectParent(long,class CValue *,class CDate,class CDate,class CEventTime,class CEventTime);	//3406
	int  SelectParentChild(long,class CValue *,class CObjID);	//3409
	int  SelectPrefix(long,class CDate);	//3425
	int  SelectValue(class CValue *,int,class CDate,class CDate,class CEventTime,class CEventTime);	//3444
	void  SetAllParamDirty(void);	//3469
	virtual void  SetBufferSize(short);	//3487
protected:
	int  SetDef(void);	//3542
public:
	void  SetDocActionsLoader(class CDocActionsLoader *);	//3556
	void  SetDocStreamFlags(void);	//3560
	void  SetGenParamDirty(int);	//3592
	void  SetGenParamReaded(int);	//3593
	void  SetHeadParamDirty(int);	//3607
	void  SetHeadParamReaded(int);	//3608
	int  SetNewNum(class CString);	//3646
	void  SetParamDirty(class CMetaDataObj *);	//3672
	virtual void  SetRelPos(long,int);	//3722
	virtual int  SetSelectionFields(void);	//3757
	int  SmartUpdate(void);	//3814
	void  TblGraphsChanged(void);	//3889
	int  TestInRange(class CDate &,long &,class CObjID &);	//3898
	virtual int  TestSelectionFields(void);	//3911
private:
	int  TestSelectionFieldsPriv(class CJournalRec *,class CCrossDocRec *);	//3920
public:
	virtual int  Unlock(void);	//3967
	int  Update(int);	//3986
	virtual int  Update(void);	//3987
	int  UpdateInJournal(void);	//4002
	void  UpdateTotals(void);	//4007
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4016
	int  WriteAllParams(void);	//4121
	int  WriteCrossRefs(int,int);	//4126
	int  WriteDoc(int,int);	//4129
	int  WriteGenJrnlParam(int);	//4132
	int  WriteHead(int);	//4133
	int  WriteParam(int);	//4136
	int  WriteTbl(int);	//4140
	static class CObjID  s_NewDocID;	//4245
	static class CDocDef *  s_pNewDocDef;	//4254
};


class IMPORT_1C CDocStreamDef: public CMetaDataObj
{
public:
	 CDocStreamDef(class CItemList const &);	//116
	 CDocStreamDef(long,char const *,char const *);	//117
	virtual  ~CDocStreamDef(void);	//398
	virtual void  AddToList(class CItemList &)const;	//869
	int  GetByProgram(void)const;	//1475
	class CMetaDataRefObj *  GetDocs(void)const;	//1683
	static char const *  GetDocsListFunctor(void);	//1684
	class CString   GetFieldName(void)const;	//1730
	static char const *  GetListFunctor(void);	//1898
	virtual int  GetNProps(void)const;	//2024
	virtual char const *  GetPropName(int,int)const;	//2195
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2229
	class CMetaDataRefObj *  GetRegs(void)const;	//2320
	static char const *  GetRegsListFunctor(void);	//2321
	int  GetRuler(void)const;	//2344
	void  SetByProgram(int);	//3490
	void  SetRuler(int);	//3734
	virtual int  WhatIsIt(void)const;	//4089
};


class IMPORT_1C CDocStreamService
{
public:
	 CDocStreamService(class CDocStreamService const &);	//118
	 CDocStreamService(void);	//119
	 ~CDocStreamService(void);	//399
	class CDocStreamService &  operator=(class CDocStreamService const &);	//528
	void  AddAcc(class CObjID,long);	//800
	void  AddReg(long);	//846
	void  AfterPosting(int);	//899
	void  BeforePosting(class CDocSet *);	//911
	int  CheckPlan(long);	//975
	void  Done(void);	//1258
	void  GetDate(long,class CDate &,class CEventTime &);	//1621
	void  Init(void);	//2677
private:
	void  InitRec(void);	//2732
public:
	int  SetDate(long,class CDate const &,class CEventTime const &);	//3533
};


class IMPORT_1C CDocTblFldDef: public CDocFldDef
{
public:
	 CDocTblFldDef(class CDocTblFldDef const &);	//120
	 CDocTblFldDef(class CItemList const &);	//121
	 CDocTblFldDef(long,char const *,char const *,class CType const &);	//122
	 CDocTblFldDef(class CMetaDataTypedObj const *);	//123
	virtual  ~CDocTblFldDef(void);	//400
	class CDocTblFldDef &  operator=(class CDocTblFldDef const &);	//529
	virtual void  AddToList(class CItemList &)const;	//870
	int  GetHaveTotal(void)const;	//1792
	static char const *  GetListFunctor(void);	//1899
	virtual int  GetNProps(void)const;	//2025
	virtual char const *  GetPropName(int,int)const;	//2196
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2230
	void  SetHaveTotal(int);	//3606
	virtual int  WhatIsIt(void)const;	//4090
};


class IMPORT_1C CEnumDef: public CMetaDataObj
{
public:
	 CEnumDef(class CItemList const &);	//124
	 CEnumDef(long,char const *,char const *);	//125
	virtual  ~CEnumDef(void);	//401
	virtual void  AddToList(class CItemList &)const;	//871
	int  AddValDef(class CEnumValDef *);	//897
	void  DelValDef(int,int);	//1165
	void  DelValDef(class CEnumValDef *,int);	//1166
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1518
	virtual char const *  GetChildName(int,int)const;	//1533
	static char const *  GetListFunctor(void);	//1900
	int  GetMaxTitleLen(void);	//1959
	virtual int  GetNChilds(void)const;	//1984
	int  GetNValDefs(void)const;	//2054
	class CEnumValDef *  GetValDef(long)const;	//2531
	class CEnumValDef *  GetValDef(char const *)const;	//2532
	class CEnumValDef *  GetValDefAt(int)const;	//2533
	int  GetValDefIdx(long)const;	//2534
	int  GetValDefIdx(class CEnumValDef *)const;	//2535
	int  GetValDefIdx(char const *)const;	//2536
	class CMetaDataObjArray *  GetVals(void);	//2538
	int  InsertValDef(int,class CEnumValDef *);	//2773
	int  MoveValDef(class CEnumValDef *,int);	//3024
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3307
	void  SetMaxTitleLen(int);	//3640
	int  SortValDefs(void);	//3839
	virtual int  WhatIsIt(void)const;	//4091
};


class IMPORT_1C CEnumValDef : public CMetaDataObj
{
public:
	 CEnumValDef(class CEnumValDef const &);	//126
	 CEnumValDef(class CItemList const &);	//127
	 CEnumValDef(long,char const *,char const *);	//128
	virtual  ~CEnumValDef(void);	//402
	class CEnumValDef &  operator=(class CEnumValDef const &);	//530
	virtual void  AddToList(class CItemList &)const;	//872
	static char const *  GetListFunctor(void);	//1901
	virtual int  GetNProps(void)const;	//2026
	virtual char const *  GetPropName(int,int)const;	//2197
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2231
	char const *  GetTitle(void)const;	//2480
	void  SetTitle(char const *);	//3777
	virtual int  WhatIsIt(void)const;	//4092
};


class IMPORT_1C CEventTime
{
public:
	char buffer[12];
//DmitrO: buffer
	 CEventTime(class CEventTime const &);	//129
	 CEventTime(long);	//130
	 CEventTime(long,class CObjID const &);	//131
	 CEventTime(void);	//132
	class CEventTime &  operator=(class CEventTime const &);	//531
	class CEventTime &  operator=(long);	//532
	int  operator==(class CEventTime const &)const;	//606
	int  operator!=(class CEventTime const &)const;	//613
	int  operator<(class CEventTime const &)const;	//633
	int  operator>(class CEventTime const &)const;	//636
	class CObjID const &  GetEventID(void)const;	//1705
	long  GetlTime(void)const;	//2557
	void  SetEventID(class CObjID const &);	//3573
	void  SetlTime(long);	//3810
};


class IMPORT_1C CFFList : public CDWordArray
{
public:
	 CFFList(void);	//133
	virtual  ~CFFList(void);	//403
	int  Find(unsigned long);	//1347
};


class IMPORT_1C CFileNameSuffix
{
public:
	 CFileNameSuffix(class CFileNameSuffix const &);	//134
	 CFileNameSuffix(long);	//135
	 CFileNameSuffix(class CString);	//136
	class CFileNameSuffix &  operator=(class CFileNameSuffix &);	//533
//	void  `default constructor closure'(void);	//767
};


class IMPORT_1C CFldDefsArray
{
public:
	 CFldDefsArray(void);	//141
	 ~CFldDefsArray(void);	//405
	int  Add(class CFldDef *);	//796
	void  Del(int,int);	//1088
	void  Del(class CFldDef *,int);	//1089
	class CFldDef *  Get(long)const;	//1394
	class CFldDef *  Get(char const *)const;	//1395
	class CFldDef *  GetAt(int)const;	//1461
	int  GetIdx(long)const;	//1809
	int  GetIdx(class CFldDef *)const;	//1810
	int  GetIdx(char const *)const;	//1811
	char const *  GetListFunctor(void);	//1903
	class CMetaDataObjArray *  GetMetaDataArray(void);	//1961
	int  GetN(void)const;	//1968
	int  Insert(int,class CFldDef *);	//2741
	int  Move(class CFldDef *,int);	//3004
	int  Sort(void);	//3816
};


class IMPORT_1C CFormDef
{
public:
	 CFormDef(class CFormDef const &);	//142
	 CFormDef(class CItemList const &);	//143
	 CFormDef(long,char const *,char const *);	//144
	virtual  ~CFormDef(void);	//406
	class CFormDef &  operator=(class CFormDef const &);	//535
	virtual void  AddToList(class CItemList &)const;	//874
	static char const *  GetListFunctor(void);	//1904
	virtual int  WhatIsIt(void)const;	//4094
};


class IMPORT_1C CFormDefsArray
{
public:
	 CFormDefsArray(void);	//145
	 ~CFormDefsArray(void);	//407
	int  Add(class CFormDef *);	//797
	void  Del(int,int);	//1090
	void  Del(class CFormDef *,int);	//1091
	class CFormDef *  Get(long)const;	//1396
	class CFormDef *  Get(char const *)const;	//1397
	class CFormDef *  GetAt(int)const;	//1462
	long  GetDefaultChoiseForm(void)const;	//1641
	long  GetDefaultOpenForm(void)const;	//1643
	int  GetIdx(long)const;	//1812
	int  GetIdx(class CFormDef *)const;	//1813
	int  GetIdx(char const *)const;	//1814
	char const *  GetListFunctor(void);	//1905
	class CMetaDataObjArray *  GetMetaDataArray(void);	//1962
	int  GetN(void)const;	//1969
	int  Insert(int,class CFormDef *);	//2742
	int  Move(class CFormDef *,int);	//3005
	void  SetDefaultChoiseForm(long);	//3546
	void  SetDefaultOpenForm(long);	//3549
	int  Sort(void);	//3817
};


class IMPORT_1C CFreeOsn
{
public:
	 CFreeOsn(class CFreeOsn const &);	//146
	 CFreeOsn(int,int,long);	//147
	 CFreeOsn(int,int,class CString);	//148
	class CFreeOsn   operator=(long);	//536
	class CFreeOsn   operator=(class CString);	//537
	class CString   operator=(class CFreeOsn);	//538
	 operator class CString(void);	//624
	 operator long(void)const;	//625
	void  operator++(int);	//626
	void  operator++(void);	//627
	void  operator--(int);	//628
	void  operator--(void);	//629
	class CFreeOsn   operator+(long);	//630
	class CFreeOsn   operator+(class CFreeOsn);	//631
protected:
	void  FromString(class CString);	//1386
	void  Norm(void);	//3052
	class CString   ToString(void);	//3935
	void  VerPrm(void);	//4043
public:
	int  VerifyString(class CString);	//4070
};


class IMPORT_1C CGenJrnlFldDef:public CMetaDataObj
{
public:
	 CGenJrnlFldDef(class CGenJrnlFldDef const &);	//149
	 CGenJrnlFldDef(class CItemList const &);	//150
	 CGenJrnlFldDef(long,char const *,char const *,class CType const &);	//151
	 CGenJrnlFldDef(class CMetaDataTypedObj const *);	//152
	virtual  ~CGenJrnlFldDef(void);	//408
	class CGenJrnlFldDef &  operator=(class CGenJrnlFldDef const &);	//539
	int  GetInJournal(void);	//1816
	static char const *  GetListFunctor(void);	//1906
	virtual char const *  GetPropName(int,int)const;	//2199
	virtual int  WhatIsIt(void)const;	//4095
};

//<-CGroupContext(72/40)<-CBLContext(32/28)<-CObject(4/4)
class IMPORT_1C CGroupContext:public CBLContext
{
DECLARE_DYNCREATE(CGroupContext)

public:
	// полный размер объекта: 0x48
	CPtrArray m_ContextArray; // + 0x20
	DWORD m_nPropsCount; // +0x34
	DWORD m_nMethodsCount; // +0x38
	DWORD m_dwGroupContext3; // +0x3C
	DWORD m_dwGroupContext4; // +0x40
	CString m_strTypeString; // +0x44
	//CPtrArray m_ContextArray;
	//DWORD bufCGroupContext[0x100]; // так стоит у АльФ-а

	 CGroupContext(void);	//153
	virtual  ~CGroupContext(void);	//409
	int  Accept(void);	//787
	int  AddItem(class CBLContext *);	//822
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//936
	virtual int  CallAsProc(int,class CValue * *);	//938
	class CBLContext *  FindContextByMet(int &)const;	//1360
	class CBLContext *  FindContextByProp(int &)const;	//1361
	class CRefContext *  GetAt(int)const;	//1463
	class CBLContext *  GetContext(int)const;	//1576
	virtual char const *  GetMethodName(int,int)const;	//1965
	virtual int  GetNMethods(void)const;	//2006
	virtual int  GetNParams(int)const;	//2009
	virtual int  GetNProps(void)const;	//2028
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//2123
	virtual char const *  GetPropName(int,int)const;	//2200
	virtual int  GetPropVal(int,class CValue &)const;	//2233
	int  GetSize(void)const;	//2438
	virtual char const *  GetTypeString(void)const;	//2514
	virtual int  HasRetVal(int)const;	//2658
	virtual int  IsPropReadable(int)const;	//2864
	virtual int  IsPropWritable(int)const;	//2866
	virtual int  SetPropVal(int,class CValue const &);	//3695
	void  SetTypeString(char const *);	//3788
};


class IMPORT_1C CGroupDef: public CMetaDataObj
{
public:
	 CGroupDef(class CItemList const &);	//154
	 CGroupDef(long,char const *,char const *);	//155
	virtual  ~CGroupDef(void);	//410
	virtual void  AddToList(class CItemList &)const;	//875
	int  ContainAlg(long)const;	//1036
	void  DelAlgorithmID(long);	//1097
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1519
	virtual char const *  GetChildName(int,int)const;	//1534
	char const *  GetFieldName(void)const;	//1731
	static char const *  GetListFunctor(void);	//1907
	virtual int  GetNChilds(void)const;	//1985
	virtual int  GetNProps(void)const;	//2029
	virtual char const *  GetPropName(int,int)const;	//2201
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2234
	virtual int  WhatIsIt(void)const;	//4096
};


class IMPORT_1C CHolidaysDef: public CMetaDataObj
{
public:
	 CHolidaysDef(class CHolidaysDef const &);	//156
	 CHolidaysDef(class CItemList const &);	//157
	 CHolidaysDef(long,char const *,char const *);	//158
	virtual  ~CHolidaysDef(void);	//411
	class CHolidaysDef &  operator=(class CHolidaysDef const &);	//540
	virtual void  AddToList(class CItemList &)const;	//876
	virtual void  DescribeObject(class CString &,class CMetaDataCont *,class CString,int);	//1223
	static char const *  GetListFunctor(void);	//1908
	virtual int  GetNProps(void)const;	//2030
	class CMetaDataObjDistr *  GetObjDistr(void);	//2091
	virtual char const *  GetPropName(int,int)const;	//2202
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2235
	virtual int  WhatIsIt(void)const;	//4097
};


class IMPORT_1C CJournalDef: public CMetaDataObj
{
public:
	 CJournalDef(class CItemList const &);	//159
	 CJournalDef(long,char const *,char const *);	//160
	virtual  ~CJournalDef(void);	//412
	void  Accept(class CMetaDataCont *);	//788
	int  AddFldDef(class CJournalFldDef *);	//818
	virtual void  AddToList(class CItemList &)const;	//877
	void  DelFldDef(int,int);	//1135
	void  DelFldDef(class CJournalFldDef *,int);	//1136
	int  GetApplicationCode(void)const;	//1453
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1520
	virtual char const *  GetChildName(int,int)const;	//1535
	int  GetConsistAll(void)const;	//1563
	class CMetaDataRefObj *  GetDocConsist(void);	//1662
	class CMetaDataObjArray *  GetFilds(void);	//1746
	class CJournalFldDef *  GetFldDef(long)const;	//1753
	class CJournalFldDef *  GetFldDef(char const *)const;	//1754
	class CJournalFldDef *  GetFldDefAt(int)const;	//1755
	int  GetFldDefIdx(long)const;	//1756
	int  GetFldDefIdx(class CJournalFldDef *)const;	//1757
	int  GetFldDefIdx(char const *)const;	//1758
	class CFormDefsArray *  GetFormDefsArray(void);	//1763
	int  GetGeneral(void)const;	//1773
	static char const *  GetListFunctor(void);	//1909
	virtual int  GetNChilds(void)const;	//1986
	int  GetNFldDefs(void)const;	//1999
	virtual int  GetNProps(void)const;	//2031
	virtual char const *  GetPropName(int,int)const;	//2203
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2236
	int  InsertFldDef(int,class CJournalFldDef *);	//2757
	int  MoveFldDef(class CJournalFldDef *,int);	//3012
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3308
	void  SetApplicationCode(int);	//3477
	void  SetConsistAll(int);	//3508
	void  SetGeneral(int);	//3594
	int  SortFldDefs(void);	//3826
	virtual int  WhatIsIt(void)const;	//4098
};


class IMPORT_1C CJournalFldDef
{
public:
	 CJournalFldDef(class CItemList const &);	//161
	 CJournalFldDef(long,char const *,char const *);	//162
	virtual  ~CJournalFldDef(void);	//413
	virtual void  AddToList(class CItemList &)const;	//878
	static char const *  GetListFunctor(void);	//1910
	virtual int  GetNProps(void)const;	//2032
	virtual char const *  GetPropName(int,int)const;	//2204
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2237
	virtual int  WhatIsIt(void)const;	//4099
};


class IMPORT_1C CLnkLockCtrl
{
protected:
	 CLnkLockCtrl(char const *);	//163
	virtual  ~CLnkLockCtrl(void);	//414
public:
	virtual int  Lock(int,int);	//2970
};


class IMPORT_1C CLockCtrl
{
protected:
	 CLockCtrl(char const *);	//164
	virtual  ~CLockCtrl(void);	//415
public:
	virtual int  Lock(int,int);	//2971
	virtual int  LockDataObject(int,int,long);	//2976
private:
	int  LockOperation(void);	//2977
	int  OpenLockFile(void);	//3083
public:
	void  SetSharedLockLimit(int,int);	//3766
private:
	void  UnlockOperation(void);	//3972
};

class IMPORT_1C CMetaDataObjArray :public CPtrArray
{
public:
	//char buf[0x20];
	DWORD m_data;
	 CMetaDataObjArray(int);	//170
	 ~CMetaDataObjArray(void);	//420
	class CMetaDataObj *  operator[](int)const;	//621
//	void  `default constructor closure'(void);	//769
	int  AddItem(class CMetaDataObj *);	//823
	void  DeleteItem(int,int);	//1209
	void  DeleteItem(class CMetaDataObj *,int);	//1210
protected:
	class CPtrArray *  GetArray(void);	//1454
public:
	class CMetaDataObj *  GetAt(int)const;	//1464
	int  GetIndex(long)const;	//1818
	int  GetIndex(class CMetaDataObj *)const;	//1819
	int  GetIndex(char const *)const;	//1820
	class CMetaDataObj *  GetItem(long)const;	//1842
	class CMetaDataObj *  GetItem(char const *)const;	//1843
	int  GetNItems(void)const;	//2002
	int  InsertItem(int,class CMetaDataObj *);	//2760
	int  Move(class CMetaDataObj *,int);	//3006
	void  ScanMDObjects(class SScanMDObjects *);	//3311
	int  SortByCode(void);	//3819
};


class IMPORT_1C CMetaDataCont
{
public:
    DWORD m_data1[4]; //size 0x1F8 see to 0x2916AABC in config.dll (7.70.0.25)
	CTaskDef *m_pTaskDef;
	CMetaDataObjArray m_ConstDefs;
	CMetaDataObjArray m_STypeDefs;
	CMetaDataObjArray m_RegDefs;
	CMetaDataObjArray m_DocDefs;
	CMetaDataObjArray m_JournalDefs;
	CMetaDataObjArray m_EnumDefs;
	CMetaDataObjArray m_Defs5;
	CMetaDataObjArray m_Defs6;
	CMetaDataObjArray m_AlgorithmDefs;
	CMetaDataObjArray m_Defs8;
	CMetaDataObjArray m_CalcVarDefs;
	CMetaDataObjArray m_Defs10;
	CMetaDataObjArray m_CJDefs;
	CMetaDataObjArray m_GenJrnlFldDefs;
	CMetaDataObjArray m_Defs13;
	CMetaDataObjArray m_Defs14;
	CMetaDataObjArray m_Defs15;
	CHolidaysDef *m_pHolidaysDef;
    DWORD m_data2[14];
	CBuhDef *m_pBuhDef;
	CMetaDataCont** m_ppUnk;
	CString m_str1;
	CString m_str2;

public:
	 CMetaDataCont(void);	//165
	 ~CMetaDataCont(void);	//416
	int  Accept(void);	//789
	int  AcceptOnLoad(void);	//793
	int  AddAlgorithmDef(class CAlgorithmDef *);	//804
	int  AddCJDef(class CCJDef *);	//805
	int  AddCalcVarDef(class CCalcVarDef *);	//807
	int  AddCalendarTypeDef(class CCalendarTypeDef *);	//808
	int  AddConstDef(class CConstDef *);	//810
	int  AddDocDef(class CDocDef *);	//815
	int  AddEnumDef(class CEnumDef *);	//816
	int  AddGroupDef(class CGroupDef *);	//819
	int  AddJournalDef(class CJournalDef *);	//826
	int  AddRecalcRuleDef(class CRecalcRuleDef *);	//842
	int  AddRegDef(class CRegDef *);	//847
	int  AddReportDef(class CReportDef *);	//850
	int  AddSTypeDef(class CSbCntTypeDef *);	//852
	unsigned long  ConvertRightCodFromLoad(unsigned long);	//1041
	unsigned long  ConvertRightCodToStore(unsigned long);	//1042
	unsigned int  ConvertRightTypeFromLoad(unsigned int);	//1043
	unsigned int  ConvertRightTypeToStore(unsigned int);	//1044
	void  DelAlgorithmDef(int,int);	//1095
	void  DelAlgorithmDef(class CAlgorithmDef *,int);	//1096
	void  DelAllAlgorithmDefs(int);	//1100
	void  DelAllCJDefs(int);	//1101
	void  DelAllCalcVarDefs(int);	//1102
	void  DelAllCalendarTypeDefs(int);	//1103
	void  DelAllConstDefs(int);	//1104
	void  DelAllDocDefs(int);	//1106
	void  DelAllEnumDefs(int);	//1107
	void  DelAllGroupDefs(int);	//1108
	void  DelAllJournalDefs(int);	//1109
	void  DelAllRecalcRuleDefs(int);	//1111
	void  DelAllRegDefs(int);	//1113
	void  DelAllReportDefs(int);	//1114
	void  DelAllSTypeDefs(int);	//1115
	void  DelCJDef(int,int);	//1117
	void  DelCJDef(class CCJDef *,int);	//1118
	void  DelCalcVarDef(int,int);	//1121
	void  DelCalcVarDef(class CCalcVarDef *,int);	//1122
	void  DelCalendarTypeDef(int,int);	//1123
	void  DelCalendarTypeDef(class CCalendarTypeDef *,int);	//1124
	void  DelConstDef(int,int);	//1125
	void  DelConstDef(class CConstDef *,int);	//1126
	void  DelDocDef(int,int);	//1128
	void  DelDocDef(class CDocDef *,int);	//1129
	void  DelEnumDef(int,int);	//1131
	void  DelEnumDef(class CEnumDef *,int);	//1132
	void  DelGroupDef(int,int);	//1137
	void  DelGroupDef(class CGroupDef *,int);	//1138
	void  DelJournalDef(int,int);	//1142
	void  DelJournalDef(class CJournalDef *,int);	//1143
	void  DelRecalcRuleDef(int,int);	//1151
	void  DelRecalcRuleDef(class CRecalcRuleDef *,int);	//1152
	void  DelRegDef(int,int);	//1156
	void  DelRegDef(class CRegDef *,int);	//1157
	void  DelReportDef(int,int);	//1158
	void  DelReportDef(class CReportDef *,int);	//1159
	void  DelSTypeDef(int,int);	//1160
	void  DelSTypeDef(class CSbCntTypeDef *,int);	//1161
	void  DeleteAllData(int);	//1197
	void  DescribeMDCont(class CString &);	//1221
	int  ExistAcc(void);	//1314
	class CSbCntTypeDef *  FindAnySTypeDefChild(long);	//1353
	class CSbCntTypeDef *  FindAnySTypeDefChild(char const *);	//1354
	class CSbCntTypeDef *  FindAnySTypeDefChild(class CSbCntTypeDef const *);	//1355
	class CMetaDataObj *  FindObject(long)const;	//1367
	CMetaDataObjArrayTemplate<class CDocNumDef> *  GenDocNumDefs(void)const;	//1388
//DmitrO: GenDocNumDefs
	CMetaDataObjArrayTemplate<class CDocSelRefDef> *  GenDocSelRefs(void)const;	//1389
//DmitrO: GenDocSelRefs
	class CAlgorithmDef *  GetAlgorithmDef(long)const;	//1439
	class CAlgorithmDef *  GetAlgorithmDef(char const *)const;	//1440
	class CAlgorithmDef *  GetAlgorithmDefAt(int)const;	//1441
	int  GetAlgorithmDefIdx(long)const;	//1442
	int  GetAlgorithmDefIdx(class CAlgorithmDef *)const;	//1443
	int  GetAlgorithmDefIdx(char const *)const;	//1444
	class CMetaDataObjArray *  GetAlgorithmDefs(void);	//1445
	class CBuhDef *  GetBuhDef(void)const;	//1474
	class CCJDef *  GetCJDef(long)const;	//1477
	class CCJDef *  GetCJDef(char const *)const;	//1478
	class CCJDef *  GetCJDefAt(int)const;	//1479
	int  GetCJDefIdx(long)const;	//1480
	int  GetCJDefIdx(class CCJDef *)const;	//1481
	int  GetCJDefIdx(char const *)const;	//1482
	class CMetaDataObjArray *  GetCJDefs(void);	//1483
	int  GetCalcMaxDocNumLen(void);	//1493
	int  GetCalcMaxNumCodeLen(void);	//1494
	class CCalcVarDef *  GetCalcVarDef(long)const;	//1496
	class CCalcVarDef *  GetCalcVarDef(char const *)const;	//1497
	class CCalcVarDef *  GetCalcVarDefAt(int)const;	//1498
	int  GetCalcVarDefIdx(long)const;	//1499
	int  GetCalcVarDefIdx(class CCalcVarDef *)const;	//1500
	int  GetCalcVarDefIdx(char const *)const;	//1501
	class CMetaDataObjArray *  GetCalcVarDefs(void);	//1502
	class CCalendarTypeDef *  GetCalendarTypeDef(long)const;	//1503
	class CCalendarTypeDef *  GetCalendarTypeDef(char const *)const;	//1504
	class CCalendarTypeDef *  GetCalendarTypeDefAt(int)const;	//1505
	int  GetCalendarTypeDefIdx(long)const;	//1506
	int  GetCalendarTypeDefIdx(class CCalendarTypeDef *)const;	//1507
	int  GetCalendarTypeDefIdx(char const *)const;	//1508
	class CMetaDataObjArray *  GetCalendarTypeDefs(void);	//1509
	class CConstDef *  GetConstDef(long)const;	//1566
	class CConstDef *  GetConstDef(char const *)const;	//1567
	class CConstDef *  GetConstDefAt(int)const;	//1568
	int  GetConstDefIdx(long)const;	//1569
	int  GetConstDefIdx(class CConstDef *)const;	//1570
	int  GetConstDefIdx(char const *)const;	//1571
	class CMetaDataObjArray *  GetConstDefs(void);	//1572
	class CDocDef *  GetDocDef(long)const;	//1665
	class CDocDef *  GetDocDef(char const *)const;	//1666
	class CDocDef *  GetDocDefAt(int)const;	//1667
	int  GetDocDefIdx(long)const;	//1669
	int  GetDocDefIdx(class CDocDef *)const;	//1670
	int  GetDocDefIdx(char const *)const;	//1671
	class CMetaDataObjArray *  GetDocDefs(void);	//1672
	class CDocSelRefHolder *  GetDocSelRefHolder(void)const;	//1677
	CMetaDataObjArrayTemplate<class CDocStreamDef> *  GetDocStreamDefs(void)const;	//1681
	class CEnumDef *  GetEnumDef(long)const;	//1694
	class CEnumDef *  GetEnumDef(char const *)const;	//1695
	class CEnumDef *  GetEnumDefAt(int)const;	//1696
	int  GetEnumDefIdx(long)const;	//1697
	int  GetEnumDefIdx(class CEnumDef *)const;	//1698
	int  GetEnumDefIdx(char const *)const;	//1699
	class CMetaDataObjArray *  GetEnumDefs(void);	//1700
	int  GetExistDocEditSubcFlds(void)const;	//1710
	char const *  GetFullName(long,long *,int)const;	//1768
	CTypedFldDefsArray<class CGenJrnlFldDef> *  GetGenJrnlFlds(void)const;	//1771
	int  GetGenJrnlFldsIndexCnt(void)const;	//1772
	class CGroupDef *  GetGroupDef(long)const;	//1776
	class CGroupDef *  GetGroupDef(char const *)const;	//1777
	class CGroupDef *  GetGroupDefAt(int)const;	//1778
	int  GetGroupDefIdx(long)const;	//1779
	int  GetGroupDefIdx(class CGroupDef *)const;	//1780
	int  GetGroupDefIdx(char const *)const;	//1781
	class CMetaDataObjArray *  GetGroupDefs(void);	//1782
	class CHolidaysDef *  GetHolidaysDef(void)const;	//1803
	class CJournalDef *  GetJournalDef(long)const;	//1846
	class CJournalDef *  GetJournalDef(char const *)const;	//1847
	class CJournalDef *  GetJournalDefAt(int)const;	//1848
	int  GetJournalDefIdx(long)const;	//1849
	int  GetJournalDefIdx(class CJournalDef *)const;	//1850
	int  GetJournalDefIdx(char const *)const;	//1851
	class CMetaDataObjArray *  GetJournalDefs(void);	//1852
protected:
	static char const *  GetListFunctor(void);	//1911
public:
	char const *  GetLongCopyright(void);	//1935
	static int  GetMDListVersion(class CItemList const &);	//1938
	int  GetMDObjRights(class CDWordArray *,int,class CMetaDataObj *);	//1939
	class CMetaDataObj *  GetMDObject(int,char const *,char const *)const;	//1941
	int  GetMaxDocHeadFldsCnt(void)const;	//1949
	int  GetMaxDocNumLen(void)const;	//1950
	int  GetMaxDocTblFldsCnt(void)const;	//1951
	int  GetMaxFigureCnt(void)const;	//1952
	int  GetMaxPropCnt(void)const;	//1955
	int  GetMaxRegFldCnt(void)const;	//1956
	int  GetMaxSbLevelsLimit(void)const;	//1957
	class CMetaDataObjDistr *  GetMetaDataObjDistr(long);	//1963
	int  GetNAlgorithmDefs(void)const;	//1975
	int  GetNCJDefs(void)const;	//1976
	int  GetNCalcVarDefs(void)const;	//1978
	int  GetNCalendarTypeDefs(void)const;	//1979
	int  GetNConstDefs(void)const;	//1995
	int  GetNDocDefs(void)const;	//1996
	int  GetNEnumDefs(void)const;	//1997
	int  GetNGroupDefs(void)const;	//2000
	int  GetNJournalDefs(void)const;	//2004
	int  GetNRecalcRuleDefs(void)const;	//2046
	int  GetNRegDefs(void)const;	//2048
	int  GetNReportDefs(void)const;	//2049
	int  GetNSTypeDefs(void)const;	//2050
	int  GetNewDBVersion(void)const;	//2062
	long  GetNewID(void);	//2065
	int  GetNewVersion(void)const;	//2069
	int  GetOldDBVersion(void)const;	//2098
	int  GetOldVersion(void)const;	//2101
	class CRecalcRuleDef *  GetRecalcRuleDef(long)const;	//2287
	class CRecalcRuleDef *  GetRecalcRuleDef(char const *)const;	//2288
	class CRecalcRuleDef *  GetRecalcRuleDefAt(int)const;	//2289
	int  GetRecalcRuleDefIdx(long)const;	//2290
	int  GetRecalcRuleDefIdx(class CRecalcRuleDef *)const;	//2291
	int  GetRecalcRuleDefIdx(char const *)const;	//2292
	class CMetaDataObjArray *  GetRecalcRuleDefs(void);	//2293
	class CRegDef *  GetRegDef(long)const;	//2307
	class CRegDef *  GetRegDef(char const *)const;	//2308
	class CRegDef *  GetRegDefAt(int)const;	//2311
	int  GetRegDefIdx(long)const;	//2312
	int  GetRegDefIdx(class CRegDef *)const;	//2313
	int  GetRegDefIdx(char const *)const;	//2314
	class CMetaDataObjArray *  GetRegDefs(void);	//2315
	class CReportDef *  GetReportDef(long)const;	//2328
	class CReportDef *  GetReportDef(char const *)const;	//2329
	class CReportDef *  GetReportDefAt(int)const;	//2330
	int  GetReportDefIdx(long)const;	//2331
	int  GetReportDefIdx(class CReportDef *)const;	//2332
	int  GetReportDefIdx(char const *)const;	//2333
	class CMetaDataObjArray *  GetReportDefs(void);	//2334
	int  GetRightID(char const *);	//2338
	class CString   GetRightName(int,int,int);	//2339
	class CSbCntTypeDef *  GetSTypeDef(long)const;	//2382
	class CSbCntTypeDef *  GetSTypeDef(char const *)const;	//2383
	class CSbCntTypeDef *  GetSTypeDefAt(int)const;	//2384
	int  GetSTypeDefIdx(long)const;	//2385
	int  GetSTypeDefIdx(class CSbCntTypeDef *)const;	//2386
	int  GetSTypeDefIdx(char const *)const;	//2387
	class CSbCntTypeDef *  GetSTypeDefParent(long);	//2388
	class CSbCntTypeDef *  GetSTypeDefParent(char const *);	//2389
	class CSbCntTypeDef *  GetSTypeDefParent(class CSbCntTypeDef const *);	//2390
	class CMetaDataObjArray *  GetSTypeDefs(void);	//2391
	char const *  GetShortCopyright(void);	//2434
	class CTaskDef *  GetTaskDef(void)const;	//2467
	void  GetUUID(struct _GUID &)const;	//2520
	int  InsertAlgorithmDef(int,class CAlgorithmDef *);	//2745
	int  InsertCJDef(int,class CCJDef *);	//2747
	int  InsertCalcVarDef(int,class CCalcVarDef *);	//2749
	int  InsertCalendarTypeDef(int,class CCalendarTypeDef *);	//2750
	int  InsertConstDef(int,class CConstDef *);	//2753
	int  InsertDocDef(int,class CDocDef *);	//2754
	int  InsertEnumDef(int,class CEnumDef *);	//2755
	int  InsertGroupDef(int,class CGroupDef *);	//2758
	int  InsertJournalDef(int,class CJournalDef *);	//2762
	int  InsertRecalcRuleDef(int,class CRecalcRuleDef *);	//2766
	int  InsertRegDef(int,class CRegDef *);	//2769
	int  InsertReportDef(int,class CReportDef *);	//2770
	int  InsertSTypeDef(int,class CSbCntTypeDef *);	//2771
	int  LoadAlgorithmDefs(class CItemList const &);	//2925
	int  LoadBuhDef(class CItemList const &);	//2926
	int  LoadCJParamDefs(class CItemList const &);	//2927
	int  LoadCalcVarDefs(class CItemList const &);	//2928
	int  LoadCalendarTypeDefs(class CItemList const &);	//2929
	int  LoadConstDefs(class CItemList const &);	//2930
	int  LoadData(char const *);	//2932
	int  LoadDocDefs(class CItemList const &);	//2933
	int  LoadDocStreamDefs(class CItemList const &);	//2934
	int  LoadEnumDefs(class CItemList const &);	//2935
	int  LoadFromList(class CItemList const &);	//2939
	int  LoadGenJrnlDefs(class CItemList const &);	//2952
	int  LoadGroupDefs(class CItemList const &);	//2953
	int  LoadJournalDefs(class CItemList const &);	//2954
protected:
	int  LoadMainDefs(class CItemList const &);	//2955
public:
	int  LoadRecalcRuleDefs(class CItemList const &);	//2958
	int  LoadRegDefs(class CItemList const &);	//2959
	int  LoadReportDefs(class CItemList const &);	//2960
	int  LoadSTypeDefs(class CItemList const &);	//2961
	int  LoadTaskDefs(class CItemList const &);	//2962
	int  MoveConstDef(class CConstDef *,int);	//3008
	int  MoveDocDef(class CDocDef *,int);	//3009
	int  MoveEnumDef(class CEnumDef *,int);	//3010
	int  MoveJournalDef(class CJournalDef *,int);	//3014
	int  MoveRegDef(class CRegDef *,int);	//3019
	int  MoveReportDef(class CReportDef *,int);	//3020
	int  MoveSTypeDef(class CSbCntTypeDef *,int);	//3022
	void  SaveAlgorithmDefs(class CItemList &)const;	//3263
	void  SaveBuhDef(class CItemList &)const;	//3264
	void  SaveCJParamDefs(class CItemList &)const;	//3265
	void  SaveCalcVarDefs(class CItemList &)const;	//3266
	void  SaveCalendarTypeDefs(class CItemList &)const;	//3267
	void  SaveConstDefs(class CItemList &)const;	//3268
	int  SaveData(char const *)const;	//3269
	void  SaveDocDefs(class CItemList &)const;	//3270
	void  SaveDocStreamDefs(class CItemList &)const;	//3271
	void  SaveEnumDefs(class CItemList &)const;	//3272
	void  SaveGenJrnlDefs(class CItemList &)const;	//3273
	void  SaveGroupDefs(class CItemList &)const;	//3274
	void  SaveJournalDefs(class CItemList &)const;	//3275
protected:
	void  SaveMainDefs(class CItemList &)const;	//3276
public:
	void  SaveRecalcRuleDefs(class CItemList &)const;	//3278
	void  SaveRegDefs(class CItemList &)const;	//3279
	void  SaveReportDefs(class CItemList &)const;	//3280
	void  SaveSTypeDefs(class CItemList &)const;	//3281
	void  SaveTaskDefs(class CItemList &)const;	//3282
	void  SaveToList(class CItemList &)const;	//3287
	void  ScanMDObjects(class SScanMDObjects *);	//3309
	void  SetLongCopyright(char const *);	//3630
	void  SetShortCopyright(char const *);	//3767
	void  SetUUID(struct _GUID const &);	//3790
	int  SortConstDefs(void);	//3821
	int  SortDocDefs(void);	//3823
	int  SortEnumDefs(void);	//3824
	int  SortJournalDefs(void);	//3828
	int  SortRegDefs(void);	//3835
	int  SortReportDefs(void);	//3836
	int  SortSTypeDefs(void);	//3837
	void  SyncVerison(void);	//3886
	int  TestRefers(long,class CArray<CMetaDataObj*,CMetaDataObj*>&)const;	//3901
	void  TestRefersForOne(long,class CArray<CMetaDataObj*,CMetaDataObj*>&)const;	//3902
	int  TestValidRight(int,int,class CMetaDataObj *);	//3930
	void  UpdateDBVersion(void);	//4000
};


class IMPORT_1C CMetaDataFileService
{
public:
	 CMetaDataFileService(char const *,char const *,char const *,int);	//166
	 ~CMetaDataFileService(void);	//417
	char const *  ConstRead(long,class CObjID,class CDate,long,class CObjID,char *);	//1027
	void  ConstReadValue(class CTypedValue *,char const *,int,char *);	//1028
	int  ConstValueConversion(long,class CObjID,class CMetaDataTypedObj *,class CMetaDataTypedObj *,int,int,class CTypedValue *,int);	//1029
	int  ConstValueDelete(long,class CObjID);	//1031
	void  ConstWrite(long,class CObjID,class CDate,class CTypedValue *,long,class CObjID,int,int,int);	//1033
	static int  ConvMDtoDD(class CMetaDataCont *,class CString);	//1037
	int  CopyCJTable(char const *);	//1045
	enum SuccessCode   DataStructureModify(class CMetaDataCont *,class CMetaDataCont *,int (__cdecl*)(char const *),int);	//1079
	int  InitBlobTable(int);	//2686
	int  InitCJPropTable(void);	//2687
	int  InitConsTable(int);	//2689
	int  InitCrossRefTable(int);	//2692
	int  InitJournalTable(void);	//2699
	int  InitOldJournalTable(int);	//2723
	int  InitOperTable(void);	//2725
	int  InitUniqueIDTable(void);	//2737
private:
	void  LongTransaction(void);	//2983
public:
	int  ProcessAccount(enum SuccessCode &,class CPlanDef *,class CPlanDef *,class CAccRec &,class CAccRec &,class CMetaDataCont *,class CMetaDataCont *,class intArray &,class intArray &,class SAccSbKindInfoArray &,int);	//3098
	int  ProcessAccs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3100
	int  ProcessCJParams(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3102
	int  ProcessCalendars(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3104
	int  ProcessDistrDB(enum SuccessCode &);	//3106
	int  ProcessDocs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3108
	int  ProcessOemToAnsi(enum SuccessCode &,class CMetaDataCont *);	//3110
	int  ProcessOpers(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3111
	int  ProcessProvs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *,int,int,int);	//3114
	int  ProcessRegs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3116
	int  ProcessSubcs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3118
	int  ReindexTable(char const *);	//3213
	enum SuccessCode   RetryCopyNewData(void);	//3240
private:
	void  StartLongTransaction(void);	//3855
	void  StartTransaction(void);	//3857
	void  StopLongTransaction(void);	//3871
	void  StopTransaction(void);	//3873
public:
	int  TableAccess(int,char const *);	//3887
	int  TableExist(int,char const *);	//3888
};


class IMPORT_1C CMetaDataGenericService
{
public:
	 CMetaDataGenericService(char const *,char const *,char const *,char const *,int);	//167
	 ~CMetaDataGenericService(void);	//418
	int  FlushLogFile(void);	//1378
	void  Trace(char const *,enum TraceTypes);	//3938
	int  TypeConversion(class CTypedValue &,class CTypedValue &,class CMetaDataTypedObj *,class CMetaDataTypedObj *,int);	//3942
	int  WriteRecalcCommand(class CString);	//4137
	int  WriteRecalcCommandDeletedType(int,long,long);	//4138
};


class IMPORT_1C CMetaDataObjDistr
{
public:
	 CMetaDataObjDistr(class CMetaDataObjDistr const &);	//171
	 CMetaDataObjDistr(int,char const *,int);	//172
	 ~CMetaDataObjDistr(void);	//421
	class CMetaDataObjDistr const &  operator=(class CMetaDataObjDistr const &);	//542
	int  operator==(class CMetaDataObjDistr const &)const;	//607
	int  operator!=(class CMetaDataObjDistr const &)const;	//614
//	void  `default constructor closure'(void);	//770
	void  AddToList(class CItemList &)const;	//880
	static int  Char2Ruler(char);	//959
	char const *  GetAdditDBSigns(void)const;	//1437
	void  GetAdditDBSigns(class CString &)const;	//1438
	static char const *  GetListFunctor(void);	//1912
	int  GetRuler(void)const;	//2345
	int  IsAutoRegister(void)const;	//2783
	int  IsInAdditDBSigns(char const *)const;	//2812
	void  LoadFromList(class CItemList const &);	//2941
	int  MigrationChanged(class CMetaDataObjDistr const &)const;	//3003
	static char  Ruler2Char(int);	//3253
	void  SaveToList(class CItemList &)const;	//3288
	void  Set(class CMetaDataObjDistr const *);	//3449
	void  SetAdditDBSigns(char const *);	//3466
	void  SetAutoRegister(int);	//3481
	void  SetRuler(int);	//3735
};


class IMPORT_1C CMetaDataRef
{
public:
	 CMetaDataRef(class CMetaDataRef const &);	//173
	 CMetaDataRef(class CItemList const &);	//174
	 CMetaDataRef(long);	//175
	 ~CMetaDataRef(void);	//422
	class CMetaDataRef &  operator=(class CMetaDataRef const &);	//543
//	void  `default constructor closure'(void);	//771
	virtual void  AddToList(class CItemList &)const;	//881
	long  GetID(void)const;	//1806
	class CString   GetString(void)const;	//2446
	void  SetID(long);	//3610
};


class IMPORT_1C CMetaDataRefArray
{
public:
	 CMetaDataRefArray(void);	//176
	 ~CMetaDataRefArray(void);	//423
	class CMetaDataRef *  operator[](int)const;	//622
	int  AddItem(class CMetaDataRef *);	//824
	void  DeleteItem(int,int);	//1211
	void  DeleteItem(class CMetaDataRef *,int);	//1212
protected:
	class CPtrArray *  GetArray(void);	//1455
public:
	class CMetaDataRef *  GetAt(int)const;	//1465
	int  GetIndex(long)const;	//1821
	int  GetIndex(class CMetaDataRef *)const;	//1822
	class CMetaDataRef *  GetItem(long)const;	//1844
	int  GetNItems(void)const;	//2003
	int  InsertItem(int,class CMetaDataRef *);	//2761
};


class IMPORT_1C CMetaDataSQLService
{
public:
	 CMetaDataSQLService(char const *,char const *,char const *,int);	//179
	 ~CMetaDataSQLService(void);	//425
	int  ConstValueConversion(long,class CObjID,class CMetaDataTypedObj *,class CMetaDataTypedObj *,int,int,class CTypedValue *,int);	//1030
	int  ConstValueDelete(long,class CObjID);	//1032
	static int  ConvMDtoDDSQL(class CMetaDataCont *,class CString);	//1038
	void  CopySQLConstRec(class CSQLConstRec *,class CSQLConstRec *,class CMetaDataTypedObj *,class CMetaDataTypedObj *);	//1050
	enum SuccessCode   DataStructureModify(class CMetaDataCont *,class CMetaDataCont *,int (__cdecl*)(char const *),int);	//1080
	int  InitCJPropTable(class CSQLCJPropRec * *);	//2688
	int  InitConsTable(int);	//2690
	int  InitCrossRefTable(int);	//2693
	int  InitJournalTable(void);	//2700
	int  InitOperTable(void);	//2726
	int  InitUniqueIDTable(void);	//2738
private:
	void  LongTransaction(void);	//2984
public:
	int  ProcessAccount(enum SuccessCode &,class CPlanDef *,class CPlanDef *,class CSQLAccRec &,class CSQLAccRec &,class CMetaDataCont *,class CMetaDataCont *,class intArray &,class intArray &,class SAccSbKindInfoArray &);	//3099
	int  ProcessAccs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3101
	int  ProcessCJParams(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3103
	int  ProcessCalendars(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3105
	int  ProcessDistrDB(enum SuccessCode &);	//3107
	int  ProcessDocs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3109
	int  ProcessOpers(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3112
	int  ProcessProvs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *,int,int,int);	//3115
	int  ProcessRegs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3117
	int  ProcessSubcs(enum SuccessCode &,class CMetaDataCont *,class CMetaDataCont *);	//3119
	enum SuccessCode   RetryCopyNewData(void);	//3241
private:
	void  StartLongTransaction(void);	//3856
	void  StartTransaction(void);	//3858
	void  StopLongTransaction(void);	//3872
	void  StopTransaction(void);	//3874
};


class IMPORT_1C CODBCConnectParams
{
public:
	 CODBCConnectParams(class CODBCConnectParams const &);	//183
	 CODBCConnectParams(void);	//184
	 ~CODBCConnectParams(void);	//427
	class CODBCConnectParams const &  operator=(class CODBCConnectParams const &);	//545
	int  operator==(class CODBCConnectParams const &)const;	//608
	int  operator!=(class CODBCConnectParams const &)const;	//615
	int  FromFile(char const *);	//1384
	char const *  GetDBName(void)const;	//1609
	char const *  GetPassword(void)const;	//2133
	char const *  GetServerName(void)const;	//2432
	char const *  GetUID(void)const;	//2518
	unsigned long  GetUserdefChecksum(void)const;	//2528
	int  IsEmpty(void)const;	//2801
	void  Reset(void);	//3229
	void  SetDBName(char const *);	//3525
	void  SetPassword(char const *);	//3680
	void  SetServerName(char const *);	//3765
	void  SetUID(char const *);	//3789
	void  SetUserdefChecksum(unsigned long);	//3795
	int  ToFile(char const *);	//3933
private:
    CString m_Server;
    CString m_Database;
    CString m_User;
    CString m_Password;
    DWORD m_Checksum;
};


class IMPORT_1C CODBCDB7:public CDataBase7 // VFTable OK & Size OK & Parent OK
{
DECLARE_DYNAMIC(CODBCDB7)

public:
	char size_buff[0x20];
	 CODBCDB7(class CMetaDataCont *);	//185
	virtual  ~CODBCDB7(void);	//428
//	void  `default constructor closure'(void);	//774

	virtual class CDDB7Service *  CreateDDB7Service(void);
	virtual int  AttachDDB7Service(class CDDB7Service *);
	virtual int  DoesDBExist(void);
	virtual int  Open(int,int,int,int,int);
	virtual int  Close(void);
	virtual int  IsOpen(void);
	virtual int  IsOpenExclusive(void);
	virtual void  SetLockWaitTime(int);
	virtual int  BeginTransaction(void);
	virtual int  CommitTransaction(void);
	virtual int  RollbackTransaction(void);
	virtual int  RollbackAllTransactions(void);
	virtual int  GetNActiveTransactions(void);
virtual int CanBeCommited(void);
	virtual void *  GetRealDB(void);

	int  CheckConnectParams(class CODBCConnectParams &);	//966
	int  GetDescrFileNames(class CString &,class CString &)const;	//1656
	int  GetServerType(void)const;	//2433
};


class IMPORT_1C CODBCDDB7Service:public CObject
{
DECLARE_DYNAMIC(CODBCDDB7Service)

public:
	 CODBCDDB7Service(void);	//186
	virtual  ~CODBCDDB7Service(void);	//429
protected:
	void  ClearDDB7Tables(void);	//989
	int  CreateDDB7Objects(void);	//1054
	void  DeleteDDB7Objects(void);	//1203
public:
	virtual int  DetectMDChanged(class CDBSign,class CObjID const &);	//1230
	virtual int  DetectObjChanged(long,class CObjID const &,class CDBSign);	//1233
	virtual void  Download(class CDB7Stream &);	//1272
	virtual void  DownloadAcknowledgements(class CDBSign,class CDB7Stream &);	//1280
	virtual void  GetLastUploadedID(class CDBSign,class CObjID &);	//1876
	virtual int  MakeDownload(class CDBSign const &,class CObjID &);	//2991
	virtual int  ManageDB7Set(int,class CDB7Info const &);	//2996
	virtual int  NextChangedObj(class CObjID &);	//3049
	virtual int  OnInit(void);	//3059
	virtual void  OnUndo(void);	//3068
	virtual int  ReadDB7SetInfo(class CDB7SetInfo &);	//3154
	virtual void  RegisterMDUpdate(void);	//3199
	virtual void  RegisterUpdate(long,class CObjID const &,int,class CDBSign const &);	//3205
	virtual void  RegisterUpdateLow(class CDBSign const &,long,class CObjID const &,int);	//3209
	virtual void  RegisterUploaded(class CObjID const &,class CDBSign);	//3211
	virtual void  RejectUpdate(long,class CObjID const &,class CDBSign);	//3216
	virtual int  SelectChangedObj(long,class CObjID const &,class CDBSign,class CObjID const &,int);	//3361
	virtual void  SelectChangedObjs(long,class CDBSign,class CObjID const &,int);	//3363
	virtual void  UnregisterUpdates(class CDBSign const &,long,class CDBSign const &);	//3977
	virtual void  Upload(class CDB7Stream &);	//4017
	virtual void  UploadAcknowledgements(class CDB7Stream &);	//4025
protected:
	int  VerifyDDB7Objects(void);	//4065
	int  VerifySystemSet(void);	//4071
public:
	virtual int  WriteDB7SetInfo(class CDB7SetInfo &);	//4128
};


class IMPORT_1C CODBCQuery:public CObject //Parent OK & Size OK
{
DECLARE_DYNAMIC(CODBCQuery)

public:
	char buff_size[0x4000];
	 CODBCQuery(class CODBCDB7 *);	//187
	virtual  ~CODBCQuery(void);	//430
	virtual void  OnNextRow(void);	//3063

	//	void  `default constructor closure'(void);	//775
	void  Close(void);	//1013
	int  DescribeParam(int,char const *,char const *);	//1225
	int  FindField(char const *)const;	//1363
	class CODBCDB7 *  GetDB7(void)const;	//1603
	int  GetDate(int,class CDate &);	//1622
	int  GetLong(int,long &);	//1934
	int  GetNumeric(int,class CNumeric &);	//2084
	int  GetObjID(int,class CObjID &);	//2095
	int  GetRawData(int,void *,int);	//2280
	int  GetSbCnt(int,class CValue &,long,class CSbKindToType *);	//2395
	int  GetString(int,class CString &);	//2447
	int  GetValue(int,class CValue &);	//2540
	int  GetValueAndType(int,class CValue &,int);	//2542
	int  GotoNext(void);	//2607
	int  IsOK(void)const;	//2842
	int  Open(void);	//3078
	int  Prepare(char const *,int,int);	//3093
	void  Reset(void);	//3230
	int  SetDateParam(int,class CDate);	//3537
	int  SetLongParam(int,long);	//3631
	int  SetNumericParam(int,class CNumeric const &);	//3657
	int  SetObjIDParam(int,class CObjID const &);	//3659
	int  SetRawDataParam(int,void const *,int);	//3712
	int  SetSbCntParam(int,class CValue const &,long,class CSbKindToType *);	//3738
	int  SetStringParam(int,char const *);	//3771
	int  SetValueAndTypeParam(int,class CValue const &,int);	//3801
	int  SetValueParam(int,class CValue const &);	//3803
};


class IMPORT_1C CODBCTmpTblQuery
{
public:
	 CODBCTmpTblQuery(void);	//188
	virtual  ~CODBCTmpTblQuery(void);	//431
	int  GenerateTempTable(char const *,class CType const &,class PValueArray const &,int);	//1390
	int  GenerateTempTable(char const *,class CType const &,class CValue const *);	//1391
	static class CString   GenerateTempTableName(void);	//1392
protected:
	class CString   GetSQLFieldTypeString(class CType const &);	//2381
private:
	int  InsertValueInTempTable(char const *,class CType const &,class CValue const *);	//2774
	int  MakeTempTable(char const *,class CType const &,class PValueArray const &);	//2994
	static long  m_lTableNumber;	//4233
};


class IMPORT_1C COEMStoreObjTracer
{
public:
	 COEMStoreObjTracer(void);	//189
	 ~COEMStoreObjTracer(void);	//432
	class COEMStoreObjTracer &  operator=(class COEMStoreObjTracer const &);	//546
	static int  m_bOEMIsSelected;	//4231
};


class IMPORT_1C CObjID
{
public:
	long ObjID; // ИД конкретного объекта, например, текущего документа
	class CDBSign DBSign;
	 CObjID(class CObjID const &);	//190
	 CObjID(long,class CDBSign const &);	//191
	 CObjID(void);	//192
	class CObjID &  operator=(class CObjID const &);	//547
	int  operator==(class CObjID const &)const;	//609
	int  operator!=(class CObjID const &)const;	//616
	int  operator<(class CObjID const &)const;	//634
	int  operator>(class CObjID const &)const;	//637
	class CObjID const &  Empty(void);	//1287
	int  FromString(class CString const &);	//1387
	class CDBSign const &  GetDBSign(void)const;	//1613
	static class CDBSign const &  GetDefDBSign(void);	//1638
	class CObjID   GetNextInSequence(void)const;	//2071
	class CObjID   GetPrevInSequence(void)const;	//2173
	class CString   GetString(void)const;	//2448
	long  GetlObjID(void)const;	//2556
	int  IsEmpty(void)const;	//2802
	void  SetDBSign(class CDBSign const &);	//3528
	static void  SetDefDBSign(class CDBSign const &);	//3543
	void  SetlObjID(long);	//3809
};


class IMPORT_1C CObjIDSet
{
public:
	 CObjIDSet(class CObjIDSet const &);	//193
	 CObjIDSet(void);	//194
	 ~CObjIDSet(void);	//433
	class CObjIDSet &  operator=(class CObjIDSet const &);	//549
	int  Find(class CObjID);	//1348
	int  Find(class CObjID,struct std::__rb_tree_iterator<class CObjID,class CObjID const &,class CObjID const *> &);	//1349
	void  Insert(class CObjID);	//2743
};


class IMPORT_1C CObjectSkipper:public CSkipper
{
DECLARE_DYNAMIC(CObjectSkipper)

public:
	 CObjectSkipper(class CObjectSet *);	//196
	virtual  ~CObjectSkipper(void);	//435

	virtual int  next(void);	//4241
	virtual int  prev(void);	//4242
	virtual void  gotop(void);	//4230
	virtual void  gobot(void);	//4229
	virtual long  getpos(void);	//4228
	virtual long  setpos(void);	//4261
	virtual void  read(void);	//4243
	virtual int  Ok(void);	//3056
	virtual long  GetIndex(void);	//1823
	virtual void  SetIndex(long);	//3613
	virtual void  SetMinMaxIndex(void);	//3642

	void  Delete(void);	//1179
	int  GetRead(void)const;	//2281
	void  SetRead(int);	//3713
};


class IMPORT_1C COleGenericBLContext:public CBLContext
{
DECLARE_DYNCREATE(COleGenericBLContext)

public:
	IUnknown *m_IUnknown;
	//IDispatch *m_pDispatch; // 4 байта
	int        m_int0;      // 4 байта

public:
	 COleGenericBLContext(int);	//197
	virtual  ~COleGenericBLContext(void);	//436
//	void  `default constructor closure'(void);	//777
	virtual int  IsOleContext(void)const;	//2847
};


class IMPORT_1C COperJournalDef:public CJournalDef
{
public:
	 COperJournalDef(class CItemList const &);	//198
	 COperJournalDef(long,char const *,char const *);	//199
	virtual  ~COperJournalDef(void);	//437
	static char const *  GetListFunctor(void);	//1914
	virtual int  WhatIsIt(void)const;	//4101
};


class IMPORT_1C COperParamDef
{
public:
	 COperParamDef(class COperParamDef const &);	//200
	 COperParamDef(class CItemList const &);	//201
	 COperParamDef(long,char const *,char const *,class CType const &);	//202
	 COperParamDef(class CMetaDataTypedObj const *);	//203
	virtual  ~COperParamDef(void);	//438
	class COperParamDef &  operator=(class COperParamDef const &);	//550
	static char const *  GetListFunctor(void);	//1915
	virtual int  WhatIsIt(void)const;	//4102
};


class IMPORT_1C COperService
{
public:
	 COperService(void);	//204
	 ~COperService(void);	//439
	class COperService &  operator=(class COperService const &);	//551
	void  Done(void);	//1259
	class COperSet *  GetOperSet(void);	//2109
	class CProvSet *  GetProvSet(void);	//2268
	void  InitOper(void);	//2724
	void  InitProv(void);	//2730
};


class IMPORT_1C COperSet:public CUniSet
{
DECLARE_DYNAMIC(COperSet)

public:
	 COperSet(class CDataBase7 *);	//205
	virtual  ~COperSet(void);	//440
//	void  `default constructor closure'(void);	//778
	void  AddCrossRefs(void);	//812
	int  AddProvs(int);	//840
	int  CheckProvs(int &,class CString &,int);	//976
	void  ClearParams(void);	//1002
	virtual int  Delete(enum MoveAfterDel);	//1180
	int  DeleteProvs(void);	//1216
	virtual void  Download(class CDB7Stream &);	//1273
	int  GetKeyDescrList(class PValueItemArray *,int,class CDate,class CDate,int,class CValue);	//1860
	int  GetKeySumList(class PValueItemArray *,int,class CDate,class CDate,int,class CValue,class CValue);	//1865
	int  GetKeyValueList(class PValueItemArray *,int,int,class CDate,class CDate,int,class CValue,class CValue);	//1869
	class CString   GetPresent(void);	//2161
	class CValue *  GetValueByCtlID(long);	//2543
	virtual int  GotoCurrent(void);	//2562
	virtual int  GotoFirst(void);	//2575
	virtual int  GotoLast(void);	//2592
	virtual int  GotoNext(void);	//2608
	virtual void  GotoNull(void);	//2620
	virtual long  GotoPos(long);	//2630
	virtual int  GotoPrev(void);	//2642
	virtual void  InitNew(void);	//2713
	void  InitProvSet(void);	//2731
	virtual int  IsMultSel(void);	//2829
	void  LinkAllParams(int,int);	//2903
	void  LinkParam(int,int,int);	//2915
	void  LinkParam(class COperParamDef *,int,int);	//2916
	int  LoadProvs(int);	//2956
	int  LoadProvsWithCheck(void);	//2957
	virtual int  New(void);	//3035
	int  OnOffProvs(int);	//3066
	int  SaveProvs(int);	//3277
	int  SelectAll(int);	//3329
	int  SelectByValue(class CDate,class CDate,class CValue const &,long,class CEventTime,class CEventTime);	//3357
	int  SelectDescr(class CDate,class CDate,class CString,class CEventTime,class CEventTime);	//3371
	int  SelectDocID(class CObjID);	//3377
	int  SelectPeriod(class CDate,class CDate,class CEventTime,class CEventTime);	//3419
	int  SelectSort(class CDate,class CDate,long);	//3431
	int  SelectSortDescr(class CDate,class CDate);	//3432
	int  SelectSortSum(class CDate,class CDate);	//3433
	int  SelectSum(class CDate,class CDate,class CNumeric,class CEventTime,class CEventTime);	//3435
protected:
	void  SetCurOrder(int,int,int);	//3513
public:
	virtual void  SetRelPos(long,int);	//3723
protected:
	void  SetSearchTime(class CEventTime const &,class CEventTime const &);	//3744
public:
	virtual int  SetSelectionFields(void);	//3758
	virtual int  TestSelectionFields(void);	//3912
	virtual int  Update(void);	//3988
	int  UpdateProvs(void);	//4005
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4018
};


class IMPORT_1C COperTemplateCtlInfo
{
public:
	 COperTemplateCtlInfo(class COperTemplateCtlInfo const &);	//206
	 COperTemplateCtlInfo(class COperTemplateCtlInfo *);	//207
	 COperTemplateCtlInfo(void);	//208
	virtual  ~COperTemplateCtlInfo(void);	//441
	class COperTemplateCtlInfo &  operator=(class COperTemplateCtlInfo const &);	//552
	void  LoadFromList(class CItemList const &);	//2943
	void  LoadFromListEx(class CItemList const &,class CString &,class CString &);	//2947
	void  SaveToList(class CItemList &);	//3289
	void  SaveToListEx(class CItemList &);	//3292
};


class IMPORT_1C COperTemplateCtlsInfo
{
public:
	 COperTemplateCtlsInfo(void);	//209
	virtual  ~COperTemplateCtlsInfo(void);	//442
	class COperTemplateCtlInfo *  GetCtlInfo(long,int);	//1582
	void  LoadFromList(class CItemList const &);	//2944
	void  SaveToList(class CItemList &);	//3290
	void  SaveToListEx(class CItemList &);	//3293
};


class IMPORT_1C COperTemplateInfo
{
public:
	 COperTemplateInfo(class CString const &);	//210
	 COperTemplateInfo(class COperTemplateInfo *);	//211
	 COperTemplateInfo(void);	//212
	virtual  ~COperTemplateInfo(void);	//443
	class CProvTemplateInfo *  AddProv(int,int,int);	//833
	void  DelProv(int);	//1150
	int  GetAddRemove(void);	//1436
	int  GetAnybody(void);	//1451
	static long  GetNewVerEx(void);	//2068
	static long  GetOldVer(void);	//2099
	static long  GetOldVerEx(void);	//2100
	class CProvTemplateInfo *  GetProv(int);	//2252
	class PProvTemplateInfoArray *  GetProvInfoList(void);	//2263
	class CStringArray *  GetUsers(void);	//2529
	int  IsEmptyValue(class CValue const &);	//2804
	int  LoadFromStringEx(class CString &);	//2950
	void  SaveToString(class CString &,int);	//3296
	void  SaveToStringEx(class CString &);	//3298
	void  SetAddRemove(int);	//3465
	void  SetAnybody(int);	//3475
	static void  SetOldVerEx(long);	//3660
	static long  nNewVer;	//4237
	static long  nNewVerEx;	//4238
	static long  nOldVer;	//4239
	static long  nOldVerEx;	//4240
};


class IMPORT_1C CPlanDef: public CMetaDataObj
{
public:
	 CPlanDef(class CItemList const &);	//213
	 CPlanDef(long,char const *,char const *);	//214
	virtual  ~CPlanDef(void);	//444
	int  AddAccDef(class CAccDef *,int);	//801
	virtual void  AddToList(class CItemList &)const;	//884
	void  DelAccChildsDef(class CAccDef *,int);	//1092
	void  DelAccDef(int,int);	//1093
	void  DelAccDef(class CAccDef *,int);	//1094
	class CAccDef *  FindAccDef(class CString);	//1351
	class CAccDef *  FindAncestor(class CString);	//1352
	class CAccDef *  FindParent(class CString);	//1368
	class CMetaDataObjArray *  GetAccArray(void);	//1399
	class CAccDef *  GetAccDef(long)const;	//1404
	class CAccDef *  GetAccDef(char const *)const;	//1405
	class CAccDef *  GetAccDefAt(int)const;	//1406
	int  GetAccDefIdx(long)const;	//1407
	int  GetAccDefIdx(class CAccDef *)const;	//1408
	int  GetAccDefIdx(char const *)const;	//1409
	class std::map<class CString,class CAccDef *,struct std::less<class CString>,class std::__alloc<1,0> > *  GetAccDefs(void)const;	//1410
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1522
	virtual char const *  GetChildName(int,int)const;	//1537
	int  GetCodeLen(void)const;	//1549
	class CString   GetCodeMask(void)const;	//1552
	static char const *  GetListFunctor(void);	//1916
	int  GetNAccDefs(void)const;	//1970
	virtual int  GetNChilds(void)const;	//1988
	virtual int  GetNProps(void)const;	//2035
	int  GetNSubCodes(void)const;	//2052
	class CMetaDataObjDistr *  GetObjDistr(void);	//2092
	virtual char const *  GetPropName(int,int)const;	//2207
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2240
	int  GetSubCodeLen(int)const;	//2450
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3312
	int  SetAccCode(class CAccDef *,class CString);	//3452
	int  SetCodeMask(class CString);	//3504
	int  TestCodeUnique(class CString,long);	//3894
	virtual int  WhatIsIt(void)const;	//4103
};


class IMPORT_1C CPresentItem
{
public:
	 CPresentItem(class CPresentItem const &);	//215
	 CPresentItem(class CItemList const &);	//216
	 CPresentItem(long,class CString,int);	//217
	 CPresentItem(void);	//218
	virtual  ~CPresentItem(void);	//445
	class CPresentItem &  operator=(class CPresentItem const &);	//553
	void  AddToList(class CItemList &);	//885
	long  GetFldID(void)const;	//1759
	virtual int  GetNProps(void)const;	//2036
	int  GetNewLine(void)const;	//2066
	virtual char const *  GetPropName(int,int)const;	//2208
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2241
	class CString   GetTitle(void)const;	//2481
	void  SetFldID(long);	//3588
	void  SetNewLine(int);	//3645
	void  SetTitle(class CString);	//3778
	virtual int  WhatIsIt(void)const;	//4104
};


class IMPORT_1C CProvHardFldDef
{
public:
	 CProvHardFldDef(class CProvHardFldDef const &);	//219
	 CProvHardFldDef(class CItemList const &);	//220
	 CProvHardFldDef(long,char const *,char const *,class CType const &);	//221
	virtual  ~CProvHardFldDef(void);	//446
	class CProvHardFldDef &  operator=(class CProvHardFldDef const &);	//554
	static char const *  GetListFunctor(void);	//1917
	virtual int  WhatIsIt(void)const;	//4105
};


class IMPORT_1C CProvObj
{
public:
	 CProvObj(class CProvRec &);	//222
	 CProvObj(class CSQLProvRec &);	//223
	 CProvObj(class CBuhDef *);	//224
	 ~CProvObj(void);	//447
	class CProvObj const &  operator=(class CProvObj &);	//555
//	void  `default constructor closure'(void);	//779
	void  CopyCorrespond(class CProvObj *,int);	//1046
	class CTypedValue *  GetSbCnt(long,int);	//2396
	class CTypedValue *  GetSbCnt(class CString,int);	//2397
	class CValue *  GetValueByCtlID(long,int);	//2544
};


class IMPORT_1C CProvObjArray
{
public:
	 CProvObjArray(void);	//225
	virtual  ~CProvObjArray(void);	//448
	class CProvObj *  AddCorrespond(int &,class CProvObj *);	//811
	class CProvObj *  AddProv(int &,class CProvObj *);	//834
	void  DelAll(void);	//1099
	void  DelAt(int);	//1116
};


//class IMPORT_1C CProvParamDef
// 2006-01-29: spock
// на самом деле это наследованный класс
//
class IMPORT_1C CProvParamDef : public CMetaDataObj
{
public:
	CProvParamDef(class CProvParamDef const &); //226
	CProvParamDef(class CItemList const &); //227
	CProvParamDef(long,char const *,char const *,class CType const &); //228
	CProvParamDef(class CMetaDataTypedObj const *); //229
	virtual ~CProvParamDef(void); //449
	class CProvParamDef &  operator=(class CProvParamDef const &); //556
	static char const * GetListFunctor(void); //1918
	virtual int WhatIsIt(void)const; //4106
};

class IMPORT_1C CProvQuery
{
public:
	 CProvQuery(class CODBCDB7 *,class CProvObj *);	//230
	virtual  ~CProvQuery(void);	//450
	virtual void  OnNextRow(void);	//3064
};


class IMPORT_1C CProvSet:public CUniSet
{
DECLARE_DYNAMIC(CProvSet)

public:
	 CProvSet(class CDataBase7 *,char const *);	//231
	virtual  ~CProvSet(void);	//451
//	void  `default constructor closure'(void);	//780
	void  AddAccSel(void);	//802
	void  AddCrossRefs(void);	//813
	int  CalcAccSels(class CDate const &,class CDate const &,int);	//928
	int  CalcSbSels(class CDate const &,class CDate const &,int);	//929
	void  ChangeTime(class CObjID &,long);	//951
	virtual int  Delete(enum MoveAfterDel);	//1181
	void  DeleteAccSel(void);	//1188
	void  DeleteByDocID(class CObjID);	//1199
	void  DeleteSelByDocID(class CObjID);	//1219
	virtual long  GetCurrentPos(void);	//1598
	int  GetKeyAccList(class PValueItemArray *,int,class CDate,class CDate,int);	//1856
	int  GetKeyAmountList(class PValueItemArray *,int,class CDate,class CDate,int,class CValue,class CValue);	//1857
	int  GetKeyCurrencyList(class PValueItemArray *,int,class CDate,class CDate);	//1858
	int  GetKeyCurrencySumList(class PValueItemArray *,int,class CDate,class CDate,int,class CValue,class CValue);	//1859
	int  GetKeyPlanList(class PValueItemArray *,int,class CDate,class CDate);	//1863
	int  GetKeySbList(class PValueItemArray *,long,int,class CDate,class CDate);	//1864
	int  GetKeySumList(class PValueItemArray *,int,class CDate,class CDate,int,class CValue,class CValue);	//1866
	int  GetKeyValueList(class PValueItemArray *,int,int,class CDate,class CDate,int,class CValue,class CValue);	//1870
	int  GetSelectedByDt(void);	//2428
	int  GetSelectedByKt(void);	//2429
	virtual int  GotoCurrent(void);	//2563
	virtual int  GotoFirst(void);	//2576
	virtual int  GotoLast(void);	//2593
	virtual int  GotoNext(void);	//2609
	virtual void  GotoNull(void);	//2621
	virtual long  GotoPos(long);	//2631
	virtual int  GotoPrev(void);	//2643
	virtual void  InitNew(void);	//2714
	virtual int  IsMultSel(void);	//2830
	void  LinkAllParams(int,int);	//2904
	void  LinkParam(int,int,int);	//2917
	void  LinkParam(class CProvParamDef *,int,int);	//2918
	virtual int  New(void);	//3036
	int  ProcessProv(class CDate,int,int,int,int,class CAccTotalSet *,class CTotalSet *);	//3113
	int  SelectAll(int);	//3330
	int  SelectByAccount(class CDate,class CDate,class CObjID,int,class CEventTime,class CEventTime);	//3345
	int  SelectByAccountWithAcc(class CDate,class CDate,class CObjID,int,class CEventTime,class CEventTime);	//3346
	int  SelectByAmount(class CDate,class CDate,class CNumeric,class CEventTime,class CEventTime);	//3347
	int  SelectByCurrency(class CDate,class CDate,class CObjID,class CEventTime,class CEventTime);	//3348
	int  SelectByCurrencySum(class CDate,class CDate,class CNumeric,class CEventTime,class CEventTime);	//3349
	int  SelectByPlan(class CDate,class CDate,long,class CEventTime,class CEventTime);	//3353
	int  SelectBySb(class CDate,class CDate,long,class CValue const &,class CEventTime,class CEventTime);	//3354
	int  SelectBySbWithAcc(class CDate,class CDate,long,class CValue const &,class CEventTime,class CEventTime);	//3355
	int  SelectBySum(class CDate,class CDate,class CNumeric,class CEventTime,class CEventTime);	//3356
	int  SelectByValue(class CDate,class CDate,class CValue const &,long,class CEventTime,class CEventTime);	//3358
	int  SelectCheck(int,int);	//3364
	int  SelectDocID(class CObjID);	//3378
	int  SelectPeriod(class CDate,class CDate,class CEventTime,class CEventTime);	//3420
	int  SelectPeriodWithAcc(class CDate,class CDate,class CEventTime,class CEventTime);	//3423
	int  SelectProv(class CObjID,long,int);	//3428
protected:
	void  SetCurOrder(int,int,int);	//3514
public:
	virtual void  SetRelPos(long,int);	//3724
protected:
	void  SetSearchTime(class CEventTime const &,class CEventTime const &);	//3745
public:
	virtual int  SetSelectionFields(void);	//3759
protected:
	void  SyncPos(int);	//3885
public:
	virtual int  TestSelectionFields(void);	//3913
	virtual void  UnSelect(void);	//3950
	virtual int  Update(void);	//3989
	int  UpdateActive(class CObjID,int);	//3997
};


class IMPORT_1C CProvTemplateInfo
{
public:
	 CProvTemplateInfo(class CProvTemplateInfo *);	//232
	 CProvTemplateInfo(void);	//233
	virtual  ~CProvTemplateInfo(void);	//452
	void  LoadFromList(class CItemList const &);	//2945
	void  LoadFromListEx(class CItemList const &);	//2948
	void  SaveToList(class CItemList &);	//3291
	void  SaveToListEx(class CItemList &);	//3294
};


class IMPORT_1C CRGDB7:public CDataBase7
{
DECLARE_DYNAMIC(CRGDB7)

public:
	 CRGDB7(void);	//234
	virtual  ~CRGDB7(void);	//453
	virtual class CDDB7Service *	CreateDDB7Service(void);	//1057
	virtual int  AttachDDB7Service(class CDDB7Service *);	//908
	virtual int  DoesDBExist(void);	//1250
	virtual int  Open(int,int,int,int,int);	//3079
	virtual int  Close(void);	//1014
	virtual int  IsOpen(void);	//2852
	virtual int  IsOpenExclusive(void);	//2855
	virtual void  SetLockWaitTime(int);	//3628
	virtual int  BeginTransaction(void);	//918
	virtual int  CommitTransaction(void);	//1020
	virtual int  RollbackTransaction(void);	//3252
	virtual int  RollbackAllTransactions(void);	//3247
	virtual int  GetNActiveTransactions(void);	//1974
virtual int CanBeCommited(void);
	virtual void *  GetRealDB(void);	//2284

};


class IMPORT_1C CRecalcRule
{
public:
	 CRecalcRule(class CRecalcRuleDef *);	//235
	 ~CRecalcRule(void);	//454
	class CRecalcRule &  operator=(class CRecalcRule const &);	//557
	void  AppendDepend(long);	//900
	void  AppendLead(long);	//901
	void  DelAllDepends(void);	//1105
	void  DelAllLeads(void);	//1110
	void  DeleteDepend(long);	//1205
	void  DeleteLead(long);	//1213
	class CFFList *  GetDepends(void);	//1650
	class CFFList *  GetLeads(void);	//1878
	int  GetPeriods(void);	//2141
	int  GetRuleType(void);	//2342
	int  HasDepend(long);	//2652
	int  HasLead(long);	//2655
	void  SetPeriods(int);	//3685
	void  SetRuleType(int);	//3732
};


class IMPORT_1C CRecalcRuleDef: public CMetaDataObj
{
public:
	 CRecalcRuleDef(class CRecalcRuleDef const &);	//236
	 CRecalcRuleDef(class CItemList const &);	//237
	 CRecalcRuleDef(long,char const *,char const *);	//238
	virtual  ~CRecalcRuleDef(void);	//455
	class CRecalcRuleDef &  operator=(class CRecalcRuleDef const &);	//558
	virtual void  AddToList(class CItemList &)const;	//886
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1523
	virtual char const *  GetChildName(int,int)const;	//1538
	class CMetaDataRefObj *  GetDependAlgs(void);	//1649
	class CMetaDataRefObj *  GetLeadAlgs(void);	//1877
	static char const *  GetListFunctor(void);	//1919
	virtual int  GetNChilds(void)const;	//1989
	virtual int  GetNProps(void)const;	//2037
	int  GetPeriods(void)const;	//2142
	virtual char const *  GetPropName(int,int)const;	//2209
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2242
	int  GetRuleType(void)const;	//2343
	void  SetPeriods(int);	//3686
	void  SetRuleType(int);	//3733
	virtual int  WhatIsIt(void)const;	//4107
};


class IMPORT_1C CRecalcRulesContainer
{
public:
	 CRecalcRulesContainer(void);	//239
	 ~CRecalcRulesContainer(void);	//456
	int  GetRREnable(void);	//2278
	void  SetRREnable(int);	//3710
};


class IMPORT_1C CRecalcStatus
{
public:
	 CRecalcStatus(void);	//240
	 ~CRecalcStatus(void);	//457
};


class IMPORT_1C CRefContext: public CBLContext
{
DECLARE_DYNAMIC(CRefContext)
public:
	CBLContext* m_pContext;

	 CRefContext(void);	//241
	virtual  ~CRefContext(void);	//458
};


class IMPORT_1C CRefRecData
{
public:
	 CRefRecData(class CRefRecData const &);	//242
	 CRefRecData(long,class CObjID const &,long,long,class CObjID const &,int,class CMetaDataTypedObj *);	//243
	class CRefRecData &  operator=(class CRefRecData const &);	//559
};


class IMPORT_1C CReferenceIntegrity
{
public:
	 CReferenceIntegrity(class CDataBase7 *,int);	//244
	virtual  ~CReferenceIntegrity(void);	//459
//	void  `default constructor closure'(void);	//781
private:
	int  AccCheck(void);	//786
public:
	int  AddDelRecData(class CDelRecData *);	//814
private:
	int  CJCheck(class CCJDef *,int);	//927
	int  CheckAndPutRef(class CType,long,class CObjID,long,class CObjID,long,int,class CMetaDataTypedObj *,long);	//964
	int  CheckField(class CType const &);	//968
public:
	void  ClearRefs(void);	//1004
private:
	int  ConstCheck(class CMetaDataTypedObj *,long,class CObjID,int);	//1026
	int  CycleCheck(void);	//1074
public:
	void  DelDelRecData(int);	//1127
protected:
	int  DelDocOrSbCnt(class CMetaDataObj *,class CObjID,int);	//1130
public:
	int  DelRecs(int,int);	//1153
private:
	int  DocCheck(class CDocDef *);	//1242
	void  DropTmpDelRecTable(void);	//1283
public:
	int  FillDelList(void);	//1342
private:
	int  FillTmpDelRecTable(void);	//1346
	int  FindDelRec(long,class CObjID &);	//1362
	int  FindRefRec(long,class CObjID &);	//1370
protected:
	int  FindRefRec_DelOrder(long,class CObjID &);	//1371
private:
	void  FlushTransaction(void);	//1379
public:
	class CString   GetPresent(long,class CObjID,class CMetaDataTypedObj *,long);	//2162
protected:
	virtual void  MarkDelAcc(class CAccSet &);	//2997
	virtual void  MarkDelDoc(class CDocSet &);	//2998
	virtual void  MarkDelSbCnt(class CSbCntSet &);	//2999
	virtual void  OnLogMessage(class CString,struct LogEventInfo *);	//3062
public:
	virtual void  OnWait(void);	//3073
private:
	int  OperCheck(void);	//3086
	int  ProvCheck(int,int);	//3121
public:
	int  ReferenceCheck(int,int);	//3189
private:
	int  RegCheck(class CRegDef *);	//3193
	int  SQLAccCheck(void);	//3255
	int  SQLCJCheck(class CCJDef *,int);	//3256
	int  SQLDocCheck(class CDocDef *);	//3257
	int  SQLOperCheck(void);	//3258
	int  SQLProvCheck(int,int);	//3259
	int  SQLRegCheck(class CRegDef *);	//3260
	int  SQLSbCntCheck(class CSbCntTypeDef *,int,int);	//3261
	int  SbCntCheck(class CSbCntTypeDef *,int,int);	//3301
	void  SortDelRec(void);	//3822
	void  SortRefRec(void);	//3833
	void  SortRefRec_DelOrder(void);	//3834
	void  StartTransaction(void);	//3859
public:
	virtual void  StartWait(void);	//3864
protected:
	void  StatusLine(class CString const &);	//3865
private:
	void  StopTransaction(void);	//3875
public:
	virtual void  StopWait(void);	//3880
};


class IMPORT_1C CRegActSet:public CUniSet
{
DECLARE_DYNAMIC(CRegActSet)

public:
	 CRegActSet(class CRegDef *,class CDataBase7 *);	//245
	 CRegActSet(class CRegDef *,char const *,class CDataBase7 *);	//246
	virtual  ~CRegActSet(void);	//460
	void  ChangeTime(class CObjID &,long);	//952
	virtual int  Delete(enum MoveAfterDel);	//1182
	virtual void  Download(class CDB7Stream &);	//1274
	virtual long  GetCurrentPos(void);	//1599
	class CEventTime   GetEventTime(void)const;	//1707
	class CString   GetPresent(void);	//2163
	class CRegDef *  GetRegDef(void)const;	//2309
	virtual int  GotoFirst(void);	//2577
	virtual int  GotoLast(void);	//2594
	virtual int  GotoNext(void);	//2610
	virtual long  GotoPos(long);	//2632
	virtual int  GotoPrev(void);	//2644
	virtual void  InitNew(void);	//2715
	int  IsActNoUsed(long)const;	//2779
	virtual int  IsMultSel(void);	//2831
	void  LinkAll(int,int);	//2897
	void  LinkProp(int,int,int);	//2921
	virtual int  New(void);	//3037
	void  RegisterActNo(long);	//3194
	void  ResetActNos(void);	//3233
	int  SelectAll(void);	//3331
	int  SelectDoc(class CObjID);	//3373
	int  SelectDocForvOnly(class CObjID);	//3375
	int  SelectRange(class CDate,class CDate,class CEventTime,class CEventTime,long,class CValue *);	//3430
	int  SetPropsFigures(class CValue * *,class CValue * *,class CValue * *);	//3696
	virtual int  SetSelectionFields(void);	//3760
	int  TestInRange(class CDate &,long &,class CObjID &);	//3899
	virtual int  TestSelectionFields(void);	//3914
	virtual void  UnSelect(void);	//3951
	virtual int  Update(void);	//3990
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4019
	int  WriteDocAct(class CObjID,long,long,class CValue * *,class CValue * *,class CValue * *,int,class CDate,class CDate,int,int,class CDate,long,long);	//4131
	int  WriteNew(class CObjID,long,long,class CValue * *,class CValue * *,class CValue * *,int,class CDate,long,long);	//4134
};


class IMPORT_1C CRegDef: public CMetaDataObj
{
public:
	 CRegDef(class CItemList const &);	//247
	 CRegDef(long,char const *,char const *);	//248
	virtual  ~CRegDef(void);	//461
	void  Accept(void);	//791
	int  AddFigureDef(class CRegFigureDef *);	//817
	int  AddPropDef(class CRegPropDef *);	//832
	virtual void  AddToList(class CItemList &)const;	//887
	void  DelFigureDef(int,int);	//1133
	void  DelFigureDef(class CRegFigureDef *,int);	//1134
	void  DelPropDef(int,int);	//1148
	void  DelPropDef(class CRegPropDef *,int);	//1149
	void  EnableActsMigration(int);	//1289
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1524
	virtual char const *  GetChildName(int,int)const;	//1539
	int  GetDateTimeInActs(void)const;	//1633
	char const *  GetFieldName(void)const;	//1733
	class CMetaDataObjArray *  GetFigure(void);	//1738
	class CRegFigureDef *  GetFigureDef(long)const;	//1739
	class CRegFigureDef *  GetFigureDef(char const *)const;	//1740
	class CRegFigureDef *  GetFigureDefAt(int)const;	//1741
	int  GetFigureDefIdx(long)const;	//1742
	int  GetFigureDefIdx(class CRegFigureDef *)const;	//1743
	int  GetFigureDefIdx(char const *)const;	//1744
	class CMetaDataObjArray *  GetFilds(void);	//1747
	CTypedFldDefsArray<class CRegFldDef> *  GetFlds(void)const;	//1760
	int  GetHasIndexInActs(void)const;	//1789
	int  GetKind(void)const;	//1872
	static char const *  GetListFunctor(void);	//1920
	virtual int  GetNChilds(void)const;	//1990
	int  GetNFigureDefs(void)const;	//1998
	int  GetNPropDefs(void)const;	//2011
	virtual int  GetNProps(void)const;	//2038
	enum PeriodType   GetPeriod(void)const;	//2134
	class CRegPropDef *  GetPropDef(long)const;	//2178
	class CRegPropDef *  GetPropDef(char const *)const;	//2179
	class CRegPropDef *  GetPropDefAt(int)const;	//2180
	int  GetPropDefIdx(long)const;	//2181
	int  GetPropDefIdx(class CRegPropDef *)const;	//2182
	int  GetPropDefIdx(char const *)const;	//2183
	virtual char const *  GetPropName(int,int)const;	//2210
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2243
	class CMetaDataObjArray *  GetProps(void);	//2251
	char const *  GetRegActTableName(void)const;	//2306
	char const *  GetRegTableName(void)const;	//2319
	char const *  GetTableName(char)const;	//2463
	int  InsertFigureDef(int,class CRegFigureDef *);	//2756
	int  InsertPropDef(int,class CRegPropDef *);	//2765
	int  IsActsMigrationEnabled(void)const;	//2780
	int  IsRests(void)const;	//2872
	int  IsTurnovers(void)const;	//2879
	int  MoveFigureDef(class CRegFigureDef *,int);	//3011
	int  MovePropDef(class CRegPropDef *,int);	//3018
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3313
	void  SetHasIndexInActs(int);	//3602
	void  SetKind(int);	//3621
	void  SetPeriod(enum PeriodType);	//3681
	int  SortFigureDefs(void);	//3825
	int  SortPropDefs(void);	//3832
	virtual int  WhatIsIt(void)const;	//4108
};


class IMPORT_1C CRegFigureDef: public CMetaDataTypedObj
{
public:
	 CRegFigureDef(class CRegFigureDef const &);	//249
	 CRegFigureDef(class CItemList const &);	//250
	 CRegFigureDef(long,char const *,char const *,class CType const &);	//251
	 CRegFigureDef(class CMetaDataTypedObj const *);	//252
	virtual  ~CRegFigureDef(void);	//462
	class CRegFigureDef &  operator=(class CRegFigureDef const &);	//560
	virtual void  AddToList(class CItemList &)const;	//888
	static char const *  GetListFunctor(void);	//1921
	virtual int  WhatIsIt(void)const;	//4109
};


class IMPORT_1C CRegFilter
{
public:
	 CRegFilter(class CRegFilter const &);	//253
	 CRegFilter(class CRegDef *);	//254
	 ~CRegFilter(void);	//463
	class CRegFilter &  operator=(class CRegFilter const &);	//561
	static int  AutoDetectUse(class CValue * &,int);	//909
	int  Compare(class CValue * *,class CValue * *,int);	//1022
	int  CompareNeeded(int);	//1024
	int  CopyFrom(class CRegFilter *);	//1048
	class CRegDef *  GetDef(void);	//1637
	class CValue *  GetFld(int);	//1752
	class CValue * *  GetPProps(void);	//2114
	class CValue *  GetPropA(int);	//2177
	int  GetUseFld(int);	//2522
	int  GetUseProp(int);	//2524
	int  GetUsedPropCnt(void);	//2525
	void  Reset(void);	//3231
	void  SetFld(int,class CValue *,int);	//3587
	void  SetPropA(int,class CValue *,int);	//3693
	long  TestUseActsKey(class CValue * &,long);	//3925
	long  TestUseTotalsKey(int &);	//3929
};


class IMPORT_1C CRegFldDef: public CMetaDataTypedObj
{
public:
	 CRegFldDef(class CRegFldDef const &);	//255
	 CRegFldDef(class CItemList const &);	//256
	 CRegFldDef(long,char const *,char const *,class CType const &);	//257
	 CRegFldDef(class CMetaDataTypedObj const *);	//258
	virtual  ~CRegFldDef(void);	//464
	class CRegFldDef &  operator=(class CRegFldDef const &);	//562
	static char const *  GetListFunctor(void);	//1922
	virtual char const *  GetPropName(int,int)const;	//2211
	virtual int  WhatIsIt(void)const;	//4110
};


class IMPORT_1C CRegPropDef: public CMetaDataTypedObj
{
public:
	 CRegPropDef(class CRegPropDef const &);	//259
	 CRegPropDef(class CItemList const &);	//260
	 CRegPropDef(long,char const *,char const *,class CType const &);	//261
	 CRegPropDef(class CMetaDataTypedObj const *);	//262
	virtual  ~CRegPropDef(void);	//465
	class CRegPropDef &  operator=(class CRegPropDef const &);	//563
	virtual void  AddToList(class CItemList &)const;	//889
	int  GetHasIndex(void)const;	//1787
	int  GetHasIndexInTotals(void)const;	//1790
	static char const *  GetListFunctor(void);	//1923
	virtual int  GetNProps(void)const;	//2039
	virtual char const *  GetPropName(int,int)const;	//2212
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2244
	void  SetHasIndex(int);	//3600
	void  SetHasIndexInTotals(int);	//3603
	virtual int  WhatIsIt(void)const;	//4111
};


class IMPORT_1C CRegSet:public CUniSet
{
DECLARE_DYNAMIC(CRegSet)

public:
	 CRegSet(class CRegDef *,class CDataBase7 *);	//263
	 CRegSet(class CRegDef *,char const *,class CDataBase7 *);	//264
	virtual  ~CRegSet(void);	//466
	int  Change(class CDate,class CValue * *,class CValue * *,int,int);	//947
	virtual int  Delete(enum MoveAfterDel);	//1183
	void  DeleteAll(void);	//1193
	class CRegDef *  GetRegDef(void)const;	//2310
	virtual int  GotoFirst(void);	//2578
	virtual int  GotoLast(void);	//2595
	virtual void  InitNew(void);	//2716
	virtual int  IsMultSel(void);	//2832
	void  LinkAll(int,int);	//2898
	void  LinkProp(int,int,int);	//2922
	virtual int  New(void);	//3038
	int  SelectAll(void);	//3332
	int  SelectFiltKey(class CDate,long,class CValue *);	//3384
	int  SelectPeriod(class CDate);	//3421
	int  SelectProps(class CDate,class CValue * *,int);	//3426
	int  SetPropsFigures(class CValue * *,class CValue * *);	//3697
	virtual int  SetSelectionFields(void);	//3761
	virtual int  TestSelectionFields(void);	//3915
	virtual void  UnSelect(void);	//3952
	virtual int  Update(void);	//3991
	int  WriteNew(class CDate,class CValue * *,class CValue * *);	//4135
};


class IMPORT_1C CReportDef: public CMetaDataObj
{
public:
	 CReportDef(class CReportDef const &);	//265
	 CReportDef(class CItemList const &);	//266
	 CReportDef(long,char const *,char const *);	//267
	virtual  ~CReportDef(void);	//467
	class CReportDef &  operator=(class CReportDef const &);	//564
	virtual void  AddToList(class CItemList &)const;	//890
	static char const *  GetListFunctor(void);	//1924
	virtual int  WhatIsIt(void)const;	//4112
};


class IMPORT_1C CSQLBuhQuery
{
public:
	 CSQLBuhQuery(
		class CDate,
		class CEventTime,
		class CDate,
		class CEventTime,
		class std::vector<
			struct std::pair<class CString,long>,
			class std::__alloc<1,0> > *,
		class std::vector<
			struct std::pair<class CString,long>,
			class std::__alloc<1,0> > *,
			long *,
			class CValue *,
			int *,
			long *,
			class CValue *,
			int *,
			class CObjID,
			int,
			class _std,
			int,
			int,
			class CBelong * *,
			class CBelong * *);	//268
	virtual  ~CSQLBuhQuery(void);	//468
protected:
	void  BindAccDiv(int);	//922
	int  CreateTmpAccTable(void);	//1070
	int  CreateTmpSbCntValTable(class CBelong * *,class CBelong * *);	//1072
public:
	int  ExistTurnOversOnly(void);	//1315
protected:
	int  ExpandSbCnt(class CValue &,class CSbKindDef *,int,class CBelong *);	//1316
	class CString   GetAccSumString(int,int,int);	//1422
	class CString   GetAccsFromString(int,int &);	//1429
public:
	int  GetMaxTableCount(void);	//1958
protected:
	class CString   GetProvSelString(int,int,int);	//2267
	class CString   GetProvWhereBySbCnt(int,int);	//2274
	class CString   GetSCFromString(int,int &);	//2380
public:
	class CString   GetSelectInitBalanceByAccount(void);	//2415
	class CString   GetSelectInitBalanceByAccountWithSbCnt(void);	//2416
	class CString   GetSelectOb(void);	//2417
	class CString   GetSelectOborot(int);	//2418
protected:
	class CString   GetSelectPeriodString(int);	//2419
public:
	class CString   GetSelectSaldo(void);	//2420
	class CString   GetSelectSaldoR(int);	//2421
	class CString   GetSelectSaldoRA(int);	//2422
	class CString   GetSelectTmpCalc(int);	//2423
	class CString   GetSelectTurnoverBetweenAccounts(int);	//2424
	class CString   GetSelectTurnoverBetweenAccountsWithSbCnt(void);	//2425
	class CString   GetSelectTurnoverByAccount(int);	//2426
	class CString   GetSelectTurnoverByAccountWithSbCnt(int);	//2427
protected:
	class CString   GetSumString(int,int,int);	//2456
	class CString   GetTmpCalcSumString(class CDate const &,int,class CDate const &,int,class CString const &,class CString const &,class CString const &);	//2483
	class CString   GetTmpSelectByProv(class CString const &,class CString const &,class CString const &,class CString const &,class CDate const &,class CEventTime const &,class CDate const &,class CEventTime const &,int,int,class CDate const &,class CEventTime const &,int,int);	//2484
	class CString   GetTotalSelString(int);	//2486
	class CString   GetTotalWhereBySbCnt(void);	//2489
	virtual void  OnNextRow(void);	//3065
	void  ResetMembers(void);	//3237
public:
	int  SelectInitBalanceByAccount(void);	//3394
	int  SelectInitBalanceByAccountWithSbCnt(void);	//3395
	int  SelectTmpCalc(void);	//3436
	int  SelectTotal(class CNumeric &);	//3437
	int  SelectTurnoverBetweenAccounts(void);	//3440
	int  SelectTurnoverBetweenAccountsWithSbCnt(void);	//3441
	int  SelectTurnoverByAccount(void);	//3442
	int  SelectTurnoverByAccountWithSbCnt(void);	//3443
protected:
	void  SetDates(void);	//3540
	static int  s_nMaxTableCount;	//4250
};


class IMPORT_1C CSbCntParamDef: public CMetaDataTypedObj
{
public:
	 CSbCntParamDef(class CSbCntParamDef const &);	//269
	 CSbCntParamDef(class CItemList const &);	//270
	 CSbCntParamDef(long,char const *,char const *,int,class CType const &);	//271
	 CSbCntParamDef(class CMetaDataTypedObj const *);	//272
	virtual  ~CSbCntParamDef(void);	//469
	class CSbCntParamDef &  operator=(class CSbCntParamDef const &);	//565
	virtual void  AddToList(class CItemList &)const;	//891
	int  GetDocEdit(void)const;	//1673
	int  GetHasIndex(void)const;	//1788
	int  GetHasSelect(void)const;	//1791
	static char const *  GetListFunctor(void);	//1925
	int  GetManualEdit(void)const;	//1948
	virtual int  GetNProps(void)const;	//2040
	virtual char const *  GetPropName(int,int)const;	//2213
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2245
	virtual int  IsDateDep(void)const;	//2796
	int  IsEnableForFolder(void)const;	//2805
	int  IsEnableForItem(void)const;	//2806
	void  SetDateDep(int);	//3536
	void  SetDocEdit(int);	//3558
	void  SetEnableForFolder(int);	//3567
	void  SetEnableForItem(int);	//3568
	void  SetHasIndex(int);	//3601
	void  SetHasSelect(int);	//3604
	void  SetManualEdit(int);	//3637
	virtual int  WhatIsIt(void)const;	//4113
};


class IMPORT_1C CSbCntSet:public CUniSet//CObjectSet
{
DECLARE_DYNAMIC(CSbCntSet)

public:
	 CSbCntSet(class CSbCntTypeDef *,class CDataBase7 *);	//273
	virtual  ~CSbCntSet(void);	//470
	void  BeginReadParams(void);	//914
	void  ClearAllParams(void);	//984
protected:
	void  ClearDirty(void);	//992
public:
	void  ClearDirtyForDateDep(void);	//993
	void  ClearParam(int);	//999
	void  ClearParam(class CSbCntParamDef *);	//1000
	virtual int  Delete(enum MoveAfterDel);	//1184
	int  DocNumLock(char const *,char const *);	//1245
	int  DocNumUnLock(char const *,char const *);	//1247
	virtual void  Download(class CDB7Stream &);	//1275
	void  EnableAutoRegister(int);	//1291
	void  EnableDeleteChilds(int);	//1294
	void  EnableDeleteDocParams(int);	//1296
	void  EndReadParams(void);	//1302
	virtual void  EscalateTLock(enum TLockLevel);	//1309
	virtual long  GetCurrentPos(void);	//1600
	int  GetFilterCurrentPos(class CObjID);	//1749
protected:
	int  GetKeyKind(void)const;	//1862
public:
	int  GetKeyValueList(class PValueItemArray *,int,int,int,class CObjID,class CObjID,int);	//1871
	virtual long  GetNearPos(enum MoveAfterDel);	//2058
	int  GetParamIdx(long);	//2124
	class CString   GetPresent(void);	//2164
	virtual long  GetRelPos(void);	//2326
	class CSbCntTypeDef *  GetTypeDef(void)const;	//2503
	virtual int  GotoCurrent(void);	//2564
	virtual int  GotoFirst(void);	//2579
	virtual int  GotoLast(void);	//2596
	virtual int  GotoNext(void);	//2611
	virtual void  GotoNull(void);	//2622
	virtual long  GotoPos(long);	//2633
	virtual int  GotoPrev(void);	//2645
	int  IncrementDocNum(class CString &,class CSbCntTypeDef *);	//2671
protected:
	void  InitKey(int,int);	//2702
public:
	virtual void  InitNew(void);	//2717
protected:
	void  InitParamSet(void);	//2727
	void  InitParentKey(int,int);	//2728
public:
	int  IsAutoRegister(void)const;	//2784
	virtual int  IsMultSel(void);	//2833
	virtual int  IsOK(void);	//2843
	int  IsParamDirty(int)const;	//2857
	int  IsParamReaded(int);	//2860
	void  LinkAllParams(int,int);	//2905
	void  LinkParam(int,int,int);	//2919
	void  LinkParam(class CSbCntParamDef *,int,int);	//2920
	virtual int  Lock(void);	//2972
	int  MarkDelete(int,int);	//3002
	virtual int  New(void);	//3039
	void  ReadAllParams(void);	//3151
	void  ReadParam(int);	//3166
	void  ReadParam(class CSbCntParamDef *);	//3167
	void  ReadParamWhithCheck(int);	//3169
	int  SelectAll(class CDate,int);	//3333
	int  SelectByParentValue(class CValue *,long,class CDate,class CObjID,class CObjID,int,int);	//3351
	int  SelectByParentValueView(class CValue *,long,class CDate,class CObjID,class CObjID);	//3352
	int  SelectByValue(class CValue *,long,class CDate,int);	//3359
	int  SelectCode(char const *,class CDate,int,class CValue *,long);	//3367
	int  SelectDescr(char const *,class CDate,int,class CValue *,long);	//3372
	int  SelectID(class CObjID,class CDate);	//3389
	int  SelectParent(class CObjID,class CObjID,class CDate,int);	//3407
	int  SelectParentCode(char const *,class CDate,class CObjID,class CObjID,int,class CValue *,long);	//3410
	int  SelectParentDescr(char const *,class CDate,class CObjID,class CObjID,int,class CValue *,long);	//3412
	int  SelectParentFolder(class CObjID,class CObjID,class CDate,int,int);	//3413
	int  SelectParentValue(class CValue *,long,class CDate,class CObjID,class CObjID,int,int);	//3415
	int  SelectValue(class CValue *,long,class CDate,int,int);	//3445
	void  SetAllParamDirty(void);	//3470
	virtual void  SetBufferSize(short);	//3488
	void  SetFilterList(class PValueItemArray *);	//3581
	int  SetFilterPos(void);	//3582
	int  SetNewCode(class CObjID,class CObjID,class CString);	//3644
	void  SetParamDirty(int);	//3673
	void  SetParamDirty(class CSbCntParamDef *);	//3674
	void  SetParamReaded(int);	//3676
	virtual void  SetRelPos(long,int);	//3725
	void  SetSearchDate(class CDate);	//3741
	void  SetSearchID(class CObjID);	//3742
	virtual int  SetSelectionFields(void);	//3762
	int  SetType(class CSbCntTypeDef *);	//3782
	int  TestCodeUnique(class CObjID,class CObjID);	//3895
	virtual int  TestSelectionFields(void);	//3916
private:
	int  TestSelectionFieldsPriv(class CSbCntRec *);	//3921
public:
	virtual void  UnSelect(void);	//3953
	virtual int  Unlock(void);	//3968
	virtual int  Update(void);	//3992
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4020
};


class IMPORT_1C CSbCntTypeDef: public CMetaDataObj
{
public:
	char buffer [4000];
	 CSbCntTypeDef(class CItemList const &);	//274
	 CSbCntTypeDef(long,char const *,char const *,long);	//275
	virtual  ~CSbCntTypeDef(void);	//471
	void  Accept(void);	//792
	int  AddParamDef(class CSbCntParamDef *);	//829
	virtual void  AddToList(class CItemList &)const;	//892
	void  DelParamDef(int,int);	//1144
	void  DelParamDef(class CSbCntParamDef *,int);	//1145
	int  GetCheckCodeUnique(void)const;	//1511
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1525
	virtual char const *  GetChildName(int,int)const;	//1540
	int  GetCodeAuto(void)const;	//1547
	int  GetCodeEditType(void)const;	//1548
	int  GetCodeLen(void)const;	//1550
	int  GetCodeUnique(void)const;	//1555
	int  GetDescLen(void)const;	//1653
	int  GetEditType(void)const;	//1687
	int  GetFolderFirst(void)const;	//1761
	class CFormDefsArray *  GetFormDefsArray(void);	//1764
	int  GetLevelsLimit(void)const;	//1881
	static char const *  GetListFunctor(void);	//1926
	int  GetMainPresent(void)const;	//1944
	virtual int  GetNChilds(void)const;	//1991
	int  GetNParamDefs(void)const;	//2007
	virtual int  GetNProps(void)const;	//2041
	class CMetaDataObjDistr *  GetObjDistr(void);	//2093
	class CSbCntParamDef *  GetParamDef(long)const;	//2116
	class CSbCntParamDef *  GetParamDef(char const *)const;	//2117
	class CSbCntParamDef *  GetParamDefAt(int)const;	//2118
	int  GetParamDefIdx(long)const;	//2119
	int  GetParamDefIdx(class CSbCntParamDef *)const;	//2120
	int  GetParamDefIdx(char const *)const;	//2121
	class CMetaDataObjArray *  GetParams(void);	//2126
	long  GetParentID(void)const;	//2130
	virtual char const *  GetPropName(int,int)const;	//2214
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2246
	char const *  GetTableName(void)const;	//2464
	int  GetUseItemFormForFolder(void)const;	//2523
	int  HasDateDepParams(void)const;	//2651
	int  HasFieldSelect(void);	//2653
	int  HasSeparetedParams(void)const;	//2660
	int  InsertParamDef(int,class CSbCntParamDef *);	//2763
	int  MoveParamDef(class CSbCntParamDef *,int);	//3016
	virtual void  ScanMDObjects(class SScanMDObjects *);	//3314
	void  SetCheckCodeUnique(int);	//3495
	void  SetCodeAuto(int);	//3501
	void  SetCodeEditType(int);	//3502
	void  SetCodeLen(int);	//3503
	void  SetCodeUnique(int);	//3505
	void  SetDescLen(int);	//3554
	void  SetEditType(int);	//3564
	void  SetFolderFirst(int);	//3589
	void  SetLevelsLimit(int);	//3623
	void  SetMainPresent(int);	//3633
	void  SetParentID(long);	//3678
	void  SetUseItemFormForFolder(int);	//3793
	int  SortParamDefs(void);	//3830
	virtual int  WhatIsIt(void)const;	//4114
};


class IMPORT_1C CSbKindDef: public CMetaDataObj
{
public:
DWORD m_0;
long  m_Kind;
int   m_TypeCode;
public:
	 CSbKindDef(class CSbKindDef const &);	//276
	 CSbKindDef(class CItemList const &);	//277
	 CSbKindDef(long,char const *,char const *,class CType const &);	//278
	 CSbKindDef(class CMetaDataTypedObj const *);	//279
	virtual  ~CSbKindDef(void);	//472
	class CSbKindDef &  operator=(class CSbKindDef const &);	//566
	virtual void  AddToList(class CItemList &)const;	//893
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1526
	virtual char const *  GetChildName(int,int)const;	//1541
	long  GetCurrencyPriceID(void)const;	//1590
	int  GetEnableEmpty(void)const;	//1689
	static char const *  GetListFunctor(void);	//1927
	virtual int  GetNChilds(void)const;	//1992
	virtual int  GetNProps(void)const;	//2042
//	class CMetaDataObjArrayTemplate<class CPresentItem> *  GetPresentFields(void)const;	//2169
	long  GetPriceID(void)const;	//2175
	virtual char const *  GetPropName(int,int)const;	//2215
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2247
	int  GetQuickChoice(void)const;	//2275
	void  SetCurrencyPriceID(long);	//3519
	void  SetEnableEmpty(int);	//3566
	void  SetPriceID(long);	//3691
	void  SetQuickChoice(int);	//3709
	virtual int  WhatIsIt(void)const;	//4115
};


class IMPORT_1C CSubcService
{
public:
	 CSubcService(void);	//280
	 ~CSubcService(void);	//473
	class CSubcService &  operator=(class CSubcService const &);	//568
	void  Done(void);	//1260
	int  GetDateDepParam(class CMetaDataTypedObj *,class CObjID,class CDate,class CValue *,class CTypedValue *);	//1626
	class CString   GetDefaultPrefix(int);	//1646
	class CString   GetFullCode(class CSbCntTypeDef *,class CObjID,class CSbCntSet *);	//1766
	class CString   GetFullName(class CSbCntTypeDef *,class CObjID,class CSbCntSet *);	//1769
	class CString   GetFullPresent(class CSbCntTypeDef *,class CObjID,class CSbCntSet *);	//1770
	class CString   GetPresent(long,class CObjID);	//2165
	int  GetSubcLevel(class CSbCntTypeDef *,class CObjID);	//2451
	class CSbCntSet *  GetSubcSet(class CSbCntTypeDef *);	//2453
	class CSbCntSet *  Init(long);	//2679
	class CSbCntSet *  Init(class CSbCntTypeDef *);	//2680
	int  IsChildOfFolder(class CSbCntTypeDef *,class CObjID,class CSbCntSet *,class CObjID);	//2790
	int  IsFolder(class CValue const *);	//2810
	int  IsItem(class CValue const *);	//2814
	void  MainInit(void);	//2987
	void  SetDefaultPrefix(int,class CString);	//3552
};


class IMPORT_1C CSystemSet
{
public:
	 CSystemSet(class CDataBase7 *);	//281
	 ~CSystemSet(void);	//474
	class CSystemSet &  operator=(class CSystemSet const &);	//569
	int  CompareTA(class CDate,class CEventTime);	//1025
	void  Download(class CDB7Stream &);	//1276
	class CDate   GetAccDate(void);	//1403
	int  GetAccRecalc(void);	//1417
	int  GetAccSelRecalc(void);	//1419
	struct _GUID const &  GetDBSetUUID(void)const;	//1611
	class CDBSign   GetDBSign(void);	//1614
	class CDate   GetDateTA(void);	//1629
	class CDate   GetPeriodTA(void);	//2138
	int  GetSbSelRecalc(void);	//2408
	enum PeriodType   GetSnapShotPeriod(void);	//2441
	class CEventTime   GetTimeTA(void);	//2479
	int  Lock(void);	//2973
private:
	int  New(void);	//3040
	int  Refresh(void);	//3190
public:
	int  SetAccDate(class CDate);	//3454
	int  SetAccRecalc(int);	//3459
	int  SetAccSelRecalc(int);	//3460
	int  SetDBSetUUID(struct _GUID const &);	//3527
	int  SetDBSign(class CDBSign);	//3529
	int  SetDateTimeTA(class CDate,class CEventTime);	//3539
	int  SetSbSelRecalc(int);	//3740
	int  SetSnapShotPeriod(enum PeriodType);	//3768
	void  Unlock(void);	//3969
private:
	int  Update(void);	//3993
public:
	void  Upload(class CDB7Stream &);	//4021
};


class IMPORT_1C CTaskDef: public CMetaDataObj
{
public:
	 CTaskDef(class CTaskDef const &);	//282
	 CTaskDef(class CMetaDataCont *,class CItemList const &);	//283
	 CTaskDef(class CMetaDataCont *,long,char const *,char const *);	//284
	virtual  ~CTaskDef(void);	//475
	class CTaskDef &  operator=(class CTaskDef const &);	//570
	virtual void  AddToList(class CItemList &)const;	//894
	void  DescribeSpecial(class CString &,class CMetaDataCont *,int);	//1226
	virtual class CMetaDataObjArray *  GetChild(int,class CMetaDataCont *);	//1527
	virtual char const *  GetChildName(int,int)const;	//1542
	int  GetDefaultLanguage(void)const;	//1642
	int  GetEnableImmediateDelete(void)const;	//1690
	long  GetJournalForAllDocs(void)const;	//1853
	long  GetJournalForChildsDocs(void)const;	//1854
	static char const *  GetListFunctor(void);	//1928
	class CString   GetMDPassword(void)const;	//1942
	virtual int  GetNChilds(void)const;	//1993
	virtual int  GetNProps(void)const;	//2043
	virtual char const *  GetPropName(int,int)const;	//2216
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2248
	int  GetUseBuchRounding(void)const;	//2521
	int  HasSameDefaultLanguage(class CTaskDef const *);	//2659
	virtual void  LoadFromList(class CItemList const &,int &);	//2946
	void  SetDefaultLanguage(int);	//3547
	void  SetDefaultLanguageImitation(int);	//3548
	void  SetEnableImmediateDelete(int);	//3569
	void  SetJournalForAllDocs(long);	//3619
	void  SetJournalForChildsDocs(long);	//3620
	void  SetMDPassword(class CString);	//3632
	void  SetUseBuchRounding(int);	//3792
	virtual int  WhatIsIt(void)const;	//4116
};


class IMPORT_1C CTempAccTotal
{
public:
	 CTempAccTotal(class CTempAccTotal const &);	//285
	 CTempAccTotal(void);	//286
	 ~CTempAccTotal(void);	//476
	class CTempAccTotal &  operator=(class CTempAccTotal const &);	//571
private:
	void  AddProvToAccTotal(int,class CProvObj *,int,int,int,class CNumeric);	//835
public:
	void  AddProvToAccTotals(class CProvObj *,int,int,int);	//836
private:
	void  AddProvToTotal(int,class CProvObj *,class CNumeric,int);	//837
public:
	void  AddProvToTotals(class CProvObj *,int);	//838
private:
	void  AddProvs(class CDate const &,class CDate const &,class CEventTime const &,class CEventTime const &,int,int);	//841
public:
	void  CalcTempTotals(class CDate const &,class CDate const &,class CEventTime const &,class CEventTime const &,int,int,class CValue const &,class CObjIDSet *,class CObjIDSet *,class std::vector<struct std::pair<class CString,long>,class std::__alloc<1,0> > *,long);	//930
private:
	int  CheckProvs(class CProvSet &,class CObjIDSet *);	//977
public:
	int  InitAccTotalsTable(int,int);	//2685
private:
	int  ODBC30AddProvs(class CDate const &,class CEventTime const &,class CDate const &,class CEventTime const &,int,int);	//3054
	int  ODBC30TempCalc(void);	//3055
public:
	void  OnUpdate(void);	//3069
private:
	void  ProcessAccTotal(class CDate const &,int,class CDate const &,int,class CAccTotalSet &,int,class CAccTotalItem &);	//3097
	void  ProcessTotal(class CDate const &,int,class CDate const &,int,class CTotalSet &,class CTotalItem &);	//3120
public:
	void  RecalcTempTotals(class CDate const &,class CDate const &,class CEventTime const &,class CEventTime const &);	//3184
	void  StartTransaction(void);	//3860
	void  StopTransaction(void);	//3876
};


class IMPORT_1C CTempDB
{
public:
	 CTempDB(class CTempDB const &);	//287
	 CTempDB(void);	//288
	 ~CTempDB(void);	//477
	class CTempDB &  operator=(class CTempDB const &);	//572
private:
	int  CreateAdmDir(char const *);	//1053
public:
	class CTable *  CreateTable(void);	//1067
	void  DeInit(void);	//1083
	void  DestroyTable(class CTable *);	//1227
	class CDataBase7 *  GetDB7(void);	//1605
	int  Init(char const *);	//2681
	void  OnUpdate(void);	//3070
private:
	void  RemoveAdmDir(void);	//3221
public:
	void  StartTransaction(void);	//3861
	void  StopTransaction(void);	//3877
};


class IMPORT_1C CTempProv
{
public:
	 CTempProv(void);	//289
	 ~CTempProv(void);	//478
	class CTempProv &  operator=(class CTempProv const &);	//573
	class CProvSet *  GetProvSet(void);	//2269
	void  InitTable(void);	//2735
	void  OnUpdate(void);	//3071
	void  StartTransaction(void);	//3862
	void  StopTransaction(void);	//3878
};


class IMPORT_1C CTempRegs
{
public:
	 CTempRegs(class CTempRegs const &);	//290
	 CTempRegs(void);	//291
	 ~CTempRegs(void);	//479
	class CTempRegs &  operator=(class CTempRegs const &);	//574
	void  AddRegs(long,class CValue * *,class CValue * *,int,int);	//849
	int  CalcTmpRegs(class CDate,class CEventTime,class CDocSelRefDef *,int);	//931
private:
	int  CalcTurnoversRegs(class CDate,class CEventTime,class CDocSelRefDef *,int);	//934
	int  ClearCurSet(void);	//987
	void  CreateTable(int);	//1068
public:
	class CDataBase7 *  GetDB7(void);	//1606
	class CRegFilter *  GetFilter(int);	//1748
	class CRegSet *  GetRegSet(int);	//2318
private:
	int  InitReg(int);	//2734
public:
	void  JoinFilters(void);	//2890
	void  OnUpdate(void);	//3072
	void  SeparateFilters(void);	//3448
	void  SetFilters(int,class CValue * *,int);	//3583
	void  SetFilters(int,class CRegFilter *);	//3584
	void  StartTransaction(void);	//3863
	void  StopTransaction(void);	//3879
	int  TestUseRegs(int);	//3926
	class CDocSelRefDef *  TestUseSelRef(class CString *);	//3928
	int  UseReg(int,int);	//4029
};


class IMPORT_1C CTotalService
{
public:
	 CTotalService(void);	//292
	class CTotalService &  operator=(class CTotalService const &);	//575
	int  CheckCorrectTotals(void);	//967
	void  Done(void);	//1261
	class CAccTotalSet *  GetAccTotalSet(void);	//1423
	int  GetCorrectTotals(void);	//1579
	class CTotalSet *  GetTotalSet(void);	//2488
	int  GetTotals(class CDate,class CDate,class CObjID,class CObjID,int,class CNumeric *,class CNumeric *,class CNumeric *,class CNumeric *,class CNumeric *,class CNumeric *,class CValue);	//2490
	void  SetCorrectTotals(int);	//3509
};


class IMPORT_1C CTotalSet:public CUniSet
{
DECLARE_DYNAMIC(CTotalSet)

public:
	 CTotalSet(class CDataBase7 *,char const *);	//293
	virtual  ~CTotalSet(void);	//480
//	void  `default constructor closure'(void);	//782
	int  CalcTurnoverBetweenAccountsByQuartWithTmpProvTbl(class CDate);	//933
	int  ChangeTotal(class CDate,int,class CProvObj *,int,class CNumeric);	//954
	int  ChangeTotals(class CProvObj *,int,class CDate);	//956
	int  CreateTmpProvTbl(class CDate,class CEventTime,class CDate,class CEventTime);	//1071
	virtual int  Delete(enum MoveAfterDel);	//1185
	int  DeleteByPeriod(class CDate,class CDate);	//1201
	int  DeleteProv(class CProvObj *,class CDate);	//1215
	void  DropTmpProvTbl(void);	//1285
	class CMetaDataTypedObj *  GetAccountingDivisionDef(void)const;	//1427
	virtual int  GotoFirst(void);	//2580
	virtual int  GotoLast(void);	//2597
	virtual int  GotoNext(void);	//2612
	virtual int  GotoPrev(void);	//2646
	virtual void  InitNew(void);	//2718
	virtual int  IsMultSel(void);	//2834
	virtual int  New(void);	//3041
	int  NewProv(class CProvObj *,class CDate);	//3046
	int  Select(class CDate,int,class CObjID,class CObjID,class CObjID,int,class CValue);	//3322
	int  SelectAll(int);	//3334
	int  SelectPeriod(class CDate,class CDate,int);	//3422
	int  SelectWithFilter(class CDate,int,class CObjID,class CObjID,class CObjID,int,class CValue);	//3447
	void  SetAccDiv(class CValue const &,class CValue &);	//3457
protected:
	void  SetCurOrder(int);	//3515
public:
	virtual int  SetSelectionFields(void);	//3763
	int  TestAllFields(void);	//3892
	virtual int  TestSelectionFields(void);	//3917
	virtual int  Update(void);	//3994
	int  UpdateProv(class CProvObj *,class CProvObj *,class CDate);	//4004
};


class IMPORT_1C CTypOperService
{
public:
	 CTypOperService(void);	//294
	 ~CTypOperService(void);	//481
	class CTypOperService &  operator=(class CTypOperService const &);	//576
	void  Done(void);	//1262
	class CString   GetFullCode(class CObjID,class CTypOperSet *);	//1767
	class CString   GetPresent(class CObjID);	//2166
	int  GetTypOperLevel(class CObjID);	//2492
	class CTypOperSet *  GetTypOperSet(void);	//2494
	void  Init(void);	//2682
	int  IsChildOfFolder(class CObjID,class CTypOperSet *,class CObjID);	//2791
};


class IMPORT_1C CTypOperSet:public CObjectSet
{
DECLARE_DYNAMIC(CTypOperSet)

public:
	char buffer[4000];
	 CTypOperSet(class CDataBase7 *);	//295
	virtual  ~CTypOperSet(void);	//482
//	void  `default constructor closure'(void);	//783
	void  ClearReaded(void);	//1003
	virtual int  Delete(enum MoveAfterDel);	//1186
	virtual void  Download(class CDB7Stream &);	//1277
	void  EnableDeleteChilds(int);	//1295
	class CString   GetPresent(void);	//2167
	void  GetTemplateInfo(class CString &);	//2477
	virtual int  GotoFirst(void);	//2581
	virtual int  GotoLast(void);	//2598
	virtual int  GotoNext(void);	//2613
	virtual int  GotoPrev(void);	//2647
protected:
	void  InitKey(void);	//2703
public:
	virtual void  InitNew(void);	//2719
protected:
	void  InitParentKey(void);	//2729
public:
	virtual int  IsMultSel(void);	//2835
	virtual int  New(void);	//3042
	int  SelectAll(void);	//3335
	int  SelectCode(char const *,int);	//3368
	int  SelectID(class CObjID);	//3390
	int  SelectParent(class CObjID);	//3408
	int  SelectParentCode(char const *,class CObjID,int);	//3411
	int  SelectParentFolder(class CObjID,int);	//3414
	virtual int  SetSelectionFields(void);	//3764
	void  SetTemplateInfo(class CString const &);	//3774
	int  TestCodeUnique(class CObjID);	//3896
	virtual int  TestSelectionFields(void);	//3918
	virtual void  UnSelect(void);	//3954
	virtual int  Update(void);	//3995
	virtual void  Upload(class CDB7Stream &,class CDBSign,int);	//4022
};


class IMPORT_1C CTypOpersDef
{
public:
	 CTypOpersDef(class CTypOpersDef const &);	//296
	 CTypOpersDef(class CItemList const &);	//297
	 CTypOpersDef(long,char const *,char const *);	//298
	virtual  ~CTypOpersDef(void);	//483
	class CTypOpersDef &  operator=(class CTypOpersDef const &);	//577
	virtual void  AddToList(class CItemList &)const;	//895
	static char const *  GetListFunctor(void);	//1929
	virtual int  GetNProps(void)const;	//2044
	class CMetaDataObjDistr *  GetObjDistr(void);	//2094
	virtual char const *  GetPropName(int,int)const;	//2217
	virtual int  GetPropVal(int,class CMetaDataCont *,class CValue &,class CMetaDataObj * *)const;	//2249
	virtual int  WhatIsIt(void)const;	//4117
};


class IMPORT_1C CType
{
public:
	DWORD m_length;
	DWORD m_mdid;
	WORD type;
	BYTE m_prec;
	BYTE m_flags;	// Для чисел 1-разделять триады, 2 - неотрицательный
	 CType(class CType const &);	//299
	 CType(int);	//300
	 CType(int,int,int);	//301
	 CType(int,int,int,long);	//302
	 CType(int,long);	//303
	 ~CType(void);	//484
	class CType &  operator=(class CType const &);	//578
	int  operator==(class CType const &)const;	//610
	int  operator!=(class CType const &)const;	//617
//	void  `default constructor closure'(void);	//784
	void  AssignWithoutFormat(class CType const &);	//904
	static int  C2TypeCode(char);	//925
	char  GetCTypeCode(void)const;	//1491
	int  GetLength(void)const;	//1880
	int  GetPrecision(void)const;	//2153
	int  GetTypeCode(void)const;	//2500
	long  GetTypeID(void)const;	//2506
	class CString   GetTypeTitle(void);	//2516
	int  IsNumSeparated(void)const;	//2837
	int  IsObjReference(void);	//2845
	int  IsPositiveOnly(void)const;	//2861
	int  IsValid(void);	//2888
	void  LoadTypeFromList(class CItemList const &,int &);	//2963
	void  SaveTypeToList(class CItemList *)const;	//3299
	void  SetFormat(int,int);	//3590
	void  SetNumSeparated(int);	//3654
	void  SetPositiveOnly(int);	//3689
	void  SetTypeCode(int);	//3785
	void  SetTypeID(long);	//3787
};


class IMPORT_1C CTypedValue
{
public:
	 CTypedValue(class CTypedValue const &);	//304
	 CTypedValue(class CType const &);	//305
	 CTypedValue(void);	//306
	virtual  ~CTypedValue(void);	//485
	class CTypedValue &  operator=(class CTypedValue const &);	//579
	class CValue const &  operator=(class CValue const &);	//580
	class CValue const &  operator=(class CNumeric const &);	//581
	class CValue const &  operator=(long);	//582
	class CValue const &  operator=(char const *);	//583
	class CValue const &  operator=(class CDate);	//584
protected:
	virtual int  IsTypeSafe(void)const;	//2880
};


class IMPORT_1C CUpDlFNStr
{
public:
	 CUpDlFNStr(class CUpDlFNStr const &);	//308
	 CUpDlFNStr(class CString const &);	//309
	 ~CUpDlFNStr(void);	//487
	class CUpDlFNStr &  operator=(class CUpDlFNStr const &);	//585
	class CString   Ext(void);	//1317
	void  IncFileName(void);	//2665
	class CString   Prefix(void);	//3090
	void  Split(class CString &,class CString &,class CString &);	//3853
	int  SuffExists(void);	//3882
	class CString   Suffix(void);	//3883
};


class IMPORT_1C CVTColumn
{
public:
	 CVTColumn(class CVTColumn const &);	//310
	 CVTColumn(class CString const &,class CType const &,class CString const &,int);	//311
	 ~CVTColumn(void);	//488
	class CVTColumn &  operator=(class CVTColumn const &);	//587
	void  Add(class CValue const &,int);	//798
	void  AddItems(int,int);	//825
	void  Clear(void);	//980
	int  Compare(int,int)const;	//1023
	void  EmptyKeys(void);	//1288
	void  FillKeys(int);	//1344
	int  Find(class CValue const &,int &);	//1350
	class CValue const &  Get(int)const;	//1398
	int  GetAlign(void)const;	//1446
	enum EArrayType   GetArrayType(void)const;	//1456
	class CString const &  GetCode(void)const;	//1546
	class CString const &  GetFormat(void)const;	//1765
	int  GetImageOffset(void)const;	//1815
	class CString &  GetKeyFromValue(class CValue const &,class CString &,int)const;	//1861
	unsigned int  GetSize(void)const;	//2439
	int  GetSortDirection(void)const;	//2442
	class CString const &  GetTitle(void)const;	//2482
	class CType const &  GetType(void)const;	//2499
	int  GetViewAsImage(void)const;	//2552
	int  GetVisNumber(void)const;	//2553
	int  GetVisible(void)const;	//2554
	int  GetWidth(void)const;	//2555
	void  Insert(class CValue const &,int);	//2744
	void  Move(int,int);	//3007
	void  Remove(int,int);	//3220
	void  Set(class CValue const &,int);	//3450
	void  SetAlign(int);	//3467
	void  SetCode(class CString const &);	//3500
	void  SetFormat(class CString const &);	//3591
	void  SetImageOffset(int);	//3611
	void  SetSortDirection(int);	//3769
	void  SetTitle(class CString const &);	//3779
	void  SetType(class CType const &);	//3783
	void  SetViewAsImage(int);	//3805
	void  SetVisNumber(int);	//3806
	void  SetVisible(int);	//3807
	void  SetWidth(int);	//3808
	class CNumeric   Total(void);	//3936
};


class IMPORT_1C CValue: public CType
{
public:
	// void** pVtable
	class CNumeric m_Number; // числовое значение // align 8
	class CString m_String; // строковое значение
	class CDate m_Date; // значение даты
	long ValTypeID; //int Flag6;
	CBLContext* /* void* */ m_Context; // значение контекста
	class CObjID m_ObjID; // значение в базе
	// total obj size = 84



	 CValue(class CValue const &);	//312
	 CValue(class CNumeric const &);	//313
	 CValue(class CType const &);	//314
	 CValue(long);	//315
	 CValue(char const *);	//316
	 CValue(class CDate);	//317
	 CValue(void);	//318
	class CValue const &  operator=(class CValue const &);	//588
	class CValue const &  operator=(class CNumeric const &);	//589
	class CValue const &  operator=(long);	//590
	class CValue const &  operator=(char const *);	//591
	class CValue const &  operator=(class CDate);	//592
	int  operator==(class CValue const &)const;	//611
	int  operator!=(class CValue const &)const;	//618
	int  AssignContext(class CBLContext *);	//903
	int  CopyToClipboard(class CWnd *,char const *);	//1051
	int  CreateObject(char const *);	//1064
	int  FastSaveToString(class CString &);	//1338
	char const *  Format(void)const;	//1380
	class CBLContext *  GetContext(void)const;	//1577
	class CDate   GetDate(void)const;	//1623
	static enum CDateFormat   GetDefDateFmt(void);	//1639
	class CNumeric const &  GetNumeric(void)const;	//2085
	class CObjID   GetObjID(void)const;	//2096
	long  GetRealTypeID(void)const;	//2286
	class CString const &  GetString(void)const;	//2449
	char const *  GetTypeString(void)const;	//2515
	long  GetValTypeID(void)const;	//2537
protected:
	void  Init(void);	//2684
public:
	int  IsEmpty(void)const;	//2803
	int  IsExactValue(void)const;	//2809
protected:
	virtual int  IsTypeSafe(void)const;	//2881
public:
	virtual  ~CValue(void);	//489
	void  Link(int,int);	//2894
	int  LinkContext(int);	//2907
	int  LoadFromString(char const *,int);	//2949
	int  LoadValueFromList(class CItemList *,int);	//2964
	void  MakeExactValueFrom(class CValue const *);	//2992
	int  PasteFromClipboard(class CWnd *);	//3089
	void  Reset(void);	//3232
	int  SaveToString(class CString &);	//3297
	int  SaveValueToList(class CItemList *)const;	//3300
	static void  SetDefDateFmt(enum CDateFormat);	//3544
	void  SetObjID(class CObjID);	//3658
	void  SetType(class CType const &);	//3784
	void  SetValTypeID(long);	//3798
	void  UnlinkContext(void);	//3961
	void  ValidateType(void);	//4034
	static unsigned int  cfValueId;	//4193
};

// пример работы с объектом "СписокЗначений" как с объектом С++, а не через контекст
// можно посмотреть у ДмитрО в MetaDataWork.cpp или 1CPP_ODBCRecordSet.cpp
class IMPORT_1C CValueItem
{
public:
	CValueItem(const CValue& val, const CString& txt) : m_value(val), m_text(txt){}
	CValue m_value;
	CString m_text;
	BOOL m_bCheck;
	 CValueItem(class CValueItem const &);	//319
	 CValueItem(void);	//320
	 ~CValueItem(void);	//490
	class CValueItem &  operator=(class CValueItem const &);	//593
	int  GetCheck(void);	//1510
	class CString   GetPresent(void);	//2168
	void  SetCheck(int);	//3494
};

// пример работы с объектом "СписокЗначений" как с объектом С++, а не через контекст
// можно посмотреть у ДмитрО в MetaDataWork.cpp или 1CPP_ODBCRecordSet.cpp
class IMPORT_1C CValueListContextData:public CBLContextInternalData
{
DECLARE_DYNAMIC(CValueListContextData)

public:
	 CValueListContextData(void);	//321
	virtual  ~CValueListContextData(void);	//491
	CArray<class CValueItem *,class CValueItem *> *  GetValueList(void);	//2545
	void  SetValueList(class PValueItemArray *);	//3802
};

// пример работы с объектом "ТаблицаЗначений" как с объектом С++, а не через контекст
// можно посмотреть у ДмитрО в 1CPP_ODBCRecordSet.cpp
class IMPORT_1C CValueTable
{
public:
	BYTE m_unknown[0x38];
	BOOL m_bEnableRefresh;

	 CValueTable(class CValueTable const &);	//322
	 CValueTable(void);	//323
	virtual  ~CValueTable(void);	//492
	class CValueTable &  operator=(class CValueTable const &);	//594
	int  FindValue(class CValue const &,int &,int &);	//1373
	class CVTColumn *  GetColumn(int)const;	//1556
	class CString const &  GetColumnCode(int)const;	//1557
	int  GetColumnCount(void)const;	//1558
	int  GetColumnIdx(class CString const &)const;	//1559
	int  GetColumns(class CString const &,class std::vector<class CVTColumn *,class std::__alloc<1,0> > &,int);	//1560
	int  GetIndex(int)const;	//1824
	int  GetRowCount(void)const;	//2341
	class CValue const &  GetValue(int,int)const;	//2541
protected:
	void  ResetSort(void);	//3238
public:
	class CNumeric   Total(int);	//3937
	static enum EValueTableError  CValueTable::s_ErrorCode;	//4244
public: //v_table
virtual void  SetColumnCount(int Count);
virtual int  AddColumn(class CString const &,class CType const &,class CString const &,int,class CString const &,int);
virtual int  InsertColumn(int,class CVTColumn *);
virtual int  InsertColumn(class CString const &,int,class CType const &,class CString const &,int,class CString const &,int);
virtual void  DeleteColumn(int);
virtual void  SetRowCount(int Count);
virtual int  NewRow(int);
virtual int  AddRow(class CValue * * ppValues, int nColAfter);
virtual void  DeleteRow(int);
virtual int  MoveRow(int,int);
virtual void  SetValue(class CValue const & val, int col, int row);
virtual int  Sort(class CString const &,int);
virtual void  Clear(BOOL bDeleteColumns);
virtual int  GroupBy(class CString const &,class CString const &);
};


class IMPORT_1C CValueTableContextData:public CBLContextInternalData
{
DECLARE_DYNAMIC(CValueTableContextData)

public:
	 CValueTableContextData(void);	//324
	virtual  ~CValueTableContextData(void);	//493
	class CValueTable *  GetValueTable(void);	//2546
	void  SetValueTable(class CValueTable *);	//3804
};


class IMPORT_1C RIArray
{
public:
	 RIArray(void);	//325
	 ~RIArray(void);	//494
	void  Add(class C1SZReferInfo *);	//799
	int  FindByID(long);	//1357
	int  FindByName(class CString);	//1358
	int  FindMacro(class CString);	//1365
	class C1SZReferInfo *  GetAt(int);	//1466
	int  GetSize(void);	//2440
};


class IMPORT_1C SScanMDObjects
{
public:
	 SScanMDObjects(void);	//339
	 ~SScanMDObjects(void);	//497
};


class IMPORT_1C CAccCode
{
public:
	class CAccCode &  operator=(class CAccCode const &);	//498
	static class CString   ChangeCode(class CString,class CString);	//948
	static class CString &  DelSpacesFromCode(class CString &);	//1162
	static int  GetCodeLevel(class CString);	//1551
	static class CString   GetParentCode(class CString);	//2128
	static class CString   GetParentCode(class CString,int);	//2129
	static int  IsChild(class CString,class CString);	//2789
	static int  IsEmpty(class CString);	//2798
	static class CString   JustifyCodeByMask(class CPlanDef *,class CString);	//2892
	static int  ValidateCode(class CPlanDef *,class CString);	//4031
	static int  ValidateCode(class CString,class CString);	//4032
};


class IMPORT_1C CDocSelRefItem
{
public:
	class CDocSelRefItem &  operator=(class CDocSelRefItem const &);	//526
	int  Compare(class CDocSelRefItem const &);	//1021
};


class IMPORT_1C CObjIDComp
{
public:
	class CObjIDComp &  operator=(class CObjIDComp const &);	//548
	bool  operator()(class CObjID const &,class CObjID const &)const;	//638
};


class IMPORT_1C CSortOrder
{
public:
	class CSortOrder &  operator=(class CSortOrder const &);	//567
	static int  Char2Weight(char);	//960
	static int  CheckSortOrder(unsigned int,unsigned char const *);	//978
	static unsigned int  GetCodePage(void);	//1553
	static char  GetNextChar(char);	//2070
	static char  GetPrevChar(char);	//2172
	static void  Init(void);	//2678
	static int  LoadFromFile(char const *,unsigned int &,unsigned char *,unsigned char *);	//2937
	static char  Lower2Upper(char);	//2985
	static int  SaveToFile(char const *);	//3284
	static int  SaveToFile(char const *,unsigned int,unsigned char const *,unsigned char const *);	//3285
	static char  Weight2Char(int);	//4073
private:
	static int  s_bInitialized;	//4247
	static unsigned int  s_nCodePage;	//4249
	static unsigned char *  s_ucChar2Weight;	//4258
	static unsigned char *  s_ucLower2Upper;	//4259
	static unsigned char *  s_ucWeight2Char;	//4260
};


class IMPORT_1C CUsersSet
{
public:
	class CUsersSet &  operator=(class CUsersSet const &);	//586
	static int  DecrUsersCnt(void);	//1087
	static int  DetectNetChanges(void);	//1231
	static int  GetUsersCnt(void);	//2530
	static int  IncrNetChangesCnt(void);	//2667
	static int  IncrUsersCnt(void);	//2669
	static int  Init(class CDataBase7 *);	//2683
	static int  IsInitialized(void);	//2813
private:
	static int  Lock(void);	//2975
public:
	static int  SetNetChangesCnt(long);	//3643
	static int  SetUsersCnt(int);	//3797
	static void  Undo(void);	//3956
private:
	static int  Unlock(void);	//3971
	static long  m_lOldNetChangesCnt;	//4232
	static class CDataBase7 *  s_pDB7;	//4253
	static class CSQLUsersRec *  s_pSQLUsrRec;	//4255
	static class CUsersRec *  s_pUsrRec;	//4256
};

// End class descriptions
//DmitrO: begin defs
template<class TYPE> class CTypedFldDefsArray: public CMetaDataObjArray
{
public:
	TYPE* operator[](int nAt)const { return (TYPE*)CMetaDataObjArray::operator[](nAt);}
	int AddItem(TYPE * pObj){ return CMetaDataObjArray::AddItem(pObj);}
	void DeleteItem(TYPE * pObj,int n){ CMetaDataObjArray::DeleteItem(pObj,n);}
	TYPE* GetAt(int nAt) const{ return (TYPE*)CMetaDataObjArray::GetAt(nAt);}
	int GetIndex(TYPE * pObj)const { return CMetaDataObjArray::GetIndex(pObj);}
	TYPE* GetItem(long nId)const { return (TYPE*)CMetaDataObjArray::GetItem(nId);}
	TYPE* GetItem(char const * pchName)const { return (TYPE*)CMetaDataObjArray::GetItem(pchName);}
	int InsertItem(int n,TYPE* pObj){ return CMetaDataObjArray::InsertItem(n,pObj);}
	int Move(TYPE* pObj,int n){ return CMetaDataObjArray::InsertItem(n,pObj);}
};


template<class TYPE> class CMetaDataObjArrayTemplate: public CMetaDataObjArray
{
public:
	TYPE* operator[](int nAt)const { return (TYPE*)CMetaDataObjArray::operator[](nAt);}
	int AddItem(TYPE * pObj){ return CMetaDataObjArray::AddItem(pObj);}
	void DeleteItem(TYPE * pObj,int n){ CMetaDataObjArray::DeleteItem(pObj,n);}
	TYPE* GetAt(int nAt) const{ return (TYPE*)CMetaDataObjArray::GetAt(nAt);}
	int GetIndex(TYPE * pObj)const { return CMetaDataObjArray::GetIndex(pObj);}
	TYPE* GetItem(long nId)const { return (TYPE*)CMetaDataObjArray::GetItem(nId);}
	TYPE* GetItem(char const * pchName)const { return (TYPE*)CMetaDataObjArray::GetItem(pchName);}
	int InsertItem(int n,TYPE* pObj){ return CMetaDataObjArray::InsertItem(n,pObj);}
	int Move(TYPE* pObj,int n){ return CMetaDataObjArray::InsertItem(n,pObj);}
};

//DmitrO: end defs

//CCommitListener
class IMPORT_1C CCommitListener
{
public:
	CCommitListener(void);
	~CCommitListener(void);
	int AddDownLoader(class CDownLoader *);
	void RemoveDownLoader(class CDownLoader *);
private:
	virtual void OnRealCommit(class CDataBase7 *);
	virtual void OnRealRollback(class CDataBase7 *);
};

class CDownLoader
{
};

// теперь в basic.h
// class IMPORT_1C CFormAllCtrlsContext:public CBLContext
// {
// DECLARE_DYNCREATE(CFormAllCtrlsContext)
// 
// public:
// 	void** m_Data1; // указатель на массив атрибутов формы
// 	void* m_Data2;
// 	void* m_Data3;
// 	void* m_Data4;
// 
// 	// по уточненным данным от АльФ-а и изучения исходников 1С++ это указатель на GetDoc7
// 	//CGetDoc77* m_GetDoc; // АльФ - указатель на атрибуты класса GetDoc7
// 	CGetDoc7* m_GetDoc; // АльФ - указатель на атрибуты класса GetDoc7
// 	CPtrArray m_PropArray; // массив указателей на поименованные атрибуты формы
// 	int m_Flag1;
// 	int m_Flag2;
// };

IMPORT_1C class CCommitListener * __cdecl GetCommitListener(void);
//---------------------

// Begin free functions definitions
IMPORT_1C int __cdecl AddAccountsFromMD(int);
IMPORT_1C int __cdecl AdjusteType(class CType &,class CMetaDataCont *,class CMetaDataCont *);
IMPORT_1C int __cdecl BeginTransaction(void);
IMPORT_1C int __cdecl BelongValue(class CValue *,class CValue *,int *,class CString *);
IMPORT_1C void __cdecl BinaryToHex(void const *,int,char *);
IMPORT_1C void __cdecl BkEndValueFromList(class CValue &,class CItemList const &);
IMPORT_1C void __cdecl BkEndValueToList(class CValue const &,class CItemList &);
IMPORT_1C double __cdecl CDateToDATE(class CDate);
IMPORT_1C void __cdecl CallDbDoctor(class CString const &,int);
IMPORT_1C int __cdecl CanSeparateNum(void);
IMPORT_1C long __cdecl CharIDToLong36(char const *,int);
IMPORT_1C long __cdecl CharIDToLong64(char const *,int);
IMPORT_1C int __cdecl CheckCJOnEmpty(class CCJDef *);
IMPORT_1C int __cdecl ClearAllRegs(void);
IMPORT_1C int __cdecl ClearDocPosting(int,class CDocSet *,class CConstSet *,class CDocActions *,struct SClearActsRule *);
IMPORT_1C void __cdecl ClearObjCache(void);
IMPORT_1C int __cdecl ClearTurns(class CDate,int);
IMPORT_1C class CString  __cdecl ClipSumNumToStr(class CNumeric);
IMPORT_1C class CNumeric  __cdecl ClipSumStrToNum(class CString &);
IMPORT_1C int __cdecl CommitTransaction(void);
IMPORT_1C int __cdecl CopyFiles(char const *,char const *,char const *);
IMPORT_1C class CDataDictionary * __cdecl CreateRGDD(void);
IMPORT_1C class CNumeric  __cdecl CyToNum(union tagCY const &);
IMPORT_1C class CDate  __cdecl DATEToCDate(double);
IMPORT_1C void __cdecl DX_Text(class CGetExchange *,int,class CValue &);
IMPORT_1C enum SuccessCode  __cdecl DataStructureModify(char const *,char const *,char const *,char const *,class CMetaDataCont *,class CMetaDataCont *,int (__cdecl*)(char const *),int,int);
IMPORT_1C int __cdecl DeleteAllCJDoc(class CObjID);
IMPORT_1C int __cdecl DeleteAllCJSingle(long,class CObjID);
IMPORT_1C int __cdecl DeleteAllCalcs(class CObjID);
IMPORT_1C int __cdecl DeleteAndStorno(class CObjID);
IMPORT_1C void __cdecl DownloadAll(class CDataBase7 &,class CDBSign const &,class CDB7Stream &);
IMPORT_1C void __cdecl DownloadChanges(class CDataBase7 &,class CDBSign const &,class CDB7Stream &,int &,class CObjID &);
IMPORT_1C int __cdecl EraseFiles(class CString,class CString);
IMPORT_1C int __cdecl EventPosFromStr(class CString const &,class CDate &,class CEventTime &);
IMPORT_1C class CString  __cdecl EventPosToStr(class CDate const &,class CEventTime const &);
IMPORT_1C void __cdecl FX_DBSign(class CKeyObj *,int,class CDBSign const &);
IMPORT_1C void __cdecl FX_DBSign(class CStoreObj *,int,class CDBSign &,int);
IMPORT_1C void __cdecl FX_MDID(class CKeyObj *,int,long const &);
IMPORT_1C void __cdecl FX_MDID(class CStoreObj *,int,long &,int);
IMPORT_1C void __cdecl FX_ObjID(class CKeyObj *,int,class CObjID const &);
IMPORT_1C void __cdecl FX_ObjID(class CStoreObj *,int,class CObjID &,int);
IMPORT_1C void __cdecl FX_SbCnt(class CKeyObj *,int,class CValue &,int,class CSbKindToType *);
IMPORT_1C void __cdecl FX_SbCnt(class CStoreObj *,int,class CValue &,int,int,class CSbKindToType *);
IMPORT_1C void __cdecl FX_Time(class CKeyObj *,int,long const &);
IMPORT_1C void __cdecl FX_Time(class CStoreObj *,int,long &,int);
IMPORT_1C void __cdecl FX_Type(class CKeyObj *,int,class CType const &);
IMPORT_1C void __cdecl FX_Type(class CStoreObj *,int,class CType &,int);
IMPORT_1C void __cdecl FX_UUID(class CKeyObj *,int,struct _GUID const &);
IMPORT_1C void __cdecl FX_UUID(class CStoreObj *,int,struct _GUID &,int);
IMPORT_1C void __cdecl FX_Value(class CKeyObj *,int,class CValue &,int);
IMPORT_1C void __cdecl FX_Value(class CStoreObj *,int,class CValue &,int,int);
IMPORT_1C void __cdecl FX_ValueAndType(class CKeyObj *,int,class CValue &);
IMPORT_1C void __cdecl FX_ValueAndType(class CStoreObj *,int,class CValue &,int,int,int);
IMPORT_1C int __cdecl FileCopy(char const *,char const *);
IMPORT_1C unsigned int __cdecl FileExceptionToResID(int,int *);
IMPORT_1C int __cdecl FindApp7SpecialFile(char const *,class CString &,char const *);
IMPORT_1C class CString  __cdecl FixFldsIDToStr(long);
IMPORT_1C class CString  __cdecl FixFldsIDToStr(long,int);
IMPORT_1C long __cdecl FixFldsStrToID(class CString);
IMPORT_1C char const * __cdecl FormatUUID(struct _GUID const &);
IMPORT_1C void __cdecl FormatUUID(struct _GUID const &,class CString &);
IMPORT_1C int __cdecl FromStreamTime(long,int &,int &,int &,int &);
IMPORT_1C class CAccService * __cdecl GetAccService(void);
IMPORT_1C class CString  __cdecl GetActivePresent(unsigned int,int,int);
IMPORT_1C class CDate  __cdecl GetBegOfPeriod(class CDate,enum PeriodType);
IMPORT_1C class CBkEndUI * __cdecl GetBkEndUI(void);
IMPORT_1C class CConstService * __cdecl GetConstService(void);
IMPORT_1C long __cdecl GetCrazyTime(void);
IMPORT_1C class CCrossDocSet * __cdecl GetCrossDocSet(void);
IMPORT_1C void __cdecl GetDateTime(class CDate &,long &,char *,int);
IMPORT_1C int __cdecl GetDateTimeFromValue(class CValue const &,class CDate &,class CEventTime &);
IMPORT_1C void __cdecl GetDateTimeIdDoc(class CDate &,long &,class CObjID &,char *,int);
IMPORT_1C class CDeNomination * __cdecl GetDeNomination(void);
IMPORT_1C int __cdecl GetDebugMode(void);
IMPORT_1C class CBLSpeller * __cdecl GetDefaultSpeller(void);
IMPORT_1C class CString  __cdecl GetDefaultValue(class CType const &,int);
IMPORT_1C class CDocService * __cdecl GetDocService(void);
IMPORT_1C class CDocStreamService * __cdecl GetDocStreamService(void);
IMPORT_1C class CDate  __cdecl GetEndOfPeriod(class CDate,enum PeriodType);
IMPORT_1C int __cdecl GetFindBadSymbolsInNewDocs(void);
IMPORT_1C class CTempRegs * __cdecl GetGeneralTempRegs(void);
IMPORT_1C class CLockCtrl * __cdecl GetLockCtrl(int);
IMPORT_1C class CString  __cdecl GetMDObjTitle(class CMetaDataObj *);
IMPORT_1C class CString  __cdecl GetMDTitle(class CMetaDataObj *);
IMPORT_1C class CObjID & __cdecl GetMaxObjID(long);
IMPORT_1C class CMetaDataCont * __cdecl GetMetaData(void);
IMPORT_1C int __cdecl GetNActiveTransactions(void);
IMPORT_1C class CString  __cdecl GetNameMetaDataDef(class CMetaDataObj *);
IMPORT_1C class CDate  __cdecl GetNextPeriod(class CDate);
IMPORT_1C long __cdecl GetNowTime(void);
IMPORT_1C char const * __cdecl GetObjCachePresent(int,long, class CObjID const &, int);
IMPORT_1C class CString  __cdecl GetObjIdString(class CObjID const &);
IMPORT_1C class COperService * __cdecl GetOperService(void);
IMPORT_1C class CString  __cdecl GetPeriodTitle(class CDate);
IMPORT_1C class CDate  __cdecl GetPrevPeriod(class CDate);
IMPORT_1C class CDataBase7 * __cdecl GetRGDB7(void);
IMPORT_1C class CRecalcRulesContainer * __cdecl GetRRContainer(void);
IMPORT_1C enum PeriodType  __cdecl GetRestsSnapShotPeriod(void);
IMPORT_1C class CDate  __cdecl GetSavePeriod(class CDate,class CRegDef *);
IMPORT_1C class CString  __cdecl GetSbCntPresent(class CSbKindDef *,class CValue const &,class CDate,int);
IMPORT_1C class CSubcService * __cdecl GetSubcService(void);
IMPORT_1C class CSystemSet * __cdecl GetSysSet(void);
IMPORT_1C class CTempDB * __cdecl GetTempDB(void);
IMPORT_1C class CTotalService * __cdecl GetTotalService(void);
IMPORT_1C class CTypOperService * __cdecl GetTypOperService(void);
IMPORT_1C class CString  __cdecl GetTypeFullTitle(class CType &,class CMetaDataCont *);
IMPORT_1C class CString  __cdecl GetTypeName(int,int);
IMPORT_1C class CString  __cdecl GetTypePeriodTitle(enum PeriodType);
IMPORT_1C long __cdecl GetTypeValuesCnt(class CValue *);
IMPORT_1C class CString  __cdecl GetValueTypeFullTitle(class CValue &,class CMetaDataCont *);
IMPORT_1C void __cdecl InitDateSpelling(void);
IMPORT_1C int __cdecl InitLockCtrl(int,char const *);
IMPORT_1C int __cdecl InitV7ODBC(class CSQLDataDictionary *,int);
IMPORT_1C int __cdecl IsRealDataBaseOpen(void);
IMPORT_1C int __cdecl JustDeleteDoc(class CDocSet *);
IMPORT_1C int __cdecl LogValueFromString(class CValue &,class CString &,int);
IMPORT_1C int __cdecl LogValueToString(class CValue *,class CString &);
IMPORT_1C void __cdecl LongToCharID36(long,char *,int);
IMPORT_1C void __cdecl LongToCharID64(long,char *,int);
IMPORT_1C class CString  __cdecl MakeDocNum(class CDocDef *,char const *);
IMPORT_1C class CString  __cdecl MakeDocNumInTable(class CDocDef *,char const *);
IMPORT_1C class CString  __cdecl MakePropertyString(int,char const *,char const *,int);
IMPORT_1C void __cdecl NextDateTime(class CDate const &,class CEventTime const &,class CDate &,class CEventTime &);
IMPORT_1C union tagCY  __cdecl NumToCy(class CNumeric const &);
IMPORT_1C int __cdecl OnChangeDocTime(class CDocSet *);
IMPORT_1C int __cdecl OnLoadSource(class CString *);
IMPORT_1C int __cdecl OnLoadSourcePriv(class CString *,class CString &);
IMPORT_1C int __cdecl OpenAccTotalPeriod(class CDate);
IMPORT_1C int __cdecl OpenPeriod(class CDate);
IMPORT_1C void __cdecl PrevDateTime(class CDate const &,class CEventTime const &,class CDate &,class CEventTime &);
IMPORT_1C void __cdecl PutTable(class CBufdFile &,int,class CString,class CString,class CString,int);
IMPORT_1C int __cdecl ReCalcRegs(class CDate,class CEventTime,class CDate,class CEventTime,class CDate,int);
IMPORT_1C int __cdecl ReadUUID(char const *,struct _GUID &);
IMPORT_1C int __cdecl RecalcAllAccTotals(int,int,int,int,int);
IMPORT_1C int __cdecl RecalcAllRegs(class CDate,class CEventTime);
IMPORT_1C int __cdecl RecalcCJFF(long);
IMPORT_1C int __cdecl RecalcProcess(class CRecalcStatus &);
IMPORT_1C int __cdecl RecalcProcess(class CString);
IMPORT_1C int __cdecl RecalcTurnsToDateTime(class CDate,class CEventTime);
IMPORT_1C int __cdecl RegisterRecalcs(int,class CCJDef *,class CCJRecsArray *,long,class CObjID,class CDate,class CDate,long);
IMPORT_1C enum SuccessCode  __cdecl RetryCopyNewData(char const *,char const *,char const *,char const *);
IMPORT_1C int __cdecl RollbackAllTransactions(void);
IMPORT_1C int __cdecl RollbackTransaction(void);
IMPORT_1C int __cdecl RunDlgKiller(unsigned int,class CDialog *);
IMPORT_1C void __cdecl SbCntToBuf(char *,class CValue const &,long,class CSbKindToType *,int);
IMPORT_1C int __cdecl SelectTotalsOpt(class CRegSet *,class CDate,class CRegFilter *);
IMPORT_1C void __cdecl SetBkEndUI(class CBkEndUI *);
IMPORT_1C void __cdecl SetCanSeparateNum(int);
IMPORT_1C void __cdecl SetDateTimeIdDoc(class CDate,long,class CObjID,char *,int);
IMPORT_1C int __cdecl SetDebugMode(int);
IMPORT_1C void __cdecl SetFindBadSymbolsInNewDocs(int);
IMPORT_1C void __cdecl SetFindSpecialFileFunc(int (__cdecl*)(char const *,class CString &,char const *));
IMPORT_1C void __cdecl SetLockWaitTime(int);
IMPORT_1C void __cdecl SetRestsSnapShotPeriod(enum PeriodType);
IMPORT_1C void __cdecl SetTimePresentDec(int);
IMPORT_1C void __cdecl SortValueList(class CArray<class CValueItem *,class CValueItem *> *,int,int,int);
IMPORT_1C class CString  __cdecl StdLogAccToDescr(class CAccSet *);
IMPORT_1C class CString  __cdecl StdLogDocToDescr(class CDocSet *);
IMPORT_1C class CString  __cdecl StdLogSubcToDescr(class CSbCntSet *);
IMPORT_1C class CString  __cdecl StdLogValueToDescr(class CValue *);
IMPORT_1C int __cdecl StopDlgKiller(int);
IMPORT_1C char const * __cdecl TermString(int,int);
IMPORT_1C int __cdecl TestDocRefDifferent(class CType const *,class CType const *);
IMPORT_1C int __cdecl TestSubkCodeValid(char const *,class CSbCntTypeDef *);
IMPORT_1C int __cdecl TestTypeDifferent(class CType const *,class CType const *,int);
IMPORT_1C class CDocSelRefDef * __cdecl TestUseSelRef(int,class CRegFilter *,int *,class CAllRegsFilters *,class CValue *,class CString *,int);
IMPORT_1C long __cdecl TimeFromString(char const *);
IMPORT_1C class CString  __cdecl TimeToString(long,int);
IMPORT_1C long __cdecl ToStreamTime(int,int,int);
IMPORT_1C enum PeriodType  __cdecl TypePeriodFromChar(char);
IMPORT_1C char __cdecl TypePeriodToChar(enum PeriodType);
IMPORT_1C void __cdecl UndoCrossDocSet(void);
IMPORT_1C void __cdecl UndoLockCtrl(int);
IMPORT_1C void __cdecl UndoV7ODBC(void);
IMPORT_1C int __cdecl UpdateAllCJ(long,class CObjID,long,int,class CDate);
IMPORT_1C void __cdecl UploadAll(class CDataBase7 &,class CDB7Stream &,char const *);
IMPORT_1C void __cdecl UploadChanges(class CDataBase7 &,class CDB7Stream &);
IMPORT_1C class CValue & __cdecl ValueAndTypeFromBuf(char const *,class CValue &,int,char const *,int);
IMPORT_1C void __cdecl ValueAndTypeToBuf(char *,class CValue const &,int,char *,int);
IMPORT_1C int __cdecl ValueAveragePresentLen(class CType,int *,int,int,int);
IMPORT_1C void __cdecl ValueFromDBStr(class CTypedValue *,char const *,int,char const *,int);
IMPORT_1C void __cdecl ValueToDBStr(class CTypedValue *,class CString &,int,char *,int);
// End free functions definitions
// ??_C@_04BAEN@Flds?$AA@
//??_C@_04PJJK@Form?$AA@
//??_C@_06DFEA@SbKind?$AA@
//??_C@_09CNKJ@AccSbCnts?$AA@
//??_C@_09EOJE@AccParams?$AA@
//??_C@_0L@HFCK@ProvParams?$AA@
//??_C@_0L@LHLM@OperParams?$AA@
//??_C@_0M@BFIH@TypOpersDef?$AA@
//??_C@_0M@PBIM@HolidaysDef?$AA@
//??_C@_0N@EBJO@CorrProvsDef?$AA@
//??_C@_0N@FDJM@DocSelRefObj?$AA@
//??_C@_0O@IBBG@GenJrnlFldDef?$AA@
//??_C@_0P@BNLF@ProvHardFldDef?$AA@
IMPORT_1C extern struct _GUID const  IID_IExportContext;
IMPORT_1C extern class CType const  _dummyAccount;
IMPORT_1C extern class CType const  _dummyAlhoritm;
IMPORT_1C extern class CType const  _dummyCalendar;
IMPORT_1C extern class CType const  _dummyDate;
IMPORT_1C extern class CType const  _dummyDocument;
IMPORT_1C extern class CType const  _dummyNumeric;
IMPORT_1C extern class CType const  _dummyPlanKind;
IMPORT_1C extern class CType const  _dummyString;
IMPORT_1C extern class CType const  _dummySubcKind;
IMPORT_1C extern class CType const  _dummySubconto;
IMPORT_1C extern class CType const  _dummyUndef;
