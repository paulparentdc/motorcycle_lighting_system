#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>

enum Color
{
    COLOR_RED = 0,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_WHITE,
    COLOR_OFF
};

/*TLC5940 parameters*/
constexpr int MAX_POWER = 4095; /*TLC5940 max output value*/

/*Blinkers switch*/
constexpr int WARNING_BUTTON_PIN = A5;       /*Arduino digital pin for warning control button*/
constexpr int BLINKER_SWITCH_PIN = A0;       /*Arduino analog pin for blinker control switch*/
constexpr int LEFT_BLINKER_THRESHOLD = 600;  /*Analog value for left blinker control (when <)*/
constexpr int RIGHT_BLINKER_THRESHOLD = 250; /*Analog value for right blinker control (when <)*/

/*Blinkers parameters*/
constexpr int BLINKERS_STARTUP_SEQUENCE[] = {3, 4, 5, 6, 7,                  /*Front left blinker LEDs*/
                                             12, 11, 10, 9, 8,               /*Front right blinker LEDs*/
                                             19, 20, 21, 22, 23,             /*Back right blinker LEDs*/
                                             28, 27, 26, 25, 24};            /*Back left blinker LEDs*/
constexpr int MAX_INTENSITY = 255;                                           /*Maximum intensity of the LEDs*/
constexpr int SEQUENCING_SPEED = 80;                                         /*Time to light up one LED row, in ms*/
constexpr int FADE_OUT_SPEED = 400;                                          /*Time to fade out from max intensity to zero, in ms*/
constexpr int FADE_OUT_STEP = 100;                                           /*Power difference from one step to an other when fading out*/
constexpr int FADE_OUT_DELAY = FADE_OUT_SPEED / (MAX_POWER / FADE_OUT_STEP); /*Delay value for fading out, in ms*/

/*Lights inputs*/
constexpr int LIGHTS_SWITCH_PIN = A3;       /*Arduino analog pin for headlight switch*/
constexpr int HIGH_BEAM_SWITCH_PIN = A4;    /*Arduino digital pin for high beam switch*/
constexpr int LIGHTS_OFF_THRESHOLD = 300;   /*Analog value for OFF (when <)*/
constexpr int LIGHTS_NIGHT_THRESHOLD = 700; /*Analog value for DAY (when <) and NIGHT (when >)*/

/* Lights outputs */
constexpr int LIGHTS_POWER_PIN = 5;     /*Arduino pin for lights power relay (2) (0 = OFF, 1 = ON)*/
constexpr int LIGHTS_BACKLIGHT_PIN = 7; /*Arduino pin for back light relay (1) (0 = OFF, 1 = ON)*/
constexpr int HEADLIGHT_MODE_PIN = 6;   /*Arduino pin for head light mode relay (3) (0 = NORMAL beam, 1 = FULL beam)*/

constexpr int HEADLIGHT_LEDSTRIP_RED_PIN = 0;   /*TLC5940 pin for red channel of the LED strip inside the headlight*/
constexpr int HEADLIGHT_LEDSTRIP_GREEN_PIN = 1; /*TLC5940 pin for green channel of the LED strip inside the headlight*/
constexpr int HEADLIGHT_LEDSTRIP_BLUE_PIN = 2;  /*TLC5940 pin for blue channel of the LED strip inside the headlight*/

constexpr int DASHBOARD_LEDSTRIP_RED_PIN = 29;   /*TLC5940 pin for red channel of the LED strip on the dashboard*/
constexpr int DASHBOARD_LEDSTRIP_GREEN_PIN = 30; /*TLC5940 pin for green channel of the LED strip on the dashboard*/
constexpr int DASHBOARD_LEDSTRIP_BLUE_PIN = 31;  /*TLC5940 pin for blue channel of the LED strip on the dashboard*/

constexpr int PLATE_LEDSTRIP_RED_PIN = 13;   /*TLC5940 pin for red channel of the LED strip on the license plate*/
constexpr int PLATE_LEDSTRIP_GREEN_PIN = 14; /*TLC5940 pin for green channel of the LED strip on the license plate*/
constexpr int PLATE_LEDSTRIP_BLUE_PIN = 15;  /*TLC5940 pin for blue channel of the LED strip on the license plate*/

/*Lights parameters*/
constexpr int CITY_DASHBOARD_COLOR = COLOR_YELLOW; /*Color of the dashboard LED strip in city mode*/
constexpr int CITY_DASHBOARD_BRIGHTNESS = 64;      /*Brightness of the dashboard LED strip in city mode*/

constexpr int CITY_PLATE_COLOR = COLOR_WHITE; /*Color of the plate LED strip in city mode*/
constexpr int CITY_PLATE_BRIGHTNESS = 64;     /*Brightness of the plate LED strip in city mode*/

/* Startup sequence */
constexpr int STARTUP_LED_COUNT = sizeof(BLINKERS_STARTUP_SEQUENCE) / sizeof(BLINKERS_STARTUP_SEQUENCE[0]);
constexpr int STARTUP_FADE_STEPS = 20;
constexpr int STARTUP_FADE_DELAY = 1000 / STARTUP_FADE_STEPS; // 1s / nombre d'étapes
constexpr int STARTUP_SEQUENCING_SPEED = 100;                 // ms between each LED in the startup sequence

#endif // CONFIGURATION_H