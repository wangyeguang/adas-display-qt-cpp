# ADAS系统 (C++版本)

这是一个使用Qt和C++开发的高级驾驶辅助系统(ADAS)监控界面。该系统显示多路摄像头画面并提供车辆状态信息，包括车速、报警状态和驾驶员信息。

## 功能特点

- 显示5路摄像头实时画面（4个外部摄像头和1个驾驶员摄像头）
- 全屏无边框显示，最大化显示区域
- 摄像头画面完全填充显示区域，无边框和间隙
- 监控车辆速度，支持手动调整
- 监控系统报警状态，支持手动触发/解除
- 监控驾驶员疲劳状态，疲劳度超过阈值自动触发报警
- 现代化深色主题UI设计
- 适配全高清显示屏（1920*1080分辨率）
- 支持ESC键切换全屏/窗口模式

## 系统要求

- Linux/Windows系统
- CMake 3.14或更高版本
- GCC/Visual Studio（带有C++桌面开发工作负载）
- Qt 5.12或更高版本
- OpenCV 4.2或更高版本（用于摄像头处理）

## 安装与运行

1. 安装必要的开发环境：

   ```
   # Linux
   sudo apt-get install build-essential cmake qt5-default libopencv-dev

   # Windows
   powershell -ExecutionPolicy Bypass -File setup_environment.ps1
   ```
2. 构建并运行应用程序：

   ```
   # Linux
   mkdir -p build && cd build
   cmake ..
   make
   ./ADAS_System

   # Windows
   powershell -ExecutionPolicy Bypass -File build_and_run.ps1
   ```

## 项目架构

### 整体架构

本项目采用Qt框架的MVC架构设计：

- 模型层：数据处理和状态管理（车速、报警状态、驾驶员疲劳度等）
- 视图层：UI界面展示（摄像头面板、状态面板等）
- 控制层：用户交互处理（按钮点击、快捷键等）

### 目录结构

```
industrial-display-qt-cpp/
├── CMakeLists.txt        # CMake项目配置文件
├── main.cpp              # 程序入口点
├── adasdisplay.h/cpp     # 主窗口类实现
├── draggablecamerapanel.h/cpp # 摄像头面板组件（历史保留）
├── icon.h/cpp            # 应用程序图标生成
├── styles.h              # UI样式定义
├── setup_environment.ps1 # 环境安装脚本(Windows)
└── build/                # 构建目录
```

## 类说明

### ADASDisplay类

`ADASDisplay`是应用程序的主窗口类，继承自 `QMainWindow`，负责管理整个界面和业务逻辑。

#### 主要成员变量

- `m_cameraLabels`: 摄像头画面标签集合
- `m_driverFeed`: 驾驶员摄像头画面标签
- `m_statusPanel`: 状态面板框架
- `m_speedValue`: 车速值标签
- `m_speedProgress`: 车速进度条
- `m_alarmStatus`: 警报状态标签
- `m_alarmButton`: 警报按钮
- `m_driverStatus`: 驾驶员状态标签
- `m_driverFatigue`: 驾驶员疲劳度进度条
- `m_dataTimer`: 数据更新定时器
- `m_datetimeTimer`: 日期时间更新定时器
- `m_cameraTimer`: 摄像头更新定时器
- `m_currentSpeed`: 当前车速
- `m_alarmActive`: 警报激活状态
- `m_fatigueLevel`: 疲劳度级别
- `m_camera0`: 摄像头0（/dev/video0）
- `m_camera1`: 摄像头1（/dev/video2）

#### 主要方法

- `initUI()`: 初始化用户界面
- `setupTimers()`: 设置定时器
- `createStatusPanel()`: 创建状态面板
- `initCameras()`: 初始化摄像头
- `updateCameraFeeds()`: 更新摄像头画面
- `simulateOtherCameras()`: 模拟其他摄像头画面
- `updateData()`: 更新显示数据
- `updateDateTime()`: 更新日期时间显示
- `increaseSpeed()`: 增加车速
- `decreaseSpeed()`: 减少车速
- `toggleAlarm()`: 切换警报状态
- `toggleFullScreen()`: 切换全屏模式
- `matToQImage()`: 将OpenCV的Mat转换为QImage

## 主要功能实现

### 摄像头显示

摄像头显示使用QLabel组件实现，通过OpenCV读取摄像头数据：

1. 在 `initCameras()`中初始化摄像头设备
2. 在 `updateCameraFeeds()`中读取摄像头帧并更新UI
3. 使用 `matToQImage()`将OpenCV的Mat图像转换为Qt的QImage
4. 对于未连接的摄像头，使用 `simulateOtherCameras()`生成模拟画面

```cpp
// 读取摄像头帧并显示
if (m_camera0Active) {
    cv::Mat frame;
    if (m_camera0.read(frame)) {
        QImage image = matToQImage(frame);
        m_cameraLabels[0]->setPixmap(QPixmap::fromImage(image));
    }
}
```

### 全屏显示与切换

全屏显示通过Qt的窗口标志和全屏API实现：

1. 在构造函数中设置窗口标志和全屏模式：

   ```cpp
   // 隐藏标题栏
   setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

   // 设置全屏显示
   showFullScreen();
   ```
2. 添加ESC键快捷键切换全屏模式：

   ```cpp
   // 添加ESC键退出全屏快捷键
   QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
   connect(shortcut, &QShortcut::activated, this, &ADASDisplay::toggleFullScreen);
   ```
3. 实现切换全屏的方法：

   ```cpp
   void ADASDisplay::toggleFullScreen()
   {
       if (isFullScreen()) {
           showNormal();
       } else {
           showFullScreen();
       }
   }
   ```

### 数据更新与模拟

数据更新通过定时器实现，主要在 `updateData()`方法中：

1. 使用 `QRandomGenerator`生成随机数据模拟车速变化
2. 根据随机概率增加驾驶员疲劳度
3. 根据疲劳度更新驾驶员状态和警报状态

```cpp
// 模拟车速的小幅随机变化
double speedChange = QRandomGenerator::global()->bounded(4.0) - 2.0;
m_currentSpeed = qBound(0.0, m_currentSpeed + speedChange, 200.0);
```

### 用户交互

用户交互主要通过按钮点击和快捷键实现：

1. 增加/减少车速按钮：调用 `increaseSpeed()`/`decreaseSpeed()`方法
2. 警报按钮：调用 `toggleAlarm()`方法
3. 帮助按钮：调用 `showHelp()`方法
4. 退出按钮：调用 `closeApplication()`方法
5. ESC键：调用 `toggleFullScreen()`方法切换全屏模式

## 二次开发指南

### 添加新功能

1. **添加新的状态监控**

   在 `ADASDisplay`类中添加新的成员变量和UI组件：

   ```cpp
   // 在adasdisplay.h中添加
   private:
       QLabel *m_newStatusLabel;
       double m_newStatusValue;
   ```

   在 `createStatusPanel()`方法中创建新的UI组件：

   ```cpp
   m_newStatusLabel = new QLabel("新状态");
   layout->addWidget(m_newStatusLabel);
   ```

   在 `updateData()`方法中更新新的状态：

   ```cpp
   m_newStatusValue = /* 计算新状态值 */;
   m_newStatusLabel->setText(QString("新状态: %1").arg(m_newStatusValue));
   ```
2. **添加新的摄像头**

   在 `initUI()`方法中添加新的摄像头标签：

   ```cpp
   QLabel *newCameraFeed = new QLabel();
   newCameraFeed->setAlignment(Qt::AlignCenter);
   newCameraFeed->setText("无信号");
   newCameraFeed->setStyleSheet("color: white;");
   newCameraFeed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   newCameraFeed->setScaledContents(true);
   m_cameraLabels.append(newCameraFeed);
   ```

### 修改现有功能

1. **修改车速计算逻辑**

   在 `updateData()`方法中修改车速计算逻辑：

   ```cpp
   // 自定义车速计算逻辑
   m_currentSpeed = /* 自定义计算方法 */;
   m_speedValue->setText(QString("%1 km/h").arg(m_currentSpeed, 0, 'f', 1));
   m_speedProgress->setValue(static_cast<int>(m_currentSpeed));
   ```
2. **修改摄像头布局**

   在 `initUI()`方法中修改摄像头布局：

   ```cpp
   // 自定义摄像头布局
   QGridLayout *customLayout = new QGridLayout();
   // 添加摄像头标签到自定义位置
   customLayout->addWidget(m_cameraLabels[0], 0, 0);
   customLayout->addWidget(m_cameraLabels[1], 0, 1);
   // ...
   ```

## 界面优化历史

### 2025年3月更新

1. **界面布局优化**

   - 移除了摄像头拖拽功能，改为固定布局
   - 隐藏了窗口标题栏，使用无边框窗口
   - 将窗口大小修改为1920x1080，并默认全屏显示
   - 添加了ESC键切换全屏/窗口模式的功能
   - 移除了所有摄像头标题标签，最大化显示区域
   - 摄像头画面完全填充摄像头框架，无边框和间隙
   - 扩展了状态面板到窗口宽度
   - 驾驶员摄像头窗口拉长填充留白区域
2. **摄像头处理优化**

   - 不再将摄像头2的画面同步到驾驶员监测画面
   - 优化了摄像头初始化和读取逻辑
   - 添加了摄像头画面缩放以适应显示区域
   - 改进了模拟摄像头画面的生成逻辑

## 程序流程图

### 应用程序启动流程

```
+---------------------+     +---------------------+     +---------------------+
|                     |     |                     |     |                     |
|  程序入口 (main.cpp) +---->+  创建ADASDisplay实例  +---->+  设置窗口属性        |
|                     |     |                     |     |  (全屏、无边框)       |
+---------------------+     +---------------------+     +---------------------+
                                                               |
                                                               v
+---------------------+     +---------------------+     +---------------------+
|                     |     |                     |     |                     |
|  启动定时器          <-----+  初始化摄像头         <-----+  初始化UI界面        |
|                     |     |                     |     |                     |
+---------------------+     +---------------------+     +---------------------+
        |
        v
+---------------------+
|                     |
|  应用程序主循环      |
|  (Qt事件循环)        |
|                     |
+---------------------+
```

### 摄像头数据处理流程

```
+---------------------+     +---------------------+     +---------------------+
|                     |     |                     |     |                     |
|  摄像头定时器触发    +---->+  读取摄像头数据      +---->+  转换为QImage格式    |
|                     |     |                     |     |                     |
+---------------------+     +---------------------+     +---------------------+
                                      |
                                      | 读取失败
                                      v
                            +---------------------+
                            |                     |
                            |  生成模拟摄像头画面  |
                            |                     |
                            +---------------------+
                                      |
                                      v
                            +---------------------+
                            |                     |
                            |  更新UI显示         |
                            |                     |
                            +---------------------+
```

### 状态数据更新流程

```
+---------------------+     +---------------------+     +---------------------+
|                     |     |                     |     |                     |
|  数据定时器触发      +---->+  更新车速数据        +---->+  更新驾驶员疲劳度    |
|                     |     |                     |     |                     |
+---------------------+     +---------------------+     +---------------------+
                                                               |
                                                               v
+---------------------+     +---------------------+     +---------------------+
|                     |     |                     |     |                     |
|  更新UI显示         <-----+  更新警报状态        <-----+  检查疲劳度阈值      |
|                     |     |                     |     |                     |
+---------------------+     +---------------------+     +---------------------+
```

### 用户交互处理流程

```
+---------------------+     +---------------------+
|                     |     |                     |
|  用户按下ESC键      +---->+  切换全屏/窗口模式  |
|                     |     |                     |
+---------------------+     +---------------------+

+---------------------+     +---------------------+
|                     |     |                     |
|  用户点击增速按钮    +---->+  增加车速值         |
|                     |     |                     |
+---------------------+     +---------------------+

+---------------------+     +---------------------+
|                     |     |                     |
|  用户点击减速按钮    +---->+  减少车速值         |
|                     |     |                     |
+---------------------+     +---------------------+

+---------------------+     +---------------------+
|                     |     |                     |
|  用户点击警报按钮    +---->+  切换警报状态       |
|                     |     |                     |
+---------------------+     +---------------------+

+---------------------+     +---------------------+
|                     |     |                     |
|  用户点击帮助按钮    +---->+  显示帮助信息       |
|                     |     |                     |
+---------------------+     +---------------------+

+---------------------+     +---------------------+
|                     |     |                     |
|  用户点击退出按钮    +---->+  关闭应用程序       |
|                     |     |                     |
+---------------------+     +---------------------+
```

## 许可证

本项目采用MIT许可证。详见LICENSE文件。

## 联系方式

如有问题或建议，请联系项目维护者。
