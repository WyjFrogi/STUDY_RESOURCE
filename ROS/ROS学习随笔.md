# ROS学习随笔

本文档用于记录ROS学习中的一些小tips，便于查阅

- **创建工作空间、初始化以及功能包的创建**

```bash
$ mkdir catkin_ws/src
$ cd src
$ catkin_init_workspace #初始化，在src下生成CMakeLists文件
$ cd ~/catkin_ws/
$ catkin_make 
$ cd ~/catkin_ws/src/
$ catkin_create_pkg package_name std_msgs roscpp rospy #创建功能包并添加依赖
$ cd ~/catkin_ws/
$ catkin_make
```

- **指定编译功能包**

```bash
$ catkin_make -DCATKIN_WHITELIST_PACKAGES=package name
```

- **rosbag与rostopic的使用**

在ROS中，将bag包播放之后（播包`rosbag play bag_name`），可以使用以下命令查看topic的详细信息：

1. `rostopic list`：列出当前ROS系统中所有可用的topic。
2. `rostopic info <topic_name>`：显示指定topic的详细信息，包括topic名称、数据类型、发布者和订阅者等。
3. `rostopic echo <topic_name>`：在终端上显示指定topic的数据，以便实时查看topic数据的变化情况。
4. `rostopic hz <topic_name>`：显示指定topic的发布频率，即每秒钟发布的消息数量。
5. `rostopic bw <topic_name>`：显示指定topic的带宽使用情况，即每秒钟发布的数据量。

例如，假设有一个名为`/robot/pose`的topic，可以使用以下命令查看该topic的详细信息：

```bash
$ rostopic info /robot/pose
```

输出的信息包括topic名称、数据类型、发布者和订阅者等。如果想实时查看该topic的数据，可以使用以下命令：

```shell
$ source devel/setup.bash #配置环境变量
$ rostopic echo /robot/pose
```

这样可以在终端上显示该topic的数据，以便实时查看数据的变化情况。如果只是想看topic的最新数据，可以使用`-c`选项。例如，以下命令将可以使得发送完一条信息后自动清屏发送并自动发送下条数据：

```bash
$ rostopic echo -c /robot/pose
```

如果只想查看topic的指定变量，以下命令将只显示`/robot/pose`topic的`x`变量：

```bash
$ rostopic echo /robot/pose/x
```

补充：ros中打印msg，在终端输入

```bash
$ rosmsg show sensor_msgs/LaserScan
```

如果想查看topic的frame_id，在终端输入

```bash
$ rostopic echo /topic_name | grep frame_id
```

显示bag包中包含的topic：

```bash
$ rostopic list -b /bagfile.bag
```



- **ros中的私有句柄与普通句柄**

在ROS中，私有句柄（Private NodeHandle）是一种特殊的ROS节点句柄，它可以让ROS节点在其命名空间内仅与自己发布和订阅话题、服务和参数服务器，而不会影响其他节点的操作。私有句柄可以使用`ros::NodeHandle`的构造函数来创建，只需将其命名空间作为参数传递即可。

使用私有句柄的好处是可以使节点之间的通信更加独立和安全。例如，如果节点使用的是普通句柄（默认构造函数创建的），它会将所有话题、服务和参数都发布到全局命名空间中，这可能会与其他节点产生冲突或造成不必要的混淆。使用私有句柄可以将节点的操作限制在其命名空间内，从而避免这些问题。

在C++中，私有句柄的创建方式类似于普通句柄。例如，下面的代码使用私有句柄来创建一个ROS订阅器：

```perl
ros::NodeHandle private_nh("~");   // 创建私有句柄，命名空间为当前节点的命名空间
ros::Subscriber sub = private_nh.subscribe("topic_name", 1, callback);
```

在上面的例子中，`ros::NodeHandle`的构造函数的参数`"~"`表示使用当前节点的命名空间作为私有命名空间。这样，创建的私有句柄将只能在该命名空间内访问和发布/订阅话题，而无法影响其他节点的操作。

普通句柄和私有句柄的区别在于，普通句柄使用全局命名空间来发布和订阅ROS话题、服务和参数，而私有句柄只能在其命名空间内访问和发布/订阅话题、服务和参数。通过同时使用两种类型的句柄，节点可以同时与全局和私有的ROS话题、服务和参数进行交互。

下面是一个使用普通句柄和私有句柄的例子，其中`nh`是普通句柄，`private_nh`是私有句柄：

```c++
#include <ros/ros.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "my_node");
  ros::NodeHandle nh;                  // 普通句柄，使用全局命名空间
  ros::NodeHandle private_nh("~");     // 私有句柄，使用当前节点的命名空间

  ros::Subscriber sub1 = nh.subscribe("global_topic", 1, callback1);         // 在全局命名空间下订阅话题
  ros::Subscriber sub2 = private_nh.subscribe("private_topic", 1, callback2); // 在私有命名空间下订阅话题

  ros::spin();
  return 0;
}
```

在上面的例子中，`nh`是普通句柄，使用全局命名空间，用于订阅全局命名空间下的话题`"global_topic"`。`private_nh`是私有句柄，使用当前节点的命名空间，用于订阅当前节点的私有话题`"private_topic"`。这样，节点既可以访问全局命名空间下的话题，又可以在私有命名空间下进行一些更为独立的操作。

- **全局命名空间跟当前节点的命名空间的区别**

在ROS中，节点、话题、服务和参数等都有各自的命名空间。命名空间是ROS用来区分不同组件的一种机制，同一个ROS系统中的不同节点可以使用相同的名称来表示它们的组件，而不会发生冲突。

全局命名空间是指ROS系统中所有节点都能访问的命名空间。在ROS中，以`/`开头的名称被认为是全局命名空间下的名称。例如，`/rosout`是ROS中的全局话题，任何节点都可以发布和订阅这个话题。

当前节点的命名空间是指节点所在的命名空间。ROS节点启动时可以通过命令行参数指定节点的命名空间，如果没有指定，ROS默认将节点的命名空间设置为全局命名空间。在ROS中，以相对路径（即不以`/`开头的名称）表示的名称被认为是当前节点命名空间下的名称。

在节点程序中，可以通过ROS API创建普通句柄或私有句柄，来访问全局或当前节点命名空间下的组件。普通句柄使用全局命名空间，而私有句柄使用当前节点的命名空间。通过使用私有句柄，节点可以在其自己的命名空间中发布和订阅话题、服务和参数，而不会与其他节点的命名空间产生冲突。

> 此外：在C++中，命名空间（namespace）也是一种机制，用来避免不同组件之间的命名冲突。但是，在C++中，命名空间是编译时的概念，即在编译时将不同组件的代码分别编译到不同的命名空间中。在程序运行时，不同的命名空间已经不存在，只有不同的符号（symbol）。
>
> 在ROS中，命名空间是运行时的概念，是在程序运行时通过ROS Master进行管理的。每个节点的命名空间在程序运行时是可以动态配置的，不同的节点可以在运行时订阅和发布相同的话题，而不会发生命名冲突。此外，在ROS中，还有其他一些特殊的命名空间，如全局命名空间和参数服务器命名空间等，用来管理全局的组件和参数。因此，ROS中的命名空间与C++中的命名空间是不同的概念。

- **roslaunch问题：RLException: [scan_match_icp.launch] is neither a launch file in package [lessons2] nor is [lessons2] a launch file name The traceback for the exception was written to the log file**

这个错误提示表明 ROS 包 "lessons2" 中的一个 launch 文件存在问题。被引用的 "scan_match_icp.launch" 文件无法在 "lessons2" 包中找到，或者找不到 "lessons2" 包本身。

要解决这个问题，可以尝试以下步骤：

1. 验证你的 ROS 工作空间中是否存在 "lessons2" 包。你可以在终端中运行命令 `rospack find lessons2` 来检查。
2. 检查 "scan_match_icp.launch" 文件是否位于 "lessons2" 包内正确的目录中。该文件应该位于包的 "launch" 子目录中。
3. 确保在你的 ROS 环境中正确地源化了 "lessons2" 包。你可以在终端中运行命令 `rospack list | grep lessons2` 来检查。
4. 检查 launch 文件本身是否存在拼写错误或错误的包名、缺失的依赖项等问题。
5. 如果上述步骤无法解决问题，可以检查日志文件以获取更多有关具体错误的信息，并尝试从那里进行故障排除。

> **补充**：`rospack profile` 命令的作用是重新生成 ROS 软件包的依赖关系缓存文件，以便在 ROS 运行时更快地查找软件包的依赖关系。
>
> 在 ROS 中，软件包之间的依赖关系是通过 ROS 软件包的 manifest 文件来指定的。当你安装、升级或删除 ROS 软件包时，它们之间的依赖关系也会发生变化。ROS 使用一个缓存文件来记录软件包之间的依赖关系，以避免在运行时重复解析 manifest 文件。但是，如果你在安装、升级或删除 ROS 软件包后没有更新缓存文件，则 ROS 运行时可能会无法正确地找到软件包的依赖关系。
>
> 因此，当安装、升级或删除 ROS 软件包后，可以运行 `rospack profile` 命令来重新生成缓存文件，以便在 ROS 运行时更快地查找软件包的依赖关系。此命令会根据当前安装的软件包重新生成缓存文件，并保存在 ROS 工作空间中的 `.ros` 目录下。
