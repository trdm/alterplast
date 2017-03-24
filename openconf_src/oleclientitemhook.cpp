// oleclientitemhook.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "oleclientiemhook.h"
#include "iconfig.h"

static const IID SID_Application={
	0xB722BE00,0x4E68,0x101B,{0xA2,0xBC,0x00,0xAA,0x00,0x40,0x47,0x70}};

//DEFINE_GUID(SID_Application,0xB722BE00,0x4E68,0x101B,0xA2,0xBC,0x00,0xAA,0x00,0x40,0x47,0x70);
// {B722BE00-4E68-101B-A2BC-00AA00404770}

BEGIN_INTERFACE_MAP(COleClientItemHook,COleDocObjectItem)
    INTERFACE_PART(COleClientItemHook, IID_IServiceProvider,ServProv)
END_INTERFACE_MAP()

ULONG FAR EXPORT COleClientItemHook::XServProv::AddRef()
{
    METHOD_PROLOGUE(COleClientItemHook, ServProv)
    return pThis->ExternalAddRef();
}

ULONG FAR EXPORT COleClientItemHook::XServProv::Release()
{
    METHOD_PROLOGUE(COleClientItemHook, ServProv)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT COleClientItemHook::XServProv::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(COleClientItemHook, ServProv)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}
STDMETHODIMP COleClientItemHook::XServProv::QueryService(REFGUID guidService,
														 REFIID riid,void** ppv)
{
	if(guidService==SID_Application)
	{
		CConfigSvcImpl::m_pTheService->m_Configurator->QueryInterface(riid,ppv);
		if(*ppv)
			return S_OK;
		return E_NOINTERFACE;
	}
	return E_NOINTERFACE;
}