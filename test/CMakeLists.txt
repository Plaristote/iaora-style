set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src/ ${KDE4_INCLUDES} )

set( qttest_SRCS 
     qttest.cpp 
     mywindow.cpp )
kde4_add_ui_files(qttest_SRCS mainwindow.ui)
kde4_add_executable(qttest ${qttest_SRCS})
target_link_libraries(qttest iaora-qt ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBRARY} ${KDE4_KDEUI_LIBRARY})
