#ifndef STYLES_H
#define STYLES_H

#include <QString>

// 主样式
const QString MAIN_STYLE = R"(
QMainWindow {
    background-color: #1a1a1a;
}

QFrame {
    border: 1px solid #3498db;
    border-radius: 2px;
    background-color: #2c3e50;
}

QLabel {
    color: #ecf0f1;
    font-size: 12px;
}

QPushButton {
    background-color: #3498db;
    color: white;
    border: none;
    padding: 6px 12px;
    border-radius: 2px;
    font-weight: bold;
    font-size: 12px;
}

QPushButton:hover {
    background-color: #2980b9;
}

QPushButton:pressed {
    background-color: #1a5276;
}

QPushButton#dangerButton {
    background-color: #e74c3c;
}

QPushButton#dangerButton:hover {
    background-color: #c0392b;
}

QPushButton#warningButton {
    background-color: #f39c12;
}

QPushButton#warningButton:hover {
    background-color: #d35400;
}

QProgressBar {
    border: 1px solid #34495e;
    border-radius: 2px;
    text-align: center;
    background-color: #2c3e50;
    color: white;
    font-size: 10px;
    height: 15px;
}

QProgressBar::chunk {
    background-color: #2ecc71;
    width: 5px;
    margin: 0.5px;
}
)";

// 状态面板样式
const QString DATA_PANEL_STYLE = R"(
QFrame#statusPanel {
    background-color: #2c3e50;
    padding: 5px;
    border-top: 1px solid #3498db;
}

QLabel {
    color: #ecf0f1;
    font-size: 11px;
}

QProgressBar {
    height: 12px;
}
)";

// 控制面板样式
const QString CONTROL_PANEL_STYLE = R"(
QFrame#controlPanel {
    background-color: #2c3e50;
    padding: 5px;
}

QPushButton {
    min-width: 30px;
    padding: 4px 8px;
}
)";

// 图像面板样式
const QString IMAGE_PANEL_STYLE = R"(
QFrame#cameraPanel {
    background-color: #1a1a1a;
    border: 1px solid #3498db;
    margin: 0px;
}

QLabel {
    color: #ecf0f1;
    font-size: 10px;
    background-color: #1a1a1a;
    border: none;
}
)";

// 数值面板样式
const QString VALUE_PANEL_STYLE = R"(
QFrame#valuePanel {
    background-color: #2c3e50;
    padding: 5px;
}

QLabel {
    font-size: 11px;
}
)";

#endif // STYLES_H
