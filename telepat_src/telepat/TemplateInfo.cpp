// TemplateInfo.cpp
#include "stdafx.h"
#include "TemplateInfo.h"
#include "TxtTrap.h"
#include "TeleGroup.h"
#include "TEntryDrawing.h"
#include "blmap.h"
#include "telelist.h"
#include "parser.h"
#include "KeyWords.h"
#include "option.h"
CTemplateInfo CTemplateInfo::m_TemplateInfo;

static void MovePtr(LPCTSTR& ptr)
{
	while(*ptr && *ptr!='{' && *ptr!='}' && *ptr!=',' && *ptr!='\"')
		ptr++;
}
static CString ReadLexem(LPCTSTR& ptr)
{
	if(!*ptr || *ptr!='\"')
		throw int(1);
	LPCTSTR pStart=++ptr;
	int state=0;
	while(1)
	{
		if(!*ptr)
			throw int(1);
		if(!state)
		{
			if(*ptr=='\"')
				state=1;
		}
		else if(state==1)
		{
			if(*ptr=='\"')
				state=0;
			else
				break;
		}
		ptr++;
	}
	CString ret=CString(pStart,ptr-pStart-1);
	ret.Replace("\"\"","\"");
	MovePtr(++ptr);
	return ret;
}

void CTemplateInfo::AddRecent(TemplateItem* item)
{
	int size=recent.GetSize();
	for(int i=0;i<size;i++)
	{
		if(recent[i]==item)
		{
			recent.RemoveAt(i);
			break;
		}
	}
	recent.InsertAt(0,item);
	if(recent.GetSize()==11)
		recent.RemoveAt(10);
}

void CTemplateInfo::CheckRecent(TemplateItem* item)
{
	int size=recNames.GetSize();
	for(int i=0;i<size;i++)
	{
		if(recNames[i]==item->name)
		{
			recent.SetAt(i,item);
			break;
		}
	}
}

bool CTemplateInfo::ReadTemplate()
{
	CString path;
	POSITION p=CTxtTrap::m_pAssistant->GetFirstDocPosition();
	if(p)
	{
		CAssistantDoc* pAss=(CAssistantDoc*)CTxtTrap::m_pAssistant->GetNextDoc(p);
		CSrcTemplDocParent* pTemplate=pAss->GetSourceTemplate();
		if(pTemplate)
			path=pTemplate->GetPathName();
	}
	if(path.IsEmpty())
		return false;

	WIN32_FILE_ATTRIBUTE_DATA fData;
	if(!GetFileAttributesEx(path,GetFileExInfoStandard,&fData))
		return false;
	if((CompareFileTime(&LastReadTemplate,&fData.ftLastWriteTime)>=0) &&
		path==m_strFileName)
		return true;
	try{
		CFile file(path,CFile::modeRead);
		CString text;
		int size=file.GetLength();
		file.Read(text.GetBuffer(size),size);
		text.ReleaseBuffer(size);
		file.Close();
		if(root)
		{
			recNames.RemoveAll();
			int size=recent.GetSize();
			for(int i=0;i<size;i++)
			{
				recNames.Add(((TemplateItem*)recent[i])->name);
				recent.SetAt(i,NULL);
			}
			delete root;
			root=NULL;
		}
		LPCTSTR ptr=text;
		if(*ptr=='{')
		{
			ReadLexem(++ptr);
			if(*ptr=='{')
			{
				root=ProcessItem(ptr);
				if(root)
				{
					int size=recent.GetSize();
					for(int i=0;i<size;i++)
					{
						if(!recent[i])
						{
							recent.RemoveAt(i);
							size--;
						}
					}
				}
			}
		}
	}
	catch(CFileException* pE)
	{
		pE->Delete();
		return false;
	}
	catch(int)
	{
		return false;
	}
	catch(...)
	{
		return false;
	}
	LastReadTemplate=fData.ftLastWriteTime;
	m_strFileName=path;
	return true;
}

static bool IsIdent(LPCTSTR text)
{
	if(*text>='0' && *text<='9')
		return false;
	while(*text)
	{
		if(CBLMap::m_LowerTable[(BYTE)*text++]==' ')
			return false;
	}
	return true;
}

TemplateItem* CTemplateInfo::ProcessItem(LPCTSTR& ptr)
{
	CString name,replace,text;
	bool InMenu;
	TemplateItem *fitem=NULL,*item=NULL,*child=NULL,*next=NULL;
	ptr++;
	while(1)
	{
		name=ReadLexem(ptr);
		if(*ptr=='{')				// Это группа, считаем вложенную группу
		{
			child=ProcessItem(ptr);
			if(child)
			{
				next=new TemplateItem(name);
				next->child=child;
				if(item)
					item->next=next;
				else
					fitem=next;
				item=next;
			}
			MovePtr(++ptr);
		}
		else if(*ptr=='\"')					// Это шаблон, считаем его
		{
			ReadLexem(ptr);					// id
			replace=ReadLexem(ptr);			// what replace
			text=ReadLexem(ptr);			// replace text
			InMenu=ReadLexem(ptr)[0]=='1';	// InMenu flag
			ReadLexem(ptr);					// Edit in wnd flag
			ReadLexem(ptr);					// Use in TO flag
			bool add=InMenu;
			if(!add && !replace.IsEmpty())
				add=IsIdent(replace);
			if(add)
			{
				next=new TemplateItem(name);
				next->text=text;
				next->replace=replace;
				next->InMenu=InMenu;
				CheckRecent(next);
				if(item)
					item->next=next;
				else
					fitem=next;
				item=next;
			}
		}
		if(*ptr==',')
		{
			MovePtr(++ptr);
			ptr++;
		}
		else
			break;
	}
	return fitem;
}

int CTemplateInfo::FillMenu(TemplateItem* item,HMENU hMenu,int id)
{
	if(id==ID_LAST_TEMPLATE)
		return id;
	if(item->child)
	{
		HMENU hNewMenu=CreatePopupMenu();
		id=FillMenu(item->child,hNewMenu,id);
		if(GetMenuItemCount(hNewMenu))
			AppendMenu(hMenu,MF_POPUP|MF_STRING,(UINT)hNewMenu,item->name);
		else
			DestroyMenu(hNewMenu);
	}
	else
	{
		if(item->InMenu)
		{
			AppendMenu(hMenu,MF_STRING,++id,item->name);
			id2temp[(void*)id]=item;
		}
	}
	if(item->next)
		id=FillMenu(item->next,hMenu,id);
	return id;
}

void CTemplateInfo::FillMenu(HMENU hMenu)
{
	id2temp.RemoveAll();
	int size=recent.GetSize();
	for(int i=0;i<size;)
	{
		TemplateItem* item=(TemplateItem*)recent[i];
		CString name,n=item->name;
		n.Replace("&","");
		if(i<9)
			name.Format("&%i %s",i+1,n);
		else
			name.Format("1&0 %s",n);
		AppendMenu(hMenu,MF_STRING,++i,name);
		id2temp[(void*)i]=item;
	}
	if(size)
		AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	if(root)
		FillMenu(root,hMenu,i);
}

class CTemplateEntry:public CTeleEntry
{
public:
	CTemplateEntry(TemplateItem* pItem):m_pItem(pItem){m_Name=pItem->replace;}
	TemplateItem* m_pItem;
	virtual void GetHint(CString& buf)
	{
		buf.Format("Шаблон \"%s\"",m_pItem->name);
		if(!m_pItem->text.IsEmpty())
		{
			LPCTSTR ptr=m_pItem->text,start=ptr;
			int line=0;
			while(*ptr && line<3)
			{
				if(*ptr=='\r')
					line++;
				ptr++;
			}
			CString txt=CString(start,ptr-start);
			txt.Replace("\r\n","\n");
			if(*ptr)
				txt+="\n....";
			buf=buf+"\n"+txt;
		}
	}
	virtual bool MakeInsert()
	{
		CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
		CTextDocument* pDoc=pEditor->GetDocument();
		CPoint ptStart=CTeleList::m_pOneList->m_CaretPoint;
		ptStart.x-=CTeleList::m_pOneList->m_curPosInBuf;
		CPoint ptEnd=ptStart;
		ptEnd.x+=CTeleList::m_pOneList->m_buf.GetLength();
		CTeleList::m_pOneList->Hide();
		pEditor->ReplaceTemplate(m_pItem->text,m_pItem->name,ptStart,ptEnd,1);
		return true;
	}
};

TemplateItem::TemplateItem(CString n):name(n),child(NULL),next(NULL)
{
	int end=name.Find('¦');
	DWORD fl=0;
	if(end>-1)
	{
		CString f=name.Mid(end+1);
		name=name.Left(end);
		/*
		proc	1
		func	2
		expr	3
		keyw	4
		*/
		if(!f.Find("proc"))
			fl=1<<CKeyWords::kwLast;
		else if(!f.Find("func"))
			fl=2<<CKeyWords::kwLast;
		else if(!f.Find("expr"))
			fl=3<<CKeyWords::kwLast;
		else if(!f.Find("keyw:"))
		{
			fl=4<<CKeyWords::kwLast;
			f=f.Mid(5);
			f=CString(",")+f+",";
			f.MakeLower();
			for(int i=0;i<CKeyWords::kwLast;i++)
			{
				for(int j=0;j<2;j++)
				{
					CString kw=CKeyWords::keyWords[i].name[j];
					kw.MakeLower();
					kw=CString(",")+kw+",";
					if(f.Find(kw)>-1)
					{
						fl|=(1<<i);
						break;
					}
				}
			}
		}
	}
	flags=fl;
}

static void FillGroupFromTemplate(CTeleGroup* pGroup,TemplateItem* item)
{
	while(item)
	{
		if(!item->replace.IsEmpty() && IsIdent(item->replace))
		{
			bool add=false;
			if(item->flags)
			{
				CParser* pParser=CTxtTrap::GetParser();
				DWORD f=item->flags>>CKeyWords::kwLast;
				if(f<4)
				{
					if(pParser->m_AllowedConts[CParser::Methods])
					{
						bool InExpr=(pParser->m_flags & CParser::InExpression)!=0;
						if((f==1 && !InExpr) || (f==2) || (f==3 && InExpr))
							add=true;
					}
				}
				else
				{
					if((item->flags & ~(0xFF<<CKeyWords::kwLast)) & pParser->m_kwords.m_allowedWords)
						add=true;
				}
			}
			else
				add=true;
			if(add)
			{
				CTeleEntry* pEntry=new CTemplateEntry(item);
				pGroup->AddEntry(pEntry);
			}
		}
		FillGroupFromTemplate(pGroup,item->child);
		item=item->next;
	}
}

void CTemplateInfo::FillGroup(CPtrArray& arr)
{
	static CTeleGroup Group;
	Group.m_Image=IDI_TEMPLATES_;
	Group.ClearEntries();
	if(!COption::m_pOption->GetAddTemplate())
		return;
	ReadTemplate();
	FillGroupFromTemplate(&Group,root);
	if(Group.GetCount())
		arr.Add(&Group);
}