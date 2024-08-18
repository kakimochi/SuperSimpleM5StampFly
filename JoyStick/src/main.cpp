#include <M5Unified.h>

#include <driver_led.h>
#include <driver_i2c.h>
#include <driver_buzzer.h>
#include <driver_joystick.h>
#include <driver_ble.h>

// Application timer
unsigned long current_ms = 0;
const unsigned long interval_300ms = 300;
unsigned long pre_ms = 0;
const unsigned long interval_3sec = 3000;
unsigned long pre_ms_3sec = 0;


void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    I2C::init();
    USBSerial.begin(115200);
    BUZZER::init();
    LED::init();
    JOY::init();
    BLE::init();

    // GUI
    M5.Display.begin();
    M5.Display.startWrite();
    M5.Display.fillScreen(BLACK);
    M5.Display.setFont(&fonts::efontCN_10);
    M5.Display.setTextColor(GOLD);
    M5.Display.setTextSize(2);
    M5.Display.drawString("JoyCon!", M5.Display.width()-100, 0);
    M5.Display.endWrite();

    // BLE
    BLE::connect();
    do {
        USBSerial.println("[info] BLE connecting...");
        delay(2000);
    } while(!BLE::isConnected());

    // Application timer
    pre_ms = millis();
    BUZZER::beep_init_done();
    USBSerial.println("[info] init done.");
}

void loop()
{
    M5.update();
    JOY::update();
    BLE::send(JOY::joy_data);

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
        pre_ms = current_ms;
    }
    if ((current_ms - pre_ms_3sec) >= interval_3sec)
    {
        USBSerial.printf("[info] stick_l_raw: %5d, %5d, stick_r_raw: %5d, %5d, button: %1x%1x%1x%1x\n", JOY::joy_data.stick_l_raw.x, JOY::joy_data.stick_l_raw.y, JOY::joy_data.stick_r_raw.x, JOY::joy_data.stick_r_raw.y, JOY::isPressed(JOY::BTN_TRG_L), JOY::isPressed(JOY::BTN_TRG_R), JOY::isPressed(JOY::BTN_STICK_L), JOY::isPressed(JOY::BTN_STICK_R));
        pre_ms_3sec = current_ms;
    }

    vTaskDelay(10);
}
