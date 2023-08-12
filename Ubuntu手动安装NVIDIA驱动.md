# Ubuntu手动安装NVIDIA驱动

本文档用于记录ubuntu手动安装驱动的方法，便于查阅

## 1.前置工作

### 1.1 安装驱动所需的依赖

```bash
sudo apt update   #更新软件列表
sudo apt install g++
sudo apt install gcc
sudo apt install make
```

### 1.2 卸载原有驱动

```bash
sudo apt remove --purge nvidia*
```

### 1.3 禁用nouveau驱动

```bash
sudo gedit /etc/modprobe.d/blacklist.conf
```

在末尾添加以下两行，保存并退出

```vim
blacklist nouveau
options nouveau modeset=0
```

然后在终端更新并重启

```bash
sudo update-initramfs -u
sudo reboot
```

重启后在终端输入`lsmod | grep nouveau`，若无输出则表示已经关闭nouveau

### 1.4 关闭Secure Boot

重新启动进入电脑的[BIOS](https://so.csdn.net/so/search?q=BIOS&spm=1001.2101.3001.7020)设置，关闭Secure Boot，保存后退出重启

### 1.5 下载对应驱动

查看GPU型号

```bash
lspci | grep -i nvidia
0000:01:00.0 VGA compatible controller: NVIDIA Corporation Device 2504 (rev a1)
0000:01:00.1 Audio device: NVIDIA Corporation Device 228e (rev a1)
```

根据上面的`NVIDIA Corporation Device`型号下载对应驱动,比如这里的PCI号是2504，可在[PCI Device](http://pci-ids.ucw.cz/mods/PC/10de?action=help?help=pci)查看对应的GPU型号，然后去[NVIDIA驱动官网](https://www.nvidia.cn/Download/index.aspx?lang=cn)下载驱动即可

## 2. 安装

安装需要在非图形化界面进行

```bash
sudo telinit 3  #进入文本界面
```

进入文本界面后输入用户名和密码，然后关闭gdm3的显示服务

```bash
sudo service gdm3 stop   #停止显示服务
```

进到`.run`驱动文件的下载路径，注意下载路径一定不能是中文，给文件赋予可执行权限，然后安装

```bash
sudo chmod 777 NVIDIA-Linux-x86_64-530.41.03.run
sudo ./NVIDIA-Linux-x86_64-430.64.run -no-x-check -no-nouveau-check -no-opengl-files
```

-no-x-check：安装驱动时关闭X服务 (已经关闭图形界面则不需要该指令)
-no-nouveau-check：安装驱动时禁用nouveau （已经禁用nouveau不需要该指令）
-no-opengl-files：只安装驱动文件，不安装OpenGL文件，只有禁用opengl这样安装才不会出现循环登陆的问题。（**注意，如果带上该指令之后，系统的详细信息里面没有显卡型号而是llvmpipe，且nividia-smi的Process中只有Xorg，没有默认窗口管理器，如compiz，则重装驱动，去掉该指令，一般台机带独显容易有这个问题，不加该指令安装**）

> 可通过命令wmctrl -m来查看默认的窗口管理器名称，name字段即为名称

安装中会遇到的问题

Install NVIDIA's 32-bit compatibility libraries?  NO

Would you like to run the nvidia-xconfig utility to automatically update your x configuration so that the NVIDIA x driver will be used when you restart x? Any pre-existing x confile will be backed up.   YES

安装完成后重启gdm3显示服务

```bash
sudo service gdm3 start
```

检查是否安装成功

```bash
wyj@wyjpc:~$ nvidia-smi 
Mon May 15 11:11:07 2023       
+---------------------------------------------------------------------------------------+
| NVIDIA-SMI 530.41.03              Driver Version: 530.41.03    CUDA Version: 12.1     |
|-----------------------------------------+----------------------+----------------------+
| GPU  Name                  Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf            Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                                         |                      |               MIG M. |
|=========================================+======================+======================|
|   0  NVIDIA GeForce RTX 3060         Off| 00000000:01:00.0  On |                  N/A |
| 35%   34C    P8               11W / 170W|    504MiB / 12288MiB |      7%      Default |
|                                         |                      |                  N/A |
+-----------------------------------------+----------------------+----------------------+

+---------------------------------------------------------------------------------------+
| Processes:                                                                            |
|  GPU   GI   CI        PID   Type   Process name                            GPU Memory |
|        ID   ID                                                             Usage      |
|=======================================================================================|
|    0   N/A  N/A      1135      G   /usr/lib/xorg/Xorg                           35MiB |
|    0   N/A  N/A      2006      G   /usr/lib/xorg/Xorg                          197MiB |
|    0   N/A  N/A      2135      G   /usr/bin/gnome-shell                         43MiB |
|    0   N/A  N/A      4031      G   ...91977502,3538332435134589705,262144      134MiB |
|    0   N/A  N/A      5353      G   ...sion,SpareRendererForSitePerProcess       62MiB |
|    0   N/A  N/A     10133      G   /proc/self/exe                               19MiB |
+---------------------------------------------------------------------------------------+
```

