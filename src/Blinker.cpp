#include "Blinker.h"
#include <Tlc5940.h>
#include "configuration.h"

Blinker::Blinker(int pin1, int pin2, int pin3, int pin4, int pin5)
{
    m_pins[0] = pin1;
    m_pins[1] = pin2;
    m_pins[2] = pin3;
    m_pins[3] = pin4;
    m_pins[4] = pin5;
}

void Blinker::initialize()
{
    for (int i = 0; i < 5; i++)
    {
        pinMode(m_pins[i], OUTPUT);
        Tlc.set(m_pins[i], 0); // Initialize all LEDs to off
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
        Tlc.set(m_pins[i], 0);
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
                Tlc.set(m_pins[m_currentLedRow], MAX_POWER);
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
                    Tlc.set(m_pins[i], m_fadeValue);

                m_lastUpdate = now;
                Tlc.update();
            }
            else
            {
                for (int i = 0; i < 5; i++)
                    Tlc.set(m_pins[i], 0);
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
        Tlc.set(m_pins[i], 0);
    Tlc.update();
}

BlinkerState Blinker::getState() const
{
    return m_state;
}

int *Blinker::getPins() const
{
    return (int *)m_pins;
}