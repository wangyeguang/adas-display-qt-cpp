#include "icon.h"
#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QPen>
#include <QFont>

QIcon createAppIcon()
{
    // 创建一个空白的图标
    QPixmap pixmap(128, 128);
    pixmap.fill(Qt::transparent);
    
    // 创建画笔
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.setBrush(QBrush(QColor(40, 80, 140)));  // 深蓝色
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(4, 4, 120, 120, 15, 15);
    
    // 绘制汽车轮廓
    painter.setBrush(QBrush(QColor(220, 220, 220)));
    painter.setPen(QPen(QColor(50, 50, 50), 2));
    
    // 车身
    painter.drawRoundedRect(24, 48, 80, 30, 10, 10);
    
    // 车顶
    painter.drawRoundedRect(44, 28, 40, 20, 8, 8);
    
    // 车轮
    painter.setBrush(QBrush(QColor(50, 50, 50)));
    painter.drawEllipse(34, 78, 20, 20);
    painter.drawEllipse(74, 78, 20, 20);
    
    // 绘制雷达/摄像头符号
    painter.setBrush(QBrush(QColor(255, 100, 100)));
    painter.setPen(QPen(QColor(200, 0, 0), 1));
    painter.drawEllipse(59, 18, 10, 10);
    
    // 绘制雷达波
    painter.setPen(QPen(QColor(100, 200, 255), 2));
    painter.drawArc(49, 8, 30, 30, 30 * 16, 120 * 16);
    painter.drawArc(44, 3, 40, 40, 30 * 16, 120 * 16);
    
    // 绘制文本
    painter.setPen(QPen(QColor(255, 255, 255)));
    QFont font("Arial", 12, QFont::Bold);
    painter.setFont(font);
    painter.drawText(QRect(14, 98, 100, 20), Qt::AlignCenter, "ADAS");
    
    // 结束绘制
    painter.end();
    
    // 创建图标
    QIcon icon(pixmap);
    return icon;
}
