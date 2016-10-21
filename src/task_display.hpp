#include <time.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <xXx/os/arduinotask.hpp>

#include "ds1307.hpp"
#include "ht16k33_segment.hpp"

using namespace xXx;

class Task_Display : public ArduinoTask {
  public:
    Task_Display(HT16K33_Segment &display, DS1307 &rtc);
    ~Task_Display();
    void setup();
    void loop();

  private:
    TickType_t _lastWakeTime;
    HT16K33_Segment &_display;
    DS1307 &_rtc;
};
