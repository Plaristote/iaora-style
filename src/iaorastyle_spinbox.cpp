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

void IaOraStyle::drawSpinBox(const QStyleOptionComplex *opt, QPainter *p)
{
    const QStyleOptionSpinBox *sb = qstyleoption_cast<const QStyleOptionSpinBox *>(opt);
    QStyleOptionSpinBox copy = *sb;
    QRect r = opt->rect;

    if (!sb)
        return;

    QStyle::PrimitiveElement pe;

    if (sb->frame && (sb->subControls & QStyle::SC_SpinBoxFrame)) {
        QRect r = m_parent->subControlRect(QStyle::CC_SpinBox, sb, QStyle::SC_SpinBoxFrame);

        copy.rect = r;
        drawFrame(&copy, p);
    }

    /* SPIN-UP */
    if (sb->subControls & QStyle::SC_SpinBoxUp) {
        copy.subControls = QStyle::SC_SpinBoxUp;
        QPalette pal2 = sb->palette;
        if (!(sb->stepEnabled & QAbstractSpinBox::StepUpEnabled)) {
            pal2.setCurrentColorGroup(QPalette::Disabled);
            copy.state &= ~QStyle::State_Enabled;
        }

        copy.palette = pal2;

        if (sb->activeSubControls == QStyle::SC_SpinBoxUp && (sb->state & QStyle::State_Sunken)) {
            copy.state |= QStyle::State_On;
            copy.state |= QStyle::State_Sunken;
        } else {
            copy.state |= QStyle::State_Raised;
            copy.state &= ~QStyle::State_Sunken;
        }
        pe = (sb->buttonSymbols == QAbstractSpinBox::PlusMinus ? QStyle::PE_IndicatorSpinPlus : QStyle::PE_IndicatorSpinUp);
        copy.rect = m_parent->subControlRect(QStyle::CC_SpinBox, sb, QStyle::SC_SpinBoxUp);
        m_parent->drawPrimitive(QStyle::PE_PanelButtonBevel, &copy, p);
        drawBorder(p, opt, copy.rect, sb->state & QStyle::State_Enabled, IaOraStyle::BorderLeft);
        /* FIXME: don't really know why the rect is not right at this point, but anyway... */
        copy.rect.adjust(1, 2, 0, 0);
        m_parent->drawPrimitive(pe, &copy, p);

    }
    /* SPIN-DOWN */
    if (sb->subControls & QStyle::SC_SpinBoxDown) {
        copy.subControls = QStyle::SC_SpinBoxDown;
        copy.state = sb->state;
        QPalette pal2 = sb->palette;
        if (!(sb->stepEnabled & QAbstractSpinBox::StepDownEnabled)) {
            pal2.setCurrentColorGroup(QPalette::Disabled);
            copy.state &= ~QStyle::State_Enabled;
        }

        copy.palette = pal2;

        if (sb->activeSubControls == QStyle::SC_SpinBoxDown && (sb->state & QStyle::State_Sunken)) {
            copy.state |= QStyle::State_On;
            copy.state |= QStyle::State_Sunken;
        } else {
            copy.state |= QStyle::State_Raised;
            copy.state &= ~QStyle::State_Sunken;
        }
        pe = (sb->buttonSymbols == QAbstractSpinBox::PlusMinus ? QStyle::PE_IndicatorSpinMinus : QStyle::PE_IndicatorSpinDown);
        copy.rect = m_parent->subControlRect(QStyle::CC_SpinBox, sb, QStyle::SC_SpinBoxDown);
        m_parent->drawPrimitive(QStyle::PE_PanelButtonBevel, &copy, p);
        drawBorder(p, opt, copy.rect, sb->state & QStyle::State_Enabled, IaOraStyle::BorderLeft | IaOraStyle::BorderTop);
        /* FIXME: don't really know why the rect is not right at this point, but anyway... */
        copy.rect.adjust(1, 2, 0, 0);
        m_parent->drawPrimitive(pe, &copy, p);

    }
}

