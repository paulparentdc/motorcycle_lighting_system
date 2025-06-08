

class Headlight
{
private:
    int headlightSwitchPin;
    bool headlightOn = false;
    bool headlightFull = false;

public:
    Headlight(int headlightSwitchPin, int headlightOffThreshold, int headlightNormalThreshold);
    void update();
    bool isHeadlightOn() const;
    bool isHeadlightFull() const;
}