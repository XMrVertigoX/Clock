#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/arduinotask.hpp>

#include "task_display.hpp"

using namespace xXx;

Task_Display::Task_Display(HT16K33_Segment &display, DS1307 &rtc)
    : ArduinoTask(128, 1), _display(display), _rtc(rtc) {}

Task_Display::~Task_Display() {}

void Task_Display::setup() {
    _lastWakeTime = xTaskGetTickCount();
    _display.setBrightness(0xF);
}

void Task_Display::loop() {
    vTaskDelayUntil(&_lastWakeTime, 1000 / portTICK_PERIOD_MS);

    time_t time_gm    = _rtc.read();
    struct tm *tm_rtc = localtime(&time_gm);

    _display.updateDigit(digit0, tm_rtc->tm_hour / 10);
    _display.updateDigit(digit1, tm_rtc->tm_hour % 10);
    _display.updateDigit(digit2, tm_rtc->tm_min / 10);
    _display.updateDigit(digit3, tm_rtc->tm_min % 10);
}
