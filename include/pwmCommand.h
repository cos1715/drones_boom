#pragma once

#include <Arduino.h>

enum class PwmCommand : uint8_t
{
  INVALID = 0,
  STOP,
  START,
  TRIGGER
};

class PwmCommandReader
{
private:
  static constexpr unsigned int STOP_MIN = 900;
  static constexpr unsigned int STOP_MAX = 1200;

  static constexpr unsigned int START_MIN = 1400;
  static constexpr unsigned int START_MAX = 1600;

  static constexpr unsigned int TRIGGER_MIN = 1800;
  static constexpr unsigned int TRIGGER_MAX = 2100;
  static constexpr uint8_t PWM_PIN = 19;

public:
  PwmCommand decode(unsigned int pulseWidthUs);

  void begin();
  unsigned long readPulseWidth();
  PwmCommand readCommand();
};