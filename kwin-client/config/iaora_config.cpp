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

#include <KDE/KVBox>
#include <KDE/KGlobal>
#include <KDE/KLocale>
#include <KDE/KConfigGroup>

#include "iaora_config.h"

extern "C" KDE_EXPORT QObject *allocate_config(KConfig *conf, QWidget *parent)
{
    return new IaOraConfig(conf, parent);
}

IaOraConfig::IaOraConfig(KConfig *conf, QWidget *parent) : QObject(parent)
{
    Q_UNUSED(conf);

    KGlobal::locale()->insertCatalog("kwin_clients");

    iaoraConfig = new KConfig("iaorarc");
    KConfigGroup configGroup(iaoraConfig, "Windeco");

    m_ui = new IaOraConfigUI(parent);
    connect(m_ui->cmbBorderSize, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect(m_ui->cmbAlign, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    //connect(m_ui->chkGroup, SIGNAL(clicked()), SIGNAL(changed()));
    //connect(m_ui->chkCloseButtons, SIGNAL(clicked()), SIGNAL(changed()));

    load(configGroup);
    m_ui->show();
}

IaOraConfig::~IaOraConfig()
{
    delete m_ui;
    delete iaoraConfig;
}

// loads the configuration
void IaOraConfig::load(const KConfigGroup&)
{
    KConfigGroup configGroup(iaoraConfig, "Windeco");

    int alignment;
    int borderSize;
    //bool windowGrouping; // TABS
    //bool closeButtonsOnTabs; // TABS

    // 1 is normal (0 = Tiny; 2 = Large; 3 = Very Large; 4 = Huge; 5 = Very Huge; 6 = Oversized)
    borderSize = configGroup.readEntry("BorderSize", 1);
    m_ui->cmbBorderSize->setCurrentIndex(borderSize);

    // 1 is centered (0 = left; 2 = right)
    alignment = configGroup.readEntry("TitleBarTextAlignment", 1);
    m_ui->cmbAlign->setCurrentIndex(alignment);

    /*
    // TABS
    windowGrouping = configGroup.readEntry("UseWindowGrouping", false);
    m_ui->chkGroup->setChecked(windowGrouping);
    closeButtonsOnTabs = configGroup.readEntry("CloseButtonsOnTabs", true);
    m_ui->chkCloseButtons->setChecked(closeButtonsOnTabs);
    */
}

// we need to save the configurations
void IaOraConfig::save(KConfigGroup&)
{
    KConfigGroup configGroup(iaoraConfig, "Windeco");

    configGroup.writeEntry("BorderSize", m_ui->cmbBorderSize->currentIndex());
    configGroup.writeEntry("TitleBarTextAlignment", m_ui->cmbAlign->currentIndex());
    //configGroup.writeEntry("UseWindowGrouping", m_ui->chkGroup->isChecked());
    //configGroup.writeEntry("CloseButtonsOnTabs", m_ui->chkCloseButtons->isChecked());

    iaoraConfig->sync();
}

// simple the default configuration
void IaOraConfig::defaults()
{
    m_ui->cmbBorderSize->setCurrentIndex(1);
    m_ui->cmbAlign->setCurrentIndex(1);
    //m_ui->chkGroup->setChecked(false);
    //m_ui->chkCloseButtons->setChecked(true);
}

#include "iaora_config.moc"

