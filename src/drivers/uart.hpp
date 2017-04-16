#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

class Uart {
   public:
    Uart()  = default;
    ~Uart() = default;

    static void init();
    static int UART_Rx(FILE *stream);
    static int UART_Tx(char byte, FILE *stream);
    static void enableIOStreams();
};

#endif
