/* buzzer.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "buzzer.h"

#if defined (__arm__) && __has_include(<wiringPi.h>)
#include <wiringPi.h>
#endif


#if defined (__WIRING_PI_H__)
// ******************************************
// DEVICE: Raspberry Pi (wiringPi available)

#include "preferenceprovider.h"

Buzzer::Buzzer(QObject *parent) : QObject(parent),
  m_stop(false)
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
  emit triggered();
}
#else

// **************************************
// DEVICE: other (no wiringPi available)

Buzzer::Buzzer(QObject *parent) : QObject(parent), m_stop(false) { /* no wiringPi framework available */ }

void Buzzer::queryPin() { /* no wiringPi framework available */ }

#endif


void Buzzer::stop()
{
  //set std::atomic to true
  m_stop = true;
}
