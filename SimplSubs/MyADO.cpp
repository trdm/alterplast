// MyADO.cpp: implementation of the CMyADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyADO.h"
#include "plugin.h"
#include "InputStrDlg.h"
#include "SubsView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//using namespace ADODB;


 


CString strErr;


extern CConfigService* pSvc;
//CMapStringToString CMyADO::m_SubToSub = NULL;	// отмеченые подсистемы для фильтра.....

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyADO::CMyADO(){
	m_iVersion = 1;
	m_StateSubs = 0;
	m_LastIDSubs = 0;
}

CMyADO::~CMyADO(){
	SaveOrLoad(true);
}

bool CMyADO::Init()
{
	m_sIBDir = pSvc->IBDir();
	m_sFileDataName = m_sIBDir + "SubData.dat";
	SaveOrLoad(false);
	m_sFileSettingsName = pSvc->BinDir()+"config\\SimplSubs.ini";
	SaveOrLoadSettings(false);


	
	//m_Settings.EnableDDGroup
	return true;
}


bool CMyADO::SubsInsertNew(int nID)
{
	CInputStrDlg pInpDlg;
	if (pInpDlg.DoModal() != IDOK){ 
		return false;
	}
	if (pInpDlg.m_StringInput.IsEmpty()){ 
		return false;
	}
	bool bFindSub = false;
	int nMaxId = m_LastIDSubs+1;
	CString key;

	SubsStorage* strSS;
	if (m_SubSelfs.GetCount() >0 ){ 
		POSITION pos = m_SubSelfs.GetStartPosition();
		while (pos != NULL)	{
			m_SubSelfs.GetNextAssoc( pos, key, (CObject*&)strSS );
			nMaxId = strSS->m_ID>nMaxId ? strSS->m_ID : nMaxId;
			if (strSS->m_Name.CollateNoCase(pInpDlg.m_StringInput)==0){ 
				bFindSub = true;
				break;
			}
		}
	}
	if (!bFindSub){ 
		SubsStorage* strSSAdd = new SubsStorage;
		strSSAdd->m_ID = nMaxId;
		strSSAdd->m_Name = pInpDlg.m_StringInput;
		strSSAdd->m_Check = 0;
		strSSAdd->m_IDParent = 0;
		key.Format("%d",nMaxId);
		m_SubSelfs.SetAt(key,(CObject*&)strSSAdd);
		m_LastIDSubs = nMaxId;

	}
	SaveOrLoad(true,false);
	return true;
}

bool CMyADO::SaveOrLoad(bool bSave, bool bClearAll)
{
	CFile file;
	if (bSave){ 
		file.Open(m_sFileDataName, CFile::modeWrite | CFile::modeCreate);	// создать файл  
	} else {
		CFileStatus pFS;
		if (!CFile::GetStatus(m_sFileDataName, pFS)){ 
			return true;
		}
		file.Open(m_sFileDataName, CFile::modeRead);	// создать файл  
		//CArchive ar(&file, CArchive::load);	
	}
	if (bSave){ 
		if (m_StateSubs == 1){ 
			SaveOrLoadSubsSelection();
		}		
	}
	CArchive ar(&file, bSave?CArchive::store:CArchive::load);	

	int iSubCnt = m_SubSelfs.GetCount();		
	int iObjAndSubCnt = m_ObjToSub.GetCount();		

	if (bSave){ 
		ar << m_iVersion;
		ar << iSubCnt;
		ar << m_StateSubs;
		ar << m_LastIDSubs;
		ar << iObjAndSubCnt;
	} else {
		ar >> m_iVersion;
		ar >> iSubCnt;
		ar >> m_StateSubs;
		ar >> m_LastIDSubs;
		ar >> iObjAndSubCnt;
	}
	int i = 0;
	if (iSubCnt>0){ 

		CString key;
		if (bSave){ 
			// выгружаем данные подсистем.
			POSITION pos = m_SubSelfs.GetStartPosition();
			SubsStorage* strSS;
			while (pos != NULL)	{
				m_SubSelfs.GetNextAssoc( pos, key, (CObject*&)strSS );
				ar << strSS->m_ID;
				ar << strSS->m_Check;
				ar << strSS->m_IDParent;
				ar << strSS->m_Name;
				if (bClearAll){ 
					delete strSS;
					strSS = NULL;
				}
			}
			if (bClearAll)
				m_SubSelfs.RemoveAll();
			pos = m_ObjToSub.GetStartPosition();
			// выгружаем связку объект-подсистемы
			CString sSubsistem = "";
			while (pos != NULL)	{
				m_ObjToSub.GetNextAssoc( pos, key, sSubsistem );
				ar << key;
				ar << sSubsistem;
			}
			if (bClearAll)
				m_ObjToSub.RemoveAll();
		} else {
			int i;
			for (i = 0; i<iSubCnt; i++)	{
				SubsStorage* strSSAdd = new SubsStorage;
				ar >> strSSAdd->m_ID;
				ar >> strSSAdd->m_Check;
				ar >> strSSAdd->m_IDParent;
				ar >> strSSAdd->m_Name;
				key.Format("%d",strSSAdd->m_ID);
				m_SubSelfs.SetAt(key,(CObject*&)strSSAdd);
			}
			for (i = 0; i<iObjAndSubCnt; i++)	{
				CString sKey="", sAllSubsObj = "";
				ar >> sKey;
				ar >> sAllSubsObj;
				m_ObjToSub.SetAt(sKey, sAllSubsObj);
			}
		}
	}
	ar.Close();		// закрыть архив   
	file.Close();		// закрыть файл

	return true;
}



void CMyADO::FillListSubs(CListBox &nListBox)
{
	nListBox.ResetContent();
	POSITION pos = m_SubSelfs.GetStartPosition();
	SubsStorage* strSS;
	CString key;
	int n_iStr = 0;

	while (pos != NULL)	{
		m_SubSelfs.GetNextAssoc( pos, key, (CObject*&)strSS );
		n_iStr = nListBox.AddString(strSS->m_Name);
		nListBox.SetItemData(n_iStr,(DWORD)strSS->m_ID);
	}
}

// удаляем текущую запись.......
bool CMyADO::SubsDelete(CListBox &nListBox){
	int nData=0, nCount = nListBox.GetCount();
	CString message,key, sSubsistem;

	SubsStorage* strSS;
	int i = nListBox.GetCurSel();
	if (i != LB_ERR ){
		// типа помечено
		nData = nListBox.GetItemData(i);
		key.Format("%d",nData);
		if (m_SubSelfs.Lookup(key,(CObject*&)strSS)){ 
			message.Format("Delete:\r\n\"%s\"?",strSS->m_Name);
			if (AfxMessageBox(message,MB_YESNO) !=IDYES ){ 
				return false;
			}
			delete strSS;
			strSS = NULL;
			m_SubSelfs.RemoveKey(key);
			// Тут еще надо почистить в списке Обект-Подсистема 
			// вхождение этой подсистемы...
			message = "";
			message.Format(",%s,",key);
			POSITION pos = m_ObjToSub.GetStartPosition();
			while (pos != NULL)	{			
				m_ObjToSub.GetNextAssoc( pos, key, sSubsistem );
				if (sSubsistem.Find(message) != -1){ 
					sSubsistem.Replace(message,"");
					m_ObjToSub.SetAt(key,(LPCSTR)sSubsistem);
				}
			}
		}
	}else {
		return false;
	}
	SaveOrLoad(true,false);
	return true;
}

bool CMyADO::SubsRename(CListBox &nListBox){
	int nData=0, nCount = nListBox.GetCount();
	CString key;
	SubsStorage* strSS;
	int i = nListBox.GetCurSel();
	if (i != LB_ERR ){
		// типа помечено
		nData = nListBox.GetItemData(i);
		key.Format("%d",nData);
		if (m_SubSelfs.Lookup(key,(CObject*&)strSS)){ 
			CInputStrDlg pInpDlg;
			key = strSS->m_Name;
			pInpDlg.m_StringInput = key;
			if (pInpDlg.DoModal() != IDOK){ 
				return false;
			}
			if (pInpDlg.m_StringInput.IsEmpty()){ 
				return false;
			}
			key = pInpDlg.m_StringInput;
			key.TrimLeft();
			key.TrimRight();
			strSS->m_Name = key;
			SaveOrLoad(true,false);
			return true;
		}
	}
	return false;
}

bool CMyADO::IsStoredObj(long nWhatIsIt)
{
	switch(nWhatIsIt)
	{
	case 1: // константа
	case 3:	// справочник. получим владельца....
	case 6:	// регистр, тип
	case 9:	// документ. получим журнал
	case 11:// журнал
	case 13:// перечисление
	case 16:// отчет
	case 23:// Обработка
	case 18:// КаленДурь
	case 19:// вид расчета
	case 20:// группа расчетов
	case 22:// журнал расчетов
	case 35:// нумератор
	case 44:// нумератор
	case 30:// нумератор
	case 36:// нумератор
		break;
	default:
		return false;
		break;
	}
	return true;


}
void CMyADO::BuildMarkUpSubs(){
// выбираем все помеченные подсистемы.....
	m_SubToSub.RemoveAll();
	if (CSubsView::m_pView == NULL){ 
		return;
	}
	CListBox& list_b = CSubsView::m_pView->m_ListCtrl;
	CString sSub = "";
	int iSubID = 0, iCountSubMark = 0,  iCountSub = list_b.GetCount();
	for (int i= 0; i<iCountSub; i++){
		if (list_b.GetSel(i)>0){ 
			sSub.Format("%d",list_b.GetItemData(i));
			m_SubToSub.SetAt(sSub,sSub);
		}
	}
}

bool CMyADO::LineHaveMarkUpSubs (CString sSubsFromObj){
	// предполагается, что в sSubsFromObj подсистемы сидят так: ",2,,12,"
	if (sSubsFromObj.IsEmpty()){ 
		return false;
	}
	CString sSubOne = "", sTempStr="";
	int nLen = sSubsFromObj.GetLength();
	for (int i = 0; i<nLen; i++) {
		if (sSubsFromObj[i] != ','){ 
			sSubOne += sSubsFromObj[i];
		} else {
			if (!sSubOne.IsEmpty()){ 
				if (m_SubToSub.Lookup(sSubOne,sTempStr)){ 
					return true;
				}
				sSubOne = "";
			}
		}
	}
	return false;
	
}

void CMyADO::SaveOrLoadSubsSelection(bool bSave) {
	// сохраняем или восстанавливаем выделение в списке подсистем
	// удобно когда пользователь закрывает систему со статусом 
	// Фильтр.
	if (CSubsView::m_pView == NULL){ 
		return;
	}
	CListBox& list_b = CSubsView::m_pView->m_ListCtrl;
	int nCount = list_b.GetCount();
	CString sSubID = "";
	for (int i=0; i<nCount; i++){
		sSubID.Format("%d",list_b.GetItemData(i));
		SubsStorage* stSusSt;
		m_SubSelfs.Lookup(sSubID,(CObject*&)stSusSt);
		if (bSave){ 
			stSusSt->m_Check = list_b.GetSel(i);
			m_SubSelfs.SetAt(sSubID, (CObject*&)stSusSt);
		} else {
			list_b.SetSel(i, (stSusSt->m_Check==1) );
		}
	}

}

void CMyADO::SaveOrLoadSettings(bool bSave)
{
	CString tTmpChrt;
	if (!bSave){ 
		//AfxMessageBox("Load");
		// Восстановим настройки, сохранять будем при OnOK в диалоге натройки....
		m_Settings.EnableDDGroup	= (int) GetPrivateProfileInt("SimplSubsMainSet","EnableDDGroup",1,m_sFileSettingsName);
		m_Settings.EnableFullPathDD	= (int) GetPrivateProfileInt("SimplSubsMainSet","EnableFullPathDD",0,m_sFileSettingsName);
		
		m_Settings.ShowColComm		= (int) GetPrivateProfileInt("SimplSubsMainSet","ShowColComm",1,m_sFileSettingsName);
		m_Settings.ShowColType		= (int) GetPrivateProfileInt("SimplSubsMainSet","ShowColType",1,m_sFileSettingsName);
		m_Settings.ShowColID		= (int) GetPrivateProfileInt("SimplSubsMainSet","ShowColID",0,m_sFileSettingsName);
		m_Settings.ShowSubPanePos	= (int) GetPrivateProfileInt("SimplSubsMainSet","ShowSubPanePos",0,m_sFileSettingsName);
		m_Settings.Show_ID			= (int) GetPrivateProfileInt("SimplSubsMainSet","Show_ID",0,m_sFileSettingsName);
		char szBuffer[MAXBUFSIZE];
		GetPrivateProfileString("SimplSubsMainSet","CharsTerminator",", ",szBuffer,MAXBUFSIZE,m_sFileSettingsName);
		tTmpChrt = (LPTSTR) szBuffer;
		if (tTmpChrt.GetLength()>2){ 
			tTmpChrt = tTmpChrt.Mid(1,tTmpChrt.GetLength()-2);
		}
		m_Settings.strCharsTerminator = (LPCTSTR) tTmpChrt;

	} else {
		
		//AfxMessageBox("Save");

		CString strSave;
		strSave.Format("%d",m_Settings.EnableDDGroup);		
		WritePrivateProfileString("SimplSubsMainSet","EnableDDGroup",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.EnableFullPathDD);		
		WritePrivateProfileString("SimplSubsMainSet","EnableFullPathDD",strSave,m_sFileSettingsName);
		
		
		strSave.Format("@%s@",m_Settings.strCharsTerminator);	
		WritePrivateProfileString("SimplSubsMainSet","CharsTerminator",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.ShowColType);
		WritePrivateProfileString("SimplSubsMainSet","ShowColType",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.ShowColComm);
		WritePrivateProfileString("SimplSubsMainSet","ShowColComm",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.ShowColID);	
		WritePrivateProfileString("SimplSubsMainSet","ShowColID",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.ShowSubPanePos);	
		WritePrivateProfileString("SimplSubsMainSet","ShowSubPanePos",strSave,m_sFileSettingsName);

		strSave.Format("%d",m_Settings.Show_ID);	
		WritePrivateProfileString("SimplSubsMainSet","Show_ID",strSave,m_sFileSettingsName);		

	}


}

// например по ESC найдо убежать с панели
void CMyADO::ActivateStandartView()
{
	SECMDIFrameWnd* pFrame=(SECMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd* pChild=pFrame->MDIGetActive();
	if(pChild && pChild->IsWindowVisible())
	{
		pChild->ActivateFrame();
		CView* pView=pChild->GetActiveView();
		if(pView)
		{
			pView->SetFocus();
			return;
		}
	}
}
