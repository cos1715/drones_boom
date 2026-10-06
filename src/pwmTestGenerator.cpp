#include "pwmTestGenerator.h"

void PwmTestGenerator::begin()
{
  if (!ENABLED)
  {
    return;
  }

  ledcSetup(PWM_CHANNEL, FREQUENCY_HZ, RESOLUTION_BITS);
  ledcAttachPin(PWM_TEST_OUTPUT_PIN, PWM_CHANNEL);
  setCommand(PwmCommand::STOP);
  Serial.println("PWM loopback test ready: connect GPIO25 to GPIO19; send 1, 2, or 3 in Serial Monitor.");
}

void PwmTestGenerator::writePulseWidth(unsigned int pulseWidthUs)
{
  const uint32_t periodUs = 1000000UL / FREQUENCY_HZ;
  const uint32_t maxDuty = (1UL << RESOLUTION_BITS) - 1UL;
  const uint32_t duty = (static_cast<uint32_t>(pulseWidthUs) * maxDuty) / periodUs;
  ledcWrite(PWM_CHANNEL, duty);
}

void PwmTestGenerator::setCommand(PwmCommand command)
{
  if (!ENABLED)
  {
    return;
  }

  switch (command)
  {
  case PwmCommand::STOP:
    writePulseWidth(1000);
    break;
  case PwmCommand::START:
    writePulseWidth(1500);
    break;
  case PwmCommand::TRIGGER:
    writePulseWidth(2000);
    break;
  default:
    writePulseWidth(0);
    break;
  }
}

void PwmTestGenerator::handleSerialInput()
{
  if (!ENABLED || !Serial.available())
  {
    return;
  }

  switch (Serial.read())
  {
  case '1':
    setCommand(PwmCommand::STOP);
    break;
  case '2':
    setCommand(PwmCommand::START);
    break;
  case '3':
    setCommand(PwmCommand::TRIGGER);
    break;
  default:
    break;
  }
}
