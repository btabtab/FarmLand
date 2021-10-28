#include "RNG_Handling.h"

//in total this is a long line of 512 (64 * 8) bits to handle RNG.
uint64_t rng_seed_sections[8];

uint64_t getRngSeedSection(uint8_t index)
{
    if(index < 7){}
    return rng_seed_sections[index];
}
uint32_t getRandomNumber()
{
    return rand() % ~((uint32_t)0);
}
