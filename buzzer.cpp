/* buzzer.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "buzzer.h"

#if defined (__arm__)
#include <wiringPi.h>
#endif


#if defined (__WIRING_PI_H__)
// ******************************************
// DEVICE: Raspberry Pi (wiringPi available)

#include "preferenceprovider.h"

Buzzer::Buzzer(QObject *parent) : QObject(parent)
{
  //wiringPi http://wiringpi.com/reference/setup/
  //function always returns zero 0, no need to check result!
  wiringPiSetup();

  //set mode of the pin (INPUT, OUTPUT, PWM_OUTPUT)
  pinMode(PreferenceProvider::instance().outputPin(), OUTPUT);
  pinMode(PreferenceProvider::instance().inputPin(), INPUT);
}


auto Buzzer::queryPin() -> void
{
  //query pin
  while (digitalRead(PreferenceProvider::instance().inputPin()) != HIGH) {
    //wait before query pin again
    delay(PreferenceProvider::instance().queryInterval());
  }

  //buzzer was pressed
  emit resultReady();
}

#else

// **************************************
// DEVICE: other (no wiringPi available)

Buzzer::Buzzer(QObject *parent) : QObject(parent) { /* no wiringPi framework available */ }

auto Buzzer::queryPin() -> void { /* no wiringPi framework available */ }

#endif
