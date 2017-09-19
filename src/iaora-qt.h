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

#ifndef IAORA_QT_H
#define IAORA_QT_H

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QStylePlugin>

class IaOraStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "iaora.json")

public:
    QStringList keys() const;
    QStyle *create(const QString &key);
};

class IaOraStyle;

class Q_DECL_EXPORT IaOraQt : public QCommonStyle
{
    Q_OBJECT
public:
    IaOraQt();
    ~IaOraQt();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;

    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;

    virtual void drawItemText(QPainter *painter, const QRect &rect, int alignment, const QPalette &pal,
                              bool enabled, const QString& text, QPalette::ColorRole textRole) const;
    virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = 0) const;
    virtual int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const;
    virtual QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget = 0) const;

    virtual QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget = 0) const;
    virtual QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;

    void polish(QWidget *widget);
    void unpolish(QWidget *widget);

protected slots:
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const;

private:
    IaOraStyle *m_style;
};

#endif
