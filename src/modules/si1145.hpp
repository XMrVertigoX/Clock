#ifndef SI1145_H_
#define SI1145_H_

#include <stdint.h>

#include "../drivers/twi.hpp"

class SI1145 {
   private:
    Twi &_twi;
    uint8_t _address;

   public:
    SI1145(Twi &twi, uint8_t address);
    ~SI1145();
};

#endif
