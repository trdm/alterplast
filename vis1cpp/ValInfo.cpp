// ValInfo.cpp: implementation of the CValInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "ValInfo.h"
#include "classinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// CValInfo::~CValInfo()
// {
// 
// }
// 
// void CValInfo::OnSelect()
// {
// // 	CClassInfo* pInfo=(CClassInfo*)GetParentData(GetCurItem());
// // 	pInfo->OpenKeys(m_DisplayName);
// 	OpenKeysForClassInfoFromCurItem();
// }