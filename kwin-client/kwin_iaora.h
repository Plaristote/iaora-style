/*
 * kwin_iaora.h
 *
 * Copyright 2008 Gustavo Pichorim Boiko <boiko@mandriva.com>
 * Skulpture Copyright 2008 Christoph Feck
 * Copyright 2008-2010 Rodrigo Gonçalves de Oliveira <rodrigo@mandriva.com>
 *
 ** Window tabbing feature based on the original code from Tabstrip KWin window decoration
 ** Copyright (C) 2009 Jorge Mata <matamax123@gmail.com>
 ** Copyright (C) 2009 Lucas Murray <lmurray@undefinedfire.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KWIN_IAORA_H
#define KWIN_IAORA_H 1

#include <QColor>

typedef struct {
    QColor gray[7];
    QColor color[5];
    QColor black;
    QColor white;
} ColorData;

/*-----------------------------------------------------------------------*/

#include <kcommondecoration.h>
class QStyleOptionButton;

class IaOraDecorationButton : public KCommonDecorationButton
{
    Q_OBJECT

public:
    IaOraDecorationButton(ButtonType type, KCommonDecoration *parent);
    virtual ~IaOraDecorationButton();
    virtual void reset(unsigned long changed);

protected:
    virtual void paintEvent(QPaintEvent *event);

protected:
    void initStyleOption(QStyleOptionButton &opt);

private:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void init();
    bool m_isHover;
};


/*-----------------------------------------------------------------------*/

class QStyleOption;

//class IaOraDecoration : public KCommonDecorationUnstable // TABS
class IaOraDecoration : public KCommonDecoration
{
    Q_OBJECT

public:
    IaOraDecoration(KDecorationBridge *bridge, KDecorationFactory *factory);
    virtual ~IaOraDecoration();

public:
    virtual QString visibleName() const;
    virtual bool decorationBehaviour(DecorationBehaviour behaviour) const;
    virtual int layoutMetric(LayoutMetric lm, bool respectWindowState = true, const KCommonDecorationButton *button = 0) const;
    virtual void updateWindowShape();

    virtual void init();
    virtual KCommonDecorationButton *createButton(ButtonType type);

    virtual void paintEvent(QPaintEvent *event);

    // tabbing related
    //virtual bool eventFilter(QObject *object, QEvent *event);
    //int itemClicked(const QPoint &point, bool between = false);
    //bool mouseButtonPressEvent(QMouseEvent *event);
    //bool mouseButtonReleaseEvent(QMouseEvent *event);
    //bool mouseMoveEvent(QMouseEvent *event);
    //bool dragEnterEvent(QDragEnterEvent *event);
    //bool dragMoveEvent(QDragMoveEvent *event);
    //bool dragLeaveEvent(QDragLeaveEvent *event);
    //bool dropEvent(QDropEvent *event);
    //

protected:
    void initStyleOption(QStyleOption &opt);

private:
    static ColorData *lookupData(const QPalette &pal);

    // load the configuration
    void loadConfig();

    // draw a tab for some application
    //void drawTab(QPainter &painter, const QRect &rect, ClientGroupItem &item, bool border, bool active, bool dragging = false); // TABS

    // use tabbing feature?
    //bool useWindowGrouping; // TABS
    //bool closeButtonsOnTabs; // TABS

    // what kind of border we're using
    int borderSize;
    // just the titlebar text alignment
    int titleBarTextAlign;

    /* cached style hints and pixel metrics */
    bool coloredFrame;
    bool contrastFrame;
    bool noBorder;
    bool autoRaise;
    int titleHeight;
    int borderWidth;

    // tabbing related
    QPoint clickPos, releasePos;
    Qt::MouseButton pressedButton;
    QList<IaOraDecorationButton*> closeButtonsList;
    bool clicking, dragging;
    int targetTab;
    //
};


/*-----------------------------------------------------------------------*/

#include <kdecorationfactory.h>

//class IaOraDecorationFactory : public KDecorationFactoryUnstable // TABS
class IaOraDecorationFactory : public KDecorationFactory
{
public:
    IaOraDecorationFactory();
    virtual ~IaOraDecorationFactory();

public:
    bool loadConfig();
    virtual KDecoration *createDecoration(KDecorationBridge *bridge);
    virtual bool reset(unsigned long changed);
    //virtual QList<BorderSize> borderSizes() const;
    virtual bool supports(Ability ability) const;

private:
    int borderSize;
    int alignFlag;
    //bool useWindowGrouping; // TABS
    //bool closeButtonsOnTabs; // TABS

};


/*-----------------------------------------------------------------------*/

#endif


