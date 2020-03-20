#ifdef QMK_KEYBOARD_H
    #include QMK_KEYBOARD_H
#else
    #include "../../gergoplex.h"
    #include "../../config.h"
#endif

#define TESTING

#ifdef TESTING
    void printChord(uint64_t* chord);
#endif

short count_bits(uint64_t value);
