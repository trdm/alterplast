// MyADO.h: interface for the CMyADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_)
#define AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct strSubs_ {
	int m_ID;		// Идентификатор подсистемы
	int m_Check;	// помеченная
	int m_IDParent; // Идентификатор дочерней подсистемы
	CString m_Name; // Имя подсистемы 
} SubsStorage;

typedef struct strSubsGetSettings {
	int		EnableDDGroup;
	int		EnableFullPathDD;	// активировать режим перетаскивания польного пути...
	int		ShowColComm;
	int		ShowColID;
	int		ShowColType;
	int		ShowSubPanePos;		// Отображать панель подсистем 0-справа, 1-снизу, 2-слева, 3-сверху.
	int		Show_ID;			// Отображать ID - объекта в дереве

	CString strCharsTerminator; 
} SSubsGenSettings;

typedef struct strSubsDDSettings {
	bool	bCtrl;		// нажат Ctrl
	bool	bShift;		// нажат Shift
	int		iTypeMDObj; // тип объекта
	CString	strBefore;	// вставляем перед
	CString strAfter;	// вставляем после
} SubsDDSettings;



class CMyADO
{
//	friend class CXBaseContext;
public:
	CMyADO();
	virtual ~CMyADO();

public:
	int m_StateSubs;	// Состояние режима (0-пометка, 1-фильтрация)
	int m_iVersion;		// версия соответственно данных
	
	CMapStringToString m_ObjToSub;	// № подсистмы к перечню объектов
	CMapStringToOb m_SubSelfs;		// сами подсистемы.
	CMapStringToString m_SubToSub;	// отмеченые подсистемы для фильтра.....

	// Сохраняеем m_ObjToSub и m_SubSelfs
	CString m_sIBDir;
	CString m_sBinDirOC;
	CString m_sFileDataName;
	CString m_sFileSettingsName;
// настройки
	SSubsGenSettings m_Settings;

public:
	void ActivateStandartView();
	void SaveOrLoadSettings (bool bSave = true);
	void SaveOrLoadSubsSelection (bool bSave = true);
	void BuildMarkUpSubs();
	bool IsStoredObj(long nWhatIsIt);
	int m_LastIDSubs;
	bool SubsDelete (class CListBox& nListBox);
	bool SubsRename (class CListBox& nListBox);
	void FillListSubs (class CListBox& nListBox);
	bool SaveOrLoad (bool bSave, bool bClearAll = true);
	bool Init();
	bool SubsInsertNew( int nID );
	bool LineHaveMarkUpSubs (CString sSubsFromObj);
};

#endif // !defined(AFX_MYADO_H__1F860084_0FEB_4E85_80CF_54E0EAD0AFA9__INCLUDED_)
