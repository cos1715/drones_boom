#pragma once

#include <Arduino.h>

enum class State : uint8_t
{
  INIT = 0,
  SAFE = 1,
  WAITING = 2,
  ARM = 3,
  TRIGGERED = 4,
  ERROR = 5
};

const char *stateToString(State state);