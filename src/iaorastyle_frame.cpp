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

void IaOraStyle::drawFrame(const QStyleOption *option, QPainter *p, FrameType type, QRect optRect)
//void IaOraStyle::drawFrame(const QStyleOption *option, QPainter *p, FrameType type)
{
    ColorData *data = lookupData(option->palette);

    bool enabled = (option->state & QStyle::State_Enabled);

    QRect r;
    if (optRect.isValid())
        r = optRect;
    else
        r = option->rect;
    //QRect r = option->rect;

    int top = r.top(), left = r.left(), bottom = r.bottom(), right = r.right();
    QPen oldPen = p->pen();
    QBrush oldBrush = p->brush();

    switch (type) {
    case FrameFocus: {
        QPen pen = p->pen();
        pen.setStyle(Qt::DotLine);
        pen.setColor(option->palette.color(QPalette::ButtonText));
        p->setPen(pen);
        p->setBrush(Qt::NoBrush);
        p->drawRect(option->rect.adjusted(1, 1, -1, -1));
        break;
    }
    case FrameToolBar:
    case FrameMenuBar: {
        // fill the rect using the specified graytone
        QColor c;
        if (type == FrameToolBar)
            c = data->gray[1];
        else if (type == FrameMenuBar)
            c = data->gray[0];
        else
            c = data->gray[0];

        p->fillRect(r, c);

        // now the lines around
        p->setPen(data->gray[2]);

        // we assume that the top was already drawn, or that the toolbar is the topmost
        // widget from the window, and in this case it does not need the top line
        p->drawLine(left, bottom - 1, right, bottom - 1);
        p->setPen(data->white);
        p->drawLine(left, bottom, right, bottom);
        break;
    }
    case FrameProgressBar: {
        // fill the internal rect
        QRect contents = r.adjusted(1, 1, -1, -1);
        p->fillRect(contents, data->gray[0]);

        // draw the round frame
        drawFrameCuttingOne(p, r, data->gray[5]);
        break;
    }
    case FrameLineEdit: {
        const QStyleOptionFrame *panel = qstyleoption_cast<const QStyleOptionFrame *>(option);
        QRect contents = r;
        contents.adjust(1, 1, -1, -1);

        if (panel && panel->lineWidth > 0) {
            drawFrameCuttingOne(p, r, data->gray[4]);
        }

        p->fillRect(contents, (option->state & QStyle::State_Enabled) ?  panel->palette.base().color() : data->gray[0]);

        if (option->state & QStyle::State_HasFocus) {
            p->setPen(data->color[3]);
            p->setBrush(Qt::NoBrush);
            p->drawRect(contents);
        }
        break;
    }
    case FrameComboBox: {
        p->save();

        QRect tabRect = r.adjusted(0, 0, -1, -1);
        p->setPen(data->gray[2]);
        p->setBrush(data->gray[0]);

        p->drawLine(left, top + 1, left, bottom - 1);
        p->drawLine(right, top + 1, right, bottom - 1);
        p->drawLine(left + 1, top, right - 1, top);
        p->drawLine(left + 1, bottom, right - 1, bottom);

        p->restore();
        break;
    }
    case FrameHeader: {
        const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option);

        // draw the button appearance
        drawButton(option, p, true);

        if (!header)
            break;

        bool vertical = (header->orientation == Qt::Vertical);
        QRect r = option->rect;

        QLine outterLine, innerLine, previousLine, nextLine;
        if (vertical) {
            outterLine = QLine(r.topLeft(), r.bottomLeft());
            innerLine = QLine(r.topRight(), r.bottomRight());
            previousLine = QLine(r.topLeft(), r.topRight());
            nextLine = QLine(r.bottomLeft(), r.bottomRight());
        } else {
            outterLine = QLine(r.topLeft(), r.topRight());
            innerLine = QLine(r.bottomLeft(), r.bottomRight());
            previousLine = QLine(r.topLeft(), r.bottomLeft());
            nextLine = QLine(r.topRight(), r.bottomRight());
        }

        p->setPen(data->gray[5]);
        p->drawLine(innerLine);

        if (header->position != QStyleOptionHeader::Beginning) {
            p->setPen(Qt::white);
            p->drawLine(previousLine);
        }
        p->setPen(data->gray[2]);
        p->drawLine(nextLine);

        break;
    }
    case FrameMenu: {
        QRect contents = r.adjusted(1, 1, -1, -1);
        p->fillRect(contents, data->gray[0]);

        drawFrameCuttingOne(p, r, enabled ? data->gray[5] : data->gray[4]);
        break;
    }
    case FrameTabBar: {
        p->save();

        QRect tabRect = r.adjusted(0, 0, -1, -1);
        p->setPen(data->gray[4]);
        p->setBrush(data->gray[0]);
        p->drawRect(tabRect);

        p->restore();
        break;
    }
    case FrameGroupBox:
    case FrameFrame: {
        p->save();

        const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option);

        if (!frame)
            return;

        bool flat = (frame->features & QStyleOptionFrame::Flat);
        bool sunken = (frame->state & QStyle::State_Sunken);
        bool raised = (frame->state & QStyle::State_Raised);

        if (flat && type == FrameGroupBox) {
            QRect rect(left, top, r.width(), 4);
            QStyleOption option;
            option.state &= ~QStyle::State_Horizontal;
            option.rect = rect;
            drawSeparator(&option, p, 0);
        } else {
            if (sunken || raised) {
                drawFrameCuttingOne(p, r, data->gray[2]);

                QLine lines[2];
                lines[0] = QLine(left + 1, top + 1, right - 1, top + 1);
                lines[1] = QLine(left + 1, top + 1, left + 1, bottom - 2);
                p->setPen(sunken ? data->gray[1] : data->white);
                p->drawLines(lines, 2);

                lines[0] = QLine(right - 1, top + 1, right - 1, bottom - 1);
                lines[1] = QLine(left + 1, bottom - 1, right - 1, bottom - 1);
                p->setPen(sunken ? data->white : data->gray[1]);
                p->drawLines(lines, 2);
            } else {
                QLine lines[4];
                // the white lines
                lines[0] = QLine(left + 1, top + 1, right, top + 1); // top
                lines[1] = QLine(right, top + 1, right, bottom - 1); // right
                lines[2] = QLine(left + 1, bottom, right - 1, bottom); // bottom
                lines[3] = QLine(left + 1, top + 1, left + 1, bottom); // left
                p->setPen(data->white);
                p->drawLines(lines, 4);

                lines[0] = QLine(left + 1, top, right - 1, top); // top
                lines[1] = QLine(right - 1, top + 1, right - 1, bottom - 1); // right
                lines[2] = QLine(left, bottom - 1, right - 1, bottom - 1); // bottom
                lines[3] = QLine(left, top + 1, left, bottom - 1); // left
                p->setPen(data->gray[3]);
                p->drawLines(lines, 4);
            }
        }
        p->restore();
        break;
    }
    case FrameGeneric:
    default:
        drawFrameCuttingOne(p, r, enabled ? data->gray[5] : data->gray[4]);
    }

    p->setPen(oldPen);
    p->setBrush(oldBrush);
}

void IaOraStyle::drawBorder(QPainter *painter, const QStyleOption *opt, const QRect &r, bool active, int border)
{
    QPen oldPen = painter->pen();
    ColorData *data = lookupData(opt->palette);

    int top = r.top(), left = r.left(), bottom = r.bottom(), right = r.right();
    painter->setPen(active ? data->gray[5] : data->gray[4]);
    if (border & BorderTop)
        painter->drawLine(left, top, right, top);
    if (border & BorderLeft)
        painter->drawLine(left, top, left, bottom);
    if (border & BorderBottom)
        painter->drawLine(left, bottom, right, bottom);
    if (border & BorderRight)
        painter->drawLine(right, top, right, bottom);

    painter->setPen(oldPen);
}

void IaOraStyle::drawSeparator(const QStyleOption *opt, QPainter *p, int margin)
{
    ColorData *data = lookupData(opt->palette);
    bool toolBarHorizontal = (opt->state & QStyle::State_Horizontal);
    QRect r = opt->rect;
    QLine line1, line2;

    QPen oldPen = p->pen();
    QBrush oldBrush = p->brush();

    if (toolBarHorizontal) {
        int middle = (r.left() + r.right()) / 2;
        line1 = QLine(middle, r.top(), middle, r.bottom());
        if (r.height() >= 20) {
            line1.setP1(QPoint(line1.x1(), line1.y1() + margin));
            line1.setP2(QPoint(line1.x2(), line1.y2() - margin));
        }
        line2 = line1.translated(1, 0);
    } else {
        int middle = (r.top() + r.bottom()) / 2;
        line1 = QLine(r.left(), middle, r.right(), middle);
        // remove some points from the edges
        if (r.width() >= 20) {
            line1.setP1(QPoint(line1.x1() + margin, line1.y1()));
            line1.setP2(QPoint(line1.x2() - margin, line1.y1()));
        }
        line2 = line1.translated(0, 1);
    }

    p->setPen(data->gray[2]);
    p->drawLine(line1);
    p->setPen(data->white);
    p->drawLine(line2);

    p->setPen(oldPen);
}

void IaOraStyle::drawHandle(const QStyleOption *opt, QPainter *p)
{
    QPalette pal = opt->palette;
    pal.setCurrentColorGroup(QPalette::Active);
    ColorData *data = lookupData(pal);

    bool toolBarHorizontal = (opt->state & QStyle::State_Horizontal);
    QRect r = opt->rect;
    QList<QPoint> dots;

    QPen oldPen = p->pen();

    // assume the space between the dots is of 4 pixels (TODO: check if it is not more)
    // each dot has height of 4 (*3 = 12)
    // the space is assumed to be 4 (*2 = 8)
    // --> the total height is then 20
    if (toolBarHorizontal) {
        if (r.height() > 30) {
            int start = (r.top() + r.bottom()) / 2 - 10; // 10 is half the size of the dot block
            int middle = (r.left() + r.right()) / 2 - 2;

            for (int i = 0; i < 3; ++i) {
                dots.append(QPoint(middle, start));
                start += 8; // 4 from the size and 4 from the spacing
            }
        } else // only a single dot in the middle
            dots.append(QPoint((r.left() + r.right()) / 2 - 2, (r.top() + r.bottom()) / 2 - 2));
    } else {
        if (r.width() > 30) {
            int start = (r.left() + r.right()) / 2 - 10; // 10 is half the size of the dot block
            int middle = (r.top() + r.bottom()) / 2 - 2;

            for (int i = 0; i < 3; ++i) {
                dots.append(QPoint(start, middle));
                start += 8; // 4 from the size and 4 from the spacing
            }
        } else // only a single dot in the middle
            dots.append(QPoint((r.left() + r.right()) / 2 - 2, (r.top() + r.bottom()) / 2 - 2));
    }

    // draw all dots
    foreach(QPoint dot, dots) {
        int x = dot.x(), y = dot.y();

        // draw the first color
        p->setPen(data->color[4]);
        p->drawLine(x + 1, y, x + 2, y);
        p->drawLine(x, y + 1, x + 3, y + 1);

        // the second color
        p->setPen(data->color[2]);
        p->drawLine(x, y + 2, x + 1, y + 2);
        p->drawPoint(x + 1, y + 3);

        // the third color
        p->setPen(data->color[1]);
        p->drawLine(x + 2, y + 2, x + 3, y + 2);
        p->drawPoint(x + 2, y + 3);
    }

    // restore pen
    p->setPen(oldPen);
}

