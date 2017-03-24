#if !defined(AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_)
#define AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_

#pragma once
#include "resource.h"

//////////////////////////////////////////////////////////////////////////
// IPropertyProfile
//////////////////////////////////////////////////////////////////////////
class IPropertyProvider
{
public:
	virtual int ReadInt(LPCSTR szKey, int defVal) = 0;
	virtual BOOL WriteInt(LPCSTR szKey, int nVal) = 0;
};

class CRegistryPropertyProvider : public IPropertyProvider
{
private:
	CString m_csSection;
	CApp7* m_pApp7;
public:
	CRegistryPropertyProvider(LPCSTR szPath = NULL)
	{
		m_csSection = szPath ? szPath : "1C++ options";
		m_pApp7 = static_cast<CApp7*>(AfxGetApp());
	}
	~CRegistryPropertyProvider();

	virtual int ReadInt(LPCSTR szKey, int defVal)
	{
		return m_pApp7->GetProfileInt(m_csSection, szKey, defVal);
	}

	virtual BOOL WriteInt(LPCSTR szKey, int nVal)
	{
		return m_pApp7->WriteProfileInt(m_csSection, szKey, nVal);
	}
};

//////////////////////////////////////////////////////////////////////////
// CAddIn
//////////////////////////////////////////////////////////////////////////

class CAddIn
{
public:
	CAddIn();
	~CAddIn();

    BOOL m_EnabledCheckType;        // This property enables or disables component
    CButton m_boxEnabledCheckType;  // Corresponded check box

    BOOL m_EnableOptimizate;            
    CButton m_boxOptimizate;  

    BOOL m_EnableDebugging;        
    CButton m_boxEnableDebugging;     

private:
	boost::shared_ptr<class CAddInPropertyCollection> m_pPropertyCollection;
	void SafeRegisterContextClass(CRuntimeClass* pRuntimeClass , LPCSTR szContextName);
	void RegisterAllContextClasses();
	void RemoveAllOfRegisterContextClasses();
	std::vector<CRuntimeClass*> m_vRegisteredRTC;

public: 
	__declspec(property(get=get_Property, put=put_Property)) BOOL Property [];
	BOOL get_Property(LPCSTR key);
	void put_Property(LPCSTR key, BOOL newValue);

private:
	void InitProperties(); // только настраивает свойства, из реестра ничего не читается !!
    void LoadProperties(IPropertyProvider* pProvider = NULL);
    void SaveProperties();
public:
	CString GetPropertyInfo() const;

public:
	STDMETHOD(Init());
	STDMETHOD(Done()) throw ();
};
#endif // !defined(AFX_ADDIN_H__FC223DE1_F108_11D0_A459_004095E1DAEA__INCLUDED_)
