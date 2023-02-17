cmake_minimum_required(VERSION 3.14)

project(mkultra VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O0")


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(OpenGL REQUIRED)




find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(UI_HEADERS app/gui/mainwindow.ui)

add_executable(${PROJECT_NAME}
app/gui/main.cpp
app/gui/mainwindow.cpp
app/gui/mainwindow.ui
app/gui/myglwidget.cpp
${UI_HEADERS}
app/slicer/slicer_module.cpp 
app/slicer/generators/inner_shell_generator.cpp
app/slicer/generators/outer_shell_generator.cpp
app/slicer/generators/gentools/interval.cpp
app/slicer/generators/gentools/polylines.cpp
app/slicer/generators/gentools/section_organiser.cpp
app/slicer/global/section.cpp
app/slicer/object/RBF/FunctionCreator.cpp
app/slicer/object/RBF/NormalCreator.cpp
app/slicer/generators/infill_generator.cpp
app/slicer/generators/gentools/plane.cpp
app/slicer/printable_object.cpp
app/settings.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer/global ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer/generators ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer/generators/gentools ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer/object ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer/object/RBF ${CMAKE_CURRENT_SOURCE_DIR}/app/slicer)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/app/gui ${CMAKE_CURRENT_SOURCE_DIR}/app)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL Qt5::Widgets)