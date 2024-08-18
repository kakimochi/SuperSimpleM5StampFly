#pragma once

#include <typedef.h>
#include <driver_i2c.h>

#define I2C_ADDR_ATOM_JOYSTICK 0x59
#define I2C_BTN_REG 0x70
#define I2C_REG_JOY1_ADC_VALUE_12BITS 0x00
#define I2C_REG_JOY2_ADC_VALUE_12BITS 0x20

namespace JOY
{

    JoyData_t joy_data;

    void init()
    {
        memset(&joy_data, 0, sizeof(joy_data));
    }

    void update()
    {
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY1_ADC_VALUE_12BITS, (uint8_t *)&joy_data.stick_l_raw, sizeof(joy_data.stick_l_raw));
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY2_ADC_VALUE_12BITS, (uint8_t *)&joy_data.stick_r_raw, sizeof(joy_data.stick_r_raw));
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_BTN_REG, joy_data.btn, sizeof(joy_data.btn));

        // // normalize
        // joy_data.stick_l.x = (float)(joy_data.stick_l_raw.x - 2048) / 2048.0;
        // joy_data.stick_l.y = (float)(joy_data.stick_l_raw.y - 2048) / 2048.0;
        // joy_data.stick_r.x = (float)(joy_data.stick_r_raw.x - 2048) / 2048.0;
        // joy_data.stick_r.y = (float)(joy_data.stick_r_raw.y - 2048) / 2048.0;

        // // correct axis
        // joy_data.stick_l.y *= -1.0;
        // joy_data.stick_r.y *= -1.0;
    }

    void getAxis(uint8_t index, uint16_t *x, uint16_t *y)
    // void getAxis(uint8_t index, uint16_t *x, uint16_t *y, bool normalized = false)
    {
        if (index == STICK_L)
        {
            *x = joy_data.stick_l_raw.x;
            *y = joy_data.stick_l_raw.y;
            // if(normalized) {
            //     *x = joy_data.stick_l.x;
            //     *y = joy_data.stick_l.y;
            // } else {
            //     *x = joy_data.stick_l_raw.x;
            //     *y = joy_data.stick_l_raw.y;
            // }
        } else if (index == STICK_R) {
            *x = joy_data.stick_r_raw.x;
            *y = joy_data.stick_r_raw.y;
            // if(normalized) {
            //     *x = joy_data.stick_r.x;
            //     *y = joy_data.stick_r.y;
            // } else {
            //     *x = joy_data.stick_r_raw.x;
            //     *y = joy_data.stick_r_raw.y;
            // }
        }
    }

    void normalize(uint16_t x, uint16_t y, float *nx, float *ny)
    {
        const int BIT12_MAX = (1 << 12);
        *nx = (float)((x - BIT12_MAX / 2) / BIT12_MAX);
        *ny = (float)((y - BIT12_MAX / 2) / BIT12_MAX);
    }

    bool isPressed(uint8_t index)
    {
        if(index >= NUM_BTN_MAX) {
            return false;
        }
        return joy_data.btn[index] == LOW;
    }

    uint8_t isPressedAll()
    {
        uint8_t ret = 0;
        for (int i = 0; i < NUM_BTN_MAX; i++)
        {
            if (joy_data.btn[i] == LOW)
            {
                ret |= (1 << i);
            }
        }
        return ret;
    }
}