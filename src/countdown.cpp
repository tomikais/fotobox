/* countdown.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "countdown.h"

#include <QTimer>


Countdown::Countdown(QObject *parent, const unsigned int i_seconds) : QObject(parent),
  m_timer(new QTimer(this)),
  m_startTime(i_seconds),
  m_timeLeft(i_seconds),
  m_isActive(false)
{
  //set update intervall to one second
  m_timer->setInterval(ONE_SECOND);

  connect(m_timer, &QTimer::timeout, this, &Countdown::run);
}


Countdown::~Countdown()
{
  //stop and delete timer
  m_timer->stop();
  m_timer->deleteLater();
}


void Countdown::run()
{
  if (m_timeLeft > 0) {
      //go on
      m_timer->start();

      emit update(m_timeLeft);
      --m_timeLeft;
      return;
    }

  //countdown finished/elapsed
  emit elapsed();
}


void Countdown::setStartTime(const unsigned int i_seconds)
{
  //only set start time if countdown not running
  if (!m_isActive) {
      m_startTime = i_seconds;
      m_timeLeft = i_seconds;
    }
}


bool Countdown::isActive() const
{
  return m_isActive;
}


bool Countdown::start()
{
  //not running and start time set
  if (!m_isActive && m_startTime > 0) {
      //start countdown
      m_timer->start();

      //check if really running
      if (m_timer->isActive()) {
          //countdown active
          m_isActive = true;
          //set start time and time left are equal
          m_timeLeft = m_startTime;
          return true;
        }
    }

  return false;
}


bool Countdown::stop()
{
  m_timer->stop();
  m_isActive = m_timer->isActive();

  return m_isActive;
}
