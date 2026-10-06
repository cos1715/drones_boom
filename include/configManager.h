#pragma once

#include <Arduino.h>

class ConfigManager
{
public:
  bool load();

  bool isValid() const;
  String getWifiSsid() const;
  String getWifiPassword() const;

private:
  static constexpr const char *CONFIG_PATH = "/config.txt";

  String wifiSsid;
  String wifiPassword;

  bool parseLine(const String &line);
};