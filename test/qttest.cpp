#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include <QAction>
#include <QString>
#include <QStyleFactory>

#include <KIcon>
#include "mywindow.h"
#include "ui_mainwindow.h"

int main(int argc, char **argv)
{
    KAboutData aboutData(
        // The program name used internally.
        "qttest",
        // The message catalog name
        // If null, program name is used instead.
        0,
        // A displayable program name string.
        ki18n("Qt Style Test"),
        // The program version string.
        "1.0",
        // Short description of what the app does.
        ki18n("A testbed for trying out Qt styles"),
        // The license this code is released under
        KAboutData::License_GPL,
        // Copyright Statement
        ki18n("(c) 2007"),
        // Optional text shown in the About box.
        // Can contain any information desired.
        ki18n("Some text..."),
        // The program homepage string.
        "http://tutorial.com/",
        // The bug report email address
        "submit@bugs.kde.org");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    MyWindow win;
    Ui::MainWindow win_ui;

    win_ui.setupUi(&win);

    // the styles toolbar
    win_ui.stylesCombo->addItem("IaOraQt");
    win_ui.stylesCombo->addItems(QStyleFactory::keys());
    QObject::connect(win_ui.stylesCombo, SIGNAL(activated(const QString &)),
                     &win, SLOT(slotSetStyle(const QString &)));

    // the big toolbar
    win_ui.toolBar->addAction(KIcon("akonadi"), "Foobar");
    win_ui.toolBar->addSeparator();
    win_ui.toolBar->addAction(KIcon("konqueror"), "Web Browser");
    win_ui.toolBar->setIconSize(QSize(32, 32));

    win_ui.notEditableCombo->addItem("Foobar");
    win_ui.notEditableCombo->addItem("Foobar2");
    win_ui.notEditableCombo->addItem("Foobar3");
    win_ui.notEditableCombo->addItem("Foobar4");
    win_ui.notEditableCombo->addItem("Foobar5");

    win_ui.comboBox->addItem("Foobar");
    win_ui.comboBox->addItem("Foobar2");
    win_ui.comboBox->addItem("Foobar3");
    win_ui.comboBox->addItem("Foobar4");
    win_ui.comboBox->addItem("Foobar5");

    win_ui.tristateCheck->setCheckState(Qt::PartiallyChecked);
    win_ui.tristateDisable->setCheckState(Qt::PartiallyChecked);

    //the small toolbar
    win_ui.smallToolBar->addAction(KIcon("plasma"), "Desktop");
    win_ui.smallToolBar->addSeparator();
    win_ui.smallToolBar->addAction(KIcon("kopete"), "Instant Message");
    win.slotSetStyle("IaOraQt");
    win.show();

    QMenuBar *menubar = new QMenuBar(win_ui.subwindow);
    QMenu *menu = new QMenu(win_ui.subwindow);
    QAction *action;
    menu->setTearOffEnabled(true);

    menu->addAction(KIcon("digikam"), "Take snapshot");
    action = menu->addAction(KIcon("kde"), "Make KDE4 work");
    action->setEnabled(false);
    menu->addSeparator();

    action = new QAction(KIcon("kchart"), "Checked icon item", win_ui.subwindow);
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);

    action = new QAction(KIcon("ktip"), "Unchecked icon item", win_ui.subwindow);
    action->setCheckable(true);
    menu->addAction(action);

    action = new QAction("Checked item", win_ui.subwindow);
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);

    action = new QAction("Unchecked item", win_ui.subwindow);
    action->setCheckable(true);
    menu->addAction(action);

    QActionGroup group(win_ui.subwindow);
    group.setExclusive(true);
    action = new QAction("Unique checked item", win_ui.subwindow);
    action->setCheckable(true);
    action->setChecked(true);
    group.addAction(action);
    menu->addAction(action);

    action = new QAction("Unique unchecked item", win_ui.subwindow);
    action->setCheckable(true);
    group.addAction(action);
    menu->addAction(action);

    menu->addSeparator();
    menu->addAction(KIcon("kbugbuster"), "Kill some bugs");
    menu->setTitle("A menu");
    menubar->addMenu(menu);
    menubar->addMenu(KIcon("kde"), "Foobar");
    menubar->addSeparator();
    menubar->addMenu("Another menu");

    menubar->show();
    win_ui.subwindow->layout()->addWidget(menubar);
    return app.exec();
}
