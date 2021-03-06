; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{02D022E0-659B-41CA-AF18-A4A00A933C8B}
AppName=貪吃蛇的危機
AppVersion=1.0.0
;AppVerName=Slither_lite 0.9.0
AppPublisher=家瀚工具股份有限公司
AppPublisherURL=http://www.ical.tw/
AppSupportURL=http://www.ical.tw/
AppUpdatesURL=http://www.ical.tw/
DefaultDirName={userdesktop}\Slither_Lite
DisableProgramGroupPage=yes
InfoBeforeFile=D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\CopyRight.md
LicenseFile=D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\同意條款.txt
OutputBaseFilename=貪吃蛇的危機_Setup
SetupIconFile=D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\assets\icon.ico
SolidCompression=yes
Uninstallable=yes
PrivilegesRequired=lowest
AllowUNCPath=False
ShowTasksTreeLines=True
VersionInfoVersion=1.0.0
VersionInfoCompany=家瀚工具股份有限公司
VersionInfoDescription=家瀚工具竭誠為您服務
VersionInfoTextVersion=V1.0.0
VersionInfoProductName=貪吃蛇的危機
VersionInfoProductVersion=1.0.0
VersionInfoProductTextVersion=V1.0.0
WizardSmallImageFile=D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\assets\icon.bmp
WizardImageBackColor=clWhite
WizardImageFile=D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\wizmodernimage.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "ChineseTraditional" ; MessagesFile: "compiler:Languages\ChineseTraditional.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\bin\Debug\Final_Project_Slither_lite_allegro.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\assets\*"; DestDir: "{app}\assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\CodeBlocks_HW\Final_Project_Slither_lite_allegro\src\*"; DestDir: "{app}\src"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\Slither_lite"; Filename: "{app}\Final_Project_Slither_lite_allegro.exe"
Name: "{userdesktop}\Slither_lite"; Filename: "{app}\Final_Project_Slither_lite_allegro.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Final_Project_Slither_lite_allegro.exe"; Description: "{cm:LaunchProgram,Slither_lite}"; Flags: nowait postinstall skipifsilent

