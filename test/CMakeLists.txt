set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src/  )

set( qttest_SRCS 
     qttest.cpp 
     mywindow.cpp )
ki18n_wrap_ui(qttest_SRCS mainwindow.ui)
add_executable(qttest ${qttest_SRCS})
target_link_libraries(qttest iaora-qt Qt5::Gui  )
