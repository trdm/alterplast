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

enum ShareModeCtl{sm1,sm2,sm3};
enum SafetyCtl{sc1,sc2,sc3};
enum NavCtrl{navFirst=1,navLast,navNext,navPrev};
enum LockCtrl{wr=1,rd};
enum TLockModeCtl{m0,m1,m2};
enum CompCtl{ccE, ccL, ccLE, ccGE, ccG};

//+ SIZEOF OK R27 0x08
class CDBEngDef
{
public:
	enum ObjectType { ot_none =0, ot_dd =1, ot_table =2, ot_index =3, ot_field =4 };
	LPCSTR szName;						// 0x04 ; "DDV7120" for CDataDictionary

	virtual ~CDBEngDef();
	virtual ObjectType GetObjType() const = 0;	// name approved
};

class IMPORT_1C CDataDictionary : public CDBEngDef
{
	typedef int (*F_I_II)(int, int);
public:
	DWORD dw_08;
	DWORD dw_0C;
	DWORD dw_10;
	CPtrArray Tables;							// 0x14 ; <CTable>
	DWORD dw_28;								// 0x28 ; def 5
	F_I_II pf_2C;
	F_I_II pf_30;
	DWORD* p_34;
	DWORD dw_38;
	LPCSTR szAdmFilePath;
	LPCSTR szSysFilePath;
	LPCSTR szUserFilePath;
	DWORD dw_48;
	DWORD dw_4C;
	DWORD dw_50;
	CRITICAL_SECTION crs_54;					// 0x54
	//////////////////////////////////////////////////////////////////////////
	virtual  ~CDataDictionary(void);
	virtual enum ObjectType GetObjType(void) const { return ot_dd; };
	//////////////////////////////////////////////////////////////////////////
	CDataDictionary(char const *,char const *,char const *,char const *);	//1
	class CTable *  AddTable(char const *);	//13
	int  BeginTransaction(void);	//15
	void  Close(void);	//17
	int  DeleteTable(int);	//27
	int  DeleteTable(class CTable *);	//28
	int  DeleteTable(char const *);	//29
	int  EndTransaction(enum TransactionCtl);	//32
	char const *  GetFilePath(enum TableType)const;	//65
	class CTable *  GetTable(int);	//78
	class CTable *  GetTable(char const *);	//79
	int  Open(char const *);	//88
	int  OpenEmpty(void);	//90
	int  SetAdmFilePath(char const *);	//95
	static void  SetLocale(int,unsigned char *,unsigned char *);	//97
	int  SetSysFilePath(char const *);	//102
	int  SetUserFilePath(char const *);	//103
};

//+ SIZEOF R27 0x70
//+ RECONSTRUCTED, NAME GUESSED
class CDBFDataDictionary : public CDataDictionary
{
public:
	DWORD dw_Version;					// 0x6C ; def 0 ; usually 7120 (DD file version field)
	//////////////////////////////////////////////////////////////////////////
	virtual  ~CDBFDataDictionary(void);
	virtual enum ObjectType GetObjType(void) const { return ot_dd; };
};

class CField : public CDBEngDef
{
public:
	CDBFDataDictionary* pDD;			// 0x08
	long TypeCField;
	int sizeCField;
	int precCField;
	DWORD dw_18;
//////////////////////////////////////////////////////////////////////////
	virtual ~CField();
	virtual enum ObjectType GetObjType(void) const { return ot_field; };
};

class IMPORT_1C CKeyObj
{
public:
	CIndex* m_pIndex;
	CStoreObj* m_pStoreObj;
	long databuf[2];

	CKeyObj(class CIndex *,int,int);	//2
	virtual  ~CKeyObj(void);	//7
	virtual enum ObjectType   GetObjType(void)const;	//71
	virtual void PrepareKey()=0;
	void  SetKeyLength(int,int);	//96
};

class IMPORT_1C CRecAddr
{
public:
	long m_pos;	//0x4
	long data;	//0x8

	CRecAddr(class CRecAddr const &);	//3
	CRecAddr(class CRecAddr const *);	//4
	virtual  ~CRecAddr(void);	//8
	virtual enum ObjectType   GetObjType(void)const;	//72
	int  operator==(class CRecAddr const &)const;	//10
	long  GetLongVal(void)const;	//68
	void  SetLongVal(long);	//98
};


class IMPORT_1C CStoreObj : public CRecAddr
{
public:
	CTable *pTable;//0xC
	CIndex *pIndex;
	char bufer[28];

	 CStoreObj(class CTable *,class CIndex *);	//5
	virtual  ~CStoreObj(void);	//9
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
protected:
	virtual void  FieldExchange(int);	//60
public:
	virtual enum ObjectType   GetObjType(void)const;	//73
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

//+ SIZEOF OK R27 0x20
struct stIndex_descr
{
	LPCSTR szName;
	LPCSTR szIdxExpr;
	LPCSTR szDeleted;
	DWORD dw_0C;
	DWORD dw_10;
	DWORD dw_14;
	DWORD dw_18;
	DWORD dw_1C;
};

//+ WORK NAME CDBEngDef3 // I=
//+ SIZEOF OK R27 0x28
//+ PARENT OK
//+ VTBL OK
//+ EXPORTS OK
class IMPORT_1C CIndex : public CDBEngDef
{
public:
	CDBFDataDictionary* pDD;			// 0x08
	DWORD dw_0C;
	stIndex_descr* p_10;				// 0x10
	CTypedPtrArray<CPtrArray, CField*> Fields;					// 0x14
//////////////////////////////////////////////////////////////////////////
	virtual ~CIndex();
	virtual enum ObjectType GetObjType(void) const { return ot_index; };
//////////////////////////////////////////////////////////////////////////
	class CField *  GetField(int);	//62
};

class CIndexEx : public CIndex
{
public:
	DWORD fieldsCount() const { return Fields.GetSize(); }
	CField* field(DWORD idx) const
	{
		return Fields[idx];
	}
};

//+ WORK NAME CDBEngDef2 // T=
//+ SIZEOF OK R27 0x6C
//+ PARENT OK
//+ VTBL OK
//+ EXPORTS OK

struct real_field
{
	char buf[0x18];
	// Смещение в буфере записи указано от начала реальной записи, в которой
	// еще в начале есть один символ - пометка удаления записи в дбф
	// то есть смещение от адреса, возвращаемого CTableEx::recordBuffer будет
	// на 1 меньше.
	DWORD offsetInRec;
};

struct real_fields
{
	char buf[0xC];
	real_field** fields;
};

class IMPORT_1C CTable : public CDBEngDef
{
public:
	CDBFDataDictionary* pDD;			// 0x08
	DWORD dw_0C;
	DWORD dw_10;
	DWORD dw_14;
	DWORD dw_18;
	real_fields* p_1C;
	DWORD dw_20;
	DWORD dw_24;
	DWORD dw_28;
	DWORD dw_2C;
	CTypedPtrArray<CPtrArray, CField*> Fields;					// 0x30 ; <CDBEngDef4> == <CField>
	CTypedPtrArray<CPtrArray, CIndexEx*> Indices;					// 0x44 ; <CDBEngDef3> == <CIndex>
	CTypedPtrArray<CPtrArray, CStoreObj*> StoreObjs;				// 0x58 ; <CDBEngDef6> == <CStoreObj>
//////////////////////////////////////////////////////////////////////////
	virtual ~CTable();
	virtual enum ObjectType GetObjType() const { return ot_table; };
//////////////////////////////////////////////////////////////////////////
	CField* AddField(char const *);
	CIndex* AddIndex(char const *);
	int BeginReadSequence(void);
	int Clear(void);
	int CloseAccess(enum SafetyCtl);
	int DeleteField(int);
	int DeleteField(class CField *);
	int DeleteField(char const *);
	int DeleteIndex(int);
	int DeleteIndex(class CIndex *);
	int DeleteIndex(char const *);
	int EnableIndexing(int);
	void EndReadSequence(void);
	class CField * GetField(int);
	class CField * GetField(char const *);
	class CIndex * GetIndex(int);
	class CIndex * GetIndex(char const *);
	enum TLockModeCtl  GetTLockMode(void)const ;
	int IsBufDirty(void)const ;
	int IsOpen(void)const ;
	int IsRecLocked(class CRecAddr &)const ;
	int OpenAccess(enum ShareModeCtl,enum SafetyCtl);
	int Pack(void);
	int PutTLock(enum TLockModeCtl);
	int ReIndex(void);
};

class CTableEx : public CTable
{
public:
	DWORD fieldsCount() const	{return Fields.GetSize();}
	DWORD indexesCount() const	{return Indices.GetSize();}
	CField* field(DWORD idx) const
	{
		return Fields[idx];
	}
	CIndexEx* index(DWORD idx) const
	{
		return Indices[idx];
	}
	char* recordBuffer() const {return *(char**)((*(char**)((**(char***)(((char*)p_1C) + 0xC)) + 0x1C)) + 0x14) + 1;}
};


class IMPORT_1C CDataBaseStaticObj
{
public:
	char const *  GetDescription(void)const;	//61
};
