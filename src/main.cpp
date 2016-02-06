#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <util/eu_dst.h>

#include <i2cmaster.h>
#include <uart.h>

#include "ds1307.h"
#include "ht16k33_display.h"

#define disableInterrupts cli
#define enableInterrupts sei

const i2cAddress_t displayAddress = 0x70;
const i2cAddress_t rtdAddress = 0x68;

void* operator new(size_t objsize) {
    return malloc(objsize);
}

void operator delete(void* obj) {
    free(obj);
}

DS1307 *rtc;
HT16K33_Display *display;
I2cMaster *i2cMaster;
Uart *uart;

static int put(char byte, FILE *stream) {
    uart->sendByte(byte);
    return 0;
}

static int get(FILE *stream) {
    return uart->receiveByte();
}

static inline time_t getNtpTime(time_t unixTime) {
    return (unixTime - UNIX_OFFSET);
}

static uint8_t calculateBrightness(struct tm *tm_rtc) {
    uint16_t minutes = tm_rtc->tm_hour * 60;
    if (minutes < 360 || minutes >= 1320) {
        return 0x0;
    } else if (minutes >= 360 && minutes < 1320) {
        return 0xF;
    }
}

static void updateDisplay(time_t *rtcTime, HT16K33_Display *display) {
    struct tm *tm_rtc = localtime(rtcTime);
    display->setBrightness(calculateBrightness(tm_rtc));
    display->toggleColon();
    display->updateDigit(digit0, tm_rtc->tm_hour / 10);
    display->updateDigit(digit1, tm_rtc->tm_hour % 10);
    display->updateDigit(digit2, tm_rtc->tm_min / 10);
    display->updateDigit(digit3, tm_rtc->tm_min % 10);
}

static void setIOStream() {
    FILE* stream = fdevopen(put, get);
    stdin = stream;
    stdout = stream;
    stderr = stream;
}

ISR(TIMER1_OVF_vect) {
    time_t time_gm = rtc->read();
    updateDisplay(&time_gm, display);
}

int main() {
    setIOStream();

    set_dst(eu_dst);
    set_zone(+1 * ONE_HOUR);

    i2cMaster = new I2cMaster;
    uart = new Uart;

    display = new HT16K33_Display(i2cMaster, displayAddress);
    rtc = new DS1307(i2cMaster, rtdAddress);

    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0x0BDC;
    TCCR1B |= (1 << CS12);

    enableInterrupts();

    while (1) {
    }
}
