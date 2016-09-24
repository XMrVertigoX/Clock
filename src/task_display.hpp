#include <xXx/templates/singleton.hpp>
#include <xXx/utils/arduinotask.hpp>

using namespace xXx;

class Task_Display : public ArduinoTask, public Singleton<Task_Display> {
    friend class Singleton<Task_Display>;

   public:
    void setup();
    void loop();

   private:
    Task_Display() = default;
    ~Task_Display() = default;
};
