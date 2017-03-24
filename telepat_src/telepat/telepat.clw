; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMethComboBox
LastTemplate=CComboBox
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "telepat.h"
LastPage=0

ClassCount=12
Class1=CHintWnd
Class2=CMyCmdTarget
Class3=CTeleList
Class4=CTelePanel
Class5=CTelepatApp
Class6=CToolTip
Class7=CTxtTrap

ResourceCount=9
Resource1=IDR_ACCELERATOR1
Resource2=IDR_TBFRAME
Resource3=IDD_CJSTACK
Resource4=IDR_TOOLBAR1
Class8=CEnumProcDlg
Resource5=IDR_ENUMPROC
Class9=CCJStack
Class10=CParamWnd
Resource6=IDR_TOOLBAR2
Resource7=IDD_ENUMPROC
Resource8=IDR_MENU1
Class11=CTeleToolBar
Class12=CMethComboBox
Resource9=IDD_PLUGIN

[CLS:CHintWnd]
Type=0
BaseClass=CWnd
HeaderFile=HintWnd.h
ImplementationFile=HintWnd.cpp
LastObject=CHintWnd
Filter=W
VirtualFilter=WC

[CLS:CMyCmdTarget]
Type=0
BaseClass=CCmdTarget
HeaderFile=MyCmdTarget.h
ImplementationFile=MyCmdTarget.cpp
LastObject=CMyCmdTarget
Filter=N
VirtualFilter=C

[CLS:CTeleList]
Type=0
BaseClass=CListBox
HeaderFile=TeleList.h
ImplementationFile=TeleList.cpp
LastObject=CTeleList
Filter=W
VirtualFilter=bWC

[CLS:CTelePanel]
Type=0
BaseClass=CWnd
HeaderFile=TelePanel.h
ImplementationFile=TelePanel.cpp
Filter=W
VirtualFilter=WC

[CLS:CTelepatApp]
Type=0
BaseClass=CWinApp
HeaderFile=telepat.cpp
ImplementationFile=telepat.cpp

[CLS:CToolTip]
Type=0
BaseClass=CWnd
HeaderFile=ToolTip.h
ImplementationFile=ToolTip.cpp

[CLS:CTxtTrap]
Type=0
BaseClass=CTextEditor
HeaderFile=txttrap.h
ImplementationFile=txttrap.cpp
Filter=W
VirtualFilter=WC
LastObject=CTxtTrap

[DLG:IDD_PLUGIN]
Type=1
Class=?
ControlCount=24
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHECK3,button,1342242819
Control3=IDC_CHECK4,button,1342242819
Control4=IDC_CHECK5,button,1342242819
Control5=IDC_CHECK15,button,1342242819
Control6=IDC_STATIC,button,1342177287
Control7=IDC_CHECK1,button,1342242819
Control8=IDC_CHECK2,button,1342242819
Control9=IDC_CHECK6,button,1342242819
Control10=IDC_STATIC,button,1342177287
Control11=IDC_CHECK7,button,1342242819
Control12=IDC_CHECK8,button,1342242819
Control13=IDC_CHECK9,button,1342242819
Control14=IDC_STATIC,button,1342177287
Control15=IDC_CHECK10,button,1342242819
Control16=IDC_CHECK11,button,1342242819
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT2,edit,1350639616
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,button,1342177287
Control22=IDC_CHECK12,button,1342242819
Control23=IDC_CHECK13,button,1342242819
Control24=IDC_CHECK14,button,1342242819

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_AUTOCOMPLETE
Command2=ID_SHOWTYPE
Command3=ID_SHOWPARAMS
Command4=ID_CODEJUMP
Command5=ID_CJBACK
Command6=ID_CJFORWARD
Command7=ID_CJSTACK
Command8=ID_METHOD_LIST
CommandCount=8

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_AUTOCOMPLETE
Command2=ID_SHOWPARAMS
Command3=ID_SHOWTYPE
Command4=ID_CODEJUMP
CommandCount=4

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=ID_METHOD_LIST_SHOW
Command2=ID_SHOWTEMPLATE
Command3=ID_SHOWTYPE
Command4=ID_PARAM_NEXT
Command5=ID_CJBACK
Command6=ID_CODEJUMP
Command7=ID_CJSTACK
Command8=ID_CJFORWARD
Command9=ID_AUTOCOMPLETE
Command10=ID_SHOWPARAMS
Command11=ID_PARAM_PREV
CommandCount=11

[DLG:IDD_ENUMPROC]
Type=1
Class=CEnumProcDlg
ControlCount=3
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_LIST1,listbox,1352728659
Control3=IDC_TOTAL,static,1342308864

[CLS:CEnumProcDlg]
Type=0
HeaderFile=EnumProcDlg.h
ImplementationFile=EnumProcDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=CEnumProcDlg
VirtualFilter=dWC

[DLG:IDD_CJSTACK]
Type=1
Class=CCJStack
ControlCount=2
Control1=IDC_LIST1,listbox,1352728833
Control2=IDC_TEXT,static,1342308364

[CLS:CCJStack]
Type=0
HeaderFile=CJStack.h
ImplementationFile=CJStack.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCJStack

[CLS:CParamWnd]
Type=0
HeaderFile=ParamWnd.h
ImplementationFile=ParamWnd.cpp
BaseClass=CWnd
Filter=W
LastObject=CParamWnd
VirtualFilter=WC

[TB:IDR_TOOLBAR2]
Type=1
Class=?
Command1=ID_SORT
Command2=ID_FILTER
CommandCount=2

[ACL:IDR_ENUMPROC]
Type=1
Class=?
Command1=ID_FILTER
Command2=ID_SORT
CommandCount=2

[TB:IDR_TBFRAME]
Type=1
Class=?
Command1=ID_JUMPMETHOD
Command2=ID_JUMP_ANCHOR
CommandCount=2

[CLS:CTeleToolBar]
Type=0
HeaderFile=TeleToolBar.h
ImplementationFile=TeleToolBar.cpp
BaseClass=CToolBarCtrl
Filter=W

[CLS:CMethComboBox]
Type=0
HeaderFile=MethComboBox.h
ImplementationFile=MethComboBox.cpp
BaseClass=CComboBox
Filter=W
VirtualFilter=cWC

