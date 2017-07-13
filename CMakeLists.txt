project(iaora)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.11 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(KF5 QUIET COMPONENTS Config)

add_subdirectory(src)
if(KF5_FOUND)
    message(STATUS "KF5 found, installing colorschemes too.")
    include(KDEInstallDirs)
    add_subdirectory(color-schemes)
endif(KF5_FOUND)
