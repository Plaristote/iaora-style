project(iaora)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

add_subdirectory(src)
add_subdirectory(color-schemes)
