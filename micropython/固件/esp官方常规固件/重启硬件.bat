::重启设备

@echo off
set  findcmd=esptool.py.exe
for %%I in ( %findcmd% ) do IF EXIST %%~$PATH:I ( echo %%~$PATH:I&goto end ) ELSE ( echo %%I is not found in PATH!&goto nopyexe )
:nopyexe
@echo off
set findcmd=esptool.exe
for %%I in ( %findcmd% ) do IF EXIST %%~$PATH:I ( echo %%~$PATH:I&goto end ) ELSE ( echo %%I is not found in PATH!&goto noexe )
:noexe
echo "not find esptool"
pause
exit /b "erro not find esptool.exe or esptool.py.exe"
:end

%findcmd% run