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
* provides a countdown
*/
class Countdown : public QObject
{
  Q_OBJECT


public:
  /*!
  * \brief Countdown constructor
  * \param parent QObject
  * \param i_seconds set start time \sa m_startTime
  */
  explicit Countdown(QObject *parent = nullptr, const unsigned int i_seconds = 0);

  /*!
  * \brief Countdown destructor
  */
  ~Countdown() override;

  /*!
  * \brief Set the countdown start time
  * \param i_seconds set countdown start value \sa m_startTime
  */
  void setStartTime(const unsigned int i_seconds);

  /*!
  * \brief Show countdown status
  * \return bool true: is active / false: not active \sa m_isActive
  */
  bool isActive() const;

  /*!
  * \brief Start countdown
  * \details only start if not already running \sa m_isActive and start value \sa m_startValue is set
  * \return bool true: is active / false: not active
  */
  bool start();

  /*!
  * \brief Stop countdown
  * \return bool true: is stopped / false: not stopped
  */
  bool stop();


signals:
  /*!
  * \brief update countdown
  * \param timeLeft \sa m_timeLeft
  */
  void update(const unsigned int timeLeft);

  /*!
  * \brief countdown elapsed
  */
  void elapsed();


private:
  /*!
  * \brief elapse timer until time left \sa m_timeLeft reaches 0
  */
  void run();


  //one second = 1000 ms
  static constexpr int ONE_SECOND = 1000;

  //timer each intervall (1 sec)
  QTimer *m_timer;

  //countdown start time
  unsigned int m_startTime;

  //time left until countdown elapsed
  unsigned int m_timeLeft;

  //status
  bool m_isActive;

};

#endif // COUNTDOWN_H
