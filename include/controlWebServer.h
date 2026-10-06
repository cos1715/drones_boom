#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <LittleFS.h>

#include "stateMachine.h"
#include "configManager.h"

class ControlWebServer
{
private:
  StateMachine &stateMachine;
  ConfigManager &config;

  WebServer server;
  static constexpr const char *HTML_PATH = "/index.html";

  File loadHtmlFromFile();
  String stateJson() const;

public:
  ControlWebServer(
      StateMachine &stateMachine,
      ConfigManager &config);

  void begin();
  void handleClient();
};
