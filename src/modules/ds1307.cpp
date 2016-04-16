#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "i2cmaster.h"

#include "ds1307.h"

static inline uint8_t decode(uint8_t num) {
    return (num - 6 * (num >> 4));
}

static inline uint8_t encode(uint8_t num) {
    return (num + 6 * (num / 10));
}

DS1307::DS1307(I2cMaster* i2cMaster, uint8_t address) {
    this->address = address;
    this->i2cMaster = i2cMaster;
}

DS1307::~DS1307() {
}

time_t DS1307::read() {
    uint8_t timeRegistersStart = 0x00;
    uint8_t timeRegisters[7];

    i2cMaster->startTransmission();
    i2cMaster->writeBytes(address, &timeRegistersStart, 1);
    i2cMaster->startTransmission();
    i2cMaster->readBytes(address, timeRegisters, sizeof(timeRegisters));
    i2cMaster->stopTransmission();

    struct tm tm_rtc;
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

    uint8_t timeRegisters[] = {timeRegistersStart, encode(tm_ntp->tm_sec),
            encode(tm_ntp->tm_min), encode(tm_ntp->tm_hour),
            encode(tm_ntp->tm_wday + 1),  // rtc_wday is 1-7
            encode(tm_ntp->tm_mday),
            encode(tm_ntp->tm_mon + 1),  // rtc_mon is 1-12
            encode(tm_ntp->tm_year)};

    i2cMaster->startTransmission();
    i2cMaster->writeBytes(address, timeRegisters, sizeof(timeRegisters));
    i2cMaster->stopTransmission();
}
