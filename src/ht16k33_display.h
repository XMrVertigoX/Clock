#ifndef HT16K33_DISPLAY_H_
#define HT16K33_DISPLAY_H_

#include <stdbool.h>
#include <stdint.h>

#include <i2cmaster.h>

enum digit_t {
    digit0 = 0,
    digit1 = 2,
    digitColon = 4,
    digit2 = 6,
    digit3 = 8
};

class HT16K33_Display {
    public:
        HT16K33_Display(I2cMaster *i2cMaster, i2cAddress_t address);
        ~HT16K33_Display();
        void setBrightness(uint8_t brightness);
        void toggleColon();
        void updateDigit(digit_t digit, uint8_t number);
    private:
        I2cMaster *i2cMaster;
        i2cAddress_t address;
        bool colonState;
};

#endif
