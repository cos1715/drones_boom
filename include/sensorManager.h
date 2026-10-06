#pragma once

#include <Arduino.h>

class ISensor
{
public:
  virtual ~ISensor() = default;
  virtual bool begin() = 0;
  virtual bool isHealthy() const = 0;
};

class VirtualSensor final : public ISensor
{
public:
  bool begin() override;
  bool isHealthy() const override;

private:
  bool initialized = false;
};

class SensorManager
{
public:
  bool beginAndCheck();

private:
  VirtualSensor virtualSensor;
};
