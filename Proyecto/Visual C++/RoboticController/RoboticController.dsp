# Microsoft Developer Studio Project File - Name="RoboticController" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RoboticController - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RoboticController.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RoboticController.mak" CFG="RoboticController - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RoboticController - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RoboticController - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RoboticController - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "RoboticController - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Jarboter_V_2_1\GUI\Include" /I "C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Controlador\Include" /I "C:\Jarboter_V_2_1\GUI\External\roboop\source" /I "C:\Jarboter_V_2_1\GUI\External\roboop\newmat" /I "C:\Jarboter_V_2_1\Proyecto\Visual C++\RoboticController" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_STANDARD_" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"Resource/RoboticController.res" /i "C:\Jarboter_V_2_1\GUI\Include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\Jarboter_V_2_1\GUI\External\Instalador 1.4-1\robomosp\PMDControler\PMDControler.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RoboticController - Win32 Release"
# Name "RoboticController - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\GUI\C\conversion.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\DialogoAjusteSetRobot.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\DialogoRTC.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\DialogoTechPendant.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\DialogoTRACE.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\DialogoTrajecEnvia.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\Interfaz_grafica.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\C\mem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\menuFunc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\C\messages.c
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\C\packetIO.c
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\C\Procesamiento_archivos.c
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\C\robotData.c
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\C\USBfunc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\COMMONtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\conversion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\DialogoAjusteSetRobot.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\DialogoRTC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\DialogoTechPendant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\DialogoTRACE.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\DialogoTrajecEnvia.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\Interfaz_grafica.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\mem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\menuFunc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\messages.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\packetIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\PCtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\Include\Procesamiento_archivos.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\robotData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Comunicacion\Interface_PC_Controlador\Include\USBfunc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource\aceptar.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\ajuste.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\back.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\close_gripper.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\emergency_button.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\forward.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\get.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\grafica.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\init_ejes.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\nest.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\open_gripper.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\origen.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\Play.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\reloj.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\reset.bmp
# End Source File
# Begin Source File

SOURCE=.\RoboticController.rc
# End Source File
# Begin Source File

SOURCE=.\Resource\set.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\Techbox.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\testUSB.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\traj.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\wicon.bmp
# End Source File
# End Group
# End Target
# End Project
