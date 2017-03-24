// savewrapper.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "savewrapper.h"
#include "metadata.h"
#include "iconfig.h"
#include "hackvirtual.h"

static CTrapSwap saveMD;
static CTrapSwap saveDoc;
static CTrapSwap saveErt;
static CTrapSwap saveText;
static CTrapSwap saveMXL;

typedef int (CDocument::*PONSAVE)();
typedef int (CDocument::*PSAVETOPATH)(LPCTSTR,int);

void CSaveWrapper::Init()
{
	saveMD.SetTrap  ("seven.dll","?DoFileSaveToPath@CConfigCont@@QAEHPBDH@Z",(AFX_PMSG)DoFileSaveToPath);
	saveDoc.SetTrap ("seven.dll","?DoFileSave@CConfigCont@@UAEHXZ",(AFX_PMSG)DoFileSaveDoc);
	saveErt.SetTrap ("seven.dll","?DoFileSave@CWorkBookDoc@@UAEHXZ",(AFX_PMSG)DoFileSaveErt);
	saveText.SetTrap("txtedt.dll","?DoFileSave@CTextDocument@@UAEHXZ",(AFX_PMSG)DoFileSaveText);
	saveMXL.SetTrap ("moxel.dll","?DoFileSave@CSheetDoc@@UAEHXZ",(AFX_PMSG)DoFileSaveMXL);
}

// Собственно функция-перехватчик
int CSaveWrapper::DoFileSaveToPath(LPCTSTR name,int param)
{
	saveMD.Swap();	// Снимаем заглушку с настоящей функции
	// Вызываем настоящую функцию, пусть сохранит.
	int ret=(this->*(PSAVETOPATH)saveMD.m_pFuncTrapped.pClassFunc)(name,param);
	// Ставим заглушку обратно
	saveMD.Swap();	// Снимаем заглушку с настоящей функции
	if(ret)
	{
		// Сохранились успешно.
		CIConfigEvents::m_ConfigEvents.FireMDSaved(name);
		/*
		Важно: в этот момент окошечко "Реорганизация данных завершена"
		еще не выскочило. Файл мд в new_stru уже сохранен и соответствует
		новой структуре мд. (name есть полный путь к нему).
		Поэтому считывать новый мдшник надо из new_stru
		*/
	}
	return ret;
}

bool CSaveWrapper::FastSaveMDToFile(LPCTSTR lpPath,bool FireEvent)
{
	CIMetaData* pMD=CIMetaData::m_pOneMetaData;
	short modif=0;
	pMD->get_Modified(&modif);
	if(!FireEvent)
		saveMD.Swap();
	bool ret=CConfigCont::GetContainer()->DoFileSaveToPath(lpPath,FALSE)!=0;
	if(!FireEvent)
		saveMD.Swap();
	if(modif==1)
		pMD->SetDocModified();
	else if(modif==2)
		pMD->SetMDModified();
	return ret;
}

int CSaveWrapper::DoFileSaveErt()
{
	saveErt.Swap();
	int ret=(this->*(PONSAVE)saveErt.m_pFuncTrapped.pClassFunc)();
	saveErt.Swap();
	if(ret)
		CIConfigEvents::m_ConfigEvents.FireFileSave(this);
	return ret;
}

int CSaveWrapper::DoFileSaveText()
{
	saveText.Swap();
	int ret=(this->*(PONSAVE)saveText.m_pFuncTrapped.pClassFunc)();
	saveText.Swap();
	if(ret)
		CIConfigEvents::m_ConfigEvents.FireFileSave(this);
	return ret;
}

int CSaveWrapper::DoFileSaveMXL()
{
	saveMXL.Swap();
	int ret=(this->*(PONSAVE)saveMXL.m_pFuncTrapped.pClassFunc)();
	saveMXL.Swap();
	if(ret)
		CIConfigEvents::m_ConfigEvents.FireFileSave(this);
	return ret;
}

int CSaveWrapper::DoFileSaveDoc()
{
	saveDoc.Swap();
	int ret=(this->*(PONSAVE)saveDoc.m_pFuncTrapped.pClassFunc)();
	saveDoc.Swap();
	if(ret)
		CIConfigEvents::m_ConfigEvents.FireMDSaved(CConfigSvcImpl::m_pTheService->m_IBPath+"1cv7.md");
	return ret;
}

