echo "upload touch2pin"

cd "E:\os\Gitee\wifi-16-head-clicker\micropython\cmdtool\micropython\touch2pin"
ampy.exe -p com6 put boot.py
ampy.exe -p com6 put main.py
ampy.exe -p com6 put config.txt

cd "E:\os\Gitee\wifi-16-head-clicker\micropython\cmdtool\micropython"
python comreset.py
