#include <WebServer.h>
#include <WiFi.h>
#include <LittleFS.h>
#include "stateMachine.h"
#include "controlWebServer.h"

ControlWebServer::ControlWebServer(
    StateMachine &stateMachine,
    ConfigManager &config)
    : stateMachine(stateMachine),
      config(config),
      server(80)
{
}

File ControlWebServer::loadHtmlFromFile()
{
    if (!LittleFS.exists(HTML_PATH))
    {
        Serial.println("HTML file not found");
        stateMachine.fail();
        return File();
    }

    File file = LittleFS.open(HTML_PATH, "r");

    if (!file)
    {
        Serial.println("Cannot open HTML file");
        stateMachine.fail();
        return File();
    }

    return file;
}

String ControlWebServer::stateJson() const
{
  const State currentState = stateMachine.getCurrentState();
  String response = "{\"state\":\"" + String(stateToString(currentState)) + "\"";
  response += ",\"safetyRemainingSeconds\":" + String(stateMachine.getRemainingSafetyTimeMs() / 1000UL);
  response += ",\"simulation\":true}";
  return response;
}

void ControlWebServer::begin()
{
    String ssid = config.getWifiSsid();
    String password = config.getWifiPassword();

    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(
        ssid.c_str(),
        password.c_str());

    const unsigned long connectStartedAt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - connectStartedAt < 15000UL)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi connection timeout; entering fail-safe ERROR state");
        stateMachine.fail();
        return;
    }
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [this]()
              {
                            
    File htmlFile = loadHtmlFromFile();
      if (!htmlFile)
    {
        server.send(500, "text/plain", "HTML file error");
        return;
    }
   server.streamFile(htmlFile, "text/html");
 htmlFile.close(); });

    server.on("/state", HTTP_GET, [this]()
              {
        server.send(200, "application/json", stateJson()); });

    server.on("/start", HTTP_POST, [this]()
              {
        stateMachine.start();

        server.send(200, "application/json", stateJson()); });

    server.on("/stop", HTTP_POST, [this]()
              {
        stateMachine.stop();

        server.send(200, "application/json", stateJson()); });

    server.on("/trigger", HTTP_POST, [this]()
              {
        stateMachine.trigger();

        server.send(200, "application/json", stateJson()); });

    server.begin();

    Serial.println("Web server started");
}
void ControlWebServer::handleClient()
{
    server.handleClient();
}
