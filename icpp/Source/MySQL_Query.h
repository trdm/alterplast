// MySQL_Query.h: interface for the CMySQL_Query class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_QUERY_H__8DBFD417_784B_40F5_BDAA_16F6CDF7ABA1__INCLUDED_)
#define AFX_MYSQL_QUERY_H__8DBFD417_784B_40F5_BDAA_16F6CDF7ABA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include ".//ODBC//Odbccore.h"
//#include ".//SQL//ODBCRecordset.h"
class CMySQL_Query  : public CBLContext
{
  // Construction
DECLARE_DYNCREATE(CMySQL_Query);
public:
	CMySQL_Query();
	virtual ~CMySQL_Query();
  
  virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	virtual int  GetDestroyUnRefd(void)const;	//1657
	virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	//virtual void  InitObject(CType const &);	//2721
	//virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,CValue const &);	//3694

	static class CObjID ObjID;
private:
   
  static CParamDefs defFnNames;
  CODBCRecordset m_rs;
  //static CDatabase m_db;  
  //static CODBCDatabase m_db;
  CODBCDatabase m_db;

};

#endif // !defined(AFX_MYSQL_QUERY_H__8DBFD417_784B_40F5_BDAA_16F6CDF7ABA1__INCLUDED_)
