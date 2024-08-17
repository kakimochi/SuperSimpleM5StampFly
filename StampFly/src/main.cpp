#include <M5Unified.h>

#include <driver_led.h>
#include <driver_motor.h>

// Application timer
const unsigned long interval_300ms = 300;
const unsigned long interval_1s = 3000;
const unsigned long interval_3s = 3000;
const unsigned long interval_5s = 5000;
unsigned long pre_ms_300ms = 0;
unsigned long pre_ms_1s = 0;
unsigned long pre_ms_3s = 0;
unsigned long pre_ms_5s = 0;
unsigned long current_ms = 0;

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    USBSerial.begin(115200);
    delay(100);
    LED::init();
    MOTOR::init();

    // Application timer
    pre_ms_300ms = millis();
    pre_ms_1s = millis();
    pre_ms_3s = millis();
    pre_ms_5s = millis();
    USBSerial.println("[info] init done.");
}

void loop()
{
    M5.update();
    MOTOR::update();

    // Application timer
    current_ms = millis();
    if ((current_ms - pre_ms_300ms) >= interval_300ms)
    {
        LED::color_rotation();
        LED::update();
        pre_ms_300ms = current_ms;
    }
    if ((current_ms - pre_ms_1s) >= interval_1s)
    {
        static int run_once_1s = 0;
        if(run_once_1s == 0) {
            MOTOR::setSpeed(MOTOR::MTR_FL, 0.1);
            MOTOR::setSpeed(MOTOR::MTR_FR, 0.1);
            MOTOR::setSpeed(MOTOR::MTR_BL, 0.1);
            MOTOR::setSpeed(MOTOR::MTR_BR, 0.1);
            run_once_1s = 1;
        }
        pre_ms_1s = current_ms;
    }
    if ((current_ms - pre_ms_3s) >= interval_3s)
    {
        pre_ms_3s = current_ms;
    }
    if ((current_ms - pre_ms_5s) >= interval_5s)
    {
        static int run_once_5s = 0;
        if(run_once_5s == 0) {
            MOTOR::stop();
            run_once_5s = 1;
        }
        pre_ms_5s = current_ms;
    }

    vTaskDelay(10);
}
