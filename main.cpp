/**
 * @file main.cpp
 * @brief 高级驾驶辅助系统(ADAS)应用程序的入口点
 */
#include "adasdisplay.h"

#include <QApplication>

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 * 
 * 创建Qt应用程序实例和ADAS显示界面，并启动应用程序的事件循环
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    ADASDisplay display;
    display.show();
    
    return app.exec();
}
