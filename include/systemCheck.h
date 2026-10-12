#pragma once

#include <Arduino.h>
#include "hardwareManager.h"
#include "configManager.h"
#include "sensorManager.h"

enum class SystemCheckResult : uint8_t
{
    RUNNING = 0,
    OK,
    FILESYSTEM_FAILED,
    CONFIG_FAILED,
    ESP_ID_FAILED,
    BUTTON_FAILED,
    LED_FAILED,
    SENSOR_FAILED,
    POWER_FAILED
};

class SystemCheck
{
public:
    SystemCheck(HardwareManager &hardware, ConfigManager &config, SensorManager &sensors);

    void begin();
    SystemCheckResult update();
    static const char *resultToString(SystemCheckResult result);

private:
    enum class CheckState : uint8_t
    {
        FILESYSTEM,
        CONFIG,
        ESP_ID,
        BUTTON,
        LED,
        SENSORS,
        POWER,
        COMPLETE,
        FAILED
    };

    HardwareManager &hardware;
    ConfigManager &config;
    SensorManager &sensors;
    CheckState checkState = CheckState::COMPLETE;
    SystemCheckResult result = SystemCheckResult::RUNNING;

    static constexpr const char *CONFIG_PATH = "/config.txt";

    bool checkFilesystem();
    bool checkConfig();
    bool checkEspId();
    bool checkButton();
    bool checkOutput();
    bool checkSensors();
    bool checkPower();
    SystemCheckResult fail(SystemCheckResult failure);
};
