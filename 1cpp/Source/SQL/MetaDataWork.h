// MetaDataWork.h: interface for the CMetaDataWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATAWORK_H__111BA136_A805_11D3_9EEC_000001196471__INCLUDED_)
#define AFX_METADATAWORK_H__111BA136_A805_11D3_9EEC_000001196471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\\ODBC\\MetaNameParser.h"
#include "..\\blptr.h"
//#include "..\\Common\\dev_serv.h"

//<-CMetadataContext(36/4)<-CBLContext(32/28)<-CObject(4/4)
class CMetadataContext : public CBLContext
{
public:
	CTaskDef* m_pTaskDef;
	CMetadataContext(CTaskDef* pTaskDef):CBLContext(1),m_pTaskDef(pTaskDef)
	{
		*(DWORD*)this = GetVFTable();
		HashMethods(1);
		HashProperties(1);
	}

	static DWORD m_VFTableAddress;
	static DWORD GetVFTable()
	{
		if (!m_VFTableAddress)
		{
			CBLContext* pCont = CBLContext::GetLoadedContext(CBLContext::GetFirstLoadedContextID());
			int nProp = pCont->FindProp("Metadata");
			CValue value;
			pCont->GetPropVal(nProp, value);
			pCont = value.GetContext();
			m_VFTableAddress = *(DWORD*)pCont;
			//TraceBLContext(value.GetContext());
			//m_VFTableAddress = 0x2A0F3F58;
		}
		return m_VFTableAddress;
	};
};

class CMetaDataWork : public CBLContext  
{
public:

	enum {
	methGetConstID	= 0,
	methGetRefID,
	methGetRefFieldID,
	methGetRefTableName,
	methGetDocID,
	methGetDocHeadFieldID,
	methGetDocHeadTableName,
	methGetDocTblFieldID,
	methGetDocTblTableName,
	methGetCJID,
	methGetCJFieldID,
	methGetCJTableName,
	methGetRegID,
	methGetRegDimensionID,
	methGetRegResourceID,
	methGetRegAttributeID,
	methGetRegActsTableName,
	methGetRegTotalsTableName,
	methGetMetaDataID,
  methToDBString,
	methToDBStringLong,
	methFromDBString,
	methFromDBStringLong,
  methGetODBCConnectionHandle,
  methIntToBase,
	methBaseToInt,
  methGetDateTimeIDDOC,
  funcGetEndOfPeriod,
  funcGetEndOfPeriodForReg,
  funcGetBegOfPeriod,
  funcGetBegOfPeriodForReg,
  funcGetStringInVL,
  funcGetStrFromDate,
  funcGetAddPeriod,
  funcProcessMetaSQL,
  procSetTextParam,
	methToDBStringSuperLong,
    procAttachMD,
	
	methGetEnumAttributeID,

	lastMethod
	};
  
	DECLARE_DYNCREATE(CMetaDataWork);
	CMetaDataWork();
	CMetaDataWork(int);
	virtual ~CMetaDataWork();
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

  static bool SetTypeAndKind(CType &dest_type, const CString& source);
  enum TypeStringValue
  {
    ShortString,
    LongString,
    VeryLongString
  };
	static void Make1C_ValueFromString(CValue &res, const CType& type, const CString &source, TypeStringValue eType);

	//static CString ToDBStringLong(const CValue &val);
	//static CString ToDBString(const CValue& val);
  static CString GetStrFor1C_Date(const CDate &date);
    static CString GetObjDBString(const CValue & Value, const TypeStringValue enTypeStr);
    static BOOL LoadMD(CString strFileName, CMetaDataCont* &pMDC);
    static void UnloadMD(CMetaDataCont* &pMDC);
private:
	void Init();
  static CParamDefs defFnNames;
  CMetaNameParser m_MetaNameParser;
  CMetaDataCont* m_pMetaDataCont;

	static int nmethIsSelected;
	static _GUID EncryptGUIDArr[12];

	mutable CBLPtr<CMetadataContext> m_pMetadataContext;

};

#endif // !defined(AFX_METADATAWORK_H__111BA136_A805_11D3_9EEC_000001196471__INCLUDED_)
