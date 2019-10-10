# Microsoft Developer Studio Project File - Name="Teshyria" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Teshyria - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Teshyria.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Teshyria.mak" CFG="Teshyria - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Teshyria - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Teshyria - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Teshyria - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib dsound.lib winmm.lib dinput.lib amstrmid.lib strmbase.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Teshyria - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib ddraw.lib winmm.lib dinput.lib dsound.lib amstrmid.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Teshyria - Win32 Release"
# Name "Teshyria - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Animal.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation.cpp
# End Source File
# Begin Source File

SOURCE=.\Assist.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMng.cpp
# End Source File
# Begin Source File

SOURCE=.\CharObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DM6Mp3.cpp
# End Source File
# Begin Source File

SOURCE=.\DM6Show.cpp
# End Source File
# Begin Source File

SOURCE=.\Dx7Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Dx7Input.cpp
# End Source File
# Begin Source File

SOURCE=.\Dx7Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\function.cpp
# End Source File
# Begin Source File

SOURCE=.\IDdata.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MainChar.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\MapImg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MstKnight.cpp
# End Source File
# Begin Source File

SOURCE=.\OurKnight.cpp
# End Source File
# Begin Source File

SOURCE=.\OurMage.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\RangerKnight.cpp
# End Source File
# Begin Source File

SOURCE=.\RangerMage.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoutKnight.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoutMage.cpp
# End Source File
# Begin Source File

SOURCE=.\Teshyria.rc
# End Source File
# Begin Source File

SOURCE=.\TextData.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Animal.h
# End Source File
# Begin Source File

SOURCE=.\Animation.h
# End Source File
# Begin Source File

SOURCE=.\Assist.h
# End Source File
# Begin Source File

SOURCE=.\CharMng.h
# End Source File
# Begin Source File

SOURCE=.\CharObj.h
# End Source File
# Begin Source File

SOURCE=.\DM6Mp3.h
# End Source File
# Begin Source File

SOURCE=.\DM6Show.h
# End Source File
# Begin Source File

SOURCE=.\Dx7Draw.h
# End Source File
# Begin Source File

SOURCE=.\Dx7Input.h
# End Source File
# Begin Source File

SOURCE=.\Dx7Sound.h
# End Source File
# Begin Source File

SOURCE=.\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\function.h
# End Source File
# Begin Source File

SOURCE=.\IDdata.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceMng.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItfDefine.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\MainChar.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\MapImg.h
# End Source File
# Begin Source File

SOURCE=.\MapMng.h
# End Source File
# Begin Source File

SOURCE=.\MstKnight.h
# End Source File
# Begin Source File

SOURCE=.\NeedStruct.h
# End Source File
# Begin Source File

SOURCE=.\OurKnight.h
# End Source File
# Begin Source File

SOURCE=.\OurMage.h
# End Source File
# Begin Source File

SOURCE=.\PathFinder.h
# End Source File
# Begin Source File

SOURCE=.\RangerKnight.h
# End Source File
# Begin Source File

SOURCE=.\RangerMage.h
# End Source File
# Begin Source File

SOURCE=.\ScoutKnight.h
# End Source File
# Begin Source File

SOURCE=.\ScoutMage.h
# End Source File
# Begin Source File

SOURCE=.\TextData.h
# End Source File
# Begin Source File

SOURCE=.\Textindex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Teshyria.ico
# End Source File
# End Group
# End Target
# End Project
