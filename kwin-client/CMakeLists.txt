project (kwin-iaora)

set (kwin_iaora_SOURCES kwin_iaora.cpp)
kde4_add_plugin (kwin3_iaora ${kwin_iaora_SOURCES})
target_link_libraries (kwin3_iaora ${KDE4_KDEUI_LIBS} kdecorations)
install (TARGETS kwin3_iaora DESTINATION ${PLUGIN_INSTALL_DIR})
install (FILES iaora.desktop DESTINATION ${DATA_INSTALL_DIR}/kwin)

add_subdirectory (config)

