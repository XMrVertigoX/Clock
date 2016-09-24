#include <xXx/templates/singleton.hpp>
#include <xXx/utils/arduinotask.hpp>

using namespace xXx;

class Task_RTC : public ArduinoTask, public Singleton<Task_RTC> {
    friend class Singleton<Task_RTC>;

   public:
    void setup();
    void loop();

   private:
    Task_RTC() = default;
    ~Task_RTC() = default;
};
