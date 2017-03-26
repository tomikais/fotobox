/* buzzer.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QObject>
#include <QThread>

class Buzzer : public QThread
{
  Q_OBJECT
public:
  explicit Buzzer(QObject *parent = 0);

  void run() Q_DECL_OVERRIDE;

};
