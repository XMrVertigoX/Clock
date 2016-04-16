#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/twi.h>

#include "twi.h"

#ifndef F_SCL
#define F_SCL 400000
#endif

Twi* Twi::theInstance = NULL;

static inline uint8_t calculateBitRate() {
    return (((F_CPU / F_SCL) - 16) / 2);
}

static inline void setBitRate() {
    TWBR = calculateBitRate();
}

static inline void waitForInterrupt() {
    while (!(TWCR & (1 << TWINT)))
        ;
}

static inline void writeControlRegister(uint8_t byte) {
    TWCR = byte;
}

static inline uint8_t readDataRegister() {
    return TWDR;
}

static inline void writeDataRegister(uint8_t byte) {
    TWDR = byte;
}

static uint8_t sendStartCondition() {
    writeControlRegister((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
    waitForInterrupt();
    return TW_STATUS;
}

static void sendStopCondition() {
    writeControlRegister((1 << TWINT) | (1 << TWSTO) | (1 << TWEN));
}

static uint8_t sendAddress(uint8_t address, uint8_t rw) {
    writeDataRegister((address << 1) | rw);
    writeControlRegister((1 << TWINT) | (1 << TWEN));
    waitForInterrupt();
    return TW_STATUS;
}

static uint8_t sendByte(uint8_t byte) {
    writeDataRegister(byte);
    writeControlRegister((1 << TWINT) | (1 << TWEN));
    waitForInterrupt();
    return TW_STATUS;
}

static uint8_t sendACK() {
    writeControlRegister((1 << TWINT) | (1 << TWEA) | (1 << TWEN));
    waitForInterrupt();
    return TW_STATUS;
}

static uint8_t sendNACK() {
    writeControlRegister((1 << TWINT) | (1 << TWEN));
    waitForInterrupt();
    return TW_STATUS;
}

Twi::Twi() {
    setBitRate();
}

Twi::~Twi() {
    delete theInstance;
}

Twi* Twi::getInstance() {
    if (!theInstance) {
        theInstance = new Twi;
    }

    return theInstance;
}

uint8_t Twi::startTransmission() {
    uint8_t status = sendStartCondition();

    if (status == TW_START || status == TW_REP_START) {
        return 0;
    } else {
        return 1;
    }
}

void Twi::stopTransmission() {
    sendStopCondition();
}

uint8_t Twi::readBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes) {
    uint8_t status = sendAddress(address, TW_READ);

    if (status != TW_MR_SLA_ACK) {
        return 1;
    }

    for (uint32_t i = 0; i < numBytes; i++) {
        if (i < (numBytes - 1)) {
            status = sendACK();
        } else {
            status = sendNACK();
        }

        if (status == TW_MR_DATA_ACK || status == TW_MR_DATA_NACK) {
            bytes[i] = readDataRegister();
        } else {
            return 1;
        }
    }

    return 0;
}

uint8_t Twi::writeBytes(uint8_t address, uint8_t bytes[], uint32_t numBytes) {
    uint8_t status = sendAddress(address, TW_WRITE);

    if (status != TW_MT_SLA_ACK) {
        return 1;
    }

    for (uint32_t i = 0; i < numBytes; i++) {
        status = sendByte(bytes[i]);
        if (status != TW_MT_DATA_ACK) {
            return 1;
        }
    }
    return 0;
}
