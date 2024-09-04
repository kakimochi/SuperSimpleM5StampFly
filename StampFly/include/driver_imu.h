#pragma once

#include <M5Unified.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <MadgwickAHRS.h>
#include <typedef.h>

// SPI
#define IMU_PIN_SPI_CS      (GPIO_NUM_46)
#define IMU_PIN_SPI_MISO    (GPIO_NUM_43)
#define IMU_PIN_SPI_MOSI    (GPIO_NUM_14)
#define IMU_PIN_SPI_CLK     (GPIO_NUM_44)
// #define IMU_CONFIG_CLK_FREQ 100000

// madgwick filter
#define IMU_MADGWICK_SAMPLE_FREQ_HZ 100

namespace IMU
{
    BMI270 imu;
    Madgwick filter;

    typedef struct
    {
        float roll;
        float pitch;
        float yaw;
    } Posture;

    Posture posture;

    void init()
    {
        SPI.begin(IMU_PIN_SPI_CLK, IMU_PIN_SPI_MISO, IMU_PIN_SPI_MOSI);
        while(imu.beginSPI(IMU_PIN_SPI_CS) != BMI2_OK)
        {
            USBSerial.println("[error] IMU beginSPI failed");
            delay(1000);
        }

        posture = {0};
        filter.begin(IMU_MADGWICK_SAMPLE_FREQ_HZ);

        USBSerial.println("[info] IMU init done");
    }

    void update()
    {
        imu.getSensorData();

        // calculate posture
        filter.updateIMU(imu.data.gyroX, imu.data.gyroY, imu.data.gyroZ, imu.data.accelX, imu.data.accelY, imu.data.accelZ);
        posture.roll  = filter.getRoll();
        posture.pitch = filter.getPitch();
        posture.yaw   = filter.getYaw();
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

    void getPosture(Posture& data)
    {
        data.roll  = posture.roll;
        data.pitch = posture.pitch;
        data.yaw   = posture.yaw;
    }
}
