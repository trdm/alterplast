// DynaValueList.h: interface for the CDynaValueList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaValueList_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_)
#define AFX_DynaValueList_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DynaValue.h"

//#define CDYNAVALUELIST_MAGIC	0x4B384B3F
#define CDYNAVALUELIST_MAGIC	0
#define CDYNAVALUELIST_NAME		"DynaValueList"

#define PDFLAG_TOTAL	0x00000001
#define PDFLAG_TOTAL_P	0x00000002

class CDynaValueList : public CBLContext
{
	DECLARE_DYNCREATE(CDynaValueList);
public:


	static struct paramdefs {
		char * Names[2];
		int HasReturnValue;
		int NumberOfParams;
	}  defFnNames[];

	
	struct propdefs	*firstProp;
	struct propdefs	*lastProp;
	int				nProps;
	int				selected;

	unsigned long	flags;

	struct propdefs	*firstVal;
	struct propdefs	*lastVal;
	int				nVals;
	int				selectedVal;
	int	uMem;
	CValue	*lastUsedVal;

	long			*hash;

	CDynaValueList(int=0);
	virtual ~CDynaValueList();
	virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int	FindProp(char const *) const;	//1369
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
	virtual void  InitObject(CType const &);	//2721
	virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,CValue const &);	//3694

	static class CObjID ObjID;
	static class CDMFeature*	m_fNode;
	int						AddProp(const CString &, CValue *, unsigned int flags);
	int						AddValue(const CString &, const CString&, CValue *, int);
	CValue*			FindPropVal(int iPropNum)const;
	CValue*			FindDynaVal(int iPropNum)const;
	int						LoadFromString(const char*, int);
	void					Reset(void);
	void					DeleteValues();
	void					DeleteProps();
	int						SelectProps(void);
	int						GetSelectedProp(CValue*,CValue*);
	int						SelectValues(void);
	int						GetSelectedValue(CValue*,CValue*);
	int						BuildHash(const char*);
	int						FindByHash(CValue *, CValue *);
	void					ClearHash(void);
	int						TotalByProps(void);
	void					Clone(struct propdefs *pd);
	int						DeleteValueByName(const CString&);
	int						DeleteValueByNum(int);
	int						FindPropEx(char const *,int);
	int						FindByValue(const char *name, CValue* val, CValue *rValue);

	int						SaveToFile(CFile *file);
	int						LoadFromFile(CFile *file);

	void					DumpContent(const char *msg);
	void operator delete (void *p);

};

#define DYNAVALUE_PROPOGATE		0x00000001
#define DYNAVALUE_ACCUMULATE	0x00000002

#endif // !defined(AFX_DynaValueList_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_)
