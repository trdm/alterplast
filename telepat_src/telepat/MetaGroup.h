// MetaGroup.h: interface for the CMetaGroup class.
#if !defined(AFX_METAGROUP_H__1B498FE0_D3B8_4ACF_9F1B_302EB7145BAF__INCLUDED_)
#define AFX_METAGROUP_H__1B498FE0_D3B8_4ACF_9F1B_302EB7145BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "telecont.h"
#include "blmap.h"
struct SLexem;

class CMetaGroup : public CTeleCont
{
public:
	void FillSubconto(CMetaDataObj* pObj);
	void FillSubList(CMetaDataObj* pObj);
	void FillDoc(CMetaDataObj* pObj);
	void FillJournal(CMetaDataObj* pObj);
	void FillAccountChart(CMetaDataObj* pObj);
	void FillOperation(CMetaDataObj* pObj);
	void FillOperationList(CMetaDataObj* pObj);
	void FillCJ(CMetaDataObj* pObj);
	void FillCJournal(CMetaDataObj* pObj);
	void FillCalcAlg(CMetaDataObj* pObj);
	
	CMetaGroup(CParser* pParser);
	virtual void Clear();
	virtual void FillGroup(CPtrArray& arr);
	
	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	bool IsAttribExist(SLexem& name);
	void FillFromObjArr(CMetaDataObjArray* pArr);
	typedef void (CMetaGroup::*PFILLFUNC)(CMetaDataObj*);
	PFILLFUNC m_FillFunc;
	CBLMap m_atribs;
	static PFILLFUNC m_func[];

};

#endif // !defined(AFX_METAGROUP_H__1B498FE0_D3B8_4ACF_9F1B_302EB7145BAF__INCLUDED_)
