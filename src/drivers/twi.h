#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void TWI_init(void);
uint8_t TWI_startTransmission(void);
void TWI_stopTransmission(void);
uint8_t TWI_readBytes(uint8_t address, uint8_t* bytes, uint32_t numBytes);
uint8_t TWI_writeBytes(uint8_t address, uint8_t* bytes, uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif /* TWI_H_ */
