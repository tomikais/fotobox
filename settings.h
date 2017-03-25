/* settings.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);


signals:

public slots:


private:
    QSettings m_settings;

};
