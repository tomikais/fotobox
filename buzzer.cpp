/* buzzer.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "buzzer.h"

#include <wiringPi.h>


Buzzer::Buzzer(QObject *parent) :
  QThread(parent)
{

}


auto Buzzer::run() -> void
{
  //wiringPi
  //http://wiringpi.com/reference/setup/
  wiringPiSetup();

  //GPIO Pins
  unsigned int pin_zero = 0, pin_five = 5;
  pinMode(pin_zero, OUTPUT); //GPIO 17 (=wiringPi 0)
  pinMode(pin_five, INPUT);  //GPIO 24 (=wiringPi 5)

  //query pin 5
  unsigned int queryIntervalMilliseconds = 10;
  while(digitalRead(pin_five) != HIGH) {
    //wait
    delay(queryIntervalMilliseconds);
  }
}
