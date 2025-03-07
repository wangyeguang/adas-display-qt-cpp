cmake_minimum_required(VERSION 3.14)
project(ADAS_System LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 查找OpenCV包
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PROJECT_SOURCES
    main.cpp
    adasdisplay.h
    adasdisplay.cpp
    draggablecamerapanel.h
    draggablecamerapanel.cpp
    icon.h
    icon.cpp
    styles.h
)

add_executable(ADAS_System
    ${PROJECT_SOURCES}
)

# 链接Qt和OpenCV库
target_link_libraries(ADAS_System PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS}
)
