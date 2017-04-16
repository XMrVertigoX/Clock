#include "ht16k33_segment.hpp"

#include <stdint.h>
#include <stdio.h>

#include "../drivers/twi.hpp"

static const uint8_t colonMask[] = {0b00000000, 0b00000010};

static const uint8_t numberMasks[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
                                      0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

HT16K33_Segment::HT16K33_Segment(Twi &twi, uint8_t address) : _twi(twi), _address(address) {}

HT16K33_Segment::~HT16K33_Segment() {}

void HT16K33_Segment::init() {
    uint8_t initSequence[] = {0x21, 0xA0, 0xE0, 0x81};

    for (uint8_t i = 0; i < sizeof(initSequence); i++) {
        _twi.startTransmission();
        _twi.writeBytes(_address, &initSequence[i], 1);
    }

    _twi.stopTransmission();
}

void HT16K33_Segment::setBrightness(uint8_t brightness) {
    if (brightness > 0xF) {
        brightness = 0xF;
    }

    brightness += 0xE0;  // Brightness register offset

    _twi.startTransmission();
    _twi.writeBytes(_address, &brightness, 1);
    _twi.stopTransmission();
}

void HT16K33_Segment::updateColon(bool enable) {
    uint8_t tmp[] = {digitColon, colonMask[enable % sizeof(colonMask)]};

    _twi.startTransmission();
    _twi.writeBytes(_address, tmp, 2);
    _twi.stopTransmission();
}

void HT16K33_Segment::updateDigit(digit_t digit, uint8_t number) {
    uint8_t tmp[] = {digit, numberMasks[number % sizeof(numberMasks)]};

    _twi.startTransmission();
    _twi.writeBytes(_address, tmp, 2);
    _twi.stopTransmission();
}
