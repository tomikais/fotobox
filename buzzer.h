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


/*!
 * \brief This class handles the buzzer
 *  Run in a sperated thread.
 */
class Buzzer : public QThread
{
  Q_OBJECT

public:
  /*!
   * Buzzer constructor
   * \param parent QObject
   */
  explicit Buzzer(QObject* parent = nullptr);

  /*!
  * \brief Query the Raspberry Pi Pin in a loop.
  * interval: 10 mikroseconds
  */
  auto run() -> void Q_DECL_OVERRIDE;

};
