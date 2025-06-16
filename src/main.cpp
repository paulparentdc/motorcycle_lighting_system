#include "main.h"

#include <Arduino.h>
#include <Tlc5940.h>

#include "configuration.h"
#include "Blinker.h"
#include "Headlight.h"

// Blinker objects
Blinker frontLeft(7, 6, 5, 4, 3);
Blinker frontRight(12, 11, 10, 9, 8);
Blinker backLeft(28, 27, 26, 25, 24);
Blinker backRight(23, 22, 21, 20, 19);

BlinkersState readBlinkersSwitch()
{
  int val = analogRead(BLINKER_SWITCH_PIN);
  if (val < LEFT_BLINKER_THRESHOLD)
    return BLINKERSSWITCH_TURN_LEFT;
  else if (val < RIGHT_BLINKER_THRESHOLD)
    return BLINKERSSWITCH_NONE;
  else
    return BLINKERSSWITCH_TURN_RIGHT;
}

HeadlightSwitch readHeadlightSwitch()
{
  int val = analogRead(HEADLIGHT_SWITCH_PIN);
  if (val < HEADLIGHT_OFF_THRESHOLD)
    return HEADLIGHTSWITCH_OFF;
  else if (val < HEADLIGHT_NORMAL_THRESHOLD)
    return HEADLIGHTSWITCH_NORMAL;
  else
    return HEADLIGHTSWITCH_FULL;
}

WarningButton readWarningButton()
{
  // Read the state of the warning button
  int val = digitalRead(WARNING_BUTTON_PIN);
  return (val == HIGH) ? WARNING_ON : WARNING_OFF; // Assuming HIGH means pressed
}

void startupSequence()
{
  // Light up the LEDs one by one
  for (int i = 0; i < STARTUP_LED_COUNT; ++i)
  {
    Tlc.set(BLINKER_STARTUP_SEQUENCE[i], MAX_POWER);
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
      Tlc.set(BLINKER_STARTUP_SEQUENCE[i], value);
    }
    Tlc.update();
    delay(STARTUP_FADE_DELAY);
  }

  // Ensure everything is off at the end
  for (int i = 0; i < STARTUP_LED_COUNT; ++i)
  {
    Tlc.set(BLINKER_STARTUP_SEQUENCE[i], 0);
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
  startupSequence();

  // Initialize all blinkers
  frontLeft.initialize();
  frontRight.initialize();
  backLeft.initialize();
  backRight.initialize();

  // Set pin modes for the blinker control switch
  pinMode(BLINKER_SWITCH_PIN, INPUT);

  // Set pin modes for the headlight switch
  pinMode(HEADLIGHT_SWITCH_PIN, INPUT);
}

void loop()
{
  // Read inputs
  BlinkersState blinkersSwitch = readBlinkersSwitch();
  HeadlightSwitch headlightsSwitch = readHeadlightSwitch();
  WarningButton warningButton = readWarningButton();

  // Control blinkers based on the switch state
  if (blinkersSwitch == BLINKERSSWITCH_TURN_LEFT && frontLeft.getState() == IDLE && backLeft.getState() == IDLE)
  {
    frontLeft.start();
    backLeft.start();
    frontRight.stop();
    backRight.stop();
  }
  else if (blinkersSwitch == BLINKERSSWITCH_TURN_RIGHT && frontRight.getState() == IDLE && backRight.getState() == IDLE)
  {
    frontRight.start();
    backRight.start();
    frontLeft.stop();
    backLeft.stop();
  }
  else if (blinkersSwitch == BLINKERSSWITCH_NONE)
  {
    frontLeft.stop();
    frontRight.stop();
    backLeft.stop();
    backRight.stop();
  }

  // Control headlights based on the switch state

  // Update all blinkers
  frontLeft.update();
  frontRight.update();
  backLeft.update();
  backRight.update();
}