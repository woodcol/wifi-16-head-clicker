esptool.exe -p com6 -b 115200 erase_flash
cd "E:\os\Gitee\wifi-16-head-clicker\micropython\cmdtool\micropython"
esptool.exe --chip esp8266 --port com6 --baud 115200 write_flash --flash_size=detect 0 esp8266-1m-20220117-v1.18.bin
echo "upload file"
cd "E:\os\Gitee\wifi-16-head-clicker\micropython\cmdtool\micropython\comtest"
ampy.exe -p com6 put __upload.py
ampy.exe -p com6 put __download.py
ampy.exe -p com6 put boot.py
ampy.exe -p com6 put main.py
ampy.exe -p com6 put touchDriver.py
ampy.exe -p com6 put webrepl_cfg.py
cd "E:\os\Gitee\wifi-16-head-clicker\micropython\cmdtool\micropython"
python comreset.py