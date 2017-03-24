// IDialogDoc.cpp: implementation of the CIDialogDoc class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IDialogDoc.h"
#include "HackVirtual.h"
#include "ICfgDocuments.h"
#include "IConfig.h"
#include "IType1C.h"

ITypeInfo* CIDialogDoc::m_pTypeInfo=NULL;

static CTrapVirtual trapModified;
static CTrapSwap trapCrStatic;
static CTrapSwap trapCrPerm;
static CTrapSwap trapDefFixed;
static CTrapSwap trapDefNew;
static CTrapSwap trapCrInd;

class CEditDocWrap:public CEditDoc
{
public:
	int InitFA(CArchive& ar)
	{
		return InitFromArchive(ar);
	}
	void SetModifiedFlagW(BOOL bModif)
	{
		(this->*(void (CEditDocWrap::*)(BOOL))trapModified.m_pRealAdress)(bModif);
		if(bModif && GetFirstViewPosition())
			CIConfigEvents::m_ConfigEvents.FireDialogChange(this);
	}
	int  CreateIndirectW(CString &str)
	{
		trapCrInd.Swap();
		int ret=CreateIndirect(str);
		trapCrInd.Swap();
		return ret;
	}
	CCtrlHolder* CreatePermanentControlW(CRect const &rc, CType const &type,char const *str1,char const *str2,long lPar1,int iPar1,int iPar2, int iPar3)
	{
		trapCrPerm.Swap();
		CCtrlHolder* pRet=(this->*(CCtrlHolder*(CEditDocWrap::*)
			(CRect const &,CType const &,char const *,char const *,long,int,int,int))trapCrPerm.m_pFuncTrapped.pClassFunc)
			(rc,type,str1,str2,lPar1,iPar1,iPar2,iPar3);
		trapCrPerm.Swap();
		return pRet;
	}
	CCtrlHolder* CreateStaticTitleW(char const * str1,CRect const &rc)
	{
		trapCrStatic.Swap();
		CCtrlHolder* pRet=(this->*(CCtrlHolder*(CEditDocWrap::*)(char const *,CRect const&))trapCrStatic.m_pFuncTrapped.pClassFunc)(str1,rc);
		trapCrStatic.Swap();
		return pRet;
	}
	void DefineFixedControlW(CType const &type, char const * str1, char const *str2, long lPar, int iPar)
	{
		trapDefFixed.Swap();
		(this->*(void (CEditDocWrap::*)(CType const &, char const * , char const *, long , int ))
			trapDefFixed.m_pFuncTrapped.pClassFunc)
			(type, str1, str2, lPar, iPar);
		trapDefFixed.Swap();
	}
	void DefineNewControlW(CType const &type,char const *str1,char const *str2,long lPar, int iPar1, int iPar2, int iPar3)
	{
		trapDefNew.Swap();
		(this->*(void (CEditDocWrap::*)(CType const &,char const *,char const *,long , int , int , int ))
			trapDefNew.m_pFuncTrapped.pClassFunc)
			(type,str1,str2,lPar, iPar1, iPar2, iPar3);
		trapDefNew.Swap();
	}
};

CIDialogDoc::~CIDialogDoc()
{
}

void CIDialogDoc::Init()
{
	CObject* pObj=CICfgDocuments::m_pUsefulRTC[2]->CreateObject();
	trapModified.SetTrap(pObj,0x64/4,(AFX_PMSG)&CEditDocWrap::SetModifiedFlagW);
	delete pObj;
	/*
	trapCrStatic.SetTrap("editr.dll","?CreateStaticTitle@CEditDoc@@IAEPAVCCtrlHolder@@PBDABVCRect@@@Z",(AFX_PMSG)&CEditDocWrap::CreateStaticTitleW);
	trapDefFixed.SetTrap("editr.dll","?DefineFixedControl@CEditDoc@@AAEXABVCType@@PBD1JH@Z",(AFX_PMSG)&CEditDocWrap::DefineFixedControlW);
	trapCrPerm.SetTrap("editr.dll","?CreatePermanentControl@CEditDoc@@AAEPAVCCtrlHolder@@ABVCRect@@ABVCType@@PBD2JHHH@Z",(AFX_PMSG)&CEditDocWrap::CreatePermanentControlW);
	trapDefNew.SetTrap("editr.dll","?DefineNewControl@CEditDoc@@AAEXABVCType@@PBD1JHHH@Z",(AFX_PMSG)&CEditDocWrap::DefineNewControlW);
	trapCrInd.SetTrap("editr.dll","?CreateIndirect@CEditDoc@@QAEHAAVCString@@@Z",(AFX_PMSG)&CEditDocWrap::CreateIndirectW);
	*/
}


class CFakeParam
{
public:
	virtual void f1();
	virtual void f2();
	virtual int GetCtrlType();
	virtual void f3(CString& t);
};

class CCtrlHolder :public CObject		// Размер класса определен точно
{
public:
	//virtual hz()=0;		//0 - pVtable
	CRect m_rect;		//1-4
	DWORD m_param1;		//5
	CType m_type;
	CString m_formul;	//9
	CString m_strID;	//10
	CString m_title;	//11
	DWORD m_idForm;		//12
	DWORD m_mainFlags;	//13
	DWORD m_param2;		//14
	CString m_strDop;	//15
	CString m_tooltip;	//16
	CString m_strHelp;	//17
	LOGFONT m_font;		//18-32
	DWORD m_param3;		//33
	DWORD m_fontColor;		//34
	CPictureHolder7 m_picture;	//35
	DWORD m_param4;	//39
	DWORD m_pictID;	//40
	DWORD m_dopFlags;	//41
	DWORD m_param6;	//42
	DWORD m_hotKey;	//43	(LOWORD=vk code, HIWORD=modif (0x3 & 4-Shift, 8 - Ctrl, 16 - Alt)
	CString	m_layer;	//44
	DWORD m_param8;	//45
	DWORD m_param9;	//46
	DWORD m_param10;	//47
	CFakeParam* m_ctrlType;	//48 192

	virtual void f14()=0;
	virtual void f18()=0;
	virtual void f1C()=0;
	virtual void f20()=0;
	virtual void f24()=0;
	virtual void f28()=0;
	virtual void f2C()=0;
	virtual void f30()=0;
	virtual void f34()=0;
	virtual void f38()=0;
	virtual void f3C()=0;
	virtual void f40()=0;
	virtual void f44()=0;
	virtual LPCTSTR GetTitle()=0;
	virtual void f4C()=0;
	virtual void f50()=0;
	virtual void f54()=0;
	virtual void f58()=0;
	virtual void f5C()=0;
	virtual LPCTSTR GetIDName()=0;
	virtual LPCTSTR GetFormul()=0;
	virtual void f68()=0;
	virtual void f6C()=0;
	virtual void f70()=0;
	virtual void f74()=0;
	virtual void f78()=0;
};

class CBrowseHolder:public CCtrlHolder
{
public:
	CObject	m_obj1;			// 196 C4
	DWORD	m_obj1buf[4];
	CPtrArray m_ptrArr;
	DWORD m_buf[4];
};

enum ctrlTypes{
	ctText		=1,
	ctField		=2,
	ctButton	=3,
	ctFrame		=4,
	ctColumn	=5,
	ctMultiColumn=7,
	ctRefTree	=8,
	ctColumnText=9,
	ctPicture	=10,
	ctListBox	=11,
	ctCheck		=12,
	ctRadio		=13,
	ctComboBox	=14,
	ctTable		=15,
};

STDMETHODIMP CIDialogDoc::SaveToFile(BSTR FileName,VARIANT_BOOL *bSucces)
{
	if(IsBadWritePtr(bSucces,sizeof(*bSucces)))
		return E_POINTER;
	HRESULT ret=E_FAIL;
	try{
		CFile file((LPCTSTR)_bstr_t(FileName),CFile::modeWrite|CFile::modeCreate);
		CString stream;
		if(GetStream(stream))
		{
			file.Write((LPCTSTR)stream,stream.GetLength());
			ret=S_OK;
		}
	}
	catch(CFileException* pE)
	{
		char buf[1000];
		pE->GetErrorMessage(buf,1000);
		SetError(E_FAIL,"Файловая ошибка: %s",buf);
		pE->Delete();
	}
	catch(...)
	{
	}
	return ret;
}

STDMETHODIMP CIDialogDoc::LoadFromFile(BSTR FileName,VARIANT_BOOL *bSucces)
{
	if(IsBadWritePtr(bSucces,sizeof(*bSucces)))
		return E_POINTER;
	*bSucces=0;
	try{
		// Считываем файл в строку
		CFile file((LPCTSTR)_bstr_t(FileName),CFile::modeRead);
		CString stream;
		DWORD size=file.GetLength();
		file.Read(stream.GetBuffer(size),size);
		stream.ReleaseBuffer();
		if(LoadStream(stream))
			*bSucces=-1;
	}
	catch(CFileException* pE)
	{
		char buf[1000];
		pE->GetErrorMessage(buf,1000);
		SetError(E_FAIL,"Файловая ошибка: %s",buf);
		pE->Delete();
	}
	catch(...)
	{
	}
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_Stream(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString stream;
	if(GetStream(stream))
	{
		*pVal=stream.AllocSysString();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CIDialogDoc::put_Stream(BSTR newVal)
{
	CEditDoc* pDoc=GetDocument();
	CString stream=(LPCTSTR)_bstr_t(newVal);
	if(LoadStream(stream))
		return S_OK;
	return E_FAIL;
}

bool CIDialogDoc::GetStream(CString& stream)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return false;
	CMemFile mFile;
	CArchive ar(&mFile,CArchive::store);
	pDoc->Serialize(ar);

	mFile.SeekToBegin();
	CArchive load(&mFile,CArchive::load);
	load>>stream;
	return true;
}

bool CIDialogDoc::LoadStream(CString& stream)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return false;
	bool ret=false;

	CMemFile mFile;
	CArchive load(&mFile,CArchive::store);
	load<<stream;
	load.Close();
	mFile.SeekToBegin();

	CArchive ar(&mFile,CArchive::load);
	// Сохраним старую версию
	CMemFile oldFile;
	CArchive oldAr(&oldFile,CArchive::store);
	pDoc->Serialize(oldAr);
	// Очистим диалог
	pDoc->ClearList();
	for(int i=pDoc->GetLayersCount();i>0;)
		pDoc->DeleteLayer(--i,NULL);

	// Попытаемся загрузить новую версию
	int s=((CEditDocWrap*)pDoc)->InitFA(ar);
	if(!s)
	{
		oldAr.Close();
		oldFile.SeekToBegin();
		CArchive newArr(&oldFile,CArchive::load);
		s=((CEditDocWrap*)pDoc)->InitFA(newArr);
		SetError(E_FAIL,"Не удалось загрузить диалог. Возможно неправильный формат потока");
	}
	else
	{
		pDoc->SetModifiedFlag(TRUE);
		ret=true;
	}
	pDoc->UpdateAllViews(NULL);
	ar.Close();
	mFile.Close();
	return ret;
}
/*
Смещения полей в CEditDoc
total = 0x1F4 (500)
start at 0x54 (84)
0x54 -	CString m_FileName;
0x58 -	CString m_Profile;
0x5C -	CString m_strToolTip;
0x60 -	CString m_strHelpString;
0x64 -	CString m_strDependParam;
0x68 -	int m_isUseAutoFont;
0x6C -	BOOL m_iTollBarEnable;
0x70 -	BOOL m_iAutoSize;
0x74 -	BOOL m_iPaletePopup;
		char buf1[0x4];
0x7C -	CGalleryHolder* m_pGalery;
0x80 -	CDrawMaster *m_pDrawMaster;
0x84 -	CEditMaster *m_pEditMaster;
0x88 -	CPtrArray m_ctrlArray (CCtrlType*)
0x9C -	CExtListSupport m_extList		(0x28 (40))
0xC4 -	CExtListSupport m_extDocList;	(0x28 (40))
0xEC -	CPtrArray m_extCtrlArray (CCtrlHolder*)
0x100 -	CPtrArray m_docCtrlArray; (CBrowserSupport *)
0x114 -	CPtrList m_selList	(CCtrlHolder*)
0x130 -	CPtrList m_objList (CCtrlHolder*)
0x14C -	CMakeUndo *m_pUndo;
0x150 -	CMakeUndo *m_pRedo;
0x154 -	enum  EdDocType m_TemplateType;
0x158 -	CBrowseHolder *m_brwsHolder;
0x15C -	DWORD m_NextID;
0x160 -	CStringArray m_layers
0x174 -	CDWordArray m_layersState;
0x188 -	int m_iActiveLayer;
		char buf2[0x6c];
0x1F4 - long m_lDependParam;

0x200 - long m_AutoLayout
*/

STDMETHODIMP CIDialogDoc::AddLayer(BSTR Name, VARIANT_BOOL Visible)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CString lName=Name;
	if(pDoc->FindLayer(lName)<0)
		pDoc->AddLayer(lName,Visible==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_LayerCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->GetLayersCount();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_LayerName(long idx, BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	if(idx<0 || idx>=pDoc->GetLayersCount())
		return SetError(E_INVALIDARG,"Недопустимый номер слоя: %i",idx);
	*pVal=pDoc->GetLayer(idx).AllocSysString();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_LayerName(long idx, BSTR newVal)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CString lNewName=newVal;
	if(pDoc->FindLayer(lNewName)>-1)
		return SetError(E_INVALIDARG,"Слой с именем \"%s\" уже существует",lNewName);
	if(idx<0 || idx>=pDoc->GetLayersCount())
		return SetError(E_INVALIDARG,"Недопустимый номер слоя: %i",idx);
	pDoc->SetLayer(idx,lNewName,pDoc->GetLayerState(idx));
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_LayerVisible(BSTR Name, VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int idx=pDoc->FindLayer(CString(Name));
	if(idx<0)
		return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",Name);
	*pVal=pDoc->GetLayerState(idx)?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_LayerVisible(BSTR Name, VARIANT_BOOL newVal)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int idx=pDoc->FindLayer(CString(Name));
	if(idx<0)
		return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",Name);
	pDoc->SetLayer(idx,pDoc->GetLayer(idx),newVal==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_ActiveLayer(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->GetLayer(pDoc->GetActiveLayer()).AllocSysString();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_ActiveLayer(BSTR newVal)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int idx=pDoc->FindLayer(CString(newVal));
	if(idx<0)
		return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",newVal);
	pDoc->SetActiveLayer(idx);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::DeleteLayer(BSTR Name, BSTR CopyControlsTo)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int idx=pDoc->FindLayer(CString(Name));
	if(idx<0)
		return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",Name);
	LPCTSTR lpCopy=NULL;
	CString copyto=CopyControlsTo;
	if(!copyto.IsEmpty())
	{
		int idx1=pDoc->FindLayer(copyto);
		if(idx1<0)
			return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",CopyControlsTo);
		lpCopy=copyto;
	}
	pDoc->DeleteLayer(idx,lpCopy);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::MoveLayer(BSTR From,  VARIANT_BOOL Down)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int idx=pDoc->FindLayer(CString(From));
	if(idx<0)
		return SetError(E_INVALIDARG,"Слой с именем \"%ls\" не найден",From);
	pDoc->MoveLayer(idx,Down==VARIANT_TRUE);
	pDoc->SetModifiedFlag(TRUE);
	return S_OK;
}

static CCtrlHolder* FindControl(CIDialogDoc* pObj, CEditDoc* pDoc, DWORD idx)
{
	CCtrlHolder* pCtrl=NULL;
	if(idx<pDoc->m_objList.GetCount())
	{
		POSITION pos=pDoc->m_objList.GetHeadPosition();
		for(DWORD i=0;i<=idx;i++)
			pCtrl=(CCtrlHolder*)pDoc->m_objList.GetNext(pos);
	}
	else
		pObj->SetError(E_FAIL,"Недопустимый индекс контрола: %i",idx);
	return pCtrl;
}

STDMETHODIMP CIDialogDoc::get_ctrlCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->m_objList.GetCount();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_ctrlType(long idx, BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CCtrlHolder* pCtrl=FindControl(this,pDoc,idx);
	if(!pCtrl)
		return E_FAIL;
	static const LPCTSTR types[]=
	{
		"",				//0
		"Текст",		//1
		"Реквизит",		//2
		"Кнопка",		//3
		"Рамка",		//4
		"Колонка",		//5
		"",				//6
		"ТабличнаяЧасть",//7
		"Дерево",		//8
		"Колонка_текст",//9
		"Картинка",		//10
		"Список",		//11
		"Флажок",		//12
		"Переключатель",//13
		"ПолеСписок",	//14
		"ТаблицаЗначений"	//15
	};
	CString ret=types[pCtrl->m_ctrlType->GetCtrlType()];
	*pVal=ret.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_Selection(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	int i=0;
	CString ret,str;
	for(POSITION pos=pDoc->m_objList.GetHeadPosition();pos;i++)
	{
		CCtrlHolder* pCtrl=(CCtrlHolder*)pDoc->m_objList.GetNext(pos);
		if(pCtrl->m_param1<3)
		{
			str.Format("%i,",i);
			ret+=str;
		}
	}
	if(!ret.IsEmpty())
		ret.GetBufferSetLength(ret.GetLength()-1);
	*pVal=ret.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_Selection(BSTR newVal)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	for(POSITION pos=pDoc->m_selList.GetHeadPosition();pos;)
	{
		CCtrlHolder* pCtrl=(CCtrlHolder*)pDoc->m_selList.GetNext(pos);
		pCtrl->m_param1=3;
	}
	pDoc->m_selList.RemoveAll();
	CString sel=newVal;
	LPCTSTR ptr=sel;
	int iSel=1;
	do
	{
		while(*ptr && (*ptr<'0' || *ptr>'9'))
			ptr++;
		if(!*ptr)
			break;
		DWORD idx=0;
		while(*ptr>='0' && *ptr<='9')
			idx=idx*10+*ptr++-'0';
		CCtrlHolder* pCtrl=FindControl(this,pDoc,idx);
		if(pCtrl && pCtrl->m_param1==3)
		{
			pCtrl->m_param1=iSel;
			iSel=2;
			pDoc->m_selList.AddTail(pCtrl);
			if(!pDoc->IsLayerVisible(pCtrl->m_layer))
				pDoc->SetLayer(pDoc->FindLayer(pCtrl->m_layer),pCtrl->m_layer,1);
		}
	}while(1);
	pDoc->UpdateAllViews(NULL,0,0);
	return S_OK;
}





STDMETHODIMP CIDialogDoc::ctrlUID(long idx, long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(pDoc)
	{
		CCtrlHolder* pCtrl=FindControl(this,pDoc,idx);
		if(pCtrl)
		{
			*pVal=(long)pCtrl;
			return S_OK;
		}
	}
	return E_FAIL;
}

STDMETHODIMP CIDialogDoc::ctrlIdx(long UID, long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	long i=0, fnd=-1;
	for(POSITION pos=pDoc->m_objList.GetHeadPosition();pos;i++)
	{
		if(UID==(long)pDoc->m_objList.GetNext(pos))
		{
			fnd=i;
			break;
		}
	}
	*pVal=fnd;
	return S_OK;
}

static CIDialogDoc* pProcessingDoc=NULL;

static long GetCtrlLeft(CCtrlHolder* pCtrl)
{
	return pCtrl->m_rect.left;
}

static void SetCtrlLeft(CCtrlHolder* pCtrl, long newVal)
{
	int w=pCtrl->m_rect.Width();
	pCtrl->m_rect.left=newVal;
	pCtrl->m_rect.right=newVal+w;
}

static long GetCtrlTop(CCtrlHolder* pCtrl)
{
	return pCtrl->m_rect.top;
}

static void SetCtrlTop(CCtrlHolder* pCtrl, long newVal)
{
	int h=pCtrl->m_rect.Height();
	pCtrl->m_rect.top=newVal;
	pCtrl->m_rect.bottom=newVal+h;
}

static long GetCtrlWidth(CCtrlHolder* pCtrl)
{
	return pCtrl->m_rect.Width();
}

static void SetCtrlWidth(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_rect.right=pCtrl->m_rect.left+newVal;
}

static long GetCtrlHeight(CCtrlHolder* pCtrl)
{
	return pCtrl->m_rect.Height();
}

static void SetCtrlHeight(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_rect.bottom=pCtrl->m_rect.top+newVal;
}

static CString GetCtrlID(CCtrlHolder* pCtrl)
{
	return pCtrl->m_strID;
}

static void SetCtrlID(CCtrlHolder* pCtrl, CString& newVal)
{
	CEditDoc* pDoc=pProcessingDoc->GetDocument();
	if(!pDoc->ValidateCodeName(newVal,pCtrl))
	{
		pProcessingDoc->SetError(E_FAIL,"Плохой идентификатор: %s",newVal);
		pProcessingDoc=NULL;
	}
	else
		pCtrl->m_strID=newVal;
}

static CString GetCtrlTitle(CCtrlHolder* pCtrl)
{
	return pCtrl->m_title;
}

static void SetCtrlTitle(CCtrlHolder* pCtrl, CString& newVal)
{
	pCtrl->m_title=newVal;
}

static CString GetCtrlFormula(CCtrlHolder* pCtrl)
{
	return pCtrl->m_formul;
}

static void SetCtrlFormula(CCtrlHolder* pCtrl, CString& newVal)
{
	pCtrl->m_formul=newVal;
}

static CString GetCtrlToolTip(CCtrlHolder* pCtrl)
{
	return pCtrl->m_tooltip;
}

static SetCtrlToolTip(CCtrlHolder* pCtrl, CString& newVal)
{
	pCtrl->m_tooltip=newVal;
}

static CString GetCtrlHelp(CCtrlHolder* pCtrl)
{
	return pCtrl->m_strHelp;
}

static void SetCtrlHelp(CCtrlHolder* pCtrl, CString& newVal)
{
	pCtrl->m_strHelp=newVal;
}

static CString GetCtrlLayer(CCtrlHolder* pCtrl)
{
	return pCtrl->m_layer;
}

static void SetCtrlLayer(CCtrlHolder* pCtrl, CString& newVal)
{
	CEditDoc* pDoc=pProcessingDoc->GetDocument();
	if(pDoc->FindLayer(newVal)<0)
	{
		pProcessingDoc->SetError(E_FAIL,"Недопустимое имя слоя: %s",newVal);
		pProcessingDoc=NULL;
	}
	else
		pCtrl->m_layer=newVal;
}

static long GetCtrlMetaID(CCtrlHolder* pCtrl)
{
	return pCtrl->m_idForm;
}

static long GetCtrlFlags(CCtrlHolder* pCtrl)
{
	return pCtrl->m_mainFlags;
}

static void SetCtrlFlags(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_mainFlags=newVal;
}

static long GetCtrlFlagsEx(CCtrlHolder* pCtrl)
{
	return pCtrl->m_dopFlags;
}

static void SetCtrlFlagsEx(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_dopFlags=newVal;
}

static CString GetCtrlLinkWith(CCtrlHolder* pCtrl)
{
	return pCtrl->m_strDop;
}

static void SetCtrlLinkWith(CCtrlHolder* pCtrl,CString& newVal)
{
	pCtrl->m_strDop=newVal;
}

static CString GetCtrlFontName(CCtrlHolder* pCtrl)
{
	return pCtrl->m_font.lfFaceName;
}

static void SetCtrlFontName(CCtrlHolder*pCtrl, CString& newVal)
{
	strncpy(pCtrl->m_font.lfFaceName,newVal,sizeof(pCtrl->m_font.lfFaceName));
}

static long GetCtrlFontSize(CCtrlHolder* pCtrl)
{
	return -pCtrl->m_font.lfHeight;
}

static void SetCtrlFontSize(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_font.lfHeight=-newVal;
}

static long GetCtrlFontWeight(CCtrlHolder* pCtrl)
{
	return pCtrl->m_font.lfWeight;
}

static void SetCtrlFontWeight(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_font.lfWeight=newVal;
}

static long GetCtrlFontItalic(CCtrlHolder* pCtrl)
{
	return pCtrl->m_font.lfItalic;
}

static void SetCtrlFontItalic(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_font.lfItalic=newVal;
}

static long GetCtrlFontUnderline(CCtrlHolder* pCtrl)
{
	return pCtrl->m_font.lfUnderline;
}

static void SetCtrlFontUnderline(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_font.lfUnderline=newVal;
}

static long GetCtrlFontColor(CCtrlHolder* pCtrl)
{
	return pCtrl->m_fontColor;
}

static void SetCtrlFontColor(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_fontColor=newVal;
}

static long GetCtrlHotKey(CCtrlHolder* pCtrl)
{
	return pCtrl->m_hotKey;
}

static void SetCtrlHotKey(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_hotKey=newVal;
}

static long GetCtrlPictID(CCtrlHolder* pCtrl)
{
	return pCtrl->m_pictID;
}

static void SetCtrlPictID(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_pictID=newVal;
	if(newVal)
	{
		CGalleryHolder* pGal=CGalleryHolder::GetGallery();
		int pos=pGal->ID2Position(newVal);
		if(pos>=0)
		{
			pCtrl->m_picture.Copy(pGal->GetPicture(newVal));
			return;
		}
		pCtrl->m_pictID=0;
	}
	pCtrl->m_picture.CreateEmpty();
}

static long GetCtrlPictMode(CCtrlHolder* pCtrl)
{
	return pCtrl->m_picture.m_mode;
}

static void SetCtrlPictMode(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_picture.m_mode=newVal;
}

static long GetCtrlBkColor(CCtrlHolder* pCtrl)
{
	return pCtrl->m_pictID;
}

static void SetCtrlBkColor(CCtrlHolder* pCtrl, long newVal)
{
	pCtrl->m_hotKey=newVal;
}

static const struct
{
	bool IsNumeric;
	void *pSetFn;
	void *pGetFn;
}PropsParam[]=
{
	{true,	SetCtrlLeft,			GetCtrlLeft},			// cpLeft
	{true,	SetCtrlTop,				GetCtrlTop},			// cpTop
	{true,	SetCtrlWidth,			GetCtrlWidth},			// cpWidth
	{true,	SetCtrlHeight,			GetCtrlHeight},			// cpHeight
	{false,	SetCtrlID,				GetCtrlID},				// cpStrID
	{false,	SetCtrlTitle,			GetCtrlTitle},			// cpTitle
	{false,	SetCtrlFormula,			GetCtrlFormula},		// cpFormul
	{false,	SetCtrlToolTip,			GetCtrlToolTip},		// cpToolTip
	{false,	SetCtrlHelp,			GetCtrlHelp},			// cpHelpString
	{false,	SetCtrlLayer,			GetCtrlLayer},			// cpLayer
	{true,	NULL,					GetCtrlMetaID},			// cpMetaID
	{true,	SetCtrlFlags,			GetCtrlFlags},			// cpFlags
	{true,	SetCtrlFlagsEx,			GetCtrlFlagsEx},		// cpFlagsEx
	{false,	SetCtrlLinkWith,		GetCtrlLinkWith},		// cpLinkWith
	{false,	SetCtrlFontName,		GetCtrlFontName},		// cpFontName
	{true,	SetCtrlFontSize,		GetCtrlFontSize},		// cpFontSize
	{true,	SetCtrlFontWeight,		GetCtrlFontWeight},		// cpFontWeight=17,	
	{true,	SetCtrlFontItalic,		GetCtrlFontItalic},		// cpFontItalic=18,	
	{true,	SetCtrlFontUnderline,	GetCtrlFontUnderline},	// cpFontUnderline=19,	
	{true,	SetCtrlFontColor,		GetCtrlFontColor},		// cpFontColor
	{true,	SetCtrlHotKey,			GetCtrlHotKey},			// cpHotKey
	{true,	SetCtrlPictID,			GetCtrlPictID},			// cpPictID
	{true,	SetCtrlPictMode,		GetCtrlPictMode},		// cpPictMode
};

STDMETHODIMP CIDialogDoc::get_ctrlProp(long ctrlIdx, CtrlPropsIdx propIdx, VARIANT *pVal)
{
	if(sizeof(PropsParam)/sizeof(PropsParam[0])<=(DWORD)propIdx)
		return SetError(E_INVALIDARG,"Недопустимый номер свойства");
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CCtrlHolder* pCtrl=FindControl(this,pDoc,ctrlIdx);
	if(!pCtrl)
		return E_FAIL;
	if(PropsParam[propIdx].IsNumeric)
	{
		pVal->vt=VT_I4;
		pVal->lVal=((long (*)(CCtrlHolder*))PropsParam[propIdx].pGetFn)(pCtrl);
	}
	else
	{
		pVal->vt=VT_BSTR;
		pVal->bstrVal=((CString (*)(CCtrlHolder*))PropsParam[propIdx].pGetFn)(pCtrl).AllocSysString();
	}
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_ctrlProp(long ctrlIdx, CtrlPropsIdx propIdx, VARIANT newVal)
{
	if(sizeof(PropsParam)/sizeof(PropsParam[0])<=(DWORD)propIdx)
		return SetError(E_INVALIDARG,"Недопустимый номер свойства");
	if(!PropsParam[propIdx].pSetFn)
		return SetError(E_FAIL,"Свойство только для чтения");
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CCtrlHolder* pCtrl=FindControl(this,pDoc,ctrlIdx);
	if(!pCtrl)
		return E_FAIL;
	pProcessingDoc=this;
	if(PropsParam[propIdx].IsNumeric)
	{
		if(newVal.vt!=VT_I4)
			VariantChangeType(&newVal,&newVal,0,VT_I4);
		if(newVal.vt!=VT_I4)
			return SetError(E_FAIL,"Неправильный тип значения параметра");
		((void (*)(CCtrlHolder*,long))PropsParam[propIdx].pSetFn)(pCtrl,newVal.lVal);
	}
	else
	{
		if(newVal.vt!=VT_BSTR)
			VariantChangeType(&newVal,&newVal,0,VT_BSTR);
		if(newVal.vt!=VT_BSTR)
			return SetError(E_FAIL,"Неправильный тип значения параметра");
		CString val=newVal.bstrVal;
		((void(*)(CCtrlHolder*,CString&))PropsParam[propIdx].pSetFn)(pCtrl,val);
	}
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(0,0,0);
	return pProcessingDoc?S_OK:E_FAIL;
}

static long GetFormWidth(CEditDoc* pDoc)
{
	return pDoc->m_rect.right;
}

static void SetFormWidth(CEditDoc* pDoc, long newVal)
{
	pDoc->m_rect.right=newVal;
}

static long GetFormHeight(CEditDoc* pDoc)
{
	return pDoc->m_rect.bottom;
}

static void SetFormHeight(CEditDoc* pDoc, long newVal)
{
	pDoc->m_rect.bottom=newVal;
}

static long GetFormToolBar(CEditDoc* pDoc)
{
	return pDoc->m_iTollBarEnable;
}

static void SetFormToolBar(CEditDoc* pDoc, long newVal)
{
	pDoc->m_iTollBarEnable=newVal!=0;
}

static long GetFormAutoSize(CEditDoc* pDoc)
{
	return pDoc->m_iAutoSize;
}

static void SetFormAutoSize(CEditDoc* pDoc, long newVal)
{
	pDoc->m_iAutoSize=newVal!=0;
}

static long GetFormAutoLayout(CEditDoc* pDoc)
{
	return pDoc->m_iAutoLayout;
}

static void SetFormAutoLayout(CEditDoc* pDoc, long newVal)
{
	pDoc->m_iAutoLayout=newVal!=0;
}

static long GetFormUseAutoFont(CEditDoc* pDoc)
{
	return pDoc->m_isUseAutoFont;
}

static void SetFormUseAutoFont(CEditDoc* pDoc, long newVal)
{
	pDoc->m_isUseAutoFont=newVal!=NULL;
}

static CString GetFormFontName(CEditDoc* pDoc)
{
	return pDoc->m_font.lfFaceName;
}

static void SetFormFontName(CEditDoc* pDoc, CString& newVal)
{
	strncpy(pDoc->m_font.lfFaceName,newVal,sizeof(pDoc->m_font.lfFaceName));
}

static long GetFormFontSize(CEditDoc* pDoc)
{
	return -pDoc->m_font.lfHeight;
}

static void SetFormFontSize(CEditDoc* pDoc, long newVal)
{
	pDoc->m_font.lfHeight=-newVal;
}

static long GetFormFontWeight(CEditDoc* pDoc)
{
	return pDoc->m_font.lfWeight;
}

static void SetFormFontWeight(CEditDoc* pDoc, long newVal)
{
	pDoc->m_font.lfWeight=newVal;
}

static long GetFormFontItalic(CEditDoc* pDoc)
{
	return pDoc->m_font.lfItalic;
}

static void SetFormFontItalic(CEditDoc* pDoc, long newVal)
{
	pDoc->m_font.lfItalic=newVal;
}

static long GetFormFontUnderline(CEditDoc* pDoc)
{
	return pDoc->m_font.lfUnderline;
}

static void SetFormFontUnderline(CEditDoc* pDoc, long newVal)
{
	pDoc->m_font.lfUnderline=newVal;
}

static CString GetFormTitle(CEditDoc* pDoc)
{
	return pDoc->m_title;
}

static void SetFormTitle(CEditDoc* pDoc, CString& newVal)
{
	pDoc->m_title=newVal;
}

static long GetFormFlags(CEditDoc* pDoc)
{
	return pDoc->m_flags;
}

static void SetFormFlags(CEditDoc* pDoc, long newVal)
{
	pDoc->m_flags=newVal;
}

static long GetFormPictID(CEditDoc* pDoc)
{
	return pDoc->m_pictID;
}

static void SetFormPictID(CEditDoc* pDoc, long newVal)
{
	pDoc->m_pictID=newVal;
	if(newVal)
	{
		CGalleryHolder* pGal=CGalleryHolder::GetGallery();
		int pos=pGal->ID2Position(newVal);
		if(pos>=0)
		{
			pDoc->m_picture.Copy(pGal->GetPicture(newVal));
			return;
		}
	}
	pDoc->m_picture.CreateEmpty();
}

static long GetFormPictMode(CEditDoc* pDoc)
{
	return pDoc->m_picture.m_mode;
}

static void SetFormPictMode(CEditDoc* pDoc, long newVal)
{
	pDoc->m_picture.m_mode=newVal;
}

static long GetFormBkColor(CEditDoc* pDoc)
{
	return pDoc->m_bkColor;
}

static void SetFormBkColor(CEditDoc* pDoc, long newVal)
{
	pDoc->m_bkColor=newVal;
}

static const struct
{
	bool IsNumeric;
	void* pSetFn;
	void* pGetFn;
}FormParams[]=
{
	{true,SetFormWidth,			GetFormWidth},				// fpWidth
	{true,SetFormHeight,		GetFormHeight},				// fpHeight
	{true,SetFormToolBar,		GetFormToolBar},			// fpToolBar
	{true,SetFormAutoSize,		GetFormAutoSize},			// fpAutoSize
	{true,SetFormAutoLayout,	GetFormAutoLayout},			// fpAutoLayout
	{true,SetFormUseAutoFont,	GetFormUseAutoFont},		// fpUseAutoFont
	{false,SetFormFontName,		GetFormFontName},			// fpFontName
	{true,SetFormFontSize,		GetFormFontSize},			// fpFontSize
	{true,SetFormFontWeight,	GetFormFontWeight},			// fpFontWeight
	{true,SetFormFontItalic,	GetFormFontItalic},			// fpFontItalic
	{true,SetFormFontUnderline, GetFormFontUnderline},		// fpFontUnderline
	{false,SetFormTitle,		GetFormTitle},				// fpTitle
	{true,SetFormFlags,			GetFormFlags},				// fpFlags
	{true,SetFormPictID,		GetFormPictID},				// fpFlags
	{true,SetFormPictMode,		GetFormPictMode},			// fpFlags
	{true,SetFormBkColor,		GetFormBkColor},			// fpFlags
};

STDMETHODIMP CIDialogDoc::get_formProp(FormPropsIdx formPropIdx, VARIANT *pVal)
{
	if(sizeof(FormParams)/sizeof(FormParams[0])<=(DWORD)formPropIdx)
		return SetError(E_INVALIDARG,"Недопустимый номер свойства");
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	if(FormParams[formPropIdx].IsNumeric)
	{
		pVal->vt=VT_I4;
		pVal->lVal=((long (*)(CEditDoc*))FormParams[formPropIdx].pGetFn)(pDoc);
	}
	else
	{
		pVal->vt=VT_BSTR;
		pVal->bstrVal=((CString (*)(CEditDoc*))FormParams[formPropIdx].pGetFn)(pDoc).AllocSysString();
	}
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_formProp(FormPropsIdx formPropIdx, VARIANT newVal)
{
	if(sizeof(FormParams)/sizeof(FormParams[0])<=(DWORD)formPropIdx)
		return SetError(E_INVALIDARG,"Недопустимый номер свойства");
	if(!FormParams[formPropIdx].pSetFn)
		return SetError(E_FAIL,"Свойство только для чтения");
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	if(FormParams[formPropIdx].IsNumeric)
	{
		if(newVal.vt!=VT_I4)
			VariantChangeType(&newVal,&newVal,0,VT_I4);
		if(newVal.vt!=VT_I4)
			return SetError(E_FAIL,"Неправильный тип значения параметра");
		((void (*)(CEditDoc*,long))FormParams[formPropIdx].pSetFn)(pDoc,newVal.lVal);
	}
	else
	{
		if(newVal.vt!=VT_BSTR)
			VariantChangeType(&newVal,&newVal,0,VT_BSTR);
		if(newVal.vt!=VT_BSTR)
			return SetError(E_FAIL,"Неправильный тип значения параметра");
		CString val=newVal.bstrVal;
		((void(*)(CEditDoc*,CString&))FormParams[formPropIdx].pSetFn)(pDoc,val);
	}
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(0,0,0);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::get_ctrlType1C(long idx, IType1C **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CCtrlHolder* pCtrl=FindControl(this,pDoc,idx);
	if(!pCtrl)
		return E_FAIL;
	*pVal=new CIType1C(pCtrl->m_type);
	return S_OK;
}

STDMETHODIMP CIDialogDoc::put_ctrlType1C(long idx, IType1C *newVal)
{
	CEditDoc* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CCtrlHolder* pCtrl=FindControl(this,pDoc,idx);
	if(!pCtrl)
		return E_FAIL;
	CType& typeNew=((CIType1C*)newVal)->m_type;
	if(!typeNew.IsValid())
		return SetError(E_FAIL,"Недопустимый тип данных.");
	pCtrl->m_type=typeNew;
	return S_OK;
}
