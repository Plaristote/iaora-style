IaOra
====================

The old Mandriva's IaOra theme ported to Qt5...actually just the style, not the window decoration.

https://store.kde.org/p/1183616/

Installation from source
------------------------

    mkdir build
    cd build
    cmake -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` ..
    make
    sudo make install
    kbuildsycoca5
