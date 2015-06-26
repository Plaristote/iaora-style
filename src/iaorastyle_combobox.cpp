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

void IaOraStyle::drawComboBox(const QStyleOptionComplex *opt, QPainter *p)
{
	/* TODO: check why Qt is not respecting PM_ComboBoxFrame */
	const QStyleOptionComboBox *cmb = qstyleoption_cast<const QStyleOptionComboBox *>(opt);
	bool sunken = cmb->state & QStyle::State_On; // play dead if combobox has no items
	QRect r = opt->rect;
	
	QStyleOptionComboBox copy = *cmb;
	if (!cmb)
		return;

	if (!cmb->editable)
	{
		QStyleOptionButton buttonOption;
		buttonOption.QStyleOption::operator=(*cmb);
		if (!sunken) 
			buttonOption.state &= ~QStyle::State_Sunken;
		
		m_parent->drawPrimitive(QStyle::PE_PanelButtonCommand, &buttonOption, p);
	}
	else
	{
		if (cmb->frame && (cmb->subControls & QStyle::SC_ComboBoxFrame))
		{
			QRect r = m_parent->subControlRect(QStyle::CC_ComboBox, cmb, QStyle::SC_ComboBoxFrame);
		
			copy.rect = r;
			drawFrame(&copy, p);
		}
	}
	
	if (cmb->subControls & QStyle::SC_ComboBoxArrow)
	{
		copy.rect = m_parent->subControlRect(QStyle::CC_ComboBox, opt, QStyle::SC_ComboBoxArrow);
		
		if (cmb->editable)
		{
			// draw the button background
			drawButton(&copy, p, true);
			drawBorder(p, opt, copy.rect, (cmb->state & QStyle::State_Enabled), BorderLeft);
		
		}
		copy.rect.adjust(2,1,0,0);
		drawSymbol(&copy, p, IaOraStyle::SymbolArrowDown);
		if (!cmb->editable)
		{
			// draw also the separator
			copy.rect = QRect(copy.rect.left() - 2, r.top() + 4, 2, r.height() - 8);
			copy.state = QStyle::State_Horizontal;
			drawSeparator(&copy, p, 4);
		}
	}
}

void IaOraStyle::drawComboBoxLabel(const QStyleOption *opt, QPainter *p)
{
	const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox*>(opt);
	if (!cb)
		return;
            
	QRect r = m_parent->subControlRect(QStyle::CC_ComboBox, cb, QStyle::SC_ComboBoxEditField);
	r.adjust(3,1,-1,0);

	if (!cb->currentIcon.isNull()) 
	{
		QIcon::Mode mode = cb->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
		QPixmap pix = cb->currentIcon.pixmap(cb->iconSize, mode);
		QRect iconRect(r);
		iconRect.setWidth(cb->iconSize.width() + 4);
		iconRect = m_parent->alignedRect(cb->direction, Qt::AlignLeft | Qt::AlignVCenter, iconRect.size(), r);
		
		if (cb->editable)
			p->fillRect(iconRect, opt->palette.brush(QPalette::Base));
		m_parent->drawItemPixmap(p, iconRect, Qt::AlignCenter, pix);

		if (cb->direction == Qt::RightToLeft)
			r.translate(-4 - cb->iconSize.width(), 0);
		else
			r.translate(cb->iconSize.width() + 4, 0);
	}

	if (!cb->currentText.isEmpty() && !cb->editable) 
	{
		m_parent->drawItemText(p, r.adjusted(1, 0, -1, 0),
			m_parent->visualAlignment(cb->direction, Qt::AlignLeft | Qt::AlignVCenter),
			cb->palette, cb->state & QStyle::State_Enabled, cb->currentText);
	}
}
