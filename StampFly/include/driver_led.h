#pragma once

#include <vector>
#include <FastLED.h>

#define NUM_LEDS_STAMPS3 1
#define NUM_LEDS_STAMPFLY 2
#define PIN_LED_STAMPS3 21  // G21 on the M5StampS3
#define PIN_LED_STAMPFLY 39 // G39 on the M5StampFly

namespace LED
{
    enum {
        LED_ON_UPPER = 0,
        LED_ON_BOTTOM,
        LED_ON_STAMPS3,
        NUM_LEDS
    };

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
        FastLED.addLeds<WS2812, PIN_LED_STAMPS3, GRB>(&leds[LED_ON_STAMPS3], NUM_LEDS_STAMPS3);
        FastLED.addLeds<WS2812, PIN_LED_STAMPFLY, GRB>(&leds[LED_ON_UPPER], NUM_LEDS_STAMPFLY);
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
        LED::setColor(LED_ON_UPPER, color);
        LED::setColor(LED_ON_BOTTOM, color);
        LED::setColor(LED_ON_STAMPS3, color);
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