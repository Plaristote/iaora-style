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

void IaOraStyle::drawScrollBarEmptySpace(const QStyleOption *opt, QPainter *p)
{
    const QStyleOptionSlider *scr = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (!scr)
        return;

    ColorData *data = lookupData(opt->palette);
    QRect r = opt->rect;
    p->fillRect(r, data->gray[0]);
    int border;
    if (scr->orientation == Qt::Horizontal)
        border = (BorderTop | BorderBottom);
    else
        border = (BorderLeft | BorderRight);
    drawBorder(p, opt, r, (opt->state & QStyle::State_Enabled), border);
}

void IaOraStyle::drawScrollBarButton(const QStyleOption *opt, QPainter *p, Symbol s)
{
    const QStyleOptionSlider *scr = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (!scr)
        return;

    ColorData *data = lookupData(opt->palette);

    bool vertical = (scr->orientation == Qt::Vertical);
    bool active = (opt->state & QStyle::State_Enabled);
    QRect r = opt->rect;
    QStyleOption copy = *opt;

    p->save();

    /* FIXME: figure out why the scrollbar area is being filled as white at the very beginning */
    p->fillRect(r, opt->palette.brush(QPalette::Window));

    QRect arrowRect = r;

    drawFrameCuttingOne(p, r, active ? data->gray[5] : data->gray[4]);
    if (vertical) {
        arrowRect.adjust(1, 2, 0, 0);
        drawBorder(p, opt, r, active, (s == SymbolArrowRight) ? BorderTop : BorderBottom);
    } else {
        arrowRect.adjust(2, 1, 0, 0);
        drawBorder(p, opt, r, active, (s == SymbolArrowRight) ? BorderLeft : BorderRight);
    }

    copy.rect = r.adjusted(1, 1, -1, -1);
    drawButton(&copy, p, true, true);

    // only set the right arrows in vertical mode
    if (vertical)
        s = (s == SymbolArrowRight) ? SymbolArrowDown : SymbolArrowUp;
    else if (scr->direction == Qt::RightToLeft) // if direction != Qt::LeftToRight, we change the symbol orientation
        s = (s == SymbolArrowRight) ? SymbolArrowLeft : SymbolArrowRight;
    copy.rect = arrowRect;
    drawSymbol(&copy, p, s);

    p->restore();
}

void IaOraStyle::drawScrollBarHandle(const QStyleOption *opt, QPainter *p)
{
    const QStyleOptionSlider *scr = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (!scr)
        return;

    QPalette pal = opt->palette;
    pal.setCurrentColorGroup(QPalette::Active); // let the scrollbars colored when no focus
    ColorData *data = lookupData(pal);

    QPen oldPen = p->pen();

    bool vertical = (scr->orientation == Qt::Vertical);
    bool max = (scr->sliderValue == scr->maximum), min = (scr->sliderValue == scr->minimum);
    QRect r = opt->rect;
    QLine maxLine, minLine;

    // fill the empty spaces and the border
    drawScrollBarEmptySpace(opt, p);


    QRect handleRect = r;

    QLinearGradient g;
    if (vertical) {
        handleRect.adjust(1, min ? 0 : 1, -1, max ? 0 : -1);
        g = QLinearGradient(handleRect.left(), 0, handleRect.right(), 0);
        minLine = QLine(handleRect.left() + 1, r.top(), handleRect.right() - 1, r.top());
        maxLine = QLine(handleRect.left() + 1, r.bottom(), handleRect.right() - 1, r.bottom());
    } else {
        handleRect.adjust(min ? 0 : 1, 1, max ? 0 : -1, -1);
        g = QLinearGradient(0, handleRect.top(), 0, handleRect.bottom());
        minLine = QLine(r.left(), handleRect.top() + 1, r.left(), handleRect.bottom() - 1);
        maxLine = QLine(r.right(), handleRect.top() + 1, r.right(), handleRect.bottom() - 1);
    }
    g.setColorAt(0., data->color[1]);
    g.setColorAt(0.5, data->color[3]);
    g.setColorAt(0.51, data->color[4]);
    g.setColorAt(1., data->color[2]);

    p->fillRect(handleRect, g);

    p->setPen(QPen(QBrush(g), 1));
    if (!min)
        p->drawLine(minLine);
    if (!max)
        p->drawLine(maxLine);

    p->setPen(oldPen);
}

