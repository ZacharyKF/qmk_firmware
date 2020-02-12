/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H
#include "zach_engine.h"
#define RCS R1C1 = SAFE_RANGE

enum CustomCodes {
    RCS,  R1C2, R1C3, R1C4, R1C5,       R1C6, R1C7, R1C8, R1C9, R1C0,
    R2C1, R2C2, R2C3, R2C4, R2C5,       R2C6, R2C7, R2C8, R2C9, R2C0,
    R3C1, R3C2, R3C3, R3C4, R3C5,       R3C6, R3C7, R3C8, R3C9, R3C0,
                R4C3, R4C4, R4C5,       R4C6, R4C7, R4C8
};

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
    R1C1, R1C2, R1C3, R1C4, R1C5,       R1C6, R1C7, R1C8, R1C9, R1C0,
    R2C1, R2C2, R2C3, R2C4, R2C5,       R2C6, R2C7, R2C8, R2C9, R2C0,
    R3C1, R3C2, R3C3, R3C4, R3C5,       R3C6, R3C7, R3C8, R3C9, R3C0,
                R4C3, R4C4, R4C5,       R4C6, R4C7, R4C8															// Right
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return true;
    }
    return true;
}
