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

#include <QSettings>
#include <QDebug>

QHash<QString, ColorData*> IaOraStyle::m_data;

IaOraStyle::IaOraStyle(QStyle *parent)
{
	m_parent = parent;
}

IaOraStyle::~IaOraStyle()
{
}

// helper function to create the hash
inline QString hashPalette(const QPalette &pal)
{
	QString hash = pal.color(QPalette::Window).name();
	hash += pal.color(QPalette::Highlight).name();
	return hash;
}

ColorData *IaOraStyle::lookupData(const QPalette &pal)
{
	QString hash = hashPalette(pal);

	// if already cached, just return
	if (m_data.contains(hash))
		return m_data[hash];
	
	ColorData *data = new ColorData();
	QColor background = pal.color(QPalette::Window);
	QColor highlight = pal.color(QPalette::Highlight);
	
	//TODO: check whether black and white should not be black and white
	data->black = Qt::black;
	data->white = Qt::white;

	QSettings conf("/etc/iaoracolors", QSettings::NativeFormat); // TODO: define where this file must go

	if (background.name() == "#eff3f7")
	{
		conf.beginGroup("Default");
			data->gray[0] = (conf.value("gray0", background)).value<QColor>();
			data->gray[1] = (conf.value("gray1", background.dark(107))).value<QColor>();
			data->gray[2] = (conf.value("gray2", background.dark(115))).value<QColor>();
			data->gray[3] = (conf.value("gray3", background.dark(117))).value<QColor>();
			data->gray[4] = (conf.value("gray4", background.dark(131))).value<QColor>();
			data->gray[5] = (conf.value("gray5", background.dark(144))).value<QColor>();
			data->gray[6] = (conf.value("gray6", background.dark(178))).value<QColor>();
		conf.endGroup();
	} else
	{
		data->gray[0] = background;
		data->gray[1] = background.dark(107);
		data->gray[2] = background.dark(115);
		data->gray[3] = background.dark(117);
		data->gray[4] = background.dark(131);
		data->gray[5] = background.dark(144);
		data->gray[6] = background.dark(178);
	}

	QString group = "";

	if (highlight.name() == "#8ec7ff") group = "Arctic";
	else if (highlight.name() == "#4965ae") group = "Blue";
	else if (highlight.name() == "#cfd7df") group = "Gray";
	else if (highlight.name() == "#5d658e") group = "Night";
	else if (highlight.name() == "#f7b610") group = "Orange";
	else if (highlight.name() == "#7baae7") group = "Smooth";
	else if (highlight.name() == "#148dcd") group = "Steel";

	if (!group.isEmpty())
	{
		conf.beginGroup(group);
			data->color[0] = (conf.value("color0", highlight.light(150))).value<QColor>();
			data->color[1] = (conf.value("color1", highlight.light(137))).value<QColor>();
			data->color[2] = (conf.value("color2", highlight)).value<QColor>();
			data->color[3] = (conf.value("color3", highlight.light(112))).value<QColor>();
			data->color[4] = (conf.value("color4", highlight.light(130))).value<QColor>();
		conf.endGroup();
	} else
	{
		data->color[0] = highlight.light(150);
		data->color[1] = highlight.light(137);
		data->color[2] = highlight;
		data->color[3] = highlight.dark(112);
		data->color[4] = highlight.dark(130);
	}

	m_data[hash] = data;
	return data;
}

void IaOraStyle::drawItemText(QPainter *painter, const QRect &rect, int alignment, const QPalette &pal,
						  bool enabled, const QString& text, QPalette::ColorRole textRole) const
{
	ColorData *data = lookupData(pal);
	QPen oldPen = painter->pen();
	
	QPen pen = oldPen;
	
	if (!enabled)
		pen.setColor(data->gray[4]);
	else if (textRole != QPalette::NoRole)
		pen.setColor(pal.color(textRole));
		
	painter->setPen(pen);
	painter->drawText(rect, alignment, text);
	
	painter->setPen(oldPen);
}

void IaOraStyle::drawBranch(const QStyleOption *option, QPainter *painter)
{
	ColorData *data = lookupData(option->palette);
	painter->save();
	QPointF center = option->rect.center();
	QColor lineColor = option->palette.color(QPalette::Text);
	QPen pen = painter->pen();
	pen.setStyle(Qt::DotLine);
	pen.setColor(lineColor);
	painter->setPen(pen);
	
	if (option->state & (QStyle::State_Item | QStyle::State_Sibling)) 
	{
		if (option->state & QStyle::State_Sibling)
		{
			painter->drawLine(center.x(), center.y(), center.x(), option->rect.bottom());
			painter->drawLine(center.x(), option->rect.y() + 1, center.x(), center.y());
		}
		else
			painter->drawLine(center.x(), option->rect.y(), center.x(), center.y());
		
		if (option->state & QStyle::State_Item){
			if (option->direction == Qt::LeftToRight)
				painter->drawLine(m_parent->visualPos(option->direction, option->rect, QPoint(center.x() + 1, center.y())),
						m_parent->visualPos(option->direction, option->rect, QPoint(option->rect.right(), center.y())));
			else{
				painter->drawLine(center.x(), center.y(), center.x() + 1, center.y());
				painter->drawLine(center.x(), center.y(), option->rect.left(), center.y());
			}
		}
	}
	if (option->state & QStyle::State_Children)
	{
		painter->setBrush(data->white);
		painter->setPen(Qt::NoPen);
		QRect r(0,0,9,9);
		r.moveCenter(option->rect.center());
		painter->drawRect(r.adjusted(1,1,-1,-1));
		drawFrameCuttingOne(painter, r, lineColor);
		drawSymbol(option, painter, (option->state & QStyle::State_Open) ? SymbolMinus : SymbolPlus);
	}
	painter->restore();
}

void IaOraStyle::drawPanelItemViewItem(const QStyleOption *opt, QPainter *p, bool itemViewItem)
{
	const QStyleOptionViewItemV4 *pvi = qstyleoption_cast<const QStyleOptionViewItemV4 *>(opt);
	if (!pvi)
		return;
	else
		p->fillRect(opt->rect, pvi->backgroundBrush);

	ColorData *data = lookupData(opt->palette);
	
	p->save();
	p->setRenderHint(QPainter::Antialiasing);
	QRect r = pvi->rect;

	//if (pvi->state & QStyle::State_HasFocus)
	// in case of the need to use the HasFocus state, remove the commented lines and commend the above line
	/*if (( pvi->state & QStyle::State_HasFocus ) || ( pvi->state & QStyle::State_Selected))
	{
		p->setBrush(data->color[2]);
		p->setPen(Qt::NoPen);
		p->drawRoundedRect(r, 3.0, 3.0);
	}*/
	/*else*/ if (pvi->state & QStyle::State_Selected)
	{
		p->setBrush(data->color[2]);
		p->setPen(Qt::NoPen);
		p->drawRoundedRect(r, 3.0, 3.0);
	}/**/
	p->restore();

	if (itemViewItem){
		QRect checkRect;
		QRect iconRect;
		QSize iconSize = pvi->decorationSize;
		bool hasCheck = pvi->features & QStyleOptionViewItemV2::HasCheckIndicator;
		bool hasIcon = pvi->features & QStyleOptionViewItemV2::HasDecoration;
		bool hasText = pvi->features & QStyleOptionViewItemV2::HasDisplay;
		bool leftToRight = (pvi->direction == Qt::LeftToRight) ? true : false;

		// the checkbox on the item
		if (hasCheck){
			QStyleOptionButton button;

	
			checkRect = QRect(r.left(), r.center().y() - 6, 13, 13);
			if (!hasText) checkRect.adjust(6, 0, 6, 0); // TEMPORARY HACK
			//if (!hasText) checkRect.moveCenter(r.center());

			checkRect = QStyle::visualRect(pvi->direction, pvi->rect, checkRect);

			button.rect = checkRect;
			button.state = pvi->state;

			switch (pvi->checkState){
				case Qt::Unchecked:
					button.state |= QStyle::State_Off;

					break;
				case Qt::PartiallyChecked:
					button.state |= QStyle::State_NoChange;
					
					break;
				case Qt::Checked:
					button.state |= QStyle::State_On;
					
					break;
			}

			drawCheckBox(&button, p);
		}

		int shiftx = 0, shifty = 0, shiftw = 0;

		// some icon
		if (hasIcon){
			switch (pvi->decorationPosition){
				case QStyleOptionViewItem::Left:
					if (hasCheck)
						iconRect = QRect(r.topLeft() + QPoint(checkRect.width() + 2, 0), iconSize);
					else
						iconRect = QRect(r.topLeft(), iconSize);

					if (leftToRight) shiftx = 3 + iconSize.width();
					else shiftw = -(3 + iconSize.width());

					break;
				case QStyleOptionViewItem::Right:
					iconRect = QRect(r.topRight() - QPoint(iconSize.width(), 0), iconSize);
					shiftw = -(3 + iconSize.width());

					break;
				case QStyleOptionViewItem::Top:
					iconRect = QRect(r.left(), r.top(), r.width(), iconSize.height());
					shifty = iconRect.height();

					break;
				case QStyleOptionViewItem::Bottom:
					iconRect = QRect(r.left(), r.bottom() - iconSize.height(), r.width(), r.bottom());

					break;
			}

			iconRect = QStyle::visualRect(pvi->direction, pvi->rect, iconRect);

			QIcon::Mode mode = pvi->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
			QPixmap pix = pvi->icon.pixmap(iconSize, mode);

			m_parent->drawItemPixmap(p, iconRect, pvi->displayAlignment, pix);
		} else shiftx = 3;

		// and the text
		p->save();
			QFont font = pvi->font;
			if (font.bold()) p->setFont(font);

			if (hasCheck){
				if (leftToRight) shiftx += iconSize.width();
				else shiftw -= iconSize.width();
			}

			drawItemText(p, r.adjusted(shiftx, shifty, shiftw, 0), pvi->displayAlignment, pvi->palette, true,
					pvi->text, (pvi->state & QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::Text);
		p->restore();
	}
}

void IaOraStyle::drawPanelItemViewRow(const QStyleOption *opt, QPainter *p){
	const QStyleOptionViewItemV4 *row = qstyleoption_cast<const QStyleOptionViewItemV4 *>(opt);

	if (!row)
		return;

	ColorData *data = lookupData(opt->palette);
	bool alternateColor = row->features & QStyleOptionViewItemV2::Alternate;

	p->save();
	p->setRenderHint(QPainter::Antialiasing);

	QPalette::ColorGroup colorGroup(opt->state & QStyle::State_Enabled
					? opt->state & QStyle::State_Active
					? QPalette::Normal
					: QPalette::Inactive
					: QPalette::Disabled);
       if (alternateColor)
		p->fillRect(opt->rect, opt->palette.brush(colorGroup, QPalette::AlternateBase));

	p->restore();
}

