/*
 * kwin_iaora.cpp - IaOra window decoration for KDE
 *                  based on the Skulpture window decoration
 *
 * Copyright 2008 Gustavo Pichorim Boiko <boiko@mandriva.com>
 * Skulpture Copyright 2008 Christoph Feck
 * Copyright 2008-2010 Rodrigo Gonçalves de Oliveira <rodrigo@mandriva.com>
 *
 ** Window tabbing feature based on the original code from Tabstrip KWin window decoration
 ** Copyright (C) 2009 Jorge Mata <matamax123@gmail.com>
 ** Copyright (C) 2009 Lucas Murray <lmurray@undefinedfire.com>
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

#include "kwin_iaora.h"
#include <KDE/KLocale>
#include <KDE/KConfig>
#include <KDE/KConfigGroup>
#include <QtGui/QStylePainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleFactory>
#include <QtGui/QStyleOption>
#include <QtGui/QBitmap>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtGui/QStylePlugin>
#include <QSettings>
#include <QPixmapCache>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <cstdio>


/*-----------------------------------------------------------------------*/
/**
 * The IaOra window decoration for KDE actually is a wrapper.
 * It uses Qt's MDI window decoration. As such, it is very lightweight.
 *
 */

extern "C" KDE_EXPORT KDecorationFactory *create_factory()
{
	return new IaOraDecorationFactory();
}

inline QString hashPalette(const QPalette &pal)
{
	QString hash = pal.color(QPalette::Window).name();
	hash += pal.color(QPalette::Highlight).name();
	return hash;
}

ColorData *IaOraDecoration::lookupData(const QPalette &pal)
{
	QString hash = hashPalette(pal);

	// if already cached, just return
	//if (m_data.contains(hash))
	//	return m_data[hash];
	
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

	//m_data[hash] = data;
	return data;
}

/*-----------------------------------------------------------------------*/
/*
 * When KWin is already running, it doesn't notice that
 * a new style has been installed, and QStyleFactory will
 * not find the new style.
 *
 * This function works around this Qt problem by manually
 * loading the style.
 *
 */

static void setIaOraStyle(QWidget *widget)
{
	static QStyle *iaoraStyle = 0;
	QStyle *style = 0;

	if (!style && iaoraStyle) {
		style = iaoraStyle;
	}
	if (!style) {
		bool loaded = false;
		QStringList paths = QCoreApplication::libraryPaths();
		QLibrary library;
		foreach (QString path, paths) {
			library.setFileName(path + QString::fromUtf8("/styles/iaora-qt"));
			if (library.load()) {
				loaded = true;
				break;
			}
		}
		if (loaded) {
			QString libName = library.fileName();
			library.unload();

			QPluginLoader plugin(libName);
			QObject *instance = plugin.instance();
			if (instance) {
				QStyleFactoryInterface *factory = qobject_cast<QStyleFactoryInterface*>(instance);
				if (factory) {
					style = factory->create(QString::fromUtf8("iaora-qt"));
					if (style) {
						iaoraStyle = style;
					}
				}
			}
		}
	}
	if (style) {
		widget->setStyle(style);
	}
}


/*-----------------------------------------------------------------------*/

IaOraDecorationButton::IaOraDecorationButton(ButtonType type, KCommonDecoration *parent)
	: KCommonDecorationButton(type, parent)
{
	init();
}


void IaOraDecorationButton::init()
{
	setAttribute(Qt::WA_PaintOnScreen, false);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAutoFillBackground(false);
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_OpaquePaintEvent, false);
	setAttribute(Qt::WA_Hover, true);
	setIaOraStyle(this);
	m_isHover = false;
}


IaOraDecorationButton::~IaOraDecorationButton()
{
	/* */
}


void IaOraDecorationButton::reset(unsigned long /*changed*/)
{
	/* NOTE: must be implemented, because it is declared pure */
}


void IaOraDecorationButton::initStyleOption(QStyleOptionButton &opt)
{
	opt.init(this);
	if (isDown()) {
		opt.state |= QStyle::State_Selected;
	} else {
		opt.state &= ~QStyle::State_Selected;
	}
	if (decoration()->isActive()) {
		opt.state |= QStyle::State_Active;
	} else {
		opt.state &= ~QStyle::State_Active;
	}
}


void IaOraDecorationButton::paintEvent(QPaintEvent */* event */)
{
// TODO: must clear the cache when we change the color scheme (how???)
	const KCommonDecoration *deco = decoration();
	QPainter painter(this);
	QStyleOptionButton opt;
	initStyleOption(opt);

	QStyle::StandardPixmap sp;
	switch (type()) {
		case OnAllDesktopsButton:
			sp = QStyle::StandardPixmap(QStyle::SP_CustomBase + (deco->isOnAllDesktops() ? 2 : 1));
			break;
		case MenuButton:
			sp = QStyle::StandardPixmap(QStyle::SP_CustomBase + 3);
			break;
		case AboveButton:
			sp = QStyle::StandardPixmap(QStyle::SP_CustomBase + (deco->keepAbove() ? 6 : 4));
			break;
		case BelowButton:
			sp = QStyle::StandardPixmap(QStyle::SP_CustomBase + (deco->keepBelow() ? 7 : 5));
			break;
		case HelpButton:
			sp = QStyle::SP_TitleBarContextHelpButton;
			break;
		case MinButton:
			sp = QStyle::SP_TitleBarMinButton;
			break;
		case MaxButton:
			sp = deco->maximizeMode() == KDecoration::MaximizeFull ? QStyle::SP_TitleBarNormalButton : QStyle::SP_TitleBarMaxButton;
			break;
		case ShadeButton:
			sp = deco->isShade() ? QStyle::SP_TitleBarUnshadeButton : QStyle::SP_TitleBarShadeButton;
			break;
		case ItemCloseButton:
		case CloseButton:
			sp = QStyle::SP_TitleBarCloseButton;
			break;
		default:
			sp = QStyle::SP_CustomBase;
			break;
	}

	QIcon icon;
	int shift;
	
	// paint shape
	opt.rect = rect().adjusted(-1, 0, 0, 0);

	opt.palette.setColor(QPalette::Text, deco->options()->color(KCommonDecoration::ColorFont, opt.state & QStyle::State_Active));

	icon = sp == QStyle::SP_CustomBase + 3 ? deco->icon().pixmap(16, 16) : style()->standardIcon(sp, &opt, this);

	if (type() == ItemCloseButton){ // special case for a tab close button
		QPixmap pix;
		QString key = QString("iaora_standard_pixmap_tab_%1_%2_%3").arg(QString::number(sp))
								.arg(16)
								.arg(16);
		if (QPixmapCache::find(key, pix)){
			icon = QIcon(pix);
		} else{
			pix = QPixmap(16, 16);
			pix.fill(Qt::transparent);

			QImage image(pix.size(), QImage::Format_ARGB32_Premultiplied);
			image.fill(opt.palette.color(QPalette::WindowText).rgb());

			QPainter p2(&pix);
				p2.setCompositionMode(QPainter::CompositionMode_Source);
				p2.fillRect(pix.rect(), Qt::transparent);
				p2.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p2.drawPixmap(0, 0, icon.pixmap(16, 16));

				p2.setCompositionMode(QPainter::CompositionMode_SourceIn);
				p2.drawImage(0, 0, image);
			p2.end();

			QPixmapCache::insert(key, pix);
			icon = QIcon(pix);
		}
	}

	shift = opt.state & QStyle::State_Selected ? 1 : 0;
	
	// when mouse is over, we apply a darker effect
	QPixmap pix;
	if (m_isHover){
		pix = icon.pixmap(opt.rect.size());
		QPainter p;
		QPixmap temp(pix.size());

		temp.fill(Qt::transparent);
		p.begin(&temp);

			QRect r(pix.rect());
			p.drawPixmap(r, pix);

			p.setCompositionMode(QPainter::CompositionMode_DestinationIn);

			QColor color;
			color.setAlpha(0.7 * 255);
			p.fillRect(pix.rect(), color);

		p.end();
		pix = temp;
	} else
		pix = icon.pixmap(opt.rect.size());

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.drawPixmap(opt.rect.topLeft() + QPoint(shift, shift) + QPoint(0, (opt.rect.height() / 2) - (pix.height() / 2)), pix);
	//painter.drawPixmap(opt.rect.topLeft() + QPoint(shift, shift), pix);
	//qDebug("Rect: %d %d %dx%d", rect().left(), rect().top(), rect().width(), rect().height());
}

void IaOraDecorationButton::enterEvent(QEvent *e)
{
	Q_UNUSED(e);

	m_isHover = true;
	update();
}

void IaOraDecorationButton::leaveEvent(QEvent *e)
{
	Q_UNUSED(e);

	m_isHover = false;
	update();
}

/*-----------------------------------------------------------------------*/

IaOraDecoration::IaOraDecoration(KDecorationBridge *bridge, KDecorationFactory *factory)
	: KCommonDecorationUnstable(bridge, factory) // TABS
	//: KCommonDecoration(bridge, factory)
{
	/* */
	clicking = dragging = false;
	pressedButton = Qt::NoButton;

	loadConfig();
}


IaOraDecoration::~IaOraDecoration()
{
	/* */
}


QString IaOraDecoration::visibleName() const
{
	return i18n("IaOra");
}


void IaOraDecoration::initStyleOption(QStyleOption &opt)
{
	opt.init(widget());
	opt.fontMetrics = QFontMetrics(options()->font(isActive()));
	if (isActive()) {
		opt.state |= QStyle::State_Active;
	} else {
		opt.state &= ~QStyle::State_Active;
	}
}

// Loads the configuration from theme settings
void IaOraDecoration::loadConfig()
{
	KConfig *conf = new KConfig("iaorarc");
	KConfigGroup configGroup(conf, "Windeco");

	useWindowGrouping = configGroup.readEntry("UseWindowGrouping", false); // TABS
	closeButtonsOnTabs = configGroup.readEntry("CloseButtonsOnTabs", true); // TABS
	
	borderSize = configGroup.readEntry("BorderSize", 1);

	int alignFlag = configGroup.readEntry("TitleBarTextAlignment", 1);

	switch(alignFlag){
		case 0:
			titleBarTextAlign = Qt::AlignLeft | Qt::AlignVCenter;
			break;
		default:
		case 1:
			titleBarTextAlign = Qt::AlignCenter; // the default one
			break;
		case 2:
			titleBarTextAlign = Qt::AlignRight | Qt::AlignVCenter;
			break;
	}

	delete conf;
}

bool IaOraDecoration::decorationBehaviour(DecorationBehaviour behaviour) const
{
	switch (behaviour) {
		case DB_MenuClose:
		case DB_WindowMask:
		case DB_ButtonHide:
			return true;
		default:
			return KCommonDecoration::decorationBehaviour(behaviour);
	}
}


void IaOraDecoration::updateWindowShape()
{
	QStyleOptionTitleBar option;
	initStyleOption(option);
	QStyleHintReturnMask mask;

	if ((widget()->style())->styleHint(QStyle::SH_WindowFrame_Mask, &option, widget(), &mask)) {
		setMask(mask.region);
	} else if (maximizeMode() == MaximizeFull){ // we don't want mask when maximized
		QRegion mask2(0, 0, widget()->width()+1, widget()->height()+1);
		setMask(mask2);
	} else {
		int w = widget()->width();
		int h = widget()->height();

		QRegion mask2(1, 0, w-2, h);
		mask2 += QRegion(0, 1, 1, h-2);
		mask2 += QRegion(w-1, 1, 1, h-2);

		setMask(mask2);
	}
}


int IaOraDecoration::layoutMetric(LayoutMetric lm, bool respectWindowState, const KCommonDecorationButton *button) const
{
	bool border = !(maximizeMode()==MaximizeFull && !options()->moveResizeMaximizedWindows());

	if (border) {
		switch (lm) {
			case LM_BorderLeft:
			case LM_BorderRight:
			case LM_BorderBottom:
				return borderWidth;
			case LM_TitleBorderLeft:
			case LM_TitleBorderRight:
				return 4;
			case LM_TitleEdgeLeft:
			case LM_TitleEdgeRight:
				return 8;
			case LM_TitleEdgeTop:
				return borderWidth;
			case LM_ButtonHeight:
				return titleHeight;
			default:
				break;
		}
	} else {
		switch (lm) {
			case LM_BorderLeft:
			case LM_BorderRight:
				return 0;
			case LM_BorderBottom:
			case LM_TitleBorderLeft:
			case LM_TitleBorderRight:
			case LM_TitleEdgeLeft:
				return 2;
			case LM_TitleEdgeRight:
			case LM_TitleEdgeTop:
				return 0;
			case LM_ButtonHeight:
				return titleHeight;
			default:
				break;
		}
	}
	switch (lm) {
		case LM_TitleEdgeBottom:
			return 1;
		case LM_TitleHeight:
			return titleHeight + 2;
		case LM_ButtonWidth:
			return titleHeight;
		case LM_ButtonSpacing:
			return 0;
		case LM_ExplicitButtonSpacer:
			return 2;
		case LM_ButtonMarginTop:
			return 0;
		default:
			return KCommonDecoration::layoutMetric(lm, respectWindowState, button);
	}
}


KCommonDecorationButton *IaOraDecoration::createButton(ButtonType type)
{
	return new IaOraDecorationButton(type, this);
}


void IaOraDecoration::init()
{
	QWidget wid;
	setIaOraStyle(&wid);
	if (wid.style()) {
		QStyleOptionTitleBar option;
		option.init(&wid);
		option.fontMetrics = QFontMetrics(options()->font(true));
		borderWidth = wid.style()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth, &option, 0);
		titleHeight = wid.style()->pixelMetric(QStyle::PM_TitleBarHeight, &option, 0);
		noBorder = wid.style()->styleHint(QStyle::SH_TitleBar_NoBorder, &option, 0);
		autoRaise = wid.style()->styleHint(QStyle::SH_TitleBar_AutoRaise, &option, 0);
	} else {
		borderWidth = 4;
		titleHeight = 16;
		noBorder = false;
		autoRaise = false;
	}
	KConfig configFile(QString::fromAscii("kwiniaorarc"));
	KConfigGroup conf(&configFile, "General");
	coloredFrame = conf.readEntry("UseTitleBarBorderColors", false);
	contrastFrame = conf.readEntry("UseExtraContrastBorder", false);


	switch (borderSize) {
		case 0:	borderWidth = 2; break;				// Tiny
		default:
		case 1:	borderWidth = 3; break;				// Normal
		case 2:	borderWidth = 4; break;				// Large
		case 3:	borderWidth = 5; break;				// VeryLarge
		case 4:	borderWidth = titleHeight / 2; break;		// Huge
		case 5:	borderWidth = titleHeight * 3 / 4; break;	// VeryHuge
		case 6:	borderWidth = titleHeight; break;		// Oversized
	}
	/*switch (options()->preferredBorderSize(factory())) {
		case BorderTiny:	borderWidth = 2; break;
		default:
		case BorderNormal:	borderWidth = 3; break;
		case BorderLarge:	borderWidth = 4; break;
		case BorderVeryLarge:	borderWidth = 5; break;
		case BorderHuge:	borderWidth = titleHeight / 2; break;
		case BorderVeryHuge:	borderWidth = titleHeight * 3 / 4; break;
		case BorderOversized:	borderWidth = titleHeight; break;
	}*/
	if (contrastFrame) {
		borderWidth += 1;
	}
	KCommonDecoration::init();
	if (wid.style()) widget()->setStyle(wid.style());
	widget()->setAutoFillBackground(false);
	widget()->setAttribute(Qt::WA_NoSystemBackground, true);
	widget()->setAttribute(Qt::WA_OpaquePaintEvent, true);

	if (useWindowGrouping)
		widget()->setAcceptDrops(true); // TABS

	if (static_cast<const KDecorationUnstable *>(decoration())->compositingActive())
		widget()->setAttribute(Qt::WA_PaintOnScreen, false); // Kwin problem with composite
}


static inline QColor blend_color(const QColor &c0, const QColor &c1, qreal blend)
{
	int b = int(0.5 + 256.0 * blend);
	b = qMin(256, qMax(0, b));
	QRgb rgba0 = c0.rgba();
	QRgb rgba1 = c1.rgba();
	return QColor(
		qRed(rgba0) + (((qRed(rgba1) - qRed(rgba0)) * b) >> 8),
		qGreen(rgba0) + (((qGreen(rgba1) - qGreen(rgba0)) * b) >> 8),
		qBlue(rgba0) + (((qBlue(rgba1) - qBlue(rgba0)) * b) >> 8),
		qAlpha(rgba0) + (((qAlpha(rgba1) - qAlpha(rgba0)) * b) >> 8)
	);
}

// Helper function to draw a tab on the TitleBar
void IaOraDecoration::drawTab(QPainter &painter, const QRect &rect, ClientGroupItem &item, bool border, bool active, bool dragging)
{ // TABS
	QStyleOptionTab tab;
	initStyleOption(tab);

	if (dragging)
		tab.rect = rect.adjusted(0, 0, 2, 0);
	else
		tab.rect = rect.adjusted(0, 2, 0, 0);
	
	QPalette pal = tab.palette;
	pal.setCurrentColorGroup(active ? QPalette::Active : QPalette::Inactive);

	ColorData *data = lookupData(pal);
	
	int top = tab.rect.top(), bottom = tab.rect.bottom(), left = tab.rect.left(), right = tab.rect.right();
	
	if (active){
		QRect contents = tab.rect;
		QLine borders[3];
		QLine lightBlue[2];
		QLine darkBlue[2];
		
		// Tab decoration
		borders[0] = QLine(left, top + 4, left, bottom);
		if (dragging){
			borders[1] = QLine(right - 2, top + 4, right - 2, bottom);
			borders[2] = QLine(left, bottom, right - 2, bottom);
		} else
			borders[1] = QLine(right - 1, top + 4, right - 1, bottom);

		lightBlue[0] = QLine(left + 2, top, right - 3,top);
		lightBlue[1] = QLine(left + 1, top + 1, right - 2 , top + 1);
		
		darkBlue[0] = QLine(left, top + 2, right - 1, top + 2);
		darkBlue[1] = QLine(left, top + 3, right - 1, top + 3);
		
		// fill the contents
		if (dragging){
			painter.fillRect(contents, data->gray[0]);
			
		} else
			painter.fillRect(contents.adjusted(0, 4, -1, 0), data->gray[0]);
		
		// draw the gray borders
		painter.setPen(data->gray[4]);
		if (dragging){
			painter.drawLines(borders, 3);

			QPoint points[2];

			points[0] = QPoint(left + 1, bottom - 1);
			points[1] = QPoint(right - 3, bottom - 1);
			painter.drawPoints(points, 2);
		} else
			painter.drawLines(borders, 2);

		// the blue lines
		painter.setPen(data->color[2]);
		painter.drawLines(lightBlue, 2);
		painter.setPen(data->color[4]);
		painter.drawLines(darkBlue, 2);
	}
	else // inactive tab
	{
		QRect contents = tab.rect;
		
		QPoint points[2];
		QLine lines[4];

		// Tab decoration
		if (!dragging)
			top += 3;

		contents.setTop(top + 1);
		lines[0] = QLine(left + 2, top, right - 3, top); // top
		lines[1] = QLine(left, top + 2, left, bottom); // left
		lines[2] = QLine(right - 1, top + 2, right - 1, bottom); // right
		points[0] = QPoint(left + 1, top + 1);
		points[1] = QPoint(right - 2, top + 1);
		
		// draw the contents
		painter.setPen(Qt::NoPen);
		painter.setBrush(data->gray[1]);
		painter.drawRect(contents.adjusted(1, 1, -1, 0));
		painter.drawRect(contents.adjusted(2, 0, -2, 0));
		
		// draw the lines
		painter.setPen(data->gray[5]);
		painter.drawLines(lines, 4);
		painter.drawPoints(points, 2);
	}

	//--------------------------------------------------------

	// text part
	painter.save();
		int borderSpace = borderWidth;

		QRect labelRect(rect.adjusted(0, 4, 0, 0));
		if (titleBarTextAlign & Qt::AlignLeft)
			labelRect.adjust(3, 0, 0, 0);
		else if (titleBarTextAlign & Qt::AlignRight)
			labelRect.adjust(0, 0, -3, 0);

		if (closeButtonsOnTabs)
			labelRect.adjust(0, 0, -18, 0);

		QFontMetrics metrics(painter.font());
		QString string = metrics.elidedText(item.title(), Qt::ElideRight, labelRect.width());

		painter.setPen(tab.palette.color(QPalette::WindowText));
		painter.drawText(labelRect, titleBarTextAlign | Qt::TextSingleLine, string);
	painter.restore();
}

void IaOraDecoration::paintEvent(QPaintEvent */*event */)
{
	bool border = !(maximizeMode()==MaximizeFull && !options()->moveResizeMaximizedWindows());
	QStylePainter painter(widget());

	// draw the title bar
	QStyleOptionTitleBar option;
	initStyleOption(option);
	option.titleBarState = option.state;
	option.subControls = QStyle::SC_TitleBarLabel;

	QList<ClientGroupItem> tabList = clientGroupItems(); // TABS
	int tabCount = tabList.count(); // TABS

	int borderSpace = borderWidth;

	if (noBorder) {
		option.rect.setHeight(titleHeight);
	} else {
		option.rect.adjust(4, 4, -4, -4);
		option.rect.setHeight(titleHeight);
	}
	if (!border) {
		option.rect.adjust(-6, 0, 6, 0);
		borderSpace = 0;
	}

	option.text = QString();
	option.titleBarFlags = 0;
	option.rect.adjust(0, 0, 0, borderSpace);

	if (coloredFrame && border) {
		painter.fillRect(widget()->rect(), options()->color(ColorTitleBar, isActive()));
#if 0
		QColor color0 = option.palette.color(QPalette::Window);
		QColor color1 = options()->color(ColorTitleBar, isActive());
		QRect r = widget()->rect();
		for (int i = 0; i < borderWidth; ++i) {
			painter.setPen(blend_color(color1, color0, i / (borderWidth - 1.0)));
			painter.drawRect(r.adjusted(0, 0, -1, -1));
			r.adjust(1, 1, -1, -1);
		}
#endif
	} else {
		painter.fillRect(widget()->rect(), option.palette.color(QPalette::Window));

		painter.save();
			painter.setPen(Qt::white);
			painter.drawLine(widget()->rect().topLeft() + QPoint(0, titleHeight + borderSpace + 2), widget()->rect().topRight() + QPoint(0, titleHeight + borderSpace + 2));

			painter.setPen(option.palette.color(QPalette::Window));
			painter.drawLine(widget()->rect().topLeft() + QPoint(2, 0), widget()->rect().bottomLeft() + QPoint(2, 0));
			painter.drawLine(widget()->rect().topRight() - QPoint(2, 0), widget()->rect().bottomRight() - QPoint(2, 0));
			painter.drawLine(widget()->rect().bottomLeft() - QPoint(0, 2), widget()->rect().bottomRight() - QPoint(0, 2));

			// inner lines
			// #DFE7EF
			painter.setPen(QColor(223, 231, 239));
			painter.drawLine(widget()->rect().topLeft() + QPoint(1, 0), widget()->rect().bottomLeft() + QPoint(1, 0));
			painter.drawLine(widget()->rect().topRight() - QPoint(1, 0), widget()->rect().bottomRight() - QPoint(1, 0));
			painter.drawLine(widget()->rect().bottomLeft() - QPoint(0, 1), widget()->rect().bottomRight() - QPoint(0, 1));

			// outer lines
			// #CFD7DF
			painter.setPen(QColor(207, 215, 223));
			painter.drawLine(widget()->rect().topLeft() + QPoint(0, titleHeight + borderSpace + 1), widget()->rect().topRight() + QPoint(0, titleHeight + borderSpace + 1));
			painter.drawLine(widget()->rect().topLeft(), widget()->rect().bottomLeft());
			painter.drawLine(widget()->rect().topRight(), widget()->rect().bottomRight());
			painter.drawLine(widget()->rect().bottomLeft(), widget()->rect().bottomRight());
		painter.restore();
	}

	QFont font = options()->font(isActive());
	// Maybe we can add a checkbox for config to use fixed window title text size
	//font.setPointSize(10);
	font.setBold(true);
	painter.setFont(font);

	//if (tabCount <= 1){
		if (isActive()){
			option.palette.setColor(QPalette::Highlight, options()->color(ColorTitleBar, true));
			option.palette.setColor(QPalette::HighlightedText, options()->color(ColorFont, true));
			option.palette.setColor(QPalette::Window, options()->color(ColorTitleBar, false));
			option.palette.setColor(QPalette::WindowText, options()->color(ColorFont, false));
		} else{
			option.palette.setColor(QPalette::Inactive, QPalette::Highlight, options()->color(ColorTitleBar, false));
			option.palette.setColor(QPalette::Inactive, QPalette::HighlightedText, options()->color(ColorFont, false));
			option.palette.setColor(QPalette::Inactive, QPalette::Window, options()->color(ColorTitleBar, false));
			option.palette.setColor(QPalette::Inactive, QPalette::WindowText, options()->color(ColorFont, false));
		}
		painter.drawComplexControl(QStyle::CC_TitleBar, option);
	//}

	int captionWidth = width() - buttonsLeftWidth() - buttonsRightWidth() - (border ? 16 : 4);
	option.text = option.fontMetrics.elidedText(caption(), Qt::ElideMiddle, captionWidth);
	QRect labelRect = QRect((border ? 8 : 2) + buttonsLeftWidth(), 0, captionWidth, titleHeight + borderSpace + 4);

	labelRect.adjust(0, -2, 0, -2);
	if (titleBarTextAlign & Qt::AlignLeft)
		labelRect.adjust(3, 0, 0, 0);
	else if (titleBarTextAlign & Qt::AlignRight)
		labelRect.adjust(0, 0, -3, 0);

	QFontMetrics metrics(font);
	QString string = metrics.elidedText(option.text, Qt::ElideRight, labelRect.width());

	// deleting unneeded close buttons
	while ((tabCount < closeButtonsList.size()) || ((tabCount == 1) && (closeButtonsList.size() > 0))){
		IaOraDecorationButton *button = closeButtonsList.takeFirst();
		
		button->hide();
		button->deleteLater();
	}

	if (tabCount > 1){ // TABS
		//painter.fillRect(option.rect.adjusted(0, 0, 0, 2), option.palette.color(QPalette::Window));

		QRect tabRect(labelRect);
		tabRect.setWidth(tabRect.width() / tabCount + 1);
		int i;

		for (i = 0; i < tabCount; ++i){
			if (i == tabCount - 1)
				tabRect.adjust(0, 0, -3, 0);

			drawTab(painter, tabRect, tabList[i], border, isActive() && visibleClientGroupItem() == i);
			// create new close buttons
			if (closeButtonsOnTabs){
				if (i >= closeButtonsList.size())
					closeButtonsList.append(new IaOraDecorationButton(ItemCloseButton, this));
					//closeButtonsList.append(new IaOraDecorationButton(CloseButton, this));

				//closeButtonsList[i]->setActive(isActive() && visibleClientGroupItm() == i);
				closeButtonsList[i]->resize(16, 16);
				closeButtonsList[i]->move(tabRect.right() - 18, tabRect.top() + (tabRect.height() / 2) - 5);
				closeButtonsList[i]->installEventFilter(this);
				closeButtonsList[i]->show();
			}

			tabRect.translate(tabRect.width() - 1, 0);
		}
	} else{ // NO TABBED WINDOW
		// draw the title caption
		// TODO use MDI code
		painter.save();
		qreal opacity = painter.opacity();

		if (option.state & QStyle::State_Active){
			painter.setOpacity(opacity * 0.9);
			painter.setPen(Qt::black);
			painter.drawText(labelRect.adjusted(1, 1, 1, 1), titleBarTextAlign | Qt::TextSingleLine, string);
			painter.setOpacity(opacity);
		}

		painter.setPen(options()->color(ColorFont, isActive()));
	
		painter.drawText(labelRect, titleBarTextAlign | Qt::TextSingleLine, string);
		painter.restore();
	}
}

bool IaOraDecoration::eventFilter(QObject *object, QEvent *event) // TABS
{
	if (!useWindowGrouping)
		return KCommonDecorationUnstable::eventFilter(object, event);

	// this 'if' is for the tab close buttons
	if (IaOraDecorationButton *button = dynamic_cast<IaOraDecorationButton *>(object)){
		if (event->type() == QEvent::MouseButtonPress)
			return true;
		else if (event->type() == QEvent::MouseButtonRelease){
			const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

			if ((mouseEvent->button() == Qt::LeftButton) && (button->rect().contains(mouseEvent->pos())))
				closeClientGroupItem(closeButtonsList.indexOf(button));
		}
	}

	bool state = false;
	if (event->type() == QEvent::MouseButtonPress)
		state = mouseButtonPressEvent(static_cast<QMouseEvent*>(event));
	else if (event->type() == QEvent::MouseButtonRelease && widget() == object)
		state = mouseButtonReleaseEvent( static_cast<QMouseEvent*>(event));
	else if (event->type() == QEvent::MouseMove)
		state = mouseMoveEvent(static_cast<QMouseEvent*>(event));
	else if (event->type() == QEvent::DragEnter && widget() == object)
		state = dragEnterEvent(static_cast<QDragEnterEvent*>(event));
	else if (event->type() == QEvent::DragMove && widget() == object)
		state = dragMoveEvent(static_cast<QDragMoveEvent*>(event));
	else if (event->type() == QEvent::DragLeave && widget() == object)
		state = dragLeaveEvent(static_cast<QDragLeaveEvent*>(event));
	else if (event->type() == QEvent::Drop && widget() == object)
		state = dropEvent(static_cast<QDropEvent*>(event));

	return state || KCommonDecorationUnstable::eventFilter(object, event);
}

int IaOraDecoration::itemClicked(const QPoint &point, bool between) // TABS
{
	QRect frame = widget()->frameGeometry();
	QList<ClientGroupItem> list = clientGroupItems();
	int tabs = list.count();
	int t_x = titleRect().x();
	int t_y = frame.y();
	int t_w = titleRect().width();
	int t_h = layoutMetric(LM_TitleEdgeTop) + layoutMetric(LM_TitleHeight) + layoutMetric(LM_TitleEdgeBottom);
	int tabWidth = t_w / tabs;

	if (between) // inserting a new tab between two existing ones
		t_x -= tabWidth / 2;

	int rem = t_w % tabs;
	int tab_x = t_x;

	int i;
	for (i = 0; i < tabs; ++i){
		QRect tabRect(tab_x, t_y, (i < rem) ? tabWidth + 1 : tabWidth, t_h);

		if (tabRect.contains(point))
			return i;
	
		tab_x += tabRect.width();
	}
	
	return -1;
}

bool IaOraDecoration::mouseButtonPressEvent(QMouseEvent *event) // TABS
{
	clickPos = widget()->mapToParent(event->pos());
	
	int item = itemClicked(clickPos);
	
	if (buttonToWindowOperation(event->button()) == OperationsOp){
		displayClientMenu(item, widget()->mapToGlobal(clickPos));
		
		return true;
	}
	
	if (item >= 0){
		clicking = true;
		pressedButton = event->button();

		return true;
	}
	
	clicking = false;
	return false;
}

bool IaOraDecoration::mouseButtonReleaseEvent(QMouseEvent *event) // TABS
{
	releasePos = event->pos();
	
	int item = itemClicked(releasePos);
	
	if ((clicking) && (item >= 0)){
		clicking = false;
		setVisibleClientGroupItem(item);
		
		return true;
	}

	clicking = false;
	return false;
}

bool IaOraDecoration::mouseMoveEvent(QMouseEvent *event) // TABS
{
	QPoint point = event->pos();
	int item = itemClicked(point);
	
	if ((item >= 0) && (clicking) &&
			(buttonToWindowOperation(pressedButton) == ClientGroupDragOp) &&
			((point - clickPos).manhattanLength()) >= 4){
		clicking = false;
		dragging = true;

		QDrag *drag = new QDrag(widget());
		QMimeData *groupData = new QMimeData();

		groupData->setData(clientGroupItemDragMimeType(), QString().setNum(itemId(item)).toAscii());
		drag->setMimeData(groupData);

		// Create draggable tab pixmap
		QList<ClientGroupItem> tabList = clientGroupItems();
		const int tabCount = tabList.count();
		QRect frame(QPoint(0, 0), widget()->frameGeometry().size());
		QRect titlebar(frame.topLeft(),
				QSize(frame.width(), layoutMetric(LM_TitleEdgeTop) +
				layoutMetric(LM_TitleHeight) +
				layoutMetric(LM_TitleEdgeBottom) - 1));
		QRect geom = titleRect().adjusted(-1, -layoutMetric(LM_TitleEdgeTop), 1, 0);

		geom.setWidth((geom.width() / tabCount) + 1); // Split titlebar evenly
		geom.translate((geom.width() * item) - item, 0);

		QPixmap pix(geom.size());
		QPainter painter(&pix);

		drawTab(painter, QRect(QPoint(0, 0), geom.size()), tabList[item], true, isActive() && visibleClientGroupItem() == item, true);
		painter.end();

		QPixmap pix2(geom.size());
		QPainter painter2(&pix2);

		pix2.fill(Qt::transparent);
		painter2.setPen(Qt::red);
		
		int x = 0, y = 0, w = geom.width(), h = geom.height();
		// top-left
		painter2.drawLine(x, y, x + 1, y);
		painter2.drawLine(x, y, x, y + 1);
		// top-right
		painter2.drawLine(w - 2, y, w, y);
		painter2.drawLine(w - 1, y, w - 1, y + 1);
		// bottom-left
		painter2.drawLine(x, h - 2, x, h - 1);
		painter2.drawLine(x, h - 1, x + 1, h - 1);
		// bottom-right
		painter2.drawLine(w - 2, h - 1, w, h - 1);
		painter2.drawLine(w - 1, h - 2, w - 1, h - 1);

		pix.setMask(pix2.createMaskFromColor(Qt::red));

		drag->setPixmap(pix);

		drag->setHotSpot(QPoint(point.x() - geom.x(), -1));

		drag->exec(Qt::MoveAction);
		dragging = false;

		if ((drag->target() == 0) && (tabList.count() > 1)){
			QPoint pos = QCursor::pos();
			
			frame.moveTo(pos.x() - point.x(), pos.y() - point.y());
			removeFromClientGroup(itemClicked(clickPos), frame);
		}
		
		return true;
	}

	return false;
}

bool IaOraDecoration::dragEnterEvent(QDragEnterEvent *event) // TABS
{
	if ((event->source() != 0) && (event->source()->objectName() == "decoration widget")){
		dragging = true;
		event->acceptProposedAction();
		
		QPoint point = widget()->mapToParent(event->pos());
		targetTab = itemClicked(point);
		widget()->update();
		
		return true;
	}
	
	return false;
}

bool IaOraDecoration::dragMoveEvent(QDragMoveEvent *event) // TABS
{
	Q_UNUSED(event)

	return false;
}

bool IaOraDecoration::dragLeaveEvent(QDragLeaveEvent *event) // TABS
{
	Q_UNUSED(event)

	return false;
}

bool IaOraDecoration::dropEvent(QDropEvent *event) // TABS
{
	QPoint point = widget()->mapToParent(event->pos());
	dragging = false;
	int tabClick = itemClicked(point);

	if (tabClick >= 0){
		const QMimeData *groupData = event->mimeData();
		
		if (groupData->hasFormat(clientGroupItemDragMimeType())){
			if (widget() == event->source()){
				int from = itemClicked(clickPos);

				moveItemInClientGroup(from, itemClicked(point, true));
			} else{
				long source = QString(groupData->data(clientGroupItemDragMimeType())).toLong();

				moveItemToClientGroup(source, itemClicked(point, true));
			}
			
			return true;
		}
	}

	return false;
}

/*-----------------------------------------------------------------------*/

IaOraDecorationFactory::IaOraDecorationFactory()
{
	loadConfig();
}


IaOraDecorationFactory::~IaOraDecorationFactory()
{
	/* */
}

bool IaOraDecorationFactory::loadConfig()
{
	bool changed = false; // if no changes, just return false
	bool wGrouping, cboTabs;
	int alignment, brdrSize;

	KConfig *conf = new KConfig("iaorarc");
	KConfigGroup configGroup(conf, "Windeco");

	brdrSize = configGroup.readEntry("BorderSize", 1);
	alignment = configGroup.readEntry("TitleBarTextAlignment", 1);
	wGrouping = configGroup.readEntry("UseWindowGrouping", false); // TABS
	cboTabs = configGroup.readEntry("CloseButtonsOnTabs", true); // TABS

	if (brdrSize != borderSize){
		borderSize = brdrSize;
		changed = true;
	}
	if (alignment != alignFlag){
		alignFlag = alignment;
		changed = true;
	}
	if (wGrouping != useWindowGrouping){
		useWindowGrouping = wGrouping;
		changed = true;
	}
	if (cboTabs != closeButtonsOnTabs){
		closeButtonsOnTabs = cboTabs;
		changed = true;
	}

	delete conf;

	return changed;
}

KDecoration *IaOraDecorationFactory::createDecoration(KDecorationBridge *bridge)
{
	return (new IaOraDecoration(bridge, this))->decoration();
}


bool IaOraDecorationFactory::reset(unsigned long changed)
{
	if (loadConfig())
		return true;
	else{
		resetDecorations(changed);
		return false;
	}
}


/*QList<IaOraDecorationFactory::BorderSize> IaOraDecorationFactory::borderSizes() const
{
	return QList<BorderSize>()
			<< BorderTiny
			<< BorderNormal
			<< BorderLarge
			<< BorderVeryLarge
			<< BorderHuge
			<< BorderVeryHuge
			<< BorderOversized;
}*/


bool IaOraDecorationFactory::supports(Ability ability) const
{
	switch (ability)
	{
		case AbilityAnnounceButtons:
		case AbilityButtonMenu:
		case AbilityButtonHelp:
		case AbilityButtonShade:
		case AbilityButtonMinimize:
		case AbilityButtonMaximize:
		case AbilityButtonClose:
		case AbilityButtonSpacer:
		case AbilityButtonOnAllDesktops:
		case AbilityButtonAboveOthers:
		case AbilityButtonBelowOthers:
			return true;
		case AbilityAnnounceColors:
		case AbilityColorTitleBack:
		case AbilityColorTitleFore:
			return true;
		case AbilityClientGrouping: // TABS
			return useWindowGrouping;
		default:
			return false;
	}
}


/*-----------------------------------------------------------------------*/

#include "kwin_iaora.moc"


