/* buzzer.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef BUZZER_H
#define BUZZER_H

//detect pigpio library
#if defined (__arm__) && __has_include(<pigpiod_if2.h>)
#define BUZZER_AVAILABLE
#endif

#include <QObject>

#include <atomic>

namespace FotoBox {
/*!
 * \brief This class handles the buzzer
 * \details http://abyz.me.uk/rpi/pigpio/pdif2.html
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
    ~Buzzer() override;

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
     * \brief Initialises the library and sets the GPIO mode
     * \return true: initialisation okay / false: initialisation failed
     */
    auto initialise() -> bool;

    /*!
     * \brief Check if pigpio daemon is reachable
     * \return true: reachable / false: unreachable
     */
    auto static checkDeamon() -> bool;

    /*!
     * \brief Stop executing \see queryPin()
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
    /*! This value is passed to the GPIO routines to specify the Pi to be operated on. */
    int m_pi{-1};
    /*! atomic bool to stop \see queryPin() */
    std::atomic<bool> m_stop{false};
};

}
#endif // BUZZER_H
