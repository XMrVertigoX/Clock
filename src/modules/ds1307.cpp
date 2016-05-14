#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "twi.hpp"

#include "ds1307.h"

#define TWI() Twi::getInstance()

static inline uint8_t decode(uint8_t num) {
    return (num - 6 * (num >> 4));
}

static inline uint8_t encode(uint8_t num) {
    return (num + 6 * (num / 10));
}

DS1307::DS1307(uint8_t address) {
    _address = address;
}

DS1307::~DS1307() {}

time_t DS1307::read() {
    uint8_t timeRegistersStart = 0x00;
    uint8_t timeRegisters[7];
    struct tm tm_rtc;

    TWI()->startTransmission();
    TWI()->writeBytes(_address, &timeRegistersStart, 1);
    TWI()->startTransmission();
    TWI()->readBytes(_address, timeRegisters, sizeof(timeRegisters));
    TWI()->stopTransmission();

    tm_rtc.tm_sec = decode(timeRegisters[0]);
    tm_rtc.tm_min = decode(timeRegisters[1]);
    tm_rtc.tm_hour = decode(timeRegisters[2]);
    tm_rtc.tm_wday = decode(timeRegisters[3]) - 1;  // tm_wday is 0-6
    tm_rtc.tm_mday = decode(timeRegisters[4]);
    tm_rtc.tm_mon = decode(timeRegisters[5]) - 1;  // tm_mon is 0-11
    tm_rtc.tm_year = decode(timeRegisters[6]);

    return mk_gmtime(&tm_rtc);
}

void DS1307::write(time_t ntpTime) {
    struct tm* tm_ntp = gmtime(&ntpTime);
    uint8_t timeRegistersStart = 0x00;

    // rtc_wday is 1-7, rtc_mon is 1-12
    uint8_t timeRegisters[] = {
        timeRegistersStart,          encode(tm_ntp->tm_sec),
        encode(tm_ntp->tm_min),      encode(tm_ntp->tm_hour),
        encode(tm_ntp->tm_wday + 1), encode(tm_ntp->tm_mday),
        encode(tm_ntp->tm_mon + 1),  encode(tm_ntp->tm_year)};

    TWI()->startTransmission();
    TWI()->writeBytes(_address, timeRegisters, sizeof(timeRegisters));
    TWI()->stopTransmission();
}
