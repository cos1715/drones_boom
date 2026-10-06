#include "hardwareManager.h"

void HardwareManager::begin()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  setLed(false);

  if (VOLTAGE_SENSE_PIN >= 0)
  {
    pinMode(VOLTAGE_SENSE_PIN, INPUT);
  }
}

void HardwareManager::setLed(bool enabled)
{
  ledState = enabled;

  digitalWrite(
      LED_PIN,
      enabled ? HIGH : LOW);
}

bool HardwareManager::isButtonReleased() const
{
  return digitalRead(BUTTON_PIN) == HIGH;
}

bool HardwareManager::isLedOff() const
{
  return digitalRead(LED_PIN) == LOW;
}

bool HardwareManager::isPowerVoltageOk() const
{
  if (VOLTAGE_SENSE_PIN < 0)
  {
    return true;
  }

  return analogRead(VOLTAGE_SENSE_PIN) >= MIN_ADC_READING;
}

void HardwareManager::resetIndicator()
{
  indicatorTimestamp = millis();
  errorStep = 0;
  ledState = false;

  digitalWrite(LED_PIN, LOW);
}

void HardwareManager::updateIndicator(State state)
{
  if (state != previousState)
  {
    resetIndicator();
    previousState = state;
  }

  unsigned long currentTime = millis();

  switch (state)
  {
  case State::INIT:
    setLed(false);
    break;

  case State::SAFE:
    setLed(false);
    break;

  case State::WAITING:
    if (currentTime - indicatorTimestamp >=
        WAITING_BLINK_INTERVAL_MS)
    {
      indicatorTimestamp = currentTime;
      setLed(!ledState);
    }
    break;

  case State::ARM:
    setLed(true);
    break;

  case State::TRIGGERED:
    if (currentTime - indicatorTimestamp >=
        TRIGGERED_BLINK_INTERVAL_MS)
    {
      indicatorTimestamp = currentTime;
      setLed(!ledState);
    }
    break;

  case State::ERROR:
    if (currentTime - indicatorTimestamp >=
        ERROR_BLINK_INTERVAL_MS)
    {
      indicatorTimestamp = currentTime;

      errorStep++;

      switch (errorStep)
      {
      case 1:
        setLed(true);
        break;

      case 2:
        setLed(false);
        break;

      case 3:
        setLed(true);
        break;

      case 4:
        setLed(false);
        break;

      case 5:
        setLed(false);
        break;

      case 6:
        errorStep = 0;
        break;
      }
    }
    break;
  }
}
