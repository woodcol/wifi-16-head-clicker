pip install esptool -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install adafruit-ampy -i https://pypi.tuna.tsinghua.edu.cn/simple
@REM esptool.exe -p com6 -b 115200 erase_flash

::���뵱ǰĿ�ű�Ŀ¼
cd /d %~dp0



::�����Ƿ���esptool��װ�ɹ�
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

%findcmd% -b 115200 erase_flash
%findcmd% --chip esp8266 -b 115200 --after soft_reset write_flash 0x00000 bemfa_config.ino.bin
