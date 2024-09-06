#pragma once

#include <M5Unified.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <DFRobot_BMM150.h>
#include <MadgwickAHRS.h>
#include <typedef.h>

// SPI
#define IMU_PIN_SPI_CS      (GPIO_NUM_46)
#define IMU_PIN_SPI_MISO    (GPIO_NUM_43)
#define IMU_PIN_SPI_MOSI    (GPIO_NUM_14)
#define IMU_PIN_SPI_CLK     (GPIO_NUM_44)
// #define IMU_CONFIG_CLK_FREQ 100000

// I2C
#define IMU_COMPASS_I2C_ADDR (I2C_ADDRESS_1)    // 0x10
#define IMU_PIN_I2C_SDA (GPIO_NUM_3)
#define IMU_PIN_I2C_SCL (GPIO_NUM_4)

// madgwick filter
#define IMU_MADGWICK_SAMPLE_FREQ_HZ 100

// type wrapper to simplify usage
typedef DFRobot_BMM150_I2C BMM150;
typedef sBmm150MagData_t BMM150_SensorData;

namespace IMU
{
    BMI270 imu;

    BMM150 compass(&Wire, IMU_COMPASS_I2C_ADDR);
    BMM150_SensorData compass_data;
    float compass_degree;

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
        // BMI270 IMU via SPI
        SPI.begin(IMU_PIN_SPI_CLK, IMU_PIN_SPI_MISO, IMU_PIN_SPI_MOSI);
        while(imu.beginSPI(IMU_PIN_SPI_CS) != BMI2_OK)
        {
            USBSerial.println("[error] IMU beginSPI failed");
            delay(1000);
        }

        // BMM150 Compass via I2C
        Wire.begin(IMU_PIN_I2C_SDA, IMU_PIN_I2C_SCL);
        while(compass.begin() != 0)
        {
            USBSerial.println("[error] IMU compass begin failed");
            delay(1000);
        }
        compass.setOperationMode(BMM150_POWERMODE_NORMAL);
        compass.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
        compass.setRate(BMM150_DATA_RATE_10HZ);
        compass.setMeasurementXYZ();
        compass_data = {0};
        compass_degree = 0.0f;

        // Madgwick Filter
        posture = {0};
        filter.begin(IMU_MADGWICK_SAMPLE_FREQ_HZ);

        USBSerial.println("[info] IMU init done");
    }

    void update()
    {
        imu.getSensorData();

        compass_data = compass.getGeomagneticData();
        compass_degree = compass.getCompassDegree();

        // calculate posture
        filter.update(imu.data.gyroX, imu.data.gyroY, imu.data.gyroZ, imu.data.accelX, imu.data.accelY, imu.data.accelZ, (float_t)compass_data.x, (float_t)compass_data.y, (float_t)compass_data.z);
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

    void getCompassData(BMM150_SensorData& data)
    {
        data.x = compass_data.x;
        data.y = compass_data.y;
        data.z = compass_data.z;
    }

    void getCompassDegree(float& data)
    {
        data = compass_degree;
    }

    void getData(BMI270_SensorData& data, BMM150_SensorData& compass_data, float_t& compass_degree)
    {
        getAccelData(data);
        getGyroData(data);
        getCompassData(compass_data);
        getCompassDegree(compass_degree);
    }

    void getPosture(Posture& data)
    {
        data.roll  = posture.roll;
        data.pitch = posture.pitch;
        data.yaw   = posture.yaw;
    }
}
