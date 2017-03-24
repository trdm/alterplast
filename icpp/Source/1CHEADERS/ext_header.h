//////////////////////////////////////////////////////////////////////
//not export 1CHeader
//
//тут определеяем только классы 1С не вошедшие в экспорт
//

//<-CAllConstsContext(292/260)<-CBLContext(32/28)<-CObject(4/4)
class CAllConstsContext : public CBLContext
{
	DWORD data[65];
};

//<-CAllEnumsContext(40/8)<-CBLContext(32/28)<-CObject(4/4)
class CAllEnumsContext : public CBLContext
{
	DWORD data[2];
};

//<-CMetadataContext(36/4)<-CBLContext(32/28)<-CObject(4/4)
class CMetadataContext1 : public CBLContext
{
public:
	CMetaDataObj* m_pMetaDataObj;
	//DWORD data;
};

//<-CGeneralContext(456/424)<-CBLContext(32/28)<-CObject(4/4)
class CGeneralContext : public CBLContext
{
public:
	CAllConstsContext m_AllConstsContext;
	CAllEnumsContext m_AllEnumsContext;
	CBLContext m_UnkCont;
	CObject m_UnkObj;
	DWORD data[4];
	CMetadataContext1 m_MetadataContext;
	DWORD data2;
};

class CTableOutputContext : public CBLContext
{
public:
	CSheetDoc* m_pSheetDoc1;
	UINT m_nID;
	CSheet* m_pSheet1;
	CSheetDoc* m_pSheetDoc2;
	DWORD m_dw2;
	DWORD m_dw3;
	CSheet* m_pSheet2;
	CSheetSelection m_SheetSelection1;
	CSheetSelection m_SheetSelection2;
	CSheetSelection m_SheetSelection3;
	CSheet* m_pSheet3;
	CSheetDoc m_SheetDoc3;
	DWORD m_dw4;
	DWORD m_dw5;
	DWORD m_dw6;
	DWORD m_dw7;
	CString strFormatString;
	DWORD m_dw8;
	CString m_strFileName;
	CString m_strCaption;
	DWORD m_dw9;
	DWORD m_dw10;
	DWORD m_dw11;
	DWORD m_dw12;
	DWORD m_dw13;
};

class CTwoWaySplitter : public CSplitterWnd
{
	DWORD data;
};

//<-CRepFormContext(40/8)<-CRepFormContextDef(32/0)<-CBLContext(32/28)<-CObject(4/4)
class CRepFormContext : public CRepFormContextDef
{
	CString m_strDirName;
	CString m_strFileName;
};

class CUnknown
{
	DWORD* __vfptr;
public:
	CReportForm* m_pReportForm;
	CTemplate7* m_pTemplate;
	CSheetDoc* m_pSheetDoc;
	DWORD data1;
	DWORD data2;
	DWORD data3;
	CTableOutputContext* m_pTableOutputContext;
	CBLContext* m_pBLContext;
	CString m_str;
	DWORD data[10];
};


//<-CSbCntDoc(2204/912)<-CGetDoc7(1292/1132)<-CGetDoc(160/76)<-CDocument(84/52)<-CCmdTarget(32/28)<-CObject(4/4)
class CSbCntDoc : public CGetDoc7
{
public:
	CMenu7 m_Menu;                    //50Ch
	DWORD m_data[11];                 //514h
	CString m_str1;                   //540h
	CString m_str2;                   //544h
	CSbCntTypeDef* m_pSbCntTypeDef;   //548h
	CSbCntSet* m_pSbCntSet;           //54Ch
	CSbCntSet* m_pSbCntSet2;          //550h
	DWORD m_data2[210];               //554h
	//size 89Ch
};
class CSbCntList : public CBrowseView7
{
public:
	CTreeCtrl* m_pTreeCtrl;
	DWORD m_data[5];
};

//<-CReportForm(6ACh/1A0h)<-CGetDoc7(50Ch/46Ch)<-CGetDoc(A0h/4Ch)<-CDocument(54h/34h)<-CCmdTarget(20h/1Ch)<-CObject(4h/4h))
//
class CReportForm : public CGetDoc7
{
public:
	DWORD data0;
	DWORD data1;
	DWORD data2;
	DWORD data3;
	CRepFormContext m_RepFormContext;
	CFormContext m_FormContext;
	CGroupContext m_GroupContext;
	CRepHistory m_RepHistory;
	DWORD data2_1;
	DWORD data2_2;
	CMenu7 m_Menu;
	DWORD data2_3;
};

//<-CReportFormView(176/0)<-CBrowseView7(176/24)<-CBrowseView(152/24)<-CFormView(128/16)<-CScrollView(112/44)<-CView(68/4)<-CWnd(64/32)<-CCmdTarget(32/28)<-CObject(4/4)
class CReportFormView : public CBrowseView7
{
};

//<-CDocFormView(176/0)<-CBrowseView7(176/24)<-CBrowseView(152/24)<-CFormView(128/16)<-CScrollView(112/44)<-CView(68/4)<-CWnd(64/32)<-CCmdTarget(32/28)<-CObject(4/4)
class CDocFormView : public CBrowseView7
{
};

class CJournalList : public CBrowseView7
{
};

class COperFormView : public CBrowseView7
{
};

struct CELLDEF
{
	WORD data1;
	WORD data2;
	BYTE data3;
	BYTE m_length;
	WORD data4;
	BYTE data5;
	BYTE m_prec;
	BYTE data6;
	BYTE data7;
	BYTE data8;
	BYTE data9;
	BYTE data10;
	BYTE data11;
	BYTE data12;
	BYTE data13;
	BYTE strlen;
	CString* strName;
};

class CSheetCell_ : public CSheetFormat
{
public:
	CString m_strText;
	CString m_strFormula;
	CString m_str3;
	
	//CByteArray m_PropDataArray;
	struct CELLDEF* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

class CRangeContext : public CBLContext
{
public:
	DWORD data;
};
class CRangeContext2 : public CBLContext
{
public:
	DWORD data;
};
class CMoxelFormatContext : public CBLContext
{
public:
	DWORD data;
};

class CTextOutputContext : public CBLContext
{
public:
	CBodyTextDoc* m_pBodyTextDoc;
	DWORD m_data;
	CString m_str1;
	CString m_str2;
	DWORD m_buf[5];
};

//<-CControlBar(7Ch/3Ch)<-CWnd(40h/20h)<-CCmdTarget(20h/1Ch)<-CObject(4h/4h)
//size of CControlBar is 7Ch
//size of CTabControlBar is DCh
//size of CTabCtrl is 40h
//SECControlBar

class CTabControlBar : public CControlBar //панель окон
{
public:
	CTabCtrl m_TabCtrl; //7Ch + 40h
	DWORD m_data0[6];   //BCh + 18h
	DWORD m_data1;      //D4h
	DWORD m_data2;      //D8h
	//size DCh

	virtual ~CTabControlBar(); //0x0041C2A0
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz); //0x0041C450
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler){}; //0x0041C420
	virtual void DoPaint(CDC* pDC); //0x0041C490
	DECLARE_MESSAGE_MAP();     //0x0041C410
};


//<-CMainFrame(30Ch/214h)<-SECMDIFrameWnd(F8h/34h)<-CMDIFrameWnd(C4h/4h)<-CFrameWnd(C0h/80h)<-CWnd(40h/20h)<-CCmdTarget(20h/1Ch)<-CObject(4h/4h)
class CMainFrame : public SECMDIFrameWnd
{
public:
	SECStatusBar m_StatusBar;    //0F8h + 138h
	CTabControlBar m_ControlBar; //230h + DCh
	//size 30Ch

	DECLARE_DYNCREATE(CMainFrame);
public:
	virtual ~CMainFrame();
	virtual BOOL IsFrameWnd() const;
	DECLARE_MESSAGE_MAP();
public:
	virtual BOOL DestroyWindow();                         //0x00419040
	virtual void WinHelp(DWORD dwData, UINT nCmd);        //0x00417210
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam); //0x00417350
	virtual BOOL PreTranslateMessage(MSG* pMsg);          //0x004173D0
	virtual void GetMessageString(UINT nID, CString& rMessage) const; //0x00418DD0
};


class CAddInDll_SEventStr
{
public:
	CString m_str1;
	CString m_str2;
	CString m_str3;
};

class CAddInDll_DefinedObjectInfo: public CCmdTarget 
{
public:
	DWORD m_BUF1[0x03];
	CString m_strProgID; // 1
	DWORD m_BUF2; // 1
	CPtrArray m_Arr1; // 5
	DWORD m_BUF3; // 1
	CPtrArray m_strArr; //CAddInDll_SEventStr*
	DWORD m_BUF4[0x04];
	CString m_strRegKey;
	CAddInDll_DefinedObjectInfo()
	{
		memset(m_BUF1, 0, 0x03*sizeof(DWORD));
		m_strProgID = "";
		m_BUF2 = 0;
		m_BUF3 = 0;
		memset(m_BUF4, 0, 0x04*sizeof(DWORD));
		m_strRegKey = "";
	};
};

class CAddInDll_DefinedObject: public CObject
{
public:
	DWORD m_BUF1[0x0E];
	CAddInDll_DefinedObjectInfo m_Info;
	CAddInDll_DefinedObject()
	{
		memset(m_BUF1, 0, 0x0E*sizeof(DWORD));
	};
	virtual ~CAddInDll_DefinedObject()
	{
		
	};
};

// artbear размер 0x3c, vtable seven.dll 220E2918
// райтайм-класс CCmdTargetRuntimeClass
class CLoadedAddInDllInfo: public CCmdTarget
{
public:
	HINSTANCE m_hDllInstance;
	CString m_strPath;
	//CArray<CAddInDll_DefinedObject*, CAddInDll_DefinedObject*> m_pPtrArray;
	CTypedPtrArray<CPtrArray, CAddInDll_DefinedObject*> m_DefinedObjectsArray;
	
	typedef CTypedPtrArray<CPtrArray, CLoadedAddInDllInfo*> CPtrArray;
	static CPtrArray& GetLoadedAddInDllInfoArray()
	{ 
		CApp7* pMainApp = static_cast<CApp7*>(AfxGetApp());
		extern void RuntimeError(const char *msg, ...);

		if (!pMainApp)
			RuntimeError("CLoadedAddInDllInfo::GetLoadedAddInDllInfoArray - неудача");
		
		return *(CPtrArray*)((DWORD*)pMainApp + 0x96);
	};
	
	static CLoadedAddInDllInfo* GetCurrentAddInDllInfo()
	{
		CLoadedAddInDllInfo::CPtrArray* pPtrArray = &CLoadedAddInDllInfo::GetLoadedAddInDllInfoArray();
		
		extern HINSTANCE hDllInstance;
		int sz = pPtrArray->GetSize();
		for(int i = 0; i < sz; i++)
		{
			CLoadedAddInDllInfo* pAddInContext = pPtrArray->GetAt(i);
			if(pAddInContext && (pAddInContext->m_hDllInstance == hDllInstance))
				return pAddInContext;
		}
		return NULL;
	}
};

//<-CPictureContext(3Ch/1Ch)<-CBLContext(20h/1Ch)<-CObject(4h/4h)
class CPictureContext : public CBLContext
{
	DECLARE_DYNCREATE(CPictureContext);
public:
	CPictureHolder7 m_PictureHolder7;
	DWORD m_data[3];
};

class CHitInfo
{
public:
	DWORD m_data[4];
};

//<-CDocEditGet(4Ch/Ch)<-CEditGet(40h/Ch)<-CGetCtrl(34h/14h)<-CCmdTarget(20h/1Ch)<-CObject(4h/4h)
class IMPORT_1C CDocEditGet : public CEditGet
{
	DECLARE_DYNCREATE(CDocEditGet)

public:
	CGetDoc7* m_pGetDoc1;   //40h
	CFormCtrl* m_pFormCtrl; //44h
	DWORD m_data3;         //48h
	//size 4Ch
};
