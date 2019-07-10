# Microsoft Developer Studio Project File - Name="shapes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=shapes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shapes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shapes.mak" CFG="shapes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shapes - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "shapes - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shapes - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "shapes - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "shapes - Win32 Release"
# Name "shapes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArchDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CylinderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\shapeplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\shapes.cpp
# End Source File
# Begin Source File

SOURCE=.\shapes.def
# End Source File
# Begin Source File

SOURCE=.\shapes.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArchDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConeDialog.h
# End Source File
# Begin Source File

SOURCE=.\CylinderDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShapeCreator.h
# End Source File
# Begin Source File

SOURCE=.\shapeplugin.h
# End Source File
# Begin Source File

SOURCE=.\shapes.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arch.bmp
# End Source File
# Begin Source File

SOURCE=.\cone.bmp
# End Source File
# Begin Source File

SOURCE=.\cylinder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shapes.rc2
# End Source File
# End Group
# Begin Group "TreadAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\futils.cpp
# End Source File
# Begin Source File

SOURCE=..\futils.h
# End Source File
# Begin Source File

SOURCE=..\Hooksys.cpp
# End Source File
# Begin Source File

SOURCE=..\Hooksys.h
# End Source File
# Begin Source File

SOURCE=..\linkedlist.h
# End Source File
# Begin Source File

SOURCE=..\Mathlib.cpp
# End Source File
# Begin Source File

SOURCE=..\Mathlib.h
# End Source File
# Begin Source File

SOURCE=..\szstring.cpp
# End Source File
# Begin Source File

SOURCE=..\szstring.h
# End Source File
# Begin Source File

SOURCE=..\Texport.h
# End Source File
# Begin Source File

SOURCE=..\tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\tokenizer.h
# End Source File
# Begin Source File

SOURCE=..\TreadAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\TreadAPI.h
# End Source File
# Begin Source File

SOURCE=..\wogl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\Tread3D.lib
# End Source File
# End Target
# End Project
