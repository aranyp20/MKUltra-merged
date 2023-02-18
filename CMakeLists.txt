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

qt5_wrap_ui(UI_HEADERS src/gui/main_window.ui)

add_executable(${PROJECT_NAME}
src/gui/main.cpp
src/gui/main_window.cpp
src/gui/main_window.ui
src/gui/poly_2D_widget.cpp
${UI_HEADERS}
src/slicer/slicer_module.cpp 
src/slicer/generators/inner_shell_generator.cpp
src/slicer/generators/outer_shell_generator.cpp
src/slicer/generators/gentools/interval.cpp
src/slicer/generators/gentools/polylines.cpp
src/slicer/generators/gentools/section_organiser.cpp
src/slicer/global/section.cpp
src/slicer/object/RBF/FunctionCreator.cpp
src/slicer/object/RBF/NormalCreator.cpp
src/slicer/generators/infill_generator.cpp
src/slicer/generators/gentools/plane.cpp
src/slicer/printable_object.cpp
src/settings.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer/global ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer/generators ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer/generators/gentools ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer/object ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer/object/RBF ${CMAKE_CURRENT_SOURCE_DIR}/src/slicer)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/gui ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL Qt5::Widgets)