/* buzzer.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef BUZZER_H
#define BUZZER_H

//detect wiringPi framework
#if defined (__arm__) && __has_include(<wiringPi.h>)
//wiringPi detected
#define BUZZER_AVAILABLE
#endif

#include <QObject>

#include <atomic>

/*!
 * \brief This class handles the buzzer
 */
class Buzzer : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Buzzer constructor
     * \param parent QObject
     */
    explicit Buzzer(QObject *parent = nullptr);

    /*!
     * \brief Buzzer destructor
     */
    ~Buzzer() override = default;

    /*!
     * \brief Buzzer copy constructor
     */
    Buzzer(const Buzzer &) = delete;

    /*!
     * \brief Buzzer delete copy assignment
     */
    Buzzer &operator=(const Buzzer &) = delete;

    /*!
     * \brief Buzzer default move constructor
     */
    Buzzer(Buzzer &&other) = delete;

    /*!
     * \brief Buzzer default move assignment
     */
    Buzzer &operator=(Buzzer &&other) = delete;

    /*!
     * \brief Stop executing \sa queryPin()
     */
    void stop();

public Q_SLOTS:
    /*!
     * \brief Query the Raspberry Pi pin
     */
    void queryPin();

Q_SIGNALS:
    /*!
     * \brief Buzzer was pressed
     */
    void triggered();

private:
    /*! atomic bool to stop /sa queryPin() */
    std::atomic<bool> m_stop{false};
};

#endif // BUZZER_H
