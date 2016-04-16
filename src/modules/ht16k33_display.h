#ifndef HT16K33_DISPLAY_H_
#define HT16K33_DISPLAY_H_

#include <stdint.h>

#include "twi.h"

enum digit_t { digit0 = 0, digit1 = 2, digitColon = 4, digit2 = 6, digit3 = 8 };

class HT16K33_Display {
   public:
    HT16K33_Display(uint8_t address);
    ~HT16K33_Display();

    void setBrightness(uint8_t brightness);
    void toggleColon();
    void updateDigit(digit_t digit, uint8_t number);

   private:
    uint8_t address;
};

#endif
