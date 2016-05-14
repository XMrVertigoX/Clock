#ifndef DS1307_H_
#define DS1307_H_

#include <time.h>

#include "twi.hpp"

class DS1307 {
   private:
    uint8_t _address;

   public:
    DS1307(uint8_t address);
    ~DS1307();

    time_t read();
    void write(time_t ntpTime);
};

#endif