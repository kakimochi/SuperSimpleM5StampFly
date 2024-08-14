#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS_JOYSTICK 2
#define PIN_LED_JOYSTICK 6  // G6 on the M5AtomS3 Joystick
CRGB leds_joystick[NUM_LEDS_JOYSTICK];

void setup()
{
    USBSerial.begin(115200);

    FastLED.addLeds<WS2812, PIN_LED_JOYSTICK, GRB>(leds_joystick, NUM_LEDS_JOYSTICK);

    USBSerial.println("[info] init done.");
}

void loop()
{
    USBSerial.println("[info] Hello JoyStick!");

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
        leds_joystick[0] = color;
        leds_joystick[1] = color;
        FastLED.show();

        USBSerial.printf("[info] color_code : 0x%06x\n", color);
        delay(300);
    }
}
