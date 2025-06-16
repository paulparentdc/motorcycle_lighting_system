enum HeadlightMode
{
    HEADLIGHT_OFF,
    HEADLIGHT_NORMAL,
    HEADLIGHT_FULL
};

class Headlight
{
private:
    int m_powerPin;             // Pin connected to the relay that turns the headlight ON or OFF (0 = OFF, 1 = ON)
    int m_modePin;              // Pin connected to the relay that selects the headlight mode (0 = NORMAL beam, 1 = FULL beam)
    int m_ledstripRedPin = 0;   // Pin connected to the red channel of the LED strip inside the headlight (used as daytime running light)
    int m_ledstripGreenPin = 0; // Pin connected to the green channel of the LED strip inside the headlight (used as daytime running light)
    int m_ledstripBluePin = 0;  // Pin connected to the blue channel of the LED strip inside the headlight (used as daytime running light)
    HeadlightMode m_headlightMode = HEADLIGHT_OFF;

public:
    Headlight(int powerPin, int modePin, int ledstripRedPin, int ledstripGreenPin, int ledstripBluePin);
    void set(HeadlightMode mode);
    HeadlightMode getHeadlightMode() const;
};