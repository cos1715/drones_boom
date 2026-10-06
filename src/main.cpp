#include <Arduino.h>

#include "hardwareManager.h"
#include "systemCheck.h"
#include "stateMachine.h"
#include "configManager.h"
#include "controlWebServer.h"
#include "pwmCommand.h"
#include "sensorManager.h"
#include "pwmTestGenerator.h"

HardwareManager hardware;
ConfigManager configManager;
PwmCommandReader pwmCommandReader;
SensorManager sensorManager;
SystemCheck systemCheck(hardware, configManager, sensorManager);
StateMachine stateMachine;
PwmTestGenerator pwmTestGenerator;
ControlWebServer controlWebServer(
    stateMachine,
    configManager);

bool prevButtonReleased = true;
PwmCommand previousPwmCommand = PwmCommand::INVALID;

void setup()
{
  Serial.begin(115200);

  hardware.begin();

  bool systemCheckResult = systemCheck.runSelfTest();
  stateMachine.begin(systemCheckResult);

  pwmCommandReader.begin();
  pwmTestGenerator.begin();
  controlWebServer.begin();
}

void loop()
{
  stateMachine.update();
  pwmTestGenerator.handleSerialInput();

  PwmCommand command = pwmCommandReader.readCommand();
  bool buttonReleased = hardware.isButtonReleased();

  if (command != PwmCommand::INVALID &&
      command != previousPwmCommand)
  {
    switch (command)
    {
    case PwmCommand::START:
      stateMachine.start();
      break;

    case PwmCommand::STOP:
      stateMachine.stop();
      break;

    case PwmCommand::TRIGGER:
      stateMachine.trigger();
      break;

    default:
      break;
    }
  }

  previousPwmCommand = command;

  if (prevButtonReleased && !buttonReleased)
  {
    stateMachine.stop();
  }

  controlWebServer.handleClient();

  prevButtonReleased = buttonReleased;

  hardware.updateIndicator(
      stateMachine.getCurrentState());
}
