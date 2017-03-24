// FormHolder.h: interface for the CFormHolder class.
#if !defined(AFX_FORMHOLDER_H__C453DED0_B125_4771_BC2F_F1AA24F85805__INCLUDED_)
#define AFX_FORMHOLDER_H__C453DED0_B125_4771_BC2F_F1AA24F85805__INCLUDED_

class CFormHolder: public CWnd
{
public:
	void Activate();
	bool SaveFile(LPCSTR path);
	bool LoadFile(CString path,CWnd* pParent);
	CFormHolder();
	virtual ~CFormHolder();
	
	_UserFormPtr m_pForm;
	CString m_FileName;
	CString m_moduleText;
};

#endif // !defined(AFX_FORMHOLDER_H__C453DED0_B125_4771_BC2F_F1AA24F85805__INCLUDED_)
