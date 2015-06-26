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

#include <cmath>

void IaOraStyle::drawDial(const QStyleOption *opt, QPainter *p)
{
	QPalette pal = opt->palette;
	pal.setCurrentColorGroup(QPalette::Active);
	ColorData *data = lookupData(pal);
	
	const QStyleOptionSlider *dial = qstyleoption_cast<const QStyleOptionSlider *>(opt);
	if (!dial)
		return;
	
	// use a square
	QRect r = QRect(0,0, qMin(opt->rect.width(), opt->rect.height()), qMin(opt->rect.width(), opt->rect.height()));
	r.moveCenter(opt->rect.center());
	r.adjust(3,3,-3,-3);
	
	QPen oldPen = p->pen();
	QBrush oldBrush = p->brush();
	bool aa = p->renderHints() & QPainter::Antialiasing;
	
	QLinearGradient g1(r.bottomLeft(), r.topLeft());
	g1.setColorAt(0., data->gray[0]);
	g1.setColorAt(1., data->gray[2]);
	
	QLinearGradient g2(r.topLeft(), r.bottomRight());
	g2.setColorAt(0., data->white);
	g2.setColorAt(0.5, data->gray[4]);
	g2.setColorAt(1., data->gray[5]);
	
	p->setBrush(g1);
	p->setPen(QPen(g2, 2));
	p->setRenderHint(QPainter::Antialiasing, true);
	p->drawEllipse(r);
	
	// draw the indicator
	int gripSize = opt->fontMetrics.height() * 0.5;
	
	QRect grip(0,0,gripSize,gripSize);
	grip.moveCenter(r.center());
	
	// angle calculation from qcommonstyle.cpp (c) Trolltech 1992-2007, ASA.
	qreal angle;
	int sliderPosition = dial->upsideDown ? dial->sliderPosition : (dial->maximum - dial->sliderPosition);
	int range = dial->maximum - dial->minimum;
	if (!range) 
	{
		angle = M_PI / 2;
	} 
	else if (dial->dialWrapping) 
	{
		angle = M_PI * 1.5 - (sliderPosition - dial->minimum) * 2 * M_PI / range;
	} 
	else
	{
		angle = (M_PI * 8 - (sliderPosition - dial->minimum) * 10 * M_PI / range) / 6;
	}
	int d = r.width() - 10;
	qreal rr = d / 2.0 - gripSize - 2;
	grip.translate(int(0.5 + rr * cos(angle)), int(0.5 - rr * sin(angle)));
	
	QRadialGradient g3(grip.left() + (grip.width() * 0.7), grip.top() + (grip.height() * 0.7), grip.height());
	g3.setColorAt(0., data->color[1]);
	g3.setColorAt(0.3, data->color[2]);
	g3.setColorAt(1., data->color[4]);
	p->setPen(Qt::NoPen);
	p->setBrush(g3);
	p->drawEllipse(grip);
	
	p->setPen(oldPen);
	p->setBrush(oldBrush);
	p->setRenderHint(QPainter::Antialiasing, aa);
}

