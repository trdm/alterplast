// Vis1cppPlugin.cpp : Implementation of CVis1cppPlugin
#include "stdafx.h"
#include "Vis1cpp.h"
#include "Vis1cppPlugin.h"

#include "CPPView.h"
#include "Info.h"
#include "CoDefFiles.h"

/////////////////////////////////////////////////////////////////////////////
// CVis1cppPlugin

CVis1cppPlugin* CVis1cppPlugin::pPlugin;

CVis1cppPlugin::CVis1cppPlugin()
{
	pPlugin = this;
}

STDMETHODIMP CVis1cppPlugin::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVis1cppPlugin
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CVis1cppPlugin::Method1()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	AfxMessageBox("Method 1");

	return S_OK;
}

STDMETHODIMP CVis1cppPlugin::get_FlagGroupByType(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	*pVal = CInfo::IsSort() ? TRUE : FALSE;

	return S_OK;
}

STDMETHODIMP CVis1cppPlugin::put_FlagGroupByType(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CInfo::SortAllTree( FALSE == newVal ? false : true);

	return S_OK;
}

STDMETHODIMP CVis1cppPlugin::RefreshAll()
{
	// не вставляем вызов AFX_MANAGE_STATE(AfxGetStaticModuleState()) при вызове методов, вызывающих встроенные методы 1С !!
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	
	CCPPView::GetView()->Refresh();
	
	return S_OK;
}

STDMETHODIMP CVis1cppPlugin::GetClasses(IClasses **pClasses)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	CCoClasses::Get()->IfNeeded_FillAllElements();
	CCoClasses::GetInterfacePtr().CopyTo(pClasses);

	return S_OK;
}

CVis1cppPlugin::~CVis1cppPlugin()
{
//	int i = 1;	
}

STDMETHODIMP CVis1cppPlugin::Done()
{
	CCoClasses::Destroy();
	CCoDefFiles::Destroy();

	return E_NOTIMPL;
}

STDMETHODIMP CVis1cppPlugin::Init( IConfigurator * pConfigurator, BSTR * PluginName )
{
	if (PluginName == NULL)	return E_POINTER;

	CComBSTR  pStr = "Visual1Cpp";
	pStr.CopyTo(PluginName);
	pStr.Empty();
	
	CCoClasses::Init();
	CCoDefFiles::Init();

	return S_OK;
}

STDMETHODIMP CVis1cppPlugin::GetDefFiles(IDefFiles **DefFiles)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CCoDefFiles::Get()->IfNeeded_FillAllElements();
	CCoDefFiles::GetInterfacePtr().CopyTo(DefFiles);

	return S_OK;
}
