/* countdown.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#pragma once
#include <QObject>

class QTimer;


/*!
* \brief The Countdown class
* \details Provides a countdown which operates on the base of seconds
*/
class Countdown : public QObject
{
  Q_OBJECT


public:
  /*!
  * \brief Countdown constructor
  * \param parent QObject
  */
  explicit Countdown(QObject *parent = nullptr);

  /*!
  * \brief Countdown constructor
  * \param parent QObject
  * \param i_seconds set the start time \sa m_startTime
  */
  explicit Countdown(QObject *parent = nullptr, const unsigned int i_seconds = 0);

  /*!
  * \brief Countdown destructor
  */
  ~Countdown() override;

  /*!
  * \brief Set the countdown start time
  * \param i_seconds setter \sa m_startTime
  */
  void setStartTime(const unsigned int i_seconds);

  /*!
  * \brief Show countdown status
  * \return bool getter \sa m_isActive true: is active / false: not active
  */
  bool isActive() const;

  /*!
  * \brief Start countdown
  * \details only start if not running \sa m_isActive and start value \sa m_startValue is set
  * \return bool true: running / false: not running
  */
  bool start();

  /*!
  * \brief Stop countdown and check \sa m_timer has really stopped
  * \return bool true: is stopped / false: not stopped
  */
  bool stop();

  /*!
  * \brief Reset countdown (reset \sa m_timeLeft value with \sa m_startTime)
  * \details Will also \sa stop() current countdown
  * \return bool true: is reseted / false: not reseted
  */
  bool reset();


signals:
  /*!
  * \brief Update countdown
  * \param timeLeft current \sa m_timeLeft value
  */
  void update(const unsigned int timeLeft);

  /*!
  * \brief Countdown elapsed
  * \details \sa m_timeLeft reached zero
  */
  void elapsed();


private:
  /*!
  * \brief Start the timer until there is no time left, means \sa m_timeLeft reaches zero
  * \details Attention: this function always deals with the already expired second
  */
  void run();


  //one second = 1000 ms
  static constexpr int ONE_SECOND = 1000;

  //timer intervall (one second)
  QTimer *m_timer;

  //countdown start time (seconds)
  unsigned int m_startTime;

  //time left (seconds)
  unsigned int m_timeLeft;

  //status of the countdown
  bool m_isActive;

};

#endif // COUNTDOWN_H
