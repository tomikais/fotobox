/* buzzer.h
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QObject>


/*!
 * \brief This class handles the buzzer
 *  Run in a separate thread.
 */
class Buzzer : public QObject
{
  Q_OBJECT


public slots:
  /*!
  * \brief Query the Raspberry Pi pin
  */
  void queryPin();


public:
  /*!
   * Buzzer constructor
   * \param parent QObject
   */
  explicit Buzzer(QObject *parent = nullptr);


signals:
  /*!
  * \brief Buzzer was pressed
  */
  void triggered();

};
