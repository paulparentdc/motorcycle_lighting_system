#include "Blinker.h"
#include "configuration.h"

Blinker::Blinker(int pin1, int pin2, int pin3, int pin4, int pin5)
{
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    pins[4] = pin5;
}

void Blinker::initialize()
{
    for (int i = 0; i < 5; i++)
    {
        pinMode(pins[i], OUTPUT);
        analogWrite(pins[i], 0); // Initialize all LEDs to off
    }
}

void Blinker::start()
{
    state = SEQUENCING;
    currentLedRow = 0;
    lastUpdate = millis();
    fadeValue = MAX_INTENSITY;

    // Turn off all LEDs first
    for (int i = 0; i < 5; i++)
        analogWrite(pins[i], 0);
}

void Blinker::update()
{
    unsigned long now = millis();
    switch (state)
    {
    case SEQUENCING:
        if (now - lastUpdate >= SEQUENCING_SPEED)
        {
            if (currentLedRow < 5)
            {
                analogWrite(pins[currentLedRow], MAX_INTENSITY);
                currentLedRow++;
                lastUpdate = now;
            }
            else
            {
                state = FADING;
                fadeValue = MAX_INTENSITY;
                lastUpdate = now;
            }
        }
        break;

    case FADING:
        if (now - lastUpdate >= FADE_OUT_DELAY)
        {
            if (fadeValue > 0)
            {
                fadeValue -= FADE_OUT_STEP;
                if (fadeValue < 0)
                    fadeValue = 0;

                for (int i = 0; i < 5; i++)
                    analogWrite(pins[i], fadeValue);

                lastUpdate = now;
            }
            else
            {
                for (int i = 0; i < 5; i++)
                    analogWrite(pins[i], 0);
                state = IDLE; // Reset to IDLE after fading out
            }
        }
        break;

    case IDLE:
        // Do nothing, waiting for start
        break;
    }
}

void Blinker::stop()
{
    state = IDLE;
    for (int i = 0; i < 5; i++)
        analogWrite(pins[i], 0);
}

BlinkerState Blinker::getState() const
{
    return state;
}
