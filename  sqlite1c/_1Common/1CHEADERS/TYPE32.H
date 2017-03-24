// Begin forward declarations
class  CBufdFile;
class  CDate;
class  CGenericItem;
class  CItem;
class  CItemList;
class  CNumeric;
class  CPropBox;
class  CTerm;
class  CTermsBox;
// End forward declarations

// Begin class descriptions
class IMPORT_1C CBufdFile
{
public:
	 CBufdFile(unsigned int);	//1
	virtual  ~CBufdFile(void);	//16
	CString   Gets(void);	//90
	unsigned int  Gets(CString &);	//91
	unsigned int  Gets(char *,unsigned int);	//92
	virtual int  Open(char const *,unsigned int, CFileException *);	//105
	void  Puts(char const *);	//112
	virtual unsigned int  Read(void *,unsigned int);	//113
	int  SetBufSize(unsigned int);	//121
	virtual void  Write(void *,unsigned int);	//138
};


enum CDateFormat
{
	df_dd_MM_yyyy = 1,
	df_dd_MM_yy,
	df_d_MMMM_yyyy,
	df_d_MMMM_yyyy_,
	df_MMMM,
	df_MMMM_yyyy,
	df_MMMM_yy,
	df_q_Quarter_yyyy,
	df_q_Quarter_yy,
	df_yyyy,
	df_yyyyMMdd,
	df_yyyyq,
	df_q_Quarter,
};

class IMPORT_1C CDate
{
public:
	DWORD m_DateNum;  //00h
                      //size 04h

	 CDate(int,int,int);	//2
	CDate   AddMonth(int);	//37
	CDate   BegOfMonth(void)const;	//39
	CDate   BegOfQuart(void)const;	//40
	CDate   BegOfYear(void)const;	//41
	CDate   EndOfMonth(void)const;	//51
	CDate   EndOfQuart(void)const;	//52
	CDate   EndOfYear(void)const;	//53
	char const *  Format(enum CDateFormat,struct SDateFmtInfo const *)const;	//59
	static CDate   GetCurrentDate(void);	//62
	int  GetFormatted(enum CDateFormat,char const *);	//66
	int  GetFormatted(enum CDateFormat,char const *,int,int);	//67
	int  GetMonth(void)const;	//74
	int  GetMonthDay(void)const;	//75
	int  GetWeekDay(void)const;	//87
	int  GetYear(void)const;	//88
	int  GetYearDay(void)const;	//89
};


class IMPORT_1C CGenericItem
{
public:
	 CGenericItem(void);	//3
	virtual  ~CGenericItem(void);	//17
	
	virtual int  WhatIsIt(void)const;
	virtual char const *  GetFunctor(void)const;
	virtual int  LoadFromString(char const *,int *);
	virtual void  SaveToString(CString &,int,char const *)const;
	virtual CGenericItem *  Dup(void)const;

	static CGenericItem *  CreateFromString(char const *,int *);	//45
};


class IMPORT_1C CItem : public CGenericItem
{
public:
	CString m_strName;
	
	CItem(char const *);	//4
	virtual ~CItem(void);	//18

	virtual int  WhatIsIt(void)const;	//136
	virtual char const *  GetFunctor(void)const;	//69
	virtual int  LoadFromString(char const *,int *);	//98
	virtual void  SaveToString(CString &,int,char const *)const;	//119
	virtual CGenericItem *  Dup(void)const;	//49
};

//size of CItemList is 18h
class IMPORT_1C CItemList : public CGenericItem
{
public:
	CPtrArray m_PtrArray;

	CItemList(void);	//5
	virtual ~CItemList(void);	//19

	virtual int  WhatIsIt(void)const;	//137
	virtual char const *  GetFunctor(void)const;	//70
	virtual int  LoadFromString(char const *,int *);	//99
	virtual void  SaveToString( CString &,int,char const *)const;	//120
	virtual CGenericItem *  Dup(void)const;	//50

	int  AddItem(CGenericItem *);	//36
	int  FindGenericItem(char const *,int)const;	//54
	int  FindItem(char const *,int)const;	//55
	int  FindList(char const *,int)const;	//56
	CGenericItem *  GetItemAt(int)const;	//72
	int  GetNItems(void)const;	//76
	int  InsertItem(int, CGenericItem *);	//94
	void  RemoveAll(int);	//114
	void  RemoveItem(int,int);	//115
};


class IMPORT_1C CNumeric
{
public:
	int m_0;           //00h
	int m_nBufferLen;  //04h
	int m_nUsedLen;    //08h
	int m_nScaleLen;   //0Ch
	short m_Sign;      //10h
	int m_ScaleFactor; //14h
	UINT *m_pBuffer;   //18h
	UINT m_Buffer[4];  //1Ch
	                   //size 2Ch
	 CNumeric(CNumeric const &);	//6
	 CNumeric(int);	//7
	 CNumeric(long);	//8
	 CNumeric(double);	//9
	 CNumeric(long double);	//10
	 CNumeric(void);	//11
	 ~CNumeric(void);	//20
	CNumeric &  operator=(CNumeric const &);	//24
	CNumeric &  operator=(int);	//25
	CNumeric &  operator=(long);	//26
	CNumeric &  operator=(double);	//27
	CNumeric &  operator=(long double);	//28
	 operator long(void)const;	//29
	CNumeric   operator*(CNumeric const &)const;	//30
	CNumeric   operator-(CNumeric const &)const;	//31
	CNumeric   operator+(CNumeric const &)const;	//32
	CNumeric   operator/(CNumeric const &)const;	//33
	CNumeric   Abs(void)const;	//35
	int  Compare(CNumeric const &)const;	//42
	int  CompareLong(long)const;	//43
	char *  Convert(char *,int,int)const;	//44
	CNumeric   Floor(void)const;	//58
	CNumeric &  FromString(char const *,char * *);	//60
	long double  GetDouble(void)const;	//64
	static int  GetRoundMode(void);	//84
	CNumeric   Negate(void)const;	//104
	CNumeric   Round(int)const;	//117
	static int  SetRoundMode(int);	//128
	int  Sign(void)const;	//131
};


class IMPORT_1C CPropBox
{
public:
	 CPropBox(void);	//12
	virtual  ~CPropBox(void);	//21
protected:
	void  DestroyData(void);	//48
public:
	CNumeric const &  GetCNumericProp(int)const;	//61
	CDate   GetDateProp(int)const;	//63
	double  GetDoubleProp(int)const;	//65
	int  GetIntProp(int)const;	//71
	long  GetLongProp(int)const;	//73
	virtual int  GetNProps(void)const;	//77
	void *  GetPointerProp(int)const;	//78
	char const *  GetPropName(int)const;	//79
	int  GetPropNum(char const *,char const *)const;	//80
	enum EPropType   GetPropType(int)const;	//81
protected:
	unsigned int  GetPropValLen(int)const;	//82
public:
	virtual struct SProperty *  GetProperty(int)const;	//83
	char const *  GetSectName(int)const;	//85
	char const *  GetStringProp(int)const;	//86
protected:
	int  InitData(void);	//93
public:
	virtual int  IsPropWritable(int)const;	//97
	int  SetCNumericProp(int,CNumeric const &);	//122
	int  SetDateProp(int,CDate);	//123
	int  SetDoubleProp(int,double);	//124
	int  SetIntProp(int,int);	//125
	int  SetLongProp(int,long);	//126
	int  SetPointerProp(int,void *);	//127
	int  SetStringProp(int,char const *);	//129
};


class IMPORT_1C CTerm
{
public:
	 CTerm(char const *,char const *);	//13
	 ~CTerm(void);	//22
//	void  `default constructor closure'(void);	//34
};


class IMPORT_1C CTermsBox
{
public:
	char buf [512];
	 CTermsBox(int,int,unsigned int);	//14
	 CTermsBox(CTerm *,int,int,unsigned int);	//15
	 ~CTermsBox(void);	//23
	int  FindTerm(char const *);	//57
protected:
	virtual void  LoadTermString(int,int);	//100
public:
	void  SetTermString(int,int,char const *);	//130
	char const *  TermString(int,int);	//133
};
// End class descriptions
// Begin free functions definitions
CString  __cdecl AllTrim(CString const &);
void __cdecl DeleteNumSeparators(CString &);
void __cdecl DeleteNumSeparators(char *);
CString  __cdecl GetFullPath(char const *,char const *,char const *);
void __cdecl InsertNumSeparators(CString &);
void __cdecl InsertNumSeparators(char *,int);
CString  __cdecl Ltrim(CString const &);
CString & __cdecl MakeLower(CString &);
CString & __cdecl MakeUpper(CString &);
CString  __cdecl Pad(CNumeric const &,int,int,int);
CString  __cdecl Pad(int,int,int);
CString  __cdecl Pad(long,int,int);
CString  __cdecl Pad(long double,int,int,int);
CString  __cdecl Pad(char const *,int,int);
CString & __cdecl Pad(CString &,int,int);
CString  __cdecl ResString(unsigned int);
int __cdecl SEmpty(char const *);
CString  __cdecl Stretch(CString const &,int);
int __cdecl TranStr(CString &,CString const &,CString const &);
CString  __cdecl Trim(CString const &);
CString  __cdecl strbreak(char const *,int,int &);
char * __cdecl strbreak(char *,int);
// End free functions definitions
extern "C" int GetDefCCYBefore();
extern "C" int GetDefCurYear();
extern "C" int InitDateFormatting();
extern "C" int JDateToMasked();
extern "C" int JDateToString();
extern "C" int MakeJDate();
extern "C" int MaskedToJDate();
extern "C" int MaskedToJDate2();
extern "C" int SetDefCCYBefore();
extern "C" int SetDefCurYear();
extern "C" int StringToJDate();

