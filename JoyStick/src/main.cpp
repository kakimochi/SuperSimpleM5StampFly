#include <Arduino.h>
#include <FastLED.h>
#include <M5GFX.h>

// LED
#define NUM_LEDS_JOYSTICK 2
#define PIN_LED_JOYSTICK 6 // G6 on the M5AtomS3 Joystick
CRGB leds_joystick[NUM_LEDS_JOYSTICK];

// Beep Sound
#define PIN_BUZZER 5       // G5 on the M5AtomS3 Joystick
#define TONE_C5 523 // 523.251
#define TONE_E5 659 // 659.255
#define TONE_G5 784 // 783.991
#define TONE_C6 (TONE_C5 * 2)
#define TONE_E6 (TONE_E5 * 2)
#define TONE_G6 (TONE_G5 * 2)

// Button
#define PIN_DISPLAY_BUTTON 41 // G41 on the M5AtomS3 Joystick

// GUI
M5GFX gfx;

void beep()
{
    tone(PIN_BUZZER, TONE_E5, 200);
}

void beep_init_done()
{
    tone(PIN_BUZZER, TONE_C5, 500);
    tone(PIN_BUZZER, TONE_E5, 500);
    tone(PIN_BUZZER, TONE_G5, 500);
}

void setup()
{
    USBSerial.begin(115200);

    // hardware config
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_DISPLAY_BUTTON, INPUT);

    // LED
    FastLED.addLeds<WS2812, PIN_LED_JOYSTICK, GRB>(leds_joystick, NUM_LEDS_JOYSTICK);
    FastLED.setBrightness(50);

    // GUI
    gfx.begin();
    gfx.fillScreen(TFT_BLACK);
    gfx.setTextSize(2);
    gfx.setTextColor(TFT_GOLD);
    gfx.setCursor(0, gfx.height() / 2);
    gfx.println("JoyStick!");

    beep_init_done();
    USBSerial.println("[info] init done.");
}

void loop()
{
    bool button = digitalRead(PIN_DISPLAY_BUTTON);

    USBSerial.println("[info] Hello JoyStick!");

    auto colors = {
        CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Purple,
        CRGB::Cyan, CRGB::White, CRGB::Orange, CRGB::Magenta, CRGB::Lime,
        CRGB::Pink, CRGB::Teal, CRGB::Gold, CRGB::Indigo, CRGB::Silver,
        CRGB::Black, CRGB::Gray, CRGB::Brown, CRGB::Maroon, CRGB::Navy,
        CRGB::Olive, CRGB::SkyBlue, CRGB::SlateGray, CRGB::DarkGreen,
        CRGB::DarkOrange, CRGB::DarkViolet, CRGB::DarkRed, CRGB::DarkBlue,
        CRGB::DarkCyan, CRGB::DarkMagenta};

    for (auto color : colors)
    {
        leds_joystick[0] = color;
        leds_joystick[1] = color;
        FastLED.show();

        if(button == HIGH) {
            gfx.fillScreen(TFT_BLACK);
        } else {
            // 画面ボタン押されてたら
            gfx.fillScreen(TFT_GOLD);
        }

        USBSerial.printf("[info] color_code : 0x%06x\n", color);
        delay(300);
    }
}
