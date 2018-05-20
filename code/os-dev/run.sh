#!/bin/bash 
echo -e "\033[31m========================== env check  ================================================== \033[0m"
if [ ! -e bochsrc.disk ];then 
    echo "no bochsrc.disk,please checkout!"
    exit 1
fi
if [ ! -e hd3M.img ];then 
    echo "no bootimage!"
    exit 1
fi

if [ -e hd50M.img ];then
    #echo -e  "n\np\n1\n2048\n50000\nn\ne\n2\n51200\n101807\nn\n53248\n101807\nw\n" | fdisk hd50M.img &> /dev/null
    echo -e  "n\np\n1\n\n+4M\nn\ne\n2\n\n\nn\n\n+5M\nn\n\n+6M\nn\n\n+7M\nn\n\n+8M\nn\n\n+9M\nn\n\n\nw\n" | fdisk hd50M.img &> /dev/null
else
    echo "no hd50M.img!"
    exit 1
fi

if [ ! -e  /usr/share/bochs/keymaps/x11-pc-us.map ];then
    echo "/usr/share/bochs/keymaps/x11-pc-us.map does not exist..."
    exit 1
else
    file /usr/share/bochs/keymaps/x11-pc-us.map
fi

if [ ! -e  /usr/share/bochs/BIOS-bochs-latest ];then
    echo " /usr/share/bochs/BIOS-bochs-latest does not exist..."
    exit 1
else 
    file /usr/share/bochs/BIOS-bochs-latest
fi

if [ ! -e  /usr/share/vgabios/vgabios.bin ];then
    echo "/usr/share/vgabios/vgabios.bin does not exist..."
    exit 1
else
    file /usr/share/vgabios/vgabios.bin
fi
echo -e "\033[31m========================== check over ================================================== \033[0m"
echo -e "\033[31m==========================   run      ================================================== \033[0m"
/usr/bin/bochs -f bochsrc.disk
