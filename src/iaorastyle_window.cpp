/*
 * Copyright 2008 Gustavo Pichorim Boiko <boiko@mandriva.com>
 * Copyright 2008-2010 Rodrigo Gonçalves de Oliveira <rodrigo@mandriva.com>
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

#include "iaorastyle.h"

void IaOraStyle::drawDockWindowTitle(const QStyleOption *opt, QPainter *p)
{
    const QStyleOptionDockWidget *dock = qstyleoption_cast<const QStyleOptionDockWidget *>(opt);

    if (!dock)
        return;

    QPalette pal = opt->palette;
    pal.setCurrentColorGroup(QPalette::Active);
    ColorData *data = lookupData(pal);

    p->save();

    QLinearGradient g(opt->rect.topLeft(), opt->rect.bottomLeft());
    g.setColorAt(0., data->color[1]);
    g.setColorAt(0.4, data->color[3]);
    g.setColorAt(0.41, data->color[4]);
    g.setColorAt(1., data->color[2]);
    p->fillRect(opt->rect, g);

    int text_flags = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;

    QFont font = p->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() / 1.19);
    p->setFont(font);
    p->setPen(data->white);

    int icons_width = 30; // for the close and float buttons

    QString text = dock->fontMetrics.elidedText(dock->title, Qt::ElideRight, dock->rect.width() - icons_width);
    p->drawText(dock->rect.adjusted(-icons_width, 0, 0, 0), text_flags, text);
    //p->drawText(dock->rect, text_flags, dock->title);

    p->restore();
}

void IaOraStyle::drawTitleBarButton(const QStyleOptionComplex *opt, QPainter *p, const QWidget *w, QStyle::SubControl control, QStyle::StandardPixmap icon)
{
    QRect buttonRect = m_parent->subControlRect(QStyle::CC_TitleBar, opt, control);
    QPixmap pm = m_parent->standardIcon(icon, opt, w).pixmap(14, 14);

    QRect pixRect = pm.rect();
    pixRect.moveCenter(buttonRect.center());
    p->drawPixmap(pixRect, pm);
}

void IaOraStyle::drawTitleBar(const QStyleOptionComplex *opt, QPainter *p, const QWidget *w)
{
    const QStyleOptionTitleBar *title = qstyleoption_cast<const QStyleOptionTitleBar *>(opt);
    if (!title)
        return;

    //QPalette pal = opt->palette;
    //pal.setCurrentColorGroup(QPalette::Active);
    //ColorData *data = lookupData(pal);
    ColorData *data = lookupData(opt->palette); // REGRESSION

    p->save();

    // dunno why the right border needs to be adjusted, but anyway...
    QRect contents = opt->rect.adjusted(0, 0, -1, 0);
    p->setPen(data->color[2]);
    p->setBrush(Qt::NoBrush);
    p->drawRect(contents);

    contents.adjust(1, 1, 0, 0);
    QLinearGradient g(opt->rect.topLeft(), opt->rect.bottomLeft());
    g.setColorAt(0., data->color[1]);
    g.setColorAt(0.5, data->color[3]);
    g.setColorAt(0.51, data->color[4]);
    g.setColorAt(1., data->color[2]);

    p->fillRect(contents, g);

    // do we have a label?
    if (title->subControls & QStyle::SC_TitleBarLabel) {
        QRect labelRect = m_parent->subControlRect(QStyle::CC_TitleBar, opt, QStyle::SC_TitleBarLabel);
        int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine | Qt::AlignLeft;
        QFont font = p->font();
        font.setBold(true);
        font.setPointSizeF(font.pointSizeF() / 1.19);
        p->setFont(font);
        p->setPen(data->white);
        p->drawText(labelRect, text_flags, title->text);
    }

    // the buttons
    if (title->subControls & QStyle::SC_TitleBarMaxButton
        && (title->titleBarFlags & Qt::WindowMaximizeButtonHint)
        && !(title->titleBarState & Qt::WindowMaximized))
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarMaxButton, QStyle::SP_TitleBarMaxButton);
    if (title->subControls & QStyle::SC_TitleBarMinButton
        && (title->titleBarFlags & Qt::WindowMinimizeButtonHint)
        && !(title->titleBarState & Qt::WindowMinimized))
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarMinButton, QStyle::SP_TitleBarMinButton);
    if (title->subControls & QStyle::SC_TitleBarNormalButton &&
        (((title->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
          (title->titleBarState & Qt::WindowMinimized)) ||
         ((title->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
          (title->titleBarState & Qt::WindowMaximized))))
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarNormalButton, QStyle::SP_TitleBarNormalButton);
    if (title->subControls & QStyle::SC_TitleBarShadeButton
        && title->titleBarFlags & Qt::WindowShadeButtonHint)
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarShadeButton, QStyle::SP_TitleBarShadeButton);
    if (title->subControls & QStyle::SC_TitleBarSysMenu)
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarSysMenu, QStyle::SP_TitleBarMenuButton);
    if (title->subControls & QStyle::SC_TitleBarUnshadeButton
        && title->titleBarFlags & Qt::WindowShadeButtonHint)
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarUnshadeButton, QStyle::SP_TitleBarUnshadeButton);
    if (title->subControls & QStyle::SC_TitleBarCloseButton)
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarCloseButton, QStyle::SP_TitleBarCloseButton);
    if (title->subControls & QStyle::SC_TitleBarContextHelpButton
        && (title->titleBarFlags & Qt::WindowContextHelpButtonHint))
        drawTitleBarButton(opt, p, w, QStyle::SC_TitleBarContextHelpButton, QStyle::SP_TitleBarContextHelpButton);

    p->restore();
}

