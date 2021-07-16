// FileMeta.cpp: implementation of the CFileMeta class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "FileMeta.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileMeta::CFileMeta()
{
	m_pMetaObj=0;
}

CFileMeta::~CFileMeta()
{

}

BOOL CFileMeta::Open(LPCTSTR lpszFileName, UINT nOpenFlags,CFileException* pError)
{
	if(IsMetaFile())
	{
		return 1;
	}
	else
	{
		return CFile::Open(lpszFileName,nOpenFlags,pError);
	}
}
DWORD CFileMeta::GetLength() const
{
	if(IsMetaFile())
	{
		return m_pMetaObj->csFile.GetLength();
	}
	else
	{
		return CFile::GetLength();
	}
}
UINT CFileMeta::Read(void* lpBuf, UINT nCount)
{
	if(IsMetaFile())
	{
		nCount=min(nCount,GetLength());
		memcpy(lpBuf,m_pMetaObj->csFile.GetBuffer(0),nCount);
		return nCount;
	}
	else
	{
		return CFile::Read(lpBuf,nCount);
	}
}
void CFileMeta::Write(const void* lpBuf, UINT nCount)
{
	if(IsMetaFile())
	{
		m_pMetaObj->csFile.Empty();
		if(nCount>0)
		{
			char *p=m_pMetaObj->csFile.GetBuffer(nCount);
			memcpy(p,lpBuf,nCount);
		}
	}
	else
	{
		CFile::Write(lpBuf,nCount);
	}
}
void CFileMeta::Close()
{
	if(IsMetaFile())
	{

	}
	else
	{
		CFile::Close();
	}
}
void CFileMeta::Abort()
{
	if(IsMetaFile())
	{

	}
	else
	{
		CFile::Abort();
	}
}
void CFileMeta::Flush()
{
	if(IsMetaFile())
	{

	}
	else
	{
		CFile::Flush();
	}
}
