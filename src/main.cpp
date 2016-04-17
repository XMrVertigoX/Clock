#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/eu_dst.h>

#include <FreeRTOS.h>
#include <task.h>

#include "ds1307.h"
#include "ht16k33_display.h"
#include "uart.h"

#define disableInterrupts() cli()
#define enableInterrupts() sei()
#define UART Uart::getInstance()
#define displayAddress 0x70
#define rtcAddress 0x68

HT16K33_Display* display = new HT16K33_Display(displayAddress);
DS1307* rtc = new DS1307(rtcAddress);

static void enableUsartRxInterrupt() {
    UCSR0B |= (1 << RXCIE0);
}

ISR(USART_RX_vect) {
    disableInterrupts();

    printf("USART_RX_vect: ");

    time_t timestamp;
    // fread(&timestamp, 4, 1, stdin);
    scanf("%d", &timestamp);
    // rtc->write(timestamp - UNIX_OFFSET);
    printf("%d -> %s\r\n", timestamp, ctime(&timestamp));

    enableInterrupts();
}

void task_updateDisplay(void* pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

        time_t time_gm = rtc->read();
        printf("%s\r\n", ctime(&time_gm));

        struct tm* tm_rtc = localtime(&time_gm);

        display->updateDigit(digit0, tm_rtc->tm_hour / 10);
        display->updateDigit(digit1, tm_rtc->tm_hour % 10);
        display->updateDigit(digit2, tm_rtc->tm_min / 10);
        display->updateDigit(digit3, tm_rtc->tm_min % 10);
    }

    vTaskDelete(NULL);
}

int main() {
    stderr = UART->getStream();
    stdin = UART->getStream();
    stdout = UART->getStream();

    set_dst(eu_dst);
    set_zone(1 * ONE_HOUR);
    display->setBrightness(0xF);

    enableUsartRxInterrupt();
    enableInterrupts();

    xTaskCreate(task_updateDisplay, NULL, configMINIMAL_STACK_SIZE, NULL,
            tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    return 0;
}
