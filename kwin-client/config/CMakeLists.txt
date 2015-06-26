set (kwin_iaora_config_SOURCES iaora_config.cpp)
kde4_add_ui_files(kwin_iaora_config_SOURCES iaora_config.ui)

kde4_add_plugin (kwin_iaora_config ${kwin_iaora_config_SOURCES})

target_link_libraries (kwin_iaora_config ${KDE4_KDEUI_LIBS})
install (TARGETS kwin_iaora_config DESTINATION ${PLUGIN_INSTALL_DIR})
