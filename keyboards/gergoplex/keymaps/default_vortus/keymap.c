/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define NAV 2 // SYMBers/motion
#define ____ KC_TRNS

#define Z_MALT OSM(MOD_RALT)
#define Z_MSFT OSM(MOD_RSFT)
#define Z_MGUI OSM(MOD_RGUI)
#define Z_MCTL OSM(MOD_RCTL)
#define Z_ENLT LT(NAV, KC_ENT)
#define Z_SPLT LT(SYMB, KC_SPC)

// Combos
enum combos {
	QA, DS, RH, WT, BG, JY, FN, UE, PO, SCLNI
};

// NOTE: If you're using MT,LT or anything you must
// define it here. Note this if you change your layout!
/* Keymap 0: Basic layer combos
 *
 * ,-----------------------------.       ,--------------------------------.
 * |      |     |     |     |     |      |     |     |     |     |        |
 * |-+1+--+-+2+-+-+3+-+-+4+-+-+5+-|      |-+6+-+-+7+-+-+8+-+-+9+-+---+0+--|
 * |      |     |     |     |     |      |     |     |     |     |        |
 * |------+-----+-----+-----+-----|	     |-----+-----+-----+-----+--------|
 * |      |     |     |     |     |      |     |     |     |     |        |
 * `------------------------------'		 `--------------------------------'
 *           .--------------------.      .--------------------.
 *           |      |      |      |      |      |      |      |
 *           '--------------------'      '--------------------'
 */
// Top row vertical
const uint16_t PROGMEM qa_c[] = {KC_Q, KC_A, COMBO_END};
const uint16_t PROGMEM ds_c[] = {KC_D, KC_S, COMBO_END};
const uint16_t PROGMEM rh_c[] = {KC_R, KC_H, COMBO_END};
const uint16_t PROGMEM wt_c[] = {KC_W, KC_T, COMBO_END};
const uint16_t PROGMEM bg_c[] = {KC_B, KC_G, COMBO_END};
const uint16_t PROGMEM jy_c[] = {KC_J, KC_Y, COMBO_END};
const uint16_t PROGMEM fn_c[] = {KC_F, KC_N, COMBO_END};
const uint16_t PROGMEM ue_c[] = {KC_U, KC_E, COMBO_END};
const uint16_t PROGMEM po_c[] = {KC_P, KC_O, COMBO_END};
const uint16_t PROGMEM sclni_c[] = {KC_SCLN, KC_I, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {

    // Vertical Codes, top row
    [QA]    = COMBO(qa_c,       KC_LPRN),
    [DS]    = COMBO(ds_c,       KC_LBRC),
    [RH]    = COMBO(rh_c,       KC_3),
    [WT]    = COMBO(wt_c,       KC_4),
    [BG]    = COMBO(bg_c,       KC_5),
    [JY]    = COMBO(jy_c,       KC_6),
    [FN]    = COMBO(fn_c,       KC_7),
    [UE]    = COMBO(ue_c,       KC_8),
    [PO]    = COMBO(po_c,       KC_RBRC),
    [SCLNI] = COMBO(sclni_c,    KC_RPRN),
};

// Blank template at the bottom
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,-----------------------------.       ,--------------------------------.
 * |    Q |  D  |  R  |  W  |  B  |      |  J  |  F  |  U  |  P  | SCLN   |
 * |------+-----+-----+-----+-----|      |-----+-----+-----+-----+--------|
 * |    A |  S  |  H  |  T  |  G  |      |  Y  |  N  |  E  |  O  | I      |
 * |------+-----+-----+-----+-----|	     |-----+-----+-----+-----+--------|
 * |    Z |  X  |  M  |  C  |  V  |      |  K  |  L  |COMM |DOT  | SLSH   |
 * `------------------------------'		 `--------------------------------'
 *           .------------------------.      .------------------------.
 *           | BSPC | LSFT | SYMB/SPC |      | NAV/ENT  | RGUI | DEL  |
 *           '------------------------'      '------------------------'
 */
[BASE] = LAYOUT_gergoplex(
    KC_Q, KC_D, KC_R,    KC_W,    KC_B,  	                KC_J,   KC_F,    KC_U,    KC_P,   KC_SCLN,
    KC_A, KC_S, KC_H,    KC_T,    KC_G,  	                KC_Y,   KC_N,    KC_E,    KC_O,   KC_I,
    KC_Z, KC_X, KC_M,    KC_C,    KC_V,  	                KC_K,   KC_L,    KC_COMM, KC_DOT, KC_SLSH,
 	            KC_BSPC, KC_LSFT, Z_SPLT,	                Z_ENLT, KC_RGUI, KC_DEL																// Right
    ),

/* Keymap 1: Pad/Function layer
 * ,-----------------------------.       ,-------------------------------.
 * |  1   |  2  |  3  |  4  |  5  |      |  6  |  7  |  8  |  9  |   0   |
 * |-----+-----+-----+-----+------|      |-------------------------------|
 * |  F1  | F2  | F3  | F4  |  F5 |      | LFT | DWN | UP  | RGT | VOLUP |
 * |-----+-----+-----+-----+------+	     |-------------------------------|
 * |  F6  | F7  | F8  | F9  | F10 |      |MLFT | MDWN| MUP | MRGT| VOLDN |
 * `------+-----+-----+------+----'	     `-------------------------------'
 *  				.-----------------.           .-----------------.
 *  				| F11 | F12|	  |       		|     | PLY | SKP |
 *  				'-----------------'           '-----------------'
 */
[SYMB] = LAYOUT_gergoplex(
    KC_1,  KC_2,  KC_3,  KC_4,  KC_5,		 	KC_6,  	 KC_7, 	  KC_8,    KC_9,    KC_0,
    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   		KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TAB,
    KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,  		KC_PGUP, KC_END,  KC_HOME, KC_PGDN, Z_MALT,
     			  KC_F11,KC_F12,____,  	        Z_MSFT,  Z_MGUI, Z_MCTL
    ),


/* Keymap 2: Symbols layer
 * ,-----------------------------.       ,--------------------------------.
 * |  !   |  @  |  {  |  }  |       |  `  |  ~  |     |     |    \   |
 * |-----+-----+-----+-----+------|      |--------------------------------|
 * |  #   |  $  |  (  |  )  | LMB |      |  +  |  -  |  /  |  *  |    '   |
 * |-----+-----+-----+-----+------+		   |--------------------------------|
 * |  %   |  ^  |  [  |  ]  | RMB |      |  &  |  =  |  ,  |  .  |   -    |
 * `------+-----+-----+------+----'		   `--------------------------------'
 *  				.-----------------.           .------------------.
 *  				|MMB |  ;  	|	 = 	|						|  =  |  ;  |  DEL |
 *  				'-----------------'           '------------------'
 */
[NAV] = LAYOUT_gergoplex(
    KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,   KC_GRV,  KC_TILD, KC_TRNS, KC_TRNS, KC_BSLS,
    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_QUOT,
    KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,   KC_AMPR, KC_EQL,  KC_COMM, KC_DOT,  KC_MINS,
                      KC_F11, KC_F12, KC_EQL,   KC_EQL,  KC_SCLN, KC_DEL
    )
};
