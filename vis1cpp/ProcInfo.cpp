// ProcInfo.cpp: implementation of the CProcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "ProcInfo.h"
#include "classinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// CProcInfo::~CProcInfo()
// {
// 
// }
// void CProcInfo::OnSelect()
// {
// // 	CClassInfo* pInfo=(CClassInfo*)GetParentData(GetCurItem());
// // 	pInfo->OpenKeys(m_DisplayName);
// 	OpenKeysForClassInfoFromCurItem();
// }