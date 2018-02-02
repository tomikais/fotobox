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
    Q_PROPERTY(Qt::CheckState showButtons READ showButtons WRITE setShowButtons)
    Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(int inputPin READ inputPin WRITE setInputPin)
    Q_PROPERTY(int outputPin READ outputPin WRITE setOutputPin)

public:
  Qt::CheckState showButtons() { return m_general.showButtons; }
  void setShowButtons(const Qt::CheckState i_value) { m_general.showButtons = i_value; }

  QString& backgroundColor() { return m_general.backgroundColor; }
  void setBackgroundColor(const QString& i_value) { m_general.backgroundColor = i_value; }

  int inputPin() { return m_buzzer.inputPin; }
  void setInputPin(const int i_inputPin) { m_buzzer.inputPin = i_inputPin; }

  int outputPin() { return m_buzzer.outputPin; }
  void setOutputPin(const int i_outputPin) { m_buzzer.inputPin = i_outputPin; }


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

  /*!
  * \brief struct store general settings
  */
  struct generalSettings {
    Qt::CheckState showButtons; /**< show buttons in GUI */
    QString backgroundColor;    /**< background color */
  };

  /*!
  * \brief struct store buzzer settings
  */
  struct buzzerSettings {
    int inputPin;   /**< default: GPIO 17 (=wiringPi pin 0) */
    int outputPin;  /**< default: GPIO 24 (=wiringPi pin 5) */
  };


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

  //general ettings
  generalSettings m_general;

  //buzzer settings
  buzzerSettings m_buzzer;
};
