#include <LittleFS.h>
#include "configManager.h"

bool ConfigManager::load()
{
  wifiSsid = "";
  wifiPassword = "";

  if (!LittleFS.exists(CONFIG_PATH))
  {
    Serial.println("Config file not found");
    return false;
  }

  File file = LittleFS.open(CONFIG_PATH, "r");

  if (!file)
  {
    Serial.println("Cannot open config file");
    return false;
  }

  bool parsedSuccessfully = true;
  while (file.available())
  {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.length() == 0)
    {
      continue;
    }

    if (!parseLine(line))
    {
      parsedSuccessfully = false;
      Serial.println("Invalid configuration line");
    }
  }

  file.close();

  if (!parsedSuccessfully || wifiSsid.length() == 0)
  {
    Serial.println("WiFi SSID missing");
    return false;
  }

  if (wifiPassword.length() == 0)
  {
    Serial.println("WiFi password missing");
    return false;
  }

  Serial.println("Config loaded");

  return true;
}

bool ConfigManager::parseLine(const String &line)
{
  int separatorIndex = line.indexOf('=');

  if (separatorIndex == -1)
  {
    return false;
  }

  String key = line.substring(0, separatorIndex);
  String value = line.substring(separatorIndex + 1);

  key.trim();
  value.trim();

  if (key == "wifi_ssid")
  {
    wifiSsid = value;
    return true;
  }

  if (key == "wifi_password")
  {
    wifiPassword = value;
    return true;
  }

  if (key == "version")
  {
    return value.length() > 0;
  }

  return false;
}

String ConfigManager::getWifiSsid() const
{
  return wifiSsid;
}

String ConfigManager::getWifiPassword() const
{
  return wifiPassword;
}

bool ConfigManager::isValid() const
{
  return wifiSsid.length() > 0 && wifiPassword.length() > 0;
}
