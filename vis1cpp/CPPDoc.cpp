// CPPDoc.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "config.h"
#include "CPPDoc.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

//CBkEndUI *pBkEndUI;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString PrintRTC(CRuntimeClass* pClass)
{
	CString txt;
	while(pClass)
	{
		txt=txt+pClass->m_lpszClassName+"->";
		pClass=(*pClass->m_pfnGetBaseClass)();
	}
	return txt;
}
CString PrintRTC(CObject* pObj)
{
	if(!pObj)
		return "";
	return PrintRTC(pObj->GetRuntimeClass());
}

/////////////////////////////////////////////////////////////////////////////
// CCPPDoc

IMPLEMENT_DYNCREATE(CCPPDoc, CDocument)

CCPPDoc::CCPPDoc() //: m_Meta( new CMetaDataOfClasses )
{
	UpdateMetadata();
  //CMetaDataOfClasses::m_oneMetadata = m_Meta.get();
}

BOOL CCPPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCPPDoc::~CCPPDoc()
{
	//delete m_Meta;
  CMetaDataOfClasses::m_oneMetadata = NULL;
}


BEGIN_MESSAGE_MAP(CCPPDoc, CCmdTarget)
	//{{AFX_MSG_MAP(CCPPDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPPDoc diagnostics

#ifdef _DEBUG
void CCPPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCPPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCPPDoc serialization

void CCPPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void CCPPDoc::UpdateMetadata()
{
	m_Meta.reset( new CMetaDataOfClasses );
	assert(m_Meta);
	CMetaDataOfClasses::m_oneMetadata = m_Meta.get();
}
/////////////////////////////////////////////////////////////////////////////
// CCPPDoc commands

