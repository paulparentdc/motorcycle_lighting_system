#include "Lights.h"

#include <Arduino.h>

#include "LedStrip.h"
#include "configuration.h"

Lights::Lights(int headlightPin, int highbeamPin, int backLightPin, LedStrip &frontLightStrip, LedStrip &dashboardStrip, LedStrip &plateStrip)
    : m_headlightPin(headlightPin),
      m_backLightPin(backLightPin),
      m_highBeamPin(highbeamPin),
      m_frontLightStrip(frontLightStrip),
      m_dashboardStrip(dashboardStrip),
      m_plateStrip(plateStrip)
{
    pinMode(m_headlightPin, OUTPUT);
    pinMode(m_highBeamPin, OUTPUT);
    pinMode(m_backLightPin, OUTPUT);
}

void Lights::initialize()
{
    // Set initial lights mode
    set(LIGHTS_OFF);
}

void Lights::set(LightsMode mode)
{
    switch (mode)
    {
    case LIGHTS_OFF:
        digitalWrite(m_headlightPin, LOW);     // Headlight OFF
        digitalWrite(m_backLightPin, LOW);     // Back light OFF
        digitalWrite(m_highBeamPin, LOW);      // High beam OFF
        m_frontLightStrip.setColor(COLOR_OFF); // Front LED strip OFF
        m_dashboardStrip.setColor(COLOR_OFF);  // Dashboard LED strip OFF
        m_plateStrip.setColor(COLOR_OFF);      // Plate LED strip OFF
        break;
    case LIGHTS_OFF_FLASH:
        digitalWrite(m_headlightPin, HIGH);    // Headlight ON, for high beam flash
        digitalWrite(m_backLightPin, LOW);     // Back light OFF
        digitalWrite(m_highBeamPin, HIGH);     // High beam ON (flash)
        m_frontLightStrip.setColor(COLOR_OFF); // Front LED strip OFF
        m_dashboardStrip.setColor(COLOR_OFF);  // Dashboard LED strip OFF
        m_plateStrip.setColor(COLOR_OFF);      // Plate LED strip OFF
        break;
    case LIGHTS_DAY:
        digitalWrite(m_headlightPin, LOW);       // Headlight OFF
        digitalWrite(m_backLightPin, LOW);       // Back light OFF
        digitalWrite(m_highBeamPin, LOW);        // High beam OFF
        m_frontLightStrip.setColor(COLOR_WHITE); // Front LED strip WHITE (full power)
        m_dashboardStrip.setColor(COLOR_WHITE);  // Dashboard LED strip WHITE (full power)
        m_plateStrip.setColor(COLOR_WHITE);      // Plate LED strip WHITE (full power)
        break;
    case LIGHTS_DAY_FLASH:
        digitalWrite(m_headlightPin, HIGH);      // Headlight ON, for high beam flash
        digitalWrite(m_backLightPin, LOW);       // Back light OFF
        digitalWrite(m_highBeamPin, HIGH);       // High beam ON (flash)
        m_frontLightStrip.setColor(COLOR_WHITE); // Front LED strip WHITE (full power)
        m_dashboardStrip.setColor(COLOR_WHITE);  // Dashboard LED strip WHITE (full power)
        m_plateStrip.setColor(COLOR_WHITE);      // Plate LED strip WHITE (full power)
        break;
    case LIGHTS_CITY:
        digitalWrite(m_headlightPin, HIGH);                                         // Headlight ON
        digitalWrite(m_backLightPin, HIGH);                                         // Back light ON
        digitalWrite(m_highBeamPin, LOW);                                           // High beam OFF
        m_frontLightStrip.setColor(COLOR_OFF);                                      // Front LED strip off, only headlight is on
        m_dashboardStrip.setColor(CITY_DASHBOARD_COLOR, CITY_DASHBOARD_BRIGHTNESS); // Dashboard LED strip adapted to city mode
        m_plateStrip.setColor(CITY_PLATE_COLOR, CITY_PLATE_BRIGHTNESS);             // Plate LED strip adapted to city mode
        break;
    case LIGHTS_COUNTRY:
        digitalWrite(m_headlightPin, HIGH);                                         // Headlight ON, for high beam
        digitalWrite(m_backLightPin, HIGH);                                         // Back light ON
        digitalWrite(m_highBeamPin, HIGH);                                          // High beam ON
        m_frontLightStrip.setColor(COLOR_OFF);                                      // Front LED strip off, only high beam is on
        m_dashboardStrip.setColor(CITY_DASHBOARD_COLOR, CITY_DASHBOARD_BRIGHTNESS); // Dashboard LED strip adapted to city mode
        m_plateStrip.setColor(CITY_PLATE_COLOR, CITY_PLATE_BRIGHTNESS);             // Plate LED strip adapted to city mode
        break;
    }
}