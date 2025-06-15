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
target_link_libraries (iaora-qt Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

if (QT_VERSION_MAJOR EQUAL 6)
  get_target_property(REAL_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
else()
  get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)
endif()

execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
  OUTPUT_VARIABLE _Qt_PLUGIN_INSTALL_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Install path = ${_Qt_PLUGIN_INSTALL_DIR}")

install (TARGETS iaora-qt LIBRARY DESTINATION ${_Qt_PLUGIN_INSTALL_DIR}/styles/)
