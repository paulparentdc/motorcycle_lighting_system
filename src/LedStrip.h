#include "configuration.h"

class LedStrip
{
private:
    int m_redPin;         // Pin connected to the red channel of the LED strip
    int m_greenPin;       // Pin connected to the green channel of the LED strip
    int m_bluePin;        // Pin connected to the blue channel of the LED strip
    int m_redValue = 0;   // Current value for the red channel (0-255)
    int m_greenValue = 0; // Current value for the green channel (0-255)
    int m_blueValue = 0;  // Current value for the blue channel (0-255)
public:
    LedStrip(int redPin, int greenPin, int bluePin);

    void setColor(int red, int green, int blue, int brightness = 255);
    void setColor(Color color, int brightness = 255);
};