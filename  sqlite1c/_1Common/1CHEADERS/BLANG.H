// Begin forward declarations

class  CBLModule;
class  CBLProcInfo;
class  CBLVarInfo;
class  CStdOleBLContext;
class  CTokensBox;

// End forward declarations
// Begin class descriptions


class CUnkBLangClass
{
	virtual void* funk00(unsigned int); //??_Gfilebuf@@UAEPAXI@Z; public: virtual void * __thiscall filebuf::`scalar deleting destructor'(unsigned int)
	virtual int funk01(void);
};

class CProcLabelInfo
{
public:
	CString m_Name;
	int m_Addr;

public:
	virtual ~CProcLabelInfo(void);
	virtual LPCSTR GetName(void);
};

class CProcVarInfo
{
public:
	CString m_Name;       //04h
	int m_ArraySize;      //08h
	BOOL m_IsExport;      //0Ch
	int m_Flags;          //10h  // &1==0 - Знач , &2==1 - in-параметр
	int m_ConstInfoIndex; //14h  //значение по-умолчанию

public:
	virtual ~CProcVarInfo(void);
	virtual LPCSTR GetName(void);
};

class CConstInfo
{
public:
	CTypedValue m_Value;

public:
	virtual ~CConstInfo(void);
	virtual void* GetUnkData(void);
};

class CProcInfo
{
public:
	CString m_Name;      //04h
	int m_Type;          //08h //0 - переменная; 1 - процедура; 2 - функция; 3 - тело модуля
	int m_BaseAddr;      //0Ch
	int m_NParams;       //10h
	CUnkBLangClass Unk1; //14h
	CTypedPtrArray<CPtrArray,CProcVarInfo*> m_VarArr;    //18h //переменные модуля
	CUnkBLangClass Unk2; //2Ch
	CTypedPtrArray<CPtrArray,CProcLabelInfo*> m_LabelArr;//30h //метки модуля
	void* m_Data1;       //3Ch
	BOOL m_IsExport;     //40h
	int m_Data2;         //44h
	int m_Data3;         //48h
	DWORD m_Data4;       //4Ch
	
	virtual ~CProcInfo(void);
	virtual LPCSTR GetName(void);
};

class CCompiledModule
{
public:
	CUnkBLangClass Unk1;    //00h
	CTypedPtrArray<CPtrArray, CConstInfo*> m_ConstArr; //04h //константы модуля
	CUnkBLangClass Unk2;    //18h
	CTypedPtrArray<CPtrArray, CProcInfo*> m_ProcArr;   //1Ch //процедуры модуля
	CDWordArray m_PCode;    //30h
	void* m_Data1;          //40h
	void* m_Data2;          //44h
};

class CCompiler;

class IMPORT_1C CBLModuleInternals
{
public:
	CBLModule* m_pModule;               //00h
	CBLContext* m_pContext;     //04h
	DWORD m_Data1;                      //08h
	CBLSpeller* m_pBLSpeller;           //0Ch
	DWORD m_Data2;                      //10h
	CString m_SourceStr;                //14h
	DWORD* m_pStartIDArray;             //18h
	DWORD* m_pEndIDArray1;              //1Ch
	DWORD* m_pEndIDArray2;              //20h
	CCompiler* m_pCompiler;             //24h
	CCompiledModule* m_pCompiledModule; //28h
	CBLModule** m_ppExecutedModule;     //2Ch
	char** m_ppData3;                   //30h
	                                    //size 34h
};


class IMPORT_1C CBLModule //VF Table OK
{
public:
	CBLModuleInternals* m_pIntInfo;

	CBLModule(class CBLModule const &);	//1
	CBLModule(class CBLContext *,char const *);	//2

	virtual					~CBLModule(void);	
	virtual int				GetKind(void)const;	
	virtual LPCSTR			GetSyntaxMark(void);	
	virtual int				OnSyntaxError(void);	//128
	virtual int				OnRuntimeError(void);	//124
	virtual int				OnStartExecution(void);	//126
	virtual int				OnNextLine(void);	//123
	virtual void			OnEnterProc(int);	//119
	virtual void			OnExitProc(int);	//121
	virtual void			OnStopExecution(void);	//127
	virtual void			OnErrorMessage(char const *);	//120
	virtual void			OnSetSpeller(class CValue const &);	//125
	virtual int				OnDoMessageBox(char const *,unsigned int,unsigned int);	//118
	virtual void			OnGetErrorDescription(class CString &);	//122

	static void  AddToKeywordColorList(char const *);	//21
	static void  AddToKeywordColorList(class CBLContext const *);	//22
	static int  ColorSourceLine(char const *,class CBLSyntaxColoring &);	//33
	static void  EnableDebugDump(int);	//36
	static class CBLModule *  GetExecutedModule(void);	//53
	static unsigned long  GetFirstLoadedModuleID(void);	//54
	static class CBLModule *  GetLoadedModule(unsigned long);	//59
	static unsigned long  GetNextLoadedModuleID(unsigned long);	//74
	static void  GetSyntaxErrDescr(int,class CString &);	//93
	static int  IsDebugDumpEnabled(void);	//106
	static int  IsLeftAdjFormat(char const *);	//111
	static void  RaiseExtRuntimeError(char const *,int);	//130
	static void  RemoveFromKeywordColorList(char const *);	//131
	static void  RemoveFromKeywordColorList(class CBLContext const *);	//132
	static void  SetOnCommandExceptionHandler(void (__cdecl*)(class CException *,int &));	//136
	static int  TokenizeSourceLine(char const *,int &,class CString *);	//139

	class CBLModule &  operator=(class CBLModule const &);	//11
//	void  `default constructor closure'(void);	//18
	void  AddSourceLine(char const *);	//20
	void  AssignCompiled(class CBLModule const &);	//23
	void  AssignContext(class CBLContext *);	//24
	void  AssignFriendModule(class CBLModule *);	//25
	void  AssignSource(class CBLModule const &);	//26
	void  AssignSource(char const *);	//27
	void  AssignSpeller(class CBLSpeller *);	//28
	int  CallAsFunc(int,class CValue &,int,class CValue * *);	//29
	int  CallAsProc(int,int,class CValue * *);	//31
	int  Compile(void);	//34
	int  EvalExpr(char const *,class CValue &,class CValue * *);	//37
	int  Execute(void);	//38
	int  ExecuteBatch(char const *,class CValue * *);	//39
	int  FindFunc(char const *)const;	//40
	int  FindProc(char const *,int)const;	//42
	int  FindStaticVar(char const *)const;	//44
	class CString   FormatValue(class CValue const &,char const *);	//45
	int  GetCallLevelProcInfo(int,class CBLProcInfo &,int &)const;	//46
	int  GetCallStackDepth(void)const;	//47
	int  GetCurSourceLine(int,class CString &)const;	//48
	class CBLModule const *  GetCurrentModule(void)const;	//49
	unsigned long  GetExecutedCmdAddr(void)const;	//51
	int  GetExecutedLineNum(void)const;	//52
	int  GetFirstSrcLineInfo(class CBLSrcLineInfo &)const;	//55
	class CBLModuleInternals *  GetInternalData(void)const;	//56
	unsigned long  GetLoadedID(void)const;	//58
	int  GetNCurSourceLines(void)const;	//63
	int  GetNProcs(void)const;	//67
	int  GetNSourceLines(void)const;	//69
	int  GetNStaticVars(void)const;	//70
	int  GetNextSrcLineInfo(class CBLSrcLineInfo &)const;	//75
	int  GetProcInfo(int,class CBLProcInfo &)const;	//76
	int  GetRuntimeErrCode(void)const;	//82
	void  GetRuntimeErrDescr(int,class CString &)const;	//83
	char const *  GetRuntimeErrIdent(void)const;	//84
	int  GetRuntimeErrLineNum(void)const;	//85
	class CBLModule *  GetRuntimeErrModule(void)const;	//86
	void  GetRuntimeErrSourceLine(class CString &)const;	//87
	int  GetSourceLine(int,class CString &)const;	//88
	class CBLSpeller *  GetSpeller(void)const;	//89
	int  GetStaticVarDescr(int,class CBLVarInfo &)const;	//90
	int  GetStaticVarValue(int,class CValue &,int)const;	//91
	int  GetSyntaxErrCode(void)const;	//92
	char const *  GetSyntaxErrIdent(void)const;	//94
	int  GetSyntaxErrLineNum(void)const;	//95
	char const *  GetSyntaxErrMarkedLine(void)const;	//96
	int  HasSource(void)const;	//104
	int  IsCompiled(void)const;	//105
	int  IsExecuted(void)const;	//107
	int  IsInBatchMode(void)const;	//110
	int  IsLoaded(void)const;	//112
	int  IsValidObject(void)const;	//115
	int  Load(void);	//116
	int  LoadSource(char const *);	//117
	int  PrepareToLoad(void);	//129
	void  Reset(void);	//133
	void  ResetCompiled(void);	//134
	void  ResetExecuted(void);	//135
	int  SetStaticVarValue(int,class CValue const &,int);	//138
	void  Unload(void);	//141
};

class IMPORT_1C CBLProcInfo
{
public:
	//char size_buffer[1000];
	CBLModule* m_Module;
	ULONG m_ProcID;
	DWORD m_Unknown;
	 CBLProcInfo(void);	//3
	class CBLProcInfo &  operator=(class CBLProcInfo const &);	//12
	class CBLModule *  GetModule(void)const;	//61
	int  GetNParams(void)const;	//65
	int  GetNVars(void)const;	//71
	char const *  GetName(void)const;	//72
	int  GetProcPos(void)const;	//77
	int  GetType(void)const;	//98
	int  GetVarDescr(int,class CBLVarInfo &)const;	//100
	int  HasRetVal(void)const;	//102
	int  IsExported(void)const;	//108
};


class IMPORT_1C CBLVarInfo
{
public:
	CBLModule* pMod;
	int nProcPos;
	int nVarPos;
	DWORD size_buffer[0xFF];
	 CBLVarInfo(void);	//4
	class CBLVarInfo &  operator=(class CBLVarInfo const &);	//13
	int  GetDim(void)const;	//50
	class CBLModule *  GetModule(void)const;	//62
	char const *  GetName(void)const;	//73
	int  GetProcPos(void)const;	//78
	int  GetVarPos(void)const;	//101
	int  IsExported(void)const;	//109
};


//<-CStdOleBLContext(30h/8h)<-COleGenericBLContext(28h/8h)<-CBLContext(20h/1Ch)<-CObject(4h/4h)
class IMPORT_1C CStdOleBLContext:public COleGenericBLContext
{
DECLARE_DYNCREATE(CStdOleBLContext)

public:
	CString m_ClassName;
	DWORD m_data2;

	 CStdOleBLContext(int);	//5
	virtual  ~CStdOleBLContext(void);	//9
//	void  `default constructor closure'(void);	//19
	virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//30
	virtual int  CallAsProc(int,class CValue * *);	//32
	virtual int  FindMethod(char const *)const;	//41
	virtual int  FindProp(char const *)const;	//43
	virtual char const *  GetMethodName(int,int)const;	//60
	virtual int  GetNMethods(void)const;	//64
	virtual int  GetNParams(int)const;	//66
	virtual int  GetNProps(void)const;	//68
	virtual char const *  GetPropName(int,int)const;	//79
	virtual int  GetPropVal(int,class CValue &)const;	//80
	virtual char const *  GetTypeString(void)const;	//99
	virtual int  HasRetVal(int)const;	//103
	virtual int  IsPropReadable(int)const;	//113
	virtual int  IsPropWritable(int)const;	//114
	virtual int  SetPropVal(int,class CValue const &);	//137
	int  TryFindFriendInterface(void);	//140
};


class IMPORT_1C CTokensBox: public CTermsBox
{
public:
	char buff[512];
	 CTokensBox(class CTokensBox const &);	//6
	 CTokensBox(void);	//7
	 ~CTokensBox(void);	//10
	class CTokensBox &  operator=(class CTokensBox const &);	//14
};

// End class descriptions
// Begin free functions definitions
// End free functions definitions


IMPORT_1C void __cdecl DispError2String(class CString &,long);
