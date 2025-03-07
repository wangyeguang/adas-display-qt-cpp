# ADAS系统 (C++版本)

这是一个使用Qt和C++开发的高级驾驶辅助系统(ADAS)监控界面。该系统显示多路摄像头画面并提供车辆状态信息，包括车速、报警状态和驾驶员信息。

## 功能特点

- 显示5路摄像头实时画面（4个外部摄像头和1个驾驶员摄像头）
- 支持摄像头窗口拖拽交换位置
- 监控车辆速度，支持手动调整
- 监控系统报警状态，支持手动触发/解除
- 监控驾驶员疲劳状态，疲劳度超过阈值自动触发报警
- 现代化深色主题UI设计
- 适配7寸显示屏（1280*720分辨率）

## 系统要求

- Linux/Windows系统
- CMake 3.14或更高版本
- GCC/Visual Studio（带有C++桌面开发工作负载）
- Qt 5.12或更高版本

## 安装与运行

1. 安装必要的开发环境：
   ```
   # Linux
   sudo apt-get install build-essential cmake qt5-default
   
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
- 控制层：用户交互处理（按钮点击、拖拽操作等）

### 目录结构

```
industrial-display-qt-cpp/
├── CMakeLists.txt        # CMake项目配置文件
├── main.cpp              # 程序入口点
├── adasdisplay.h/cpp     # 主窗口类实现
├── draggablecamerapanel.h/cpp # 可拖拽摄像头面板组件
├── icon.h/cpp            # 应用程序图标生成
├── styles.h              # UI样式定义
├── setup_environment.ps1 # 环境安装脚本(Windows)
└── build/                # 构建目录
```

## 类说明

### ADASDisplay类

`ADASDisplay`是应用程序的主窗口类，继承自`QMainWindow`，负责管理整个界面和业务逻辑。

#### 主要成员变量

- `m_driverCamera`: 驾驶员摄像头面板
- `m_cameras`: 其他摄像头面板集合
- `m_statusPanel`: 状态面板框架
- `m_speedValue`: 车速值标签
- `m_speedProgress`: 车速进度条
- `m_alarmStatus`: 警报状态标签
- `m_alarmButton`: 警报按钮
- `m_driverStatus`: 驾驶员状态标签
- `m_driverFatigue`: 驾驶员疲劳度进度条
- `m_dataTimer`: 数据更新定时器
- `m_datetimeTimer`: 日期时间更新定时器
- `m_currentSpeed`: 当前车速
- `m_alarmActive`: 警报激活状态
- `m_fatigueLevel`: 疲劳度级别

#### 主要方法

- `initUI()`: 初始化用户界面
- `setupTimers()`: 设置定时器
- `createStatusPanel()`: 创建状态面板
- `updateData()`: 更新显示数据
- `simulateCameraFeeds()`: 模拟摄像头数据
- `updateDateTime()`: 更新日期时间显示
- `increaseSpeed()`: 增加车速
- `decreaseSpeed()`: 减少车速
- `toggleAlarm()`: 切换警报状态
- `swapCameras()`: 交换摄像头位置

### DraggableCameraPanel类

`DraggableCameraPanel`是一个可拖拽的摄像头面板类，继承自`QFrame`，实现了拖放功能。

#### 主要成员变量

- `m_cameraFeed`: 摄像头图像显示标签
- `m_cameraPosition`: 摄像头位置索引

#### 主要方法

- `mousePressEvent()`: 鼠标按下事件处理
- `dragEnterEvent()`: 拖拽进入事件处理
- `dropEvent()`: 放下事件处理

## 主要功能实现

### 摄像头显示与拖拽

摄像头显示使用`DraggableCameraPanel`类实现，该类继承自`QFrame`并重写了Qt的拖放相关事件处理函数：

1. `mousePressEvent()`：当用户按下鼠标左键时，创建一个拖拽对象，并将当前摄像头位置作为数据传递
2. `dragEnterEvent()`：当拖拽对象进入此面板时，如果包含文本数据（摄像头位置），则接受拖拽操作
3. `dropEvent()`：当拖拽对象在此面板上释放时，获取源摄像头位置，并通过父窗口的`swapCameras`方法交换两个摄像头的位置

### 数据更新与模拟

数据更新通过定时器实现，主要在`updateData()`方法中：

1. 使用`QRandomGenerator`生成随机数据模拟车速变化
2. 根据随机概率增加驾驶员疲劳度
3. 根据疲劳度更新驾驶员状态和警报状态
4. 调用`simulateCameraFeeds()`模拟摄像头画面更新

```cpp
// 模拟车速的小幅随机变化
double speedChange = QRandomGenerator::global()->bounded(4.0) - 2.0;
m_currentSpeed = qBound(0.0, m_currentSpeed + speedChange, 200.0);
```

### 用户交互

用户交互主要通过按钮点击和拖拽操作实现：

1. 增加/减少车速按钮：调用`increaseSpeed()`/`decreaseSpeed()`方法
2. 警报按钮：调用`toggleAlarm()`方法
3. 帮助按钮：调用`showHelp()`方法
4. 退出按钮：调用`closeApplication()`方法
5. 摄像头拖拽：通过`DraggableCameraPanel`类的拖放功能实现

## 二次开发指南

### 添加新功能

1. **添加新的状态监控**

   在`ADASDisplay`类中添加新的成员变量和UI组件：

   ```cpp
   // 在adasdisplay.h中添加
   private:
       QLabel *m_newStatusLabel;
       double m_newStatusValue;
   ```

   在`createStatusPanel()`方法中创建新的UI组件：

   ```cpp
   m_newStatusLabel = new QLabel("新状态");
   layout->addWidget(m_newStatusLabel);
   ```

   在`updateData()`方法中更新新的状态：

   ```cpp
   m_newStatusValue = /* 计算新状态值 */;
   m_newStatusLabel->setText(QString("新状态: %1").arg(m_newStatusValue));
   ```

2. **添加新的摄像头**

   在`initUI()`方法中添加新的摄像头面板：

   ```cpp
   DraggableCameraPanel *newCamera = new DraggableCameraPanel();
   newCamera->setCameraPosition(m_cameras.size());
   newCamera->cameraFeed()->setText("新摄像头");
   m_cameras.append(newCamera);
   cameraLayout->addWidget(newCamera, /* 行 */, /* 列 */);
   ```

### 修改现有功能

1. **修改车速计算逻辑**

   在`updateData()`方法中修改车速计算逻辑：

   ```cpp
   // 原代码
   double speedChange = QRandomGenerator::global()->bounded(4.0) - 2.0;
   m_currentSpeed = qBound(0.0, m_currentSpeed + speedChange, 200.0);
   
   // 修改为
   double speedChange = /* 新的计算逻辑 */;
   m_currentSpeed = /* 新的范围限制 */;
   ```

2. **修改UI样式**

   在`styles.h`文件中修改样式定义：

   ```cpp
   // 修改主样式
   #define MAIN_STYLE "QMainWindow { background-color: #1e1e1e; } ..."
   
   // 修改面板样式
   #define IMAGE_PANEL_STYLE "QFrame { border: 2px solid #3498db; ... }"
   ```

### 连接真实设备

要将模拟数据替换为真实设备数据，需要修改以下部分：

1. **摄像头数据**

   在`simulateCameraFeeds()`方法中替换模拟代码：

   ```cpp
   // 替换模拟代码
   void ADASDisplay::simulateCameraFeeds()
   {
       // 连接真实摄像头并获取图像
       QImage driverImage = /* 从驾驶员摄像头获取图像 */;
       m_driverCamera->cameraFeed()->setPixmap(QPixmap::fromImage(driverImage));
       
       for (int i = 0; i < m_cameras.size(); ++i) {
           QImage cameraImage = /* 从第i个摄像头获取图像 */;
           m_cameras[i]->cameraFeed()->setPixmap(QPixmap::fromImage(cameraImage));
       }
   }
   ```

2. **车速数据**

   在`updateData()`方法中替换模拟代码：

   ```cpp
   // 替换模拟车速代码
   double realSpeed = /* 从车速传感器获取真实车速 */;
   m_currentSpeed = realSpeed;
   ```

3. **驾驶员状态**

   在`updateData()`方法中替换模拟代码：

   ```cpp
   // 替换模拟驾驶员疲劳度代码
   int realFatigueLevel = /* 从驾驶员监控系统获取真实疲劳度 */;
   m_fatigueLevel = realFatigueLevel;
   ```

## 常见问题解决

### 编译错误

1. **QRandomGenerator::bounded错误**

   错误：`call of overloaded 'bounded(double, double)' is ambiguous`
   
   解决方案：QRandomGenerator没有直接接受两个double参数的bounded方法，应使用：
   
   ```cpp
   // 错误用法
   double value = QRandomGenerator::global()->bounded(-2.0, 2.0);
   
   // 正确用法
   double value = QRandomGenerator::global()->bounded(4.0) - 2.0;
   ```

2. **Qt相关头文件找不到**

   错误：`No such file or directory`
   
   解决方案：确保正确安装Qt并在CMakeLists.txt中正确配置：
   
   ```cmake
   find_package(Qt5 COMPONENTS Widgets REQUIRED)
   target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
   ```

### 运行问题

1. **UI显示异常**

   问题：UI组件显示不完整或布局混乱
   
   解决方案：检查布局管理器设置和样式表，确保窗口大小适配目标设备

2. **定时器不工作**

   问题：数据不更新或更新不正常
   
   解决方案：检查定时器连接和启动代码：
   
   ```cpp
   m_dataTimer = new QTimer(this);
   connect(m_dataTimer, &QTimer::timeout, this, &ADASDisplay::updateData);
   m_dataTimer->start(100);  // 确保定时器已启动
   ```

## 联系与支持

如有问题或需要技术支持，请联系项目维护者。
