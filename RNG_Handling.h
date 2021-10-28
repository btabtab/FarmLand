#ifndef RNG_HANDLING
#define RNG_HANDLING

#include <stdint.h>
#include <stdlib.h>

uint64_t getRngSeedSection(uint8_t index);
uint32_t getRandomNumber();

#endif