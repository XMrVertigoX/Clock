#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <stdint.h>

class Twi {
   private:
    static Twi* theInstance;

    Twi();

   public:
    virtual ~Twi();

    uint8_t startTransmission();
    uint8_t stopTransmission();
    uint8_t readBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes);
    uint8_t writeBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes);

    static Twi* getInstance();
};

#endif
