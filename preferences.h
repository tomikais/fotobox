/* preferences.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QDialog>
#include <QSettings>

class QLineEdit;
class QTimer;


namespace Ui {
  class Preferences;
}


/*!
* \brief Preference class to change and store the application settings
*/
class Preferences : public QDialog
{
  Q_OBJECT


public:
  /*!
   * \brief getInstance (Meyers Singleton)
   * \return Preferences&
   */
  static auto getInstance() -> Preferences&;

  /*!
  * \brief return the directory to store the pictures
  * \return QString absolute directory path
  */
  auto pictureDirectory() const -> QString;


private slots:
  /*!
  * \brief Open a color picker to choose background color for FotoBox
  */
  void colorDialog();

  /*!
  * \brief Save the preferences to QSettings
  */
  void savePreferences();

  /*!
  * \brief Accept dialog after 10 seconds. You can stop that by moving mouse into preference dialog.
  */
  void autoAcceptDialog();

  /*!
  * \brief Restore default preferences.
  */
  void restoreDefaultPreferences();

  /*!
  * \brief show the given color in QLineEdit
  * \param i_colorName QString hex name of the color
  */
  void showColor(const QString i_colorName);


private:
  /*!
  * \brief hide Preferences constructor (Singleton)
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief hide Preferences destructor (Singleton)
  */
  virtual ~Preferences();

  /*!
  * \brief delete copy constructor (Singleton)
  */
  Preferences(const Preferences&) = delete;

  /*!
  * \brief delete assigment (Singleton)
  */
  Preferences& operator=(const Preferences&) = delete;

  /*!
  * \brief load application settings from INI file
  */
  auto loadPreferences() -> void;

  /*!
  * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse move events for the widget
  * \details stopping auto accept dialog \sa autoAcceptDialog()
  */
  auto mouseMoveEvent(QMouseEvent *event) -> void override;

  /*!
  * \brief Hide different preferences if they aren't available
  */
  auto hidePreferences() -> void;


  //User Interface
  Ui::Preferences *m_ui;

  //Store and read settings (INI file)
  QSettings m_settings;

  //Auto close dialog
  int m_counter = 9;
  QTimer* m_timer;


  //Qt Property
public:
  Q_PROPERTY(bool showButtons READ showButtons WRITE setShowButtons NOTIFY showButtonsChanged)
  Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(int inputPin READ inputPin WRITE setInputPin NOTIFY setChanged NOTIFY inputPinChanged)
  Q_PROPERTY(int outputPin READ outputPin WRITE setOutputPin NOTIFY outputPinChanged)
  Q_PROPERTY(int queryInterval READ queryInterval WRITE setQueryInterval NOTIFY queryIntervalChanged)
  Q_PROPERTY(QString cameraMode READ cameraMode WRITE setCameraMode NOTIFY cameraModeChanged)
  Q_PROPERTY(QString argumentLine READ argumentLine WRITE setArgumentLine NOTIFY argumentLineChanged)
  Q_PROPERTY(int timeoutValue READ timeoutValue WRITE setTimeoutValue NOTIFY timeoutValueChanged)

  bool showButtons();
  QString& backgroundColor();
  int inputPin();
  int outputPin();
  int queryInterval();
  QString cameraMode();
  QString argumentLine();
  int timeoutValue();


private:
  void setShowButtons(const bool i_value);
  void setBackgroundColor(const QString& i_value);
  void setInputPin(const int i_value);
  void setOutputPin(const int i_value);
  void setQueryInterval(const int i_value);
  void setCameraMode(const QString& i_value);
  void setArgumentLine(const QString& i_value);
  void setTimeoutValue(const int i_value);

  bool m_showButtons;
  QString m_backgroundColor;
  int m_inputPin;
  int m_outputPin;
  int m_queryInterval;
  QString m_cameraMode;
  QString m_argumentLine;
  int m_timeoutValue;

signals:
  void showButtonsChanged(bool);
  void backgroundColorChanged(QString);
  void inputPinChanged(int);
  void outputPinChanged(int);
  void queryIntervalChanged(int);
  void cameraModeChanged(QString);
  void argumentLineChanged(QString);
  void timeoutValueChanged(int);
};
