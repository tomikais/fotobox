/* buzzer.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef BUZZER_H
#define BUZZER_H

#pragma once
#include <QObject>
#include <atomic>

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

  /*!
  * \brief Buzzer default destructor
  */
  ~Buzzer() override = default;

  /*!
  * \brief Buzzer default move constructor
  */
  Buzzer(Buzzer&& other) = default;

  /*!
  * \brief Buzzer default move assignment
  */
  Buzzer& operator=(Buzzer&& other) = default;

  /*!
  * \brief Buzzer default copy constructor
  */
  Buzzer(const Buzzer& other) = default;

  /*!
  * \brief Buzzer default copy assignment
  */
  Buzzer& operator=(const Buzzer& other) = default;

  /*!
   * \brief Stop executing \sa queryPin()
   */
  void stop();


signals:
  /*!
   * \brief Buzzer was pressed
   */
  void triggered();

private:
  /*!
  * atomic bool to stop /sa queryPin()
  */
  std::atomic<bool> m_stop;

};

#endif // BUZZER_H
