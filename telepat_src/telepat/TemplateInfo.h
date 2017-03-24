// TemplateInfo.h
#ifndef TEMPLATEINFO_H
#define TEMPLATEINFO_H

#define ID_FIRST_TEMPLATE	1
#define ID_LAST_TEMPLATE	500

class CTemplateInfo
{
public:
	CTemplateInfo():root(NULL){FILETIME n={0,0};LastReadTemplate = n;}
	struct TemplateItem
	{
		TemplateItem(CString n);
		~TemplateItem()
		{
			if(child)
				delete child;
			if(next)
				delete next;
		}
		CString name;
		DWORD flags;
		CString replace;
		CString text;
		bool InMenu;
		TemplateItem* child;
		TemplateItem* next;
	};
	bool	ReadTemplate();
	void	FillMenu(HMENU hMenu);
	int		FillMenu(TemplateItem* item,HMENU hMenu,int id);
	void	AddRecent(TemplateItem* item);
	void	CheckRecent(TemplateItem* item);
	void	FillGroup(CPtrArray& arr);
	TemplateItem* ProcessItem(LPCTSTR& ptr);
	TemplateItem* GetFromID(UINT id){return (TemplateItem*)id2temp[(void*)id];}

	TemplateItem* root;
	CPtrArray recent;
	CStringArray recNames;
	CMapPtrToPtr id2temp;
	CString	m_strFileName;
	FILETIME LastReadTemplate;
	static CTemplateInfo m_TemplateInfo;
};
typedef CTemplateInfo::TemplateItem TemplateItem;
#endif //TEMPLATEINFO_H