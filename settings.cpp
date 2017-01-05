/* settings.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "settings.h"


#include <QApplication>

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_settings(qApp->applicationName(), QSettings::IniFormat)
{

}
