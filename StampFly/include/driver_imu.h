#pragma once

#include <M5Unified.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <typedef.h>

#define IMU_PIN_SPI_CS      (GPIO_NUM_46)
#define IMU_PIN_SPI_MISO    (GPIO_NUM_43)
#define IMU_PIN_SPI_MOSI    (GPIO_NUM_14)
#define IMU_PIN_SPI_CLK     (GPIO_NUM_44)
// #define IMU_CONFIG_CLK_FREQ 100000

namespace IMU
{
    BMI270 imu;

    void init()
    {
        SPI.begin(IMU_PIN_SPI_CLK, IMU_PIN_SPI_MISO, IMU_PIN_SPI_MOSI);
        while(imu.beginSPI(IMU_PIN_SPI_CS) != BMI2_OK)
        {
            USBSerial.println("[error] IMU beginSPI failed");
            delay(1000);
        }

        USBSerial.println("[info] IMU init done");
    }

    void update()
    {
        imu.getSensorData();
    }

    void getAccelData(BMI270_SensorData& data)
    {
        data.accelX = imu.data.accelX;
        data.accelY = imu.data.accelY;
        data.accelZ = imu.data.accelZ;
    }

    void getGyroData(BMI270_SensorData& data)
    {
        data.gyroX = imu.data.gyroX;
        data.gyroY = imu.data.gyroY;
        data.gyroZ = imu.data.gyroZ;
    }

    void getData(BMI270_SensorData& data)
    {
        getAccelData(data);
        getGyroData(data);
    }
}
