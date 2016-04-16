#ifndef DS1307_H_
#define DS1307_H_

#include <time.h>

#include "i2cmaster.h"

class DS1307 {
   public:
    DS1307(uint8_t address);
    ~DS1307();

    time_t read();
    void write(time_t ntpTime);

   private:
    uint8_t address;
};

#endif
