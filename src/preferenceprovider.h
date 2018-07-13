/* preferenceprovider.h
*
* Copyright (c) 2018 Thomas Kais
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE', which is part of this source code package.
*/
#pragma once
#include <QObject>

/*!
* \brief Preference who stores the preferences
*/
class PreferenceProvider : public QObject
{
  Q_OBJECT


private:
  /*!
  * \brief hide Preferences constructor (Singleton)
  * \param parent QWidget
  */
  explicit PreferenceProvider(QObject *parent = nullptr);

  /*!
  * \brief hide Preferences destructor (Singleton)
  */
  ~PreferenceProvider() override = default;

  QString m_photoFolder;
  QString m_photoName;
  int m_countdown = 0;
  QString m_countdownColor;
  bool m_showButtons = false;
  QString m_backgroundColor;
  int m_inputPin = 0;
  int m_outputPin = 0;
  int m_queryInterval = 0;
  QString m_cameraMode;
  QString m_argumentLine;
  int m_timeoutValue = 0;


public:
  //Qt Property System
  Q_PROPERTY(QString m_photoFolder     READ photoFolder     WRITE setPhotoFolder     NOTIFY photoFolderChanged)
  Q_PROPERTY(QString m_photoName       READ photoName       WRITE setPhotoName       NOTIFY photoNameChanged)
  Q_PROPERTY(int m_countdown           READ countdown       WRITE setCountdown       NOTIFY countdownChanged)
  Q_PROPERTY(QString m_countdownColor  READ countdownColor  WRITE setCountdownColor  NOTIFY countdownColorChanged)
  Q_PROPERTY(bool m_showButtons        READ showButtons     WRITE setShowButtons     NOTIFY showButtonsChanged)
  Q_PROPERTY(QString m_backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(int m_inputPin            READ inputPin        WRITE setInputPin        NOTIFY inputPinChanged)
  Q_PROPERTY(int m_outputPin           READ outputPin       WRITE setOutputPin       NOTIFY outputPinChanged)
  Q_PROPERTY(int m_queryInterval       READ queryInterval   WRITE setQueryInterval   NOTIFY queryIntervalChanged)
  Q_PROPERTY(QString m_cameraMode      READ cameraMode      WRITE setCameraMode      NOTIFY cameraModeChanged)
  Q_PROPERTY(QString m_argumentLine    READ argumentLine    WRITE setArgumentLine    NOTIFY argumentLineChanged)
  Q_PROPERTY(int m_timeoutValue        READ timeoutValue    WRITE setTimeoutValue    NOTIFY timeoutValueChanged)

  /*!
  * \brief delete copy constructor (Singleton)
  */
  PreferenceProvider(const PreferenceProvider&) = delete;

  /*!
  * \brief delete assigment (Singleton)
  */
  PreferenceProvider& operator=(const PreferenceProvider&) = delete;

  /*!
  * \brief getInstance (Meyers Singleton)
  * \return Preferences&
  */
  auto static instance() -> PreferenceProvider&;

  /*!
  * \brief one second = 1000 ms
  */
  static int ONE_SECOND;


  /*!
  * \brief photo output directory
  * \return QString m_photoFolder
  */
  QString photoFolder();

  /*!
  * \brief photo name template
  * \return QString m_photoName
  */
  QString photoName();

  /*!
  * \brief countdown until photo is taken
  * \return int m_countdown
  */
  int countdown();

  /*!
  * \brief font color of the countdown
  * \return QString m_countdownColor
  */
  QString countdownColor();

  /*!
  * \brief buttons are displayed on the UI
  * \return bool m_showButtons
  */
  bool showButtons();

  /*!
  * \brief background color of the FotoBox UI
  * \return QString m_backgroundColor
  */
  QString backgroundColor();

  /*!
  * \brief wiringPi GPIO input pin
  * \return int m_inputPin
  */
  int inputPin();

  /*!
  * \brief wiringPi GPIO output pin
  * \return int m_outputPin
  */
  int outputPin();

  /*!
  * \brief how often the pin should be queried
  * \return int m_queryInterval
  */
  int queryInterval();

  /*!
  * \brief the camera framework to be used
  * \return QString m_cameraMode
  */
  QString cameraMode();

  /*!
  * \brief camera framework arguments
  * \return QString m_argumentLine
  */
  QString argumentLine();

  /*!
  * \brief timeout value for the camera framework
  * \return int m_timeoutValue
  */
  int timeoutValue();


public slots:
  /*!
  * \brief set photo output directory
  * \param i_value QString&
  */
  void setPhotoFolder(const QString& i_value);

  /*!
  * \brief set photo name template
  * \param i_value QString&
  */
  void setPhotoName(const QString& i_value);

  /*!
  * \brief set countdown until photo is taken
  * \param i_value int
  */
  void setCountdown(int i_value);

  /*!
  * \brief set font color of the countdown
  * \param i_value QString&
  */
  void setCountdownColor(const QString& i_value);

  /*!
  * \brief set whether buttons are displayed on the UI
  * \param i_value bool
  */
  void setShowButtons(bool i_value);

  /*!
  * \brief set background color of the FotoBox UI
  * \param i_value QString&
  */
  void setBackgroundColor(const QString& i_value);

  /*!
  * \brief set wiringPi GPIO input pin
  * \param i_value int
  */
  void setInputPin(int i_value);

  /*!
  * \brief set wiringPi GPIO output pin
  * \param i_value int
  */
  void setOutputPin(int i_value);

  /*!
  * \brief set how often the pin should be queried
  * \param i_value int
  */
  void setQueryInterval(int i_value);

  /*!
  * \brief set the camera framework to be used
  * \param i_value QString&
  */
  void setCameraMode(const QString& i_value);

  /*!
  * \brief set camera framework arguments
  * \param i_value QString&
  */
  void setArgumentLine(const QString& i_value);

  /*!
  * \brief set timeout value for the camera framework
  * \param i_value int
  */
  void setTimeoutValue(int i_value);


signals:
  /*!
  * \brief signal: photo output directory has changed
  */
  void photoFolderChanged(QString);

  /*!
  * \brief signal: photo name template has changed
  */
  void photoNameChanged(QString);

  /*!
  * \brief signal: countdown until photo is taken has changed
  */
  void countdownChanged(int);

  /*!
  * \brief signal: font color of the countdown has changed
  */
  void countdownColorChanged(QString);

  /*!
  * \brief signal: whether buttons are displayed on the UI has changed
  */
  void showButtonsChanged(bool);

  /*!
  * \brief signal: background color of the FotoBox UI has changed
  */
  void backgroundColorChanged(QString);

  /*!
  * \brief signal: wiringPi GPIO input pin has changed
  */
  void inputPinChanged(int);

  /*!
  * \brief signal: wiringPi GPIO output pin has changed
  */
  void outputPinChanged(int);

  /*!
  * \brief signal: how often the pin should be queried has changed
  */
  void queryIntervalChanged(int);

  /*!
  * \brief signal: the camera framework to be used has changed
  */
  void cameraModeChanged(QString);

  /*!
  * \brief signal: camera framework arguments has changed
  */
  void argumentLineChanged(QString);

  /*!
  * \brief signal: timeout value for the camera framework has changed
  */
  void timeoutValueChanged(int);

};
