// localvarcont.h
#ifndef LOCALVARCONT_H
#define LOCALVARCONT_H

#include "telecont.h"

struct SLocalVarInfo{
	SLexem name;
	char type;	// 0 - param, 1 - Var, 2 - assignment
	CString array;
	SLexem lastDef;
};

class CLocalVarCont:public CTeleCont
{
public:
	CLocalVarCont(CParser*);
	
	virtual void FillGroup(CPtrArray& arr);
	virtual void Clear();
	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	int GetCount()
	{
		return m_Vars.GetSize();
	}
	SLocalVarInfo* GetVar(int i)
	{
		return (SLocalVarInfo*)m_Vars[i];
	}
	SLocalVarInfo* GetVar(SLexem& lexem)
	{
		return (SLocalVarInfo*)m_mVars.GetKey(lexem);
	}
	SLocalVarInfo* GetVar(CString& lexem)
	{
		return (SLocalVarInfo*)m_mVars.GetKey(lexem);
	}
	SLocalVarInfo* AddVar(SLexem& name);
	CPtrArray m_Vars;
	CBLMap	m_mVars;
};

#endif