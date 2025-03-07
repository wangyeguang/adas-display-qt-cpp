/**
 * @file draggablecamerapanel.h
 * @brief 可拖拽摄像头面板的头文件
 * 
 * 该文件定义了DraggableCameraPanel类，实现了一个可以通过拖拽操作
 * 进行位置交换的摄像头显示面板。
 */
#ifndef DRAGGABLECAMERAPANEL_H
#define DRAGGABLECAMERAPANEL_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

/**
 * @class DraggableCameraPanel
 * @brief 可拖拽的摄像头面板类
 * 
 * 该类继承自QFrame，实现了一个可以通过拖拽操作进行位置交换的摄像头显示面板。
 * 支持鼠标拖放操作，允许用户自定义摄像头布局。
 */
class DraggableCameraPanel : public QFrame
{
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    explicit DraggableCameraPanel(QWidget *parent = nullptr);
    
    /**
     * @brief 获取摄像头显示标签
     * @return 返回摄像头显示标签指针
     */
    QLabel* cameraFeed() const { return m_cameraFeed; }
    
    /**
     * @brief 获取摄像头位置
     * @return 返回摄像头位置索引
     */
    int cameraPosition() const { return m_cameraPosition; }
    
    /**
     * @brief 设置摄像头位置
     * @param position 摄像头位置索引
     */
    void setCameraPosition(int position) { m_cameraPosition = position; }
    
protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     * 
     * 当用户按下鼠标时触发，开始拖拽操作
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 拖拽进入事件处理
     * @param event 拖拽进入事件
     * 
     * 当拖拽操作进入该面板时触发
     */
    void dragEnterEvent(QDragEnterEvent *event) override;
    
    /**
     * @brief 放下事件处理
     * @param event 放下事件
     * 
     * 当拖拽操作在该面板上释放时触发，完成摄像头位置交换
     */
    void dropEvent(QDropEvent *event) override;
    
private:
    QLabel *m_cameraFeed;    ///< 摄像头图像显示标签
    int m_cameraPosition;    ///< 摄像头位置索引
};

#endif // DRAGGABLECAMERAPANEL_H
