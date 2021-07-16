// FileMeta.h: interface for the CFileMeta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMETA_H__BCD06C80_D618_48AE_8482_7B5F9BBF475E__INCLUDED_)
#define AFX_FILEMETA_H__BCD06C80_D618_48AE_8482_7B5F9BBF475E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMetaObject;

//Класс предназначен для прозрачной поддержки встраивания новых 
//объектов (основанных на CDocument) в метаданные
class CFileMeta : public CFile  
{
public:
	CFileMeta();
	virtual ~CFileMeta();
protected:
	CMetaObject *m_pMetaObj;	//текущий объект метаданных

public:
	BOOL IsMetaFile()const{return m_pMetaObj!=0;};
	void SetObj(CMetaObject *pMeta){m_pMetaObj=pMeta;};

	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError = NULL);
	virtual DWORD GetLength() const;
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual void Abort();
	virtual void Flush();
	virtual void Close();
};

#endif // !defined(AFX_FILEMETA_H__BCD06C80_D618_48AE_8482_7B5F9BBF475E__INCLUDED_)
