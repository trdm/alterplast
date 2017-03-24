// typegroup.h
#ifndef TYPEGROUP_H
#define TYPEGROUP_H
#include "TeleCont.h"
#include "blmap.h"
struct SLexem;

class CTypeGroup:public CTeleCont
{
public:
	static void ReFillAll();
	CTypeGroup(LPCTSTR type);

	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	virtual void Clear()
	{
		CTeleCont::Clear();
		m_mAttribs.RemoveAll();
	}
	virtual void FillGroup(CPtrArray& arr);
	
	bool IsAttribExist(SLexem& name);
	CBLMap	m_mAttribs;

	static void Init();
	static CBLMap m_mMetaTypes;
	static LPCTSTR ConfType2TType(LPCTSTR typeName);
	static int GetConfTypeNum(LPCTSTR typeName);

	static CTypeGroup* GetTypeGroup(LPCTSTR type);
	static CBLMap	m_mTypeGroups;
};


#endif