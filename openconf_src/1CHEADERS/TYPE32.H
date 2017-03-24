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
class CBufdFile
{
public:
	 CBufdFile(unsigned int);	//1
	virtual  ~CBufdFile(void);	//16
	class CString   Gets(void);	//90
	unsigned int  Gets(class CString &);	//91
	unsigned int  Gets(char *,unsigned int);	//92
	virtual int  Open(char const *,unsigned int,class CFileException *);	//105
	void  Puts(char const *);	//112
	virtual unsigned int  Read(void *,unsigned int);	//113
	int  SetBufSize(unsigned int);	//121
	virtual void  Write(void *,unsigned int);	//138
};


class CDate
{
public:
	DWORD m_DateNum;
	 CDate(int,int,int);	//2
	class CDate   AddMonth(int);	//37
	class CDate   BegOfMonth(void)const;	//39
	class CDate   BegOfQuart(void)const;	//40
	class CDate   BegOfYear(void)const;	//41
	class CDate   EndOfMonth(void)const;	//51
	class CDate   EndOfQuart(void)const;	//52
	class CDate   EndOfYear(void)const;	//53
	char const *  Format(enum CDateFormat,struct SDateFmtInfo const *)const;	//59
	static class CDate   GetCurrentDate(void);	//62
	int  GetFormatted(enum CDateFormat,char const *);	//66
	int  GetFormatted(enum CDateFormat,char const *,int,int);	//67
	int  GetMonth(void)const;	//74
	int  GetMonthDay(void)const;	//75
	int  GetWeekDay(void)const;	//87
	int  GetYear(void)const;	//88
	int  GetYearDay(void)const;	//89
};


class CGenericItem
{
public:
	 CGenericItem(void);	//3
	virtual  ~CGenericItem(void);	//17
	static class CGenericItem *  CreateFromString(char const *,int *);	//45
};


class CItem
{
public:
	char buff[100];
	 CItem(char const *);	//4
	virtual  ~CItem(void);	//18
	virtual class CGenericItem *  Dup(void)const;	//49
	virtual char const *  GetFunctor(void)const;	//69
	virtual int  LoadFromString(char const *,int *);	//98
	virtual void  SaveToString(class CString &,int,char const *)const;	//119
	virtual int  WhatIsIt(void)const;	//136
};


class CItemList
{
public:
	int buf[0xFF];
	 CItemList(void);	//5
	virtual  ~CItemList(void);	//19
	int  AddItem(class CGenericItem *);	//36
	virtual class CGenericItem *  Dup(void)const;	//50
	int  FindGenericItem(char const *,int)const;	//54
	int  FindItem(char const *,int)const;	//55
	int  FindList(char const *,int)const;	//56
	virtual char const *  GetFunctor(void)const;	//70
	class CGenericItem *  GetItemAt(int)const;	//72
	int  GetNItems(void)const;	//76
	int  InsertItem(int,class CGenericItem *);	//94
	virtual int  LoadFromString(char const *,int *);	//99
	void  RemoveAll(int);	//114
	void  RemoveItem(int,int);	//115
	virtual void  SaveToString(class CString &,int,char const *)const;	//120
	virtual int  WhatIsIt(void)const;	//137
};


class CNumeric
{
public:
	 CNumeric(class CNumeric const &);	//6
	 CNumeric(int);	//7
	 CNumeric(long);	//8
	 CNumeric(double);	//9
	 CNumeric(long double);	//10
	 CNumeric(void);	//11
	 ~CNumeric(void);	//20
	class CNumeric &  operator=(class CNumeric const &);	//24
	class CNumeric &  operator=(int);	//25
	class CNumeric &  operator=(long);	//26
	class CNumeric &  operator=(double);	//27
	class CNumeric &  operator=(long double);	//28
	 operator long(void)const;	//29
	class CNumeric   operator*(class CNumeric const &)const;	//30
	class CNumeric   operator-(class CNumeric const &)const;	//31
	class CNumeric   operator+(class CNumeric const &)const;	//32
	class CNumeric   operator/(class CNumeric const &)const;	//33
	class CNumeric   Abs(void)const;	//35
	int  Compare(class CNumeric const &)const;	//42
	int  CompareLong(long)const;	//43
	char *  Convert(char *,int,int)const;	//44
	class CNumeric   Floor(void)const;	//58
	class CNumeric &  FromString(char const *,char * *);	//60
	long double  GetDouble(void)const;	//64
	static int  GetRoundMode(void);	//84
	class CNumeric   Negate(void)const;	//104
	class CNumeric   Round(int)const;	//117
	static int  SetRoundMode(int);	//128
	int  Sign(void)const;	//131
protected:
	int m_0;
	int m_nBufferLen;
	int m_nUsedLen;
	int m_nScaleLen;
	short m_Sign;
	int m_ScaleFactor;
	UINT *m_pBuffer;
	UINT m_Buffer[4];
};


class CPropBox
{
public:
	 CPropBox(void);	//12
	virtual  ~CPropBox(void);	//21
protected:
	void  DestroyData(void);	//48
public:
	class CNumeric const &  GetCNumericProp(int)const;	//61
	class CDate   GetDateProp(int)const;	//63
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
	int  SetCNumericProp(int,class CNumeric const &);	//122
	int  SetDateProp(int,class CDate);	//123
	int  SetDoubleProp(int,double);	//124
	int  SetIntProp(int,int);	//125
	int  SetLongProp(int,long);	//126
	int  SetPointerProp(int,void *);	//127
	int  SetStringProp(int,char const *);	//129
};


class CTerm
{
public:
	 CTerm(char const *,char const *);	//13
	 ~CTerm(void);	//22
//	void  `default constructor closure'(void);	//34
};


class CTermsBox
{
public:
	char buf [512];
	 CTermsBox(int,int,unsigned int);	//14
	 CTermsBox(class CTerm *,int,int,unsigned int);	//15
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
class CString  __cdecl AllTrim(class CString const &);
void __cdecl DeleteNumSeparators(class CString &);
void __cdecl DeleteNumSeparators(char *);
class CString  __cdecl GetFullPath(char const *,char const *,char const *);
void __cdecl InsertNumSeparators(class CString &);
void __cdecl InsertNumSeparators(char *,int);
class CString  __cdecl Ltrim(class CString const &);
class CString & __cdecl MakeLower(class CString &);
class CString & __cdecl MakeUpper(class CString &);
class CString  __cdecl Pad(class CNumeric const &,int,int,int);
class CString  __cdecl Pad(int,int,int);
class CString  __cdecl Pad(long,int,int);
class CString  __cdecl Pad(long double,int,int,int);
class CString  __cdecl Pad(char const *,int,int);
class CString & __cdecl Pad(class CString &,int,int);
class CString  __cdecl ResString(unsigned int);
int __cdecl SEmpty(char const *);
class CString  __cdecl Stretch(class CString const &,int);
int __cdecl TranStr(class CString &,class CString const &,class CString const &);
class CString  __cdecl Trim(class CString const &);
class CString  __cdecl strbreak(char const *,int,int &);
char * __cdecl strbreak(char *,int);
// End free functions definitions
extern "C" GetDefCCYBefore();
extern "C" GetDefCurYear();
extern "C" InitDateFormatting();
extern "C" JDateToMasked();
extern "C" JDateToString();
extern "C" MakeJDate();
extern "C" MaskedToJDate();
extern "C" MaskedToJDate2();
extern "C" SetDefCCYBefore();
extern "C" SetDefCurYear();
extern "C" StringToJDate();

