//��������� ����� - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_STDAFX_H__CAB0F08B_DDA6_485C_92D7_909EA161BF67__INCLUDED_)
#define AFX_STDAFX_H__CAB0F08B_DDA6_485C_92D7_909EA161BF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef IsSHIFTpressed
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  (( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 ))
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxadv.h> 

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxtempl.h>
#include <afxtempl.h>
#include <afxrich.h>		// MFC rich edit classes

#include "Prof-UI\Include\Prof-UIS.h"


//Messages
#define WM_ONSELECT			WM_USER+1
#define WM_MESSAGE_FROM_2C	(WM_USER + 2)
#define WM_MYMESSAGE		(WM_USER + 3)
#define WM_CLOSEFORM		WM_USER+4
#define IDC_LIST			WM_USER+5
#define SWM_SELCHANGE		(WM_USER + 6)
#define SWM_UNSELECT		(WM_USER + 7)
#define WM_DROP_MESSAGE		(WM_USER + 8)
#define ANY_MESSAGE			WM_USER+9
#define UPDATE_PROPERTY		WM_USER+10

#define FORM_START_ID		WM_USER+100
#define FORM_FINISH_ID		WM_USER+1100
#define MENU_START_ID		WM_USER+1101
#define MENU_FINISH_ID		WM_USER+2100


//������� ����������� ������
extern class CProcUnit *afxCurrentRunModule;
//CString mUpper(CString );
inline CString mUpper(CString Str)
{
	Str.TrimRight();
	Str.TrimLeft();
	Str.MakeUpper();
	return Str;
}


//������ ������� ���������:
#define CONFIG_MODE		1
#define ENTERPRISE_MODE	2
#define START_MODE		3
extern int afxAppRunMode;
extern class CModuleManager m_RunModuleManager;//����������� �����
//����� ����������
#define METADATA_FILENAME CString("config.md2")
extern CString csCurrentExeDir;
extern CString csIBDir;


extern class CMetadataTree *afxMetaTree;//������� ������ ����������
extern class CMetaObject *afxMetaObj;	//������� ������ ����������
extern class CMetadataTree* AfxGetMetadataTree();
extern int nReturnStatus;//������ ��������
extern class CValue afxFormParam;//�������� ��� �����
extern CString afxFormPath;



#include "Resource.h"

#include <afxcview.h>
//��������� �������
//TODO

//#include "SourceUT\OXSplashWnd.h"
//#include "SourceUT\OXStaticText.h"
//#include "SourceUT\OXSizeCtrlBar.h"
//#include "SourceUT\OXFrameWndDock.h"
//#include "GuiLib\GuiLib.h"
//#include "OXSizeDlgBar.h"


//��������� ���������� �������� ��� �����-����� 1�
#include "Compiler\Value.h"
#include "Compiler\ValueList.h"
#include "Compiler\ValueListBox.h"
#include "Compiler\ValueComboBox.h"
#include "Compiler\ValueTreeCtrl.h"
#include "Compiler\ValueMetaFile.h"
#include "Compiler\ValueSQL.h"
#include "Compiler\ValueObject.h"//���
//class CModuleManager* AfxGetModuleManager();


//������ �� ������������ ������������ ��������
#define MAX_OBJECTS_LEVEL	20

//����������� ������� 1�
//#include "Functions.h"

//��������� ����������
//#include "Metadata.h"

//������� �������
#include "1CCtrl.h"


#include "enterprise.h"



#include "ObjectInspector/PropertyBar.h"



#define  ID_PICKED_TOOL0	 44444
#define VK_ENTER	13


#define CONFIG_NAME	"������������"
#define FORM_NAME CString("�����")

#define DEFAULT_LAYER_NAME	"��������"

//����� ����������� ������
void Error(int nErr,...);
#define VERSION "0.55 beta ("__TIMESTAMP__")"
//��������� ������ ������
CString FindErrorCodeLine(CString &Buffer,int nCurPos,int nOnlyCodeLine);
class CByteCode *AfxGetCurrentByteCode();
void DoError();




#ifdef _DEBUG
	#define ARC_LIB_NAME "../DebugZip/ZipArchive.lib"
#else
	#define ARC_LIB_NAME "../ReleaseZip/ZipArchive.lib"
#endif

#pragma comment( lib, ARC_LIB_NAME ) 

//#include "Prof-UI\Include\all.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CAB0F08B_DDA6_485C_92D7_909EA161BF67__INCLUDED_)
