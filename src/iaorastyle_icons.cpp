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
#include <QPixmapCache>
#include <QApplication>

bool IaOraStyle::hasIcon(QStyle::StandardPixmap icon)
{
    switch (icon) {
    case QStyle::SP_DockWidgetCloseButton:
    case QStyle::SP_TitleBarCloseButton:
    case QStyle::SP_TitleBarMaxButton:
    case QStyle::SP_TitleBarMinButton:
    case QStyle::SP_TitleBarContextHelpButton:
    case QStyle::SP_TitleBarNormalButton:
    case QStyle::SP_TitleBarShadeButton:
    case QStyle::SP_TitleBarUnshadeButton:
    case QStyle::SP_TitleBarMenuButton:
        return true;
    default:
        return false;
    }
}

QIcon IaOraStyle::standardIcon(QStyle::StandardPixmap icon, const QStyleOption *option, const QWidget *widget)
{
    // do some caching
    QPixmap pix;
    QImage img;
    QSize s(64, 64);
    QPalette pal = QApplication::palette();
    if (option) {
        pal = option->palette;
        s = option->rect.size();

        // handling the TitleBar icons
        switch (icon) {
        case QStyle::SP_TitleBarMinButton:
        case QStyle::SP_TitleBarMenuButton:
        case QStyle::SP_TitleBarMaxButton:
        case QStyle::SP_TitleBarCloseButton:
        case QStyle::SP_TitleBarNormalButton:
        case QStyle::SP_TitleBarShadeButton:
        case QStyle::SP_TitleBarUnshadeButton:
        case QStyle::SP_TitleBarContextHelpButton:
            s.scale(16, 16, Qt::IgnoreAspectRatio);
            break;
        default:
            (s.width() > s.height()) ? s.setHeight(s.width()) : s.setWidth(s.height()); // the icon was getting the wrong size of subtitles
        }
    }
    QString key = QString("iaora_standard_pixmap_%1_%2_%3").arg(QString::number(icon))
                  .arg(QString::number(s.width()))
                  .arg(QString::number(s.height()));
    if (QPixmapCache::find(key, pix))
        return pix;

    ColorData *data = lookupData(pal);

    int margin = s.height() * 0.20;

    //qDebug("Returning a pixmap of size %dx%d", s.width(), s.height());
    img = QImage(s, QImage::Format_ARGB32_Premultiplied);
    //img.fill(Qt::black); // it removes the blue ghost behind the icon
    img.fill(Qt::transparent);
    QPainter p(&img);
    QRect r = img.rect();
    r.adjust(margin, margin, -margin, -margin);
    int top = r.top(), bottom = r.bottom(), left = r.left(), right = r.right(), centerY = r.center().y();
    int width = r.width(), height = r.height();

    p.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient g(left, top, left, bottom);
    g.setColorAt(0., data->white);
    g.setColorAt(0.3, data->white);
    g.setColorAt(0.7, data->gray[2]);
    g.setColorAt(1., data->gray[2]);
    QBrush b(g);
    QPen pen(b, 2);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    switch (icon) {
    case QStyle::SP_DockWidgetCloseButton:
    case QStyle::SP_TitleBarCloseButton: {
        p.drawLine(left, top + 1, right, bottom); /*  \  */
        p.drawLine(right, top + 1, left, bottom); /*  /  */
        break;
    }

    case QStyle::SP_TitleBarMaxButton: {
        p.setRenderHint(QPainter::Antialiasing, false);
        p.setPen(QPen(b, 1));
        p.setBrush(Qt::NoBrush);
        p.drawRect(r);

        // draw one more line in the top
        p.drawLine(left, top + 1, right, top + 1);
        break;
    }
    case QStyle::SP_TitleBarMinButton: {
        pen.setWidth(3);
        p.setPen(pen);
        p.drawLine(left, centerY + 1, right, centerY + 1);
        break;
    }
    case QStyle::SP_TitleBarNormalButton: {
        pen.setWidth(1);
        p.setPen(pen);

        // for the Normal button, use a bit bigger rect
        r.adjust(-2, -2, 2, 2);
        QRect firstSquare(left + width * 0.35, top, width * 0.7, height * 0.6);
        QRect secondSquare(left, top + height * 0.35, width * 0.7, height * 0.6);

        p.setBrush(Qt::NoBrush);
        p.setRenderHint(QPainter::Antialiasing, false);
        p.setBackground(Qt::transparent);

        p.drawRect(firstSquare);

        // TODO: this is a hack, please use a better method for drawing the button
        QPainter::CompositionMode mode = p.compositionMode();
        p.setCompositionMode(QPainter::CompositionMode_Clear);
        p.fillRect(secondSquare, data->white);
        p.setCompositionMode(mode);


        p.drawRect(secondSquare);

        // and some extra-lines
        p.drawLine(firstSquare.topLeft() + QPoint(0, 1), firstSquare.topRight() + QPoint(0, 1));
        p.drawLine(secondSquare.topLeft() + QPoint(0, 1), secondSquare.topRight() + QPoint(0, 1));
        break;
    }
    case QStyle::SP_TitleBarContextHelpButton: {
        // just to identify cases where it appears
        QFont f = p.font();
        f.setPixelSize(r.height());
        p.setFont(f);
        p.drawText(r, Qt::AlignCenter, "?");
        break;
    }
    case QStyle::SP_TitleBarMenuButton: {
        p.drawLine(left, top + (height * .45), centerY, bottom);
        p.drawLine(centerY, bottom, right, top + (height * 0.3));
        break;
    }
    case QStyle::SP_TitleBarShadeButton:
    case QStyle::SP_TitleBarUnshadeButton: {
        // just to identify cases where it appears
        p.fillRect(r, Qt::magenta);
        break;
    }
    default:
        break;

    }
    pix = QPixmap::fromImage(img);
    QPixmapCache::insert(key, pix);
    return QIcon(pix);
}

