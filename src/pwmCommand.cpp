#include "pwmCommand.h"

PwmCommand PwmCommandReader::decode(unsigned int pulseWidthUs)
{
  if (pulseWidthUs >= STOP_MIN && pulseWidthUs <= STOP_MAX)
  {
    return PwmCommand::STOP;
  }

  if (pulseWidthUs >= START_MIN && pulseWidthUs <= START_MAX)
  {
    return PwmCommand::START;
  }

  if (pulseWidthUs >= TRIGGER_MIN && pulseWidthUs <= TRIGGER_MAX)
  {
    return PwmCommand::TRIGGER;
  }

  return PwmCommand::INVALID;
}

void PwmCommandReader::begin()
{
  pinMode(PWM_PIN, INPUT);
}

unsigned long PwmCommandReader::readPulseWidth()
{
  return pulseIn(PWM_PIN, HIGH, 25000);
}

PwmCommand PwmCommandReader::readCommand()
{
  unsigned long pulseWidth = readPulseWidth();
  return decode(pulseWidth);
}