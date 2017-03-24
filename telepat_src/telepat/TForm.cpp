// TForm.cpp: implementation of the CTForm class.
#include "stdafx.h"
#include "telecont.h"
#include "telelist.h"
#include "TForm.h"
#include "TEntryDrawing.h"
#include "syntax.h"
#include "resource.h"
#include "txttrap.h"
#include "parser.h"


static void ReadLexem(LPCTSTR& pRead,SLexem& lexem)
{
	lexem.pStart=NULL;
	while(*pRead && *pRead!='\"')
	{
		if(*pRead=='{')
			lexem.line++;
		else if(*pRead=='}')
			lexem.line--;
		pRead++;
	}
	if(!*pRead)
		return;
	lexem.pStart=++pRead;
	lexem.type=1;
	while(*pRead)
	{
		if(*pRead=='\"')
			lexem.type=3-lexem.type;
		else if(lexem.type==2)
			break;
		pRead++;
	}
	lexem.len=pRead-lexem.pStart;
	if(*pRead)
		lexem.len--;
	while(*pRead && *pRead=='}')
		pRead++,lexem.line--;
}

void CTForm::FillGroup(CPtrArray& arr)
{
	if(m_pParser->m_flags & CParser::NotArrayVar)
		return;
	if(m_Group[0]->GetCount())
		arr.Add(m_Group[0]);
}

const char* CTForm::m_TypeNames[]={
	"Поле ввода",
	"Поле ввода",
	"Текст",
	"Рамка группы",
	"Кнопка",
	"Таблица значений",
	"Переключатель",
	"Поле со списком",
	"Флажок",
	"Список значений"
};
const char* CTForm::m_1CTypeNames[]=
{
	"1CEDIT",
	"BMASKED",
	"STATIC",
	"1CGROUPBOX",
	"BUTTON",
	"TABLE",
	"RADIO",
	"COMBOBOX",
	"CHECKBOX",
	"LISTBOX",
};

void CTFormEntry::GetHint(CString& buf)
{
	buf=m_Name+"\n"+CTForm::m_TypeNames[m_type];
	if(m_type<CTForm::Static)
	{
		CType type(CType::C2TypeCode(m_DataType),m_len,m_prec);
		buf=buf+"\n"+type.GetTypeTitle();
		if(m_kind)
		{
			CMetaDataObj* pObj=pMetaDataCont->FindObject(m_kind);
			if(pObj)
				buf=buf+"."+pObj->GetCode();
		}
		else if(m_len)
		{
			CString fmt;
			fmt.Format(" Длина %i",m_len);
			buf+=fmt;
			if(m_prec)
			{
				fmt.Format(" Точность %i",m_prec);
				buf+=fmt;
			}
		}
	}
}

const int	CTForm::GetGroupCount() const
{
	return 1;
}

const int*	CTForm::GetImagesNum()  const
{
	static const int img[]={IDI_DLGFIELD_};
	return img;
}

LPCTSTR*	CTForm::GetGroupNames() const
{
	static LPCTSTR name[]={"Реквизит формы"};
	return name;
}

CTForm::CTForm(CParser* pParser,CWorkBookDoc* pDoc):CTeleCont(pParser)
{
	InitGroups();
	CEditDoc* pEdit=(CEditDoc*)pDoc->GetDEditDocument();
	if(!pEdit)
		return;
	CMemFile f;
	CArchive arc(&f,CArchive::store);
	pEdit->Serialize(arc);
	DWORD size=f.GetLength();
	CString data;
	f.SeekToBegin();
	f.Read(data.GetBuffer(size),size);
	data.ReleaseBuffer(size);
	LPCTSTR pRead=data;
	while(*pRead!='{')pRead++;
	SLexem lexem;
	lexem.line=0;
	do{
		ReadLexem(pRead,lexem);
		if(!lexem.pStart)
			break;
	}while(!(CString(lexem)=="Controls" && lexem.line==2));
	if(!lexem.pStart)
		return;
	do{
		ReadLexem(pRead,lexem); // Заголовок
		CString strTitle=lexem;
		ReadLexem(pRead,lexem); // Тип
		CString strType=lexem;
		TypesOfControl type;
		for(int i=Edit;i<lastTypeOfControl;i++)
		{
			if(strType==m_1CTypeNames[i])
			{
				type=(TypesOfControl)i;
				break;
			}
		}
		if(type!=Button)
		{
			for(i=0;i<11;i++)
				ReadLexem(pRead,lexem);
			CString strID=lexem;
			if(!strID.IsEmpty())
			{
				ReadLexem(pRead,lexem);
				int metaID=atoi(CString(lexem));
				if(!metaID || metaID==-1)
				{
					CTFormEntry* pEntry=new CTFormEntry(type,strID);
					ReadLexem(pRead,lexem);
					pEntry->m_DataType=*lexem.pStart;
					ReadLexem(pRead,lexem);
					pEntry->m_len=atoi(lexem.pStart);
					ReadLexem(pRead,lexem);
					pEntry->m_prec=atoi(lexem.pStart);
					ReadLexem(pRead,lexem);
					pEntry->m_kind=atoi(lexem.pStart);
					m_Group[0]->AddEntry(pEntry);
					m_mNames.InsertKey(strID,pEntry);
				}
			}
		}
		do
			ReadLexem(pRead,lexem);
		while(lexem.line==3);
	}while(lexem.line==2);
}

bool CTForm::IsAttribExist(SLexem &name)
{
	return m_mNames.GetKey(name)!=NULL;
}
