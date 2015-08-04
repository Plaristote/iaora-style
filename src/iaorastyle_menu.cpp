/*
 * Copyright 2008 Gustavo Pichorim Boiko <boiko@mandriva.com>
 * Copyright 2008-2009 Rodrigo Gonçalves de Oliveira <rodrigo@mandriva.com>
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

void IaOraStyle::drawMenuItem(const QStyleOption *opt, QPainter *p, bool saveSpaceForIcon)
{
    const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt);

    if (!menuItem)
        return;

    ColorData *data = lookupData(opt->palette);
    QRect r = opt->rect;
    bool selected = menuItem->state & QStyle::State_Selected;
    bool checkable = menuItem->checkType != QStyleOptionMenuItem::NotCheckable;
    bool checked = menuItem->checked;
    bool enabled = menuItem->state & QStyle::State_Enabled;


    p->save();
    QBrush textBrush;
    if (opt->palette.resolve() & (1 << QPalette::ButtonText))
        textBrush = opt->palette.buttonText();
    else
        textBrush = opt->palette.windowText(); // KDE uses windowText rather than buttonText for menus

    switch (menuItem->menuItemType) {
    case QStyleOptionMenuItem::Separator: {
        drawSeparator(opt, p, 10);
        break;
    }
    case QStyleOptionMenuItem::TearOff: {
        drawHandle(opt, p);
        break;
    }

    case QStyleOptionMenuItem::Normal:
    case QStyleOptionMenuItem::DefaultItem:
    case QStyleOptionMenuItem::SubMenu: {
        if (selected && enabled) {
            QLinearGradient g(r.topLeft(), r.bottomLeft());
            g.setColorAt(0., data->color[1]);
            g.setColorAt(0.5, data->color[3]);
            g.setColorAt(0.51, data->color[4]);
            g.setColorAt(1., data->color[2]);
            fillRectCuttingOne(p, r.adjusted(0, 0, -1, 0), g);
        }

        /* from QPlastiqueStyle */
        //TODO: add CopyRight
        QRect checkRect(r.left() + 7, r.center().y() - 6, 13, 13);
        checkRect = QStyle::visualRect(menuItem->direction, menuItem->rect, checkRect);
        if (checkable) {
            if ((menuItem->checkType & QStyleOptionMenuItem::Exclusive) && menuItem->icon.isNull()) {
                QStyleOptionButton button;
                button.rect = checkRect;
                button.state = menuItem->state;
                if (checked)
                    button.state |= QStyle::State_On;
                button.palette = menuItem->palette;
                drawRadioButton(&button, p);
            } else {
                QStyleOptionButton button;
                button.rect = checkRect;
                button.state = menuItem->state;
                if (checked)
                    button.state |= QStyle::State_On;
                button.palette = menuItem->palette;

                if (menuItem->icon.isNull())
                    drawCheckBox(&button, p);
                else if (checked) {
                    int size = r.height() - 4;
                    QRect buttonRect(0, 0, size, size);
                    buttonRect.moveCenter(checkRect.center());
                    button.rect = buttonRect;
                    drawButton(&button, p);
                }
            }
        }

        if (!menuItem->icon.isNull() && saveSpaceForIcon) {
            QIcon::Mode mode = !enabled ? QIcon::Disabled : QIcon::Normal;
            if (selected && enabled)
                mode = QIcon::Active;
            QPixmap pixmap;
            if (checked)
                pixmap = menuItem->icon.pixmap(m_parent->pixelMetric(QStyle::PM_SmallIconSize, opt), mode, QIcon::On);
            else
                pixmap = menuItem->icon.pixmap(m_parent->pixelMetric(QStyle::PM_SmallIconSize, opt), mode);

            QRect pmr = pixmap.rect();
            pmr.moveCenter(checkRect.center());
            p->setPen(textBrush.color());
            p->drawPixmap(pmr.topLeft(), pixmap);
        }

        // Text
        bool leftToRight = (menuItem->direction == Qt::LeftToRight) ? true : false;
        int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine | Qt::AlignLeft;

        if (!m_parent->styleHint(QStyle::SH_UnderlineShortcut, menuItem))
            text_flags |= Qt::TextHideMnemonic;

        QRect subMenuRect(r.right() - 10, r.top(), 10, r.height());
        if (!menuItem->text.isEmpty()) {
            QRect textRect;
            int tab = menuItem->tabWidth;
            if (saveSpaceForIcon && leftToRight)
                textRect.setLeft(checkRect.right() + 8);
            else if (!leftToRight)
                textRect.setLeft(tab);
            else
                textRect.setLeft(r.left() + 4);

            textRect.setTop(r.top() + 4);

            if (leftToRight) textRect.setRight(r.right() - 4 - tab);
            else {
                if (saveSpaceForIcon) textRect.setRight(checkRect.left() - 8);
                else textRect.setRight(r.right() - 4);
            }

            textRect.setBottom(r.bottom() - 4);

            if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu)
                textRect.adjust(0, 0, -subMenuRect.width(), 0);
            else if (menuItem->menuItemType == QStyleOptionMenuItem::DefaultItem) {
                QFont f = p->font();
                f.setBold(true);
                p->setFont(f);
            }

            QStringList textList = menuItem->text.split(QRegExp("\t"));

            // draw the shortcut
            if (textList.first() != textList.last()) {
                QRect shortcutRect(textRect);

                if (leftToRight)
                    shortcutRect.setLeft(textRect.right());
                else
                    shortcutRect.setRight(textRect.left() + 4);

                drawItemText(p, shortcutRect, text_flags, menuItem->palette, enabled, textList.last(), selected ? QPalette::HighlightedText : QPalette::Text);
            }

            // and the text
            drawItemText(p, textRect, text_flags, menuItem->palette, enabled, textList.first(), selected ? QPalette::HighlightedText : QPalette::Text);
        }

        if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu) {
            QStyleOption copy = *opt;
            copy.rect = subMenuRect;

            if (leftToRight)
                drawSymbol(&copy, p, SymbolArrowRight);
            else {
                copy.rect = QStyle::visualRect(menuItem->direction, menuItem->rect, copy.rect);
                drawSymbol(&copy, p, SymbolArrowLeft);
            }
        }
        break;
    }
    case QStyleOptionMenuItem::Scroller:
    default:
        break;
    }

    p->restore();
}

