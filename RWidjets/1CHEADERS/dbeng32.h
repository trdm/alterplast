// Begin forward declarations
class  CDataDictionary;
class  CKeyObj;
class  CRecAddr;
class  CStoreObj;
class  CTable;
class  CDataBaseStaticObj;
class  CIndex;
// End forward declarations
// Begin class descriptions

class IMPORT_1C CDataDictionary
{
public:
	 CDataDictionary(char const *,char const *,char const *,char const *);	//1
	virtual  ~CDataDictionary(void);	//6
	class CTable *  AddTable(char const *);	//13
	int  BeginTransaction(void);	//15
	void  Close(void);	//17
	int  DeleteTable(int);	//27
	int  DeleteTable(class CTable *);	//28
	int  DeleteTable(char const *);	//29
	int  EndTransaction(enum TransactionCtl);	//32
	char const *  GetFilePath(enum TableType)const;	//65
	virtual enum ObjectType   GetObjType(void)const;	//69
	class CTable *  GetTable(int);	//78
	class CTable *  GetTable(char const *);	//79
	int  Open(char const *);	//88
	int  OpenEmpty(void);	//90
	int  SetAdmFilePath(char const *);	//95
	static void  SetLocale(int,unsigned char *,unsigned char *);	//97
	int  SetSysFilePath(char const *);	//102
	int  SetUserFilePath(char const *);	//103
};


class IMPORT_1C CKeyObj
{
public:
	 CKeyObj(class CIndex *,int,int);	//2
	virtual  ~CKeyObj(void);	//7
	virtual enum ObjectType   GetObjType(void)const;	//71
	void  SetKeyLength(int,int);	//96
};


class IMPORT_1C CRecAddr
{
public:
	DWORD m_dwLongVal;
	DWORD m_Flag;

	 CRecAddr(class CRecAddr const &);	//3
	 CRecAddr(class CRecAddr const *);	//4
	virtual  ~CRecAddr(void);	//8
	virtual enum ObjectType   GetObjType(void)const;	//72

	int  operator==(class CRecAddr const &)const;	//10
	long  GetLongVal(void)const;	//68
	void  SetLongVal(long);	//98
};

class IMPORT_1C CStoreObj: public CRecAddr
{
public:

	DWORD m_dwFlag1;
	DWORD m_dwFlag2;
	DWORD m_dwFlag3;
	DWORD m_dwFlag4;
	DWORD m_dwFlag5;
	DWORD m_dwFlag6;
	DWORD m_dwFlag7;
	//DWORD m_dwFlag8;
	//CDBSign m_DBSign;

	 CStoreObj(class CTable *,class CIndex *);	//5
	virtual  ~CStoreObj(void);	//9
	virtual enum ObjectType   GetObjType(void)const;	//73
protected:
	virtual void  FieldExchange(int);	//60
public:

	int  Compare(class CKeyObj *,enum CompCtl);	//19
	int  Delete(void);	//20
	void  FX_CNumeric(int,class CNumeric &,int);	//33
	void  FX_CNumeric(class CField *,class CNumeric &,int);	//34
	void  FX_CNumeric(char const *,class CNumeric &,int);	//35
	void  FX_Char(int,char &,int);	//36
	void  FX_Char(class CField *,char &,int);	//37
	void  FX_Char(char const *,char &,int);	//38
	void  FX_Date(int,class CDate &,int);	//39
	void  FX_Date(class CField *,class CDate &,int);	//40
	void  FX_Date(char const *,class CDate &,int);	//41
	void  FX_Double(int,double &,int);	//42
	void  FX_Double(class CField *,double &,int);	//43
	void  FX_Double(char const *,double &,int);	//44
	void  FX_Int(int,int &,int);	//45
	void  FX_Int(class CField *,int &,int);	//46
	void  FX_Int(char const *,int &,int);	//47
	void  FX_Long(int,long &,int);	//48
	void  FX_Long(class CField *,long &,int);	//49
	void  FX_Long(char const *,long &,int);	//50
	void  FX_LongDouble(int,long double &,int);	//51
	void  FX_LongDouble(class CField *,long double &,int);	//52
	void  FX_LongDouble(char const *,long double &,int);	//53
	void  FX_String(int,class CString &,int);	//54
	void  FX_String(int,char *,long,int);	//55
	void  FX_String(class CField *,class CString &,int);	//56
	void  FX_String(class CField *,char *,long,int);	//57
	void  FX_String(char const *,class CString &,int);	//58
	void  FX_String(char const *,char *,long,int);	//59
	class CRecAddr   GetRecAddr(void)const;	//75
	double  GetRelPosition(void);	//76
	int  Goto(class CRecAddr const &,int);	//80
	int  Goto(class CKeyObj *,enum CompCtl,int);	//81
	int  Goto(enum NavCtrl,int);	//82
	int  Locking(enum LockCtrl);	//86
	int  New(void);	//87
	int  ReRead(void);	//94
	void  SetOrderIndex(class CIndex *);	//99
	int  SetRelPosition(double,int);	//100
	int  SetSourceTable(class CTable *);	//101
	int  Update(void);	//104
};

class CStoreObj_Ref: public CStoreObj
{
public:
	CObjID m_ObjID;
	DWORD m_dwFlag11;
	DWORD m_dwFlag12;
	DWORD m_dwFlag13;
	DWORD m_dwFlag14;
	DWORD m_dwFlag15;
	DWORD m_dwFlag16;
	DWORD m_dwFlag17;
	DWORD m_dwFlag18;
	DWORD m_dwFlag19;
	DWORD m_dwFlag20;
	CObjID m_ObjID_SQL;
};

class CStoreObj_Doc: public CStoreObj
{
public:
	DWORD m_dwFlag8;
	DWORD m_dwFlag9;
	DWORD m_dwFlag10;
	CObjID m_ObjID;
	DWORD m_dwFlag11;
	DWORD m_dwFlag12;
	DWORD m_dwFlag13;
	DWORD m_dwFlag14;
	DWORD m_dwFlag15;
	DWORD m_dwFlag16;
	CObjID m_ObjID_SQL;
};

class CStoreObj_Calendar: public CStoreObj
{
public:
	CObjID m_ObjID;
};

class CStoreObj_Acc: public CStoreObj
{
public:
	DWORD m_dwFlag8;
	CObjID m_ObjID;
	DWORD m_dwFlag11;
	DWORD m_dwFlag12;
	DWORD m_dwFlag13;
	DWORD m_dwFlag14;
	DWORD m_dwFlag15;
	DWORD m_dwFlag16;
	DWORD m_dwFlag17;
	DWORD m_dwFlag18;
	DWORD m_dwFlag19;
	DWORD m_dwFlag20;
	DWORD m_dwFlag21;
	CObjID m_ObjID_SQL;
};

class CStoreObj_CJ: public CStoreObj
{
public:
	DWORD m_dwFlag8;
	CObjID m_ObjID;
	DWORD m_dwFlag11;
	DWORD m_dwFlag12;
	DWORD m_dwFlag13;
	DWORD m_dwFlag14;
	DWORD m_dwFlag15;
	DWORD m_dwFlag16;
	DWORD m_dwFlag17;
	DWORD m_dwFlag18;
	DWORD m_dwFlag19;
	CObjID m_ObjID_SQL;
};

class IMPORT_1C CTable
{
public:
	class CField *  AddField(char const *);	//11
	class CIndex *  AddIndex(char const *);	//12
	int  BeginReadSequence(void);	//14
	int  Clear(void);	//16
	int  CloseAccess(enum SafetyCtl);	//18
	int  DeleteField(int);	//21
	int  DeleteField(class CField *);	//22
	int  DeleteField(char const *);	//23
	int  DeleteIndex(int);	//24
	int  DeleteIndex(class CIndex *);	//25
	int  DeleteIndex(char const *);	//26
	int  EnableIndexing(int);	//30
	void  EndReadSequence(void);	//31
	class CField *  GetField(int);	//63
	class CField *  GetField(char const *);	//64
	class CIndex *  GetIndex(int);	//66
	class CIndex *  GetIndex(char const *);	//67
	virtual enum ObjectType   GetObjType(void)const;	//74
	enum TLockModeCtl   GetTLockMode(void)const;	//77
	int  IsBufDirty(void)const;	//83
	int  IsOpen(void)const;	//84
	int  IsRecLocked(class CRecAddr &)const;	//85
	int  OpenAccess(enum ShareModeCtl,enum SafetyCtl);	//89
	int  Pack(void);	//91
	int  PutTLock(enum TLockModeCtl);	//92
	int  ReIndex(void);	//93
};


class IMPORT_1C CDataBaseStaticObj
{
public:
	char const *  GetDescription(void)const;	//61
};


class IMPORT_1C CIndex
{
public:
	class CField *  GetField(int);	//62
	virtual enum ObjectType   GetObjType(void)const;	//70
};

// End class descriptions

// Begin free functions definitions
// End free functions definitions
