#include "LedStrip.h"
#include <Arduino.h>
#include <Tlc5940.h>
#include "configuration.h"

LedStrip::LedStrip(int redPin, int greenPin, int bluePin)
    : m_redPin(redPin), m_greenPin(greenPin), m_bluePin(bluePin)
{
    pinMode(m_redPin, OUTPUT);
    pinMode(m_greenPin, OUTPUT);
    pinMode(m_bluePin, OUTPUT);
}

void LedStrip::setColor(int red, int green, int blue, int brightness)
{
    // Ensure brightness is within 0-255 range
    brightness = constrain(brightness, 0, 255);

    // Scale the RGB values based on the brightness
    red = (red * brightness) / 255;
    green = (green * brightness) / 255;
    blue = (blue * brightness) / 255;

    // Constrain the RGB values to be within 0-255
    m_redValue = constrain(red, 0, 255);
    m_greenValue = constrain(green, 0, 255);
    m_blueValue = constrain(blue, 0, 255);

    // Set the color values to the TLC5940
    // Scale the values to the TLC5940's range (0 to MAX_POWER)
    Tlc.set(m_redPin, (m_redValue * MAX_POWER) / 255);
    Tlc.set(m_greenPin, (m_greenValue * MAX_POWER) / 255);
    Tlc.set(m_bluePin, (m_blueValue * MAX_POWER) / 255);
    Tlc.update();
}

void LedStrip::setColor(Color color, int brightness)
{
    // Ensure brightness is within 0-255 range
    brightness = constrain(brightness, 0, 255);

    // Set the color based on the enum value
    switch (color)
    {
    case COLOR_RED:
        setColor(255, 0, 0, brightness);
        break;
    case COLOR_GREEN:
        setColor(0, 255, 0, brightness);
        break;
    case COLOR_BLUE:
        setColor(0, 0, 255, brightness);
        break;
    case COLOR_YELLOW:
        setColor(255, 255, 0, brightness);
        break;
    case COLOR_CYAN:
        setColor(0, 255, 255, brightness);
        break;
    case COLOR_MAGENTA:
        setColor(255, 0, 255, brightness);
        break;
    case COLOR_WHITE:
        setColor(255, 255, 255, brightness);
        break;
    case COLOR_OFF:
    default:
        setColor(0, 0, 0, brightness);
        break;
    }
}