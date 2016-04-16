#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

class Uart {
   public:
    Uart();
    virtual ~Uart();

    int8_t receiveBytes(uint8_t bytes[], uint32_t numBytes);
    int8_t sendBytes(uint8_t bytes[], uint32_t numBytes);

    FILE* getStream();

   private:
    FILE* stream;
};

#endif
