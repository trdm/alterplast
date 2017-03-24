// Initializer.cpp: implementation of the CInitializer class.

#include "stdafx.h"
#include "configsvcimpl.h"
#include "Initializer.h"
#include "savewrapper.h"
#include "metadata.h"
#include "scriptmaneger.h"
#include "pluginmanager.h"
#include "silenser.h"
#include "cfgwindows.h"
#include "activedoc.h"
#include "ocmenu.h"
#include "iplugins.h"
#include "iscripts.h"
#include "icfgdocuments.h"
#include "ITimer.h"
#include "IdleHandler.h"
#include "SECToolBarMgr.h"
#include "IMDWindow.h"
#include "PanelManager.h"
#include "IDialogDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PINITFUNC CInitializer::m_InitOnDLLMain[]=
	{
		CFakeWnd::Init,
		CConfigSvcImpl::InitTextDoc,
		CSaveWrapper::Init,
		CSilenser::Init,
		CICfgWindows::Init,
		CICfgDocuments::Init,
		CIdleHandler::Init,
		SECToolBarMgr::Init,
		CIMetaData::Init,
		CIMDWindow::Init,
		CIDialogDoc::Init,
		NULL,
	};
PINITFUNC CInitializer::m_InitOnCreateMainWnd[]=
	{
		CScriptManager::InitScriptManeger,
		CActiveDoc::Init,
		NULL,
	};
PINITFUNC CInitializer::m_InitOnCreateConfigWnd[]=
	{
		NULL,
	};
PINITFUNC CInitializer::m_Done[]=
	{
		CScriptManager::Done,
		CPluginManager::Done,
		CSilenser::Done,
		CIPlugins::Done,
		CIScripts::Done,
		CITimer::Done,
		CIdleHandler::Done,
		SECToolBarMgr::Done,
		NULL,
	};

void CInitializer::Init(PINITFUNC* ppArrayOfFunc)
{
	while(*ppArrayOfFunc)
	{
		LOG("Init func %08X",ppArrayOfFunc);
		(*ppArrayOfFunc)();
		LOG("Init func %08X success",ppArrayOfFunc);
		*ppArrayOfFunc++=NULL;
	}
}
