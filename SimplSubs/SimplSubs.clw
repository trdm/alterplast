; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=11
ResourceCount=5
NewFileInclude1=#include "stdafx.h"
Class1=CMyView
LastClass=CMyListBox
LastTemplate=CCmdTarget
Class2=CMyDoc
Class3=CMainSubView
Class4=CSubsListView
Class5=CMyListBox
Resource1=IDD_SUBS_DLG
Class6=CInputStrDlg
Resource2=IDD_SETUP_DLG
Class7=CSubsView
Class8=CSubsDlg
Resource3=IDD_MY_DLG
Class9=CSubsViewMain
Class10=CMyCheckListBox
Resource4=IDD_INPUT_STR_DLG
Class11=CSetupDlg
Resource5=IDR_TOOLBAR1

[DLG:IDD_MY_DLG]
Type=1
Class=CMyView
ControlCount=0

[CLS:CMyView]
Type=0
HeaderFile=MyView.h
ImplementationFile=MyView.cpp
BaseClass=CColumnTreeView
Filter=D
VirtualFilter=VWC
LastObject=CMyView

[CLS:CMyDoc]
Type=0
HeaderFile=MyDoc.h
ImplementationFile=MyDoc.cpp
BaseClass=CDocument
Filter=N

[CLS:CMainSubView]
Type=0
HeaderFile=MainSubView.h
ImplementationFile=MainSubView.cpp
BaseClass=CTreeView
Filter=C

[CLS:CSubsListView]
Type=0
HeaderFile=SubsListView.h
ImplementationFile=SubsListView.cpp
BaseClass=CListView
Filter=C

[DLG:IDD_INPUT_STR_DLG]
Type=1
Class=CInputStrDlg
ControlCount=3
Control1=IDC_EDIT_INPUT,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[CLS:CInputStrDlg]
Type=0
HeaderFile=InputStrDlg.h
ImplementationFile=InputStrDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInputStrDlg

[DLG:IDD_SUBS_DLG]
Type=1
Class=CSubsView
ControlCount=1
Control1=IDC_SUB_LIST,listbox,1344340315

[CLS:CSubsView]
Type=0
HeaderFile=SubsView.h
ImplementationFile=SubsView.cpp
BaseClass=CFormView
Filter=D
LastObject=CSubsView
VirtualFilter=VWC

[CLS:CSubsDlg]
Type=0
HeaderFile=SubsDlg.h
ImplementationFile=SubsDlg.cpp
BaseClass=CDialog
Filter=D

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_BUTTON32771
Command2=ID_BUTTON32772
Command3=ID_BUTTON32773
Command4=ID_BUTTON32779
Command5=ID_BUTTON32780
Command6=ID_BUTTON32774
Command7=ID_BUTTON32775
Command8=ID_BUTTON32781
CommandCount=8

[CLS:CSubsViewMain]
Type=0
HeaderFile=SubsViewMain.h
ImplementationFile=SubsViewMain.cpp
BaseClass=CFrameWnd
Filter=T
VirtualFilter=fWC

[CLS:CMyCheckListBox]
Type=0
HeaderFile=MyCheckListBox.h
ImplementationFile=MyCheckListBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC
LastObject=CMyCheckListBox

[CLS:CMyListBox]
Type=0
HeaderFile=MyListBox.h
ImplementationFile=MyListBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC
LastObject=CMyListBox

[DLG:IDD_SETUP_DLG]
Type=1
Class=CSetupDlg
ControlCount=17
Control1=IDC_STATIC,button,1342177287
Control2=IDC_SHOW_COL_TYPE,button,1476460547
Control3=IDC_SHOW_COL_COMM,button,1476460547
Control4=IDC_SHOW_COL_ID,button,1476460547
Control5=IDC_STATIC,button,1342177287
Control6=IDC_ENABBLE_GR_DD,button,1342251011
Control7=IDC_STATIC_DD_GR_TERM,static,1342308352
Control8=IDC_EDT_CHAR_TERMINATOR_DD,edit,1350631552
Control9=IDC_STATIC,static,1476526080
Control10=IDC_LST_ADD_DD_TEXT,SysListView32,1484883981
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_ENABBLE_FULL_PATH_DD,button,1342251011
Control14=IDC_CMB_PLACE_SUBS,combobox,1344339971
Control15=IDC_STATIC_DD_GR_TERM2,static,1342308354
Control16=IDC_STATIC_DD_GR_TERM3,static,1476526082
Control17=IDC_SHOW_ID_TO_TREE,button,1342242819

[CLS:CSetupDlg]
Type=0
HeaderFile=SetupDlg.h
ImplementationFile=SetupDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CMB_PLACE_SUBS
VirtualFilter=dWC

