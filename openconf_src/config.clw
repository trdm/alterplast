; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPanelSetupDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "config.h"
LastPage=0

ClassCount=19
Class1=CActiveDoc
Class2=CFireDlg
Class3=CFireWnd
Class4=CFormCnt
Class5=CFormDlg
Class6=CFormDoc
Class7=CFormScript
Class8=CLeftTree
Class9=CMacrosDlg
Class10=COCDoc
Class11=COCMenu
Class12=COCView
Class13=COleFormsView
Class14=CPluginPageDoc
Class15=CPluginPageView

ResourceCount=29
Resource1=128
Resource2=IDD_EMPTY
Resource3=IDR_PANEL_MENU_4
Resource4=IDR_PANEL_MENU_7
Resource5=IDR_MDI
Resource6=IDD_PANELS_SETTING
Resource7=IDD_MACROS
Resource8=IDR_PANEL_MENU_9
Class16=CCustomCmdDlg
Resource9=IDR_MDI1
Resource10=IDR_PANEL_MENU_6
Resource11=IDR_PANEL_MENU_2
Class17=CHotKeyDlg
Resource12=IDR_PANEL_MENU_13
Class18=CPanelView
Resource13=IDR_OPENCONF
Resource14=IDR_PANEL_MENU_12
Resource15=IDD_HOTKEYS
Resource16=IDR_PANEL_MENU_5
Resource17=IDD_CUSTOM_CMD
Resource18=IDR_PANEL_MENU_15
Resource19=IDR_MDI2
Resource20=IDR_PANEL_MENU_10
Resource21=IDR_PANEL_MENU_14
Resource22=IDR_PANEL_MENU_3
Resource23=IDR_PANEL_MENU_8
Resource24=IDR_PANEL_MENU_11
Resource25=IDR_CUSTOMCMD_DLG_TB
Resource26=IDD_FIREWND1
Resource27=IDR_PANEL_MENU_1
Resource28=IDR_PANEL_MENU_16
Class19=CPanelSetupDlg
Resource29=IDR_CCMD

[CLS:CActiveDoc]
Type=0
BaseClass=COleDocument
HeaderFile=ActiveDoc.h
ImplementationFile=ActiveDoc.cpp
LastObject=CActiveDoc

[CLS:CFireDlg]
Type=0
BaseClass=CDialog
HeaderFile=FireDlg.h
ImplementationFile=FireDlg.cpp

[CLS:CFireWnd]
Type=0
BaseClass=CStatic
HeaderFile=FIREWND.H
ImplementationFile=FIREWND.CPP

[CLS:CFormCnt]
Type=0
BaseClass=CView
HeaderFile=FormCnt.h
ImplementationFile=FormCnt.cpp
Filter=W
VirtualFilter=VWC

[CLS:CFormDlg]
Type=0
BaseClass=CDialog
HeaderFile=FormDlg.h
ImplementationFile=FormDlg.cpp

[CLS:CFormDoc]
Type=0
BaseClass=CDocument
HeaderFile=FormDoc.h
ImplementationFile=FormDoc.cpp

[CLS:CFormScript]
Type=0
BaseClass=COleDocument
HeaderFile=FormScript.h
ImplementationFile=FormScript.cpp
Filter=N
VirtualFilter=ODC

[CLS:CLeftTree]
Type=0
BaseClass=CTreeCtrl
HeaderFile=LeftTree.h
ImplementationFile=LeftTree.cpp

[CLS:CMacrosDlg]
Type=0
BaseClass=CDialog
HeaderFile=MacrosDlg.h
ImplementationFile=MacrosDlg.cpp
Filter=D
VirtualFilter=dWC

[CLS:COCDoc]
Type=0
BaseClass=CCmdTarget
HeaderFile=OCDoc.h
ImplementationFile=OCDoc.cpp

[CLS:COCMenu]
Type=0
BaseClass=CFrameWnd
HeaderFile=OCMenu.h
ImplementationFile=OCMenu.cpp
LastObject=COCMenu
Filter=W
VirtualFilter=fWC

[CLS:COCView]
Type=0
BaseClass=CView
HeaderFile=OCView.h
ImplementationFile=OCView.cpp

[CLS:COleFormsView]
Type=0
BaseClass=CView
HeaderFile=OleFormsView.h
ImplementationFile=OleFormsView.cpp

[CLS:CPluginPageDoc]
Type=0
BaseClass=CDocument
HeaderFile=PluginPageDoc.h
ImplementationFile=PluginPageDoc.cpp

[CLS:CPluginPageView]
Type=0
BaseClass=CView
HeaderFile=PluginPageView.h
ImplementationFile=PluginPageView.cpp
Filter=C
VirtualFilter=VWC

[DLG:IDD_FIREWND1]
Type=1
Class=CFireDlg
ControlCount=2
Control1=IDC_FIRE,static,1342308352
Control2=IDC_VERSION,static,1342308353

[DLG:IDD_EMPTY]
Type=1
Class=CFormCnt
ControlCount=0

[DLG:IDD_MACROS]
Type=1
Class=CMacrosDlg
ControlCount=3
Control1=IDC_TREE1,SysTreeView32,1350631431
Control2=IDOK,button,1342242816
Control3=IDCANCEL,button,1342242816

[MNU:IDR_MDI]
Type=1
Class=?
Command1=ID_DEBUG_CONSTRUCT
Command2=ID_SHOW_TEXT
CommandCount=2

[MNU:IDR_MDI1]
Type=1
Class=?
CommandCount=0

[MNU:IDR_MDI2]
Type=1
Class=?
CommandCount=0

[TB:128]
Type=1
Class=?
Command1=ID_LOAD_SCRIPT
Command2=ID_CUSTOMIZE
CommandCount=2

[DLG:IDD_CUSTOM_CMD]
Type=1
Class=CCustomCmdDlg
ControlCount=2
Control1=IDC_LIST1,SysListView32,1350631949
Control2=IDOK,button,1342242817

[CLS:CCustomCmdDlg]
Type=0
HeaderFile=CustomCmdDlg1.h
ImplementationFile=CustomCmdDlg1.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCustomCmdDlg

[TB:IDR_CUSTOMCMD_DLG_TB]
Type=1
Class=?
Command1=ID_NEW_CCMD
Command2=ID_DEL_CCMD
Command3=ID_EDIT_CCMD
Command4=ID_MACROS_CCMD
Command5=ID_UP_CCMD
Command6=ID_DOWN_CCMD
CommandCount=6

[ACL:IDR_CCMD]
Type=1
Class=?
Command1=ID_DEL_CCMD
Command2=ID_DOWN_CCMD
Command3=ID_EDIT_CCMD
Command4=ID_MACROS_CCMD
Command5=ID_NEW_CCMD
Command6=ID_UP_CCMD
CommandCount=6

[DLG:IDD_HOTKEYS]
Type=1
Class=CHotKeyDlg
ControlCount=6
Control1=IDC_LIST1,SysListView32,1350631437
Control2=IDC_HOTKEY1,msctls_hotkey32,1350631424
Control3=IDC_BUTTON1,button,1342242816
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_TEXT,static,1342308864

[CLS:CHotKeyDlg]
Type=0
HeaderFile=HotKeyDlg.h
ImplementationFile=HotKeyDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CHotKeyDlg

[CLS:CPanelView]
Type=0
HeaderFile=PanelView.h
ImplementationFile=PanelView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CPanelView

[MNU:IDR_PANEL_MENU_1]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_2]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_3]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_4]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_5]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_6]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_7]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_8]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_9]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_10]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_11]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_12]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_13]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_14]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_15]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[MNU:IDR_PANEL_MENU_16]
Type=1
Class=?
Command1=ID_MENUITEM32797
CommandCount=1

[DLG:IDD_PANELS_SETTING]
Type=1
Class=CPanelSetupDlg
ControlCount=10
Control1=IDC_STATIC,static,1342308865
Control2=IDC_TREE1,SysTreeView32,1350635567
Control3=IDC_STATIC,static,1342308865
Control4=IDC_LIST1,SysListView32,1350647903
Control5=IDC_ADD_PANEL,button,1342242816
Control6=IDC_REMOVE_PANEL,button,1342242816
Control7=IDC_MOVE_LEFT,button,1342242816
Control8=IDC_MOVE_RIGHT,button,1342242816
Control9=IDOK,button,1342242816
Control10=IDCANCEL,button,1342242816

[CLS:CPanelSetupDlg]
Type=0
HeaderFile=PanelSetupDlg.h
ImplementationFile=PanelSetupDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPanelSetupDlg
VirtualFilter=dWC

[MNU:IDR_OPENCONF]
Type=1
Class=?
Command1=ID_CUSTOMIZE
Command2=ID_SET_HOTKEYS
Command3=ID_OCFORM_NEW
Command4=ID_OCFORM_EDIT
Command5=ID_OCFORM_RUN
Command6=ID_OCFORM_LASTEDIT
Command7=ID_OCFORM_LASTRUN
Command8=ID_SETUP_PANELS
CommandCount=8

