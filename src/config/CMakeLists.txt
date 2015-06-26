set (kstyle_skulpture_config_SOURCES sk_config.cpp)
qt4_add_resources (kstyle_skulpture_config_SOURCES sk_preview2.qrc)
kde4_add_ui_files (kstyle_skulpture_config_SOURCES sk_dialog.ui sk_preview1.ui sk_preview2.ui)

kde4_add_plugin (kstyle_skulpture_config ${kstyle_skulpture_config_SOURCES})
target_link_libraries (kstyle_skulpture_config ${KDE4_KDEUI_LIBS})
install (TARGETS kstyle_skulpture_config DESTINATION ${PLUGIN_INSTALL_DIR})

