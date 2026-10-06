#include "sensorManager.h"

bool VirtualSensor::begin()
{
  initialized = true;
  return true;
}

bool VirtualSensor::isHealthy() const
{
  return initialized;
}

bool SensorManager::beginAndCheck()
{
  return virtualSensor.begin() && virtualSensor.isHealthy();
}
