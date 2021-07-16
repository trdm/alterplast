// CntrItem.cpp : implementation of the CRichEgCntrItem class
//

#include "stdafx.h"
//#include "RichEg.h"

#include "RichEgDoc.h"
#include "RichEgView.h"
#include "CntrItemEg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEgCntrItem implementation

IMPLEMENT_SERIAL(CRichEgCntrItem, CRichEditCntrItem, 0)

CRichEgCntrItem::CRichEgCntrItem(REOBJECT* preo, CRichEgDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CRichEgCntrItem::~CRichEgCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CRichEgCntrItem diagnostics

#ifdef _DEBUG
void CRichEgCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CRichEgCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
