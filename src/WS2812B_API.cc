#include "WS2812B_API.hh"

enum led_logic_timing
{
    T0H = 400, // 0 logic, high time
    T1H = 850, // 1 logic, high time
    T0L = 800, // 0 logic, low time
    T1L = 450, // 1 logic, low time
    RES = 50000 // Reset time >= 50000
};