// PicService.cpp: implementation of the CPicService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PicService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________
//
CPicService::CPicService() : 
	m_IsRendered(FALSE), 
	m_pPicHolder7(NULL), 
	m_pRenderedPicHolder7(new CPictureHolder7)
{
	m_pRenderedPicHolder7 = new CPictureHolder7;
}

//_____________________________________________________________________________
//
CPicService::~CPicService()
{
	if(m_pPicHolder7)
		delete m_pPicHolder7;
	m_pPicHolder7 = NULL;

	if(m_pRenderedPicHolder7)
		delete m_pRenderedPicHolder7;
	m_pRenderedPicHolder7 = NULL;

	m_IsRendered = FALSE;
}

//_____________________________________________________________________________
//
BOOL CPicService::SelectPictureHolder7(enum eForm eType, const CValue* pValue)
{
	if(CPicService::eForm::Picture == eType)
		m_pPicHolder7 = new CPictureHolder7(reinterpret_cast<CPictureContext*>(pValue->GetContext())->m_PictureHolder7);
	else if(CPicService::eForm::Gallery == eType)
	{
		CGalleryHolder* pGalleryHolder = CGalleryHolder::GetGallery();
		if(pGalleryHolder)
		{
			ULONG picID = pGalleryHolder->FindByAlias(pValue->GetString());
			if(picID)
				m_pPicHolder7 = new CPictureHolder7(pGalleryHolder->GetPicture(picID));
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
	{
		if(m_pPicHolder7)
			delete m_pPicHolder7;

		m_pPicHolder7 = NULL;

		return TRUE;
	}

	if(!m_pPicHolder7)
		return FALSE;

	return TRUE;
}

//_____________________________________________________________________________
//
CPictureHolder7* CPicService::GetOriginalPictureHolder7() const
{
	return m_pPicHolder7;
}

//_____________________________________________________________________________
//
BOOL CPicService::GetRenderedPicture(IPicture** ppPicture)
{
	BOOL bRet = TRUE;
	
	if(m_IsRendered)
	{
		ppPicture[0] = m_pRenderedPicHolder7->GetPicture();
		ppPicture[0]->AddRef();
	}
	else
		bRet = FALSE;
	
	return bRet;
}

//_____________________________________________________________________________
//
CSize CPicService::GetOriginalPictureSize() const
{
	if(!m_pPicHolder7)
		return CSize(0, 0);

	return m_pPicHolder7->GetSize();
}

//_____________________________________________________________________________
//
BOOL CPicService::SaveRenderedPictureAsFile(CString& strPath, CString& strFileName, CString& strError)
{
	if(!m_IsRendered)
	{
		strError = "Картинка не была отрисована";
		return FALSE;
	}

	HGLOBAL hGlobal = ::GlobalAlloc(GHND, 0);
	
	IStream* pStream = NULL; // она очистится сама, см. параметр 2 в CreateStreamOnHGlobal()
	if(FAILED(::CreateStreamOnHGlobal(hGlobal, TRUE, &pStream)))
	{
		::GlobalFree(hGlobal);

		strError = "Не удалось создать поток под образ картинки в памяти";
		return FALSE;
	}

	IPicture* pIPicFrame = m_pRenderedPicHolder7->m_IPicture;

	LONG nSize = 0;
	if(FAILED(pIPicFrame->SaveAsFile(pStream, TRUE, &nSize)))
	{
		::GlobalFree(hGlobal);

		strError = "Не удалось сохранить образ картинки в поток";
		return FALSE;
	}

	if(0 == nSize)
	{
		::GlobalFree(hGlobal);

		strError = "Не удалось подготовить поток данных";
		return FALSE;
	}

	char szTempName[MAX_PATH];
	char lpPathBuffer[4096];
	DWORD dwBufSize = 4096;
	
	::memset(szTempName, 0, MAX_PATH);
	::memset(lpPathBuffer, 0, dwBufSize);
	
	if(strPath.IsEmpty())
	{
		::GetTempPath(dwBufSize, lpPathBuffer);
		strPath = lpPathBuffer;
	}
	else
	{
		int nLength = strPath.GetLength();
		::memcpy(lpPathBuffer, strPath, nLength > dwBufSize ? dwBufSize : nLength);
	}

	CString strFileNameLoc;
	short nType = m_pPicHolder7->GetType();
	
	if(strFileName.IsEmpty())
	{
		int nAttemps = 100; // даем сто попыток для создания файла
		while(nAttemps > 0)
		{
			::memset(szTempName, 0, MAX_PATH);
			::GetTempFileName(lpPathBuffer, "pic", 0, szTempName);

			strFileNameLoc = szTempName;
			strFileNameLoc.TrimRight(".tmp");
			switch(nType)
			{
			case 1: strFileNameLoc += ".bmp";
				break;
			case 2: strFileNameLoc += ".wmf";
				break;
			case 3: strFileNameLoc += ".ico";
				break;
			};

			try
			{
				CFile::Rename(szTempName, strFileNameLoc);
				nAttemps = -100; // ура
				break;
			}
			catch(...)
			{
				nAttemps--;

				try
				{
					CFile::Remove(szTempName);
				}
				catch(...)
				{
					// ну и ладно
				}
			}
		}

		if(nAttemps != -100)
		{
			::GlobalFree(hGlobal);
			
			strError = "Произошла ошибка при создании файла";
			return FALSE;
		}

		strFileName = strFileNameLoc.Right(strFileNameLoc.GetLength() - strPath.GetLength());
	}
	else
	{
		strFileNameLoc = strFileName;
		CString strFileExt = strFileNameLoc.Right(4);
		if(strFileExt.CompareNoCase(".bmp") 
			|| strFileExt.CompareNoCase(".wmf") 
			|| strFileExt.CompareNoCase(".ico"))
		{
			switch(nType)
			{
			case 1:
				strFileNameLoc += ".bmp";
				break;
			case 2:
				strFileNameLoc += ".wmf";
				break;
			case 3:
				strFileNameLoc += ".ico";
				break;
			};
		}
		
		strFileName = strFileNameLoc;
	}

	CFile file;
	CFileException Err;
	
	bool IsError = false;
	TCHAR szCause[255];
	
	CString strPathFileNameLoc = strPath + strFileName;

	LPVOID pBufferBytes = ::GlobalLock(hGlobal);
	
	if(file.Open(strPathFileNameLoc, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite, &Err))
	{
		file.Write(pBufferBytes, nSize);
		file.Close();
	}
	else
	{
		IsError = true;
		Err.GetErrorMessage(szCause, 255, NULL);
		strError = szCause;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);
	
	if(IsError)
		return FALSE;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL CPicService::RenderPicture(int nPicIndx, CSize size, CRect rect)
{
	BOOL bRet = TRUE;
	m_IsRendered = FALSE;

	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();
	
	if(pDC)
	{
		CDC hDeskDC;
		if(hDeskDC.CreateCompatibleDC(pDC))
		{
			CBitmap hBitmap;
			if(hBitmap.CreateCompatibleBitmap(pDC, size.cx, size.cy))
			{
				// связываем битмап с контекстом устройства
				//
				CBitmap* hBitmapOld = hDeskDC.SelectObject(&hBitmap);
				
				// Закрашиваем регион системным цветом
				//
				hDeskDC.FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
				
				// Отрисоваем картинку в контекст устройства
				//
				m_pPicHolder7->Render(hDeskDC, rect, 0, NULL);
				
				// возвращаем на место
				//
				hDeskDC.SelectObject(hBitmapOld);
				
				// Освобождаем hBitmap от владения GDI объектом
				//
				HGDIOBJ obj = hBitmap.Detach();
				
				if(!m_pRenderedPicHolder7->CreateFromBitmap(reinterpret_cast<HBITMAP>(obj), NULL))
					bRet = FALSE;
				else
					m_IsRendered = TRUE;
			}
			else
				bRet = FALSE;
		}
		else
			bRet = FALSE;
	}
	else
		bRet = FALSE;
	
	CWnd::GetDesktopWindow()->ReleaseDC(pDC);
	
	return bRet;
}
