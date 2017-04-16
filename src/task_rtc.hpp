#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/simpletask.hpp>
#include <xXx/templates/queue.hpp>

#include "modules/ds1307.hpp"

using namespace xXx;

class Task_RTC : public SimpleTask {
   public:
    Task_RTC(DS1307 &rtc, Queue<uint8_t> &queue);
    ~Task_RTC();
    void setup();
    void loop();

   private:
    Queue<uint8_t> &_queue;
    DS1307 &_rtc;
};
