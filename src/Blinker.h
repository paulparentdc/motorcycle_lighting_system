#include <Arduino.h>

enum BlinkerState
{
    IDLE,       // No blinker active
    SEQUENCING, // Blinker is lighting up LEDs in sequence
    FADING      // Blinker is fading out LEDs
};

class Blinker
{
private:
    int pins[5];
    BlinkerState state = IDLE;
    int currentLedRow = 0;
    unsigned long lastUpdate = 0;
    int fadeValue = 255;

public:
    Blinker(int pin1, int pin2, int pin3, int pin4, int pin5);
    void start();
    void initialize();
    void stop();
    void update();
    BlinkerState getState() const;
};