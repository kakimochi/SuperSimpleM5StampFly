#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS_STAMPS3 1
#define NUM_LEDS_STAMPFLY 2
#define PIN_LED_STAMPS3 21  // G21 on the M5StampS3
#define PIN_LED_STAMPFLY 39 // G39 on the M5StampFly
CRGB leds_stamp3[NUM_LEDS_STAMPS3];
CRGB leds_stampfly[NUM_LEDS_STAMPFLY];

void setup()
{
    USBSerial.begin(115200);

    FastLED.addLeds<WS2812, PIN_LED_STAMPS3, GRB>(leds_stamp3, NUM_LEDS_STAMPS3);
    FastLED.addLeds<WS2812, PIN_LED_STAMPFLY, GRB>(leds_stampfly, NUM_LEDS_STAMPFLY);

    USBSerial.println("[info] init done.");
}

void loop()
{
    USBSerial.println("[info] Hello World!");

    auto colors = {
        CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Purple,
        CRGB::Cyan, CRGB::White, CRGB::Orange, CRGB::Magenta, CRGB::Lime,
        CRGB::Pink, CRGB::Teal, CRGB::Gold, CRGB::Indigo, CRGB::Silver,
        CRGB::Black, CRGB::Gray, CRGB::Brown, CRGB::Maroon, CRGB::Navy,
        CRGB::Olive, CRGB::SkyBlue, CRGB::SlateGray, CRGB::DarkGreen,
        CRGB::DarkOrange, CRGB::DarkViolet, CRGB::DarkRed, CRGB::DarkBlue,
        CRGB::DarkCyan, CRGB::DarkMagenta
    };

    for (auto color : colors)
    {
        leds_stamp3[0] = color;
        leds_stampfly[0] = color;
        leds_stampfly[1] = color;
        FastLED.show();

        USBSerial.printf("[info] color_code : 0x%06x\n", color);
        delay(300);
    }
}
