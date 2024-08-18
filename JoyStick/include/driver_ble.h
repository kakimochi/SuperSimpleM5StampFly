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
    static TaskHandle_t _bleTaskHandle = NULL;

    // BLE Central
    static BLEClient *pClient = NULL;
    static BLEAdvertisedDevice *targetDevice = NULL;
    static BLERemoteCharacteristic *pRemoteCharacteristic = NULL;

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
        void onResult(BLEAdvertisedDevice advertisedDevice) override
        {
            // USBSerial.println(advertisedDevice.toString().c_str());
            if (advertisedDevice.getName() == BLE_DEVICE_NAME)
            {
                USBSerial.println(advertisedDevice.getAddress().toString().c_str());
                advertisedDevice.getScan()->stop();
                targetDevice = new BLEAdvertisedDevice(advertisedDevice);
            }
        }
    };

    void ScanTask(void *params)
    {
        USBSerial.println("[info] BLE: Scan...");

        BLEScan *pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(449);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(30, false);
 
        _bleTaskHandle = NULL;
        vTaskDelete(NULL);
    }

    void init()
    {
        // USBSerial.println("BLE_CENTRAL");
        BLEDevice::init(BLE_DEVICE_NAME);
    }

    void connect()
    {
        if (_bleTaskHandle == NULL)
        {
            xTaskCreatePinnedToCore(ScanTask, "BLE_ScanTask", 2048, NULL, 1, &_bleTaskHandle, 1);
        }
    }

    static void _check()
    {
        if (targetDevice)
        {
            if (pClient == NULL)
            {
                pClient = BLEDevice::createClient();
                pClient->connect(targetDevice);

                BLERemoteService *pRemoteService = pClient->getService(BLE_UUID_SERVICE);
                pRemoteCharacteristic = pRemoteService->getCharacteristic(BLE_UUID_CHARACTERISTIC);
            }
        }
    }

    void send(const JOY::JoyData_t &data)
    {
        _check();
        if (pRemoteCharacteristic)
        {
            pRemoteCharacteristic->writeValue((uint8_t *)&data, sizeof(JOY::JoyData_t));
            // USBSerial.printf("[info] BLE: send %d bytes\n", sizeof(JOY::JoyData_t));
        }
    }

    bool isConnected()
    {
        _check();
        return pClient != NULL;
    }
} // namespace BLE
