#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>

#include "uart.hpp"

#define WAIT_UNTIL(x) while (!(x))

static FILE *__stream;

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

void Uart::init() {
    setBaudRate();
    setFrameFormat();
    enableRxAndTx();
}

int Uart::UART_Rx(FILE *stream) {
    WAIT_UNTIL(incomingTransmissionComplete());
    WAIT_UNTIL(transmissionBufferReady());

    return (UDR0);
}

int Uart::UART_Tx(char byte, FILE *stream) {
    WAIT_UNTIL(transmissionBufferReady());
    UDR0 = byte;
    WAIT_UNTIL(outgoingTransmissionComplete());

    return (0);
}

void Uart::enableIOStreams() {
    __stream = fdevopen(UART_Tx, UART_Rx);
    stderr   = __stream;
    stdin    = __stream;
    stdout   = __stream;
}
