/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H
#include "vortus_engine.h"
#include <print.h>
#define HOLD_TIME 300

#define BASE 0 // default layer

// Blank template at the bottom
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,-----------------------------.       ,--------------------------------.
 * |    Q |  W  |  E  |  R  |  T  |      |  Y  |  U  |  I  |  O  |    P   |
 * |-----+-----+-----+-----+------|      |--------------------------------|
 * |CTRL/A|  S  |  D  |  F  |  G  |      |  H  |  J  |  K  |  L  | CTRL/; |
 * |-----+-----+-----+-----+------+		   |--------------------------------|
 * |SHFT/Z|  X  |  C  |  V  |  B  |      |  N  |  M  |  <  |  >  | SHFT/? |
 * `------+-----+-----+------+----'		   `--------------------------------'
 *  .-------------------------.           .-----------------.
 *  |ESC/META|ENT/ALT|SPC(SYM)|           |SPC(NUM)|BSPC|TAB|
 *  '-------------------------'           '-----------------'
 */
[BASE] = LAYOUT_gergoplex(
    KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,   KC_GRV,  KC_TILD, KC_TRNS, KC_TRNS, KC_BSLS,
    KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_BTN2,   KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_QUOT,
    KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_BTN1,   KC_AMPR, KC_EQL,  KC_COMM, KC_DOT,  KC_MINS,
                      KC_BTN3, KC_SCLN, KC_EQL,   KC_EQL,  KC_SCLN, KC_DEL															// Right
    )
};


uint64_t keymap = 0llu;
uint64_t new_keymap = 0llu;
uint64_t largest_chord = 0llu;
bool unpressing = false;
uint16_t *tmp_test = (uint16_t *)&new_keymap;
uint16_t *tmp_test_2 = (uint16_t *)&keymap;

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

    new_keymap = (1llu << ((record->event.key.row + (MATRIX_ROWS * record->event.key.col))));

    // Translate the row-column to a bitwise position ((row + (width * col)) - 1)
    if (record->event.pressed){
        keymap = new_keymap | keymap;
    } else {
        keymap = (~new_keymap) & keymap;
    }

    if (true){
        dprintf("\r\nPressed: %s", record->event.pressed ? "True" : "False");
        dprintf("\r\nRow: %d, Col: %d", record->event.key.row, record->event.key.col);

        dprint("\r\nKey : ");

        for(uint8_t i = 0; i < 4; i++) {
            dprintf("%ux", tmp_test[i]);
        }

        dprint("\r\nMask: ");

        for(uint8_t i = 0; i < 4; i++) {
            dprintf("%ux", tmp_test_2[i]);
        }

        for(uint16_t i = 0; i < 36; i++) {
            if (new_keymap == key_to_binary[i]){
                dprintf("\r\nKeyToBinary: %u", i);
                break;
            }
        }

        for(uint16_t i = 0; i < 36; i++) {
            if (keymap == chords[i]){
                dprintf("\r\nChord: %u", i);
                break;
            }
        }

        dprint("\r\n");
    }

    return false;

    // If the record is a de-press, ensure the bitwise position is 0, otherwise set it to 1
    if (!record->event.pressed){
        new_keymap = ~new_keymap & keymap;
    } else {
        new_keymap = new_keymap | keymap;
    }

    // a) Sending a chord if the user is depressing the chord
    // If our keymap's value is less than the old keymap, then something has been depressed and we can send the chord
    //   once, if something new is pressed then the value is greater and we'll rely on the rest of the algorithm to send
    //   keys if required. The third case of equality indicates holding and will be implemented later
    if (new_keymap < keymap) {
        if (!unpressing) {
            // TODO: Send keymap logic here
            unpressing = true;
        }
        keymap = new_keymap;
        return false;
    } else if (new_keymap > keymap) {
        keymap = new_keymap;
        unpressing =false;
    }

    // b) Determine if our keymap is a strict subset, 0 is the subset of all sets so ignore it
    if (keymap != 0){

        // Zero out our largest chord
        largest_chord = 0;

        for (int i = 0; i < num_chords; i ++) {

            // Double check for equality to make our checks only match strict subsets or supersets
            if (chords[i] != keymap) {

                // Check if the chord from the list is a superset of our current chord
                if ((chords[i] & keymap) == keymap) {
                    return false;

                // Check if the chord from the list is a subset of our keymap, and larger than the largest chord found
                //   so far
                } else if ((chords[i] & keymap) == chords[i] && chords[i] > largest_chord) {
                    largest_chord = chords[i];
                }
            }
        }

        // If we're still here that means that the keycode is not a strict subset of another chord, this leaves us with
        //   the possibility of it being either an exact chord, a superset of another chord, or not a chord at all

        // Check if it's an exact chord (the largest chord found will be the same as our keymap)
        if (largest_chord == keymap) {

            // In this case we just want to send the chord

            return false;


        // Check if it's not a chord at all (the largest chord found will still be 0)
        } else if (largest_chord == 0) {

            // In this case we want to send the original keycode, this combined with the next option allow clean
            //   interaction with QMK
            return true;

        // Check if it's the superset of another chord (the largest chord will not be equal to keymap)
        } else if (largest_chord != keymap) {

            // In this case we can send the largest chord, followed by returning true. The complicated bit is masking
            //   out the chord from QMK output

            return true;

        }


    }

    // If all else fails, then just return false
    return false;
}


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
//   debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
