#pragma once

#include <driver_i2c.h>

#define I2C_ADDR_ATOM_JOYSTICK 0x59
#define I2C_BTN_REG 0x70
#define I2C_REG_JOY1_ADC_VALUE_12BITS 0x00
#define I2C_REG_JOY2_ADC_VALUE_12BITS 0x20

namespace JOY
{

    enum
    {
        STICK_L,
        STICK_R,
        NUM_STICK_MAX
    };

    enum
    {
        BTN_TRG_L,
        BTN_TRG_R,
        BTN_STICK_L,
        BTN_STICK_R,
        NUM_BTN_MAX
    };

    typedef struct
    {
        struct
        {
            uint16_t x; // 12bit
            uint16_t y;
        } joy1;
        struct
        {
            uint16_t x; // 12bit
            uint16_t y;
        } joy2;
        uint8_t btn[NUM_BTN_MAX];
    } JoyData_t;

    JoyData_t joy_data;

    void init()
    {
        memset(&joy_data, 0, sizeof(joy_data));
    }

    void update()
    {
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY1_ADC_VALUE_12BITS, (uint8_t *)&joy_data.joy1, sizeof(joy_data.joy1));
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY2_ADC_VALUE_12BITS, (uint8_t *)&joy_data.joy2, sizeof(joy_data.joy2));
        I2C::read(I2C_ADDR_ATOM_JOYSTICK, I2C_BTN_REG, joy_data.btn, sizeof(joy_data.btn));
    }

    void getAxis(uint8_t index, uint16_t *x, uint16_t *y)
    {
        if (index == STICK_L)
        {
            *x = joy_data.joy1.x;
            *y = joy_data.joy1.y;
        }
        else if (index == STICK_R)
        {
            *x = joy_data.joy2.x;
            *y = joy_data.joy2.y;
        }
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