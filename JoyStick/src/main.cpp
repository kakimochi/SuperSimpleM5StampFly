#include <M5Unified.h>
#include <FastLED.h>

// LED
std::vector<CRGB> colors = {
    CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Purple,
    CRGB::Cyan, CRGB::White, CRGB::Orange, CRGB::Magenta, CRGB::Lime,
    CRGB::Pink, CRGB::Teal, CRGB::Gold, CRGB::Indigo, CRGB::Silver,
    CRGB::Black, CRGB::Gray, CRGB::Brown, CRGB::Maroon, CRGB::Navy,
    CRGB::Olive, CRGB::SkyBlue, CRGB::SlateGray, CRGB::DarkGreen,
    CRGB::DarkOrange, CRGB::DarkViolet, CRGB::DarkRed, CRGB::DarkBlue,
    CRGB::DarkCyan, CRGB::DarkMagenta
};

#define NUM_LEDS_JOYSTICK 2
#define PIN_LED_JOYSTICK 6 // G6 on the M5AtomS3 Joystick
CRGB leds_joystick[NUM_LEDS_JOYSTICK];

// Beep Sound
#define PIN_BUZZER 5       // G5 on the M5AtomS3 Joystick
#define TONE_C5 523.251
#define TONE_E5 659.255
#define TONE_G5 783.991
#define TONE_C6 (TONE_C5 * 2)
#define TONE_E6 (TONE_E5 * 2)
#define TONE_G6 (TONE_G5 * 2)

// Application timer
const unsigned long interval_300ms = 300;
unsigned long pre_ms = 0;
unsigned long current_ms = 0;

void beep()
{
    M5.Speaker.tone(TONE_E5, 200);
}

void beep_init_done()
{
    // tone(PIN_BUZZER, TONE_C5, 500);     // 音きれい
    M5.Speaker.tone(TONE_C5, 500);   // 音スカる、なんでだろうね
    // M5.Speaker.tone(TONE_E5, 500);
    // M5.Speaker.tone(TONE_G5, 500);
}

void led_color_update()
{
    static int color_index = 0;
    auto color = colors[color_index];
    leds_joystick[0] = color;
    leds_joystick[1] = color;
    FastLED.show();
    USBSerial.printf("[info] color_code : 0x%06x\n", color);

    if (color_index >= colors.size())
    {
        color_index = 0;
    } else {
        color_index++;
    }
}

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    USBSerial.begin(115200);

    // buzzer
    auto spk_cfg = M5.Speaker.config();
    spk_cfg.pin_data_out = PIN_BUZZER;
    M5.Speaker.config(spk_cfg);

    // LED
    FastLED.addLeds<WS2812, PIN_LED_JOYSTICK, GRB>(leds_joystick, NUM_LEDS_JOYSTICK);
    FastLED.setBrightness(50);

    // GUI
    M5.Display.begin();
    M5.Display.startWrite();
        M5.Display.fillScreen(BLACK);
        M5.Display.setFont(&fonts::efontCN_10);
        M5.Display.setTextColor(GOLD);
        M5.Display.setTextSize(2);
        M5.Display.drawString("JoyStick!", 0, M5.Display.height() / 2);
    M5.Display.endWrite();

    // Application timer
    pre_ms = millis();

    beep_init_done();
    USBSerial.println("[info] init done.");
}

void loop()
{
    M5.update();

    if(M5.BtnA.wasReleased()) {
        static bool toggle_switch;
        if(toggle_switch) {
            M5.Display.fillScreen(BLACK);
        } else {
            M5.Display.fillScreen(GOLD);
        }
        toggle_switch = !toggle_switch;
    }

    // Application timer
    current_ms = millis();
    if((current_ms - pre_ms) >= interval_300ms) {
        led_color_update();
        pre_ms = current_ms;
    }

    vTaskDelay(10);
}
