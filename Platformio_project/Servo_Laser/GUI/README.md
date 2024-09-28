Создание установочного файла setup.exe для scsgui.py на базе Inno Setup
Этот файл содержит пошаговые инструкции по созданию установочного файла setup.exe для Python-приложения scsgui.py, используя PyInstaller и Inno Setup на Linux с использованием Wine.

Шаг 1: Установка необходимых инструментов
1.1 Установите Python и необходимые пакеты
bash

sudo apt update
sudo apt install python3 python3-pip
pip3 install pyinstaller
1.2 Установка Wine
Wine позволяет запускать программы для Windows на Linux.

Для Ubuntu и Debian:

bash

sudo dpkg --add-architecture i386
sudo apt update
sudo apt install wine64 wine32
1.3 Установка Inno Setup
Скачайте установочный файл Inno Setup с официального сайта: Inno Setup Downloads.

Установите Inno Setup с помощью Wine:

bash

wine innosetup-6.2.0.exe
Обновите имя файла, если версия отличается.

Шаг 2: Упаковка scsgui.py в исполняемый файл
Перейдите в директорию, где находится scsgui.py:

bash

cd {$BASE}/Servo_Laser/GUI/
Запустите команду PyInstaller для создания автономного исполняемого файла:

bash

pyinstaller --onefile --windowed --icon=free-icon-laser-2881010.png scsgui.py
--onefile: Упаковывает все в один исполняемый файл.
--windowed: Скрывает консольное окно, так как приложение использует графический интерфейс.
--icon=free-icon-laser-2881010.png: Добавляет иконку к исполняемому файлу.
После завершения процесса в директории dist будет создан файл scsgui.exe.

Шаг 3: Создание скрипта Inno Setup
Создайте файл setup.iss в директории {$BASE}/Servo_Laser/GUI/ с следующим содержимым:

iss

; -- Inno Setup Script --

[Setup]
AppName=Servo Control GUI
AppVersion=1.0
DefaultDirName={pf}\Servo Control GUI
DefaultGroupName=Servo Control GUI
DisableProgramGroupPage=yes
OutputDir=dist\installer
OutputBaseFilename=setup
SetupIconFile=free-icon-laser-2881010.png
Compression=lzma
SolidCompression=yes

[Files]
Source: "dist\scsgui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "free-icon-laser-2881010.png"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commondesktop}\Servo Control GUI"; Filename: "{app}\scsgui.exe"; IconFilename: "{app}\free-icon-laser-2881010.png"; Tasks: desktopicon
Name: "{autoprograms}\Servo Control GUI"; Filename: "{app}\scsgui.exe"; IconFilename: "{app}\free-icon-laser-2881010.png"

[Tasks]
Name: desktopicon; Description: "Создать значок на рабочем столе"; GroupDescription: "Дополнительные значки:"; Flags: checked

[Run]
Filename: "{app}\scsgui.exe"; Description: "Запустить Servo Control GUI"; Flags: nowait postinstall skipifsilent
Убедитесь, что пути и имена файлов соответствуют вашему проекту.
Шаг 4: Компиляция установочного файла с помощью Inno Setup
Запустите компилятор Inno Setup через Wine, используя следующий команду:

bash

wine "C:\\Program Files (x86)\\Inno Setup 6\\iscc.exe" {$BASE}/Servo_Laser/GUI/setup.iss
Путь к iscc.exe может отличаться в зависимости от вашей версии Inno Setup. Убедитесь, что вы используете правильный путь.
После компиляции установочный файл setup.exe будет создан в папке dist\installer.

Шаг 5: Тестирование установочного файла
Перейдите в папку {$BASE}/Servo_Laser/GUI/dist/installer.
Скопируйте файл setup.exe на компьютер с Windows.
Запустите setup.exe и следуйте инструкциям для установки приложения.
Убедитесь, что приложение запускается и работает корректно.
Дополнительные рекомендации
Тестирование: Всегда тестируйте установочный файл на чистой системе, чтобы убедиться, что все зависимости включены.
Иконка: Убедитесь, что путь к иконке правильный, и она соответствует требованиям Windows (рекомендуется формат .ico)