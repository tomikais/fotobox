/* preferences.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include <QDialog>
#include <QSettings>

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
  * \brief Preferences constructor
  * \param parent QWidget
  */
  explicit Preferences(QWidget *parent = nullptr);

  /*!
  * \brief FotoBox destructor
  */
  ~Preferences();


private:
  /*!
  * \brief override void QWidget::closeEvent(QCloseEvent *event)
  * \details save application settings to INI file
  */
  auto closeEvent(QCloseEvent *event) -> void override;

  //User Interface
  Ui::Preferences *m_ui;

  //Store and read settings (INI file)
  QSettings m_settings;
};
