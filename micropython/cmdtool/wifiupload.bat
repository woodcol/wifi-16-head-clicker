esptool.exe -p com6 -b 115200 erase_flash
esptool.exe --chip esp8266 --port com6 -b 115200 --after soft_reset write_flash 0x00000 "E:\os\Gitee\wifi-16-head-clicker\wifi�����esp8266�̼�\wifiTouch_20220707.ino.bin" 
pause