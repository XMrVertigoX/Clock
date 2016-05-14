#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/eu_dst.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include "uart.hpp"

#include "ds1307.hpp"
#include "ht16k33_segment.hpp"
#include "si1145.hpp"

#define UART Uart::getInstance()
#define disableInterrupts() cli()
#define enableInterrupts() sei()
#define UART Uart::getInstance()
#define displayAddress 0x70
#define rtcAddress 0x68
#define sensorAddress 0x60

HT16K33_Segment *display;
DS1307 *rtc;
SI1145 *sensor;

SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;

static void enableUsartRxInterrupt() {
    UCSR0B |= (1 << RXCIE0);
}

ISR(USART_RX_vect) {
    uint8_t byte;
    fread(&byte, 1, 1, stdin);
    xQueueSendFromISR(xQueue, &byte, 0);
    xSemaphoreGiveFromISR(xSemaphore, NULL);
}

void task_updateDisplay(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    display->setBrightness(0xF);

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_PERIOD_MS);

        time_t time_gm = rtc->read();
        struct tm *tm_rtc = localtime(&time_gm);

        display->updateDigit(digit0, tm_rtc->tm_hour / 10);
        display->updateDigit(digit1, tm_rtc->tm_hour % 10);
        display->updateDigit(digit2, tm_rtc->tm_min / 10);
        display->updateDigit(digit3, tm_rtc->tm_min % 10);

        printf("%s\n", asctime(tm_rtc));
    }

    vTaskDelete(NULL);
}

void task_updateRtc(void *pvParameters) {
    for (;;) {
        if (pdTRUE == xSemaphoreTake(xSemaphore, 0)) {
            if (4 == uxQueueMessagesWaiting(xQueue)) {
                union {
                    uint8_t a[4];
                    time_t b;
                } timestamp;

                for (size_t i = 0; i < 4; i++) {
                    xQueueReceive(xQueue, &timestamp.a[i], 0);
                }

                rtc->write(timestamp.b - UNIX_OFFSET);
            }
        }
    }

    vTaskDelete(NULL);
}

int main() {
    stderr = UART->getStream();
    stdin = UART->getStream();
    stdout = UART->getStream();

    xSemaphore = xSemaphoreCreateBinary();
    xQueue = xQueueCreate(4, 1);

    display = new HT16K33_Segment(displayAddress);
    rtc = new DS1307(rtcAddress);
    sensor = new SI1145(sensorAddress);

    set_dst(eu_dst);
    set_zone(1 * ONE_HOUR);

    enableUsartRxInterrupt();
    enableInterrupts();

    xTaskCreate(task_updateDisplay, NULL, configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(task_updateRtc, NULL, configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    return 0;
}
