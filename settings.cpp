#include "settings.h"


#include <QApplication>

Settings::Settings(QObject *parent) :
  QObject(parent),
  m_settings(qApp->applicationName(), QSettings::IniFormat)
{

}
