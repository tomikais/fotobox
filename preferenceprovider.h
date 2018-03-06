/* preferences.h
*
* Copyright (c) 2017 Thomas Kais
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

public:
  //Qt Property
  Q_PROPERTY(bool showButtons READ showButtons WRITE setShowButtons NOTIFY showButtonsChanged)
  Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(int inputPin READ inputPin WRITE setInputPin NOTIFY setChanged NOTIFY inputPinChanged)
  Q_PROPERTY(int outputPin READ outputPin WRITE setOutputPin NOTIFY outputPinChanged)
  Q_PROPERTY(int queryInterval READ queryInterval WRITE setQueryInterval NOTIFY queryIntervalChanged)
  Q_PROPERTY(QString cameraMode READ cameraMode WRITE setCameraMode NOTIFY cameraModeChanged)
  Q_PROPERTY(QString argumentLine READ argumentLine WRITE setArgumentLine NOTIFY argumentLineChanged)
  Q_PROPERTY(int timeoutValue READ timeoutValue WRITE setTimeoutValue NOTIFY timeoutValueChanged)

  /*!
  * \brief getInstance (Meyers Singleton)
  * \return Preferences&
  */
  auto static instance()->PreferenceProvider&;

  /*!
  * \brief return the directory to store the pictures
  * \return QString absolute directory path
  */
  auto static pictureDirectory()->QString;

  bool showButtons();
  QString& backgroundColor();
  int inputPin();
  int outputPin();
  int queryInterval();
  QString cameraMode();
  QString argumentLine();
  int timeoutValue();

  void setShowButtons(const bool i_value);
  void setBackgroundColor(const QString& i_value);
  void setInputPin(const int i_value);
  void setOutputPin(const int i_value);
  void setQueryInterval(const int i_value);
  void setCameraMode(const QString& i_value);
  void setArgumentLine(const QString& i_value);
  void setTimeoutValue(const int i_value);


signals:
  void showButtonsChanged(bool);
  void backgroundColorChanged(QString);
  void inputPinChanged(int);
  void outputPinChanged(int);
  void queryIntervalChanged(int);
  void cameraModeChanged(QString);
  void argumentLineChanged(QString);
  void timeoutValueChanged(int);


private:
  /*!
  * \brief hide Preferences constructor (Singleton)
  * \param parent QWidget
  */
  explicit PreferenceProvider(QObject *parent = nullptr);

  /*!
  * \brief hide Preferences destructor (Singleton)
  */
  virtual ~PreferenceProvider() {};

  /*!
  * \brief delete copy constructor (Singleton)
  */
  PreferenceProvider(const PreferenceProvider&) = delete;

  /*!
  * \brief delete assigment (Singleton)
  */
  PreferenceProvider& operator=(const PreferenceProvider&) = delete;


  bool m_showButtons;
  QString m_backgroundColor;
  int m_inputPin;
  int m_outputPin;
  int m_queryInterval;
  QString m_cameraMode;
  QString m_argumentLine;
  int m_timeoutValue;

};
