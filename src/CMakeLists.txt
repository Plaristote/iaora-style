project (kstyle-iaora)

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
                   iaorastyle_q3listview.cpp
		   iaorastyle_toolbutton.cpp )

set (iaora-kde_SOURCES iaora-kde.cpp ${STYLE_SOURCES})
set (iaora-qt_SOURCES iaora-qt.cpp ${STYLE_SOURCES})

if (KDE4_FOUND)
	kde4_add_plugin (iaora-kde ${iaora-kde_SOURCES})
	target_link_libraries (iaora-kde ${KDE4_KDEUI_LIBS})
	install (TARGETS iaora-kde DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/styles)
endif (KDE4_FOUND)

	qt4_automoc (${iaora-qt_SOURCES})
	add_library (iaora-qt SHARED ${iaora-qt_SOURCES})
	target_link_libraries (iaora-qt ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
	install (TARGETS iaora-qt LIBRARY DESTINATION ${QT_PLUGINS_DIR}/styles)
    #install (FILES qtiaorarc DESTINATION ${KCFG_INSTALL_DIR})
    install (FILES iaoracolors DESTINATION /etc )

if (KDE4_FOUND)
	install (FILES iaora-kde.themerc DESTINATION ${DATA_INSTALL_DIR}/kstyle/themes)
	#install (FILES qtiaorarc DESTINATION ${KCFG_INSTALL_DIR})
	#add_subdirectory (config)
endif (KDE4_FOUND)

