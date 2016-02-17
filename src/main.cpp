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

#include <modules/ds1307/ds1307.h>
#include <modules/ht16k33/ht16k33_display.h>

#define disableInterrupts cli
#define enableInterrupts sei

#define displayAddress 0x70
#define rtcAddress 0x68

static volatile uint32_t milliseconds = 0;
static volatile time_t timestamp = 0;

I2cMaster *i2cMaster = new I2cMaster;
Uart *uart = new Uart;

static inline time_t getNtpTime(time_t unixTimestamp) {
	return (unixTimestamp - UNIX_OFFSET);
}

static void updateDisplay(struct tm *tm_rtc, HT16K33_Display *display) {
	display->updateDigit(digit0, tm_rtc->tm_hour / 10);
	display->updateDigit(digit1, tm_rtc->tm_hour % 10);
	display->updateDigit(digit2, tm_rtc->tm_min / 10);
	display->updateDigit(digit3, tm_rtc->tm_min % 10);
}

// Configure timer 0 as ms counter
static void configureTimer0Interrupt() {
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);

	// set prescaler to 64
	TCCR0B |= (1 << CS01) | (1 << CS00);

	// Set the value to count to 249 (0-249 means 250 ticks)
	OCR0A = F_CPU / 64 / 1000 - 1;

	//Set the ISR COMPA vect
	TIMSK0 |= (1 << OCIE0A);
}

static void enableUsartRxInterrupt() {
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
	timestamp = (time_t) buffer.a;
	// enableInterrupts();
}

int main() {
	HT16K33_Display display(i2cMaster, displayAddress);
	DS1307 rtc(i2cMaster, rtcAddress);

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
			struct tm *tm_rtc = localtime(&time_gm);
			updateDisplay(tm_rtc, &display);

			display.toggleColon();

			enableInterrupts();
		}
	}
}
