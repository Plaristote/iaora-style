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
 ** ** QTabBarLabel code based on QCommonStyle
 *     Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
 */

#include "iaorastyle.h"

void IaOraStyle::drawTabBarTab(const QStyleOption *opt, QPainter *p)
{
	const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(opt);
	QRect r = opt->rect;
	
	if (!tab)
		return;

	QPalette pal = opt->palette;
	pal.setCurrentColorGroup(QPalette::Active);
	ColorData *data = lookupData(pal);
	
	bool selected = (tab->state & QStyle::State_Selected);
	int top = r.top(), bottom = r.bottom(), left = r.left(), right = r.right();
	
	p->save();
	
	if (selected) // active tab
	{
		QRect contents = opt->rect.adjusted(1,1,-1,-1);
		QLine borders[2];
		QLine lightBlue[2];
		QLine darkBlue[2];
		
		if (tab->shape == QTabBar::RoundedNorth || tab->shape == QTabBar::TriangularNorth)
		{
			contents.adjust(0,3,0,1);
			borders[0] = QLine(left, top+4, left, bottom);
			borders[1] = QLine(right, top+4, right, bottom);
			lightBlue[0] = QLine(left+2,top,right-2,top);
			lightBlue[1] = QLine(left+1,top+1,right-1,top+1);
			darkBlue[0] = QLine(left,top+2,right,top+2);
			darkBlue[1] = QLine(left,top+3,right,top+3);
		}
		else if (tab->shape == QTabBar::RoundedSouth || tab->shape == QTabBar::TriangularSouth)
		{
			contents.adjust(0,-1,0,-3);
			borders[0] = QLine(left,top,left,bottom-4);
			borders[1] = QLine(right,top,right,bottom-4);
			lightBlue[0] = QLine(left, bottom-3, right, bottom-3);
			lightBlue[1] = QLine(left, bottom-2, right, bottom-2);
			darkBlue[0] = QLine(left+1, bottom-1, right-1, bottom-1);
			darkBlue[1] = QLine(left+2, bottom, right-2, bottom);
		}
		else if (tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::TriangularEast) // right
		{
			contents.adjust(-1,0,-3,0);
			borders[0] = QLine(left, top, right-4, top);
			borders[1] = QLine(left, bottom, right-4, bottom);
			lightBlue[0] = QLine(right-3, top, right-3, bottom);
			lightBlue[1] = QLine(right-2, top, right-2, bottom);
			darkBlue[0] = QLine(right-1, top+1, right-1, bottom-1);
			darkBlue[1] = QLine(right, top+2, right, bottom-2);
		}
		else // left
		{
			contents.adjust(3,0,1,0);
			borders[0] = QLine(left+4, top, right, top);
			borders[1] = QLine(left+4, bottom, right, bottom);
			lightBlue[0] = QLine(left, top+2, left, bottom-2);
			lightBlue[1] = QLine(left+1, top+1, left+1, bottom-1);
			darkBlue[0] = QLine(left+2, top, left+2, bottom);
			darkBlue[1] = QLine(left+3, top, left+3, bottom);
		}
		
		// fill the contents
		p->fillRect(contents, data->gray[0]);
		
		// draw the gray borders
		p->setPen(data->gray[4]);
		p->drawLines(borders, 2);

		// the blue lines
		p->setPen(data->color[2]);
		p->drawLines(lightBlue, 2);
		p->setPen(data->color[4]);
		p->drawLines(darkBlue, 2);
	}
	else // inactive tab
	{
		QRect contents = opt->rect.adjusted(1,1,-1,-1);
		
		QPoint points[2];
		QLine lines[4];
		if (tab->shape == QTabBar::RoundedNorth || tab->shape == QTabBar::TriangularNorth)
		{
			top+=3;
			contents.setTop(top+1);
			lines[0] = QLine(left+2,top,right-2,top); // top
			lines[1] = QLine(left, top+2, left, bottom); // left
			lines[2] = QLine(right, top+2, right, bottom); // right
			lines[3] = QLine(left, bottom, right, bottom); // bottom
			points[0] = QPoint(left+1, top+1);
			points[1] = QPoint(right-1, top+1);
		}
		else if (tab->shape == QTabBar::RoundedSouth || tab->shape == QTabBar::TriangularSouth)
		{
			bottom-=3;
			contents.setBottom(bottom-1);
			lines[0] = QLine(left,top,right,top); // top
			lines[1] = QLine(left, top, left, bottom-2); // left
			lines[2] = QLine(right, top, right, bottom-2); // right
			lines[3] = QLine(left+2, bottom, right-2, bottom); // bottom
			points[0] = QPoint(left+1, bottom-1);
			points[1] = QPoint(right-1, bottom-1); 
		}
		else if (tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::TriangularEast) // right
		{
			right-=3;
			contents.setRight(right-1);
			lines[0] = QLine(left,top,right-2,top); // top
			lines[1] = QLine(left,top,left,bottom); // left
			lines[2] = QLine(right,top+2,right,bottom-2); // right
			lines[3] = QLine(left,bottom,right-2,bottom); // bottom
			points[0] = QPoint(right-1,top+1);
			points[1] = QPoint(right-1,bottom-1);
		}
		else // left
		{
			left+=3;
			contents.setLeft(left+1);
			lines[0] = QLine(left+2,top,right,top); // top
			lines[1] = QLine(left,top+1,left,bottom-2); // left
			lines[2] = QLine(right,top,right,bottom); // right
			lines[3] = QLine(left+2,bottom,right,bottom); // bottom
			points[0] = QPoint(left+1,top+1);
			points[1] = QPoint(left+1,bottom-1);
		}
		
		// draw the contents
		p->setPen(Qt::NoPen);
		p->setBrush(data->gray[1]);
		p->drawRect(contents);
		
		// draw the lines
		p->setPen(data->gray[5]);
		p->drawLines(lines, 4);
		p->drawPoints(points, 2);
	}

	p->restore();

	// The code below is based on the implementation of QCommonStyle; 
	// check the copyright on the top of this file
	QStyleOptionTabV2 tabV2(*tab);
	QRect tabRect = tabV2.rect;

	// we have the vertical tab text
	bool verticalTabs = tabV2.shape == QTabBar::RoundedEast	
				|| tabV2.shape == QTabBar::RoundedWest 
				|| tabV2.shape == QTabBar::TriangularEast 
				|| tabV2.shape == QTabBar::TriangularWest;
	if (verticalTabs) {
		p->save();

		int newX, newY, newRot;
		if (tabV2.shape == QTabBar::RoundedEast || tabV2.shape == QTabBar::TriangularEast){
			newX = tabRect.width();
			newY = tabRect.y();
			newRot = 90;
		}else {
			newX = 0;
			newY = tabRect.y() + tabRect.height();
			newRot = -90;
		}
		tabRect.setRect(0, 0, tabRect.height(), tabRect.width());
		
		QTransform trans;

		trans.translate(newX, newY);
		trans.rotate(newRot);
		p->setTransform(trans, true);
	}

	// and the icon
	QSize iconSize = tabV2.iconSize;

	if (!tabV2.icon.isNull()){
		if (!iconSize.isValid()) {
			int iconExtent = m_parent->pixelMetric(QStyle::PM_SmallIconSize);
			iconSize = QSize(iconExtent, iconExtent);
		}

		QPixmap tabIcon = tabV2.icon.pixmap(iconSize, (tabV2.state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled);
		
		QRect iconRect = QRect(tabRect.left() + 6, tabRect.center().y() - tabIcon.height() / 2, iconSize.width(), iconSize.height());

		if (tabV2.direction != Qt::LeftToRight){
			iconRect = QStyle::visualRect(tabV2.direction, tabV2.rect, iconRect);
			tabRect.setRight(tabRect.right() - iconSize.width() - 4);
		} else
			tabRect.setLeft(tabRect.left() + iconSize.width() + 4);

		p->drawPixmap(iconRect, tabIcon);
	}

	// finally draw the text
	QRect textRect = tabRect;
	QFontMetrics fontMetrics = tabV2.fontMetrics;

	QSize textSize = fontMetrics.size(Qt::TextSingleLine, tab->text);

	// we must adjust the textRect if we have a close button on the tabbar
	if ((!tabV2.icon.isNull()) && (tabRect.width() - (textSize.width() + iconSize.width()) > 8)){
		if (tabV2.direction == Qt::LeftToRight)
			textRect.adjust(-10, 0, -10, 0);
		else
			textRect.adjust(+10, 0, +10, 0);
	}

	drawItemText(p, textRect, Qt::AlignCenter | Qt::TextShowMnemonic, tab->palette, true, tab->text, QPalette::WindowText);
	//drawItemText(p, textRect, Qt::AlignCenter | Qt::TextShowMnemonic, tab->palette, true, tab->text, QPalette::Text);
	if (verticalTabs) p->restore();
}

