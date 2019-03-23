/* buzzer.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef BUZZER_H
#define BUZZER_H

//detect wiringPi framework
#if defined (__arm__) && __has_include(<wiringPi.h>)
//wiringPi detected
#include <wiringPi.h>
  #if defined (__WIRING_PI_H__)
    //enable implementation
    #define BUZZER_AVAILABLE
  #endif
#endif


#if defined (BUZZER_AVAILABLE)
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
  * \brief Buzzer delete copy constructor (Singleton)
  */
  Buzzer(const Buzzer&) = delete;

  /*!
  * \brief Buzzer delete copy assignment (Singleton)
  */
  Buzzer& operator=(const Buzzer&) = delete;

  /*!
  * \brief Buzzer default move constructor
  */
  Buzzer(Buzzer&& other) = default;

  /*!
  * \brief Buzzer default move assignment
  */
  Buzzer& operator=(Buzzer&& other) = default;

  /*!
  * \brief get instance (Meyers Singleton)
  * \return Buzzer&
  */
  static Buzzer& instance();


public slots:
  /*!
   * \brief Query the Raspberry Pi pin
   */
  void queryPin();

  /*!
  * \brief Stop executing \sa queryPin()
  */
  void stop();


signals:
  /*!
   * \brief Buzzer was pressed
   */
  void triggered();


private:
  /*!
  * \brief hide Buzzer default constructor (Singleton)
  */
  explicit Buzzer();

  /*!
  * \brief hide Preferences destructor (Singleton)
  */
  ~Buzzer() override = default;


  /*!
  * atomic bool to stop /sa queryPin()
  */
  std::atomic<bool> m_stop;

};
#else
//Buzzer stub: fix Qt moc warning
#pragma once
#include <QObject>
class Buzzer : QObject { Q_OBJECT };
#endif

#endif // BUZZER_H
