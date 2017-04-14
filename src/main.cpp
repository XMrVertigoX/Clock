#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/eu_dst.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include "twi.h"
#include "uart.h"

#include "ds1307.hpp"
#include "ht16k33_segment.hpp"
#include "si1145.hpp"

#include "task_display.hpp"
#include "task_rtc.hpp"

#define disableInterrupts() cli()
#define enableInterrupts() sei()

static const uint8_t displayAddress     = 0x70;
static const uint8_t rtcAddress         = 0x68;
static const uint8_t lightSensorAddress = 0x60;

HT16K33_Segment display(displayAddress);
DS1307 rtc(rtcAddress);
SI1145 lightSensor(lightSensorAddress);

QueueHandle_t usartRxQueue;

Task_Display task_display(display, rtc);
Task_RTC task_rtc(rtc, usartRxQueue);

static void enableUartRxInterrupt() {
    UCSR0B |= (1 << RXCIE0);
}

ISR(USART_RX_vect) {
    uint8_t byte = UART_Rx(NULL);
    xQueueSendToBackFromISR(usartRxQueue, &byte, 0);
}

int main() {
    UART_Init();
    TWI_init();

    FILE *stream = fdevopen(UART_Tx, UART_Rx);
    stderr       = stream;
    stdin        = stream;
    stdout       = stream;

    usartRxQueue = xQueueCreate(4, sizeof(uint8_t));

    set_dst(eu_dst);
    set_zone(1 * ONE_HOUR);

    enableUartRxInterrupt();
    enableInterrupts();

    vTaskStartScheduler();
}
