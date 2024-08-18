#pragma once

#include <M5Unified.h>

#define PIN_MTR_FL 5    // G5 on the M5StampS3
#define PIN_MTR_FR 42   // G42
#define PIN_MTR_BL 10   // G10
#define PIN_MTR_BR 41   // G41

namespace MOTOR {
    const int MTR_PWM_FREQ = 150000;
    const int MTR_PWM_RESOLUTION_BITS = 8;
    const uint32_t MTR_PWM_MAX = (1 << MTR_PWM_RESOLUTION_BITS) - 1;

    enum {
        MTR_FL = 0, // Front Left
        MTR_FR, // Front Right
        MTR_BL, // Back Left
        MTR_BR, // Back Right
        NUM_MOTORS
    };

    static uint32_t pwm_duty[NUM_MOTORS] = {0};

    void init()
    {
        memset(pwm_duty, 0, sizeof(pwm_duty));

        ledcSetup(MTR_FL, MTR_PWM_FREQ, MTR_PWM_RESOLUTION_BITS);
        ledcSetup(MTR_FR, MTR_PWM_FREQ, MTR_PWM_RESOLUTION_BITS);
        ledcSetup(MTR_BL, MTR_PWM_FREQ, MTR_PWM_RESOLUTION_BITS);
        ledcSetup(MTR_BR, MTR_PWM_FREQ, MTR_PWM_RESOLUTION_BITS);
        ledcAttachPin(PIN_MTR_FL, MTR_FL);
        ledcAttachPin(PIN_MTR_FR, MTR_FR);
        ledcAttachPin(PIN_MTR_BL, MTR_BL);
        ledcAttachPin(PIN_MTR_BR, MTR_BR);

        // stop motors
        ledcWrite(MTR_FL, 0);
        ledcWrite(MTR_FR, 0);
        ledcWrite(MTR_BL, 0);
        ledcWrite(MTR_BR, 0);
 
        USBSerial.println("[info] motor init done.");
    }

    void setSpeed(int index, float_t pwm_duty_ratio)
    {
        if(index >= NUM_MOTORS) {
            return;
        }
        pwm_duty[index] = uint32_t(pwm_duty_ratio * MTR_PWM_MAX);
        // USBSerial.printf("[info] setSpeed: motor[%d] pwm_duty : %d\n", index, pwm_duty[index]);
    }

    void update()
    {
        ledcWrite(MTR_FL, pwm_duty[MTR_FL]);
        ledcWrite(MTR_FR, pwm_duty[MTR_FR]);
        ledcWrite(MTR_BL, pwm_duty[MTR_BL]);
        ledcWrite(MTR_BR, pwm_duty[MTR_BR]);
        // USBSerial.printf("[info] motor pwm updated. (%d,%d,%d,%d)\n",pwm_duty[MTR_FL], pwm_duty[MTR_FR], pwm_duty[MTR_BL], pwm_duty[MTR_BR]);
    }

    void stop()
    {
        setSpeed(MTR_FL, 0.0f);
        setSpeed(MTR_FR, 0.0f);
        setSpeed(MTR_BL, 0.0f);
        setSpeed(MTR_BR, 0.0f);
        update();
    }
}