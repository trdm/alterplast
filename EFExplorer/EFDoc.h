// EFDoc.h: interface for the CEFDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFDOC_H__CA12204B_39D5_405D_8DFB_EEF2F118F785__INCLUDED_)
#define AFX_EFDOC_H__CA12204B_39D5_405D_8DFB_EEF2F118F785__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEFDoc : public CDocument  
{
	DECLARE_DYNCREATE(CEFDoc)
protected:
	CEFDoc();           // protected constructor used by dynamic creation
public:
	virtual ~CEFDoc();

};

#endif // !defined(AFX_EFDOC_H__CA12204B_39D5_405D_8DFB_EEF2F118F785__INCLUDED_)
