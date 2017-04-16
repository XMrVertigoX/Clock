#include "si1145.hpp"

#include <stdint.h>

#include "../drivers/twi.hpp"

SI1145::SI1145(Twi &twi, uint8_t address) : _twi(twi), _address(address) {}

SI1145::~SI1145() {}
