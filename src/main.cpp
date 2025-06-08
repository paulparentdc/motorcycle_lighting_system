#include <Arduino.h>
#include <main.h>
#include "Blinker.h"
#include "configuration.h"

// Blinker objects
Blinker frontLeft(7, 6, 5, 4, 3);
Blinker frontRight(12, 11, 10, 9, 8);
Blinker backLeft(28, 27, 26, 25, 24);
Blinker backRight(23, 22, 21, 20, 19);

BlinkersState readBlinkersSwitch()
{
  int val = analogRead(BLINKER_SWITCH_PIN);
  if (val < LEFT_BLINKER_THRESHOLD)
    return TURN_LEFT;
  else if (val < RIGHT_BLINKER_THRESHOLD)
    return NONE;
  else
    return TURN_RIGHT;
}

HeadlightSwitch readHeadlightSwitch()
{
  int val = analogRead(HEADLIGHT_SWITCH_PIN);
  if (val < HEADLIGHT_OFF_THRESHOLD)
    return EXTINGUISHED;
  else if (val < HEADLIGHT_NORMAL_THRESHOLD)
    return NORMAL;
  else
    return FULL;
}

ButtonState readWarningButton()
{
  // Read the state of the warning button
  int val = digitalRead(WARNING_BUTTON_PIN);
  return (val == HIGH) ? ON : OFF; // Assuming HIGH means pressed
}

void setup()
{
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
  BlinkersState blinkersState = readBlinkersSwitch();
  HeadlightSwitch headlightState = readHeadlightSwitch();
  ButtonState warningButtonState = readWarningButton();

  // Control blinkers based on the switch state
  if (blinkersState == TURN_LEFT && frontLeft.getState() == IDLE && backLeft.getState() == IDLE)
  {
    frontLeft.start();
    backLeft.start();
    frontRight.stop();
    backRight.stop();
  }
  else if (blinkersState == TURN_RIGHT && frontRight.getState() == IDLE && backRight.getState() == IDLE)
  {
    frontRight.start();
    backRight.start();
    frontLeft.stop();
    backLeft.stop();
  }
  else if (blinkersState == NONE)
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