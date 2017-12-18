/* settings.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent),
  m_qsettings() //TODO constructor
{

}

/*
 * default:
 *  Buttons = bool
 *
 * buzzer:
 *  OutputPin = int
 *  InputPin = int
 *  QueryIntervall = int
 *
 * camera:
 *  gphotoArgument = QString
 *  timout = int
 *
 */

