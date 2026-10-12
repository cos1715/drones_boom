#include <LittleFS.h>
#include "systemCheck.h"
#include "hardwareManager.h"
#include "configManager.h"

SystemCheck::SystemCheck(HardwareManager &hardware, ConfigManager &config, SensorManager &sensors)
    : hardware(hardware), config(config), sensors(sensors) {}

void SystemCheck::begin()
{
  Serial.println("Starting self-test...");
  checkState = CheckState::FILESYSTEM;
  result = SystemCheckResult::RUNNING;
}

SystemCheckResult SystemCheck::update()
{
  if (checkState == CheckState::COMPLETE || checkState == CheckState::FAILED)
  {
    return result;
  }

  switch (checkState)
  {
  case CheckState::FILESYSTEM:
    if (!checkFilesystem())
    {
      return fail(SystemCheckResult::FILESYSTEM_FAILED);
    }
    checkState = CheckState::CONFIG;
    break;

  case CheckState::CONFIG:
    if (!checkConfig())
    {
      return fail(SystemCheckResult::CONFIG_FAILED);
    }
    checkState = CheckState::ESP_ID;
    break;

  case CheckState::ESP_ID:
    if (!checkEspId())
    {
      return fail(SystemCheckResult::ESP_ID_FAILED);
    }
    checkState = CheckState::BUTTON;
    break;

  case CheckState::BUTTON:
    if (!checkButton())
    {
      return fail(SystemCheckResult::BUTTON_FAILED);
    }
    checkState = CheckState::LED;
    break;

  case CheckState::LED:
    if (!checkOutput())
    {
      return fail(SystemCheckResult::LED_FAILED);
    }
    checkState = CheckState::SENSORS;
    break;

  case CheckState::SENSORS:
    if (!checkSensors())
    {
      return fail(SystemCheckResult::SENSOR_FAILED);
    }
    checkState = CheckState::POWER;
    break;

  case CheckState::POWER:
    if (!checkPower())
    {
      return fail(SystemCheckResult::POWER_FAILED);
    }
    checkState = CheckState::COMPLETE;
    result = SystemCheckResult::OK;
    break;

  default:
    return fail(SystemCheckResult::FILESYSTEM_FAILED);
  }

  return result;
}

SystemCheckResult SystemCheck::fail(SystemCheckResult failure)
{
  checkState = CheckState::FAILED;
  result = failure;
  return result;
}

const char *SystemCheck::resultToString(SystemCheckResult result)
{
  switch (result)
  {
  case SystemCheckResult::RUNNING:
    return "RUNNING";
  case SystemCheckResult::OK:
    return "OK";
  case SystemCheckResult::FILESYSTEM_FAILED:
    return "FILESYSTEM_FAILED";
  case SystemCheckResult::CONFIG_FAILED:
    return "CONFIG_FAILED";
  case SystemCheckResult::ESP_ID_FAILED:
    return "ESP_ID_FAILED";
  case SystemCheckResult::BUTTON_FAILED:
    return "BUTTON_FAILED";
  case SystemCheckResult::LED_FAILED:
    return "LED_FAILED";
  case SystemCheckResult::SENSOR_FAILED:
    return "SENSOR_FAILED";
  case SystemCheckResult::POWER_FAILED:
    return "POWER_FAILED";
  }

  return "UNKNOWN";
}

bool SystemCheck::checkFilesystem()
{
  Serial.print("Checking filesystem...");
  bool state = LittleFS.begin();
  Serial.println(state ? "ok" : "fail");
  return state;
}

bool SystemCheck::checkConfig()
{
  Serial.print("Checking configuration...");

  bool loaded = config.load();

  if (!loaded || !config.isValid())
  {
    Serial.println("fail");
    return false;
  }

  Serial.println("ok");
  return true;
}

bool SystemCheck::checkEspId()
{
  Serial.print("Checking ESP ID...");
  uint64_t mac = ESP.getEfuseMac();
  bool state = (mac != 0);
  Serial.println(state ? "ok" : "fail");
  Serial.print("ESP ID: ");
  Serial.println(mac);
  return state;
}

bool SystemCheck::checkButton()
{
  Serial.print("Checking button...");

  bool state = hardware.isButtonReleased();

  Serial.println(state ? "ok" : "fail");

  return state;
}

bool SystemCheck::checkOutput()
{
  Serial.print("Checking output...");

  bool state = hardware.isLedOff();

  Serial.println(state ? "ok" : "fail");

  return state;
}

bool SystemCheck::checkSensors()
{
  Serial.print("Checking sensors...");
  const bool state = sensors.beginAndCheck();
  Serial.println(state ? "ok" : "fail");
  return state;
}

bool SystemCheck::checkPower()
{
  Serial.print("Checking power monitor...");
  const bool state = hardware.isPowerVoltageOk();
  Serial.println(state ? "ok" : "fail");
  return state;
}
