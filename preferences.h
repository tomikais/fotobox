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
class FotoBox;


namespace Ui {
  class PreferencesDialog;
}


/*!
* \brief Preference class to change and store the application settings
*/
class Preferences : public QDialog
{
  Q_OBJECT


public:
  /*!
  * \brief hide Preferences constructor (Singleton)
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief hide Preferences destructor (Singleton)
  */
  virtual ~Preferences();


  private slots:
  /*!
  * \brief start the FotoBox
  */
  void startFotoBox();

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
  void showColor(const QString& i_colorName);

  /*!
  * \brief check camera application availability and provide help
  * \param i_name QString name of the application
  */
  void applicationAvailable(const QString& i_name);


private:
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


  //FotoBox
  FotoBox* m_fotoBox;

  //User Interface
  Ui::PreferencesDialog *m_ui;

  //Store and read settings (INI file)
  QSettings m_settings;

  //Auto close dialog
  int m_counter = 9;
  QTimer* m_timer;

};


