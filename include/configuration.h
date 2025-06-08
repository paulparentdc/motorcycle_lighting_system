#include <Arduino.h>

/*TLC5940 parameters*/
constexpr int MAX_POWER = 4095;                    /*TLC5940 max output value*/
constexpr int blinkersLedOrder[] = {3, 4, 5, 6, 7, /*blinkers' LEDs outputs on TLC module, clockwise order beginning front left*/
                                    12, 11, 10, 9, 8,
                                    19, 20, 21, 22, 23,
                                    28, 27, 26, 25, 24};

/*Blinkers switch*/
constexpr int WARNING_BUTTON_PIN = A5;       /*Digital pin for warning control button*/
constexpr int BLINKER_SWITCH_PIN = A0;       /*Analog pin for blinker control switch*/
constexpr int LEFT_BLINKER_THRESHOLD = 400;  /*Analog value for left blinker control (when <)*/
constexpr int RIGHT_BLINKER_THRESHOLD = 600; /*Analog value for right blinker control (when >)*/

/*Blinkers parameters*/
constexpr int MAX_INTENSITY = 255;                                           /*Maximum intensity of the LEDs*/
constexpr int SEQUENCING_SPEED = 100;                                        /*Time to light up one LED row, in ms*/
constexpr int FADE_OUT_SPEED = 1500;                                         /*Time to fade out from max intensity to zero, in ms*/
constexpr int FADE_OUT_STEP = 100;                                           /*Power difference from one step to an other when fading out*/
constexpr int FADE_OUT_DELAY = FADE_OUT_SPEED / (MAX_POWER / FADE_OUT_STEP); /*Delay value for fading out, in ms*/

/*Headlight switch*/
constexpr int HEADLIGHT_SWITCH_PIN = A3;        /*Analog pin for headlight switch*/
constexpr int HEADLIGHT_OFF_THRESHOLD = 300;    /*Analog value for OFF (when <)*/
constexpr int HEADLIGHT_NORMAL_THRESHOLD = 700; /*Analog value for NORMAL (when <) and FULL (when >)*/