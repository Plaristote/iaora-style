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

#ifndef IAORA_CONFIG_H
#define IAORA_CONFIG_H

#include <KConfig>
#include <QtCore/QObject>

#include "ui_iaora_config.h"

class QWidget;
class KConfig;
class KConfigGroup;

class IaOraConfigUI : public QWidget, public Ui_IaOraConfigDialog
{
public:
    IaOraConfigUI(QWidget *parent) : QWidget(parent)
    {
        setupUi(this);
    }
};

class IaOraConfig : public QObject
{
    Q_OBJECT

public:
    IaOraConfig(KConfig *conf, QWidget *parent);
    virtual ~IaOraConfig();

signals:
    void changed();

public slots:
    void load(const KConfigGroup&);
    void save(KConfigGroup&);
    void defaults();

private:
    IaOraConfigUI *m_ui;
    KConfig *iaoraConfig;
};

#endif

