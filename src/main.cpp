#include "main.h"

#include <Arduino.h>
#include <Tlc5940.h>

#include "configuration.h"
#include "Blinker.h"
#include "Lights.h"
#include "LedStrip.h"

// Blinker objects
// These objects control the left and right blinkers of the vehicle
Blinker blinkersLeft(7, 6, 5, 4, 3, 28, 27, 26, 25, 24, 18);
Blinker blinkersRight(12, 11, 10, 9, 8, 23, 22, 21, 20, 19, 17);

// LedStrip objects
// These objects control the LED strips for different parts of the vehicle
LedStrip headlightStrip(HEADLIGHT_LEDSTRIP_RED_PIN, HEADLIGHT_LEDSTRIP_GREEN_PIN, HEADLIGHT_LEDSTRIP_BLUE_PIN);
LedStrip dashboardStrip(DASHBOARD_LEDSTRIP_RED_PIN, DASHBOARD_LEDSTRIP_GREEN_PIN, DASHBOARD_LEDSTRIP_BLUE_PIN);
LedStrip plateStrip(PLATE_LEDSTRIP_RED_PIN, PLATE_LEDSTRIP_GREEN_PIN, PLATE_LEDSTRIP_BLUE_PIN);

// Lights object
// This object controls the power to the lights and manages the LED strips
Lights lights(LIGHTS_POWER_PIN, HEADLIGHT_MODE_PIN, LIGHTS_BACKLIGHT_PIN,
              headlightStrip,
              dashboardStrip,
              plateStrip);

BlinkersState readBlinkersSwitch()
{
  int val = analogRead(BLINKER_SWITCH_PIN);

  if (val < RIGHT_BLINKER_THRESHOLD)
    return BLINKERSSWITCH_TURN_RIGHT;
  else if (val < LEFT_BLINKER_THRESHOLD)
    return BLINKERSSWITCH_TURN_LEFT;
  else
    return BLINKERSSWITCH_NONE;
}

LightsSwitch readLightsSwitch()
{
  int val = analogRead(LIGHTS_SWITCH_PIN);

  if (val < LIGHTS_OFF_THRESHOLD)
    return LIGHTSSWITCH_OFF;
  else if (val < LIGHTS_NIGHT_THRESHOLD)
    return LIGHTSSWITCH_DAY;
  else
    return LIGHTSSWITCH_NIGHT;
}

HighBeamSwitch readHighBeamSwitch()
{
  int val = digitalRead(HIGH_BEAM_SWITCH_PIN);

  return val == HIGH ? HIGH_BEAM_ON : HIGH_BEAM_OFF;
}

// Warning button state
// This variable keeps track of whether the warning button is currently active
bool warningActive = false;
int lastRawWarningButton = LOW;

bool readWarningButton()
{
  int val = digitalRead(WARNING_BUTTON_PIN);
  bool risingEdge = (val == HIGH && lastRawWarningButton == LOW);
  lastRawWarningButton = val;
  return risingEdge;
}

void startupSequence()
{
  // Light up the LEDs one by one
  for (int i = 0; i < STARTUP_LED_COUNT; ++i)
  {
    Tlc.set(BLINKERS_STARTUP_SEQUENCE[i], MAX_POWER);
    Tlc.update();
    delay(STARTUP_SEQUENCING_SPEED);
  }

  // Keep them on for 1 second
  delay(1000);

  // Fade out over 1 second
  for (int value = MAX_POWER; value >= 0; value -= (MAX_POWER / STARTUP_FADE_STEPS))
  {
    for (int i = 0; i < STARTUP_LED_COUNT; ++i)
    {
      Tlc.set(BLINKERS_STARTUP_SEQUENCE[i], value);
    }
    Tlc.update();
    delay(STARTUP_FADE_DELAY);
  }

  // Ensure everything is off at the end
  for (int i = 0; i < STARTUP_LED_COUNT; ++i)
  {
    Tlc.set(BLINKERS_STARTUP_SEQUENCE[i], 0);
  }
  Tlc.update();
}

void setup()
{
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize the Tlc5940 library
  Tlc.init();

  // Run the startup sequence
  // This will light up the LEDs in a sequence to indicate the system is starting
  // In the end, all LEDs will be turned off
  startupSequence();

  // Initialize everything
  blinkersLeft.initialize();
  blinkersRight.initialize();
  lights.initialize();
}

void loop()
{
  // Read inputs
  BlinkersState blinkersSwitch = readBlinkersSwitch();
  LightsSwitch lightsSwitch = readLightsSwitch();
  HighBeamSwitch highBeamSwitch = readHighBeamSwitch();
  bool warningButtonPressed = readWarningButton();

  if (warningButtonPressed)
  {
    warningActive = !warningActive;
    if (warningActive)
    {
      blinkersLeft.startWarning();
      blinkersRight.startWarning();
    }
    else
    {
      blinkersLeft.stop();
      blinkersRight.stop();
    }
  }

  // If warning is active, blinkers are managed by warning
  if (!warningActive)
  {
    // If the warning is not active, control blinkers based on the switch state
    if (blinkersSwitch == BLINKERSSWITCH_TURN_LEFT && blinkersLeft.getState() != SEQUENCING)
    {
      blinkersLeft.start();
      blinkersRight.stop();
    }
    else if (blinkersSwitch == BLINKERSSWITCH_TURN_RIGHT && blinkersRight.getState() != SEQUENCING)
    {
      blinkersRight.start();
      blinkersLeft.stop();
    }
    else if (blinkersSwitch == BLINKERSSWITCH_NONE)
    {
      blinkersLeft.stop();
      blinkersRight.stop();
    }
  }

  // Update all blinkers
  blinkersLeft.update();
  blinkersRight.update();

  // Control headlights based on the switches states
  if (lightsSwitch == LIGHTSSWITCH_OFF)
  {
    if (highBeamSwitch == HIGH_BEAM_ON)
    {
      lights.set(LIGHTS_OFF_FLASH); // Turn on only high beam
    }
    else
    {
      lights.set(LIGHTS_OFF); // Turn off all lights
    }
  }
  else if (lightsSwitch == LIGHTSSWITCH_DAY)
  {
    if (highBeamSwitch == HIGH_BEAM_ON)
    {
      lights.set(LIGHTS_DAY_FLASH); // Turn on high beam in day mode
    }
    else
    {
      lights.set(LIGHTS_DAY); // Turn on normal lights in day mode
    }
  }
  else if (lightsSwitch == LIGHTSSWITCH_NIGHT)
  {
    if (highBeamSwitch == HIGH_BEAM_ON)
    {
      lights.set(LIGHTS_COUNTRY); // Turn on high beam in night mode
    }
    else
    {
      lights.set(LIGHTS_CITY); // Turn on normal lights in night mode
    }
  }
}