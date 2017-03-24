// ModuleString.h: interface for the CModuleString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
#define AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModuleString : public CString  
{
public:
	CModuleString(){}
	
	CModuleString(const CString& stringSrc):CString(stringSrc) {}

	int getOffsetLines() const;

	// получу карту классов, описанных в этом файле с помощью директив //# ClassBegin и //# ClassEnd
	// количество пробелов и наличие точки с зап€той роли не играет
	// главное, чтобы //# сто€ло в начале строки и было указано им€ класса
	//
	//#  ClassBegin  ќќѕѕредок¬ќдном‘айле—ќдинаковымићетодами
	//#   ClassEnd  ќќѕѕредок¬ќдном‘айле—ќдинаковымићетодами
	//
	bool DivideTextOnClasses(CMapStringToString* pMap);
	
	// получу исходный код класса из модул€
	//
	// если есть директивы //# ClassBegin »м€ ласса и //# ClassEnd »м€ ласса, 
	//	выдаетс€ код именно этот класса
	// иначе выдаетс€ код всего модул€
	//
	void CropModuleToClass(const CString& strClassName);

	bool LoadModule(const CString& strFileName);
	bool LoadModule(const CString& strFileName, const CString& strClassName);

private:
	int m_OffsetLines;
	
	int DeleteWithLineCountTo(int posBegin);

	bool LoadModuleFromERT(const CString& strFileName);
	bool LoadModuleFromMD(const CString& strFileName);
	bool LoadModuleFromFile(const CString& strFileName);
	bool LoadModuleFromDLL(const CString& strFileName);
};

#endif // !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
