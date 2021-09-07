/* preferenceprovider.h
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef PREFERENCEPROVIDER_H
#define PREFERENCEPROVIDER_H

#include <QObject>

namespace FotoBox {

/*!
 * \brief Preference who stores the preferences
 */
class PreferenceProvider : public QObject
{
    Q_OBJECT

private:
    /*!
     * \brief hide Preferences default constructor (Singleton)
     * \param parent QObject
     */
    explicit PreferenceProvider(QObject *parent = nullptr);

    /*!
     * \brief hide Preferences destructor (Singleton)
     */
    ~PreferenceProvider() override = default;

    /*! photo output directory */
    QString m_photoFolder;

    /*! photo name template */
    QString m_photoName;

    /*! font color of the countdown */
    QString m_countdownColor;

    /*! background color of the FotoBox UI */
    QString m_backgroundColor;

    /*! the camera framework to be used */
    QString m_cameraMode;

    /*! camera framework arguments */
    QString m_argumentLine;

    /*! countdown until photo is taken */
    int m_countdown = 0;

    /*! pigpio GPIO input pin */
    unsigned int m_inputPin = 0;

    /*! pigpio GPIO output pin */
    unsigned int m_outputPin = 0;

    /*! how often the pin should be queried */
    unsigned int m_queryInterval = 0;

    /*! timeout value for the camera framework */
    int m_timeoutValue = 0;

    /*! show photo in grayscale (monochrome photography) */
    bool m_grayscale = false;

    /*! buttons are displayed on the UI */
    bool m_showButtons = false;

    /*! turn print on or off */
    bool m_print = false;

    /*! printer name */
    QString m_printerName;

public:
    //Qt Property System
    Q_PROPERTY(QString photoFolder READ photoFolder WRITE setPhotoFolder NOTIFY photoFolderChanged)
    Q_PROPERTY(QString photoName READ photoName WRITE setPhotoName NOTIFY photoNameChanged)
    Q_PROPERTY(QString countdownColor READ countdownColor WRITE setCountdownColor NOTIFY countdownColorChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QString cameraMode READ cameraMode WRITE setCameraMode NOTIFY cameraModeChanged)
    Q_PROPERTY(QString argumentLine READ argumentLine WRITE setArgumentLine NOTIFY argumentLineChanged)
    Q_PROPERTY(int countdown READ countdown WRITE setCountdown NOTIFY countdownChanged)
    Q_PROPERTY(unsigned int inputPin READ inputPin WRITE setInputPin NOTIFY inputPinChanged)
    Q_PROPERTY(unsigned int outputPin READ outputPin WRITE setOutputPin NOTIFY outputPinChanged)
    Q_PROPERTY(unsigned int queryInterval READ queryInterval WRITE setQueryInterval NOTIFY queryIntervalChanged)
    Q_PROPERTY(int timeoutValue READ timeoutValue WRITE setTimeoutValue NOTIFY timeoutValueChanged)
    Q_PROPERTY(bool grayscale READ grayscale WRITE setGrayscale NOTIFY grayscaleChanged)
    Q_PROPERTY(bool showButtons READ showButtons WRITE setShowButtons NOTIFY showButtonsChanged)
    Q_PROPERTY(bool print READ print WRITE setPrint NOTIFY printChanged)
    Q_PROPERTY(QString printerName READ printerName WRITE setPrinterName NOTIFY printerNameChanged)

    /*!
     * \brief PreferenceProvider delete copy constructor (Singleton)
     */
    PreferenceProvider(const PreferenceProvider &) = delete;

    /*!
     * \brief PreferenceProvider delete copy assignment (Singleton)
     */
    PreferenceProvider &operator=(const PreferenceProvider &) = delete;

    /*!
     * \brief PreferenceProvider delete move constructor (Singleton)
     */
    PreferenceProvider(PreferenceProvider &&other) = delete;

    /*!
     * \brief PreferenceProvider delete move assignment (Singleton)
     */
    PreferenceProvider &operator=(PreferenceProvider &&other) = delete;

    /*!
     * \brief get instance (Meyers Singleton)
     * \return Preferences&
     */
    static PreferenceProvider &instance();

    /*!
     * \brief photo output directory
     * \return QString \see m_photoFolder
     */
    auto photoFolder() const -> QString;

    /*!
     * \brief photo name template
     * \return QString \see m_photoName
     */
    auto photoName() const -> QString;

    /*!
     * \brief font color of the countdown
     * \return QString \see m_countdownColor
     */
    auto countdownColor() const -> QString;

    /*!
     * \brief background color of the FotoBox UI
     * \return QString \see m_backgroundColor
     */
    auto backgroundColor() const -> QString;

    /*!
     * \brief the camera framework to be used
     * \return QString \see m_cameraMode
     */
    QString cameraMode() const;

    /*!
     * \brief camera framework arguments
     * \return QString \see m_argumentLine
     */
    auto argumentLine() const -> QString;

    /*!
     * \brief countdown until photo is taken
     * \return int \see m_countdown
     */
    auto countdown() const -> int;

    /*!
     * \brief pigpio GPIO input pin
     * \return unsigned int \see m_inputPin
     */
    auto inputPin() const -> unsigned int;

    /*!
     * \brief pigpio GPIO output pin
     * \return unsigned int \see m_outputPin
     */
    auto outputPin() const -> unsigned int;

    /*!
     * \brief how often the pin should be queried
     * \return unsigned int \see m_queryInterval
     */
    auto queryInterval() const -> unsigned int;

    /*!
     * \brief timeout value for the camera framework
     * \return int \see m_timeoutValue
     */
    auto timeoutValue() const -> int;

    /*!
     * \brief show photo in grayscale (monochrome photography)
     * \return bool \see m_grayscale
     */
    auto grayscale() const -> bool;

    /*!
     * \brief buttons are displayed on the UI
     * \return bool \see m_showButtons
     */
    auto showButtons() const -> bool;

    /*!
     * \brief print on or off
     * \return bool \see m_print
     */
    auto print() const -> bool;

    /*!
     * \brief printer name
     * \return QString \see m_printerName
     */
    auto printerName() const -> QString;

public Q_SLOTS:
    /*!
     * \brief set photo output directory
     * \param i_value QString&
     */
    void setPhotoFolder(const QString &i_value);

    /*!
     * \brief set photo name template
     * \param i_value QString&
     */
    void setPhotoName(const QString &i_value);

    /*!
     * \brief set font color of the countdown
     * \param i_value QString&
     */
    void setCountdownColor(const QString &i_value);

    /*!
     * \brief set background color of the FotoBox UI
     * \param i_value QString&
     */
    void setBackgroundColor(const QString &i_value);

    /*!
     * \brief set the camera framework to be used
     * \param i_value QString&
     */
    void setCameraMode(const QString &i_value);

    /*!
     * \brief set camera framework arguments
     * \param i_value QString&
     */
    void setArgumentLine(const QString &i_value);

    /*!
     * \brief set countdown until photo is taken
     * \param i_value int
     */
    void setCountdown(int i_value);

    /*!
     * \brief set pigpio GPIO input pin
     * \param i_value unsigned int
     */
    void setInputPin(unsigned int i_value);

    /*!
     * \brief set pigpio GPIO output pin
     * \param i_value unsigned int
     */
    void setOutputPin(unsigned int i_value);

    /*!
     * \brief set how often the pin should be queried
     * \param i_value unsigned int
     */
    void setQueryInterval(unsigned int i_value);

    /*!
     * \brief set timeout value for the camera framework
     * \param i_value int
     */
    void setTimeoutValue(int i_value);

    /*!
     * \brief set grayscale bool (monochrome photography)
     * \param i_value int
     */
    void setGrayscale(bool i_value);

    /*!
     * \brief set whether buttons are displayed on the UI
     * \param i_value bool
     */
    void setShowButtons(bool i_value);

    /*!
     * \brief set print on or off
     * \param i_value bool
     */
    void setPrint(bool i_value);

    /*!
     * \brief set printer name
     * \param i_value QString&
     */
    void setPrinterName(const QString &i_value);

Q_SIGNALS:
    /*!
     * \brief signal: photo output directory has changed
     */
    void photoFolderChanged(QString);

    /*!
     * \brief signal: photo name template has changed
     */
    void photoNameChanged(QString);

    /*!
     * \brief signal: font color of the countdown has changed
     */
    void countdownColorChanged(QString);

    /*!
     * \brief signal: background color of the FotoBox UI has changed
     */
    void backgroundColorChanged(QString);

    /*!
     * \brief signal: the camera framework to be used has changed
     */
    void cameraModeChanged(QString);

    /*!
     * \brief signal: camera framework arguments has changed
     */
    void argumentLineChanged(QString);

    /*!
     * \brief signal: countdown until photo is taken has changed
     */
    void countdownChanged(int);

    /*!
     * \brief signal: pigpio GPIO input pin has changed
     */
    void inputPinChanged(unsigned int);

    /*!
     * \brief signal: pigpio GPIO output pin has changed
     */
    void outputPinChanged(unsigned int);

    /*!
     * \brief signal: how often the pin should be queried has changed
     */
    void queryIntervalChanged(unsigned int);

    /*!
     * \brief signal: timeout value for the camera framework has changed
     */
    void timeoutValueChanged(int);

    /*!
     * \brief signal: grayscale (monochrome photography) has changed
     */
    void grayscaleChanged(bool);

    /*!
     * \brief signal: whether buttons are displayed on the UI has changed
     */
    void showButtonsChanged(bool);

    /*!
     * \brief signal: print has changed
     */
    void printChanged(bool);

    /*!
     * \brief signal: printer name has changed
     */
    void printerNameChanged(QString);
};

} // end namespace FotoBox

#endif // PREFERENCEPROVIDER_H
