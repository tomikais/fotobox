/* buzzer.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "buzzer.h"

#ifdef __arm__
#include <wiringPi.h>
#endif

#ifdef __WIRING_PI_H__
// ******************************************
// DEVICE: Raspberry Pi (wiringPi available)

#include "preferenceprovider.h"

Buzzer::Buzzer(QObject *parent) : QThread(parent)
{
  //wiringPi http://wiringpi.com/reference/setup/
  //function always returns zero 0, no need to check result!
  wiringPiSetup();

  //set mode of the pin (INPUT, OUTPUT, PWM_OUTPUT)
  pinMode(PreferenceProvider::instance().outputPin(), OUTPUT);
  pinMode(PreferenceProvider::instance().inputPin(), INPUT);
}


auto Buzzer::run() -> void
{
  //query pin
  while (digitalRead(PreferenceProvider::instance().inputPin()) != HIGH) {
    //wait before check again
    delay(PreferenceProvider::instance().queryInterval());
  }
}

#else

// **************************************
// DEVICE: other (no wiringPi available)

Buzzer::Buzzer(QObject *parent) : QThread(parent) { }

auto Buzzer::run() -> void { /* no wiringPi framework available */ }

#endif
