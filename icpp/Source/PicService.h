// PicService.h: interface for the CPicService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICSERVICE_H__BF2E8E86_889C_44B2_9058_FCC464383022__INCLUDED_)
#define AFX_PICSERVICE_H__BF2E8E86_889C_44B2_9058_FCC464383022__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPicService  
{
public:
	enum eForm {Error = -1, Picture = 0, Gallery = 1};

public:
	CPictureHolder7* GetOriginalPictureHolder7() const;
	BOOL SelectPictureHolder7(enum eForm eType, const CValue* pValue);
	CSize GetOriginalPictureSize() const;
	BOOL RenderPicture(int nPicIndx, CSize size, CRect rect);
	BOOL GetRenderedPicture(IPicture** ppPicture);
	BOOL SaveRenderedPictureAsFile(CString& strPath, CString& strFileName, CString& strError);

public:
	CPicService();
	virtual ~CPicService();

private:
	BOOL m_IsRendered;
	CPictureHolder7* m_pPicHolder7;
	CPictureHolder7* m_pRenderedPicHolder7;
};

#endif // !defined(AFX_PICSERVICE_H__BF2E8E86_889C_44B2_9058_FCC464383022__INCLUDED_)