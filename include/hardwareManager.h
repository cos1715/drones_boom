#pragma once

#include <Arduino.h>
#include "state.h"

class HardwareManager
{
public:
  void begin();

  void updateIndicator(State state);

  bool isButtonReleased() const;
  bool isLedOff() const;
  bool isPowerVoltageOk() const;

private:
  static constexpr uint8_t BUTTON_PIN = 18;
  static constexpr uint8_t LED_PIN = 23;

  static constexpr int8_t VOLTAGE_SENSE_PIN = -1;
  static constexpr int MIN_ADC_READING = 0;

  static constexpr unsigned long WAITING_BLINK_INTERVAL_MS = 500;
  static constexpr unsigned long TRIGGERED_BLINK_INTERVAL_MS = 100;
  static constexpr unsigned long ERROR_BLINK_INTERVAL_MS = 200;

  unsigned long indicatorTimestamp = 0;
  bool ledState = false;
  uint8_t errorStep = 0;

  State previousState = State::INIT;

  void setLed(bool enabled);
  void resetIndicator();
};
