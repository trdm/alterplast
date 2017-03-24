// VLResultLoader.h: VLResultLoader - реализация универсального загрузчика IResultLoader для СпискаЗначений.
//
//	Автор - Артур Аюханов aka artbear, 2008
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VLResultLoader__INCLUDED_)
#define _VLResultLoader__INCLUDED_

#pragma once

#include "../SQLITE1C/resultloader.h"

boost::shared_ptr<IResultLoader> CValue2ResultLoader(CValue const& vDest);
boost::shared_ptr<IResultLoader> CBLContext2ResultLoader(CBLContext* pCont);

boost::shared_ptr<IResultPublisher> CValue2ResultPublisher(CValue const& vDest);
boost::shared_ptr<IResultPublisher> CBLContext2ResultPublisher(CBLContext* pCont);

class CVLResultLoader : public IResultLoader
{
public:
	CVLResultLoader() : m_pCont(NULL), m_pList(NULL) {}
	~CVLResultLoader() {}

	typedef CValueListContext VLC_TYPE; //typedef CValueListContextFriend VLC_TYPE;
	
	// artbear TODO при использовании штатных методов, например, CValueListContextFriend::P_Add(ppValues)
	// пропускаются пустые значения, и в приемник-СЗ перестает соответствовать источнику, что плохо :(
	// но без штатных методов есть баг с выгрузкой в список значений на форме - нужно думать :(
	// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=4016

	// сейчас же пустые значения копируются также, как и заполненные.
	// и есть точное соответствие между источником и СЗ

	void setContext(CValueListContext* pCont);
	
	CBLPtr<CValueListContext> createAndSetContext();

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw();
	virtual HRESULT ldrDone() throw();
	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw();
	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw();

	virtual LPCSTR GetLastError() throw();

private:
	CBLPtr<VLC_TYPE> m_pCont;
	CValueItemList* m_pList;
	DWORD m_colCount;
};

class CVTResultLoader : public IResultLoader
{
public:
	CVTResultLoader() : m_pCont(NULL) {}
	~CVTResultLoader() {}

	void setContext(CValueTableContext* pCont);

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw();
	virtual HRESULT ldrDone() throw();
	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw();
	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw();

	virtual LPCSTR GetLastError() throw();

protected:
	CBLPtr<CBLContext> m_pCont;
	CValueTable* m_pTable;
	BOOL m_bPrevEnableRefresh;

	size_t m_nColCount;
	size_t m_nAddedCount;
	std::vector<CValue*> m_vp;
	CValue m_vEmpty;
};

class CTableOfDocResultLoader : public IResultLoader
{
public:
	CTableOfDocResultLoader() : m_pDoc(NULL) {}
	~CTableOfDocResultLoader() {}

	void setContext(CDocContext* pDoc);
	void setError(LPCSTR csLastError) { m_csLastError = csLastError; }

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw();
	virtual HRESULT ldrDone() throw();
	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw();
	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw();

	virtual LPCSTR GetLastError() throw();

protected:
	CBLPtr<CDocContext> m_pDoc;
	CValue m_vVT;
	boost::shared_ptr<IResultLoader> m_pVTLoader;
	CString m_csLastError;
};

class CVLResultPublisher : public IResultPublisher
{
public:
	CVLResultPublisher() : m_pCont(NULL), m_pList(NULL) {}
	~CVLResultPublisher() {}

	typedef CValueListContext VLC_TYPE; //typedef CValueListContextFriend VLC_TYPE;

	// artbear TODO при использовании штатных методов, например, CValueListContextFriend::P_Add(ppValues)
	// пропускаются пустые значения, и в приемник-СЗ перестает соответствовать источнику, что плохо :(
	// но без штатных методов есть баг с выгрузкой в список значений на форме - нужно думать :(
	// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=4016

	// сейчас же пустые значения копируются также, как и заполненные.
	// и есть точное соответствие между источником и СЗ

	void setContext(CValueListContext* pCont);
	//CBLPtr<CValueListContext> createAndSetContext()

	//////////////////////////////////////////////////////////////////////////
	//	IResultPublisher interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw();
	virtual HRESULT pubDone() throw();
	virtual HRESULT pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw();
	virtual HRESULT pubGetRow(CValue const* const*& ppValues) throw();

	virtual LPCSTR GetLastError() throw();

private:
	CBLPtr<VLC_TYPE> m_pCont;
	CValueItemList* m_pList;

	size_t m_nRowIndex;
	boost::scoped_ptr<CValue> m_Value;
	boost::scoped_ptr<CValue> m_TextValue;
	std::vector<CValue*> m_ValueCollection;
};

class CVTResultPublisher : public IResultPublisher
{
public:
	CVTResultPublisher() : m_pCont(NULL) {}
	~CVTResultPublisher() {}

	void setContext(CValueTableContext* pCont);

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw();
	virtual HRESULT pubDone() throw();
	virtual HRESULT pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw();
	virtual HRESULT pubGetRow(CValue const* const*& ppValues) throw();

	virtual LPCSTR GetLastError() throw();

protected:
	CBLPtr<CBLContext> m_pCont;
	CValueTable* m_pTable;

	std::vector<CValue> m_vector;
	std::vector<CValue*> m_pVector;
	int iRowIndex;
};

#endif