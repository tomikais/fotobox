/* buzzer.h
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QThread>


/*!
 * \brief This class handles the buzzer
 *  Run in a separate thread.
 */
class Buzzer : public QThread
{
  Q_OBJECT


public:
  /*!
   * Buzzer constructor
   * \param parent QObject
   */
  explicit Buzzer(QObject *parent = nullptr);


private:
  /*!
  * \brief Query the Raspberry Pi pin
  */
  auto run() -> void override;

};
