enum BlinkersState
{
    TURN_LEFT,  // Left blinker
    TURN_RIGHT, // Right blinker
    NONE        // No blinker active
};

enum HeadlightSwitch
{
    EXTINGUISHED, // Headlights extinguished
    NORMAL,       // Headlights normal
    FULL          // Headlights full
};

typedef enum
{
    OFF,
    ON
} ButtonState;
