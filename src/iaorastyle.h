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

#ifndef IAORASTYLE_H
#define IAORASTYLE_H

#include <QColor>
#include <QRect>
#include <QPalette>
#include <QIcon>
#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include <QHash>

typedef struct
{
	QColor gray[7];
	QColor color[5];
	QColor black;
	QColor white;
} ColorData;

class IaOraStyle
{
public:
	enum FrameType 
	{
		FrameFocus,
		FrameGeneric,
		FrameMenuBar,
		FrameToolBar,
		FrameProgressBar,
		FrameLineEdit,
		FrameHeader,
		FrameComboBox,
		FrameMenu,
		FrameTabBar,
		FrameGroupBox,
		FrameFrame
	};
	enum Border 
	{
		BorderNone = 0,
		BorderTop = 1,
		BorderLeft = 2,
		BorderBottom = 4,
		BorderRight = 8,
		BorderAll = 15
	};
	
	enum Symbol
	{
		SymbolArrowLeft,
		SymbolArrowRight,
		SymbolArrowUp,
		SymbolArrowDown,
		SymbolPlus,
		SymbolMinus
	};
    IaOraStyle(QStyle *parent);
    ~IaOraStyle();
	
	void drawFrame(const QStyleOption *opt, QPainter *p, FrameType f = FrameGeneric, QRect optRect = QRect());
	//void drawFrame(const QStyleOption *opt, QPainter *p, FrameType f = FrameGeneric);
	void drawBorder(QPainter *painter, const QStyleOption *opt, const QRect &rect, bool active, int border = BorderAll);
	void drawSeparator(const QStyleOption *opt, QPainter *p, int margin = 5);
	void drawHandle(const QStyleOption *opt, QPainter *p);
	void drawButton(const QStyleOption *opt, QPainter *p, bool simplified = false, bool vertical = false);
	void drawButtonDefaultBorder(const QStyleOption *opt, QPainter *p);
	void drawToolButton(const QStyleOption *opt, QPainter *p/*, bool simplified = false, bool vertical = false*/);
	void drawItemText(QPainter *painter, const QRect &rect, int alignment, const QPalette &pal,
					  bool enabled, const QString& text, QPalette::ColorRole textRole) const;
	void drawProgressBarContents(const QStyleOption *opt, QPainter *p, bool kde = false);
	void drawCheckBox(const QStyleOption *opt, QPainter *p);
	void drawRadioButton(const QStyleOption *opt, QPainter *p);
	void drawSymbol(const QStyleOption *opt, QPainter *p, Symbol s);
	void drawScrollBarEmptySpace(const QStyleOption *opt, QPainter *p);
	void drawScrollBarButton(const QStyleOption *opt, QPainter *p, Symbol s);
	void drawScrollBarHandle(const QStyleOption *opt, QPainter *p);
	void drawSlider(const QStyleOption *opt, QPainter *p, 
					const QRect &grooveRegion, const QRect &handle, const QRect &ticks);
	void drawComboBox(const QStyleOptionComplex *opt, QPainter *p);
	void drawComboBoxLabel(const QStyleOption *opt, QPainter *p);
	void drawSpinBox(const QStyleOptionComplex *opt, QPainter *p);
	void drawDial(const QStyleOption *opt, QPainter *p);
	void drawQ3ListView(const QStyleOption *opt, QPainter *p, const QWidget *w);
	void drawMenuItem(const QStyleOption *opt, QPainter *p, bool saveSpaceForIcon = true);
	void drawDockWindowTitle(const QStyleOption *opt, QPainter *p);
	void drawTabBarTab(const QStyleOption *opt, QPainter *p);
	void drawTitleBarButton(const QStyleOptionComplex *opt, QPainter *p, const QWidget *w, QStyle::SubControl control, QStyle::StandardPixmap icon);
	void drawTitleBar(const QStyleOptionComplex *opt, QPainter *p, const QWidget *w = 0);
	void drawBranch(const QStyleOption *opt, QPainter *p);
	void drawPanelItemViewItem(const QStyleOption *opt, QPainter *p, bool itemText = false);
	void drawPanelItemViewRow(const QStyleOption *opt, QPainter *p);
	QIcon standardIcon(QStyle::StandardPixmap icon, const QStyleOption *option, const QWidget *widget);

	bool hasIcon(QStyle::StandardPixmap icon);
protected:
	static ColorData *lookupData(const QPalette &pal);
	 
private:
	static QHash<QString, ColorData*> m_data;
	QStyle *m_parent;
};

inline void drawFrameCuttingOne(QPainter *p, const QRect &r, const QColor &c)
{
	int top = r.top(), left = r.left(), bottom = r.bottom(), right = r.right();
	p->setPen(QPen(c,1));
	p->drawLine(left+1, top, right-1, top); //top
	p->drawLine(left, top+1, left, bottom-1); //left
	p->drawLine(left+1, bottom, right-1, bottom); //bottom
	p->drawLine(right, top+1, right, bottom-1); // right*/
}

inline void fillRectCuttingOne(QPainter *p, const QRect &r, const QBrush &b)
{
	p->fillRect(r.adjusted(1,0,-1,0), b);
	p->setPen(QPen(b, 1));
	
	int top = r.top(), bottom = r.bottom(), left = r.left(), right = r.right();
	p->drawLine(left, top+1, left, bottom-1);
	p->drawLine(right, top+1, right, bottom-1);
}

#endif
