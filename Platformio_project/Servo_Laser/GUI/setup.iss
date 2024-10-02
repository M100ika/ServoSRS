[Setup]
AppName=Servo Control GUI
AppVersion=1.0
DefaultDirName={pf}\Servo Control GUI
DefaultGroupName=Servo Control GUI
DisableProgramGroupPage=yes
OutputDir=dist\installer
OutputBaseFilename=setup
SetupIconFile=laserbeam_5476.ico
Compression=lzma
SolidCompression=yes

[Files]
Source: "dist\scsgui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "laserbeam_5476.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commondesktop}\Servo Control GUI"; Filename: "{app}\scsgui.exe"; IconFilename: "{app}\laserbeam_5476.ico"; Tasks: desktopicon
Name: "{autoprograms}\Servo Control GUI"; Filename: "{app}\scsgui.exe"; IconFilename: "{app}\laserbeam_5476.ico"

[Tasks]
Name: desktopicon; Description: "Создать значок на рабочем столе"; GroupDescription: "Дополнительные значки:"

[Run]
Filename: "{app}\scsgui.exe"; Description: "Запустить Servo Control GUI"; Flags: nowait postinstall skipifsilent