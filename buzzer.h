/* buzzer.h
 *
 * Copyright (c) 2017 Thomas Kais
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
  * \brief Query the Raspberry Pi Pin
  */
  auto virtual run() -> void override;

};
