#include <M5Unified.h>

#include <driver_led.h>
#include <driver_i2c.h>
#include <driver_buzzer.h>

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
    BUZZER::beep_init_done();
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

    // I2CでJoyStickのボタン状態を取得する
    const int num_btns = 4;
    uint8_t btn[num_btns] = {0};
    I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_BUTTON_REG, btn, num_btns);

    // Application timer
    current_ms = millis();
    if((current_ms - pre_ms) >= interval_300ms) {
        LED::color_rotation();
        LED::update();
        USBSerial.printf("[info] button_state : %1x%1x%1x%1x\n", btn[0],btn[1],btn[2],btn[3]);

        pre_ms = current_ms;
    }

    vTaskDelay(10);
}
