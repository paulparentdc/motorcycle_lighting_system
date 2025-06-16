enum BlinkerState
{
    IDLE,       // No blinker active
    SEQUENCING, // Blinker is lighting up LEDs in sequence
    FADING      // Blinker is fading out LEDs
};

class Blinker
{
private:
    int m_pins[5];
    BlinkerState m_state = IDLE;
    int m_currentLedRow = 0;
    unsigned long m_lastUpdate = 0;
    int m_fadeValue = 255;

public:
    Blinker(int pin1, int pin2, int pin3, int pin4, int pin5);
    void start();
    void initialize();
    void stop();
    void update();
    BlinkerState getState() const;
    int *getPins() const;
};