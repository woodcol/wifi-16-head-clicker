pip install esptool -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install adafruit-ampy -i https://pypi.tuna.tsinghua.edu.cn/simple
@REM esptool.exe -p com6 -b 115200 erase_flash

::进入当前目脚本目录
cd /d %~dp0

::查找是否有esptool安装成功
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
%findcmd% --chip esp8266 --baud 115200 write_flash --flash_size=detect 0 "esp8266-1m-20220117-v1.18.bin"
::esptool.exe --chip esp8266 --port com6 --baud 115200 write_flash --flash_size=detect 0 "esp8266-1m-20220117-v1.18.bin"


echo "upload file"
python.exe uploadpyfile.py

upload.bat

cd /d %~dp0

::重启设备
%findcmd% run
