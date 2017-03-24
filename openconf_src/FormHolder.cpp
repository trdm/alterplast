// FormHolder.cpp: implementation of the CFormHolder class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "FormHolder.h"
#include "oleclientiemhook.h"
#include "OCView.h"

CFormHolder::CFormHolder()
{
	m_pForm=NULL;
}

CFormHolder::~CFormHolder()
{
	if(m_pForm!=NULL)
		m_pForm=NULL;
}

struct __declspec(uuid("C62A69F0-16DC-11CE-9E98-00AA00574A4F")) MSForms;

bool CFormHolder::LoadFile(CString path,CWnd* pParent)
{
	m_FileName=path;
	try{
		CFile* pFile=NULL;
		if(!m_FileName.IsEmpty())
		{
			/*
			CFile file(m_FileName,CFile::modeRead);
			int len=file.GetLength();
			DWORD modSize;
			if(len>3)
			{
				file.Read(&modSize,4);
				if(modSize)
				{
					file.Read(m_moduleText.GetBuffer(modSize),modSize);
					m_moduleText.ReleaseBuffer();
				}
			}
			else
			{
				DoMsgLine("Неправильный формат файла",mmRedErr);
				return false;
			}
			*/
			pFile=new CFile(m_FileName,CFile::modeRead);
			/*
			DWORD stgLen=len-modSize-4;
			char* buf=new char[stgLen];
			file.Read(buf,stgLen);
			pFile->Write(buf,stgLen);
			pFile->SeekToBegin();
			delete [] buf;
			*/
		}
		CRect rc;
		pParent->GetClientRect(rc);
		BOOL ret=CreateControl(__uuidof(MSForms),NULL,WS_VISIBLE,rc,pParent,100,pFile,TRUE);
		m_pForm=GetControlUnknown();//m_pCtrlSite->m_pObject;
		if(pFile)
		{
			delete pFile;
			m_pForm->put_DesignMode(fmModeOff);
		}
		m_pForm->put_DesignMode(fmModeOn);

		return ret==TRUE;
	}
	catch (CFileException* pE)
	{
		char buf[1000];
		pE->GetErrorMessage(buf,1000);
		DoMsgLine(buf,mmRedErr);
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool CFormHolder::SaveFile(LPCSTR path)
{
	IStoragePtr stg;
	DeleteFile(path);
	StgCreateDocfile(_bstr_t(path),STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_TRANSACTED,0,&stg);
	IPersistStoragePtr pers=m_pForm;
	pers->Save(stg,FALSE);
	stg->Commit(STGC_DEFAULT);
	stg=NULL;


	/*
	CFile file(path,CFile::modeWrite|CFile::modeCreate);
	DWORD modSize=m_moduleText.GetLength();
	file.Write(&modSize,4);
	if(modSize)
		file.Write((LPCTSTR)m_moduleText,modSize);
	ILockBytes* pIlb=NULL;
	HGLOBAL hGlobal=GlobalAlloc(GMEM_FIXED,0);
	CreateILockBytesOnHGlobal(hGlobal,TRUE,&pIlb);
	IStoragePtr stg;
	StgCreateDocfileOnILockBytes(pIlb,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_TRANSACTED,0,&stg);
	if(stg!=NULL)
	{
		IPersistStoragePtr pers=m_pForm;
		pers->Save(stg,FALSE);
		//pers->SaveCompleted(NULL);
		stg->Commit(STGC_DEFAULT);
		stg=NULL;
		STATSTG stat;
		pIlb->Stat(&stat,STATFLAG_NONAME);
		void* buf=GlobalLock(hGlobal);
		//char* buf=new char[stat.cbSize.QuadPart];
		//ULONG readed;
		//ULARGE_INTEGER zero;
		//zero.QuadPart=0;
		//pIlb->ReadAt(zero,buf,stat.cbSize.QuadPart,&readed);
		file.Write(buf,stat.cbSize.QuadPart);
		//delete [] buf;
		pIlb->Release();
	}
	*/
	return true;
}

void CFormHolder::Activate()
{
	//m_pCtrlSite->DoVerb(OLEIVERB_HIDE);
	m_pCtrlSite->DoVerb(OLEIVERB_SHOW);
}
