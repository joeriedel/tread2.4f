# Microsoft Developer Studio Project File - Name="Tread3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Tread3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tread3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tread3D.mak" CFG="Tread3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tread3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Tread3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tread3D - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Tread3D - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Tread3D - Win32 Release"
# Name "Tread3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccelFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AnchorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AnchorView.cpp
# End Source File
# Begin Source File

SOURCE=.\AngleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BindDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\byteparser.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ClrDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorCycleController.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleView.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu_lib.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\daikatanareader.cpp
# End Source File
# Begin Source File

SOURCE=.\dakrle.cpp
# End Source File
# Begin Source File

SOURCE=.\dakwalreader.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultActionResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityKeyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EvaluateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterHotKeyCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FlagsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSetupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GLPatchMeshRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\HighlightComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\HollowDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\htmlviewer.cpp
# End Source File
# Begin Source File

SOURCE=.\jpegreader.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\msg.cpp
# End Source File
# Begin Source File

SOURCE=.\NighthawkMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectView.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\patchmesh.cpp
# End Source File
# Begin Source File

SOURCE=.\pk3file.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\qshaders.cpp
# End Source File
# Begin Source File

SOURCE=.\Quake3MapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickHelpDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RegisterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SelListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StartupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfaceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\targastream.cpp
# End Source File
# Begin Source File

SOURCE=.\TexComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TexListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureView.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tread3D.rc
# End Source File
# Begin Source File

SOURCE=.\Tread3DView.cpp
# End Source File
# Begin Source File

SOURCE=.\unzip.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\VisgroupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VisgroupView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccelFile.h
# End Source File
# Begin Source File

SOURCE=.\AnchorDialog.h
# End Source File
# Begin Source File

SOURCE=.\AnchorView.h
# End Source File
# Begin Source File

SOURCE=.\AngleDialog.h
# End Source File
# Begin Source File

SOURCE=.\BaseObjectRenderer.h
# End Source File
# Begin Source File

SOURCE=.\BindDialog.h
# End Source File
# Begin Source File

SOURCE=.\BrowserDialog.h
# End Source File
# Begin Source File

SOURCE=.\BuildDialog.h
# End Source File
# Begin Source File

SOURCE=.\byteparser.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\ClrDialog.h
# End Source File
# Begin Source File

SOURCE=.\ColorCycleController.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleEdit.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleView.h
# End Source File
# Begin Source File

SOURCE=.\cpu_lib.h
# End Source File
# Begin Source File

SOURCE=.\CtrlStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\CustomStatic.h
# End Source File
# Begin Source File

SOURCE=.\daikatanareader.h
# End Source File
# Begin Source File

SOURCE=.\dakrle.h
# End Source File
# Begin Source File

SOURCE=.\dakwalreader.h
# End Source File
# Begin Source File

SOURCE=.\DefaultActionResponder.h
# End Source File
# Begin Source File

SOURCE=.\DialogControl.h
# End Source File
# Begin Source File

SOURCE=.\DriveTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EntityDialog.h
# End Source File
# Begin Source File

SOURCE=.\EntityKeyDialog.h
# End Source File
# Begin Source File

SOURCE=.\entityrenderer.h
# End Source File
# Begin Source File

SOURCE=.\EvaluateDialog.h
# End Source File
# Begin Source File

SOURCE=.\FilterEdit.h
# End Source File
# Begin Source File

SOURCE=.\FilterHotKeyCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FlagsDialog.h
# End Source File
# Begin Source File

SOURCE=.\GameDialog.h
# End Source File
# Begin Source File

SOURCE=.\GameSetupDialog.h
# End Source File
# Begin Source File

SOURCE=.\GLPatchMeshRenderer.h
# End Source File
# Begin Source File

SOURCE=.\HighlightComboBox.h
# End Source File
# Begin Source File

SOURCE=.\HollowDialog.h
# End Source File
# Begin Source File

SOURCE=.\htmlviewer.h
# End Source File
# Begin Source File

SOURCE=.\jpegreader.h
# End Source File
# Begin Source File

SOURCE=.\LicenseDialog.h
# End Source File
# Begin Source File

SOURCE=.\MapDialog.h
# End Source File
# Begin Source File

SOURCE=.\ModelDialog.h
# End Source File
# Begin Source File

SOURCE=.\msg.h
# End Source File
# Begin Source File

SOURCE=.\NighthawkMapFile.h
# End Source File
# Begin Source File

SOURCE=.\ObjectDialog.h
# End Source File
# Begin Source File

SOURCE=.\ObjectView.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\patchmesh.h
# End Source File
# Begin Source File

SOURCE=.\pk3file.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDialog.h
# End Source File
# Begin Source File

SOURCE=.\q3pakbase.h
# End Source File
# Begin Source File

SOURCE=.\q3texture.h
# End Source File
# Begin Source File

SOURCE=.\qshaders.h
# End Source File
# Begin Source File

SOURCE=.\Quake3MapFile.h
# End Source File
# Begin Source File

SOURCE=.\QuickHelpDialog.h
# End Source File
# Begin Source File

SOURCE=.\RegisterDialog.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "Tread3D - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=Tread3D
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Tread3D - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=Tread3D
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScriptDialog.h
# End Source File
# Begin Source File

SOURCE=.\SelListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SetupDialog.h
# End Source File
# Begin Source File

SOURCE=.\StartupDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringDialog.h
# End Source File
# Begin Source File

SOURCE=.\SurfaceDialog.h
# End Source File
# Begin Source File

SOURCE=.\targastream.h
# End Source File
# Begin Source File

SOURCE=.\TexComboBox.h
# End Source File
# Begin Source File

SOURCE=.\TexListBox.h
# End Source File
# Begin Source File

SOURCE=.\TextureBar.h
# End Source File
# Begin Source File

SOURCE=.\TextureView.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tread3DView.h
# End Source File
# Begin Source File

SOURCE=.\unzip.h
# End Source File
# Begin Source File

SOURCE=.\ViewControl.h
# End Source File
# Begin Source File

SOURCE=.\VisgroupDialog.h
# End Source File
# Begin Source File

SOURCE=.\VisgroupView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.ico
# End Source File
# Begin Source File

SOURCE=.\res\aboutclose.ico
# End Source File
# Begin Source File

SOURCE=.\res\align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\back.ico
# End Source File
# Begin Source File

SOURCE=.\res\background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\blue_arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_arro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_explorer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\build.ico
# End Source File
# Begin Source File

SOURCE=.\res\button_images.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CheckButtons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checkbuttons2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cj_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\clear.ico
# End Source File
# Begin Source File

SOURCE=.\res\clip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close.ico
# End Source File
# Begin Source File

SOURCE=.\res\connection.ico
# End Source File
# Begin Source File

SOURCE=.\console.cur
# End Source File
# Begin Source File

SOURCE=.\res\console.cur
# End Source File
# Begin Source File

SOURCE=.\res\console2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\devtool.ico
# End Source File
# Begin Source File

SOURCE=.\res\drag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drives.bmp
# End Source File
# Begin Source File

SOURCE=.\res\entity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\forward.ico
# End Source File
# Begin Source File

SOURCE=.\res\Globe.ico
# End Source File
# Begin Source File

SOURCE=.\res\home.ico
# End Source File
# Begin Source File

SOURCE=.\res\hsplitba.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ont.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_tex.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_tab2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\keyboard.ICO
# End Source File
# Begin Source File

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\minimize.ico
# End Source File
# Begin Source File

SOURCE=.\res\modebar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\objects.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ontop.ico
# End Source File
# Begin Source File

SOURCE=.\res\palette.ico
# End Source File
# Begin Source File

SOURCE=.\res\pallete2.ico
# End Source File
# Begin Source File

SOURCE=.\res\placement.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pmove.cur
# End Source File
# Begin Source File

SOURCE=.\res\poionter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\quickhelp.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texlock.ico
# End Source File
# Begin Source File

SOURCE=.\res\textureicons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tread.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tread3D.ico
# End Source File
# Begin Source File

SOURCE=.\res\Tread3D.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Tread3DDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\treadv2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TreadV2Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vertsign.bmp
# End Source File
# Begin Source File

SOURCE=.\res\visgroupicons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vsplitba.cur
# End Source File
# Begin Source File

SOURCE=.\res\zoom.cur
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ".h;.cpp"
# Begin Source File

SOURCE=.\geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapView.cpp
# End Source File
# Begin Source File

SOURCE=.\MapView.h
# End Source File
# Begin Source File

SOURCE=.\mode.cpp
# End Source File
# Begin Source File

SOURCE=.\Mode.h
# End Source File
# Begin Source File

SOURCE=.\texport.h
# End Source File
# Begin Source File

SOURCE=.\Tread3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Tread3D.h
# End Source File
# Begin Source File

SOURCE=.\Tread3DDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Tread3DDoc.h
# End Source File
# Begin Source File

SOURCE=.\treadsys.cpp
# End Source File
# Begin Source File

SOURCE=.\treadsys.h
# End Source File
# End Group
# Begin Group "TexAPI"

# PROP Default_Filter ""
# Begin Group "Quake2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\q2pakfile.cpp
# End Source File
# Begin Source File

SOURCE=.\q2pakfile.h
# End Source File
# Begin Source File

SOURCE=.\q2texdir.cpp
# End Source File
# Begin Source File

SOURCE=.\q2texdir.h
# End Source File
# Begin Source File

SOURCE=.\q2walreader.cpp
# End Source File
# Begin Source File

SOURCE=.\q2walreader.h
# End Source File
# End Group
# Begin Group "Targa"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\targareader.cpp
# End Source File
# Begin Source File

SOURCE=.\targareader.h
# End Source File
# End Group
# Begin Group "Quake3"

# PROP Default_Filter ""
# End Group
# Begin Group "Quake"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\q1pakfile.cpp
# End Source File
# Begin Source File

SOURCE=.\q1pakfile.h
# End Source File
# Begin Source File

SOURCE=.\q1walreader.cpp
# End Source File
# Begin Source File

SOURCE=.\q1walreader.h
# End Source File
# End Group
# Begin Group "Half Life"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlpakfile.cpp
# End Source File
# Begin Source File

SOURCE=.\hlpakfile.h
# End Source File
# Begin Source File

SOURCE=.\hlwalreader.cpp
# End Source File
# Begin Source File

SOURCE=.\hlwalreader.h
# End Source File
# End Group
# Begin Group "TexDirectory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gentexdir.cpp
# End Source File
# Begin Source File

SOURCE=.\gentexdir.h
# End Source File
# End Group
# Begin Group "Sof"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sofreader.cpp
# End Source File
# Begin Source File

SOURCE=.\Sofreader.h
# End Source File
# End Group
# Begin Group "JPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcomapi.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapimin.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapistd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdatasrc.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcoefct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcolor.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jddctmgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdhuff.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdinput.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmainct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmarker.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmaster.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdpostct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdsample.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdtrans.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jerror.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctflt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctflt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemmgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemnobs.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpgload.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jutils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeg\jversion.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\pakfile.h
# End Source File
# Begin Source File

SOURCE=.\pakloader.cpp
# End Source File
# Begin Source File

SOURCE=.\pakloader.h
# End Source File
# Begin Source File

SOURCE=.\texapi.cpp
# End Source File
# Begin Source File

SOURCE=.\texapi.h
# End Source File
# Begin Source File

SOURCE=.\texdir.h
# End Source File
# Begin Source File

SOURCE=.\texpalette.h
# End Source File
# Begin Source File

SOURCE=.\texreader.cpp
# End Source File
# Begin Source File

SOURCE=.\texreader.h
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Group "Brush"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brush.cpp
# End Source File
# Begin Source File

SOURCE=.\Brush.h
# End Source File
# End Group
# Begin Group "Entity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Entity.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.h
# End Source File
# End Group
# Begin Group "Face"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Face.cpp
# End Source File
# Begin Source File

SOURCE=.\Face.h
# End Source File
# End Group
# Begin Group "Winding"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\winding.cpp
# End Source File
# Begin Source File

SOURCE=.\winding.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# End Group
# Begin Group "ClipTool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipTool.h
# End Source File
# End Group
# Begin Group "Anchor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Anchor.cpp
# End Source File
# Begin Source File

SOURCE=.\Anchor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\mapobject.cpp
# End Source File
# Begin Source File

SOURCE=.\mapobject.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\futils.cpp
# End Source File
# Begin Source File

SOURCE=.\futils.h
# End Source File
# Begin Source File

SOURCE=.\linkedlist.h
# End Source File
# Begin Source File

SOURCE=.\mathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\szstring.cpp
# End Source File
# Begin Source File

SOURCE=.\szstring.h
# End Source File
# Begin Source File

SOURCE=.\tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\tokenizer.h
# End Source File
# End Group
# Begin Group "GameAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gameapi.cpp
# End Source File
# Begin Source File

SOURCE=.\gameapi.h
# End Source File
# Begin Source File

SOURCE=.\MapBuild.cpp
# End Source File
# Begin Source File

SOURCE=.\MapBuild.h
# End Source File
# End Group
# Begin Group "RenderAPI"

# PROP Default_Filter ""
# Begin Group "WinGDI"

# PROP Default_Filter ""
# Begin Group "GDIBrush"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIBrushRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIBrushRenderer.h
# End Source File
# End Group
# Begin Group "GDIEntity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIEntityRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIEntityRenderer.h
# End Source File
# End Group
# Begin Group "GDIWinding"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIWindingRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIWindingRenderer.h
# End Source File
# End Group
# Begin Group "GDIWindowRenderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dib24.cpp
# End Source File
# Begin Source File

SOURCE=.\dib24.h
# End Source File
# Begin Source File

SOURCE=.\GDIWindowRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIWindowRenderer.h
# End Source File
# End Group
# Begin Group "GDIRenderPackage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIRenderPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIRenderPackage.h
# End Source File
# End Group
# Begin Group "GDICamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDICameraRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDICameraRenderer.h
# End Source File
# End Group
# Begin Group "GDIClipTool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIClipToolRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIClipToolRenderer.h
# End Source File
# End Group
# Begin Group "GDIAnchor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GDIAnchorRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIAnchorRenderer.h
# End Source File
# End Group
# End Group
# Begin Group "OpenGL"

# PROP Default_Filter ""
# Begin Group "GLRenderPackage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLRenderPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\GLRenderPackage.h
# End Source File
# End Group
# Begin Group "3D"

# PROP Default_Filter ""
# Begin Group "GL3DFace"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GL3DFaceRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GL3DFaceRenderer.h
# End Source File
# End Group
# Begin Group "GL3DBrush"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GL3DBrushRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GL3DBrushRenderer.h
# End Source File
# End Group
# Begin Group "GL3DWindowRenderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GL3DWindowRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GL3DWindowRenderer.h
# End Source File
# End Group
# Begin Group "GL3DEntity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GL3DEntityRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GL3DEntityRenderer.h
# End Source File
# End Group
# End Group
# Begin Group "2D"

# PROP Default_Filter ""
# Begin Group "GLWindowRenderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLWindowRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLWindowRenderer.h
# End Source File
# End Group
# Begin Group "GLFace"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLFaceRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLFaceRenderer.h
# End Source File
# End Group
# Begin Group "GLBrush"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLBrushRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLBrushRenderer.h
# End Source File
# End Group
# Begin Group "GLCamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLCameraRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCameraRenderer.h
# End Source File
# End Group
# Begin Group "GLEntity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLEntityRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLEntityRenderer.h
# End Source File
# End Group
# Begin Group "GLClipTool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLClipToolRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLClipToolRenderer.h
# End Source File
# End Group
# Begin Group "GLAnchor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLAnchorRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLAnchorRenderer.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\wogl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ColorTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTable.h
# End Source File
# Begin Source File

SOURCE=.\renderapi.cpp
# End Source File
# Begin Source File

SOURCE=.\renderapi.h
# End Source File
# End Group
# Begin Group "VisgroupAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\visgroup.cpp
# End Source File
# Begin Source File

SOURCE=.\visgroup.h
# End Source File
# End Group
# Begin Group "Event System"

# PROP Default_Filter ""
# Begin Group "HotSpot System"

# PROP Default_Filter ""
# Begin Group "CameraModeGenerator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CameraModeGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraModeGenerator.h
# End Source File
# End Group
# Begin Group "VertexModeGenerator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VertexModeGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexModeGenerator.h
# End Source File
# End Group
# Begin Group "PointerModeGenerator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PointerModeGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\PointerModeGenerator.h
# End Source File
# End Group
# Begin Group "ClipToolModeGenerator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipToolModeGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipToolModeGenerator.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\HotSpotManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HotSpotManager.h
# End Source File
# End Group
# Begin Group "Mode Change Responders"

# PROP Default_Filter ""
# Begin Group "VertexModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VertexModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexModeResponder.h
# End Source File
# End Group
# Begin Group "CameraModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CameraModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraModeResponder.h
# End Source File
# End Group
# Begin Group "EntityModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EntityModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityModeResponder.h
# End Source File
# End Group
# Begin Group "BrushModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BrushModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\BrushModeResponder.h
# End Source File
# End Group
# Begin Group "PointerModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PointerModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\PointerModeResponder.h
# End Source File
# End Group
# Begin Group "ZoomModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ZoomModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomModeResponder.h
# End Source File
# End Group
# Begin Group "PaintModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PaintModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintModeResponder.h
# End Source File
# End Group
# Begin Group "ClipToolModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipToolModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipToolModeResponder.h
# End Source File
# End Group
# Begin Group "FaceModeResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FaceModeResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceModeResponder.h
# End Source File
# End Group
# End Group
# Begin Group "Window Event Responders"

# PROP Default_Filter ""
# Begin Group "2D No. 1"

# PROP Default_Filter ""
# Begin Group "Zoom Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ZoomModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomModeWinResponder.h
# End Source File
# End Group
# Begin Group "Entity Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EntityModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityModeWinResponder.h
# End Source File
# End Group
# Begin Group "Pointer Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PointerModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\PointerModeWinResponder.h
# End Source File
# End Group
# Begin Group "Brush Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BrushModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\BrushModeWinResponder.h
# End Source File
# End Group
# Begin Group "Vertex Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VertexModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexModeWinResponder.h
# End Source File
# End Group
# Begin Group "ClipTool Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClipToolModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipToolModeWinResponder.h
# End Source File
# End Group
# Begin Group "Camera Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CameraModeWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraModeWinResponder.h
# End Source File
# End Group
# End Group
# Begin Group "3D No. 1"

# PROP Default_Filter ""
# Begin Group "3DWinResponder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3DWinResponder.cpp
# End Source File
# Begin Source File

SOURCE=.\3DWinResponder.h
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# End Group
# Begin Group "TreadMapAPI"

# PROP Default_Filter ""
# Begin Group ".trd2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TreadMap2X.cpp
# End Source File
# Begin Source File

SOURCE=.\TreadMap2X.h
# End Source File
# End Group
# Begin Group "Models"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Models.cpp
# End Source File
# Begin Source File

SOURCE=.\Models.h
# End Source File
# End Group
# Begin Group ".map"

# PROP Default_Filter ""
# Begin Group "Quake .map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\QuakeMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\QuakeMapFile.h
# End Source File
# End Group
# Begin Group "Quake2 .map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Quake2MapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Quake2MapFile.h
# End Source File
# End Group
# Begin Group "HalfLife .map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HalfLifeMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\HalfLifeMapFile.h
# End Source File
# End Group
# End Group
# Begin Group ".trd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TreadMap1X.cpp
# End Source File
# Begin Source File

SOURCE=.\TreadMap1X.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\TreadMapAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\TreadMapAPI.h
# End Source File
# End Group
# Begin Group "TargetAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TargetAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetAPI.h
# End Source File
# End Group
# Begin Group "UndoAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UndoAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoAPI.h
# End Source File
# End Group
# Begin Group "PluginAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pluginapi.cpp
# End Source File
# Begin Source File

SOURCE=.\pluginapi.h
# End Source File
# Begin Source File

SOURCE=.\TreadAPI_Template.h
# End Source File
# End Group
# Begin Group "Splash"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\TransparentWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TransparentWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\JoeLog.txt
# End Source File
# Begin Source File

SOURCE=.\JoeToDo.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Tread3D.reg
# End Source File
# End Target
# End Project
# Section Tread3D : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section Tread3D : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:107
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:106
# 	1:18:CG_IDS_TIPOFTHEDAY:105
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:110
# 	1:19:CG_IDP_FILE_CORRUPT:109
# 	1:7:IDD_TIP:104
# 	1:13:IDB_LIGHTBULB:103
# 	1:18:CG_IDS_FILE_ABSENT:108
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:TipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:TipDlg.cpp
# End Section
# Section Tread3D : {94065081-C3E3-11D2-9084-004005A2D20C}
# 	1:19:IDD_DIALOG_FILEVIEW:112
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CFolderDialog:CFolderDialog
# 	2:19:Application Include:Tread3D.h
# 	2:19:IDD_DIALOG_FILEVIEW:IDD_DIALOG_FILEVIEW
# 	2:14:folderdialog.h:folderdialog.h
# 	2:16:folderdialog.cpp:folderdialog.cpp
# End Section
