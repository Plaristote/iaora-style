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

void IaOraStyle::drawButton(const QStyleOption *opt, QPainter *p, bool simplified, bool vertical)
{
    bool enabled = (opt->state & QStyle::State_Enabled);

    QPalette pal = opt->palette;
    if (!enabled)
        pal.setCurrentColorGroup(QPalette::Active); // when the button is disabled, use the same palette as if enabled

    ColorData *data = lookupData(pal);

    // TODO: cache the gradients so that it is more efficient
    bool sunken = (opt->state & QStyle::State_Sunken);
    bool on = (opt->state & QStyle::State_On);
    QRect r = opt->rect;
    int top = r.top(), left = r.left(), bottom = r.bottom(), right = r.right();

    QPen oldPen = p->pen();

    QLinearGradient g;
    if (vertical) {
        g.setStart(left, 0);
        g.setFinalStop(right, 0);
    } else {
        g.setStart(0, top);
        g.setFinalStop(0, bottom);
    }

    if (sunken || on) {
        g.setColorAt(0., data->gray[2]);
        g.setColorAt(1., data->gray[1]);
    } else {
        g.setColorAt(0., data->gray[0]);
        g.setColorAt(1., data->gray[2]);
    }

    QRect contents = r;
    if (!simplified)
        contents.adjust(2, 2, -2, -2);
    p->fillRect(contents, g);

    if (!simplified) {
        //draw the lines around
        if (!sunken && !on)
            p->setPen(data->white);
        else
            p->setPen(enabled ? data->gray[1] : data->gray[2]);
        p->drawLine(left + 2, top + 1, right - 2, top + 1);
        p->drawLine(left + 1, top + 2, left + 1, bottom - 2);

        if (enabled)
            p->setPen(data->gray[1]);
        else
            p->setPen((sunken || on) ? data->white : data->gray[1]);
        p->drawLine(left + 2, bottom - 1, right - 2, bottom - 1);
        p->drawLine(right - 1, top + 2, right - 1, bottom - 2);

        // the outter line
        if (enabled)
            p->setPen(data->gray[5]);
        else
            p->setPen(data->gray[4]);
        p->drawLine(left + 2, top, right - 2, top); // top
        p->drawLine(left, top + 2, left, bottom - 2); // left
        p->setPen(data->gray[4]);
        p->drawLine(left + 2, bottom, right - 2, bottom); // bottom
        p->drawLine(right, top + 2, right, bottom - 2); // right

        // and finally the four points
        QPoint points[4];
        points[0] = QPoint(left + 1, top + 1);
        points[1] = QPoint(right - 1, top + 1);
        points[2] = QPoint(right - 1, bottom - 1);
        points[3] = QPoint(left + 1, bottom - 1);
        p->drawPoints(points, 4);

        // if selected, draw the selection frame
        if (opt->state & QStyle::State_HasFocus) {
            QStyleOption copy = *opt;
            copy.rect = r.adjusted(2, 2, -2, -2);
            drawFrame(&copy, p, FrameFocus);
        }
    }

    p->setPen(oldPen);
}

void IaOraStyle::drawButtonDefaultBorder(const QStyleOption *opt, QPainter *p)
{
    ColorData *data = lookupData(opt->palette);
    QRect r = opt->rect;
    int top = r.top(), left = r.left(), bottom = r.bottom(), right = r.right();

    QPen oldPen = p->pen();

    p->setPen(data->color[4]);
    p->drawLine(left + 3, top, right - 3, top); //top
    p->drawLine(left + 3, bottom, right - 3, bottom); //bottom
    p->drawLine(left, top + 3, left, bottom - 3); //left
    p->drawLine(right, top + 3, right, bottom - 3); //right

    //the points
    QPoint points[8];
    points[0] = QPoint(left + 1, top + 2);
    points[1] = QPoint(left + 2, top + 1);
    points[2] = QPoint(right - 1, top + 2);
    points[3] = QPoint(right - 2, top + 1);
    points[4] = QPoint(left + 1, bottom - 2);
    points[5] = QPoint(left + 2, bottom - 1);
    points[6] = QPoint(right - 1, bottom - 2);
    points[7] = QPoint(right - 2, bottom - 1);
    p->drawPoints(points, 8);

    p->setPen(oldPen);
}

