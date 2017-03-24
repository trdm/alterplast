// Небольшой класс для получения созданных в 1С-Предприятии CGetDoc7 доков

class CFormBrowse:public CDocTemplate
{
public:
	BOOL IsGetDoc(){return m_pDocClass->IsDerivedFrom(RUNTIME_CLASS(CGetDoc7));}
	static void Init();				// Инициализация
	static void Browse(CObList&);	// Заполнение списка документов
	static CObArray m_pTemplates;	// Список CTemplate7*, у которых док наследован от CGetDoc7
};

// Инициализация
// Просматривает зарегистрированиые шаблоны доков и запоминает те,
// которые CGetDoc7
void CFormBrowse::Init()
{
	CWinApp* pApp=AfxGetApp();
	CFormBrowse* pTemp;
	for(POSITION pos=pApp->GetFirstDocTemplatePosition();pos;)
	{
		pTemp=(CFormBrowse*)pApp->GetNextDocTemplate(pos);
		if(pTemp->IsGetDoc())
			m_pTemplates.Add(pTemp);
	}
}
// Заполнение списка открытых доков
void CFormBrowse::Browse(CObList& lst)
{
	for(int i=0,len=m_pTemplates.GetSize();i<len;i++)
	{
		CDocTemplate* pTemp=(CDocTemplate*)m_pTemplates[i];
		for(POSITION pos=pTemp->GetFirstDocPosition();pos;)
			lst.AddTail(pTemp->GetNextDoc(pos));
	}
}

/*
Дальшу уже сам колдуй, получение форм, контекстов и т.д.
*/