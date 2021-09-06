/* countdown.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "countdown.h"

namespace FotoBox {
Countdown::Countdown(QObject *parent, const unsigned int i_seconds)
    : QObject(parent)
    , m_timer(this)
    , m_startTime(i_seconds)
    , m_timeLeft(i_seconds)
{
    //set update intervall to one second
    m_timer.setInterval(ONE_SECOND);

    //Every second it is checked if the countdown is elapsed
    connect(&m_timer, &QTimer::timeout, this, &Countdown::updateTimeLeft);
}

void Countdown::updateTimeLeft()
{
    //this function always deals with the already expired second
    //check if countdown isn't elapsed
    if (m_timeLeft > 1) {
        //reduce counter before sending the signal
        --m_timeLeft;

        //send the signal that one second has passed
        Q_EMIT update(m_timeLeft);

        //Because it isn't a single shot time we don't need to start it again
        return;
    }

    //countdown finished/elapsed
    stop();
    m_timeLeft = 0;
    Q_EMIT elapsed();
}

void Countdown::setStartTime(const unsigned int i_seconds)
{
    m_startTime = i_seconds;
    reset();
}

auto Countdown::isActive() const -> bool
{
    return m_isActive;
}

auto Countdown::start() -> bool
{
    //not running and start time set
    if (!m_isActive && m_startTime > 0) {
        //start countdown
        m_timer.start();

        //check if really running
        if (m_timer.isActive()) {
            //countdown active
            m_isActive = true;

            //inform whith current value
            Q_EMIT update(m_timeLeft);
            return true;
        }
    }

    return false;
}

auto Countdown::stop() -> bool
{
    //stop timer
    m_timer.stop();
    m_isActive = m_timer.isActive();

    //if isn't active it's stopped so negate bool
    return !m_isActive;
}

auto Countdown::reset() -> bool
{
    //stop countdown
    if (stop()) {
        //reset it
        m_timeLeft = m_startTime;
        return true;
    }

    return false;
}

}
