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

public:
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
  auto static instance()->PreferenceProvider&;

  //Qt Property
  Q_PROPERTY(QString photoFolder READ photoFolder WRITE setPhotoFolder NOTIFY photoFolderChanged)
  Q_PROPERTY(QString photoName READ photoName WRITE setPhotoName NOTIFY photoNameChanged)
  Q_PROPERTY(bool showButtons READ showButtons WRITE setShowButtons NOTIFY showButtonsChanged)
  Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(int inputPin READ inputPin WRITE setInputPin NOTIFY setChanged NOTIFY inputPinChanged)
  Q_PROPERTY(int outputPin READ outputPin WRITE setOutputPin NOTIFY outputPinChanged)
  Q_PROPERTY(int queryInterval READ queryInterval WRITE setQueryInterval NOTIFY queryIntervalChanged)
  Q_PROPERTY(QString cameraMode READ cameraMode WRITE setCameraMode NOTIFY cameraModeChanged)
  Q_PROPERTY(QString argumentLine READ argumentLine WRITE setArgumentLine NOTIFY argumentLineChanged)
  Q_PROPERTY(int timeoutValue READ timeoutValue WRITE setTimeoutValue NOTIFY timeoutValueChanged)

  QString photoFolder();
  QString photoName();
  bool showButtons();
  QString backgroundColor();
  int inputPin();
  int outputPin();
  int queryInterval();
  QString cameraMode();
  QString argumentLine();
  int timeoutValue();

  void setPhotoFolder(const QString& i_value);
  void setPhotoName(const QString& i_value);
  void setShowButtons(const bool i_value);
  void setBackgroundColor(const QString& i_value);
  void setInputPin(const int i_value);
  void setOutputPin(const int i_value);
  void setQueryInterval(const int i_value);
  void setCameraMode(const QString& i_value);
  void setArgumentLine(const QString& i_value);
  void setTimeoutValue(const int i_value);


signals:
  void photoFolderChanged(QString);
  void photoNameChanged(QString);
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
  ~PreferenceProvider() override = default;


  QString m_photoFolder;
  QString m_photoName;
  bool m_showButtons = false;
  QString m_backgroundColor;
  int m_inputPin = 0;
  int m_outputPin = 0;
  int m_queryInterval = 0;
  QString m_cameraMode;
  QString m_argumentLine;
  int m_timeoutValue = 0;

};
