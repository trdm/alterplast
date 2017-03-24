//////////////////////////////////////////////////////////////////////
// SQLLock.h: interface for the CSQLLock class.
//
// Разработчик: Дмитрий Ощепков aka DmitrO, mailto: dmitro@ptlan.com
// Версия: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Odbccore.h"

#define MAX_CODE_LEN        24
#define MAX_DOC_NUM_LEN     20

#define REF_CODE_UNIQUE_ALL 1
#define REF_CODE_UNIQUE_PAR 2

#define EDIT_TYPE_NUM       2
#define EDIT_TYPE_STR       1

#define DOC_NUM_PER_ALL     0
#define DOC_NUM_PER_YEAR    1
#define DOC_NUM_PER_QUART   2
#define DOC_NUM_PER_MONTH   3
#define DOC_NUM_PER_DAY     4

#define DEFAULT_LOCK_TIMEOUT  5

class CSQLLock : public CBLContext, private CODBCDatabase, CODBCRecordset
{
DECLARE_DYNCREATE(CSQLLock);
public:
	CSQLLock();
	virtual ~CSQLLock();

    virtual int  CallAsFunc(int,class CValue &,class CValue * *);	//935
	virtual int  CallAsProc(int,class CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	virtual int  GetDestroyUnRefd(void)const;	//1657
	virtual void  GetExactValue(class CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,class CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,class CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual class CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	virtual void  InitObject(class CType const &);	//2721
	virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(class CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,class CValue const &);	//3694

	static class CObjID ObjID;
private:
    static CParamDefs defFnNames;
    int m_Type;
    union
    {
        CSbCntTypeDef* m_pSbCntTypeDef;
        CDocDef* m_pDocDef;
    };
    CObjID m_ObjID;
    CObjID m_ParentID;
    CObjID m_OwnerID;
    CString m_Prefix;
    CNumeric m_LastCode;
    CDate m_DateNum;
    CString FormatCode(int EditType, int Len);
    void NextCond(CString & str, bool &fCond);
    void PrepRS();
    int m_Timeout;
    int ProcessError();
};
