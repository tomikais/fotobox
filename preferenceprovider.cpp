/* preferences.h
*
* Copyright (c) 2017 Thomas Kais
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE', which is part of this source code package.
*/
#include "preferenceprovider.h"

#include <QApplication>
#include <QDir>



auto PreferenceProvider::instance() -> PreferenceProvider&
{
  //thread safe static initializer
  static PreferenceProvider instance;
  return instance;
}

PreferenceProvider::PreferenceProvider(QObject *parent) : QObject(parent)
{
}

auto PreferenceProvider::pictureDirectory() -> QString
{
#if defined __APPLE__
  //macOS shit
  return QApplication::applicationDirPath() + QDir::separator() +
    QLatin1String("..") + QDir::separator() +
    QLatin1String("..") + QDir::separator() +
    QLatin1String("..") + QDir::separator();
#else
  return QApplication::applicationDirPath() + QDir::separator();
#endif
}


//Qt Porperty
QString PreferenceProvider::photoFolder()
{
  return m_photoFolder;
}

void PreferenceProvider::setPhotoFolder(const QString& i_value)
{
  m_photoFolder = i_value;
  emit photoFolderChanged(m_photoFolder);
}


QString PreferenceProvider::photoName()
{
  return m_photoName;
}

void PreferenceProvider::setPhotoName(const QString& i_value)
{
  m_photoName = i_value;
  emit photoNameChanged(m_photoName);
}


bool PreferenceProvider::showButtons()
{
  return m_showButtons;
}

void PreferenceProvider::setShowButtons(const bool i_value)
{
  m_showButtons = i_value;
  emit showButtonsChanged(m_showButtons);
}


QString PreferenceProvider::backgroundColor()
{
  return m_backgroundColor;
}

void PreferenceProvider::setBackgroundColor(const QString& i_value)
{
  m_backgroundColor = i_value;
  emit backgroundColorChanged(m_backgroundColor);
}


int PreferenceProvider::inputPin()
{
  return m_inputPin;
}

void PreferenceProvider::setInputPin(const int i_value)
{
  m_inputPin = i_value;
  emit inputPinChanged(m_inputPin);
}


int PreferenceProvider::outputPin()
{
  return m_outputPin;
}

void PreferenceProvider::setOutputPin(const int i_value)
{
  m_outputPin = i_value;
  emit outputPinChanged(m_outputPin);
}


int PreferenceProvider::queryInterval()
{
  return m_queryInterval;
}

void PreferenceProvider::setQueryInterval(const int i_value)
{
  m_queryInterval = i_value;
  emit queryIntervalChanged(m_queryInterval);
}


QString PreferenceProvider::cameraMode()
{
  return m_cameraMode;
}

void PreferenceProvider::setCameraMode(const QString& i_value)
{
  m_cameraMode = i_value;
  emit cameraModeChanged(m_cameraMode);
}


QString PreferenceProvider::argumentLine()
{
  return m_argumentLine;
}

void PreferenceProvider::setArgumentLine(const QString& i_value)
{
  m_argumentLine = i_value;
  emit argumentLineChanged(m_argumentLine);
}


int PreferenceProvider::timeoutValue()
{
  return m_timeoutValue;
}

void PreferenceProvider::setTimeoutValue(const int i_value)
{
  m_timeoutValue = i_value;
  emit timeoutValueChanged(m_timeoutValue);
}
