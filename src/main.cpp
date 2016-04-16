#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <util/eu_dst.h>

#include "uart.h"

#include "ds1307.h"
#include "ht16k33_display.h"

#define disableInterrupts() cli()
#define enableInterrupts() sei()

#define displayAddress 0x70
#define rtcAddress 0x68

static volatile uint32_t milliseconds = 0;
static volatile time_t timestamp = 0;

Uart* uart = new Uart;

time_t getNtpTime(time_t unixTimestamp) {
    return (unixTimestamp - UNIX_OFFSET);
}

void updateDisplay(struct tm* tm_rtc, HT16K33_Display* display) {
    display->updateDigit(digit0, tm_rtc->tm_hour / 10);
    display->updateDigit(digit1, tm_rtc->tm_hour % 10);
    display->updateDigit(digit2, tm_rtc->tm_min / 10);
    display->updateDigit(digit3, tm_rtc->tm_min % 10);
}

// Configure timer 0 as ms counter
void configureTimer0Interrupt() {
    // Set the Timer Mode to CTC
    TCCR0A |= (1 << WGM01);

    // set prescaler to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // Set the value to count to 249 (0-249 means 250 ticks)
    OCR0A = F_CPU / 64 / 1000 - 1;

    // Set the ISR COMPA vect
    TIMSK0 |= (1 << OCIE0A);
}

void enableUsartRxInterrupt() {
    UCSR0B |= (1 << RXCIE0);
}

ISR(TIMER0_COMPA_vect) {
    milliseconds++;
}

ISR(USART_RX_vect) {
    // disableInterrupts();
    union {
        uint32_t a;
        uint8_t b[4];
    } buffer;
    uart->receiveBytes(buffer.b, sizeof(buffer.b));
    timestamp = (time_t)buffer.a;
    // enableInterrupts();
}

int main() {
    HT16K33_Display display(displayAddress);
    DS1307 rtc(rtcAddress);

    configureTimer0Interrupt();
    enableUsartRxInterrupt();

    set_dst(eu_dst);
    set_zone(+1 * ONE_HOUR);

    display.setBrightness(0x8);

    enableInterrupts();

    while (1) {
        if (timestamp) {
            disableInterrupts();
            rtc.write(getNtpTime(timestamp));
            timestamp = 0;
            enableInterrupts();
        }

        if (!(milliseconds % 1000)) {
            disableInterrupts();

            time_t time_gm = rtc.read();
            struct tm* tm_rtc = localtime(&time_gm);
            updateDisplay(tm_rtc, &display);

            enableInterrupts();
        }
    }
}
