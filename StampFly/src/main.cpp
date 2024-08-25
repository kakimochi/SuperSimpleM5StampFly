#include <M5Unified.h>

#include <typedef.h>
#include <driver_led.h>
#include <driver_motor.h>
#include <driver_ble.h>
#include <driver_imu.h>

// Application timer
unsigned long current_ms = 0;
const unsigned long interval_300ms = 300;
unsigned long pre_ms_300ms = 0;
const unsigned long interval_3sec = 3000;
unsigned long pre_ms_3sec = 0;

// BLE Peripheral
static bool ble_connected = false;
static JOY::JoyData_t joydata = {0};

static void ble_event_callback(BLE::BLEEventParam_t *param)
{
    switch (param->event)
    {
    case BLE::BLE_EVENT_CONNECTED:
        ble_connected = true;
        USBSerial.println("[info] BLE_EVENT_CONNECTED");
        break;
    case BLE::BLE_EVENT_DISCONNECTED:
        ble_connected = false;
        USBSerial.println("[info] BLE_EVENT_DISCONNECTED");
        break;
    case BLE::BLE_EVENT_RECEIVED:
        joydata = param->joydata;
        break;
    default:
        break;
    }
}

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);
    USBSerial.begin(115200);
    delay(100);
    LED::init();
    MOTOR::init();
    BLE::init(ble_event_callback);
    IMU::init();

    // Application timer
    pre_ms_300ms = millis();
    USBSerial.println("[info] init done.");
}

void loop()
{
    M5.update();
    MOTOR::update();
    BLE::update();
    IMU::update();

    float_t pow = ((int16_t)joydata.stick_l_raw.y - 4096/2) / 4096.0f * 2.0f;   // -1.0 - 1.0
    pow *= -1.0f;   // レバー前に倒すと回転数アップ
    if(ble_connected) {
        if((pow > 0.1f) && (pow < 1.0f)) {
            MOTOR::setSpeed(MOTOR::MTR_FL, pow * 0.1f);
            MOTOR::setSpeed(MOTOR::MTR_FR, pow * 0.1f);
            MOTOR::setSpeed(MOTOR::MTR_BL, pow * 0.1f);
            MOTOR::setSpeed(MOTOR::MTR_BR, pow * 0.1f);
        } else {
            MOTOR::stop();
        }
    } else {
            MOTOR::stop();
    }

    // Application timer
    current_ms = millis();
    if ((current_ms - pre_ms_300ms) >= interval_300ms)
    {
        LED::color_rotation();
        LED::update();
        pre_ms_300ms = current_ms;
    }
    if ((current_ms - pre_ms_3sec) >= interval_3sec)
    {
        if(ble_connected) {
            USBSerial.printf("pow: %f\n", pow);
            USBSerial.printf("[info] stick_l_raw: %5d, %5d, stick_r_raw: %5d, %5d\n", joydata.stick_l_raw.x, joydata.stick_l_raw.y, joydata.stick_r_raw.x, joydata.stick_r_raw.y);
        }
        USBSerial.printf("[info] acc: %6.3f, %6.3f, %6.3f, gyr: %6.3f, %6.3f, %6.3f\n", IMU::imu.data.accelX, IMU::imu.data.accelY, IMU::imu.data.accelZ, IMU::imu.data.gyroX, IMU::imu.data.gyroY, IMU::imu.data.gyroZ);
        pre_ms_3sec = current_ms;
    }

    vTaskDelay(10);
}
