;-------------------------------------------------------------------------------
;
;   Nullsoft Installer script for "Nebula3 SDK"
;
;   (C) 2007 RadonLabs GmbH
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;   Include Modern UI

    !include "MUI.nsh"

;-------------------------------------------------------------------------------
;   Defines
    
    !define COMPANY "Radon Labs GmbH"
    !define PRODUCT "Nebula3 SDK (Sep 2008)"
    !define SETUP   "N3SDK_Sep2008"
    !define SRCDIR "..\.."
    !define STARTMENU "$SMPROGRAMS\${PRODUCT}"
    !define EXCLUDE '/x ".svn" /x "*xbox360*" /x "*wii*" /x ".#*"' 

;-------------------------------------------------------------------------------
;   General

    Name "${PRODUCT}"
    OutFile "${SRCDIR}\${SETUP}.exe"
    XPStyle on
    CrcCheck off
    
    ; default installation directory
    InstallDir "$PROGRAMFILES\${PRODUCT}"

    ; Get installation folder from registry if available
    InstallDirRegKey HKLM "Software\${COMPANY}\${PRODUCT}" "installdir"
       
;-------------------------------------------------------------------------------
;   Interface Settings

    !define MUI_ABORTWARNING
    !define MUI_ICON "install.ico"
    !define MUI_UNICON "uninstall.ico"
    !define MUI_HEADERIMAGE
    !define MUI_HEADERIMAGE_BITMAP "header.bmp"
    !define MUI_HEADERIMAGE_UNBITMAP "header.bmp"
    !define MUI_WELCOMEFINISHPAGE_BITMAP "wizard.bmp"
    !define MUI_UNWELCOMEFINISHPAGE_BITMAP "wizard.bmp"
    !define MUI_INSTFILESPAGE_PROGRESSBAR "smooth"

;--------------------------------
;   Pages

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH
  
    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH
    
;--------------------------------
;   Languages
 
    !insertmacro MUI_LANGUAGE "English"
    
;-------------------------------------------------------------------------------
;   Installer Sections     

Section "-Everything" Section_EverythingFiles

    ; root
    SetOutPath "$INSTDIR"
    File ${EXCLUDE} "${SRCDIR}\*.msbuild"
    File ${EXCLUDE} "${SRCDIR}\*.xml"
    
    ; bin
    SetOutPath "$INSTDIR\bin"
    File /x "*xbox360*" /x "*wii*" "${SRCDIR}\bin\*.tcl"
    SetOutPath "$INSTDIR\bin\win32"
    File "${SRCDIR}\bin\win32\*.exe"
    
    ; code
    SetOutPath "$INSTDIR\code"
    File "${SRCDIR}\code\*_win32.epk"
    File "${SRCDIR}\code\*.tcl"
    File "${SRCDIR}\code\*.rules"    
    File /r ${EXCLUDE} "${SRCDIR}\code\*.cc" 
    File /r ${EXCLUDE} "${SRCDIR}\code\*.c" 
    File /r ${EXCLUDE} "${SRCDIR}\code\*.h" 
    File /r ${EXCLUDE} "${SRCDIR}\code\*.dox" 
    File /r ${EXCLUDE} "${SRCDIR}\code\*.nidl"
    SetOutPath "$INSTDIR\code\vs8.0"
    File ${EXCLUDE} "${SRCDIR}\code\vs8.0\*.sln"
    File ${EXCLUDE} "${SRCDIR}\code\vs8.0\*.vcproj"

    ; special RakNet stuff
    SetOutPath "$INSTDIR\code\extlibs\raknet"
    File /r ${EXCLUDE} "${SRCDIR}\code\extlibs\raknet\*.lib"
    File /r ${EXCLUDE} "${SRCDIR}\code\extlibs\raknet\*.pdb"

    ; doc
    SetOutPath "$INSTDIR\doc"
    File "${SRCDIR}\doc\*.chm"
    File "${SRCDIR}\doc\*.txt"
    SetOutPath "$INSTDIR\doc\doxygen"
    File ${EXCLUDE} "${SRCDIR}\doc\doxygen\*.*"
    SetOutPath "$INSTDIR\doc\nebula3"
    File ${EXCLUDE} "${SRCDIR}\doc\nebula3\*.*"

    ; schemas
    SetOutPath "$INSTDIR\schemas"
    File "${SRCDIR}\schemas\*.xsd"
    
    ; work
    SetOutPath "$INSTDIR\work"
    File /r ${EXCLUDE} "${SRCDIR}\work\*.*"

    ; export
    SetOutPath "$INSTDIR"
    File "${SRCDIR}\export.zip"
    File "${SRCDIR}\export_win32.zip"
    SetOutPath "$INSTDIR\export\db"
    File "${SRCDIR}\export\db\*.db4"
    SetOutPath "$INSTDIR\export_win32\audio"
    File "${SRCDIR}\export_win32\audio\*.xwb"

;   write registry keys
    WriteRegStr HKLM "SOFTWARE\${COMPANY}\${PRODUCT}" "installdir" "$INSTDIR"

;   write uninstall information to the registry   
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" "$INSTDIR\bin\win32\nebula.ico,0"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "-StartMenuEntries" Section_StartMenuEntries

    ; Delete Old Start Menu Shortcuts
    IfFileExists "$SMPROGRAMS\${PRODUCT}" "" +2
        RmDir /r "$SMPROGRAMS\${PRODUCT}"

    ; this sets the "current working directory" for the shortcuts
    SetOutPath "$INSTDIR\bin\win32"
    CreateDirectory "${STARTMENU}"
    ; CreateDirectory "${STARTMENU}\Lighting Test"
    ; CreateDirectory "${STARTMENU}\App Layer Test"

    ; CreateShortCut "${STARTMENU}\Lighting Test\No MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480"
    ; CreateShortCut "${STARTMENU}\Lighting Test\2x MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480 -aa Low"
    ; CreateShortCut "${STARTMENU}\Lighting Test\4x MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480 -aa Medium"
    ; CreateShortCut "${STARTMENU}\App Layer Test\No MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480"
    ; CreateShortCut "${STARTMENU}\App Layer Test\2x MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480 -aa Low"
    ; CreateShortCut "${STARTMENU}\App Layer Test\4x MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480 -aa Medium"
    ; CreateShortCut "${STARTMENU}\Connect Web Browser.lnk" "http://127.0.0.1:2100"
    CreateShortCut "${STARTMENU}\Browse Files.lnk" "$WINDIR\explorer.exe" "$INSTDIR"
    CreateShortCut "${STARTMENU}\Documentation.lnk" "$INSTDIR\doc\nebula3.chm"
    CreateShortCut "${STARTMENU}\License.lnk" "$INSTDIR\doc\license.txt"
    CreateShortCut "${STARTMENU}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
SectionEnd
    
;-------------------------------------------------------------------------------
;   Uninstaller Section  
Section "Uninstall"

;   Delete registry keys
    DeleteRegKey HKLM "SOFTWARE\${COMPANY}\${PRODUCT}"
    DeleteRegKey /ifempty HKLM "SOFTWARE\${COMPANY}"

;   Remove the installation directory
    RMDir /r "$INSTDIR"

;   Delete Start Menu Shortcuts
    RmDir  /r "$SMPROGRAMS\${PRODUCT}"
  
;   Delete Uninstaller And Unistall Registry Entries
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"  
  
SectionEnd

