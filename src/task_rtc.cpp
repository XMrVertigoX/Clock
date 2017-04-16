#include <stdio.h>
#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/simpletask.hpp>
#include <xXx/utils/logging.hpp>

#include "modules/ds1307.hpp"

#include "task_rtc.hpp"

using namespace xXx;

Task_RTC::Task_RTC(DS1307 &rtc, Queue<uint8_t> &queue) : _queue(queue), _rtc(rtc) {}

Task_RTC::~Task_RTC() {}

void Task_RTC::setup() {}

void Task_RTC::loop() {
    if (_queue.queueMessagesWaiting() >= 4) {
        union {
            uint8_t p8[4];
            time_t time;
        } timestamp;

        for (uint8_t i = 0; i < sizeof(timestamp); i++) {
            _queue.dequeue(timestamp.p8[i]);
        }

        _rtc.write(timestamp.time - UNIX_OFFSET);
    }
}
