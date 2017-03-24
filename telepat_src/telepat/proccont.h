// proccont.h
#ifndef PROCCONT_H
#define PROCCONT_H

#include "telecont.h"

struct SParamInfo{
	SParamInfo():next(NULL){}
	~SParamInfo()
	{
		if(next)
			delete next;
	}
	CString name;
	CString def;
	bool val;
	bool array;
	SParamInfo* next;
};

struct SProcInfo{
	SProcInfo(SLexem s,SLexem n):name(n),start(s),params(NULL){}
	~SProcInfo()
	{
		if(params)
			delete params;
	}
	void AddParam(SParamInfo* par)
	{
		if(!params)
			params=par;
		else
		{
			SParamInfo* p=params;
			while(p->next)
				p=p->next;
			p->next=par;
		}
	}
	CString name;
	SLexem start;
	bool export;
	bool func;
	SParamInfo* params;
};

class CProcCont:public CTeleCont
{
public:
	SProcInfo* GetForwards(CString name);
	int FillListBox(CWnd* list,int sort,int curLine,CPtrArray& arr,bool listbox=true);
	CProcCont(CParser* pParser):CTeleCont(pParser){InitGroups();}
	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	virtual void FillGroup(CPtrArray& arr);
	virtual void Clear();

	int GetCount()
	{
		return m_Proc.GetSize();
	}
	SProcInfo* GetProc(int n)
	{
		return (SProcInfo*)m_Proc[n];
	}
	SProcInfo* GetProc(CString name)
	{
		return (SProcInfo*)m_mNames.GetKey(name);
	}
	void AddProc(SProcInfo* pInfo,bool forward);
	CPtrArray		m_Proc;
	CPtrArray		m_Forwards;
	CBLMap			m_mNames;
};

class CMethodEntry:public CTeleEntry
{
public:
	bool MakeInsert();
	CMethodEntry(SProcInfo* pInfo,bool f):m_pInfo(pInfo),m_forward(f)
	{
		m_Name=pInfo->name;
	}
	SProcInfo* m_pInfo;
	bool m_forward;
	void GetHint(CString& buf);
	void GetInsert(CString& buf);
};
#endif
