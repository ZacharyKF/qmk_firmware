#ifdef QMK_KEYBOARD_H
    #include QMK_KEYBOARD_H
#else
    #include "../../gergoplex.h"
    #include "../../config.h"
#endif

// This macro translates row column to bit mask
#define RCT64(R, C) (1llu << (R + (MATRIX_ROWS * C)))

// Creating a bit mask for each key
#define KR4C0 RCT64(4,0)
#define KR3C0 RCT64(3,0)
#define KR2C0 RCT64(2,0)
#define KR1C0 RCT64(1,0)
#define KR0C0 RCT64(0,0)
#define KR5C0 RCT64(5,0)
#define KR6C0 RCT64(6,0)
#define KR7C0 RCT64(7,0)
#define KR8C0 RCT64(8,0)
#define KR9C0 RCT64(9,0)
#define KR4C1 RCT64(4,1)
#define KR3C1 RCT64(3,1)
#define KR2C1 RCT64(2,1)
#define KR1C1 RCT64(1,1)
#define KR0C1 RCT64(0,1)
#define KR5C1 RCT64(5,1)
#define KR6C1 RCT64(6,1)
#define KR7C1 RCT64(7,1)
#define KR8C1 RCT64(8,1)
#define KR9C1 RCT64(9,1)
#define KR4C2 RCT64(4,2)
#define KR3C2 RCT64(3,2)
#define KR2C2 RCT64(2,2)
#define KR1C2 RCT64(1,2)
#define KR0C2 RCT64(0,2)
#define KR5C2 RCT64(5,2)
#define KR6C2 RCT64(6,2)
#define KR7C2 RCT64(7,2)
#define KR8C2 RCT64(8,2)
#define KR9C2 RCT64(9,2)
#define KR3C3 RCT64(3,3)
#define KR2C3 RCT64(2,3)
#define KR1C3 RCT64(1,3)
#define KR6C3 RCT64(6,3)
#define KR7C3 RCT64(7,3)
#define KR8C3 RCT64(8,3)

// Array for testing
const uint64_t key_to_binary[] = {
    KR4C0, KR3C0, KR2C0, KR1C0, KR0C0, KR5C0, KR6C0, KR7C0, KR8C0, KR9C0,
    KR4C1, KR3C1, KR2C1, KR1C1, KR0C1, KR5C1, KR6C1, KR7C1, KR8C1, KR9C1,
    KR4C2, KR3C2, KR2C2, KR1C2, KR0C2, KR5C2, KR6C2, KR7C2, KR8C2, KR9C2,
                  KR3C3, KR2C3, KR1C3, KR6C3, KR7C3, KR8C3
};

#define CMB_K1(a)                                     (a)
#define CMB_K2(a , b)                                 (a | b)
#define CMB_K3(a , b , s)                             (a | b | s)
#define CMB_K4(a , b , s , t)                         (a | b | s | t)
#define CMB_K5(a , b , s , t , u)                     (a | b | s | t | u)
#define CMB_K6(a , b , s , t , u , v )                (a | b | s | t | u | v)
#define CMB_K7(a , b , s , t , u , v , w)             (a | b | s | t | u | v | w)
#define CMB_K8(a , b , s , t , u , v , w , x )        (a | b | s | t | u | v | w | x)
#define CMB_K9(a , b , s , t , u , v , w , x , y)     (a | b | s | t | u | v | w | x | y)
#define CMB_K0(a , b , s , t , u , v , w , x , y , z) (a | b | s | t | u | v | w | x | y | z)

// Using macro overloading from https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
// Need a macro to condense unlimited Ks to a single value
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME,...) NAME
#define CMB_K(...) GET_MACRO(__VA_ARGS__, CMB_K0, CMB_K9, CMB_K8 CMB_K7, CMB_K6, CMB_K5, CMB_K4, CMB_K3, CMB_K2, CMB_K1)(__VA_ARGS__)

// Translate our chords into one array for the keys
#define PROCESS_CHORD(O, ...) CMB_K(__VA_ARGS__)
#define CHORD(O, ...) PROCESS_CHORD(O, __VA_ARGS__)
const uint64_t chords[] = {
    #include "chords.def"
};

// And one array for the values
#undef PROCESS_CHORD
#define PROCESS_CHORD(O, ...) O
#define CHORD(O, ...) PROCESS_CHORD(O, __VA_ARGS__)
const uint16_t chords_codes[] = {
    #include "chords.def"
};

const uint16_t num_chords = sizeof(chords)/sizeof(uint64_t);
