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

  /*!
  * \brief Query the Raspberry Pi Pin in a loop.
  * interval: 10 milliseconds
  * \details override method of QThread
  * \return void
  */
  auto run() -> void Q_DECL_OVERRIDE;

private:
  //GPIO 17 (=wiringPi pin 0)
  const unsigned int m_pinZero = 0;

  //GPIO 24 (=wiringPi pin 5)
  const unsigned int m_pinFive = 5;

  //intervall to wait before check PIN again
  const unsigned int m_queryIntervalMsecs = 10;

};
