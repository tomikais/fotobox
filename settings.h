/* settings.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once

#include <QObject>
#include <QSettings>

/*!
 * \brief The Settings class
 * Organizes the application settings
 */
class Settings : public QObject
{
  Q_OBJECT

public:
  /*!
   * \brief Settings construcotr
   * \param parent QObject
   */
  explicit Settings(QObject* parent = nullptr);

private:
  /*!
   * \brief m_settings Qt Settings Object
   */
  QSettings m_settings;

};
