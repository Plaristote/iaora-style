project (iaora)
cmake_minimum_required (VERSION 2.4)

find_package (KDE4)

if (KDE4_FOUND)
	include (KDE4Defaults)
	include (MacroLibrary)
	add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )
	include_directories (${KDE4_INCLUDES})
else (KDE4_FOUND)
	message (STATUS "WARNING: *** KDE4 not found, building a Qt-only version of IaOra ***")
	set (QT_MIN_VERSION "4.3.0")
	find_package (Qt4 REQUIRED)
	include (UseQt4)
	set (CMAKE_INCLUDE_CURRENT_DIR ON)
endif (KDE4_FOUND)

add_subdirectory (src)
add_subdirectory (color-schemes)
add_subdirectory (test)

#if (KDE4_FOUND)
#	add_subdirectory (kwin-client)
#endif (KDE4_FOUND)

