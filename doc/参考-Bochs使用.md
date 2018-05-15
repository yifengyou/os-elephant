<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [参考-Bochs使用](#参考-bochs使用)
	- [bochs安装](#bochs安装)
		- [源码安装](#源码安装)
		- [apt安装](#apt安装)
	- [bximage创建启动存储镜像](#bximage创建启动存储镜像)
		- [创建硬盘](#创建硬盘)
		- [创建软盘](#创建软盘)
	- [运行bochs模拟器](#运行bochs模拟器)
		- [模拟器配置文件](#模拟器配置文件)
		- [模拟器运行](#模拟器运行)
	- [Bochs调试技巧](#bochs调试技巧)
	- [总结](#总结)

<!-- /TOC -->
# 参考-Bochs使用


## bochs安装

### 源码安装

tar zxvf bochs-2.6.2.tar.gz
cd bochs-2.6.2

//bochs安装在/root/bochs目录下

```
./configure --prefix=/root/bochs/ --enable-debugger --enable-disasm --enable-iodebug --enable-x86-debugger --with-x --with-x11
make
make install
```

* --prefix=/PATH 指定软件安装目录
* --enable-debugger 打开bochs自己的调试器
* --enable-disasm 启用反编译支持
* --enable-iodebug 启用IO接口调试器
* --enable-x86-debugger 支持x86调试
* --with-x 启用X windows
* --with-x11 启用X11图形界面接口

生成bochs的配置文件bochsrc.disk根据bochs安装目录中自带的模板改改就行


### apt安装

```
apt-get update
apt-get install bochs bochsbios bochs-* -y
```

## bximage创建启动存储镜像

### 创建硬盘

    bximage -hd -mode="flat" -size=60 -q hd60M.img

### 创建软盘

## 运行bochs模拟器

### 模拟器配置文件

### 模拟器运行

执行bochs
bin/bochs -f bochsrc


写成脚本，一键运行

```

  #!/bin/bash
  /usr/bin/bochs -f bochsrc

```


## Bochs调试技巧



## 总结
