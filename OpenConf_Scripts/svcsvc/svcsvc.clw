; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSvcsvcApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "svcsvc.h"
LastPage=0

ClassCount=4
Class1=CFilterDlg
Class2=CMyEdit
Class3=CSelectDlg
Class4=CSvcsvcApp

ResourceCount=3
Resource1=IDD_DIALOG1
Resource2=IDD_DIALOG2
Resource3=IDD_DIALOG3

[CLS:CFilterDlg]
Type=0
BaseClass=CDialog
HeaderFile=FilterDlg.h
ImplementationFile=FilterDlg.cpp
LastObject=CFilterDlg
Filter=W
VirtualFilter=dWC

[CLS:CMyEdit]
Type=0
BaseClass=CEdit
HeaderFile=FilterDlg.h
ImplementationFile=FilterDlg.cpp
LastObject=CMyEdit
Filter=W
VirtualFilter=WC

[CLS:CSelectDlg]
Type=0
BaseClass=CDialog
HeaderFile=SelectDlg.h
ImplementationFile=SelectDlg.cpp

[CLS:CSvcsvcApp]
Type=0
BaseClass=CWinApp
HeaderFile=svcsvc.cpp
ImplementationFile=svcsvc.cpp
LastObject=CSvcsvcApp
Filter=N

[DLG:IDD_DIALOG2]
Type=1
Class=CFilterDlg
ControlCount=2
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_TREE1,SysTreeView32,1342246960

[DLG:IDD_DIALOG1]
Type=1
Class=CSelectDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG3]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

