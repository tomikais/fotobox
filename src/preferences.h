/* preferences.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef PREFERENCES_H
#define PREFERENCES_H

#pragma once
#include <QDialog>
#include <QSettings>

class QTimer;
class FotoBox;


namespace Ui {
  class PreferencesDialog;
} //namespace Ui


/*!
* \brief Preference class to change and store the application settings
*/
class Preferences : public QDialog
{
  Q_OBJECT


public:
  /*!
  * \brief hide Preferences constructor
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief Preferences destructor
  */
  ~Preferences() override = default;


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
  * \brief Open a File Dialog and set path in UI.
  */
  void chooseDirectory();

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
  * \brief Signal & Slot connect
  */
  void connectUi();

  /*!
  * \brief load application settings from INI file
  */
  void loadPreferences();

  /*!
  * \brief Save the preferences to QSettings
  */
  void savePreferences();

  /*!
  * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse move events for the widget
  * \details stopping auto accept dialog \sa autoAcceptDialog()
  */
  void mouseMoveEvent(QMouseEvent *event) override;


  //User Interface
  Ui::PreferencesDialog *m_ui;

  //Store and read settings (INI file)
  QSettings m_settings;

  //Auto close dialog
  int m_counter = 9;
  QTimer *m_timer;

};

#endif // PREFERENCES_H
