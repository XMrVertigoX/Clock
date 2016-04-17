#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>

#include "heap.h"
#include "uart.h"

Uart* Uart::_instance = NULL;

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
    while (!incomingTransmissionComplete()) {
    }

    while (!transmissionBufferReady()) {
    }

    return UDR0;
}

static int sendByte(char byte, FILE* stream) {
    while (!transmissionBufferReady()) {
    }

    UDR0 = byte;

    while (!outgoingTransmissionComplete()) {
    }

    return 0;
}

Uart::Uart() {
    setBaudRate();
    setFrameFormat();
    enableRxAndTx();

    stream = fdevopen(sendByte, receiveByte);
}

Uart::~Uart() {
    delete _instance;
}

Uart* Uart::getInstance() {
    if (!_instance) {
        _instance = new Uart;
    }

    return _instance;
}

FILE* Uart::getStream() {
    return stream;
}
