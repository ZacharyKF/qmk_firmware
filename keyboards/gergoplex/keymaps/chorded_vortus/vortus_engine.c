#include <stdint.h>
#include "vortus_engine.h"

#ifdef TESTING
short count_bits(uint64_t value){
    short total_bits = 0;
    while (value != 0){
        value &=(value - 1);
        total_bits++;
    }
    return total_bits;
}

uint16_t *print_short;
void printChord(uint64_t* chord){
    print_short = (uint16_t*)chord;
    for(uint16_t i = 0; i < 4; i++){
        dprintf("%u", print_short[i]);
    }
}

void printShort(uint16_t* chord){
    dprintf("%u", *chord);
}

#endif
