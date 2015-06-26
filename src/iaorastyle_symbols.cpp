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

void IaOraStyle::drawCheckBox(const QStyleOption *opt, QPainter *p)
{
	QPen oldPen = p->pen();
	bool enabled = (opt->state & QStyle::State_Enabled);

	QPalette pal = opt->palette;
	pal.setCurrentColorGroup(QPalette::Active);
	ColorData *data = lookupData(pal);

	QRect r = opt->rect;
	QRect contents = r.adjusted(1,1,-1,-1);
	p->fillRect(contents, enabled ? data->white : data->gray[0]);
	drawFrameCuttingOne(p, r, enabled ? data->gray[5] : data->gray[4]);
	
	if (opt->state & QStyle::State_NoChange) // tristate
	{
		QRect linesRect = QRect(contents.left() + 2, 0, contents.width() - 4, 3);
		linesRect.moveCenter(contents.center());
		int top = linesRect.top(), left = linesRect.left(), bottom = linesRect.bottom(), right = linesRect.right();
		p->setPen(enabled ? data->color[2] : data->gray[3]);
		p->drawLine(left+1,top,right-1,top);
		p->drawLine(left,top+1,right,top+1);
		p->setPen(enabled ? data->color[4] : data->gray[5]);
		p->drawLine(left+1,bottom,right-1,bottom);
	} 
	else if (opt->state & QStyle::State_On)
	{
		contents.adjust(1,1,-1,-1);
		QLinearGradient g(contents.topLeft(), contents.bottomLeft());
		g.setColorAt(0., enabled ? data->color[2] : data->gray[3]);
		g.setColorAt(1., enabled ? data->color[4] : data->gray[5]);
		QPen pen(QBrush(g), 2);
		pen.setCapStyle(Qt::RoundCap);
		p->setPen(pen);
		p->drawLine(contents.topLeft(), contents.bottomRight());
		p->drawLine(contents.bottomLeft(), contents.topRight());
	}
	
	p->setPen(oldPen);
}

void IaOraStyle::drawRadioButton(const QStyleOption *opt, QPainter *p)
{
	QPalette pal = opt->palette;
	pal.setCurrentColorGroup(QPalette::Active);
	ColorData *data = lookupData(pal);

	QPen oldPen = p->pen();
	QBrush oldBrush = p->brush();
	bool aa = (p->renderHints() & QPainter::Antialiasing);
	bool enabled = (opt->state & QStyle::State_Enabled);
	
	QRect r = opt->rect;
	
	p->setRenderHint(QPainter::Antialiasing, true);
	p->setPen(data->gray[4]);
	p->setBrush(enabled ? Qt::white : data->gray[0]);
	p->drawEllipse(r);
	
	if (opt->state & (QStyle::State_Sunken | QStyle::State_On)) 
	{
		QRect contents = r.adjusted(3,3,-3,-3);
		QLinearGradient g(contents.topLeft(), contents.bottomLeft());
		g.setColorAt(0., enabled ? data->color[2] : data->gray[3]);
		g.setColorAt(1., enabled ? data->color[4] : data->gray[5]);
		p->setPen(Qt::NoPen);
		p->setBrush(g);
		p->drawEllipse(contents);
	}
	
	p->setPen(oldPen);
	p->setBrush(oldBrush);
	p->setRenderHint(QPainter::Antialiasing, aa);
}


void IaOraStyle::drawSymbol(const QStyleOption *opt, QPainter *p, Symbol s)
{
	QRect r = opt->rect;
	
	QPoint center = r.center();
	bool enabled = (opt->state & QStyle::State_Enabled);
	ColorData *data = lookupData(opt->palette);
	bool selected = (opt->state & QStyle::State_Selected);
	QColor c;
        if (enabled){
	       if (selected && (  opt->palette.highlightedText().color() != Qt::white ) )
	       	       c = opt->palette.highlightedText().color();
	       else
		       c = opt->palette.text().color();
	} else
		c = data->gray[4];
	//QColor c = enabled ? data->black : data->gray[4];

	bool aa = (p->renderHints() & QPainter::Antialiasing);
	QPen oldPen = p->pen();
	QBrush oldBrush = p->brush();
	
	//p->setRenderHint(QPainter::Antialiasing, true); // with antialiasing, the arrows doesn't look like arrow :S
	switch(s)
	{
		case SymbolArrowUp:
		{
			QPoint points[3];
			points[0] = center + QPoint(0, -3);
			points[1] = points[0] + QPoint(-4, 5);
			points[2] = points[0] + QPoint(4, 4);
			
			p->setPen(Qt::NoPen);
			p->setBrush(c);
			p->drawConvexPolygon(points, 3);
			break;
		}
		case SymbolArrowDown:
		{
			QPoint points[3];
			points[0] = center + QPoint(0, 3);
			points[1] = points[0] + QPoint(-3, -4);
			points[2] = points[0] + QPoint(4, -4);
			
			p->setPen(Qt::NoPen);
			p->setBrush(c);
			p->drawConvexPolygon(points, 3);
			break;
		}
		case SymbolArrowLeft:
		{
			QPoint points[3];
			points[0] = center + QPoint(-3, 0);
			points[1] = points[0] + QPoint(4, -4);
			points[2] = points[0] + QPoint(4, 4);
			
			p->setPen(Qt::NoPen);
			p->setBrush(c);
			p->drawConvexPolygon(points, 3);
			break;
		}
		case SymbolArrowRight:
		{
			QPoint center = r.center();
			QPoint points[3];
			points[0] = center + QPoint(3, 0);
			points[1] = points[0] + QPoint(-4, -4);
			points[2] = points[0] + QPoint(-4, 4);
			
			p->setPen(Qt::NoPen);
			p->setBrush(c);
			p->drawConvexPolygon(points, 3);
			break;
		}
		case SymbolPlus:
		{
			QPoint center = r.center();
			p->setPen(c);
			p->setRenderHint(QPainter::Antialiasing, false);
			p->drawLine(center - QPoint(2,0), center + QPoint(2,0));
			p->drawLine(center - QPoint(0,2), center + QPoint(0,2));
			break;
		}
		case SymbolMinus:
		{
			QPoint center = r.center();
			p->setPen(c);
			p->setRenderHint(QPainter::Antialiasing, false);
			p->drawLine(center - QPoint(2,0), center + QPoint(2,0));
			break;
		}
	}
	
	p->setRenderHint(QPainter::Antialiasing, aa);
	p->setPen(oldPen);
	p->setBrush(oldBrush);
}

