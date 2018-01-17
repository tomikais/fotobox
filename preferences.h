/* preferences.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include <QDialog>
#include <QSettings>


class QLabel;


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

  /*!
  * \brief return the directory to store the pictures
  * \return QString absolute directory path
  */
  auto pictureDirectory() const->QString;


  private slots:
  /*!
  * \brief Open a color picker to choose background color
  * \details Save color picked by a user in tool tip of th color label.
  */
  void colorDialog();

  /*!
   * \brief quit application without saving
   */
  void quitApplication();


protected:
  /*!
  * \brief override void QWidget::closeEvent(QCloseEvent *event)
  * \details save application settings to INI file
  */
  auto virtual closeEvent(QCloseEvent *event) -> void override;

  /*!
  * \brief override void Dialog::closeEvent(QCloseEvent *e)
  * \details load application settings from INI file
  */
  auto virtual showEvent(QShowEvent *event) -> void override;


private:
  /*!
  * \brief override void Dialog::closeEvent(QCloseEvent *e)
  * \details load application settings from INI file
  */
  auto setLabelColor(QLabel *i_label, const QColor &i_color) -> void;


  //User Interface
  Ui::Preferences *m_ui;

  //Store and read settings (INI file)
  QSettings m_settings;
};
