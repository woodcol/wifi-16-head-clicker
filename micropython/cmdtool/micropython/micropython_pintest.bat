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

