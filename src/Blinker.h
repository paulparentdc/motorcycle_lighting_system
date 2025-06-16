enum BlinkerState
{
    IDLE,       // No blinker active
    SEQUENCING, // Blinker is lighting up LEDs in sequence
    FADING      // Blinker is fading out LEDs
};

class Blinker
{
private:
    int m_dashboardPin; // Pin for dashboard indicator
    int m_frontPins[5]; // Pins for front blinker LEDs
    int m_backPins[5];  // Pins for back blinker LEDs
    BlinkerState m_state = IDLE;
    int m_currentLedRow = 0;
    unsigned long m_lastUpdate = 0;
    int m_fadeValue = 255;

public:
    Blinker(int frontPin1, int frontPin2, int frontPin3, int frontPin4, int frontPin5,
            int backPin1, int backPin2, int backPin3, int backPin4, int backPin5,
            int dashboardPin);
    void start();
    void initialize();
    void stop();
    void update();
    BlinkerState getState() const;
};