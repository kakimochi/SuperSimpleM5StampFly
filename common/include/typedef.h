#pragma once

#include <stdint.h>

namespace JOY {

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

    // size : (2+2) * 2 + 4 = 12byte ,float:4byte
    // size : (2+2) * 2 + (4+4)*2 + 4 = 28byte ,float:4byte
    typedef struct
    {
        struct
        {
            uint16_t x; // for 12bit resolution
            uint16_t y;
        } stick_l_raw;
        struct
        {
            uint16_t x;
            uint16_t y;
        } stick_r_raw;
        // struct
        // {
        //     float_t x;
        //     float_t y;
        // } stick_l;
        // struct
        // {
        //     float_t x;
        //     float_t y;
        // } stick_r;

        uint8_t btn[NUM_BTN_MAX];
    } JoyData_t;
}
