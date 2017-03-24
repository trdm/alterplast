#ifndef SIMPLE_CONTEXT_H_INCLUDED
#define SIMPLE_CONTEXT_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CSimpleDef class (c) kms 2008 - 2012
// CSimpleContext class (c) kms 2008 - 2012
//////////////////////////////////////////////////////////////////////////

/*
CSimpleContext must be first entry in the list of base classes
*/

#include <string>
#include <boost/shared_ptr.hpp>

class CValue;

#define I_PROC(name) BOOL name(CValue** ppValues)
#define I_FUNC(name) BOOL name(CValue& RetValue, CValue** ppValues)
#define I_PDEF(name) BOOL name(int nIndex, CValue* pValue) const
#define I_GET(name) BOOL name(CValue& RetValue) const
#define I_SET(name) BOOL name(CValue const& Value)

#define V_PROC(name) virtual I_PROC(name)
#define V_FUNC(name) virtual I_FUNC(name)
#define V_PDEF(name) virtual I_PDEF(name)
#define V_GET(name) virtual I_GET(name)
#define V_SET(name) virtual I_SET(name)

#define SIMPLE_DECLARE(this_t, base_t) \
private: \
	typedef this_t THIS_TYPE; \
	typedef base_t BASE_TYPE; \
public: \
	DECLARE_DYNCREATE(this_t) \
	THIS_TYPE(LPCSTR szName = IsNULL(m_szTypeStringRus, m_szTypeStringEng)) \
	: BASE_TYPE(&m_StaticDef, m_bStaticDefValid, szName) \
{ \
	if (!m_bStaticDefValid) \
{ \
	InitDef(); \
	m_bStaticDefValid = TRUE; \
} \
} \
	THIS_TYPE(CSimpleDef* pDef, BOOL bDefValid, LPCSTR szName) \
	: BASE_TYPE(pDef, bDefValid, szName) \
{ \
	if (!bDefValid) \
	InitDef(); \
} \
private: \
	void InitDef(); \
private: \
	static LPCSTR m_szTypeStringEng; \
	static LPCSTR m_szTypeStringRus; \
	static CSimpleDef m_StaticDef; \
	static BOOL m_bStaticDefValid; \
	static CSimpleRegistrar m_Reg; \

#define SIMPLE_IMPLEMENT(type_t, base_t, typestring_eng, typestring_rus) \
	CSimpleDef type_t::m_StaticDef; \
	BOOL type_t::m_bStaticDefValid = FALSE; \
	LPCSTR type_t::m_szTypeStringEng = typestring_eng; \
	LPCSTR type_t::m_szTypeStringRus = typestring_rus; \
	CSimpleRegistrar type_t::m_Reg(RUNTIME_CLASS(type_t), type_t::m_szTypeStringEng, type_t::m_szTypeStringRus); \
	IMPLEMENT_DYNCREATE(type_t, base_t)

//////////////////////////////////////////////////////////////////////////
// CSimpleDef class
//////////////////////////////////////////////////////////////////////////
class CSimpleDef
{
public:
	typedef BOOL (CBLContext::*PF_Proc)(CValue**);
	typedef BOOL (CBLContext::*PF_Func)(CValue&, CValue**);
	typedef BOOL (CBLContext::*PF_ParamDef)(int, CValue*) const;
	typedef int  (CBLContext::*PF_Get)(CValue&) const;
	typedef int  (CBLContext::*PF_Set)(CValue const&);

	class s_meth_def
	{
	public:
		s_meth_def(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams, PF_Proc pProc, PF_Func pFunc, PF_ParamDef PF_ParamDef)
			: m_szNameEng(new char[strlen(szNameEng) + 1]),
			m_szNameRus(new char[strlen(szNameRus) + 1]),
			m_nParams(nParams), m_pProc(pProc), m_pFunc(pFunc), m_pParamDef(PF_ParamDef)
		{
			strcpy(const_cast<LPSTR>(m_szNameEng.get()), szNameEng);
			strcpy(const_cast<LPSTR>(m_szNameRus.get()), szNameRus);
		}

		~s_meth_def()
		{}

	private:
		s_meth_def(s_meth_def const&);
		s_meth_def& operator=(s_meth_def const&);

	public:
		boost::shared_ptr<const char> m_szNameEng;
		boost::shared_ptr<const char> m_szNameRus;
		size_t m_nParams;
		PF_Proc m_pProc;
		PF_Func m_pFunc;
		PF_ParamDef m_pParamDef;
	};

	typedef boost::shared_ptr<s_meth_def> s_meth_def_ptr;

	class s_prop_def
	{
	public:
		s_prop_def(LPCSTR szNameEng, LPCSTR szNameRus, PF_Get pGet, PF_Set pSet)
			: m_szNameEng(new char[strlen(szNameEng) + 1]),
			m_szNameRus(new char[strlen(szNameRus) + 1]),
			m_pGet(pGet), m_pSet(pSet)
		{
			strcpy(const_cast<LPSTR>(m_szNameEng.get()), szNameEng);
			strcpy(const_cast<LPSTR>(m_szNameRus.get()), szNameRus);
		}

		~s_prop_def()
		{}

	private:
		s_prop_def(s_prop_def const&);
		s_prop_def& operator=(s_prop_def const&);

	public:
		boost::shared_ptr<const char> m_szNameEng;
		boost::shared_ptr<const char> m_szNameRus;
		size_t m_nParams;
		PF_Get m_pGet;
		PF_Set m_pSet;
	};

	typedef boost::shared_ptr<s_prop_def> s_prop_def_ptr;

	CSimpleDef()
		: m_pMMap(new M_DEF(true)),
		m_pPMap(new M_DEF(true))
	{}

	~CSimpleDef()
	{}

	//////////////////////////////////////////////////////////////////////////
	// CSimpleDef interface
	//////////////////////////////////////////////////////////////////////////
	size_t AddMeth(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams, PF_Proc pProc, PF_Func pFunc, PF_ParamDef pParamDef)
	{
		size_t nIndex;
		if (!m_pMMap->Lookup(szNameEng, nIndex))
		{
			V_METH_DEF::value_type pDef(new s_meth_def(szNameEng, szNameRus, nParams, pProc, pFunc, pParamDef));
			nIndex = m_vMDefs.size();
			m_vMDefs.push_back(pDef);
			m_pMMap->SetAt(pDef->m_szNameEng.get(), nIndex);
			m_pMMap->SetAt(pDef->m_szNameRus.get(), nIndex);
		}
		return nIndex;
	}

	size_t AddMeth(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams, PF_Proc pProc, PF_ParamDef pParamDef = NULL)
	{
		return AddMeth(szNameEng, szNameRus, nParams, pProc, NULL, pParamDef);
	}
	size_t AddMeth(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams, PF_Func pFunc, PF_ParamDef pParamDef = NULL)
	{
		return AddMeth(szNameEng, szNameRus, nParams, NULL, pFunc, pParamDef);
	}

	size_t AddProp(LPCSTR szNameEng, LPCSTR szNameRus, PF_Get pGet, PF_Set pSet)
	{
		size_t nIndex;
		if (!m_pPMap->Lookup(szNameEng, nIndex))
		{
			V_PROP_DEF::value_type pDef(new s_prop_def(szNameEng, szNameRus, pGet, pSet));
			nIndex = m_vPDefs.size();
			m_vPDefs.push_back(pDef);
			m_pPMap->SetAt(pDef->m_szNameEng.get(), nIndex);
			m_pPMap->SetAt(pDef->m_szNameRus.get(), nIndex);
		}
		return nIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	// CBLContext reflection
	//////////////////////////////////////////////////////////////////////////
	int GetNMethods() const
	{
		return m_vMDefs.size();
	}

	int FindMethod(LPCSTR szName) const
	{
		size_t nIndex;
		return m_pMMap->Lookup(szName, nIndex) ? nIndex : -1;
	}

	LPCSTR GetMethodName(int nIndex, int nLanguageIdx) const
	{
		LPCSTR szName = NULL;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			szName = !nLanguageIdx ? pDef->m_szNameEng.get() : pDef->m_szNameRus.get();
		}
		return szName;
	}

	int GetNParams(int nIndex) const
	{
		int nParams = 0;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			nParams = pDef->m_nParams;
		}
		return nParams;
	}

	BOOL GetParamDefValue(CBLContext const* pCont, int nIndex, int nParamIndex, CValue* pVal) const
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			if ((size_t)nParamIndex < pDef->m_nParams)
			{
				PF_ParamDef pf = pDef->m_pParamDef;
				if (pf)
					bSuccess = (pCont->*pf)(nParamIndex, pVal);
				else
				{
					pVal->Reset();
					pVal->SetTypeCode(UNSPECIFIED_TYPE_1C);
					bSuccess = TRUE;
				}
			}
		}
		return bSuccess;
	}

	BOOL HasRetVal(int nIndex) const
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			bSuccess = pDef->m_pFunc != NULL;
		}
		return bSuccess;
	}

	BOOL CallAsFunc(CBLContext* pCont, int nIndex, CValue& RetValue, CValue** ppValue)
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			bSuccess = (pCont->*pDef->m_pFunc)(RetValue, ppValue);
		}
		return bSuccess;
	}

	BOOL CallAsProc(CBLContext* pCont, int nIndex, CValue** ppValue)
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vMDefs.size())
		{
			V_METH_DEF::const_reference pDef = m_vMDefs[nIndex];
			bSuccess = (pCont->*pDef->m_pProc)(ppValue);
		}
		return bSuccess;
	}

	int GetNProps() const
	{
		return m_vPDefs.size();
	}

	int FindProp(LPCSTR szName) const
	{
		size_t nIndex;
		return m_pPMap->Lookup(szName, nIndex) ? nIndex : -1;
	}

	LPCSTR GetPropName(int nIndex, int nLanguageIdx) const
	{
		LPCSTR szName = NULL;
		if ((size_t)nIndex < m_vPDefs.size())
		{
			V_PROP_DEF::const_reference pDef = m_vPDefs[nIndex];
			szName = !nLanguageIdx ? pDef->m_szNameEng.get() : pDef->m_szNameRus.get();
		}
		return szName;
	}

	BOOL GetPropVal(CBLContext const* pCont, int nIndex, CValue& RetValue) const
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vPDefs.size())
		{
			V_PROP_DEF::const_reference pDef = m_vPDefs[nIndex];
			bSuccess = (pCont->*pDef->m_pGet)(RetValue);
		}
		return bSuccess;
	}

	BOOL SetPropVal(CBLContext* pCont, int nIndex, CValue const& Value)
	{
		int bSuccess = FALSE;
		if ((size_t)nIndex < m_vPDefs.size())
		{
			V_PROP_DEF::const_reference pDef = m_vPDefs[nIndex];
			bSuccess = (pCont->*pDef->m_pSet)(Value);
		}
		return bSuccess;
	}

	BOOL IsPropReadable(int nIndex) const
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vPDefs.size())
		{
			V_PROP_DEF::const_reference pDef = m_vPDefs[nIndex];
			bSuccess = pDef->m_pGet != NULL;
		}
		return bSuccess;
	}

	int	IsPropWritable(int nIndex) const
	{
		BOOL bSuccess = FALSE;
		if ((size_t)nIndex < m_vPDefs.size())
		{
			V_PROP_DEF::const_reference pDef = m_vPDefs[nIndex];
			bSuccess = pDef->m_pSet != NULL;
		}
		return bSuccess;
	}

private:
	typedef CIStringMap<size_t, size_t> M_DEF;
	typedef boost::shared_ptr<M_DEF> M_DEF_ptr;
	typedef std::vector<s_meth_def_ptr> V_METH_DEF;
	typedef std::vector<s_prop_def_ptr> V_PROP_DEF;

	M_DEF_ptr m_pMMap;
	V_METH_DEF m_vMDefs;
	M_DEF_ptr m_pPMap;
	V_PROP_DEF m_vPDefs;
};

//////////////////////////////////////////////////////////////////////////
// CSimpleContext class
//////////////////////////////////////////////////////////////////////////
class CSimpleContext : public CBLContext
{
public:
	DECLARE_DYNAMIC(CSimpleContext)

	CSimpleContext(CSimpleDef* pDef, BOOL bDefValid, LPCSTR szTypeString)
		: m_pDef(pDef), m_TypeString(szTypeString ? szTypeString : "SimpleContext") {}
	virtual ~CSimpleContext() {}

	CSimpleDef* GetDef() { return m_pDef; }

	template<typename T>
	size_t AddDef(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams,
		BOOL (T::* pFunc)(CValue&, CValue**),
		BOOL (T::* pParamDef)(int, CValue*) const = NULL)
	{
		return m_pDef->AddMeth(szNameEng, szNameRus, nParams,
			static_cast<CSimpleDef::PF_Func>(pFunc),
			static_cast<CSimpleDef::PF_ParamDef>(pParamDef));
	}

	template<typename T>
	size_t AddDef(LPCSTR szNameEng, LPCSTR szNameRus, size_t nParams,
		BOOL (T::* pProc)(CValue**),
		BOOL (T::* pParamDef)(int, CValue*) const = NULL)
	{
		return m_pDef->AddMeth(szNameEng, szNameRus, nParams,
			static_cast<CSimpleDef::PF_Proc>(pProc),
			static_cast<CSimpleDef::PF_ParamDef>(pParamDef));
	}

	template<typename T>
	size_t AddDef(LPCSTR szNameEng, LPCSTR szNameRus,
		BOOL (T::* pGet)(CValue&) const,
		BOOL (T::* pSet)(CValue const&) = NULL)
	{
		return m_pDef->AddProp(szNameEng, szNameRus,
			static_cast<CSimpleDef::PF_Get>(pGet),
			static_cast<CSimpleDef::PF_Set>(pSet));
	}

	template<typename T>
	size_t AddDef(LPCSTR szNameEng, LPCSTR szNameRus,
		BOOL (T::* pSet)(CValue const&))
	{
		return m_pDef->AddProp(szNameEng, szNameRus,
			NULL,
			static_cast<CSimpleDef::PF_Set>(pSet));
	}

	template<typename T>
	static T IsNULL(T a, T b)
	{
		return a ? a : b;
	}

private:
	CSimpleContext(CSimpleContext const&);
	CSimpleContext& operator=(CSimpleContext const&);

public:
	// 	virtual void InitObject(char const* szName);

	virtual char const* GetTypeString(void) const
	{
		return m_TypeString.c_str();
	}

	virtual int	GetNMethods(void) const
	{
		return m_pDef->GetNMethods();
	}

	virtual int	FindMethod(char const* szName) const
	{
		return m_pDef->FindMethod(szName);
	}

	virtual char const* GetMethodName(int nMethIndex, int nAlias) const
	{
		return m_pDef->GetMethodName(nMethIndex, nAlias);
	}

	virtual int	GetNParams(int nMethIndex) const
	{
		return m_pDef->GetNParams(nMethIndex);
	}

	virtual int	GetParamDefValue(int nMethIndex, int nParamIndex, CValue* pDefVal) const
	{
		return m_pDef->GetParamDefValue(this, nMethIndex, nParamIndex, pDefVal);
	}

	virtual int	HasRetVal(int nMethIndex) const
	{
		return m_pDef->HasRetVal(nMethIndex);
	}

	virtual int	CallAsProc(int nMethIndex, CValue** ppVal)
	{
		BOOL bSuccess = FALSE;
		try
		{
			bSuccess = m_pDef->CallAsProc(this, nMethIndex, ppVal);
		}
		catch(std::exception const& ex)
		{
			RuntimeError(ex.what());
		}
		return bSuccess;
	}

	virtual int	CallAsFunc(int nMethIndex, CValue& RetVal, CValue** ppVal)
	{
		BOOL bSuccess = FALSE;
		try
		{
			bSuccess = m_pDef->CallAsFunc(this, nMethIndex, RetVal, ppVal);
		}
		catch(std::exception const& ex)
		{
			RuntimeError(ex.what());
		}
		return bSuccess;
	}

	virtual int	GetNProps(void) const
	{
		return m_pDef->GetNProps();
	}

	virtual int	FindProp(LPCSTR szName) const
	{
		return m_pDef->FindProp(szName);
	}

	virtual char const* GetPropName(int nPropIndex, int nLanguageIdx) const
	{
		return m_pDef->GetPropName(nPropIndex, nLanguageIdx);
	}
	virtual int	GetPropVal(int nPropIndex, CValue& RetValue) const
	{
		BOOL bSuccess = FALSE;
		try
		{
			bSuccess = m_pDef->GetPropVal(this, nPropIndex, RetValue);
		}
		catch(std::exception const& ex)
		{
			RuntimeError(ex.what());
		}
		return bSuccess;
	}

	virtual int	SetPropVal(int nPropIndex, CValue const& Value)
	{
		BOOL bSuccess = FALSE;
		try
		{
			bSuccess = m_pDef->SetPropVal(this, nPropIndex, Value);
		}
		catch(std::exception const& ex)
		{
			RuntimeError(ex.what());
		}
		return bSuccess;
	}

	virtual int	IsPropReadable(int nPropIndex) const
	{
		return m_pDef->IsPropReadable(nPropIndex);
	}

	virtual int	IsPropWritable(int nPropIndex) const
	{
		return m_pDef->IsPropWritable(nPropIndex);
	}

	//////////////////////////////////////////////////////////////////////////
private:
	CSimpleDef* m_pDef;
	std::string m_TypeString;
};

//////////////////////////////////////////////////////////////////////////
// CSimpleRegistrar class
//////////////////////////////////////////////////////////////////////////

class CSimpleRegistrar
{
public:
	CSimpleRegistrar(CRuntimeClass* pRTC, LPCSTR szEng, LPCSTR szRus = NULL)
		: m_pRTC(pRTC)
	{
		CBLContext::RegisterContextClass(pRTC, szEng, CType(100));
		if (szRus)
			CBLContext::RegisterContextClass(pRTC, szRus, CType(100));
	}
	~CSimpleRegistrar()
	{
		CBLContext::UnRegisterContextClass(m_pRTC);
	}
private:
	CSimpleRegistrar(CSimpleRegistrar const&);
	CSimpleRegistrar& operator=(CSimpleRegistrar const&);

	CRuntimeClass* m_pRTC;
};
#endif
