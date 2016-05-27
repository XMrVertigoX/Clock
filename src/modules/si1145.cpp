#include "si1145.hpp"

#include <stdint.h>

#include "twi.h"

SI1145::SI1145(uint8_t address) : _address(address) {}

SI1145::~SI1145() {}
