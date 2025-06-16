class LedStrip; // Forward declaration

enum LightsMode
{
    LIGHTS_OFF,       // All lights are off
    LIGHTS_OFF_FLASH, // Same, but high beam is on (usually used for flashing)
    LIGHTS_DAY,       // Dashboard, plate and front LED strip are on, full power
    LIGHTS_DAY_FLASH, // Same, but high beam is on (usually used for flashing)
    LIGHTS_CITY,      // Dashboard, plate and front LED strip are on, low power. Front and back lights are on, low power
    LIGHTS_COUNTRY    // Same, but front light is full beam
};

class Lights
{
private:
    int m_headlightPin = 0; // Pin connected to the relay that turns the headlight ON or OFF (0 = OFF, 1 = ON)
    int m_backLightPin = 0; // Pin connected to the relay that turns the back light ON or OFF (0 = OFF, 1 = ON)
    int m_highBeamPin = 0;  // Pin connected to the relay that selects the headlight mode (0 = NORMAL beam, 1 = FULL beam)

    LedStrip &m_frontLightStrip; // LED strip inside the headlight, used as daytime running light
    LedStrip &m_dashboardStrip;  // LED strip on the dashboard
    LedStrip &m_plateStrip;      // LED strip on the license plate

public:
    Lights(int powerPin, int modePin, int backLightPin, LedStrip &frontLightStrip, LedStrip &dashboardStrip, LedStrip &plateStrip);

    void initialize();
    void set(LightsMode mode);
};