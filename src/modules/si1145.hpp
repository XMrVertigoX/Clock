#ifndef SI1145_H_
#define SI1145_H_

#include <stdint.h>

class SI1145 {
   private:
    uint8_t _address;

   public:
    SI1145(uint8_t address);
    ~SI1145();
};

#endif
