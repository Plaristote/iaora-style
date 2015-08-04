/*
 * sk_config.cpp
 *
 */

#include "sk_config.h"
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QUuid>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QCloseEvent>
#include <QtGui/QStyleFactory>
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtCore/QDebug>
#include <KDE/KGlobal>
#include <cstdio>


/*-----------------------------------------------------------------------*/

extern "C" KDE_EXPORT QWidget* allocate_kstyle_config(QWidget* parent)
{
    return new SkulptureStyleConfig(parent);
}


/*-----------------------------------------------------------------------*/

#include "ui_sk_preview1.h"

class Preview1Window : public QDialog, private Ui::SkulpturePreview1
{
public:
    explicit Preview1Window(QWidget *parent = 0);

public:
    virtual void closeEvent(QCloseEvent *e);

private:
    void init();
};


Preview1Window::Preview1Window(QWidget *parent)
    : QDialog(parent)
{
    init();
}


void Preview1Window::closeEvent(QCloseEvent *e)
{
    e->ignore();
}


void Preview1Window::init()
{
    setWindowFlags(Qt::Widget);
    setupUi(this);
}


/*-----------------------------------------------------------------------*/

#include "ui_sk_preview2.h"

class Preview2Window : public QMainWindow, private Ui::SkulpturePreview2
{
public:
    explicit Preview2Window(QWidget *parent = 0);

public:
    virtual void closeEvent(QCloseEvent *e);

private:
    void init();
};


Preview2Window::Preview2Window(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}


void Preview2Window::closeEvent(QCloseEvent *e)
{
    e->ignore();
}


void Preview2Window::init()
{
    setWindowFlags(Qt::Widget);
    setupUi(this);
}


/*-----------------------------------------------------------------------*/

class KThemePreview::Private
{
public:
    QWidget *q;
    QMdiArea *area;

public:
    void init();
};


KThemePreview::KThemePreview(QWidget *parent)
    : QWidget(parent), d(new Private)
{
    d->q = this;
}


KThemePreview::~KThemePreview()
{
    delete d;
}


/*-----------------------------------------------------------------------*/

void SkulptureStyleConfig::subWindowActivated(QMdiSubWindow *window)
{
    if (window) {
        QWidget *parent = window->parentWidget();
        if (parent) {
            const QList<QObject *> children = parent->children();
            foreach(QObject * child, children) {
                QWidget *widget = qobject_cast<QWidget *>(child);
                if (widget) {
                    widget->setWindowTitle(QString::fromUtf8(child == window ? "Active Window" : "Inactive Window"));
                }
            }
        }
    }
}


void SkulptureStyleConfig::updatePreview()
{
    QStyle *style = QStyleFactory::create(QString::fromUtf8("skulpture"));
    if (style) {
        int index = style->metaObject()->indexOfMethod("skulpturePrivateMethod(SkulpturePrivateMethod,void*)");
        if (index >= 0) {
            QUuid uuid = QUuid::createUuid();
            QDir tempDir = QDir::temp();
            // ### we may add a way to avoid the file completely.
            QString fileName = QString::fromUtf8("skulpture-temp-settings-") + uuid.toString() + QString::fromUtf8(".ini");
            QString absFileName = tempDir.absoluteFilePath(fileName);
            {
                QSettings s(absFileName, QSettings::IniFormat);
                saveSettings(s);
                // make visible in other process
                s.sync();
            }
            SkMethodDataSetSettingsFileName data;
            data.version = 1;
            data.fileName = absFileName;

            int result;
            int id = SkulptureStyle::SPM_SetSettingsFileName;
            void *dat = &data;
            void *param[] = {&result, &id, &dat};
            style->qt_metacall(QMetaObject::InvokeMetaMethod, index, param);

            tempDir.remove(fileName);

            QList<QMdiSubWindow *> windows = mdiArea->findChildren<QMdiSubWindow *>();
            foreach(QMdiSubWindow * window, windows) {
                window->setFocusPolicy(Qt::ClickFocus);
                QList<QWidget *> widgets = window->widget()->findChildren<QWidget *>();
                foreach(QWidget * widget, widgets) {
                    // TODO: only update widgets that have changes in settings
                    widget->setStyle(style);
                    widget->setFocusPolicy(Qt::ClickFocus);
                }
            }
        }
    }
};


/*-----------------------------------------------------------------------*/

SkulptureStyleConfig::SkulptureStyleConfig(QWidget *parent)
    : QWidget(parent)
{
    init();
}


void SkulptureStyleConfig::init()
{
    KGlobal::locale()->insertCatalog(QString::fromAscii("kstyle_skulpture_config"));
    setupUi(this);
    mdiArea->setMinimumSize(QSize(500, 290));
    mdiArea->setBackground(QColor(120, 120, 150));
    load();
//  KConfig wm(QString::fromAscii("kwinskulpturerc"), KConfig::FullConfig);
//  KConfigGroup group = wm.group("WM");
//  QColor activeBackground;
//  QString c = group.readEntry("activeBackground");
//  qDebug() << "activeBackground = " << c;
    connect(this, SIGNAL(changed(bool)), this, SLOT(updatePreview()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(subWindowActivated(QMdiSubWindow *)));
    connect(cbAnimateProgressBars, SIGNAL(toggled(bool)), this, SLOT(updateChanged()));
    connect(cbAllowScrollBarSliderToCoverArrows, SIGNAL(toggled(bool)), this, SLOT(updateChanged()));
    connect(cbHideShortcutUnderlines, SIGNAL(toggled(bool)), this, SLOT(updateChanged()));
#if 1
#if 1
    QMdiSubWindow *previewwindow2 = new QMdiSubWindow(mdiArea);
//  previewwindow2->setObjectName(QString::fromAscii("SkulpturePreviewWindow"));
//  previewwindow2->setProperty("sk_kwin_activeBackground",
    previewwindow2->setStyle(QStyleFactory::create(QString::fromUtf8("skulpture")));
    previewwindow2->setWidget(new Preview2Window(previewwindow2));
    previewwindow2->setGeometry(107, 7, 450, 245);
#endif
#if 1
    QMdiSubWindow *previewwindow1 = new QMdiSubWindow(mdiArea);
//  previewwindow1->setObjectName(QString::fromAscii("SkulpturePreviewWindow"));
    previewwindow1->setStyle(QStyleFactory::create(QString::fromUtf8("skulpture")));
    previewwindow1->setWidget(new Preview1Window(previewwindow1));
    previewwindow1->setGeometry(7, 35, 450, 245);
#endif
    updatePreview();
#else
    mdiArea->hide();
#endif
}


QSize SkulptureStyleConfig::sizeHint() const
{
    return QSize(570, 0);
}


SkulptureStyleConfig::~SkulptureStyleConfig()
{
    KGlobal::locale()->removeCatalog(QString::fromAscii("kstyle_skulpture_config"));
}


/*-----------------------------------------------------------------------*/

void SkulptureStyleConfig::loadSettings(QSettings &s)
{
    savedAnimateProgressBars = s.value(QString::fromAscii("ProgressBar/AnimateProgressBars"), true).toBool();
    savedAllowScrollBarSliderToCoverArrows = s.value(QString::fromAscii("ScrollBar/AllowScrollBarSliderToCoverArrows"), true).toBool();
    savedHideShortcutUnderlines = s.value(QString::fromAscii("General/HideShortcutUnderlines"), true).toBool();
    apply();
}


void SkulptureStyleConfig::saveSettings(QSettings &s)
{
    s.setValue(QString::fromAscii("ProgressBar/AnimateProgressBars"), cbAnimateProgressBars->isChecked());
    s.setValue(QString::fromAscii("ScrollBar/AllowScrollBarSliderToCoverArrows"), cbAllowScrollBarSliderToCoverArrows->isChecked());
    s.setValue(QString::fromAscii("General/HideShortcutUnderlines"), cbHideShortcutUnderlines->isChecked());
}


void SkulptureStyleConfig::load()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, QString::fromUtf8("SkulptureStyle"), QString::fromUtf8(""));
    loadSettings(s);
}


void SkulptureStyleConfig::save()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, QString::fromUtf8("SkulptureStyle"), QString::fromUtf8(""));
    saveSettings(s);
}


void SkulptureStyleConfig::defaults()
{
    cbAnimateProgressBars->setChecked(true);
    cbAllowScrollBarSliderToCoverArrows->setChecked(true);
    cbHideShortcutUnderlines->setChecked(true);
}


/*-----------------------------------------------------------------------*/

void SkulptureStyleConfig::apply()
{
    cbAnimateProgressBars->setChecked(savedAnimateProgressBars);
    cbAllowScrollBarSliderToCoverArrows->setChecked(savedAllowScrollBarSliderToCoverArrows);
    cbHideShortcutUnderlines->setChecked(savedHideShortcutUnderlines);
}


void SkulptureStyleConfig::updateChanged()
{
    if (cbAnimateProgressBars->isChecked() == savedAnimateProgressBars
        && cbAllowScrollBarSliderToCoverArrows->isChecked() == savedAllowScrollBarSliderToCoverArrows
        && cbHideShortcutUnderlines->isChecked() == savedHideShortcutUnderlines
       ) {
        emit changed(false);
    } else {
        emit changed(true);
    }
}


/*-----------------------------------------------------------------------*/

#include "sk_config.moc"


