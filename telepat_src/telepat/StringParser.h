// StringParser.h: interface for the CStringParser class.
#if !defined(AFX_STRINGPARSER_H__044FC394_5983_4520_8578_CFDB1FE30F8A__INCLUDED_)
#define AFX_STRINGPARSER_H__044FC394_5983_4520_8578_CFDB1FE30F8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "blmap.h"
class CTxtTrap;

class CStringParser  
{
public:
	void FillFromMetaArray(CMetaDataObjArray* arr);
	void AddKey(LPCTSTR key);
	void ParseString(CString str,CTxtTrap* pEditor);
	static void Init();
	static void ReadWords();
	CStringParser();
	CStringArray	m_result;
	static CBLMap	m_map1;
	CBLMap	m_strMap;
	virtual ~CStringParser();
};

#endif // !defined(AFX_STRINGPARSER_H__044FC394_5983_4520_8578_CFDB1FE30F8A__INCLUDED_)
