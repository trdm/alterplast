// ModuleParser.h: interface for the CModuleParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEPARSER_H__5A24344B_CF09_41BA_BF89_F95AD6077D7F__INCLUDED_)
#define AFX_MODULEPARSER_H__5A24344B_CF09_41BA_BF89_F95AD6077D7F__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "VIS1CPP_my.h"

// #define KEY_PROC		0x00000000
// #define KEY_VAL			0x40000000
// #define KEY_EXPORT		0x80000000

enum KeyType2
{
	kt2KEY_PROC			= 0, //KEY_PROC >> 30,
	kt2KEY_VAL			= 1, //KEY_VAL >> 30,
	kt2KEY_PROC_EXPORT	= 2, //(KEY_PROC | KEY_EXPORT) >> 30,
	kt2KEY_VAL_EXPORT	= 3, //(KEY_VAL | KEY_EXPORT) >> 30,
};

class CKeyAndLinePair
{
	KeyType2 m_Key;
	DWORD m_Line;
public:
	KeyType2 GetKey() const { return m_Key; }
	DWORD GetLine() const { return m_Line; }
	
	CKeyAndLinePair(KeyType2 key, DWORD newLine) : m_Key(key), m_Line(newLine) {}
	CKeyAndLinePair() : m_Key(kt2KEY_PROC), m_Line(0) {}
};

typedef std::map<CString, CKeyAndLinePair, CompNoCase> MAP_Str_KeyAndLine;
typedef MAP_Str_KeyAndLine::const_iterator Str2KeyAndLine_IT;

class CModuleParser
{
public:

	static void InitMaps();

	CModuleParser() : m_curLine(0), m_OffsetLines(0) {} // m_IsParse(false), 
	void Init(const CString& strPath);
	
	CString GetSourceText();

	void ParseClassModule(const CString& ClassName);

	static CBodyTextView* FindDef(CDocument* pDoc, const CString& Class, int& line);
	
	const MAP_Str_KeyAndLine& GetKeysMap() const { return m_keys; };	
	//bool FindKey(const CString& strName, DWORD* pResult) const;
	bool FindKey(const CString& strName, CKeyAndLinePair* pResult) const;

	int getOffsetLines() const {return m_OffsetLines;};

private:
	int m_curLine;
	
	int m_OffsetLines;
	
	//bool m_IsParse;
	CString m_path;
	MAP_Str_KeyAndLine m_keys;

	static MAP_Str_Dword m_Tokens;
	static const char m_delim[];
	static CDocTemplate* m_pDocTxt;
	static CDocTemplate* m_pDocErt;

	CString GetNextToken(const char*& pos);

	void ParseSrc(const CString&);
	CString GetSrc(const CString& ClassName=CString(""));
	CString GetSrcFromModuleString(const CModuleString& ModuleString);

	CBodyTextView* InnerFindDef(CDocument* pDoc, const CString& Class, int& line);
	int InnerFindDef(const CString& src,const CString& Class);

	//long Compile(CClassInfo* pInfo,CBLModule7* pGlobModule=NULL);
};
#endif // !defined(AFX_MODULEPARSER_H__5A24344B_CF09_41BA_BF89_F95AD6077D7F__INCLUDED_)
