project (style-iaora)

set (STYLE_SOURCES iaorastyle.cpp 
    iaorastyle_button.cpp 
    iaorastyle_icons.cpp 
    iaorastyle_scrollbar.cpp 
    iaorastyle_tabbar.cpp
    iaorastyle_dial.cpp
    iaorastyle_menu.cpp
    iaorastyle_slider.cpp
    iaorastyle_window.cpp
    iaorastyle_frame.cpp
    iaorastyle_progressbar.cpp
    iaorastyle_symbols.cpp 
    iaorastyle_combobox.cpp
    iaorastyle_spinbox.cpp 
    iaorastyle_toolbutton.cpp )

set (iaora-qt_SOURCES iaora-qt.cpp ${STYLE_SOURCES})

add_library (iaora-qt SHARED ${iaora-qt_SOURCES})
target_link_libraries (iaora-qt Qt5::Core Qt5::Widgets)
    
get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)
execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
  OUTPUT_VARIABLE _Qt5_PLUGIN_INSTALL_DIR
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
install (TARGETS iaora-qt LIBRARY DESTINATION ${_Qt5_PLUGIN_INSTALL_DIR}/styles/)
