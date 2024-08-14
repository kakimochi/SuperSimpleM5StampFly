#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define PIN_LED 21  // G21 on the M5StampS3

CRGB leds[NUM_LEDS];

void setup()
{
    USBSerial.begin(115200);

    FastLED.addLeds<WS2812, PIN_LED, GRB>(leds, NUM_LEDS);
    USBSerial.println("[info] init done.");
}

void loop()
{
    USBSerial.println("[info] Hello World!");

    auto colors = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White, CRGB::Black};

    for(auto color : colors)
    {
        leds[0] = color;
        FastLED.show();
        delay(1000);
    }

}
