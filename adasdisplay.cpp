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
#include <QDebug>
#include <QPainter>
#include <QShortcut>

/**
 * @brief ADASDisplay类的构造函数
 * @param camera0Path 摄像头0的设备路径
 * @param camera1Path 摄像头1的设备路径
 * @param camera2Path 摄像头2的设备路径
 * @param camera3Path 摄像头3的设备路径
 * @param parent 父窗口指针
 * 
 * 初始化成员变量并调用initUI()和setupTimers()方法设置界面和定时器
 */
ADASDisplay::ADASDisplay(const QString& camera0Path, const QString& camera1Path, 
                         const QString& camera2Path, const QString& camera3Path, 
                         QWidget *parent)
    : QMainWindow(parent)
    , m_currentSpeed(0)
    , m_alarmActive(false)
    , m_fatigueLevel(20)
    , m_camera0Active(false)
    , m_camera1Active(false)
    , m_camera2Active(false)
    , m_camera3Active(false)
    , m_camera0Path(camera0Path)
    , m_camera1Path(camera1Path)
    , m_camera2Path(camera2Path)
    , m_camera3Path(camera3Path)
{
    // 设置窗口标题
    setWindowTitle("高级驾驶辅助系统");
    
    // 设置窗口大小为1920x1080
    resize(1920, 1080);
    
    // 隐藏标题栏
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    
    // 设置全屏显示
    showFullScreen();
    
    // 添加ESC键退出全屏快捷键
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(shortcut, &QShortcut::activated, this, &ADASDisplay::toggleFullScreen);
    
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
    setFixedSize(1920, 1080);  // 修改为1920x1080分辨率
    
    // 隐藏窗口标题栏
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    
    // 设置应用图标
    setWindowIcon(createAppIcon());
    
    // 应用样式表
    setStyleSheet(MAIN_STYLE);
    
    // 创建中央窗口部件
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    // 主布局 - 垂直布局，状态面板在底部
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(2);  // 减小间距
    mainLayout->setContentsMargins(2, 2, 2, 2);  // 减小边距
    
    // 上部摄像头区域容器
    QWidget *cameraContainer = new QWidget();
    QHBoxLayout *cameraLayout = new QHBoxLayout(cameraContainer);
    cameraLayout->setSpacing(2);  // 减小间距
    cameraLayout->setContentsMargins(0, 0, 0, 0);  // 移除边距
    
    // 左侧 - 2x2摄像头网格
    QWidget *cameraGrid = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(cameraGrid);
    gridLayout->setSpacing(2);  // 减小网格间距
    gridLayout->setContentsMargins(0, 0, 0, 0);  // 移除边距
    
    // 创建4个摄像头面板 - 使用普通QFrame替代DraggableCameraPanel
    QVector<QPair<int, int>> positions = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    
    for (int i = 0; i < 4; ++i) {
        // 创建固定的摄像头面板（不可拖拽）
        QFrame *cameraFrame = new QFrame();
        cameraFrame->setFrameShape(QFrame::NoFrame);
        cameraFrame->setLineWidth(0);
        cameraFrame->setStyleSheet("background-color: #222222;");
        cameraFrame->setMinimumSize(450, 360);
        
        // 使用QGridLayout以便更好地控制填充
        QGridLayout *frameLayout = new QGridLayout(cameraFrame);
        frameLayout->setContentsMargins(0, 0, 0, 0);
        frameLayout->setSpacing(0);
        
        // 添加摄像头画面
        QLabel *cameraFeed = new QLabel();
        cameraFeed->setAlignment(Qt::AlignCenter);
        cameraFeed->setText("无信号");
        cameraFeed->setStyleSheet("color: white;");
        cameraFeed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        cameraFeed->setMinimumSize(450, 360);
        cameraFeed->setScaledContents(true);
        
        frameLayout->addWidget(cameraFeed, 0, 0);
        
        // 存储摄像头引用
        m_cameraLabels.append(cameraFeed);
        
        gridLayout->addWidget(cameraFrame, positions[i].first, positions[i].second);
    }
    
    // 设置网格单元的拉伸
    for (int i = 0; i < 2; ++i) {
        gridLayout->setColumnStretch(i, 1);
        gridLayout->setRowStretch(i, 1);
    }
    
    // 右侧 - 驾驶员摄像头面板（不可拖拽）
    QFrame *driverFrame = new QFrame();
    driverFrame->setFrameShape(QFrame::NoFrame);
    driverFrame->setLineWidth(0);
    driverFrame->setStyleSheet("background-color: #222222;");
    driverFrame->setFixedWidth(450);
    driverFrame->setMinimumHeight(720);
    
    // 使用QGridLayout以便更好地控制填充
    QGridLayout *driverLayout = new QGridLayout(driverFrame);
    driverLayout->setContentsMargins(0, 0, 0, 0);
    driverLayout->setSpacing(0);
    
    // 添加驾驶员摄像头画面
    m_driverFeed = new QLabel();
    m_driverFeed->setAlignment(Qt::AlignCenter);
    m_driverFeed->setText("无信号");
    m_driverFeed->setStyleSheet("color: white;");
    m_driverFeed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_driverFeed->setMinimumSize(450, 720);
    m_driverFeed->setScaledContents(true);
    
    driverLayout->addWidget(m_driverFeed, 0, 0);
    
    // 添加摄像头区域到布局
    cameraLayout->addWidget(cameraGrid, 4);  // 摄像头网格占比
    cameraLayout->addWidget(driverFrame, 1);  // 驾驶员摄像头占比
    
    // 将摄像头容器添加到主布局
    mainLayout->addWidget(cameraContainer, 10);  // 摄像头区域占比
    
    // 底部状态面板 - 扩展到整个窗口宽度
    m_statusPanel = createStatusPanel();
    mainLayout->addWidget(m_statusPanel, 1);  // 状态面板占比
    
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
 * 打开摄像头设备
 */
bool ADASDisplay::initCameras()
{
    // 检查摄像头设备是否存在
    bool camera0Exists = QFile::exists(m_camera0Path);
    bool camera1Exists = QFile::exists(m_camera1Path);
    bool camera2Exists = QFile::exists(m_camera2Path);
    bool camera3Exists = QFile::exists(m_camera3Path);
    
    // 输出设备状态信息
    std::cout << "摄像头设备状态：" << std::endl;
    std::cout << "摄像头0 (" << m_camera0Path.toStdString() << "): " << (camera0Exists ? "存在" : "不存在") << std::endl;
    std::cout << "摄像头1 (" << m_camera1Path.toStdString() << "): " << (camera1Exists ? "存在" : "不存在") << std::endl;
    std::cout << "摄像头2 (" << m_camera2Path.toStdString() << "): " << (camera2Exists ? "存在" : "不存在") << std::endl;
    std::cout << "摄像头3 (" << m_camera3Path.toStdString() << "): " << (camera3Exists ? "存在" : "不存在") << std::endl;
    
    // 只尝试打开存在的摄像头设备
    // 尝试打开摄像头0
    if (camera0Exists) {
        m_camera0Active = m_camera0.open(m_camera0Path.toStdString(), cv::CAP_V4L2);
        if (m_camera0Active) {
            // 设置摄像头属性
            m_camera0.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            m_camera0.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
            // 禁用不必要的功能，减少警告
            m_camera0.set(cv::CAP_PROP_BUFFERSIZE, 1);
            // 设置更多属性以解决内存分配问题
            m_camera0.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
            std::cout << "摄像头0初始化成功" << std::endl;
        } else {
            std::cout << "摄像头0打开失败" << std::endl;
        }
    } else {
        m_camera0Active = false;
        std::cout << "摄像头0设备不存在，跳过初始化" << std::endl;
    }
    
    // 尝试打开摄像头1
    if (camera1Exists) {
        m_camera1Active = m_camera1.open(m_camera1Path.toStdString(), cv::CAP_V4L2);
        if (m_camera1Active) {
            // 设置摄像头属性
            m_camera1.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            m_camera1.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
            // 禁用不必要的功能，减少警告
            m_camera1.set(cv::CAP_PROP_BUFFERSIZE, 1);
            // 设置更多属性以解决内存分配问题
            m_camera1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
            std::cout << "摄像头1初始化成功" << std::endl;
        } else {
            std::cout << "摄像头1打开失败" << std::endl;
        }
    } else {
        m_camera1Active = false;
        std::cout << "摄像头1设备不存在，跳过初始化" << std::endl;
    }
    
    // 尝试打开摄像头2
    if (camera2Exists) {
        m_camera2Active = m_camera2.open(m_camera2Path.toStdString(), cv::CAP_V4L2);
        if (m_camera2Active) {
            // 设置摄像头属性
            m_camera2.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            m_camera2.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
            // 禁用不必要的功能，减少警告
            m_camera2.set(cv::CAP_PROP_BUFFERSIZE, 1);
            // 设置更多属性以解决内存分配问题
            m_camera2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
            std::cout << "摄像头2初始化成功" << std::endl;
        } else {
            std::cout << "摄像头2打开失败" << std::endl;
        }
    } else {
        m_camera2Active = false;
        std::cout << "摄像头2设备不存在，跳过初始化" << std::endl;
    }
    
    // 尝试打开摄像头3
    if (camera3Exists) {
        m_camera3Active = m_camera3.open(m_camera3Path.toStdString(), cv::CAP_V4L2);
        if (m_camera3Active) {
            // 设置摄像头属性
            m_camera3.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            m_camera3.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
            // 禁用不必要的功能，减少警告
            m_camera3.set(cv::CAP_PROP_BUFFERSIZE, 1);
            // 设置更多属性以解决内存分配问题
            m_camera3.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
            std::cout << "摄像头3初始化成功" << std::endl;
        } else {
            std::cout << "摄像头3打开失败" << std::endl;
        }
    } else {
        m_camera3Active = false;
        std::cout << "摄像头3设备不存在，跳过初始化" << std::endl;
    }
    
    return m_camera0Active || m_camera1Active || m_camera2Active || m_camera3Active;
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
    
    if (m_camera2Active) {
        m_camera2.release();
        m_camera2Active = false;
    }

    if (m_camera3Active) {
        m_camera3.release();
        m_camera3Active = false;
    }
}

/**
 * @brief 更新摄像头画面
 * 
 * 从摄像头获取画面并更新显示
 */
void ADASDisplay::updateCameraFeeds()
{
    try {
        // 更新真实摄像头画面
        if (m_camera0Active) {
            cv::Mat frame;
            bool success = false;
            try {
                success = m_camera0.read(frame);
            } catch (const cv::Exception& e) {
                std::cerr << "摄像头0读取异常: " << e.what() << std::endl;
            }
            
            if (success && !frame.empty()) {
                QImage image = matToQImage(frame);
                if (m_cameraLabels.size() > 0) {
                    m_cameraLabels[0]->setPixmap(QPixmap::fromImage(image));
                }
            } else if (!success) {
                std::cerr << "摄像头0读取失败" << std::endl;
                // 检查设备是否存在
                if (QFile::exists(m_camera0Path)) {
                    // 尝试重新初始化摄像头
                    m_camera0.release();
                    m_camera0Active = m_camera0.open(m_camera0Path.toStdString(), cv::CAP_V4L2);
                    if (m_camera0Active) {
                        m_camera0.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                        m_camera0.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
                        m_camera0.set(cv::CAP_PROP_BUFFERSIZE, 1);
                        m_camera0.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
                        std::cout << "摄像头0重新初始化成功" << std::endl;
                    } else {
                        std::cout << "摄像头0重新初始化失败" << std::endl;
                    }
                } else {
                    m_camera0Active = false;
                    std::cout << "摄像头0设备不存在，无法重新初始化" << std::endl;
                }
            }
        }
        
        if (m_camera1Active) {
            cv::Mat frame;
            bool success = false;
            try {
                success = m_camera1.read(frame);
            } catch (const cv::Exception& e) {
                std::cerr << "摄像头1读取异常: " << e.what() << std::endl;
            }
            
            if (success && !frame.empty()) {
                QImage image = matToQImage(frame);
                if (m_cameraLabels.size() > 1) {
                    m_cameraLabels[1]->setPixmap(QPixmap::fromImage(image));
                }
            } else if (!success) {
                std::cerr << "摄像头1读取失败" << std::endl;
                // 检查设备是否存在
                if (QFile::exists(m_camera1Path)) {
                    // 尝试重新初始化摄像头
                    m_camera1.release();
                    m_camera1Active = m_camera1.open(m_camera1Path.toStdString(), cv::CAP_V4L2);
                    if (m_camera1Active) {
                        m_camera1.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                        m_camera1.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
                        m_camera1.set(cv::CAP_PROP_BUFFERSIZE, 1);
                        m_camera1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
                        std::cout << "摄像头1重新初始化成功" << std::endl;
                    } else {
                        std::cout << "摄像头1重新初始化失败" << std::endl;
                    }
                } else {
                    m_camera1Active = false;
                    std::cout << "摄像头1设备不存在，无法重新初始化" << std::endl;
                }
            }
        }
        
        if (m_camera2Active) {
            cv::Mat frame;
            bool success = false;
            try {
                success = m_camera2.read(frame);
            } catch (const cv::Exception& e) {
                std::cerr << "摄像头2读取异常: " << e.what() << std::endl;
            }
            
            if (success && !frame.empty()) {
                QImage image = matToQImage(frame);
                if (m_cameraLabels.size() > 2) {
                    m_cameraLabels[2]->setPixmap(QPixmap::fromImage(image));
                }
            } else if (!success) {
                std::cerr << "摄像头2读取失败" << std::endl;
                // 检查设备是否存在
                if (QFile::exists(m_camera2Path)) {
                    // 尝试重新初始化摄像头
                    m_camera2.release();
                    m_camera2Active = m_camera2.open(m_camera2Path.toStdString(), cv::CAP_V4L2);
                    if (m_camera2Active) {
                        m_camera2.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                        m_camera2.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
                        m_camera2.set(cv::CAP_PROP_BUFFERSIZE, 1);
                        m_camera2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
                        std::cout << "摄像头2重新初始化成功" << std::endl;
                    } else {
                        std::cout << "摄像头2重新初始化失败" << std::endl;
                    }
                } else {
                    m_camera2Active = false;
                    std::cout << "摄像头2设备不存在，无法重新初始化" << std::endl;
                }
            }
        }
        
        if (m_camera3Active) {
            cv::Mat frame;
            bool success = false;
            try {
                success = m_camera3.read(frame);
            } catch (const cv::Exception& e) {
                std::cerr << "摄像头3读取异常: " << e.what() << std::endl;
            }
            
            if (success && !frame.empty()) {
                QImage image = matToQImage(frame);
                if (m_cameraLabels.size() > 3) {
                    m_cameraLabels[3]->setPixmap(QPixmap::fromImage(image));
                }
            } else if (!success) {
                std::cerr << "摄像头3读取失败" << std::endl;
                // 检查设备是否存在
                if (QFile::exists(m_camera3Path)) {
                    // 尝试重新初始化摄像头
                    m_camera3.release();
                    m_camera3Active = m_camera3.open(m_camera3Path.toStdString(), cv::CAP_V4L2);
                    if (m_camera3Active) {
                        m_camera3.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                        m_camera3.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
                        m_camera3.set(cv::CAP_PROP_BUFFERSIZE, 1);
                        m_camera3.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G')); // 使用MJPG格式
                        std::cout << "摄像头3重新初始化成功" << std::endl;
                    } else {
                        std::cout << "摄像头3重新初始化失败" << std::endl;
                    }
                } else {
                    m_camera3Active = false;
                    std::cout << "摄像头3设备不存在，无法重新初始化" << std::endl;
                }
            }
        }
        
        // 模拟其他摄像头画面
        simulateOtherCameras();
    } catch (const std::exception& e) {
        qDebug() << "Camera update error:" << e.what();
    } catch (...) {
        qDebug() << "Unknown camera update error";
    }
}

/**
 * @brief 模拟其他摄像头画面
 * 
 * 生成模拟的摄像头画面用于演示
 */
void ADASDisplay::simulateOtherCameras()
{
    // 模拟驾驶员监测画面
    QImage driverImage(640, 480, QImage::Format_RGB888);
    driverImage.fill(QColor(30, 30, 30));
    
    QPainter painter(&driverImage);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(driverImage.rect(), Qt::AlignCenter, "驾驶员监测\n(模拟数据)");
    
    // 绘制一个简单的人脸轮廓
    painter.setPen(QPen(Qt::green, 2));
    painter.drawEllipse(QPoint(320, 240), 100, 120);
    painter.drawEllipse(QPoint(280, 210), 20, 20); // 左眼
    painter.drawEllipse(QPoint(360, 210), 20, 20); // 右眼
    painter.drawArc(270, 260, 100, 50, 0, 180 * 16); // 微笑
    
    // 更新UI
    m_driverFeed->setPixmap(QPixmap::fromImage(driverImage));
    
    // 只模拟摄像头2和3（索引2和3），因为0和1使用真实摄像头
    for (int i = 3; i < 4 && i < m_cameraLabels.size(); ++i) {
        QImage image(640, 480, QImage::Format_RGB888);
        image.fill(QColor(30, 30, 30));
        
        QPainter painter(&image);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        
        // 根据索引绘制不同的模拟画面
        // if (i == 2) {
        //     painter.drawText(image.rect(), Qt::AlignCenter, "车道线检测\n(模拟数据)");
            
        //     // 绘制简单的车道线
        //     painter.setPen(QPen(Qt::yellow, 3));
        //     painter.drawLine(160, 480, 280, 240);
        //     painter.drawLine(480, 480, 360, 240);
        // } else
         {
            painter.drawText(image.rect(), Qt::AlignCenter, "车辆检测\n(模拟数据)");
            
            // 绘制简单的车辆轮廓
            painter.setPen(QPen(Qt::red, 2));
            painter.drawRect(220, 280, 200, 100);
            painter.drawRect(250, 230, 140, 50);
            painter.drawEllipse(250, 380, 40, 40); // 左前轮
            painter.drawEllipse(350, 380, 40, 40); // 右前轮
        }
        
        // 更新UI
        m_cameraLabels[i]->setPixmap(QPixmap::fromImage(image));
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
    frame->setLineWidth(1);  // 减小边框宽度
    frame->setStyleSheet(DATA_PANEL_STYLE);
    
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setSpacing(5);  // 减小间距
    layout->setContentsMargins(2, 2, 2, 2);  // 减小边距
    
    // 车速部分
    QWidget *speedWidget = new QWidget();
    QVBoxLayout *speedLayout = new QVBoxLayout(speedWidget);
    speedLayout->setContentsMargins(1, 1, 1, 1);  // 减小边距
    speedLayout->setSpacing(1);  // 减小间距
    
    QLabel *speedTitle = new QLabel("车速");
    speedTitle->setAlignment(Qt::AlignCenter);
    speedTitle->setFont(QFont("Arial", 10, QFont::Bold));  // 减小字体
    
    m_speedValue = new QLabel("0 km/h");
    m_speedValue->setAlignment(Qt::AlignCenter);
    m_speedValue->setFont(QFont("Arial", 14, QFont::Bold));  // 减小字体
    
    m_speedProgress = new QProgressBar();
    m_speedProgress->setRange(0, 200);
    m_speedProgress->setValue(0);
    m_speedProgress->setFixedHeight(12);  // 减小进度条高度
    
    // 添加车速调整按钮到车速部分
    QHBoxLayout *speedControlsLayout = new QHBoxLayout();
    speedControlsLayout->setSpacing(2);  // 减小间距
    
    QPushButton *decreaseSpeedBtn = new QPushButton("-");
    decreaseSpeedBtn->setFixedSize(25, 20);  // 减小按钮尺寸
    connect(decreaseSpeedBtn, &QPushButton::clicked, this, &ADASDisplay::decreaseSpeed);
    
    QPushButton *increaseSpeedBtn = new QPushButton("+");
    increaseSpeedBtn->setFixedSize(25, 20);  // 减小按钮尺寸
    connect(increaseSpeedBtn, &QPushButton::clicked, this, &ADASDisplay::increaseSpeed);
    
    speedControlsLayout->addWidget(decreaseSpeedBtn);
    speedControlsLayout->addWidget(increaseSpeedBtn);
    
    speedLayout->addWidget(speedTitle);
    speedLayout->addWidget(m_speedValue);
    speedLayout->addWidget(m_speedProgress);
    speedLayout->addLayout(speedControlsLayout);
    
    // 合并报警状态和驾驶员信息部分
    QWidget *statusWidget = new QWidget();
    QVBoxLayout *statusLayout = new QVBoxLayout(statusWidget);
    statusLayout->setContentsMargins(1, 1, 1, 1);  // 减小边距
    statusLayout->setSpacing(1);  // 减小间距
    
    QHBoxLayout *statusTitleLayout = new QHBoxLayout();
    statusTitleLayout->setSpacing(2);  // 减小间距
    
    QLabel *alarmTitle = new QLabel("系统状态");
    alarmTitle->setAlignment(Qt::AlignCenter);
    alarmTitle->setFont(QFont("Arial", 10, QFont::Bold));  // 减小字体
    
    QLabel *driverTitle = new QLabel("驾驶员状态");
    driverTitle->setAlignment(Qt::AlignCenter);
    driverTitle->setFont(QFont("Arial", 10, QFont::Bold));  // 减小字体
    
    statusTitleLayout->addWidget(alarmTitle);
    statusTitleLayout->addWidget(driverTitle);
    
    QHBoxLayout *statusValueLayout = new QHBoxLayout();
    statusValueLayout->setSpacing(2);  // 减小间距
    
    m_alarmStatus = new QLabel("正常");
    m_alarmStatus->setAlignment(Qt::AlignCenter);
    m_alarmStatus->setFont(QFont("Arial", 14, QFont::Bold));  // 减小字体
    
    m_driverStatus = new QLabel("注意力集中");
    m_driverStatus->setAlignment(Qt::AlignCenter);
    m_driverStatus->setFont(QFont("Arial", 14));  // 减小字体
    
    statusValueLayout->addWidget(m_alarmStatus);
    statusValueLayout->addWidget(m_driverStatus);
    
    QHBoxLayout *statusProgressLayout = new QHBoxLayout();
    statusProgressLayout->setSpacing(2);  // 减小间距
    
    QLabel *fatigueLabel = new QLabel("疲劳度:");
    fatigueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    fatigueLabel->setFont(QFont("Arial", 10));  // 减小字体
    
    m_driverFatigue = new QProgressBar();
    m_driverFatigue->setRange(0, 100);
    m_driverFatigue->setValue(20);
    m_driverFatigue->setFixedHeight(12);  // 减小进度条高度
    
    statusProgressLayout->addWidget(fatigueLabel, 1);
    statusProgressLayout->addWidget(m_driverFatigue, 3);
    
    // 添加报警按钮到状态部分
    QHBoxLayout *alarmButtonLayout = new QHBoxLayout();
    alarmButtonLayout->setSpacing(2);  // 减小间距
    
    m_alarmButton = new QPushButton("报警");
    m_alarmButton->setFixedHeight(20);  // 减小按钮高度
    connect(m_alarmButton, &QPushButton::clicked, this, &ADASDisplay::toggleAlarm);
    
    // 添加帮助和退出按钮
    QPushButton *helpButton = new QPushButton("帮助");
    helpButton->setFixedHeight(20);  // 减小按钮高度
    connect(helpButton, &QPushButton::clicked, this, &ADASDisplay::showHelp);
    
    QPushButton *exitButton = new QPushButton("退出");
    exitButton->setFixedHeight(20);  // 减小按钮高度
    connect(exitButton, &QPushButton::clicked, this, &ADASDisplay::closeApplication);
    
    alarmButtonLayout->addWidget(m_alarmButton);
    alarmButtonLayout->addWidget(helpButton);
    alarmButtonLayout->addWidget(exitButton);
    
    statusLayout->addLayout(statusTitleLayout);
    statusLayout->addLayout(statusValueLayout);
    statusLayout->addLayout(statusProgressLayout);
    statusLayout->addLayout(alarmButtonLayout);
    
    // 添加所有部分到主布局
    layout->addWidget(speedWidget, 1);
    layout->addWidget(statusWidget, 2);
    
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
    m_alarmActive = !m_alarmActive;
    
    if (m_alarmActive) {
        m_alarmStatus->setText("警报");
        m_alarmStatus->setStyleSheet("color: red; font-weight: bold;");
        m_alarmButton->setText("解除");
        statusBar()->showMessage("系统报警已激活", 2000);
    } else {
        m_alarmStatus->setText("正常");
        m_alarmStatus->setStyleSheet("color: green; font-weight: bold;");
        m_alarmButton->setText("报警");
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

/**
 * @brief 交换两个摄像头的位置
 * @param sourcePos 源摄像头位置
 * @param targetPos 目标摄像头位置
 * 
 * 该功能已禁用，保留函数以兼容现有代码
 */
void ADASDisplay::swapCameras(int sourcePos, int targetPos)
{
    // 该功能已禁用
    qDebug() << "摄像头交换功能已禁用";
}

/**
 * @brief 创建应用程序图标
 * @return 应用程序图标
 * 
 * 创建应用程序图标
 */
QIcon ADASDisplay::createAppIcon()
{
    // 创建应用程序图标
    return QIcon(":/icon/icon.png");
}

/**
 * @brief 切换全屏模式
 * 
 * 切换全屏模式并更新显示
 */
void ADASDisplay::toggleFullScreen()
{
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}
