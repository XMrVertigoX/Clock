#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

class Uart {
   private:
    FILE* stream;

   public:
    Uart();
    virtual ~Uart();

    uint8_t receiveBytes(uint8_t bytes[], uint32_t numBytes);
    uint8_t sendBytes(uint8_t bytes[], uint32_t numBytes);

    FILE* getStream();
};

#endif
