#pragma once

#include <Arduino.h>

namespace UTIL {

    unsigned long time_tic = 0;
    unsigned long time_toc = 0;

    void tic() {
        time_tic = micros();
    }

    void toc() {
        time_toc = micros();
        USBSerial.printf("[info] toc: %lu us\n", (time_toc - time_tic));
    }

    void toc(const char* marker) {
        time_toc = micros();
        USBSerial.printf("[info][%s] toc: %lu us\n", marker, (time_toc - time_tic));
    }

    void toc(const char* funcname, int line) {
        time_toc = micros();
        USBSerial.printf("[info] toc: %lu us, %s(%d)\n", (time_toc - time_tic), funcname, line);
    }

}