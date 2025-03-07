/**
 * @file adasdisplay.h
 * @brief 高级驾驶辅助系统(ADAS)显示界面的头文件
 * 
 * 该文件定义了ADASDisplay类，它是应用程序的主窗口，
 * 负责显示和管理所有ADAS相关的UI元素和数据。
 */
#ifndef ADASDISPLAY_H
#define ADASDISPLAY_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QStatusBar>
#include <QVector>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QWidget>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QImage>
#include <QPixmap>

// OpenCV头文件
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "draggablecamerapanel.h"

/**
 * @class ADASDisplay
 * @brief 高级驾驶辅助系统(ADAS)的主显示界面类
 * 
 * 该类继承自QMainWindow，实现了ADAS系统的用户界面，
 * 包括摄像头视图、车辆状态面板、驾驶员状态监控等功能。
 */
class ADASDisplay : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    ADASDisplay(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~ADASDisplay();
    
public slots:
    /**
     * @brief 交换两个摄像头的位置（已禁用）
     * @param sourcePos 源摄像头位置
     * @param targetPos 目标摄像头位置
     */
    void swapCameras(int sourcePos, int targetPos);
    
private slots:
    /**
     * @brief 更新显示数据，包括车速、驾驶员疲劳度等
     */
    void updateData();
    
    /**
     * @brief 更新日期时间显示
     */
    void updateDateTime();
    
    /**
     * @brief 增加车速
     */
    void increaseSpeed();
    
    /**
     * @brief 减少车速
     */
    void decreaseSpeed();
    
    /**
     * @brief 切换警报状态
     */
    void toggleAlarm();
    
    /**
     * @brief 关闭应用程序
     */
    void closeApplication();
    
    /**
     * @brief 显示帮助信息
     */
    void showHelp();
    
    /**
     * @brief 更新摄像头画面
     */
    void updateCameraFeeds();
    
    /**
     * @brief 切换全屏模式
     */
    void toggleFullScreen();
    
    /**
     * @brief 模拟其他摄像头画面
     * 
     * 生成模拟的摄像头画面用于演示
     */
    void simulateOtherCameras();
    
private:
    /**
     * @brief 初始化用户界面
     */
    void initUI();
    
    /**
     * @brief 设置定时器
     */
    void setupTimers();
    
    /**
     * @brief 创建状态面板
     * @return 返回创建好的状态面板
     */
    QFrame* createStatusPanel();
    
    /**
     * @brief 初始化摄像头
     * @return 是否成功初始化摄像头
     */
    bool initCameras();
    
    /**
     * @brief 关闭摄像头
     */
    void closeCameras();
    
    /**
     * @brief 将OpenCV的Mat转换为QImage
     * @param mat OpenCV的Mat图像
     * @return 转换后的QImage
     */
    QImage matToQImage(const cv::Mat& mat);
    
    /**
     * @brief 创建应用程序图标
     * @return 应用程序图标
     */
    QIcon createAppIcon();
    
    // 摄像头面板
    DraggableCameraPanel *m_driverCamera;     ///< 驾驶员摄像头面板（旧的，保留以避免大量修改）
    QVector<DraggableCameraPanel*> m_cameras; ///< 其他摄像头面板集合（旧的，保留以避免大量修改）
    
    // 新的摄像头UI组件
    QVector<QLabel*> m_cameraLabels;          ///< 摄像头画面标签集合
    QLabel *m_driverFeed;                     ///< 驾驶员摄像头画面标签
    
    // 状态面板组件
    QFrame *m_statusPanel;           ///< 状态面板框架
    QLabel *m_speedValue;            ///< 车速值标签
    QProgressBar *m_speedProgress;   ///< 车速进度条
    QLabel *m_alarmStatus;           ///< 警报状态标签
    QPushButton *m_alarmButton;      ///< 警报按钮
    QLabel *m_driverStatus;          ///< 驾驶员状态标签
    QProgressBar *m_driverFatigue;   ///< 驾驶员疲劳度进度条
    QLabel *m_datetimeLabel;         ///< 日期时间标签
    
    // 计时器
    QTimer *m_dataTimer;             ///< 数据更新定时器
    QTimer *m_datetimeTimer;         ///< 日期时间更新定时器
    QTimer *m_cameraTimer;           ///< 摄像头更新定时器
    
    // 数据值
    double m_currentSpeed;           ///< 当前车速
    bool m_alarmActive;              ///< 警报激活状态
    int m_fatigueLevel;              ///< 疲劳度级别
    
    // OpenCV摄像头
    cv::VideoCapture m_camera0;      ///< 摄像头0 (/dev/video0)
    cv::VideoCapture m_camera1;      ///< 摄像头1 (/dev/video2)
    bool m_camera0Active;            ///< 摄像头0是否激活
    bool m_camera1Active;            ///< 摄像头1是否激活
};

#endif // ADASDISPLAY_H
