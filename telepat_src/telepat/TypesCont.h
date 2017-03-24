// TypesCont.h: interface for the CTypesCont class.
#if !defined(AFX_TYPESCONT_H__EDB7CD01_D04B_11D8_9FCD_FF4C3EFF1708__INCLUDED_)
#define AFX_TYPESCONT_H__EDB7CD01_D04B_11D8_9FCD_FF4C3EFF1708__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "blmap.h"
#include "xml2tls\TypeNodes.h"

struct STypeBlock
{
	STypeBlock():cntTypes(0),data(NULL){}
	~STypeBlock(){if(data) delete [] data;}

	int		cntTypes;
	void*	data;
	bool Load(CString path);
	void PostLoad();
};


class CTypesCont  
{
public:
	void RegisterCreatable(DWORD tName, DWORD cName);
	void IndexTypes(STypeBlock* pBlock);
	void PrepeareTypeIndex(STypeBlock* pBlock);
	void RegisterType(STTypeInfo* pType);
	void LoadTypes(BYTE Components);
	CTypesCont();
	virtual ~CTypesCont();

	STTypeInfo* GetTypeInfo(LPCTSTR type)
	{
		return (STTypeInfo*)m_Name2Type.GetKey(type);
	}
	CString& GetString(int num)
	{
		return m_strings[num];
	}
	CStringArray	m_strings;		// блок строк
	BYTE			m_components;	// типы загружены для компонент
	STypeBlock*		m_MainType;		// встроенные типы
	CMapStringToPtr	m_Path2Block;	// Список загруженных файлов

	CBLMap			m_Name2Type;	// Тип по имени 
	CBLMap			m_Name2Methods;	// Методы по имени
	CBLMap			m_Name2Attribs;	// Атрибуты по имени
	CBLMap			m_Name2Create;	// Создаваемые типы
	void*			m_IndexData;	// Массив указателей для индексов

	CDWordArray		m_CreatableNames;
	CPtrArray		m_CreatableTypes;
	
	static CTypesCont* m_pOneCont;
};

#endif // !defined(AFX_TYPESCONT_H__EDB7CD01_D04B_11D8_9FCD_FF4C3EFF1708__INCLUDED_)
