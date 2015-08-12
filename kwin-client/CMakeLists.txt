project (kwin-iaora)

set (kwin_iaora_SOURCES kwin_iaora.cpp)
add_library(kwin3_iaora MODULE ${kwin_iaora_SOURCES})
target_link_libraries (kwin3_iaora  kdecorations)
install (TARGETS kwin3_iaora DESTINATION ${KDE_INSTALL_PLUGINDIR})
install (FILES iaora.desktop DESTINATION ${KDE_INSTALL_DATADIR}/kwin)

add_subdirectory (config)

