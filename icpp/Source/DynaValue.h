// DynaValue.h: interface for the CDynaValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaValue_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_)
#define AFX_DynaValue_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define CDYNAVALUE_MAGIC	0x033540BD
#define CDYNAVALUE_MAGIC	0
#define CDYNAVALUE_NAME		"DynaValue"

struct propdefs {
	struct propdefs		*next;
	char				*Name;
	CValue				*val;
	unsigned long		flags;
};

class CDynaValue : public CBLContext
{
	DECLARE_DYNCREATE(CDynaValue);
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
	unsigned long	iflags;//internal flags
	int uMem;//used memory for this
	
	CDynaValue(int=0);
	virtual ~CDynaValue();
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

	static class CObjID			ObjID;
	static class CDMFeature*	m_fNode;
	virtual int				AddProp(const CString &, CValue *,unsigned int flags);
	virtual CValue*	FindPropVal(int iPropNum)const;
	virtual void			DeleteProp(int iPropNum);
	virtual int				SelectProps(void);
	virtual int				GetSelectedProp(CValue*,CValue*);
	virtual int				LoadFromString(const char*, int);
	virtual void			Reset(void);
	void					ClearAll(void);
	int						TotalByProps(void);
	int						FindByValue(CValue *val, CValue *rValue);
	int						FindBySubValue(const char *name, CValue *val, CValue *rValue);

	int						SaveToFile(CFile *file);
	int						LoadFromFile(CFile *file);

	void operator delete (void *p);
};

extern struct propdefs* findNpd(struct propdefs *pd, CString name);

//CDynaValue.flags
#define FLAG_PROPS_SELECTED	0x01000000
#define FLAG_VALS_SELECTED	0x10000000
//CDynaValue.iflags
#define IFLAG_SOLID		0x00000001
#define IFLAG_NAMELESS	0x00000002
#define IFLAG_RUNTIME	0x00000004
#endif // !defined(AFX_DynaValue_H__2DA9FA83_E854_11D3_A1F2_00A024AAD7AF__INCLUDED_)
