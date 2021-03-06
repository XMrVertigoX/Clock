#ifndef HT16K33_DISPLAY_HPP_
#define HT16K33_DISPLAY_HPP_

#include <stdint.h>

#include "../drivers/twi.hpp"

enum digit_t { digit0 = 0, digit1 = 2, digitColon = 4, digit2 = 6, digit3 = 8 };

class HT16K33_Segment {
   private:
    Twi &_twi;
    uint8_t _address;

   public:
    HT16K33_Segment(Twi &twi, uint8_t address);
    ~HT16K33_Segment();

    void init();
    void setBrightness(uint8_t brightness);
    void updateColon(bool enable);
    void updateDigit(digit_t digit, uint8_t number);
};

#endif /* HT16K33_DISPLAY_HPP_ */
