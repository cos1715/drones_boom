#pragma once

#include <Arduino.h>
#include "hardwareManager.h"
#include "configManager.h"
#include "sensorManager.h"

class SystemCheck
{
public:
    SystemCheck(HardwareManager &hardware, ConfigManager &config, SensorManager &sensors);

    bool runSelfTest();

private:
    HardwareManager &hardware;
    ConfigManager &config;
    SensorManager &sensors;

    static constexpr const char *CONFIG_PATH = "/config.txt";

    bool checkFilesystem();
    bool checkConfig();
    bool checkEspId();
    bool checkButton();
    bool checkOutput();
    bool checkSensors();
    bool checkPower();
};
