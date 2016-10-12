#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void UART_Init();
int UART_Rx(FILE *stream);
int UART_Tx(char byte, FILE *stream);

#ifdef __cplusplus
}
#endif

#endif
