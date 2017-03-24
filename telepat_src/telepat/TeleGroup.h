// TeleGroup.h: interface for the CTeleGroup class.
#if !defined(AFX_TELEGROUP_H__53C19A28_98DD_41D6_88DA_BB4EBE8C7CEC__INCLUDED_)
#define AFX_TELEGROUP_H__53C19A28_98DD_41D6_88DA_BB4EBE8C7CEC__INCLUDED_

class CTeleGroup;
class CTeleEntry{
public:
	CTeleEntry():m_pParent(NULL){}
	CTeleEntry(CString t):m_Name(t),m_pParent(NULL){};
	CTeleEntry(CTeleGroup* parent):m_pParent(parent){}
	CTeleEntry(CString t,CTeleGroup* parent):m_Name(t),m_pParent(parent){}
	virtual ~CTeleEntry(){}
	virtual void GetHint(CString& buf){buf.Empty();}
	virtual void GetInsert(CString& buf){buf=m_Name;}
	virtual bool MakeInsert(){return false;}
	CString m_Name;
	CTeleGroup* m_pParent;
};

class CTeleGroup  
{
public:
	CTeleGroup();
	virtual ~CTeleGroup();
	void ClearEntries();
	int GetCount(){return m_Entries.GetSize();}
	CTeleEntry* GetEntry(int pos){return (CTeleEntry*)m_Entries[pos];}
	void AddEntry(CTeleEntry* pEntry)
	{
		pEntry->m_pParent=this;
		m_Entries.Add(pEntry);
	}

	UINT m_Image;
	CString name;
	CPtrArray m_Entries;
};

#endif // !defined(AFX_TELEGROUP_H__53C19A28_98DD_41D6_88DA_BB4EBE8C7CEC__INCLUDED_)
