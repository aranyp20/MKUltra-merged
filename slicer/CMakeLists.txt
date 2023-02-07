cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MKUltra VERSION 1.0)

add_executable(${PROJECT_NAME} 
slicer_module.cpp 
generators/inner_shell_generator.cpp
generators/outer_shell_generator.cpp
generators/gentools/interval.cpp
generators/gentools/polylines.cpp
generators/gentools/section_organiser.cpp
global/section.cpp
object/RBF/FunctionCreator.cpp
object/RBF/NormalCreator.cpp
generators/infill_generator.cpp
generators/gentools/plane.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/global ${CMAKE_CURRENT_SOURCE_DIR}/generators ${CMAKE_CURRENT_SOURCE_DIR}/generators/gentools ${CMAKE_CURRENT_SOURCE_DIR}/object ${CMAKE_CURRENT_SOURCE_DIR}/object/RBF)
