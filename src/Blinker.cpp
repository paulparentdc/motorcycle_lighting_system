#include "Blinker.h"
#include <Tlc5940.h>
#include "configuration.h"

Blinker::Blinker(int frontPin1, int frontPin2, int frontPin3, int frontPin4, int frontPin5,
                 int backPin1, int backPin2, int backPin3, int backPin4, int backPin5,
                 int dashboardPin) : m_dashboardPin(dashboardPin)
{
    m_frontPins[0] = frontPin1;
    m_frontPins[1] = frontPin2;
    m_frontPins[2] = frontPin3;
    m_frontPins[3] = frontPin4;
    m_frontPins[4] = frontPin5;

    m_backPins[0] = backPin1;
    m_backPins[1] = backPin2;
    m_backPins[2] = backPin3;
    m_backPins[3] = backPin4;
    m_backPins[4] = backPin5;
}

void Blinker::initialize()
{
    for (int i = 0; i < 5; i++)
    {
        // Initialize all LEDs to off
        Tlc.set(m_frontPins[i], 0);
        Tlc.set(m_backPins[i], 0);
    }
    Tlc.update();
}

void Blinker::start()
{
    m_state = SEQUENCING;
    m_currentLedRow = 0;
    m_lastUpdate = millis();
    m_fadeValue = MAX_POWER;

    // Turn off all LEDs first
    for (int i = 0; i < 5; i++)
    {
        Tlc.set(m_frontPins[i], 0);
        Tlc.set(m_backPins[i], 0);
    }

    Tlc.update();
}

void Blinker::update()
{
    unsigned long now = millis();
    switch (m_state)
    {
    case SEQUENCING:
        if (now - m_lastUpdate >= SEQUENCING_SPEED)
        {
            if (m_currentLedRow < 5)
            {
                Tlc.set(m_frontPins[m_currentLedRow], MAX_POWER);
                Tlc.set(m_backPins[m_currentLedRow], MAX_POWER);
                m_currentLedRow++;
                m_lastUpdate = now;
                Tlc.update();
            }
            else
            {
                m_state = FADING;
                m_fadeValue = MAX_POWER;
                m_lastUpdate = now;
            }
        }
        break;

    case FADING:
        if (now - m_lastUpdate >= FADE_OUT_DELAY)
        {
            if (m_fadeValue > 0)
            {
                m_fadeValue -= FADE_OUT_STEP;
                if (m_fadeValue < 0)
                    m_fadeValue = 0;

                for (int i = 0; i < 5; i++)
                {
                    Tlc.set(m_frontPins[i], m_fadeValue);
                    Tlc.set(m_backPins[i], m_fadeValue);
                }

                m_lastUpdate = now;
                Tlc.update();
            }
            else
            {
                for (int i = 0; i < 5; i++)
                {
                    Tlc.set(m_frontPins[i], 0);
                    Tlc.set(m_backPins[i], 0);
                }
                Tlc.update();
                m_state = IDLE; // Reset to IDLE after fading out
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
    m_state = IDLE;
    for (int i = 0; i < 5; i++)
    {
        Tlc.set(m_frontPins[i], 0);
        Tlc.set(m_backPins[i], 0);
    }

    Tlc.update();
}

BlinkerState Blinker::getState() const
{
    return m_state;
}