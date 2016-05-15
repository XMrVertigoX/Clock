#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>

#include "uart.h"

#define waitUntil(x) while (!(x))

static inline bool incomingTransmissionComplete() {
    return (UCSR0A & (1 << RXC0));
}

static inline bool outgoingTransmissionComplete() {
    return (UCSR0A & (1 << TXC0));
}

static inline bool transmissionBufferReady() {
    return (UCSR0A & (1 << UDRE0));
}

static void setBaudRate() {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
}

static void setFrameFormat() {
    // Set frame format to 8 bits, no parity and one stop bit
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

static void enableRxAndTx() {
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void UART_Init() {
    setBaudRate();
    setFrameFormat();
    enableRxAndTx();
}

int UART_Rx(FILE* stream) {
    waitUntil(incomingTransmissionComplete());
    waitUntil(transmissionBufferReady());
    return UDR0;
}

int UART_Tx(char byte, FILE* stream) {
    waitUntil(transmissionBufferReady());
    UDR0 = byte;
    waitUntil(outgoingTransmissionComplete());
    return 0;
}
