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

class Preferences : public QDialog
{
  Q_OBJECT

public:
  explicit Preferences(QWidget *parent = nullptr);
  ~Preferences();

private:
  Ui::Preferences *m_ui;
  QSettings m_settings;
};
