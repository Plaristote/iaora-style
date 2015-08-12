set (kwin_iaora_config_SOURCES iaora_config.cpp)
ki18n_wrap_ui(kwin_iaora_config_SOURCES iaora_config.ui)

add_library(kwin_iaora_config MODULE ${kwin_iaora_config_SOURCES})

target_link_libraries (kwin_iaora_config )
install (TARGETS kwin_iaora_config DESTINATION ${KDE_INSTALL_PLUGINDIR})
