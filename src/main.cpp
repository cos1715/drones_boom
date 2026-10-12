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
bool startupReady = false;
bool startupFailed = false;

void setup()
{
  hardware.begin();
  pwmCommandReader.begin();
  pwmTestGenerator.prepare();

  Serial.begin(115200);
  systemCheck.begin();
}

void loop()
{
  if (!startupReady && !startupFailed)
  {
    const SystemCheckResult systemCheckResult = systemCheck.update();

    if (systemCheckResult == SystemCheckResult::RUNNING)
    {
      return;
    }

    if (systemCheckResult != SystemCheckResult::OK)
    {
      stateMachine.begin(false);
      Serial.print("Startup stopped: ");
      Serial.println(SystemCheck::resultToString(systemCheckResult));
      startupFailed = true;
      return;
    }

    stateMachine.begin(true);
    pwmTestGenerator.begin();
    pwmTestGenerator.printReadyMessage();
    controlWebServer.begin();
    startupReady = true;
  }

  if (startupFailed)
  {
    hardware.updateIndicator(stateMachine.getCurrentState());
    return;
  }

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
