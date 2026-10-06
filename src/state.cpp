#include "stateMachine.h"

const char *stateToString(State state)
{
  switch (state)
  {
  case State::INIT:
    return "INIT";
  case State::SAFE:
    return "SAFE";
  case State::WAITING:
    return "WAITING";
  case State::ARM:
    return "ARM";
  case State::TRIGGERED:
    return "TRIGGERED";
  case State::ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}