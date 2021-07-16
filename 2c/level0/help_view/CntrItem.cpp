// CntrItem.cpp : implementation of the CTestForMyHelpDocCntrItem class
//

#include "stdafx.h"
//#include "TestForMyHelpDoc.h"

#include "MyHelpDoc.h"
#include "MyHelpView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestForMyHelpDocCntrItem implementation

IMPLEMENT_SERIAL(CTestForMyHelpDocCntrItem, CRichEditCntrItem, 0)

CTestForMyHelpDocCntrItem::CTestForMyHelpDocCntrItem(REOBJECT* preo, CMyHelpDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CTestForMyHelpDocCntrItem::~CTestForMyHelpDocCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CTestForMyHelpDocCntrItem diagnostics

#ifdef _DEBUG
void CTestForMyHelpDocCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CTestForMyHelpDocCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
