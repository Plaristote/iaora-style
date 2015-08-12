project (iaora)
cmake_minimum_required (VERSION 2.8.12)

find_package(ECM 0.0.11 REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
    
find_package (KF5)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
include(KDEInstallDirs)

add_subdirectory (src)
add_subdirectory (color-schemes)
#add_subdirectory (test)

#if (KDE4_FOUND)
#	add_subdirectory (kwin-client)
#endif (KDE4_FOUND)

