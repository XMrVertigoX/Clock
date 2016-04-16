#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>

#include "uart.h"

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

static int receiveByte(FILE* stream) {
    // while(!incomingTransmissionComplete());
    while (!transmissionBufferReady())
        ;
    return UDR0;
}

static int sendByte(char byte, FILE* stream) {
    // while(!outgoingTransmissionComplete());
    while (!transmissionBufferReady())
        ;
    UDR0 = byte;
    return 0;
}

Uart::Uart() {
    setBaudRate();
    setFrameFormat();
    enableRxAndTx();

    stream = fdevopen(sendByte, receiveByte);
}

Uart::~Uart() {
}

uint8_t Uart::receiveBytes(uint8_t bytes[], uint32_t numBytes) {
    fread(bytes, sizeof(bytes[0]), numBytes, stream);
    return 0;
}

uint8_t Uart::sendBytes(uint8_t bytes[], uint32_t numBytes) {
    fwrite(bytes, sizeof(bytes[0]), numBytes, stream);
    return 0;
}

FILE* Uart::getStream() {
    return stream;
}
