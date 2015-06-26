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

#include "iaora-kde.h"
#include "iaorastyle.h"

#include <QStyleOption>
#include <QStyleOptionSpinBox>
#include <QPainter>
#include <QComboBox>
#include <QAbstractSpinBox>
#include <QToolButton>
#include <QDockWidget>
#include <QDebug>
#include <QToolBar>

K_EXPORT_STYLE("IaOraKde", IaOraKde)

IaOraKde::IaOraKde()
{
	// not use 2 buttons on ScrollBar bottom
	setWidgetLayoutProp(WT_ScrollBar, ScrollBar::DoubleBotButton, 0);
    // We set the minimum size to 21 for the scrollbar slider like in the GTK ia ora
    setWidgetLayoutProp(WT_ScrollBar, ScrollBar::MinimumSliderHeight, 21);    
	
    // correct the CheckBox and RadioButton sizes, according IaOraQt
	setWidgetLayoutProp(WT_CheckBox, CheckBox::Size, 12);
	setWidgetLayoutProp(WT_RadioButton, RadioButton::Size, 12);

	m_style = new IaOraStyle(this);
}

IaOraKde::~IaOraKde()
{
	delete m_style;
}

void IaOraKde::polish(QWidget *widget){
	// here we change the appearence of some widget
	if (qobject_cast<QToolButton *>(widget)){
		widget->setAttribute(Qt::WA_Hover, true); // forcing the mouse enter and leave event
	}
}
void IaOraKde::unpolish(QWidget *widget){
	if (qobject_cast<QToolButton *>(widget)){
		widget->setAttribute(Qt::WA_Hover, false);
	}
}

void IaOraKde::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	switch (element)
	{
		case QStyle::PE_FrameFocusRect:
			m_style->drawFrame(option, painter, IaOraStyle::FrameFocus);
			break;
		case QStyle::PE_Frame:
			QComboBox *cmb;
	
			if (widget)
				cmb = qobject_cast<QComboBox *>(widget->parentWidget());

			m_style->drawFrame(option, painter, (cmb) ? IaOraStyle::FrameComboBox : IaOraStyle::FrameFrame);

			break;
		case QStyle::PE_FrameWindow:
			m_style->drawFrame(option, painter);
			break;
		case QStyle::PE_FrameMenu:
			m_style->drawFrame(option, painter, IaOraStyle::FrameMenu);
			break;
		case QStyle::PE_FrameTabWidget:
			 m_style->drawFrame(option, painter, IaOraStyle::FrameTabBar);
			 break;
		case QStyle::PE_PanelLineEdit:
			 {
				if (widget && widget->parentWidget() && !qstrcmp(widget->parentWidget()->metaObject()->className(), "KHBox")){
					QRect r(option->rect.adjusted(0, 0, 0, -2));
					m_style->drawFrame(option, painter, IaOraStyle::FrameLineEdit, r);
				} else
					m_style->drawFrame(option, painter, IaOraStyle::FrameLineEdit);
			 }
			break;
		case QStyle::PE_PanelToolBar:
			m_style->drawFrame(option, painter, IaOraStyle::FrameToolBar);
			break;
		case QStyle::PE_PanelMenuBar:
			m_style->drawFrame(option, painter, IaOraStyle::FrameMenuBar);
			break;
		case QStyle::PE_FrameGroupBox:
			m_style->drawFrame(option, painter, IaOraStyle::FrameGroupBox);
			break;
		case QStyle::PE_IndicatorToolBarSeparator:
			m_style->drawSeparator(option, painter);
			break;
		case QStyle::PE_IndicatorToolBarHandle:
			m_style->drawHandle(option, painter);
			break;
		case QStyle::PE_PanelButtonCommand:
			m_style->drawButton(option, painter);
			break;
		case QStyle::PE_FrameDefaultButton:
			m_style->drawButtonDefaultBorder(option, painter);
			break;
		case QStyle::PE_PanelButtonTool:
			if (widget && !qstrcmp(widget->metaObject()->className(), "QDockWidgetTitleButton"))
				break;
			m_style->drawButton(option, painter);
			break;
		case QStyle::PE_IndicatorCheckBox:
			m_style->drawCheckBox(option, painter);
			break;
		case QStyle::PE_IndicatorRadioButton:
			m_style->drawRadioButton(option, painter);
			break;
		case QStyle::PE_PanelButtonBevel:
			m_style->drawButton(option, painter, true /*simplified*/);
			break;
		case QStyle::PE_IndicatorArrowUp:
		case QStyle::PE_IndicatorSpinUp:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolArrowUp);
			break;
		case QStyle::PE_IndicatorArrowDown:
		case QStyle::PE_IndicatorSpinDown:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolArrowDown);
			break;
		case QStyle::PE_IndicatorArrowLeft:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolArrowLeft);
			break;
		case QStyle::PE_IndicatorArrowRight:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolArrowRight);
			break;
		case QStyle::PE_IndicatorSpinPlus:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolPlus);
			break;
		case QStyle::PE_IndicatorSpinMinus:
			m_style->drawSymbol(option, painter, IaOraStyle::SymbolMinus);
			break;
		case QStyle::PE_IndicatorBranch:
			m_style->drawBranch(option, painter);
			break;
		case QStyle::PE_PanelItemViewItem:
			m_style->drawPanelItemViewItem(option, painter);
			break;
		case QStyle::PE_IndicatorDockWidgetResizeHandle:
		{
			// the horizontal / vertical flags are inverted
			QStyleOption copy = *option;
			if (copy.state & QStyle::State_Horizontal)
				copy.state &= ~QStyle::State_Horizontal;
			else
				copy.state |= QStyle::State_Horizontal;
			m_style->drawHandle(&copy, painter);
			break;
		}
		case QStyle::PE_IndicatorButtonDropDown:
			//m_style->drawButton(option, painter);
			break;
		case QStyle::PE_PanelItemViewRow:
			m_style->drawPanelItemViewRow(option, painter);
			break;
		default:
			//qDebug("Element: %d", element);
			KStyle::drawPrimitive(element, option, painter, widget);
	}
}

void IaOraKde::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	switch (element)
	{
		case QStyle::CE_ToolBar:
			m_style->drawFrame(option, painter, IaOraStyle::FrameMenuBar);
			break;
		case QStyle::CE_MenuBarEmptyArea:
			m_style->drawFrame(option, painter, IaOraStyle::FrameMenuBar);
			break;
		case QStyle::CE_ProgressBarGroove:
			m_style->drawFrame(option, painter, IaOraStyle::FrameProgressBar);
			break;
		case QStyle::CE_ProgressBarContents:
			m_style->drawProgressBarContents(option, painter, true);
			break;
		case QStyle::CE_ScrollBarSubLine:
		case QStyle::CE_ScrollBarAddLine:
			{
				const QStyleOptionSlider* scr = qstyleoption_cast<const QStyleOptionSlider*>(option);
				bool vertical = (scr->orientation == Qt::Vertical);
				bool addLine = (element == QStyle::CE_ScrollBarAddLine) ? true : false;
				QRect scrRect = scr->rect;
				QRect r;

				if (addLine){
					if (vertical)
						r = QRect(0, scrRect.bottom() - 15, scrRect.width(), 16);
					else
						r = QRect(scrRect.right() - 15, scrRect.top(), 16, scrRect.height());
				} else{
					if (vertical)
						r = QRect(scrRect.left(), scrRect.top(), scrRect.width(), 16);
					else
						r = QRect(0, scrRect.top(), 16, scrRect.height());
				}

				const_cast<QStyleOption*>(option)->rect = r;

				m_style->drawScrollBarButton(option, painter, (addLine) ? IaOraStyle::SymbolArrowRight : IaOraStyle::SymbolArrowLeft);
			}
			break;
		case QStyle::CE_ScrollBarAddPage:
		case QStyle::CE_ScrollBarSubPage:
			m_style->drawScrollBarEmptySpace(option, painter);
			break;
		case QStyle::CE_ScrollBarFirst:
			painter->fillRect(option->rect, Qt::gray);
			break;
		case QStyle::CE_ScrollBarLast:
			painter->fillRect(option->rect, Qt::gray);
			break;
		case QStyle::CE_ScrollBarSlider:
			m_style->drawScrollBarHandle(option, painter);
			break;
		case QStyle::CE_HeaderSection:
		case QStyle::CE_HeaderEmptyArea:
			m_style->drawFrame(option, painter, IaOraStyle::FrameHeader);
			break;
		case QStyle::CE_MenuEmptyArea:
			break;
		case QStyle::CE_MenuBarItem:
			m_style->drawMenuItem(option, painter, false);
			break;
		case QStyle::CE_MenuItem:
			m_style->drawMenuItem(option, painter);
			break;
		case QStyle::CE_Splitter:
			m_style->drawHandle(option, painter);
			break;
		case QStyle::CE_DockWidgetTitle:
			m_style->drawDockWindowTitle(option, painter);
			break;
		case QStyle::CE_TabBarTab:
			m_style->drawTabBarTab(option, painter);
			break;
		case QStyle::CE_ComboBoxLabel:
			m_style->drawComboBoxLabel(option, painter);
			break;
		case QStyle::CE_ItemViewItem:
			m_style->drawPanelItemViewItem(option, painter, true);
			break;
		default:
			//qDebug("ControlElement: %d", element);
			KStyle::drawControl(element, option, painter, widget);
	}
}

int IaOraKde::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
	switch (metric)
	{
		case QStyle::PM_ButtonDefaultIndicator:
			return 1;
			break;
		case QStyle::PM_ButtonShiftHorizontal:
		case QStyle::PM_ButtonShiftVertical:
			return 0;
		case QStyle::PM_DefaultFrameWidth:
		case QStyle::PM_ComboBoxFrameWidth:
			return 1;
		case QStyle::PM_SliderControlThickness:
			return 8;
		case QStyle::PM_SliderLength:
			return 26;
		case QStyle::PM_SliderTickmarkOffset:
			return 5;
		case QStyle::PM_SplitterWidth:
			return 8;
		case QStyle::PM_DockWidgetTitleBarButtonMargin:
		case QStyle::PM_DockWidgetTitleMargin:
			return 0;
		case QStyle::PM_ToolBarIconSize:
			return 24;
		case QStyle::PM_MenuBarItemSpacing:
			return 5;
		case QStyle::PM_SliderSpaceAvailable:
			if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(option)) 
			{
				int size = 15;
				if (slider->tickPosition & QSlider::TicksBelow)
					++size;
				if (slider->tickPosition & QSlider::TicksAbove)
					++size;
				return size;
			}
		case QStyle::PM_ScrollView_ScrollBarSpacing:
			return -1;
		// very strange issue
		/*case QStyle::PM_ScrollBarSliderMin:
			return 31;*/
		default:
			//qDebug(); // do not remove this for now (horrible hack)
			return KStyle::pixelMetric(metric, option, widget);
	}
}

int IaOraKde::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
	switch (hint)
	{
		case QStyle::SH_TitleBar_NoBorder:
			return true;
		case SH_MenuBar_MouseTracking:
		case SH_Menu_MouseTracking:
			return true;
		case SH_ScrollBar_MiddleClickAbsolutePosition:
			return true;
		case SH_ScrollView_FrameOnlyAroundContents:
			return true;
		default:
			return KStyle::styleHint(hint, option, widget, returnData);
	}
}

QSize IaOraKde::sizeFromContents (ContentsType type, const QStyleOption * option, const QSize & contentsSize, const QWidget *widget) const
{
	switch (type)
	{
		case QStyle::CT_MenuBarItem:
			return contentsSize + QSize(8,4);
		case CT_MenuItem:
			{
			const QStyleOptionMenuItem *menuOpt = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
			if (menuOpt)
				return contentsSize + QSize(46, 4);
			}
			break;
		case CT_ComboBox:
			return contentsSize + QSize(32, 0);
		default:
			return KStyle::sizeFromContents(type, option, contentsSize, widget);
	}
	// just for the war
	return KStyle::sizeFromContents(type, option, contentsSize, widget);
}

void IaOraKde::drawItemText(QPainter *painter, const QRect &rect, int alignment, const QPalette &pal,
					  bool enabled, const QString& text, QPalette::ColorRole textRole) const
{
	m_style->drawItemText(painter, rect, alignment, pal, enabled, text, textRole);
}

void IaOraKde::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
	QRect r = option->rect;
	switch(control)
	{
		case QStyle::CC_SpinBox:
		{
			m_style->drawSpinBox(option, painter);
			break;
		}
		case CC_ComboBox:
		{
			m_style->drawComboBox(option, painter);
			break;
		}
		case CC_Slider:
		{
			QRect grooveRegion = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
			QRect handle = subControlRect(CC_Slider, option, SC_SliderHandle, widget);
			QRect ticks = subControlRect(CC_Slider, option, SC_SliderTickmarks, widget);
			
			m_style->drawSlider(option, painter, grooveRegion, handle, ticks);
			break;
		}
		case CC_Dial:
		{
			m_style->drawDial(option, painter);
			break;
		}
		case CC_TitleBar:
		{
			m_style->drawTitleBar(option, painter, widget);
			break;
		}
		case CC_Q3ListView:
		{  	
			m_style->drawQ3ListView(option, painter, widget);
			break;
		}
		//case CC_ScrollBar: // this will not be drawed here
		case CC_ToolButton:
		{	
			QDockWidget *dockWidget;
			QTabBar *tabBar;
			if (widget)
			{
				dockWidget = qobject_cast<QDockWidget *>(widget->parentWidget());
				tabBar = qobject_cast<QTabBar *>(widget->parentWidget());
			}

			if (dockWidget){ // the QDockWidget icons
				const QStyleOptionToolButton *opt = qstyleoption_cast <const QStyleOptionToolButton *>(option);
				bool sunken = (opt->state & QStyle::State_Sunken);
				bool hover = (opt->state & QStyle::State_MouseOver);

				QIcon::Mode mode = (opt->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
				if (hover){
					mode = QIcon::Selected;
				}

				int shift = 0;
				if (sunken) shift = 2;

				QPixmap pix = opt->icon.pixmap(opt->iconSize, mode);
				QRect iconRect(r);

				iconRect = alignedRect(opt->direction, Qt::AlignCenter, iconRect.size(), r);
				iconRect.setTopLeft(iconRect.topLeft() + QPoint(shift, shift));

				drawItemPixmap(painter, iconRect, Qt::AlignCenter, pix);
			} else if (tabBar){ // we have tab arrows indicators?
				const QStyleOptionToolButton *opt = qstyleoption_cast <const QStyleOptionToolButton *>(option);
				QRect toolButtonRect = opt->rect;

				bool vertical = false;
				if (opt->features && QStyleOptionToolButton::Arrow)
					if ((opt->arrowType == Qt::LeftArrow) || (opt->arrowType == Qt::RightArrow)) vertical = true;

				painter->save();
					painter->setClipRect(toolButtonRect);
					painter->fillRect(toolButtonRect, option->palette.brush(QPalette::Window));

					m_style->drawToolButton(option, painter);
				painter->restore();
				
				if (vertical)
					m_style->drawSymbol(option, painter, (opt->arrowType == Qt::LeftArrow) ? IaOraStyle::SymbolArrowLeft : IaOraStyle::SymbolArrowRight);
				else
					m_style->drawSymbol(option, painter, (opt->arrowType == Qt::UpArrow) ? IaOraStyle::SymbolArrowUp : IaOraStyle::SymbolArrowDown);
			} else // if not...
				m_style->drawToolButton(option, painter);
			break;
		}
		default:
			KStyle::drawComplexControl(control, option, painter, widget);
	}
}

QIcon IaOraKde::standardIconImplementation(QStyle::StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const
{
	if (m_style->hasIcon(standardIcon))
		return m_style->standardIcon(standardIcon, option, widget);
	
	return KStyle::standardIconImplementation(standardIcon, option, widget);
}

QRect IaOraKde::subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
	QRect r = KStyle::subControlRect(control, option, subControl, widget);
	
	switch (control)
	{
		case QStyle::CC_SpinBox:
			if (subControl == QStyle::SC_SpinBoxEditField)
				r.adjust(-1,-1,1,1);
			break;
		case QStyle::CC_ComboBox:
			if (subControl == QStyle::SC_ComboBoxEditField)
				r.adjust(-1,-1,0,0);
			break;
		default:
			break;
	}
	
	return r;
}

QRect IaOraKde::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
	QRect r = KStyle::subElementRect(element, option, widget);

	switch (element)
	{
		case SE_LineEditContents:
			if (widget->parentWidget()){
				if (!qstrcmp(widget->parentWidget()->metaObject()->className(), "KHBox"))
					r.adjust(0,-1,0,0);
				else
					r.adjust(3,1,-1,0);
			}
			break;
		default:
			break;
	}

	return r;
}

#include "iaora-kde.moc"
