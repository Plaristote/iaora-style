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

void IaOraStyle::drawToolButton(const QStyleOption *opt, QPainter *p)
{
	const QStyleOptionToolButton *tlBtn = qstyleoption_cast <const QStyleOptionToolButton *>(opt);

	if (!tlBtn)
		return;

	QStyleOptionToolButton copy = *tlBtn;
	// TODO: cache the gradients so that it is more efficient
	bool sunken = (opt->state & QStyle::State_Sunken);
	bool enabled = (opt->state & QStyle::State_Enabled);
	bool hover = (opt->state & QStyle::State_MouseOver);
	bool selected = (opt->state & QStyle::State_On);
	
	QRect r = opt->rect;
	QPen oldPen = p->pen();

	if (sunken || selected)
		drawButton(&copy, p, false);
	else if (hover && enabled)
	{
		copy.state |= QStyle::State_Enabled;
		drawButton(&copy, p, false);
	}

	// Here we draw the icon (if exists)...
	if (tlBtn->toolButtonStyle != Qt::ToolButtonTextOnly)
	{
		QIcon::Mode mode = (tlBtn->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
		if (hover && enabled)
			mode = QIcon::Active;

		QRect iconRect(r);
		QSize s;
	       
		if (tlBtn->iconSize.width() > r.width()) s = QSize(r.width(), r.width());
		else if (tlBtn->iconSize.height() > r.height()) s = QSize(r.height(), r.height());
		else s = tlBtn->iconSize;

		QPixmap pix = tlBtn->icon.pixmap(s, mode);

		int align = 0 ;
		if (tlBtn->toolButtonStyle == Qt::ToolButtonTextBesideIcon)
		{
			align |= Qt::AlignLeft;
			iconRect.adjust(-8, 0, -(r.width() - s.width()), 0);
		}
		else if (tlBtn->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
		{
			align |= Qt::AlignHCenter;
			iconRect.adjust(0, 12, 0, 0);
		}
		else if (tlBtn->toolButtonStyle == Qt::ToolButtonIconOnly)
			align |= Qt::AlignHCenter;

		iconRect = m_parent->alignedRect(tlBtn->direction, Qt::Alignment(align), iconRect.size(), r);
		m_parent->drawItemPixmap(p, iconRect, Qt::AlignCenter, pix);
	}

	// ... and here the text (if exists too)
	if (tlBtn->toolButtonStyle != Qt::ToolButtonIconOnly)
	{
		int text_flags = m_parent->visualAlignment(tlBtn->direction, Qt::AlignVCenter | Qt::AlignCenter) | Qt::TextShowMnemonic;

		QRect textRect = r;
		if (tlBtn->toolButtonStyle == Qt::ToolButtonTextBesideIcon)
		{
			if (tlBtn->direction == Qt::LeftToRight)
				textRect.adjust(tlBtn->iconSize.height() + 4, 0, 0, 0);
			else
				textRect.adjust(0, 0, -(tlBtn->iconSize.height() + 0), 0);
		}
		else if (tlBtn->toolButtonStyle == Qt::ToolButtonTextUnderIcon)
			textRect.adjust(0, tlBtn->iconSize.width() + 8, 0, 0);


		drawItemText(p, textRect, text_flags, tlBtn->palette, enabled, tlBtn->text, QPalette::Text);
	}

	// it has a menu?
	if (tlBtn->features & QStyleOptionToolButton::HasMenu)
	{
		copy.rect = m_parent->subControlRect(QStyle::CC_ToolButton, tlBtn, QStyle::SC_ToolButtonMenu);

		copy.state = QStyle::State_Horizontal;

		if (enabled)
			copy.state |= QStyle::State_Enabled;
		if (sunken)
			copy.state |= QStyle::State_Sunken;

		if (copy.rect != opt->rect){
			copy.rect.adjust(2, 1, 0, 0);
			drawSymbol(&copy, p, IaOraStyle::SymbolArrowDown);

			if (hover && enabled){
				copy.rect = QRect(copy.rect.left() - 2, r.top(), 2, r.height());
				drawSeparator(&copy, p, 4);
			}
		}
		else // I don't know why some toolbuttons don't have a correct place to their arrows...
		{
			copy.rect = QRect(0, 0, 8, 8);
			copy.rect.moveLeft(r.right() - 8);
			copy.rect.moveBottom(r.bottom());
			drawSymbol(&copy, p, IaOraStyle::SymbolArrowDown);
		}
	}

	p->setPen(oldPen);
}

