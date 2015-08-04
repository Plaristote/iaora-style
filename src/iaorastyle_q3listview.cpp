/*
 * Copyright 2008 Gustavo Pichorim Boiko <boiko@mandriva.com>
 * Copyright 2008-2009 Rodrigo Gonçalves de Oliveira <rodrigo@mandriva.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your opt) any later version.
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
 ** Q3ListView code based on KStyle for KDE4
 *   Copyright (C) 2004-2005 Maksim Orlovich <maksim@kde.org>
 *   Copyright (C) 2005,2006 Sandro Giessl <giessl@kde.org>
 *
 *   Based in part on the following software:
 *   KStyle for KDE3
 *        Copyright (C) 2001-2002 Karol Szwed <gallium@kde.org>
 *        Portions  (C) 1998-2000 TrollTech AS
 *   Keramik for KDE3,
 *        Copyright (C) 2002      Malte Starostik   <malte@kde.org>
 *                  (C) 2002-2003 Maksim Orlovich  <maksim@kde.org>
 *        Portions  (C) 2001-2002 Karol Szwed     <gallium@kde.org>
 *                  (C) 2001-2002 Fredrik Höglund <fredrik@kde.org>
 *                  (C) 2000 Daniel M. Duley       <mosfet@kde.org>
 *                  (C) 2000 Dirk Mueller         <mueller@kde.org>
 *                  (C) 2001 Martijn Klingens    <klingens@kde.org>
 *                  (C) 2003 Sandro Giessl      <sandro@giessl.com>
 *
 */

#include "iaorastyle.h"
#include <QPainter>
#include <QStyle>

void IaOraStyle::drawQ3ListView(const QStyleOption *opt, QPainter *p, const QWidget *w)
{
    QRect r = opt->rect;
    // This code is based on the implementation of KStyle for KDE4;
    // check the copyright on the top of this file
    const QStyleOptionQ3ListView* lvOpt = qstyleoption_cast<const QStyleOptionQ3ListView*>(opt);

    if (!lvOpt)
        return;

    // If we have a branch or are expanded...
    if (lvOpt->subControls & (QStyle::SC_Q3ListViewBranch | QStyle::SC_Q3ListViewExpand)) {
        QStyleOptionQ3ListViewItem item  = lvOpt->items.at(0);

        int y = r.y();

        QStyleOption opt; //For painting
        opt.palette   = lvOpt->palette;
        opt.direction = Qt::LeftToRight;

        //Remap the p so (0,0) corresponds to the origin
        //of the w, to help out the line align code.
        //Extract the paint offset. Here be dragons
        //(and not the cute green Patron of the project, either)
        int cX = w ? w->property("contentsX").toInt() : 0;
        int cY = w ? w->property("contentsY").toInt() : 0;

        QPoint adjustCoords = p->matrix().map(QPoint(0, 0)) + QPoint(cX, cY);
        p->translate(-adjustCoords);

        if (lvOpt->activeSubControls & QStyle::SC_All && (lvOpt->subControls & QStyle::SC_Q3ListViewExpand)) {
            //### CHECKME: this is from KStyle3, and needs to be re-checked/tested
            // We only need to draw a vertical line
            //Route through the Qt4 style-call.
            QStyleOption opt;
            opt.rect  = QRect(r.topLeft() + adjustCoords, r.size());
            opt.state = QStyle::State_Sibling;
            drawBranch(&opt, p);
        } else {
            int childPos = 1;

            // Draw all the expand/close boxes, and nearby branches
            while (childPos < lvOpt->items.size() && y < r.height()) {
                const QStyleOptionQ3ListViewItem& child = lvOpt->items.at(childPos);

                if (!(child.features & QStyleOptionQ3ListViewItem::Visible)) {
                    childPos++;
                    continue;
                }

                //Route through the Qt4 style-call.
                opt.rect  = QRect(r.x() + adjustCoords.x(), y + adjustCoords.y(),
                                  r.width(), child.height);
                opt.state = QStyle::State_Item;

                if (child.features & QStyleOptionQ3ListViewItem::Expandable
                    || child.childCount) {
                    opt.state |= QStyle::State_Children;
                    opt.state |= (child.state & QStyle::State_Open);
                    opt.state |= (child.state & QStyle::State_Enabled);
                }

                //See if we have a visible sibling
                int siblingPos = 0;
                for (siblingPos = childPos + 1; siblingPos < lvOpt->items.size();
                     ++siblingPos) {
                    if (lvOpt->items.at(siblingPos).features
                        & QStyleOptionQ3ListViewItem::Visible) {
                        opt.state |= QStyle::State_Sibling;
                        break;
                    }
                }

                //If on screen, paint it
                if (y + child.height > 0)
                    drawBranch(&opt, p);

                if (!siblingPos)
                    break;

                //If we have a sibling, and an expander, also have to draw
                //a line for below the immediate area
                if ((opt.state & QStyle::State_Children) && (opt.state & QStyle::State_Sibling)) {
                    opt.state = QStyle::State_Sibling;
                    opt.rect  = QRect(r.x() + adjustCoords.x(),
                                      y + adjustCoords.y() + child.height,
                                      r.width(), child.totalHeight - child.height);
                    if (opt.rect.height())
                        drawBranch(&opt, p);
                }

                y += child.totalHeight;
                childPos = siblingPos;
            } //loop through items
        } //complex case
    }
}

