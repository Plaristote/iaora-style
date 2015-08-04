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
 ** SC_SliderTickmarks code based on QtCurve style fom KDE4
 *  Copyright C( 2007-2008 Craig Drummond <Craig.Drummond@lycos.co.uk>
 *
 */

#include <QVarLengthArray>
#include "iaorastyle.h"

void IaOraStyle::drawSlider(const QStyleOption *opt, QPainter *p,
                            const QRect &grooveRegion, const QRect &handle, const QRect &ticks)
{
    const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (!slider)
        return;

    QPalette pal = opt->palette;
    pal.setCurrentColorGroup(QPalette::Active);
    ColorData *data = lookupData(pal);

    QPen oldPen = p->pen();

    bool active = (opt->state & QStyle::State_Enabled);
    bool vertical = slider->orientation == Qt::Vertical;

    if (slider->subControls & QStyle::SC_SliderGroove) {
        QRect filled = grooveRegion.adjusted(1, 1, -1, -1);
        p->fillRect(filled, data->gray[0]);

        if (slider->orientation == Qt::Horizontal) {
            if (slider->upsideDown) filled.setLeft(handle.right() - 2);
            else filled.setRight(handle.left() + 2);
        } else {
            if (slider->upsideDown) filled.setTop(handle.bottom() - 2);
            else filled.setBottom(handle.top() + 2);
        }

        p->fillRect(filled, data->gray[3]);

        drawFrameCuttingOne(p, grooveRegion, active ? data->gray[5] : data->gray[4]);
    }

    // The code for SC_SliderTickmarks is based on the implementation of QtCurve style for KDE4;
    // check the copyright on the top of this file
    if (slider->subControls & QStyle::SC_SliderTickmarks) {
        QVarLengthArray<QLine, 32> lines;

        int sliderLen = m_parent->pixelMetric(QStyle::PM_SliderLength, opt);
        int tickSize = m_parent->pixelMetric(QStyle::PM_SliderTickmarkOffset, opt);
        int availableSpace = m_parent->pixelMetric(QStyle::PM_SliderSpaceAvailable, opt);

        int value = slider->minimum;
        int tickInterval = slider->tickInterval;
        int pos = 0;

        if (tickInterval <= 0) {
            tickInterval = slider->singleStep;
            if (QStyle::sliderPositionFromValue(slider->minimum, slider->maximum,
                                                tickInterval, availableSpace) -
                QStyle::sliderPositionFromValue(slider->minimum,
                                                slider->maximum, 0, availableSpace) < 3)
                tickInterval = slider->pageStep;
        }
        if (tickInterval <= 0) tickInterval = 1;

        while (value <= slider->maximum) {
            if (slider->orientation == Qt::Horizontal) {
                pos = QStyle::sliderPositionFromValue(slider->minimum,
                                                      slider->maximum, value, slider->rect.width() + sliderLen,
                                                      slider->upsideDown) + 12;

                lines.append(QLine(pos, slider->rect.top() + 11, pos,
                                   slider->rect.top() + tickSize + 1));
            } else {
                pos = QStyle::sliderPositionFromValue(slider->minimum,
                                                      slider->maximum, value, slider->rect.height() - sliderLen,
                                                      slider->upsideDown) + 4;

                lines.append(QLine(slider->rect.left() + 11, pos,
                                   slider->rect.left() + tickSize + 1, pos));
            }

            if (value + tickInterval < value) break;
            value = value + tickInterval;
        }

        p->setPen(data->gray[1]);
        p->drawLines(lines.constData(), lines.size());
    }

    if (slider->subControls & QStyle::SC_SliderHandle) {
        QRect circle(0, 0, 4, 4);
        QRect handleCpy;
        circle.moveCenter(handle.center());
        QLinearGradient g1, g2;
        if (vertical) {
            handleCpy = handle.adjusted(-1, 0, 1, 0);
            g1 = QLinearGradient(handleCpy.left(), 0, handleCpy.right(), 0);
            g2 = QLinearGradient(circle.left(), 0, circle.right(), 0);
        } else {
            handleCpy = handle.adjusted(0, -1, 0, 1);
            g1 = QLinearGradient(0, handleCpy.top(), 0, handleCpy.bottom());
            g2 = QLinearGradient(0, circle.top(), 0, circle.bottom());
        }

        g1.setColorAt(0., data->color[2]);
        g1.setColorAt(1., data->color[4]);
        g2.setColorAt(0., data->color[4]);
        g2.setColorAt(1., data->color[2]);

        fillRectCuttingOne(p, handleCpy, g1);

        p->setPen(Qt::NoPen);
        p->setBrush(g2);
        p->drawEllipse(circle);

    }

    p->setPen(oldPen);
}

