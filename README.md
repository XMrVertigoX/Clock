# Clock

An simple kitchen clock.

This software runs on a ATmega328P microcontroller and uses a DS1307 as calendar and a HT16K33 as led display controller. Everything is wired via I2C.

## Usage

The time can be set via serial line. Every four bytes result in a timestamp that is tranfered to the rtc.

## Todo

- [ ] Transfer modules into xXx_CPP library as components
- [ ] Implement automatic sleep mode for FreeRTOS
- [ ] Reengineer drivers to object orientes variants that uses the xXx interfaces
