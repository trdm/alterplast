// varcont.h
#ifndef VARCONT_H
#define VARCONT_H
#include "telecont.h"

struct SVarInfo{
	SLexem name;
	CString array;
	bool export;
};

class CVarCont:public CTeleCont
{
public:
	CVarCont(CParser*);
	void FillGroup(CPtrArray& arr);
	void Clear();

	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	int GetCount()
	{
		return m_Vars.GetSize();
	}
	SVarInfo* GetVar(int i)
	{
		return (SVarInfo*)m_Vars[i];
	}
	SVarInfo* GetVar(SLexem& var)
	{
		return (SVarInfo*)m_mVars.GetKey(var);
	}
	SVarInfo* GetVar(CString& var)
	{
		return (SVarInfo*)m_mVars.GetKey(var);
	}
	SVarInfo* AddVar(SLexem& var);
	CPtrArray	m_Vars;
	CBLMap		m_mVars;
};

#endif