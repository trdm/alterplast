// 1CPP_ODBCRecordSet.h: interface for the C1CPP_ODBCRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_1CPP_ODBCRECORDSET_H__44B9726A_7180_41BE_9324_38CB670B6CF6__INCLUDED_)
#define AFX_1CPP_ODBCRECORDSET_H__44B9726A_7180_41BE_9324_38CB670B6CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Odbccore.h"
#include "1CPP_ODBCDatabase.h"
#include "..\\SQL\\MetaDataWork.h"
#include "MetaNameParser.h"


class C1CPP_ODBCRecordSet : 
	public CODBCRecordset, 
	public CBLContext    
{
  DECLARE_DYNCREATE(C1CPP_ODBCRecordSet);
public:
	//bool SetValueOfParam(int nParam, const CValue &vSource, const int Modificator);
	//void SetValueOfRow(BYTE* Rowset, CDBField &dbf, int Index, CValue &rVal, bool bAgg =  false);
	//void SetValueOnDBField(CDBField &field, CValue &val, bool bAgg = false);

	C1CPP_ODBCRecordSet();
	virtual ~C1CPP_ODBCRecordSet();
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
protected:
    virtual BOOL TypifyField(int nIndex, CDBField& field);
    virtual CBLContext* GetVCtorParam();

private:
	void CheckSetDB(){if (!m_pDb) CBLModule::RaiseExtRuntimeError("База данных не установлена", mmRedErr);};

private:   
  static CParamDefs defFnNames;  
  CODBCDatabase m_db;
  static int n_nCountUnlimitPars;
  C1CPP_ODBCDatabase* m_pOtherDB;
  BOOL m_IsDebugMode;
  SQLUINTEGER m_nCurIndex;  
  static CMetaDataWork *m_pMetaData;
  CMetaNameParser m_MetaNameParser;
  CString GetField_1CType(CDBField & Field, int & Length, int & Scale);
  //void SpecialTypification(CDBField & field, const char * strObjID, CValue & val);
  //void CreateBLCValue(CDBField & field, CValue & val);
  BOOL m_bAutoTypifycation;
};

#endif // !defined(AFX_1CPP_ODBCRECORDSET_H__44B9726A_7180_41BE_9324_38CB670B6CF6__INCLUDED_)
