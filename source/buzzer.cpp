/* buzzer.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "buzzer.h"

namespace FotoBox {
#if defined (BUZZER_AVAILABLE)
#include <QThread>
#include <pigpiod_if2.h>
#include "preferenceprovider.h"

Buzzer::~Buzzer()
{
    //This function resets the used DMA channels, releases memory, and terminates any running threads.
    pigpio_stop(m_pi);
};

auto Buzzer::initialise() -> bool
{
    //Connect to the pigpio daemon
    m_pi = pigpio_start(nullptr, nullptr);
    if (m_pi < 0) {
        //pigpio initialisation failed.
        return false;
    } else {
        //pigpio initialised okay, now set mode of the pin
        if (set_mode(m_pi, PreferenceProvider::instance().outputPin(), PI_OUTPUT) != 0) {
            return false;
        }
        if (set_mode(m_pi, PreferenceProvider::instance().inputPin(), PI_INPUT) != 0) {
            return false;
        }
    }
    return true;
}

auto Buzzer::checkDeamon() -> bool
{
    //nullptr -> 'localhost', nullptr -> '8888'
    auto result = pigpio_start(nullptr, nullptr);
    if (result < 0) {
        //pigpio initialisation failed.
        return false;
    } else {
        //deamon reachable -> disconnect
        pigpio_stop(result);
        return true;
    }
}

void Buzzer::queryPin()
{
    //query pin
    while (gpio_read(m_pi, PreferenceProvider::instance().inputPin()) != PI_HIGH) {
        //wait before query pin again
        QThread::msleep(PreferenceProvider::instance().queryInterval());

        //if stop is true, stop while loop and return to caller (don't emit triggered)
        if (m_stop) {
            return;
        }
    }

    //buzzer was pressed
    Q_EMIT triggered();
}
#else
Buzzer::~Buzzer(){}; // NOLINT
auto Buzzer::initialise()  -> bool{return false;}; // NOLINT
auto Buzzer::checkDeamon() -> bool{return false;}; // NOLINT
void Buzzer::queryPin(){/* stub */}; // NOLINT
#endif


//don't set parent!
Buzzer::Buzzer(QObject* /*parent*/) : QObject(nullptr) { }

void Buzzer::stop()
{
    //set std::atomic to true
    m_stop = true;
}

}
