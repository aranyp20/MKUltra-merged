cmake_minimum_required(VERSION 3.14)

project(gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(OpenGL REQUIRED)




find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(UI_HEADERS mainwindow.ui)

add_executable(${PROJECT_NAME}
main.cpp
mainwindow.cpp
mainwindow.ui
myglwidget.cpp
${UI_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL Qt5::Widgets)