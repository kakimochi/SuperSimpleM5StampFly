#include <M5Unified.h>

#include <driver_led.h>
#include <driver_i2c.h>
#include <driver_buzzer.h>
#include <driver_joystick.h>

// Application timer
const unsigned long interval_300ms = 300;
unsigned long pre_ms = 0;
unsigned long current_ms = 0;

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    I2C::init();
    USBSerial.begin(115200);
    BUZZER::init();
    LED::init();
    JOY::init();

    // GUI
    M5.Display.begin();
    M5.Display.startWrite();
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::efontCN_10);
    M5.Display.setTextColor(GOLD);
    M5.Display.setTextSize(2);
    M5.Display.drawString("JoyCon!", M5.Display.width()-100, 0);
    M5.Display.endWrite();

    // Application timer
    pre_ms = millis();
    BUZZER::beep_init_done();
    USBSerial.println("[info] init done.");
}

void loop()
{
    M5.update();
    JOY::update();

    if (M5.BtnA.wasReleased())
    {
        static bool toggle_switch;
        if (toggle_switch)
        {
            M5.Display.fillScreen(WHITE);
        }
        else
        {
            M5.Display.fillScreen(BLACK);
        }
        toggle_switch = !toggle_switch;
    }

    M5.Display.fillCircle(JOY::joy_data.stick_l_raw.x / 32, JOY::joy_data.stick_l_raw.y / 32, 5, GOLD);
    M5.Display.fillCircle(JOY::joy_data.stick_r_raw.x / 32, JOY::joy_data.stick_r_raw.y / 32, 5, CYAN);

    // Application timer
    current_ms = millis();
    if ((current_ms - pre_ms) >= interval_300ms)
    {
        LED::color_rotation();
        LED::update();
        // USBSerial.printf("[info] button_state : %1x%1x%1x%1x\n", JOY::isPressed(JOY::BTN_TRG_L), JOY::isPressed(JOY::BTN_TRG_R), JOY::isPressed(JOY::BTN_STICK_L), JOY::isPressed(JOY::BTN_STICK_R));
        USBSerial.printf("[info] stick_l: %.3f, %.3f, stick_r: %.3f, %.3f\n", JOY::joy_data.stick_l.x, JOY::joy_data.stick_l.y, JOY::joy_data.stick_r.x, JOY::joy_data.stick_r.y);
        pre_ms = current_ms;
    }

    vTaskDelay(10);
}
