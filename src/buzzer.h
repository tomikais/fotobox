/* buzzer.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef BUZZER_H
#define BUZZER_H

//detect wiringPi framework
#if defined (__arm__) && __has_include(<wiringPi.h>)
//wiringPi detected
#define BUZZER_AVAILABLE
#endif

#pragma once
#include <QObject>
#include <atomic>


/*!
 * \brief This class handles the buzzer
 */
class Buzzer : public QObject
{
  Q_OBJECT


public:
  /*!
  * \brief hide Buzzer constructor
  */
  explicit Buzzer();

  /*!
  * \brief hide Preferences destructor
  */
  ~Buzzer() override = default;

  /*!
  * \brief Buzzer copy constructor
  */
  Buzzer(const Buzzer&) = default;

  /*!
  * \brief Buzzer delete copy assignment
  */
  Buzzer& operator=(const Buzzer&) = default;

  /*!
  * \brief Buzzer default move constructor
  */
  Buzzer(Buzzer&& other) = default;

  /*!
  * \brief Buzzer default move assignment
  */
  Buzzer& operator=(Buzzer&& other) = default;

  /*!
  * \brief Stop executing \sa queryPin()
  */
  void stop();


public slots:
  /*!
  * \brief Query the Raspberry Pi pin
  */
  void queryPin();


signals:
  /*!
  * \brief Buzzer was pressed
  */
  void triggered();


private:
  /*!
  * atomic bool to stop /sa queryPin()
  */
  std::atomic<bool> m_stop;

};

#endif // BUZZER_H
