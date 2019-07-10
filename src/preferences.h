/* preferences.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef FOTOBOX_PREFERENCES_H
#define FOTOBOX_PREFERENCES_H

#include "countdown.h"

#include <QDialog>
#include <QSettings>

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
  * \brief Preferences constructor
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief Preferences default destructor
  */
  ~Preferences() override = default;

  /*!
  * \brief Preferences default copy constructor
  */
  Preferences(const Preferences& other) = delete;

  /*!
  * \brief Preferences default copy assignment
  */
  Preferences& operator=(const Preferences& other) = delete;

  /*!
  * \brief Preferences default move constructor
  */
  Preferences(Preferences&& other) = delete;

  /*!
  * \brief Preferences default move assignment
  */
  Preferences& operator=(Preferences&& other) = delete;


private Q_SLOTS:
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


  /*! User Interface */
  Ui::PreferencesDialog *m_ui;

  /*! Store and read settings (INI file) */
  QSettings m_settings;

  /*! Countdown to "auto close dialog" */
  Countdown m_countdown;

};

#endif // FOTOBOX_PREFERENCES_H
