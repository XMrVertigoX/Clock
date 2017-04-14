#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/simpletask.hpp>

#include "ds1307.hpp"

using namespace xXx;

class Task_RTC : public SimpleTask {
  public:
    Task_RTC(DS1307 &rtc, QueueHandle_t &queue);
    ~Task_RTC();
    void setup();
    void loop();

  private:
    QueueHandle_t &_queue;
    DS1307 &_rtc;
};
