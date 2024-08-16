#pragma once

#include <M5Unified.h>

// Beep Sound
#define PIN_BUZZER 5 // G5 on the M5AtomS3 Joystick
#define TONE_C5 523.251
#define TONE_E5 659.255
#define TONE_G5 783.991
#define TONE_C6 (TONE_C5 * 2)
#define TONE_E6 (TONE_E5 * 2)
#define TONE_G6 (TONE_G5 * 2)

namespace BUZZER
{
    void init()
    {
        auto spk_cfg = M5.Speaker.config();
        spk_cfg.pin_data_out = PIN_BUZZER;
        spk_cfg.magnification = UINT8_MAX;
        M5.Speaker.config(spk_cfg);
    }
    
    void beep()
    {
        M5.Speaker.tone(TONE_E5, 200);
    }

    void beep_init_done()
    {
        // tone(PIN_BUZZER, TONE_C5, 500);     // 音きれい
        M5.Speaker.tone(TONE_C5, 500); // 音スカる、なんでだろうね
        // M5.Speaker.tone(TONE_E5, 500);
        // M5.Speaker.tone(TONE_G5, 500);
    }

}