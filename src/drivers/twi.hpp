#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

class Twi {
   public:
    Twi()  = default;
    ~Twi() = default;

    static void init(void);
    static uint8_t startTransmission(void);
    static void stopTransmission(void);
    static uint8_t readBytes(uint8_t address, uint8_t *bytes, uint32_t numBytes);
    static uint8_t writeBytes(uint8_t address, uint8_t *bytes, uint32_t numBytes);
};

#endif /* TWI_H_ */
