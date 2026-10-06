#include <LittleFS.h>
#include "systemCheck.h"
#include "hardwareManager.h"
#include "configManager.h"

SystemCheck::SystemCheck(HardwareManager &hardware, ConfigManager &config, SensorManager &sensors)
    : hardware(hardware), config(config), sensors(sensors) {}

bool SystemCheck::runSelfTest()
{
  bool fs = checkFilesystem();
  bool configOk = checkConfig();
  bool espId = checkEspId();
  bool button = checkButton();
  bool output = checkOutput();
  bool sensors = checkSensors();
  bool power = checkPower();

  return fs && configOk && espId && button && output && sensors && power;
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
