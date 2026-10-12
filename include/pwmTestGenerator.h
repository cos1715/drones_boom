#pragma once

#include <Arduino.h>
#include "pwmCommand.h"

class PwmTestGenerator
{
public:
  static constexpr bool ENABLED = true;
  static constexpr uint8_t PWM_TEST_OUTPUT_PIN = 25;

  void prepare();
  void begin();
  void printReadyMessage() const;
  void setCommand(PwmCommand command);
  void handleSerialInput();

private:
  static constexpr uint32_t FREQUENCY_HZ = 50;
  static constexpr uint8_t RESOLUTION_BITS = 16;
  static constexpr uint8_t PWM_CHANNEL = 0;

  void writePulseWidth(unsigned int pulseWidthUs);
};
