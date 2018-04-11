// Begin forward declarations
// End forward declarations
//typedef CArray<int,int &> type_1;
//typedef CArray<class CTableElement,class CTableElement &> type_2;
// Begin class descriptions

class IMPORT_1C CRGVarContext:public CObject
{
DECLARE_DYNCREATE(CRGVarContext)

public:
	 CRGVarContext(void);	//1
	virtual  ~CRGVarContext(void);	//14
	void  ConstructContext(class CRGrecord *);	//70
	virtual int  GetNProps(void)const;	//120
	virtual char const *  GetPropName(int,int)const;	//133
	virtual int  GetPropVal(int,class CValue &)const;	//135
	virtual int  IsPropReadable(int)const;	//183
	virtual int  SetPropVal(int,class CValue const &);	//232
};


class IMPORT_1C CRGbase
{
public:
	enum RGType{rgtOne};
protected:
	 CRGbase(void);	//2
	virtual  ~CRGbase(void);	//15
};


class IMPORT_1C CRGcontext:public CBLContext
{
DECLARE_DYNCREATE(CRGcontext)

public:
	 CRGcontext(void);	//3
	virtual  ~CRGcontext(void);	//16
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//62
	virtual int  CallAsProc(int,class CValue * *);	//63
	void  Destructor(void);	//73
	virtual char const *  GetMethodName(int,int)const;	//115
	virtual int  GetNMethods(void)const;	//118
	virtual int  GetNParams(int)const;	//119
	virtual int  GetNProps(void)const;	//121
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//126
	virtual char const *  GetPropName(int,int)const;	//134
	virtual int  GetPropVal(int,class CValue &)const;	//136
	virtual char const *  GetTypeString(void)const;	//152
	virtual int  HasRetVal(int)const;	//163
	void  Initialize(void);	//164
	virtual int  IsPropReadable(int)const;	//184
	virtual int  IsPropWritable(int)const;	//185
	static void  Register(void);	//196
	virtual int  SetPropVal(int,class CValue const &);	//233
	static void  UnRegister(void);	//247
};


class IMPORT_1C CRGdata
{
public:
	 CRGdata(void);	//4
	 ~CRGdata(void);	//17
	void  AddCondition(class CString &);	//41
	void  AddConditionName(class CString &);	//42
	void  AddDefaultOrederBy(class CRGgroup &);	//43
	void  AddFunction(char const *);	//44
	void  AddFunction(enum FunctionType);	//45
	int  AddFunctionPathItem(char const *,int &,int,int);	//46
	void  AddGroup(void);	//47
	void  AddGroupVar(int);	//48
	int  AddOrderPathItem(char const *,int &);	//50
protected:
	void  AddTable(class CRGTree<class CTableElement> * &,int,int,int,int &);	//54
public:
	int  AddVar(char const *);	//55
	void  AddVarPath(void);	//56
	int  AddVarPathItem(char const *,int &);	//57
	int  AdditionalVar(char const *);	//58
	int  CheckAdditionalVar(void);	//64
protected:
	void  ClearAll(void);	//65
public:
	int  DoesSQLEnabled(void)const;	//76
	int  DoesWithoutTotals(void)const;	//77
	int  FindTheSame(char const *,enum CRGbase::RGType);	//83
	class CRGLogicTree *  GetAndOrTree(void);	//87
	class CRGLogicTree * &  GetAndOrTreeRef(void);	//88
	int  GetCurGroup(void);	//97
	class CDate &  GetDateFrom(void);	//101
	class CDate &  GetDateTill(void);	//102
	long  GetDefPlanID(void)const;	//103
	char const *  GetFuncName(enum FunctionType,int);	//105
	int  GetGroupLevel(char const *);	//107
	int  GetNFuncs(void);	//116
	int  GetNGroups(void);	//117
	int  GetPeriod(void);	//127
	char const *  GetPredefinedVarName(enum VariableType,int);	//129
	enum ERGProcess   GetProcess(void);	//130
	enum ERGProcess   GetProcessDoc(void);	//131
	enum ERGProcess   GetProcessOper(void);	//132
	class CEventTime &  GetTimeFrom(void);	//147
	class CEventTime &  GetTimeTill(void);	//148
	class CType   GetVarType(char const *);	//156
	int &  GetWithoutTotalsRef(void);	//158
	class CObjID   Get_ObjectID(void);	//159
	int  IsCJPeriodEnabled(void);	//169
	int  IsDefProcess(void);	//172
	int  IsDefProcessDoc(void);	//173
	int  IsDefProcessOper(void);	//174
	int  IsDocDeclared(void);	//175
	int  IsDocProcessed(int,class CDocSet *);	//176
	int  IsDocStrDeclared(void);	//177
	int  IsNumeric(int);	//180
	int  IsOperProcessed(int);	//181
	int  IsProcessed(int);	//182
	int  IsRegStrDeclared(void);	//186
	int  RemoveAdditionalVar(int);	//198
	void  SecondPass(void);	//200
	void  SetCurrentGroupFlag(enum GroupFlags);	//204
	int  SetDateFrom(class CDate const &);	//205
	int  SetDateFrom(char const *);	//206
	int  SetDateTill(class CDate const &);	//207
	int  SetDateTill(char const *);	//208
	void  SetDefPlanID(long);	//209
	void  SetGroupName(char const *);	//214
	int  SetGroupType(class CRGgroup &,int,char const *);	//215
	void  SetOrderBy(class CRGgroup &);	//226
	void  SetPeriod(int);	//227
	void  SetProcess(enum ERGProcess);	//229
	void  SetProcessDoc(enum ERGProcess);	//230
	void  SetProcessOper(enum ERGProcess);	//231
	void  SetSQL(int);	//234
	void  SetTimeFrom(class CEventTime);	//236
	void  SetTimeTill(class CEventTime);	//237
	void  SetWithoutTotals(int);	//244
};


class IMPORT_1C CRGfunction
{
public:
	 CRGfunction(char const *);	//5
	 CRGfunction(enum FunctionType);	//6
	virtual  ~CRGfunction(void);	//18
	class CRGfunction const &  operator=(class CRGfunction const &);	//23
//	void  `default constructor closure'(void);	//37
	void  Condition(int);	//69
	void  EmptyCondition(void);	//80
	class CString &  GetArgExprStr(void);	//89
	class CRGMathTree *  GetArgTree(void);	//90
	class CRGMathTree * &  GetArgTreeRef(void);	//91
	char const *  GetCondition(void);	//92
	char const *  GetConditionName(void);	//93
	int  GetConditionNameNum(void);	//94
	class CRGLogicTree * &  GetConditionRef(void);	//95
	class CRGLogicTree *  GetConditionTree(void);	//96
	int  GetHead(void)const;	//109
	class CArray<int,int &> &  GetIntVarsRef(void);	//111
	class CString const &  GetName(void);	//123
	int  GetPrecision(class CRGdata *)const;	//128
	class CString &  GetSentence(class CString &,class CRGdata *)const;	//142
	enum FunctionType   GetType(void);	//149
	int  GetVar(void);	//154
	int  HasCondition(void);	//162
	int  IsCalculatedInternal(void);	//170
	int  IsConditionCalculatedInternal(void);	//171
	int  IsNameAuto(void);	//179
	void  SetCondition(class CString const &);	//201
	void  SetConditionName(class CString &);	//202
	void  SetConditionNameNum(int);	//203
	void  SetHead(int);	//217
	void  SetName(class CString const &);	//221
	void  SetNameAuto(int);	//224
	void  SetType(enum FunctionType);	//238
	void  SetVar(int);	//242
	virtual int  WhoAmI(void);	//249
};


class IMPORT_1C CRGgroup
{
public:
	 CRGgroup(void);	//7
	virtual  ~CRGgroup(void);	//19
	class CRGgroup const &  operator=(class CRGgroup const &);	//24
	void  ClearFlag(enum GroupFlags);	//66
	char const *  GetName(void);	//124
	class CString &  GetSentence(class CString &,class CRGdata *)const;	//143
	enum GroupType   GetType(void);	//150
	int  GetVar(void);	//155
	int  IsAll(void)const;	//167
	int  IsAllExists(void)const;	//168
	int  IsWithoutGroups(void)const;	//187
	int  IsWithoutOrder(void)const;	//188
	void  SetFlag(enum GroupFlags);	//212
	void  SetGroupType(int);	//216
	void  SetName(char const *);	//222
	void  SetType(enum GroupType);	//239
	void  SetVar(int);	//243
	virtual int  WhoAmI(void);	//250
};


class IMPORT_1C CRGpath
{
public:
	 CRGpath(class CRGpath const &);	//8
	 CRGpath(class CMetaDataObj *,enum VariableType);	//9
	class CRGpath const &  operator=(class CRGpath const &);	//25
	int  operator==(class CRGpath const &);	//28
	int  operator!=(class CRGpath const &);	//29
//	void  `default constructor closure'(void);	//38
	class CMetaDataObj *  GetMDO(void)const;	//112
	enum VariableType   GetVType(void)const;	//153
protected:
	void  SetMDO(class CMetaDataObj *);	//219
	void  SetVType(enum VariableType);	//241
};


class IMPORT_1C CRGprocessor
{
public:
	 CRGprocessor(char const *);	//10
	 ~CRGprocessor(void);	//20
//	void  `default constructor closure'(void);	//39
protected:
	int  AddSecondIndex(void);	//53
public:
	int  AssignRequest(char const *);	//59
	class CDate   BegOrEndDate(int);	//60
	void  Destructor(void);	//74
	long  DownloadToValuesTable(class CValue &,class CString const &,int);	//78
	long  DownloadToValuesTable(class CValue &,int,int);	//79
	int  Execute(void);	//81
	int  ExecuteRequest(void);	//82
	int  FunctionValue(class CValue &,class CValue * *);	//84
protected:
	int  GenerateDDName(class CString &);	//85
public:
	int  Get(class CValue * *);	//86
	class CRGdata *  GetData(void);	//98
	class CValue const &  GetFunctionValue(int,class CValue &)const;	//106
	int  GetGroupLevel(int,int);	//108
	class CRGrecord *  GetRecord(void);	//137
	class CRGset *  GetSet(void);	//145
	int  GotoPos(enum NavCtrl);	//160
	int  GroupValue(class CValue &,class CValue * *);	//161
	void  Initialize(char const *);	//165
	int  OrderValue(class CValue &,class CValue * *);	//192
	int  PrepareToExecute(void);	//195
	void  ResetRequest(void);	//199
	void  SetEnableSQL(int);	//211
	void  SetPlanID(long);	//228
	void  SetSel(long);	//235
	int  Translate(void (__cdecl*)(char const *,enum MessageMarker));	//246
};


class IMPORT_1C CRGvar
{
public:
	enum ERGVariableFlags {ergvfOne};
	 CRGvar(char const *);	//11
	virtual  ~CRGvar(void);	//21
	class CRGvar const &  operator=(class CRGvar const &);	//26
//	void  `default constructor closure'(void);	//40
	int  AddItemForAllPaths(char const *,int &);	//49
	void  AddPath(void);	//51
	void  ClearFlag(enum CRGvar::ERGVariableFlags);	//67
	void  ClearType(void);	//68
	int  GetDateDepCount(void);	//100
	class std::vector<long,class std::__alloc<1,0> >   GetDocRefID(void);	//104
	long  GetID(void);	//110
	int  GetMasterVar(void)const;	//114
	int  GetNTables(class CArray<class CTableElement,class CTableElement &> const &)const;	//122
	class CString const &  GetName(void);	//125
	int  GetRepresentationLength(int &);	//139
	class CString &  GetSentence(class CString &)const;	//144
	class CType &  GetType(void);	//151
	int  IsAdditionalVar(void)const;	//166
	int  IsGroupVar(void)const;	//178
	class CRGvarItem *  LastPath(void);	//189
	void  SetDocRefID(void);	//210
	void  SetFlag(enum CRGvar::ERGVariableFlags);	//213
	long  SetID(long);	//218
	void  SetMasterVar(int);	//220
	void  SetName(char const *);	//223
protected:
	enum RGerrors   SetNewTypeWithCheck(int,long);	//225
public:
	enum RGerrors   SetType(class CRGdata *);	//240
	virtual int  WhoAmI(void);	//251
};


class IMPORT_1C CRGvarItem
{
public:
	 CRGvarItem(class CRGvarItem const &);	//12
	 CRGvarItem(void);	//13
	virtual  ~CRGvarItem(void);	//22
	class CRGvarItem const &  operator=(class CRGvarItem const &);	//27
	int  AddPathItem(char const *,int &);	//52
private:
	int  CJGetData(int &,class CMetaDataObj *,char const *,int &,int);	//61
	int  DocumentGetData(int &,class CMetaDataObj *,char const *,int &,int);	//75
public:
	int  GetData(class CMetaDataObj *,char const *,int &,int);	//99
	class CMetaDataObj *  GetMDO(void)const;	//113
protected:
	int  GetReferenceData(class CMetaDataObj *,class CType &,char const *,int &);	//138
public:
	class CString   GetString(int);	//146
	enum VariableType   GetVariableType(void)const;	//157
private:
	int  OperGetData(int &,class CMetaDataObj *,char const *,int &,int);	//190
	int  OperSecondPathItem(char const *);	//191
	int  PlanGetData(int &,class CMetaDataObj *,char const *,int &,int);	//194
	int  RegisterGetData(int &,class CMetaDataObj *,char const *,int &,int);	//197
	int  SubcontoGetData(int &,class CMetaDataObj *,char const *,int &,int);	//245
protected:
	int  WhatIsIt(char const *);	//248
public:
	virtual int  WhoAmI(void);	//252
};

// End class descriptions
// Begin free functions definitions
IMPORT_1C int __cdecl PARSER(char const *,class CRGdata *,int &,int &);
// End free functions definitions

