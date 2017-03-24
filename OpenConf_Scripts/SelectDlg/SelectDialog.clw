; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ResourceCount=1
ClassCount=4
Class1=CSelectDialogApp
LastClass=SelectFrm
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "resource.h"
ODLFile=SelectDialog.idl
LastPage=0
Class2=SelectFrm
Class3=TryEdit
Class4=CLstCt
Resource1=IDD_SELECTFRM_DIALOG


[CLS:CSelectDialogApp]
Type=0
HeaderFile=SelectDialog.h
ImplementationFile=SelectDialog.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CSelectDialogApp

[DLG:IDD_SELECTFRM_DIALOG]
Type=1
Class=SelectFrm
ControlCount=2
Control1=IDC_EDIT1,edit,1342242816
Control2=IDC_LIST1,SysListView32,1342261533

[CLS:SelectFrm]
Type=0
HeaderFile=SelectFrm.h
ImplementationFile=SelectFrm.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[CLS:TryEdit]
Type=0
HeaderFile=TryEdit.h
ImplementationFile=TryEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=TryEdit

[CLS:CLstCt]
Type=0
HeaderFile=LstCt.h
ImplementationFile=LstCt.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CLstCt

