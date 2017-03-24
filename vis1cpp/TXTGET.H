// txtget.h
#ifndef TXTGET_H
	#define TXTGET_H

class CTxtGet
{
// 	// Для работы с модулями, не привязанными к форме
// 	// ГМ, Модули Проведения, Описания, наверно и алгоритмы расчетов
// 	// короче, то что лежит в ветке TypedText
// 	// static BOOL CConfigCont::IDToTextModule(id,kind,path,retVal,hz);
// 	typedef BOOL (*PIDTOTEXT)(long,char const *,class CString &,class CModuleCont**,int);
// 	// Для вытаскивания и открытия текста.
// 	// CModuleCont::GetTextDocument,CTypedCont::GetTextDocument
// 	typedef CBodyTextDoc* (CDocument::*PGETTD)(void);
// 	
// 	// Для работы с формами
// 	// static BOOL CConfigCont::IDToPath(id,kind,path,retVal,hz);
// 	typedef int(*PIDTOPATH)(long,char const *,class CString &,class CTypedCont**,int);
// 	// Для открытия форм
// 	// CTypedCont::GetWorkBook()
// 	typedef CWorkBookDoc* (CDocument::*PGETWB)();
// 	
// 	// Не конвертит никак зараза void* в указатель на метод класса...

public:
	//CString GetText(const CString& path);
	CString GetGMText() const;

	static void Init();

// private:
// 	static HINSTANCE m_h7;
// 	static PIDTOTEXT m_pIdToText;
// 	static PIDTOPATH m_pIdToPath;
// 	static PGETTD m_pGetTD;
// 	static PGETWB m_pGetWB;
// 	union PtrConv{
// 		void* pV;
// 		PGETTD pGetTextDoc;
// 		//PGETWB pGetWorkBook;
// 	};
};
#endif