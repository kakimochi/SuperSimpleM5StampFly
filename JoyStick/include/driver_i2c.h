#pragma once

#include <Wire.h>

#define I2C_SDA_PIN 38  // G38 on the AtomS3
#define I2C_SCL_PIN 39  // G39 on the AtomS3

namespace I2C
{
    void init()
    {
        Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    }

    void write(uint8_t addr, uint8_t reg, uint8_t data)
    {
        Wire.beginTransmission(addr);
        Wire.write(reg);
        Wire.write(data);
        Wire.endTransmission();
    }

    void read(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t size)
    {
        Wire.beginTransmission(addr);
        Wire.write(reg);
        Wire.endTransmission();

        Wire.requestFrom(addr, size);
        for (int i = 0; i < size; i++)
        {
            data[i] = Wire.read();
        }
    }
} // namespace I2C