#include <Arduino.h>

/*TLC5940 parameters*/
constexpr int MAX_POWER = 4095; /*TLC5940 max output value*/

/*Blinkers switch*/
constexpr int WARNING_BUTTON_PIN = A5;       /*Digital pin for warning control button*/
constexpr int BLINKER_SWITCH_PIN = A0;       /*Analog pin for blinker control switch*/
constexpr int LEFT_BLINKER_THRESHOLD = 400;  /*Analog value for left blinker control (when <)*/
constexpr int RIGHT_BLINKER_THRESHOLD = 600; /*Analog value for right blinker control (when >)*/

/*Blinkers parameters*/
constexpr int BLINKER_STARTUP_SEQUENCE[] = {3, 4, 5, 6, 7,                   /*Front left blinker LEDs*/
                                            12, 11, 10, 9, 8,                /*Front right blinker LEDs*/
                                            19, 20, 21, 22, 23,              /*Back right blinker LEDs*/
                                            28, 27, 26, 25, 24};             /*Back left blinker LEDs*/
constexpr int MAX_INTENSITY = 255;                                           /*Maximum intensity of the LEDs*/
constexpr int SEQUENCING_SPEED = 100;                                        /*Time to light up one LED row, in ms*/
constexpr int FADE_OUT_SPEED = 1500;                                         /*Time to fade out from max intensity to zero, in ms*/
constexpr int FADE_OUT_STEP = 100;                                           /*Power difference from one step to an other when fading out*/
constexpr int FADE_OUT_DELAY = FADE_OUT_SPEED / (MAX_POWER / FADE_OUT_STEP); /*Delay value for fading out, in ms*/

/*Headlight switch*/
constexpr int HEADLIGHT_SWITCH_PIN = A3;        /*Analog pin for headlight switch*/
constexpr int HEADLIGHT_OFF_THRESHOLD = 300;    /*Analog value for OFF (when <)*/
constexpr int HEADLIGHT_NORMAL_THRESHOLD = 700; /*Analog value for NORMAL (when <) and FULL (when >)*/

/* Startup sequence */
constexpr int STARTUP_LED_COUNT = sizeof(BLINKER_STARTUP_SEQUENCE) / sizeof(BLINKER_STARTUP_SEQUENCE[0]);
constexpr int STARTUP_FADE_STEPS = 20;
constexpr int STARTUP_FADE_DELAY = 1000 / STARTUP_FADE_STEPS; // 1s / nombre d'étapes
constexpr int STARTUP_SEQUENCING_SPEED = 200;                 // ms between each LED in the startup sequence