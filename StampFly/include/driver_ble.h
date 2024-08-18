#pragma once

#include <M5Unified.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <typedef.h>

#define BLE_DEVICE_NAME         "STAMPFLY"
#define BLE_UUID_SERVICE        "0000"
#define BLE_UUID_CHARACTERISTIC "0001"

namespace BLE
{
    enum BLE_EVENT
    {
        BLE_EVENT_CONNECTED,
        BLE_EVENT_DISCONNECTED,
        BLE_EVENT_RECEIVED,
    };

    typedef struct
    {
        BLE_EVENT event;
        JOY::JoyData_t joydata;
    } BLEEventParam_t;

    // イベントコールバック関数
    typedef void (*BLECallback_t)(BLEEventParam_t *param);
    static BLECallback_t _ble_callback;

    // BLE Peripheral
    BLEServer *pServer = NULL;
    BLECharacteristic *pCharacteristic = NULL;
    bool deviceConnected = false;
    bool advertising = false;

    class BLECallbacks : public BLEServerCallbacks, public BLECharacteristicCallbacks
    {
        void onConnect(BLEServer *pServer) override
        {
            deviceConnected = true;
            {
                BLEEventParam_t param;
                param.event = BLE_EVENT_CONNECTED;
                _ble_callback(&param);
            }
        }

        void onDisconnect(BLEServer *pServer) override
        {
            deviceConnected = false;
            advertising = false;
            {
                BLEEventParam_t param;
                param.event = BLE_EVENT_DISCONNECTED;
                _ble_callback(&param);
            }
        }

        void onRead(BLECharacteristic *pCharacteristic) override
        {
        }

        void onWrite(BLECharacteristic *pCharacteristic) override
        {
            std::string value = pCharacteristic->getValue();
            // USBSerial.printf("value size: %d\n", value.length());
            // for(size_t i = 0; i < value.length(); i++) {
            //     USBSerial.printf("%02x ", value[i]);
            // }
            // USBSerial.printf("\n");
            if (value.length() == sizeof(JOY::JoyData_t))
            {
                JOY::JoyData_t joydata;
                memcpy(&joydata, value.data(), sizeof(JOY::JoyData_t));

                {
                    BLEEventParam_t param;
                    param.event = BLE_EVENT_RECEIVED;
                    param.joydata = joydata;
                    _ble_callback(&param);
                }
                // USBSerial.printf("[info] stick_l_raw: %5d, %5d, stick_r_raw: %5d, %5d\n", joydata.stick_l_raw.x, joydata.stick_l_raw.y, joydata.stick_r_raw.x, joydata.stick_r_raw.y);
            } else {
                USBSerial.printf("[error] BLE packet size unmatched, value:length()=%d, sizeof(JOY::JoyData_t)=%d\n",value.length(), sizeof(JOY::JoyData_t));
            }
        }
    };


    BLECallbacks bleCallbacks;



    void init(BLECallback_t callback)
    {
        _ble_callback = callback;
        USBSerial.println("[info] BLE_PERIPHERAL");
        BLEDevice::init(BLE_DEVICE_NAME);

        // Create BLE Server
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(&bleCallbacks);

        // Create BLE Service
        BLEService *pService = pServer->createService(BLE_UUID_SERVICE);

        // Create BLE Characteristic
        pCharacteristic = pService->createCharacteristic(
            BLE_UUID_CHARACTERISTIC,
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE |
                BLECharacteristic::PROPERTY_NOTIFY |
                BLECharacteristic::PROPERTY_INDICATE);

        pCharacteristic->setCallbacks(&bleCallbacks);

        // Start the service
        pService->start();
        USBSerial.println("[info] BLE pService->start()");
    }

    void connect()
    {
        //
    }

    void send(uint8_t *data, uint8_t len)
    {
        if (pCharacteristic)
        {
            pCharacteristic->setValue(data, len);
            pCharacteristic->notify();
        }
    }

    void update()
    {
        if (deviceConnected == false)
        {
            if (advertising == false)
            {
                // Start advertising
                advertising = true;

                BLEAdvertising *pAdvertising = pServer->getAdvertising();
                pAdvertising->start();
                USBSerial.println("[info] BLE pAdvertising->start()");
            }
        }
    }

} // namespace BLE