; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=4
Class1=CShapesApp
LastClass=CConeDialog
NewFileInclude2=#include "shapes.h"
ResourceCount=3
NewFileInclude1=#include "stdafx.h"
Class2=CArchDialog
LastTemplate=CDialog
Resource1=IDD_ARCH_DIALOG
Class3=CConeDialog
Resource2=IDD_CONE_DIALOG
Class4=CCylinderDialog
Resource3=IDD_CYLINDER_DIALOG

[CLS:CShapesApp]
Type=0
HeaderFile=shapes.h
ImplementationFile=shapes.cpp
Filter=N
LastObject=CShapesApp

[DLG:IDD_ARCH_DIALOG]
Type=1
Class=CArchDialog
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1350569998
Control4=IDC_STATIC,button,1342177287
Control5=IDC_ARCH_X,button,1342308361
Control6=IDC_ARCH_Y,button,1342177289
Control7=IDC_ARCH_Z,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_NUMBER_OF_SECTIONS_EDIT,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_DEPTH_EDIT,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_WIDTH_EDIT,edit,1350631552
Control15=IDC_SIZE_EDIT,edit,1350631552

[CLS:CArchDialog]
Type=0
HeaderFile=ArchDialog.h
ImplementationFile=ArchDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CArchDialog
VirtualFilter=dWC

[DLG:IDD_CONE_DIALOG]
Type=1
Class=CConeDialog
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1350569998
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CONE_X,button,1342308361
Control6=IDC_CONE_Y,button,1342177289
Control7=IDC_CONE_Z,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LENGTH_EDIT,edit,1350631552
Control12=IDC_CONE_SIZE_EDIT,edit,1350631552
Control13=IDC_NUMBER_OF_SIDES_EDIT,edit,1350631552

[CLS:CConeDialog]
Type=0
HeaderFile=ConeDialog.h
ImplementationFile=ConeDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CConeDialog

[DLG:IDD_CYLINDER_DIALOG]
Type=1
Class=CCylinderDialog
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1350569998
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CYLINDER_X,button,1342308361
Control6=IDC_CYLINDER_Y,button,1342177289
Control7=IDC_CYLINDER_Z,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SIZE_EDIT,edit,1350631552
Control11=IDC_DEPTH_EDIT,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_NUMBER_OF_SIDES_EDIT,edit,1350631552

[CLS:CCylinderDialog]
Type=0
HeaderFile=CylinderDialog.h
ImplementationFile=CylinderDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CYLINDER_X

