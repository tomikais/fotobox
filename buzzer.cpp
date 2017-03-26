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

void Buzzer::run()
{
  //wiringPi
  //http://wiringpi.com/reference/setup/
  wiringPiSetup();
  pinMode(0, OUTPUT); //GPIO 17 (=wiringPi 0)
  pinMode(5, INPUT);  //GPIO 24 (=wiringPi 5)

  while(digitalRead(5) != 1)
  {
    delayMicroseconds(10);
  }
}
