#ifndef DS1307_HPP_
#define DS1307_HPP_

#include <time.h>

#include "../drivers/twi.hpp"

class DS1307 {
   private:
    Twi &_twi;
    uint8_t _address;

   public:
    DS1307(Twi &twi, uint8_t address);
    ~DS1307();

    time_t read();
    void write(time_t ntpTime);
};

#endif /* DS1307_HPP_ */
