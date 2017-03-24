// oleclientiemhook.h
#ifndef OLECLIENTITEMHOOK_H
#define OLECLIENTITEMHOOK_H
class COleClientItemHook:public COleDocObjectItem
{
public:
	COleClientItemHook(COleDocument* pDoc):COleDocObjectItem(pDoc){}
	~COleClientItemHook(){}
	LPOLEOBJECT GetObject(){return m_lpObject;}
	void Load(IStorage* pStore)
	{
		HRESULT hr=OleLoad(pStore,IID_IOleObject,(IOleClientSite*)&m_xOleClientSite,(void**)&m_lpObject);
	}
	void OpenPlugin(IDispatchPtr& pPlugin)
	{
		pPlugin->QueryInterface(IID_IOleObject,(void**)&m_lpObject);
		if(!m_lpObject)
			return;
		LPOLECLIENTSITE lpClientSite =
			(LPOLECLIENTSITE)GetInterface(&IID_IOleClientSite);
		m_lpObject->SetClientSite(lpClientSite);
		/*m_dwItemNumber = GetNewItemNumber();
		GetItemStorage();
		IPersistStoragePtr pStore=m_lpObject;
		if(pStore)
		{
			pStore->InitNew(m_lpStorage);
		}
		*/
	}
    DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART(ServProv, IServiceProvider)
		STDMETHOD(QueryService)(REFGUID guidService,REFIID riid,void** ppv);
    END_INTERFACE_PART(ServProv)
};
#endif