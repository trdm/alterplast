//#include "stdafx.h"
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAddInContext_MyProxy : public CBLContext
{
DECLARE_DYNCREATE(CAddInContext_MyProxy)
protected:
	CBLPtr<CAddInContext> m_BaseCont;
	CString m_TypeString;

public:

	static CAddInContext_MyProxy* CreateWithNewObject(LPCTSTR typeString, CAddInContext* pCont);

public:
	CAddInContext_MyProxy(int iParam =NULL);
	virtual ~CAddInContext_MyProxy();
public:
	virtual void			IncrRef(void);
	virtual void			DecrRef(void);

	virtual int				GetDestroyUnRefd(void)const; // return m_FlagAutoDestroy; // в этом классе

	virtual int				IsOleContext(void)const;

	virtual CType			GetValueType(void)const;	//2547
	virtual long			GetTypeID(void)const;	//2505
	virtual CObjID			GetID(void)const;	//1804
	virtual char const *	GetCode(void)const;	//1544

	virtual int				IsExactValue(void)const;	//2808

	virtual void			InitObject(char const *);	//2722
	virtual void			InitObject(class CType const &);	//2721

	virtual void			SelectByID(CObjID,long);	//3350

	virtual char const *	GetTypeString(void)const;	//2513

	virtual int				GetNProps(void)const;	//2015
	virtual int				FindProp(char const *)const;	//1369
	virtual char const *	GetPropName(int,int)const;	//2187
	virtual int				GetPropVal(int,CValue &)const;	//2221
	virtual int				SetPropVal(int,CValue const &);	//3694
	virtual int				IsPropReadable(int)const;	//2863
	virtual int				IsPropWritable(int)const;	//2865

	virtual int				GetNMethods(void)const;	//2005
	virtual int				FindMethod(char const *)const;	//1366
	virtual char const *	GetMethodName(int,int)const;	//1964
	virtual int				GetNParams(int)const;	//2008
	virtual int				GetParamDefValue(int,int,CValue *)const;	//2122
	virtual int				HasRetVal(int)const;	//2657

	virtual int				CallAsProc(int,CValue * *);	//937
	virtual int				CallAsFunc(int,CValue &,CValue * *);	//935

	virtual int				IsSerializable(void);	//2874

	virtual int				SaveToString(CString &);	//3295
	virtual class			CBLContextInternalData *  GetInternalData(void);	//1826
	virtual void			GetExactValue(CValue &);	//1708

	static class CObjID ObjID;

};
