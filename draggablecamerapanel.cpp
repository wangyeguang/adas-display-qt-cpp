/**
 * @file draggablecamerapanel.cpp
 * @brief 可拖拽摄像头面板的实现文件
 */
#include "draggablecamerapanel.h"
#include "styles.h"

/**
 * @brief DraggableCameraPanel类的构造函数
 * @param parent 父窗口指针
 * 
 * 初始化面板样式和布局，创建摄像头图像显示标签
 */
DraggableCameraPanel::DraggableCameraPanel(QWidget *parent)
    : QFrame(parent)
    , m_cameraPosition(-1)
{
    setObjectName("cameraPanel");
    setFrameShape(QFrame::Box);
    setLineWidth(2);
    setStyleSheet(IMAGE_PANEL_STYLE);
    setAcceptDrops(true);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // 创建摄像头显示标签
    m_cameraFeed = new QLabel(this);
    m_cameraFeed->setAlignment(Qt::AlignCenter);
    m_cameraFeed->setStyleSheet("background-color: #222; color: white;");
    m_cameraFeed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_cameraFeed->setMinimumSize(320, 240);  // 设置最小尺寸
    m_cameraFeed->setScaledContents(true);   // 图像自动缩放以适应标签大小
    layout->addWidget(m_cameraFeed);
}

/**
 * @brief 鼠标按下事件处理函数
 * @param event 鼠标事件对象
 * 
 * 当用户按下鼠标左键时，创建一个拖拽对象，并将当前摄像头位置作为数据传递
 */
void DraggableCameraPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(QString::number(m_cameraPosition));
        drag->setMimeData(mimeData);
        drag->exec(Qt::MoveAction);
    }
}

/**
 * @brief 拖拽进入事件处理函数
 * @param event 拖拽进入事件对象
 * 
 * 当拖拽对象进入此面板时，如果包含文本数据（摄像头位置），则接受拖拽操作
 */
void DraggableCameraPanel::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

/**
 * @brief 放下事件处理函数
 * @param event 放下事件对象
 * 
 * 当拖拽对象在此面板上释放时，获取源摄像头位置，
 * 并通过父窗口的swapCameras方法交换两个摄像头的位置
 */
void DraggableCameraPanel::dropEvent(QDropEvent *event)
{
    int sourcePosition = event->mimeData()->text().toInt();
    if (parent() && parent()->inherits("ADASDisplay")) {
        QMetaObject::invokeMethod(parent(), "swapCameras", 
                                 Q_ARG(int, sourcePosition), 
                                 Q_ARG(int, m_cameraPosition));
    }
}
