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
* \brief The Preferences class
* Main class to change and store the application settings
*/
class Preferences : public QDialog
{
  Q_OBJECT

public:
  /*!
   * \brief getInstance (Meyers Singleton)
   */
  static Preferences& getInstance();

  /*!
  * \brief return the directory to store the pictures
  * \return QString absolute directory path
  */
  auto pictureDirectory() const -> QString;


private slots:
  /*!
  * \brief Open a color picker to choose background color
  * \details Save color picked by a user in tool tip of th color label.
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


private:
  /*!
  * \brief Preferences constructor
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief Preferences destructor
  */
  ~Preferences();

  /*!
  * \brief delete copy constructor (Singleton)
  */
  Preferences(const Preferences&) = delete;

  /*!
  * \brief delete assigment (Singleton)
  */
  Preferences& operator=(const Preferences&) = delete;


  /*!
  * \brief override void Dialog::closeEvent(QCloseEvent *e)
  * \details load application settings from INI file
  */
  auto setLabelColor(QLineEdit *i_lineEdit, const QColor &i_color) -> void;

  /*!
  * \brief load application settings from INI file
  */
  auto initializePreferences() -> void;


  /*!
  * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse move events for the widget
  * \details stopping auto accept dialog \sa autoAcceptDialog()
  */
  auto mouseMoveEvent(QMouseEvent *event) -> void override;


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
  Q_PROPERTY(QString argumentLine READ argumentLine WRITE setArgumentLine NOTIFY argumentLineChanged)
  Q_PROPERTY(int timeoutValue READ timeoutValue WRITE setTimeoutValue NOTIFY timeoutValueChanged)

  bool showButtons();
  void setShowButtons(const bool i_value);

  QString& backgroundColor();
  void setBackgroundColor(const QString& i_value);

  int inputPin();
  void setInputPin(const int i_value);

  int outputPin();
  void setOutputPin(const int i_value);

  QString argumentLine();
  void setArgumentLine(const QString& i_value);

  int timeoutValue();
  void setTimeoutValue(const int i_value);

signals:
  void showButtonsChanged(bool);
  void backgroundColorChanged(QString);
  void inputPinChanged(int);
  void outputPinChanged(int);
  void argumentLineChanged(QString);
  void timeoutValueChanged(int);

private:
  bool m_showButtons;
  QString m_backgroundColor;
  int m_inputPin;
  int m_outputPin;
  QString m_argumentLine;
  int m_timeoutValue;

};
