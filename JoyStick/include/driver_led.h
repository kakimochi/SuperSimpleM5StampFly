#pragma once

#include <vector>
#include <FastLED.h>

#define NUM_LEDS 2
#define PIN_LED_JOYSTICK 6 // G6 on the M5AtomS3 Joystick

namespace LED
{
    std::vector<CRGB> colors = {
        CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Purple,
        CRGB::Cyan, CRGB::White, CRGB::Orange, CRGB::Magenta, CRGB::Lime,
        CRGB::Pink, CRGB::Teal, CRGB::Gold, CRGB::Indigo, CRGB::Silver,
        CRGB::Black, CRGB::Gray, CRGB::Brown, CRGB::Maroon, CRGB::Navy,
        CRGB::Olive, CRGB::SkyBlue, CRGB::SlateGray, CRGB::DarkGreen,
        CRGB::DarkOrange, CRGB::DarkViolet, CRGB::DarkRed, CRGB::DarkBlue,
        CRGB::DarkCyan, CRGB::DarkMagenta};

    CRGB leds[NUM_LEDS];

    void init()
    {
        FastLED.addLeds<WS2812, PIN_LED_JOYSTICK, GRB>(leds, NUM_LEDS);
        FastLED.setBrightness(50);
    }

    void update()
    {
        FastLED.show();
    }

    void setColor(uint8_t index, CRGB color)
    {
        if (index < NUM_LEDS)
        {
            leds[index] = color;
        }
    }

    void color_rotation()
    {
        static int color_index = 0;
        auto color = colors[color_index];
        LED::setColor(0, color);
        LED::setColor(1, color);
        // USBSerial.printf("[info] color_code : 0x%06x\n", color);

        if (color_index >= colors.size())
        {
            color_index = 0;
        }
        else
        {
            color_index++;
        }
    }

} // namespace LED