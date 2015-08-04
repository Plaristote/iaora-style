#include "mywindow.h"
#include "iaora-qt.h"
#include <KApplication>
#include <QStyleFactory>
#include <QMenu>
#include <KIcon>

MyWindow::MyWindow()
    : QMainWindow()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotContextMenu(const QPoint&)));
}

MyWindow::~MyWindow()
{
    // do nothing for now
}

void MyWindow::slotSetStyle(const QString &style)
{
    if (style ==  "IaOraQt")
        KApplication::setStyle(new IaOraQt());
    else
        KApplication::setStyle(QStyleFactory::create(style));
    update();
}

void MyWindow::slotContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    QAction *action;
    menu->setTearOffEnabled(true);

    menu->addAction(KIcon("digikam"), "Take snapshot");
    action = menu->addAction(KIcon("kde"), "Make KDE4 work");
    action->setEnabled(false);
    menu->addSeparator();

    action = new QAction(KIcon("kchart"), "Checked icon item", this);
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);

    action = new QAction(KIcon("ktip"), "Unchecked icon item", this);
    action->setCheckable(true);
    menu->addAction(action);

    action = new QAction("Checked item", this);
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);

    action = new QAction("Unchecked item", this);
    action->setCheckable(true);
    menu->addAction(action);

    QActionGroup group(this);
    group.setExclusive(true);
    action = new QAction("Unique checked item", this);
    action->setCheckable(true);
    action->setChecked(true);
    group.addAction(action);
    menu->addAction(action);

    action = new QAction("Unique unchecked item", this);
    action->setCheckable(true);
    group.addAction(action);
    menu->addAction(action);

    menu->addSeparator();
    action = new QAction(KIcon("kbugbuster"), "Kill some bugs", this);
    menu->addAction(action);
    menu->setDefaultAction(action);

    QMenu *subMenu = new QMenu("A SubMenu", this);
    subMenu->setIcon(KIcon("konqueror"));
    subMenu->addAction(new QAction(KIcon("dolphin"), "A SubItem", this));
    menu->addMenu(subMenu);
    menu->exec(QCursor::pos());
    delete menu;
}
