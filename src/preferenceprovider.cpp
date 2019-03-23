/* preferenceprovider.h
*
* Copyright (c) 2019 Thomas Kais
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE', which is part of this source code package.
*/
#include "preferenceprovider.h"


PreferenceProvider& PreferenceProvider::instance()
{
  //thread safe static initializer
  static PreferenceProvider instance;
  return instance;
}

//Qt Porperty
QString PreferenceProvider::photoFolder() const
{
  return m_photoFolder;
}

void PreferenceProvider::setPhotoFolder(const QString& i_value)
{
  if (m_photoFolder == i_value) {
      return;
    }

  m_photoFolder = i_value;
  emit photoFolderChanged(m_photoFolder);
}


QString PreferenceProvider::photoName() const
{
  return m_photoName;
}

void PreferenceProvider::setPhotoName(const QString& i_value)
{
  if (m_photoName == i_value) {
      return;
    }

  m_photoName = i_value;
  emit photoNameChanged(m_photoName);
}


int PreferenceProvider::countdown() const
{
  return m_countdown;
}

void PreferenceProvider::setCountdown(const int i_value)
{
  if (m_countdown == i_value) {
      return;
    }

  m_countdown = i_value;
  emit countdownChanged(m_countdown);
}


QString PreferenceProvider::countdownColor() const
{
  return m_countdownColor;
}

void PreferenceProvider::setCountdownColor(const QString& i_value)
{
  if (m_countdownColor == i_value) {
      return;
    }

  m_countdownColor = i_value;
  emit countdownColorChanged(m_countdownColor);
}


bool PreferenceProvider::showButtons() const
{
  return m_showButtons;
}

void PreferenceProvider::setShowButtons(const bool i_value)
{
  if (m_showButtons == i_value) {
      return;
    }

  m_showButtons = i_value;
  emit showButtonsChanged(m_showButtons);
}


QString PreferenceProvider::backgroundColor() const
{
  return m_backgroundColor;
}

void PreferenceProvider::setBackgroundColor(const QString& i_value)
{
  if (m_backgroundColor == i_value) {
      return;
    }

  m_backgroundColor = i_value;
  emit backgroundColorChanged(m_backgroundColor);
}


int PreferenceProvider::inputPin() const
{
  return m_inputPin;
}

void PreferenceProvider::setInputPin(const int i_value)
{
  if (m_inputPin == i_value) {
      return;
    }

  m_inputPin = i_value;
  emit inputPinChanged(m_inputPin);
}


int PreferenceProvider::outputPin() const
{
  return m_outputPin;
}

void PreferenceProvider::setOutputPin(const int i_value)
{
  if (m_outputPin == i_value) {
      return;
    }

  m_outputPin = i_value;
  emit outputPinChanged(m_outputPin);
}


int PreferenceProvider::queryInterval() const
{
  return m_queryInterval;
}

void PreferenceProvider::setQueryInterval(const int i_value)
{
  if (m_queryInterval == i_value) {
      return;
    }

  m_queryInterval = i_value;
  emit queryIntervalChanged(m_queryInterval);
}


QString PreferenceProvider::cameraMode() const
{
  return m_cameraMode;
}

void PreferenceProvider::setCameraMode(const QString& i_value)
{
  if (m_cameraMode == i_value) {
      return;
    }

  m_cameraMode = i_value;
  emit cameraModeChanged(m_cameraMode);
}


QString PreferenceProvider::argumentLine() const
{
  return m_argumentLine;
}

void PreferenceProvider::setArgumentLine(const QString& i_value)
{
  if (m_argumentLine == i_value) {
      return;
    }

  m_argumentLine = i_value;
  emit argumentLineChanged(m_argumentLine);
}


int PreferenceProvider::timeoutValue() const
{
  return m_timeoutValue;
}

void PreferenceProvider::setTimeoutValue(const int i_value)
{
  if (m_timeoutValue == i_value) {
      return;
    }

  m_timeoutValue = i_value;
  emit timeoutValueChanged(m_timeoutValue);
}

bool PreferenceProvider::grayscale() const
{
  return m_grayscale;
}

void PreferenceProvider::setGrayscale(bool i_value)
{
  if (m_grayscale == i_value) {
      return;
    }

  m_grayscale = i_value;
  emit grayscaleChanged(m_grayscale);
}

