//////////////////////////////////////////////////////////////////////
//not export 1CHeader

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
class CMetadataContext : public CBLContext
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
	CMetadataContext m_MetadataContext;
	DWORD data2;
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

//<-CReportForm(1708/416)<-CGetDoc7(1292/1132)<-CGetDoc(160/76)<-CDocument(84/52)<-CCmdTarget(32/28)<-CObject(4/4)
class CReportForm : public CGetDoc7
{
public:
	DWORD data0;
	DWORD data1;
	DWORD data2;
	DWORD data3;
	CRepFormContext m_RepFormContext;
	CFormContext m_FormContext;
	CObject m_Object;
	DWORD data4;
	DWORD data5;
	DWORD data6;
	DWORD data7;
	CDWordArray m_DWordArray;
	DWORD data8;
	DWORD data9;
	CValue m_Value;
	DWORD data10;
	DWORD data11;
	CString m_str1;
	CGroupContext m_GroupContext;
	CRepHistory m_RepHistory;
	CItemList m_ItemList;
	DWORD data12;
	DWORD data15;
	CString m_str2;
	CString m_str3;
	DWORD data13;
	CUnknown* m_pUnk;
	CMenu7 m_Menu;
	DWORD data14;
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


class CTextOutputContext : public CBLContext
{
public:
	CBodyTextDoc* m_pBodyTextDoc;
	DWORD m_data;
	CString m_str1;
	CString m_str2;
	DWORD m_buf[5];
};

class CMainFrame : public SECMDIFrameWnd
{
public:
	DWORD m_data[133];
	
};


//<-CPictureContext(3Ch/1Ch)<-CBLContext(20h/1Ch)<-CObject(4h/4h)
class CPictureContext : public CBLContext
{
    DECLARE_DYNCREATE(CPictureContext);
public:
    CPictureHolder7 m_PictureHolder;
	DWORD m_data[3];
};

class CHitInfo
{
public:
	DWORD m_data[4];
};
