#include "ht16k33_display.h"

#include <stdint.h>
#include <stdio.h>

#include <i2cmaster.h>

static const uint8_t colonOnMasks[] = {
    0b00000000,
    0b00000010
};

static const uint8_t numberMasks[] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};

HT16K33_Display::HT16K33_Display(I2cMaster *i2cMaster, i2cAddress_t address) {
    this->address = address;
    this->i2cMaster = i2cMaster;
    uint8_t initSequence[] = {0x21, 0xA0, 0xE0, 0x81};
    for (uint8_t i = 0; i < sizeof(initSequence); i++) {
        i2cMaster->startTransmission();
        i2cMaster->writeBytes(address, &initSequence[i], 1);
    }
    i2cMaster->stopTransmission();
}

HT16K33_Display::~HT16K33_Display() {
}

void HT16K33_Display::setBrightness(uint8_t brightness) {
    if (brightness > 0xF) {
        brightness = 0xF;
    }
    brightness += 0xE0; // Brightness register offset
    i2cMaster->startTransmission();
    i2cMaster->writeBytes(address, &brightness, 1);
    i2cMaster->stopTransmission();
}

void HT16K33_Display::toggleColon() {
    uint8_t tmp[2] = {digitColon, colonOnMasks[colonState]};
    i2cMaster->startTransmission();
    i2cMaster->writeBytes(address, tmp, sizeof(tmp));
    i2cMaster->stopTransmission();
    this->colonState = !colonState;
}

void HT16K33_Display::updateDigit(digit_t digit, uint8_t number) {
    uint8_t tmp[] = {digit, numberMasks[number]};
    i2cMaster->startTransmission();
    i2cMaster->writeBytes(address, tmp, sizeof(tmp));
    i2cMaster->stopTransmission();
}
