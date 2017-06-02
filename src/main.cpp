#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/eu_dst.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/templates/queue.hpp>
#include <xXx/utils/logging.hpp>

#include "drivers/twi.hpp"
#include "drivers/uart.hpp"
#include "modules/ds1307.hpp"
#include "modules/ht16k33_segment.hpp"
#include "modules/si1145.hpp"
#include "task_display.hpp"
#include "task_rtc.hpp"

#define disableInterrupts() cli()
#define enableInterrupts() sei()

static const uint8_t displayAddress     = 0x70;
static const uint8_t rtcAddress         = 0x68;
static const uint8_t lightSensorAddress = 0x60;

Uart uart;
Twi twi;

HT16K33_Segment display(twi, displayAddress);
DS1307 rtc(twi, rtcAddress);
SI1145 lightSensor(twi, lightSensorAddress);

Queue<uint8_t> rxQueue(4);

Task_Display task_display(display, rtc);
Task_RTC task_rtc(rtc, rxQueue);

void enableUartRxInterrupt() {
    UCSR0B |= (1 << RXCIE0);
}

ISR(USART_RX_vect) {
    uint8_t byte = uart.UART_Rx(NULL);
    rxQueue.enqueueFromISR(byte);
}

int main() {
    twi.init();
    uart.init();
    uart.enableIOStreams();

    set_dst(eu_dst);
    set_zone(1 * ONE_HOUR);

    enableUartRxInterrupt();
    enableInterrupts();

    task_display.create();
    task_rtc.create();

    LOG("Enter scheduler");

    vTaskStartScheduler();
}
