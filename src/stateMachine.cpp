#include <state.h>
#include "stateMachine.h"

StateMachine::StateMachine()
{
  timestamp = 0;
  state = State::INIT;
}

State StateMachine::getCurrentState() const
{
  return state;
}

unsigned long StateMachine::getRemainingSafetyTimeMs() const
{
  if (state != State::WAITING)
  {
    return 0;
  }

  const unsigned long elapsed = millis() - timestamp;
  return elapsed >= WAITING_TIMEOUT_MS ? 0 : WAITING_TIMEOUT_MS - elapsed;
}

void StateMachine::begin(bool systemOk)
{
  if (state == State::INIT)
  {
    if (systemOk)
    {
      changeState(State::SAFE);
    }
    else
    {
      changeState(State::ERROR);
    }
  }
}

void StateMachine::start()
{
  if (state == State::SAFE)
  {
    changeState(State::WAITING);
    timestamp = millis();
  }
}

void StateMachine::stop()
{
  if (state == State::WAITING || state == State::ARM)
  {
    changeState(State::SAFE);
  }
}

void StateMachine::trigger()
{
  if (state == State::ARM)
  {
    changeState(State::TRIGGERED);
  }
}

void StateMachine::update()
{
  if (state == State::WAITING)
  {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - timestamp;

    if (elapsedTime >= WAITING_TIMEOUT_MS)
    {
      changeState(State::ARM);
      timestamp = 0;
    }
  }
}

void StateMachine::fail()
{
  if (state != State::ERROR)
  {
    changeState(State::ERROR);
  }
}

void StateMachine::changeState(State newState)
{
  Serial.print("State changed from ");
  Serial.print(stateToString(state));
  Serial.print(" to ");
  Serial.println(stateToString(newState));

  state = newState;
}
