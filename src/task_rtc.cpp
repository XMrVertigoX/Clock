#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/simpletask.hpp>

#include "ds1307.hpp"

#include "task_rtc.hpp"

using namespace xXx;

Task_RTC::Task_RTC(DS1307 &rtc, QueueHandle_t &queue)
    : _queue(queue), _rtc(rtc) {}

Task_RTC::~Task_RTC() {}

void Task_RTC::setup() {}

void Task_RTC::loop() {
    if (0 == uxQueueSpacesAvailable(_queue)) {
        union {
            uint8_t a[4];
            time_t b;
        } timestamp;

        for (uint8_t i = 0; i < sizeof(time_t); i++) {
            xQueueReceive(_queue, &timestamp.a[i], 0);
        }

        _rtc.write(timestamp.b - UNIX_OFFSET);
    }
}
