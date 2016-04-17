#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

class Twi {
   private:
    static Twi* _instance;

    Twi();

   public:
    virtual ~Twi();

    static Twi* getInstance();

    uint8_t startTransmission();
    void stopTransmission();
    uint8_t readBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes);
    uint8_t writeBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes);
};

#endif
