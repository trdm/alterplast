//////////////////////////////////////////////////////////////////////
// ValueCollection.h: interface for the CValueCollection class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

class CValueCollection : public CBLContext
{
public:
    CValueCollection(BOOL bIsWitable = FALSE);
    virtual ~CValueCollection();

//CBLContext
public:
    virtual char const* 	GetTypeString(void)const = 0;

    virtual int				GetNProps(void)const = 0;
	virtual int				FindProp(char const* szName)const = 0;
	virtual char const* 	GetPropName(int nPropIndex, int nAlias)const = 0;
	virtual int				GetPropVal(int nPropIndex, class CValue& Value)const = 0;
	virtual int				SetPropVal(int nPropIndex, class CValue const& Value) = 0;
	virtual int				IsPropReadable(int nPropIndex)const;
	virtual int				IsPropWritable(int nPropIndex)const;

	virtual int				GetNMethods(void)const;
	virtual int				FindMethod(char const* szName)const;
	virtual char const* 	GetMethodName(int nMethIndex, int nAlias)const;
	virtual int				GetNParams(int nMethIndex)const;
	virtual int				GetParamDefValue(int nMethIndex, int nParamIndex, class CValue* pDefValue)const;
	virtual int				HasRetVal(int nMethIndex)const;
	virtual int				CallAsProc(int nMethIndex, class CValue** ppValue);
	virtual int				CallAsFunc(int nMethIndex, class CValue& RetValue, class CValue** ppValue);
    BOOL m_bIsWitable;
    static CParamDefs m_ParamDefs;
};
