#include "Headlight.h"
#include <Arduino.h>

Headlight::Headlight(int powerPin, int modePin, int ledstripRedPin, int ledstripGreenPin, int ledstripBluePin)
    : m_powerPin(powerPin), m_modePin(modePin), m_ledstripRedPin(ledstripRedPin), m_ledstripGreenPin(ledstripGreenPin), m_ledstripBluePin(ledstripBluePin)
{
    pinMode(m_powerPin, OUTPUT);
    pinMode(m_modePin, OUTPUT);
    pinMode(m_ledstripRedPin, OUTPUT);
    pinMode(m_ledstripGreenPin, OUTPUT);
    pinMode(m_ledstripBluePin, OUTPUT);
}

void Headlight::set(HeadlightMode mode)
{
    m_headlightMode = mode;
    switch (mode)
    {
    case HEADLIGHT_OFF:
        // Code to turn off the headlight
        break;
    case HEADLIGHT_NORMAL:
        // Code to set the headlight to normal mode
        break;
    case HEADLIGHT_FULL:
        // Code to set the headlight to full mode
        break;
    }
}

HeadlightMode Headlight::getHeadlightMode() const
{
    return m_headlightMode;
}