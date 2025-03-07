/**
 * @file adasdisplay.cpp
 * @brief 高级驾驶辅助系统(ADAS)显示界面的实现文件
 */
#include "adasdisplay.h"
#include "styles.h"
#include "icon.h"

#include <QApplication>
#include <QFont>
#include <QRandomGenerator>

/**
 * @brief ADASDisplay类的构造函数
 * @param parent 父窗口指针
 * 
 * 初始化成员变量并调用initUI()和setupTimers()方法设置界面和定时器
 */
ADASDisplay::ADASDisplay(QWidget *parent)
    : QMainWindow(parent)
    , m_currentSpeed(0)
    , m_alarmActive(false)
    , m_fatigueLevel(20)
    , m_camera0Active(false)
    , m_camera1Active(false)
{
    initUI();
    setupTimers();
    
    // 初始化摄像头
    if (!initCameras()) {
        statusBar()->showMessage("摄像头初始化失败，请检查设备连接", 5000);
    }
}

/**
 * @brief ADASDisplay类的析构函数
 * 
 * 释放定时器资源和关闭摄像头
 */
ADASDisplay::~ADASDisplay()
{
    // 释放资源
    delete m_dataTimer;
    delete m_datetimeTimer;
    delete m_cameraTimer;
    
    // 关闭摄像头
    closeCameras();
}

/**
 * @brief 初始化用户界面
 * 
 * 设置窗口属性、创建布局和各种UI组件
 */
void ADASDisplay::initUI()
{
    // 设置窗口属性
    setWindowTitle("ADAS系统");
    setFixedSize(1280, 720);  // 7寸显示屏分辨率
    
    // 设置应用图标
    setWindowIcon(createAppIcon());
    
    // 应用样式表
    setStyleSheet(MAIN_STYLE);
    
    // 创建中央窗口部件
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    
    // 右侧 - 2x2摄像头网格和状态面板的容器
    QWidget *rightContainer = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setSpacing(5);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    
    // 摄像头网格 (2x2)
    QWidget *cameraGrid = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(cameraGrid);
    gridLayout->setSpacing(5);
    
    // 创建4个摄像头面板
    QVector<QPair<int, int>> positions = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    
    for (int i = 0; i < 4; ++i) {
        DraggableCameraPanel *camera = new DraggableCameraPanel();
        camera->setCameraPosition(i + 1);
        m_cameras.append(camera);
        gridLayout->addWidget(camera, positions[i].first, positions[i].second);
    }
    
    // 设置网格单元的拉伸
    for (int i = 0; i < 2; ++i) {
        gridLayout->setColumnStretch(i, 1);
        gridLayout->setRowStretch(i, 1);
    }
    
    rightLayout->addWidget(cameraGrid, 7);
    
    // 底部状态面板
    m_statusPanel = createStatusPanel();
    rightLayout->addWidget(m_statusPanel, 3);
    
    mainLayout->addWidget(rightContainer);
    
    // 右侧 - 驾驶员摄像头
    m_driverCamera = new DraggableCameraPanel();
    m_driverCamera->setCameraPosition(0);
    m_driverCamera->setFixedWidth(250);  // 右侧驾驶员摄像头宽度
    mainLayout->addWidget(m_driverCamera);
    
    // 添加状态栏
    QStatusBar *statusBar = new QStatusBar();
    setStatusBar(statusBar);
    statusBar->showMessage("系统就绪");
    
    // 添加日期时间标签到状态栏
    m_datetimeLabel = new QLabel();
    statusBar->addPermanentWidget(m_datetimeLabel);
    updateDateTime();
}

/**
 * @brief 设置定时器
 * 
 * 设置数据更新、日期时间更新和摄像头更新的定时器
 */
void ADASDisplay::setupTimers()
{
    // 数据更新计时器
    m_dataTimer = new QTimer(this);
    connect(m_dataTimer, &QTimer::timeout, this, &ADASDisplay::updateData);
    m_dataTimer->start(100);  // 每100毫秒更新一次
    
    // 日期时间更新计时器
    m_datetimeTimer = new QTimer(this);
    connect(m_datetimeTimer, &QTimer::timeout, this, &ADASDisplay::updateDateTime);
    m_datetimeTimer->start(1000);  // 每秒更新一次
    
    // 摄像头更新计时器
    m_cameraTimer = new QTimer(this);
    connect(m_cameraTimer, &QTimer::timeout, this, &ADASDisplay::updateCameraFeeds);
    m_cameraTimer->start(33);  // 约30帧每秒
}

/**
 * @brief 更新数据
 * 
 * 模拟车速的小幅随机变化，更新车速显示和驾驶员疲劳度
 */
void ADASDisplay::updateData()
{
    // 模拟车速的小幅随机变化
    double speedChange = QRandomGenerator::global()->bounded(4.0) - 2.0;
    m_currentSpeed = qBound(0.0, m_currentSpeed + speedChange, 200.0);
    
    // 更新车速显示
    m_speedValue->setText(QString("%1 km/h").arg(static_cast<int>(m_currentSpeed)));
    m_speedProgress->setValue(static_cast<int>(m_currentSpeed));
    
    // 模拟驾驶员疲劳度随时间略微增加
    if (QRandomGenerator::global()->bounded(1.0) < 0.1) {  // 每秒10%的几率增加疲劳度
        m_fatigueLevel = qMin(100, m_fatigueLevel + 1);
        
        // 根据疲劳度更新驾驶员状态
        if (m_fatigueLevel > 70) {
            m_driverStatus->setText("警告：驾驶员疲劳");
            m_driverStatus->setStyleSheet("color: #e74c3c;");
            
            // 如果疲劳度高，自动触发报警
            if (!m_alarmActive) {
                toggleAlarm();
            }
        } else if (m_fatigueLevel > 50) {
            m_driverStatus->setText("注意：驾驶员轻度疲劳");
            m_driverStatus->setStyleSheet("color: #f39c12;");
        } else {
            m_driverStatus->setText("正常：驾驶员状态良好");
            m_driverStatus->setStyleSheet("color: #2ecc71;");
        }
        
        // 更新疲劳度进度条
        m_driverFatigue->setValue(m_fatigueLevel);
    }
}

/**
 * @brief 初始化摄像头
 * @return 是否成功初始化摄像头
 * 
 * 打开/dev/video0和/dev/video2摄像头设备
 */
bool ADASDisplay::initCameras()
{
    // 尝试打开摄像头0 (/dev/video0)
    m_camera0Active = m_camera0.open(0);
    if (m_camera0Active) {
        // 设置摄像头属性
        m_camera0.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        m_camera0.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    }
    
    // 尝试打开摄像头1 (/dev/video2)
    m_camera1Active = m_camera1.open(2);
    if (m_camera1Active) {
        // 设置摄像头属性
        m_camera1.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        m_camera1.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    }
    
    return m_camera0Active || m_camera1Active;
}

/**
 * @brief 关闭摄像头
 * 
 * 释放摄像头资源
 */
void ADASDisplay::closeCameras()
{
    if (m_camera0Active) {
        m_camera0.release();
        m_camera0Active = false;
    }
    
    if (m_camera1Active) {
        m_camera1.release();
        m_camera1Active = false;
    }
}

/**
 * @brief 更新摄像头画面
 * 
 * 从摄像头获取图像并显示在对应的面板上
 */
void ADASDisplay::updateCameraFeeds()
{
    cv::Mat frame;
    
    // 更新摄像头0画面到摄像头1面板
    if (m_camera0Active && m_camera0.isOpened()) {
        if (m_camera0.read(frame)) {
            if (!frame.empty() && m_cameras.size() > 0) {
                // 转换为QImage并显示
                QImage image = matToQImage(frame);
                m_cameras[0]->cameraFeed()->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
    
    // 更新摄像头1画面到摄像头2面板
    if (m_camera1Active && m_camera1.isOpened()) {
        if (m_camera1.read(frame)) {
            if (!frame.empty() && m_cameras.size() > 1) {
                // 转换为QImage并显示
                QImage image = matToQImage(frame);
                m_cameras[1]->cameraFeed()->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}

/**
 * @brief 将OpenCV的Mat转换为QImage
 * @param mat OpenCV的Mat图像
 * @return 转换后的QImage
 * 
 * 将OpenCV的Mat图像格式转换为Qt的QImage格式
 */
QImage ADASDisplay::matToQImage(const cv::Mat& mat)
{
    // 检查图像是否为空
    if (mat.empty())
        return QImage();
    
    // 转换颜色空间从BGR到RGB
    cv::Mat rgbMat;
    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    
    // 创建QImage
    QImage image(rgbMat.data, rgbMat.cols, rgbMat.rows, 
                 rgbMat.step, QImage::Format_RGB888);
    
    // 深拷贝，确保数据不会被OpenCV释放
    return image.copy();
}

/**
 * @brief 创建状态面板
 * @return 状态面板指针
 * 
 * 创建状态面板，包括车速、报警状态、驾驶员信息和控制部分
 */
QFrame* ADASDisplay::createStatusPanel()
{
    QFrame *frame = new QFrame();
    frame->setObjectName("statusPanel");
    frame->setFrameShape(QFrame::Box);
    frame->setLineWidth(2);
    frame->setStyleSheet(DATA_PANEL_STYLE);
    
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setSpacing(20);
    
    // 车速部分
    QWidget *speedWidget = new QWidget();
    QVBoxLayout *speedLayout = new QVBoxLayout(speedWidget);
    
    QLabel *speedTitle = new QLabel("车速");
    speedTitle->setAlignment(Qt::AlignCenter);
    speedTitle->setFont(QFont("Arial", 12, QFont::Bold));
    
    m_speedValue = new QLabel("0 km/h");
    m_speedValue->setAlignment(Qt::AlignCenter);
    m_speedValue->setFont(QFont("Arial", 20, QFont::Bold));
    
    m_speedProgress = new QProgressBar();
    m_speedProgress->setRange(0, 200);
    m_speedProgress->setValue(0);
    
    speedLayout->addWidget(speedTitle);
    speedLayout->addWidget(m_speedValue);
    speedLayout->addWidget(m_speedProgress);
    
    // 报警状态部分
    QWidget *alarmWidget = new QWidget();
    QVBoxLayout *alarmLayout = new QVBoxLayout(alarmWidget);
    
    QLabel *alarmTitle = new QLabel("系统状态");
    alarmTitle->setAlignment(Qt::AlignCenter);
    alarmTitle->setFont(QFont("Arial", 12, QFont::Bold));
    
    m_alarmStatus = new QLabel("正常");
    m_alarmStatus->setAlignment(Qt::AlignCenter);
    m_alarmStatus->setFont(QFont("Arial", 20, QFont::Bold));
    
    QWidget *alarmButtons = new QWidget();
    QHBoxLayout *alarmButtonsLayout = new QHBoxLayout(alarmButtons);
    
    m_alarmButton = new QPushButton("触发报警");
    connect(m_alarmButton, &QPushButton::clicked, this, &ADASDisplay::toggleAlarm);
    
    alarmButtonsLayout->addWidget(m_alarmButton);
    
    alarmLayout->addWidget(alarmTitle);
    alarmLayout->addWidget(m_alarmStatus);
    alarmLayout->addWidget(alarmButtons);
    
    // 驾驶员信息部分
    QWidget *driverWidget = new QWidget();
    QVBoxLayout *driverLayout = new QVBoxLayout(driverWidget);
    
    QLabel *driverTitle = new QLabel("驾驶员信息");
    driverTitle->setAlignment(Qt::AlignCenter);
    driverTitle->setFont(QFont("Arial", 12, QFont::Bold));
    
    m_driverStatus = new QLabel("注意力集中");
    m_driverStatus->setAlignment(Qt::AlignCenter);
    m_driverStatus->setFont(QFont("Arial", 16));
    
    m_driverFatigue = new QProgressBar();
    m_driverFatigue->setRange(0, 100);
    m_driverFatigue->setValue(20);
    
    driverLayout->addWidget(driverTitle);
    driverLayout->addWidget(m_driverStatus);
    driverLayout->addWidget(new QLabel("疲劳度:"));
    driverLayout->addWidget(m_driverFatigue);
    
    // 控制部分
    QWidget *controlWidget = new QWidget();
    QVBoxLayout *controlLayout = new QVBoxLayout(controlWidget);
    
    QLabel *controlTitle = new QLabel("控制");
    controlTitle->setAlignment(Qt::AlignCenter);
    controlTitle->setFont(QFont("Arial", 12, QFont::Bold));
    
    QWidget *speedControls = new QWidget();
    QHBoxLayout *speedControlsLayout = new QHBoxLayout(speedControls);
    
    QPushButton *increaseSpeedBtn = new QPushButton("+");
    connect(increaseSpeedBtn, &QPushButton::clicked, this, &ADASDisplay::increaseSpeed);
    
    QPushButton *decreaseSpeedBtn = new QPushButton("-");
    connect(decreaseSpeedBtn, &QPushButton::clicked, this, &ADASDisplay::decreaseSpeed);
    
    speedControlsLayout->addWidget(decreaseSpeedBtn);
    speedControlsLayout->addWidget(increaseSpeedBtn);
    
    QPushButton *helpButton = new QPushButton("帮助");
    connect(helpButton, &QPushButton::clicked, this, &ADASDisplay::showHelp);
    
    QPushButton *exitButton = new QPushButton("退出");
    connect(exitButton, &QPushButton::clicked, this, &ADASDisplay::closeApplication);
    
    controlLayout->addWidget(controlTitle);
    controlLayout->addWidget(new QLabel("车速调整:"));
    controlLayout->addWidget(speedControls);
    controlLayout->addWidget(helpButton);
    controlLayout->addWidget(exitButton);
    
    // 添加所有部分到主布局
    layout->addWidget(speedWidget, 1);
    layout->addWidget(alarmWidget, 1);
    layout->addWidget(driverWidget, 1);
    layout->addWidget(controlWidget, 1);
    
    return frame;
}

/**
 * @brief 增加车速
 * 
 * 增加车速值并更新显示
 */
void ADASDisplay::increaseSpeed()
{
    // 增加车速值
    m_currentSpeed = qMin(200.0, m_currentSpeed + 10.0);
    m_speedValue->setText(QString("%1 km/h").arg(static_cast<int>(m_currentSpeed)));
    m_speedProgress->setValue(static_cast<int>(m_currentSpeed));
    statusBar()->showMessage(QString("车速增加到 %1 km/h").arg(static_cast<int>(m_currentSpeed)), 2000);
}

/**
 * @brief 减少车速
 * 
 * 减少车速值并更新显示
 */
void ADASDisplay::decreaseSpeed()
{
    // 减少车速值
    m_currentSpeed = qMax(0.0, m_currentSpeed - 10.0);
    m_speedValue->setText(QString("%1 km/h").arg(static_cast<int>(m_currentSpeed)));
    m_speedProgress->setValue(static_cast<int>(m_currentSpeed));
    statusBar()->showMessage(QString("车速减少到 %1 km/h").arg(static_cast<int>(m_currentSpeed)), 2000);
}

/**
 * @brief 切换报警状态
 * 
 * 切换报警状态并更新显示
 */
void ADASDisplay::toggleAlarm()
{
    // 切换报警状态
    m_alarmActive = !m_alarmActive;
    
    if (m_alarmActive) {
        m_alarmStatus->setText("报警");
        m_alarmStatus->setStyleSheet("color: red; font-weight: bold;");
        m_alarmButton->setText("解除报警");
        statusBar()->showMessage("系统报警已触发", 2000);
    } else {
        m_alarmStatus->setText("正常");
        m_alarmStatus->setStyleSheet("color: green; font-weight: bold;");
        m_alarmButton->setText("触发报警");
        statusBar()->showMessage("系统报警已解除", 2000);
    }
}

/**
 * @brief 关闭应用程序
 * 
 * 关闭应用程序前弹出确认对话框
 */
void ADASDisplay::closeApplication()
{
    // 关闭应用程序
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "确认退出", 
        "确定要退出ADAS系统吗?",
        QMessageBox::Yes | QMessageBox::No, 
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        close();
    }
}

/**
 * @brief 显示帮助信息
 * 
 * 显示帮助信息对话框
 */
void ADASDisplay::showHelp()
{
    // 显示帮助信息
    QString helpText = R"(
        <h3>ADAS系统帮助</h3>
        <p>本系统是高级驾驶辅助系统(ADAS)的监控界面，提供以下功能：</p>
        <ul>
            <li>显示5路摄像头实时画面</li>
            <li>监控车辆速度</li>
            <li>监控系统报警状态</li>
            <li>监控驾驶员疲劳状态</li>
        </ul>
        <p>操作说明：</p>
        <ul>
            <li>使用+/-按钮调整车速</li>
            <li>点击"触发报警"按钮可手动触发/解除系统报警</li>
            <li>驾驶员疲劳度超过70%会自动触发系统报警</li>
            <li>可以拖拽摄像头窗口互换位置</li>
        </ul>
        <p>版本：1.0.0</p>
    )";
    
    QMessageBox::information(this, "帮助", helpText);
}

/**
 * @brief 交换两个摄像头的位置
 * @param sourcePos 源摄像头位置
 * @param targetPos 目标摄像头位置
 * 
 * 交换两个摄像头的位置
 */
void ADASDisplay::swapCameras(int sourcePos, int targetPos)
{
    // 交换两个摄像头的位置
    if (sourcePos == targetPos) {
        return;
    }
    
    // 获取要交换的摄像头
    DraggableCameraPanel *sourceCamera = nullptr;
    DraggableCameraPanel *targetCamera = nullptr;
    
    // 查找源和目标摄像头
    if (sourcePos == 0) {
        sourceCamera = m_driverCamera;
    } else if (sourcePos >= 1 && sourcePos <= m_cameras.size()) {
        sourceCamera = m_cameras[sourcePos - 1];
    }
    
    if (targetPos == 0) {
        targetCamera = m_driverCamera;
    } else if (targetPos >= 1 && targetPos <= m_cameras.size()) {
        targetCamera = m_cameras[targetPos - 1];
    }
    
    if (!sourceCamera || !targetCamera) {
        return;
    }
    
    // 交换摄像头画面
    QString sourceText = sourceCamera->cameraFeed()->text();
    QString targetText = targetCamera->cameraFeed()->text();
    sourceCamera->cameraFeed()->setText(targetText);
    targetCamera->cameraFeed()->setText(sourceText);
    
    // 更新位置
    int tempPos = sourceCamera->cameraPosition();
    sourceCamera->setCameraPosition(targetCamera->cameraPosition());
    targetCamera->setCameraPosition(tempPos);
}

/**
 * @brief 更新日期时间显示
 * 
 * 获取当前日期时间并更新显示
 */
void ADASDisplay::updateDateTime()
{
    // 更新日期和时间显示
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString datetimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    m_datetimeLabel->setText(datetimeStr);
}
