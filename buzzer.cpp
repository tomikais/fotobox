/* buzzer.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "buzzer.h"

#ifdef __arm__
// ******************************************
// DEVICE: Raspberry Pi (wiringPi available)

#include <wiringPi.h>

Buzzer::Buzzer(QObject *parent) :
  QThread(parent)
{
  //wiringPi http://wiringpi.com/reference/setup/
  //function always returns zero 0, no need to check result!
  wiringPiSetup();

  //set mode of the pin (INPUT, OUTPUT, PWM_OUTPUT)
  pinMode(m_pinZero, OUTPUT);
  pinMode(m_pinFive, INPUT);
}


auto Buzzer::run() -> void
{
  //query pin 5
  while(digitalRead(m_pinFive) != HIGH) {
    //wait before check again
    delay(m_queryIntervalMsecs);
  }
}

#else
// **************************************
// DEVICE: other (no wiringPi available)

Buzzer::Buzzer(QObject *parent) :
  QThread(parent)
{}

auto Buzzer::run() -> void
{}

#endif
