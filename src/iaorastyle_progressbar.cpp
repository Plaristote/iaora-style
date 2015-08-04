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

#include <QVarLengthArray>
#include <QDebug>

#include "iaorastyle.h"

void IaOraStyle::drawProgressBarContents(const QStyleOption *opt, QPainter *p, bool kde)
{
    const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(opt);
    if (!pb)
        return;

    QPalette pal = opt->palette;
    pal.setCurrentColorGroup(QPalette::Active);
    ColorData *data = lookupData(pal);

    p->save();
    QRect r = pb->rect;
    bool vertical = false;
    bool inverted = false;
    qint64 minimum = qint64(pb->minimum);
    qint64 maximum = qint64(pb->maximum);
    qint64 progress = qint64(pb->progress);
    qint64 range = maximum - minimum;
    if (range < 0)
        return;

    if (maximum == minimum || progress <= minimum) // indeterminate state
        return;

    // Get extra style options if version 2
    const QStyleOptionProgressBarV2 *pb2 = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(opt);
    if (pb2) {
        vertical = (pb2->orientation == Qt::Vertical);
        inverted = pb2->invertedAppearance;
    }

    // If the orientation is vertical, we use a transform to rotate
    // the progress bar 90 degrees clockwise.  This way we can use the
    // same rendering code for both orientations.
    if (vertical) {
        if (kde)
            r = QRect(r.left() + 2, r.top() - 4, r.height(), r.width()); // flip width and height
        else
            r = QRect(r.left(), r.top(), r.height(), r.width()); // flip width and height
        QTransform m;
        m.translate(r.height(), 0);
        m.rotate(90.0);
        p->setTransform(m);
    }

    QRect progressRect;
    if (kde)
        progressRect = r.adjusted(-1, -1, 1, 1);
    else
        progressRect = r.adjusted(1, 1, -1, -1);

    QLinearGradient g(0, progressRect.top(), 0, progressRect.bottom());
    g.setColorAt(0., data->color[1]);
    g.setColorAt(0.5, data->color[3]);
    g.setColorAt(0.51, data->color[4]);
    g.setColorAt(1., data->color[2]);

    if (progress == maximum) {
        p->fillRect(progressRect, g);
    } else {
        progress -= minimum;
        progressRect.setWidth((progressRect.width() * progress) / range - 1);
        QRect endRect = QRect(progressRect.right() + 1, progressRect.top() + 1, 1, progressRect.height() - 2);

        bool reverse = (!vertical && (pb->direction == Qt::RightToLeft)) || vertical;
        if (inverted)
            reverse = !reverse;

        if (reverse) {
            progressRect.moveRight(r.right() - 1);
            endRect.moveLeft(progressRect.left() - 1);
        }
        p->fillRect(progressRect, g);
        p->fillRect(endRect, g);
    }

    // draw the division lines
    QVarLengthArray<QLine, 32> lines1;
    QVarLengthArray<QLine, 32> lines2;

    int interval = maximum / 8;
    int pos = 0;
    int maxSize = r.width() + (kde ? 3 : -1);
    progress = progressRect.width();
    int spacer = (kde) ? -1 : 1;

    if (interval != 0)
        do {
            pos += interval;

            if (pos >= maxSize) break;

            if (!vertical) {
                if (pos <= progress)
                    lines1.append(QLine(progressRect.left() + pos, r.top() + spacer, progressRect.left() + pos, r.bottom() - spacer));
                else
                    lines2.append(QLine(progressRect.left() + pos, r.top() + spacer, progressRect.left() + pos, r.bottom() - spacer));
            } else {
                if (pos <= progress)
                    lines2.append(QLine(progressRect.left() + pos, r.top() + spacer + 1, progressRect.left() + pos, r.bottom() + (kde ? 2 : 0)));
                else
                    lines1.append(QLine(progressRect.left() + pos, r.top() + spacer + 1, progressRect.left() + pos, r.bottom() + (kde ? 2 : 0)));
            }
        } while (pos < maxSize);

    p->setPen(data->color[2]);
    p->drawLines(lines1.constData(), lines1.size());
    p->setPen(data->gray[1]);
    p->drawLines(lines2.constData(), lines2.size());
    //

    p->restore();
}

