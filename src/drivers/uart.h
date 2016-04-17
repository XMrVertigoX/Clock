#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

class Uart {
   private:
    static Uart* _instance;
    FILE* stream = NULL;
    Uart();

   public:
    virtual ~Uart();
    static Uart* getInstance();
    FILE* getStream();
};

#endif
