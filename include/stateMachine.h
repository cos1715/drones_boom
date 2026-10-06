#pragma once

#include <Arduino.h>
#include "state.h"

class StateMachine
{
private:
    State state;
    unsigned long timestamp;
    static const unsigned long WAITING_TIMEOUT_MS = 5000; // 5 seconds
    void changeState(State newState);

public:
    StateMachine();

    State getCurrentState() const;
    unsigned long getRemainingSafetyTimeMs() const;

    void begin(bool systemCheckResult);
    void start();
    void stop();
    void trigger();
    void update();
    void fail();
};
