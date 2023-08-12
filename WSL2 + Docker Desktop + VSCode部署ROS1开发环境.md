# WSL2 + Docker Desktop + VSCode部署ROS1开发环境

用于能在Windows10/11下进行跨平台ros开发与测试，docker部署成功后同样适用于Linux下的ros开发环境。在Windows上直接安装ros1的体验远不如在Linux，笔者目前大部分的开发都是基于ros1，段时间内不会转向ros2，为了能实现Windows和Linux之间无缝的开发体验，决定尝试用WSL2 + Docker Desktop来部署ros1开发环境，尽管在微软官网有相关教程，但也不是一帆风顺，下面是部署（踩坑）过程。

## 使用WSL在Windows上安装Linux

[参考教程](https://learn.microsoft.com/zh-cn/windows/wsl/install)为微软官方教程，详细描述可以参考官方教程，本文档只记录在安装过程中本人没有理解官方教程的点。

### 先决条件

必须运行 Windows 10 版本 2004 及更高版本（内部版本 19041 及更高版本）或 Windows 11 才能使用以下命令。更早版本的安装教程参考[手动安装](https://learn.microsoft.com/zh-cn/windows/wsl/install-manual)。

### 安装WSL

在管理员模式下打开 PowerShell 或 Windows 命令提示符，方法是右键单击并选择“以管理员身份运行”，输入 `wsl --install` 命令，然后重启计算机。

```powershell
wsl --install
```

**这里，默认安装的Linux分发版本为Ubuntu（而且是最新的Ubuntu版本），若想更改默认安装的Linux发行版，输入`wsl --install -d <Distribution Name>`，比如我想安装Ubuntu20.04，则输入**

```powershell
wsl --install -d Ubuntu-20.04
```

`<Distribution Name>`可以输入`wsl --list --online`来查看，其中NAME字段为输入的名称。

安装完成后会提示创建unix的用户名和密码，正常创建后重启即可。

![](C:\Users\Wyj\OneDrive\图片\ubuntuinstall.png)



**注：**安装完之后可能会出现"占位程序接收到错误数据"的错误，本人出现该问题是在`wsl --update`操作后，[git](https://github.com/microsoft/WSL/issues/7570)上也有对该问题的讨论，讨论中有提到启动wsl使用过代理或加速器等软件或者win10/win11升级后, 替你安装或者升级了"Windows Subsystem for Linux Preview"也会出现此问题，解决方法如下：

1.以管理员身份启动powershell，输入

```powershell
netsh winsock reset
```

重启电脑后启动wsl， 如果仍然不能启动wsl，或者在使用代理加速器等软件后又重现此问题可以采取方法2

2.使用NoLsp.exe防止Windows 将 LSP DLL 加载到 wsl.exe 进程中NoLsp下载地址，管理员启动cmd/powershell，输入指令: “.\NoLsp.exe c:\windows\system32\wsl.exe”，返回success即可。

## Docker Desktop的安装过程与Docker的使用

安装前请确保已经安装完成WSL2，安装过程非常简单，参考[官方链接](https://learn.microsoft.com/zh-cn/windows/wsl/tutorials/wsl-containers)安装即可。

这里重点记录下目前Docker的使用经验，主要是一些命令。docker镜像可以理解为是一个已经配置好的运行环境，而容器则是镜像运行起来的实例，我们也可以在容器中进行各种库的部署，从而生成一个新的镜像。

### Docker 镜像使用

#### 查看本地镜像

通过wsl2进入到ubuntu20.04的终端，输入`docker images`可以列出本地主机上的镜像。

```bash
runoob@runoob:~$ sudo docker images           
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu              14.04               90d5884b1ee0        5 days ago          188 MB
php                 5.6                 f40e9e0f10c8        9 days ago          444.8 MB
nginx               latest              6f8d099c3adc        12 days ago         182.7 MB
mysql               5.6                 f2e8d6c772c0        3 weeks ago         324.6 MB
httpd               latest              02ef73cf1bc0        3 weeks ago         194.4 MB
ubuntu              15.10               4e3b13c8a266        4 weeks ago         136.3 MB
hello-world         latest              690ed74de00f        6 months ago        960 B
training/webapp     latest              6fae60ef3446        11 months ago       348.8 MB
```

各个选项说明:

- **REPOSITORY：**表示镜像的仓库源，也就是镜像的名称
- **TAG：**镜像的标签，用来区分同一个镜像的不同版本号
- **IMAGE ID：**镜像ID
- **CREATED：**镜像创建时间
- **SIZE：**镜像大小

#### 运行本地镜像

```bash
runoob@runoob:~$ sudo docker run -it ubuntu:15.10 /bin/bash 
root@d77ccb2e5cca:/# //表示已经进入容器内
```

参数说明：

- **-i**: 交互式操作。
- **-t**: 终端。
- **ubuntu:15.10**: 这是指用 ubuntu 15.10 版本镜像为基础来启动容器。
- **/bin/bash**：放在镜像名后的是命令，这里我们希望有个交互式 Shell，因此用的是 /bin/bash。

如果不指定一个镜像的版本标签，例如只使用 ubuntu，docker 将默认使用 ubuntu:latest 镜像。

`docker run -it ubuntu:15.10 /bin/bash` 只是最简单的运行命令，如果想让容器在运行的时候满足更多的功能，则需要添加参数，下面是一个示例：

```bash
runoob@runoob:~$ sudo docker run -it -v /home/uesrname/noetic_container_data_1:/data --device=/dev/dri --group-add video --volume=/tmp/.X11-unix:/tmp/.X11-unix  --env="DISPLAY=$DISPLAY" --env="QT_X11_NO_MITSHM=1" --name=noetic_ros_2 osrf/ros:noetic-desktop-full  /bin/bash
```

- `-v /home/nuc/noetic_container_data_1:/data`: 这是一个卷（Volume）选项，用于将本地文件系统中的 `/home/nuc/noetic_container_data_1` 目录挂载到容器中的 `/data` 目录。这样做可以实现容器与宿主机之间的文件共享。
- `--device=/dev/dri`: 这是一个设备（Device）选项，将宿主机的 `/dev/dri` 设备挂载到容器中。`/dev/dri` 目录包含了与图形渲染相关的设备文件。
- `--group-add video`: 这是一个用户组（Group）选项，将容器中的用户添加到 `video` 用户组。这样做可以在容器中访问与视频相关的设备。
- `--volume=/tmp/.X11-unix:/tmp/.X11-unix`: 这是一个卷选项，将宿主机的 `/tmp/.X11-unix` 目录挂载到容器中的相同路径。这样做可以实现容器与 X11 显示服务器的连接，使得容器中的图形应用程序可以在宿主机的显示器上显示。
- `--env="DISPLAY=$DISPLAY"`: 这是一个环境变量（Environment Variable）选项，用于设置容器中的 `DISPLAY` 环境变量的值与宿主机相同。`DISPLAY` 环境变量指定了 X11 显示服务器的地址。
- `--env="QT_X11_NO_MITSHM=1"`: 这是另一个环境变量选项，用于设置容器中的 `QT_X11_NO_MITSHM` 环境变量的值为 `1`。这个环境变量的设置可以解决一些与共享内存相关的问题。
- `--name=noetic_ros_2`: 这是为容器指定一个名称，即 `noetic_ros_2`。
- `osrf/ros:noetic-desktop-full`: 这是要运行的容器的镜像名称和标签。具体来说，这个镜像是基于 ROS（Robot Operating System） 的 `noetic-desktop-full` 版本，由 OSRF（Open Source Robotics Foundation） 提供。

#### 从镜像仓库获取新镜像

在本地主机上使用`docker run`一个不存在的镜像时 Docker 就会自动下载这个镜像。如果想预先下载这个镜像，可以使用 `docker pull` 命令来下载它。

```bash
runoob@runoob:~$ sudo docker pull ubuntu:13.10
13.10: Pulling from library/ubuntu
6599cadaf950: Pull complete 
23eda618d451: Pull complete 
f0be3084efe9: Pull complete 
52de432f084b: Pull complete 
a3ed95caeb02: Pull complete 
Digest: sha256:15b79a6654811c8d992ebacdfbd5152fcf3d165e374e264076aa435214a947a3
Status: Downloaded newer image for ubuntu:13.10
```

#### 删除本地镜像

镜像删除使用 `docker rmi` 命令：

```bash
runoob@runoob:~$ sudo docker rmi <image_name>/<image_id>
```

#### 在本地创建镜像

有两种方法：

- 从已经创建的容器中更新镜像，并且提交这个镜像
- 使用 Dockerfile 指令来创建一个新的镜像

##### 更新镜像

我们可以在已经运行的容器内进一步的更新各种库，或者是改变一些其他的配置，以上面运行的

ubuntu 15.10为例：

```bash
runoob@runoob:~$ sudo docker commit -m="has update" -a="runoob" e218edb10161 runoob/ubuntu:v2 sha256:70bf1840fd7c0d2d8ef0a42a817eb29f854c1af8f7c59fc03ac7bdee9545aff8
```

各个参数说明：

- **-m:** 提交的描述信息
- **-a:** 指定镜像作者
- **e218edb10161：**容器 ID
- **runoob/ubuntu:v2:** 指定要创建的目标镜像名和TAG
- **sha256**：在 Docker 中，SHA256 是一种哈希算法，用于对镜像进行唯一标识和验证。作用跟TAG类似，都是防止拉到自己不想要的版本的镜像，如果没有指定哈希值，则docker会自动生成一个哈希值。

##### 构建镜像

可以使用命令 **docker build** ， 从零开始来创建一个新的镜像。为此，需要创建一个 Dockerfile 文件，其中包含一组指令来告诉 Docker 如何构建我们的镜像， Dockerfile编写较为麻烦，但是更为高阶，后续有精力再学，不在此处更多的描述。

##### 设置镜像标签

```bash
runoob@runoob:~$ docker tag 860c279d2fec runoob/centos:dev
```

docker tag 镜像ID，这里是 860c279d2fec ,用户名称、镜像源名(repository name)和新的标签名(tag)。

使用 docker images 命令可以看到，ID为860c279d2fec的镜像多一个标签。

### Docker 容器使用

#### 启动容器

在执行上述`docker run`运行镜像后，会自动生成一个容器，并进入

```bash
runoob@runoob:~$ sudo docker run -it ubuntu:15.10 /bin/bash 
root@d77ccb2e5cca:/# 
```

若要退出容器，直接输入`exit`：

```bash
root@d77ccb2e5cca:/# exit
```

这样就会退出，并让容器停止运行，可以输入`docker ps -a`来查看全部的容器列表，使用`docker start container_id`来启动该容器。

在大部分的场景下，我们希望 docker 的服务是在后台运行的，我们可以过 **-d** 指定容器的运行模式。

```bash
docker run -itd --name ubuntu-test ubuntu /bin/bash
```

#### 进入容器

若容器是以后台运行的方式启动的，则需要通过命令进入到容器，可以通过以下指令进入：

- `docker attach` 
- `docker exec` ：推荐使用 `docker exec` 命令，因为此命令会退出容器终端，但不会导致容器的停止。

在这里，`docker attach` 后面直接加容器id就能进入容器的终端交互界面。对于`docker exec` ：

```bash
docker exec -it 243c32535da7 /bin/bash
```

#### 导出和导入容器

导出容器

```bash
$ docker export 1e560fca3906 > ubuntu.tar
```

导入容器为镜像

```bash
$ cat docker/ubuntu.tar | docker import - test/ubuntu:v1
```

此外，也可以通过指定 URL 或者某个目录来导入，例如：

```bash
$ docker import http://example.com/exampleimage.tgz example/imagerepo
```

#### 删除容器

```bash
$ docker rm -f 1e560fca3906
```

## 使用VScode在容器中进行开发

经过上面两步，Linux和Docker的环境已经搭好，笔者更习惯用VScode作为开发的IDE，所以下面介绍在VScode中如何进入容器内部

通过WSL2打开之前装好的Ubuntu20.04，然后运行镜像

```bash
$ docker run -itdp 3000:3000 -v /home/wyj/docker_play/noetic_container_codedata:/data --device=/dev/dri --group-add video --volume=/tmp/.X11-unix:/tmp/.X11-unix  --env="DISPLAY=$DISPLAY" --env="QT_X11_NO_MITSHM=1" --name=noetic_ros_2 ros_noetic:v230703  /bin/bash
```

这段命令比之前运行镜像多了一个参数`-p`，这个参数会在主机端口3000与容器端口3000之间创建映射，这样我们才能从VScode进入到容器中。

VScode需要安装对应的扩展

![](C:\Users\Wyj\OneDrive\图片\2.png)

![](C:\Users\Wyj\OneDrive\图片\3.png)

找到运行中的容器，然后右键点击附加Visual Studio Code

![](C:\Users\Wyj\OneDrive\图片\docker1.png)

右下角呈现链接状态

![](C:\Users\Wyj\OneDrive\图片\1.png)