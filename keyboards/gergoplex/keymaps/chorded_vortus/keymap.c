/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */
#ifdef QMK_KEYBOARD_H
    #include QMK_KEYBOARD_H
#else
    #include "../../gergoplex.h"
    #include "../../config.h"
#endif
#include "map_queue.h"
#include "vortus_engine.h"
#include "chords.h"
#ifdef TESTING
    #include <print.h>
#endif
#define HOLD_TIME 300

#define BASE 0 // default layer

// Blank template at the bottom
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,----------------------------.       ,-----------------------------.
 * |  Q  |  D  |  R  |  W  |  B  |      |  J  |  F  |  U  |  P  |  ;  |
 * |-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----|
 * |  A  |  S  |  H  |  T  |  G  |      |  Y  |  N  |  E  |  O  |  I  |
 * |-----+-----+-----+-----+-----|	    |-----+-----+-----+-----+-----|
 * |  Z  |  X  |  M  |  C  |  V  |      |  K  |  L  |  ,  |  .  |  /  |
 * `-----------------------------'	    `-----------------------------'
 *            .------------------.      .------------------.
 *            | DEL | LSFT | SPC |      | ENT | TAB | BSPC |
 *            '------------------'      '------------------'
 */
[BASE] = LAYOUT_gergoplex(
    KC_Q,   KC_D,   KC_R,    KC_W, KC_B,    KC_J,   KC_F,   KC_U,    KC_P,   KC_COLN,
    KC_A,   KC_S,   KC_H,    KC_T, KC_G,    KC_Y,   KC_N,   KC_E,    KC_O,   KC_I,
    KC_Z,   KC_X,   KC_M,    KC_C, KC_V,    KC_K,   KC_L,   KC_COMM, KC_DOT, KC_SLASH,
            KC_DEL, KC_LSFT, KC_SPC,        KC_ENT, KC_TAB, KC_BSPC
    )
};

uint64_t new_key_mask = 0llu;
uint64_t new_mask = 0llu;
uint64_t prev_mask = 0llu;
uint64_t largest_chord = 0llu;
uint64_t send_mask = 0llu;
short chord_code = 0;
bool unpressing = false;
bool subset = false;


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    /**
     * Algorithm for matching input to a chorded result, follows theses rules:
     *    1. If the input is not a subset of or match to a chord, output it immediately (non-blocking)
     *    2. If the input is a subset, then send it on release of any keys (prioritize long sequence)
     *    3. If the input is a match, but not a subset, send it immediately (non-blocking)
     *    4. If a chord and additional keys are pressed, send both
     *
     *  This effectively creates a chorded keyboard where inputs are not held until release if they are not part of a
     *    chord, and chords are sent as soon as they've reached a maximum length. The downsides are a limit of
     *    (64 choose MATRIX_SIZE) chord combinations, and the chord definitions are key matrix dependent instead of
     *    keycode dependent. This still leaves plenty of options and functions well with QMK though.
     * */

    // Translate the row-column to a bitwise position ((row + (width * col)) - 1)
    new_key_mask = (1llu << ((record->event.key.row + (MATRIX_ROWS * record->event.key.col))));

    if (record->event.pressed){
        set_entry(&map, &new_key_mask, &keycode);
        new_mask = new_key_mask | prev_mask;
    } else {
        new_mask = (~new_key_mask) & prev_mask;
    }

    // #ifdef TESTING
    //     dprintf("new key mask ");
    //     printChord(&new_mask);
    //     dprintf("\r\n");
    // #endif

    if (record->event.pressed){

        unpressing = false;
        largest_chord = 0;
        chord_code = 0;

        for (short i = 0; i < num_chords; i ++) {

            /*
             * If we can find a chord that is a strict superset of the keys we have pressed, then it's safe to exit
             *   immediately. The intended output cannot be determined (does the user want to press more buttons?), so
             *   send nothing.
            */
            if (chords[i] != new_mask && (chords[i] & new_mask) == new_mask) {
                #ifdef TESTING
                    printChord(&new_mask);
                    dprintf(" subset of chord ");
                    printChord((uint64_t*)(chords + i));
                    dprintf("\r\n");
                #endif

                subset = true;

                prev_mask = new_mask;
                return false;
            }

            /*
             * If we can find a chord that's a subset (not strict), then make note of it. In the case that we'll be
             *   sending something, it will be the longest subset found.
            */
            if ((chords[i] & new_mask) == chords[i] && count_bits(chords[i]) > count_bits(largest_chord)) {
                largest_chord = chords[i];
                chord_code = i;
            }
        }

        // If we made it out of the loop we know we didn't detect a subset!
        subset = false;

        /*
         * If we've found any chord, ane we haven't exited, then we need to send the chord, and any adittional remaining
         *   keys, conveniently we can just fall through to sending a normal keycode if nothing was found. We set
         *   unpressing to true here to prevent sending the chord on depress.
        */
        if (largest_chord > 0) {
            #ifdef TESTING
                dprintf("sending chord ");
                printChord(&largest_chord);
                dprintf("\r\n");
            #endif

            send_mask = (~largest_chord) & new_mask;
            // TODO: Send largest chord here
            // TODO: Get remaining chords from map

            unpressing = true;

            prev_mask = new_mask;
            return false;
        }
    } else {
        /*
         * If we found a chord on our last loop, then we need to send it. The `unpressing` boolean ensures we only send
         *   it once.
         */
        if (largest_chord > 0 && !unpressing){
            #ifdef TESTING
                dprintf("sending chord ");
                printChord(&largest_chord);
                dprintf("\r\n");
            #endif

            unpressing = true;

            prev_mask = new_mask;
            return false;
        } else if (subset && !unpressing){

            #ifdef TESTING
            dprintf("regular keypress from chord %u\r\n", keycode);
            #endif

            prev_mask = new_mask;
            return false;
        }
    }

    #ifdef TESTING
    if (record->event.pressed){
        dprintf("fallthroug keypress %u\r\n", keycode);
    }
    #endif

    prev_mask = new_mask;
    return false;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
//   debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}


