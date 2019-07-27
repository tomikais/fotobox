/* buzzer.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "buzzer.h"


#if defined (BUZZER_AVAILABLE)
#include <wiringPi.h>
#include "preferenceprovider.h"


Buzzer::Buzzer(QObject* /*parent*/) : QObject(nullptr)
{
    //wiringPi http://wiringpi.com/reference/setup/
    //function always returns zero 0, no need to check result!
    wiringPiSetup();

    //set mode of the pin (INPUT, OUTPUT, PWM_OUTPUT)
    pinMode(PreferenceProvider::instance().outputPin(), OUTPUT);
    pinMode(PreferenceProvider::instance().inputPin(), INPUT);
}

void Buzzer::queryPin()
{
    //query pin
    while (digitalRead(PreferenceProvider::instance().inputPin()) != HIGH) {
        //wait before query pin again
        delay(PreferenceProvider::instance().queryInterval());

        //if stop is true, stop while loop and return to caller (don't emit triggered)
        if (m_stop) {
            return;
        }
    }

    //buzzer was pressed
    Q_EMIT triggered();
}
#else
Buzzer::Buzzer(QObject* /*parent*/) : QObject(nullptr) {/* stub */};
void Buzzer::queryPin(){/* stub */};
#endif


void Buzzer::stop()
{
    //set std::atomic to true
    m_stop = true;
}
