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
    KC_Q,   KC_D,   KC_R,    KC_W, KC_B,    KC_J,   KC_F,   KC_U,    KC_P,   KC_SCLN,
    KC_A,   KC_S,   KC_H,    KC_T, KC_G,    KC_Y,   KC_N,   KC_E,    KC_O,   KC_I,
    KC_Z,   KC_X,   KC_M,    KC_C, KC_V,    KC_K,   KC_L,   KC_COMM, KC_DOT, KC_SLASH,
            KC_BSPC,KC_LSFT, KC_ENT,        KC_SPC, KC_TAB, KC_DEL
    )
};

#define KEYBUFFER 10

// Buffer to allocate list_nodes and depress codes
struct list_node buffer[KEYBUFFER];
uint16_t depressBuffer[KEYBUFFER];

// Pointer for allocation of list_nodes and depress buffer
uint16_t bufferHead = 0u;
uint16_t depressHead = 0u;
uint16_t depressTail = 0u;

uint64_t new_key_mask = 0llu;
uint64_t storedCodes = 0llu;
uint64_t code_mask = 0llu;

uint64_t temp_mask = 0llu;
struct list_node* temp = NULL;

struct list_node* head = NULL;
struct list_node* tail = NULL;

uint16_t chord_code = 0u;
uint16_t suffix_code = 0u;

struct list_node* new_node = NULL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // Translate the row-column to a bitwise position ((row + (width * col)) - 1)
    new_key_mask = (1llu << ((record->event.key.row + (MATRIX_ROWS * record->event.key.col))));

    // Do the initial modification of the list
    if (record->event.pressed){

        // This is just here to prevent some worst case scenarios
        if (recordPresent(new_key_mask) == true){
            return false;
        }

        // Add the node
        new_node = add_node(&new_key_mask, &keycode);

        // If there's a previous node, then we may need to do some special handling
        //   We need some work here to deal with the whole issue of a previous sent key potentially being a chord with the new key
        if (new_node->prev != NULL){

            // First combine the current node's mask with the previous unsent keys
            code_mask = 0;
            temp = new_node;
            while(temp != NULL && !temp->sent){
                code_mask = code_mask | temp->key_mask;
                temp = temp->prev;
            }

            // Check if we make a chord or suffix
            chord_code = getChordCode(&code_mask);
            suffix_code = getSuffixCode(&code_mask);

            // Before anything else we need to double check if we're part of the previous node's chord
            if(new_node->prev->sent && (chords[new_node->prev->chord_id] & new_node->key_mask) > 0){

                // We need to double check if we're the last required member of the chord, so first grab the rest of the cord
                temp_mask = 0;
                temp = new_node->prev;
                while(temp != NULL && temp->chord_id == new_node->prev->chord_id){
                    temp_mask = temp_mask | temp->key_mask;
                    temp = temp->prev;
                }

                // If we are, then we need to override the chord_code and suffix value
                if (((temp_mask | new_node->key_mask) ^ chords[new_node->prev->chord_id]) == 0){
                    chord_code = new_node->prev->chord_id;
                    suffix_code = USHRT_MAX;
                }
            }

            // If combined we've made a new suffix, then mark our current node as a suffix and exit
            if (suffix_code < USHRT_MAX){

                // If we make a chord, then update us and the previous entries
                if (chord_code < USHRT_MAX){
                    temp = new_node;
                    while(temp != NULL && !temp->sent){
                        temp->chord_id = chord_code;
                        temp = temp->prev;
                    }
                }

                new_node->chord_suffix = true;
                return false;

            // Otherwise we need to send stuff
            } else {

                // If we've made a new chord, then send it and update all the other chord members
                if (chord_code < USHRT_MAX){
                    register_code16(chords_codes[chord_code]);
                    temp = new_node;
                    while(temp != NULL && !temp->sent){
                        temp->chord_id = chord_code;
                        temp->sent = true;
                        temp = temp->prev;
                    }

                    return false;
                } else {

                    // If the previous node is a chord, then send it
                    if (new_node->prev->chord_id < USHRT_MAX){
                        temp = new_node->prev;
                        register_code16(chords_codes[temp->chord_id]);
                        while(temp != NULL && !temp->sent){
                            temp->sent = true;
                            temp = temp->prev;
                        }

                    // Otherwise we need to send all unsent keys
                    } else {

                        // Get the oldest unsent key
                        temp = head;
                        while (temp->sent){
                            temp = temp->next;
                        }

                        // Walk up to the current node sending the codes
                        while(temp->next != NULL){
                            register_code16(temp->key_code);
                            temp->sent = true;
                            temp = temp->next;
                        }
                    }

                    // If our current node is not a suffix, then send it as well.
                    if (!checkForSuffix(new_node)){

                        register_code16(new_node->key_code);
                        new_node->sent = true;
                        return false;
                    } else {

                        return false;
                    }
                }
            }

        // Otherwise we need to act based on if the current key is a suffix
        } else {

            // Check if the node is a suffix, if we are then don't do anything, otherwise send
            if (!checkForSuffix(new_node)){

                register_code16(new_node->key_code);
                new_node->sent = true;
                return false;
            } else {

                return false;
            }
        }

    } else {

        // Grab the node that has been depressed
        new_node = get_depressed(&new_key_mask);

        // Safety check, shouldn't occur
        if (new_node != NULL){

            // If we're part of a chord then do chord handling
            if (new_node->chord_id < UINT16_MAX){

                // If we're not already marked as depressed then de-register the chord
                if (!new_node->depressed){
                    unregister_code16(chords_codes[new_node->chord_id]);

                    chord_code = new_node->chord_id;
                    temp = new_node;

                    // Walk all the way to the beginning of the chord
                    while(temp->prev != NULL && temp->chord_id == chord_code){
                        temp = temp->prev;
                    }

                    // Walk all the way to the end marking things as depressed
                    while(temp != NULL && temp->chord_id == chord_code){
                        temp->depressed = true;
                        temp = temp->next;
                    }
                }

                remove_node(new_node);
                return false;

            // If there's no chordid, and the code isn't sent, we need to register it
            } else if (!new_node->sent) {

                register_code16(new_node->key_code);
                depressBuffer[depressHead++] = new_node->key_code;

                if (depressHead == KEYBUFFER){
                    depressHead = 0;
                }

                remove_node(new_node);

                _delay_ms(1);
                return false;

            // Otherwise just de-register the current key
            } else {

                unregister_code16(new_node->key_code);
                remove_node(new_node);

                return false;
            }
        } else {

            return true;
        }
    }
}

void printNodes(void){
    struct list_node* temp = head;
    uint16_t num = 0;
    while (temp != NULL){
        dprintf("Node %u: ", num++);
        printChord(&temp->key_mask);
        dprintf(", ");
        printShort(&temp->key_code);
        dprintf("\n");
        temp = temp->next;
    }
}

uint16_t getChordCode(uint64_t* chord_mask){
    for( uint16_t i = 0; i < num_chords; i++){
        if (chords[i] == (*chord_mask)){
            return i;
        }
    }
    return USHRT_MAX;
}

uint16_t getSuffixCode(uint64_t* chord_mask){
    for( uint16_t i = 0; i < num_chords; i++){
        if (chords[i] != (*chord_mask) && (chords[i] & (*chord_mask)) == (*chord_mask)){
            return i;
        }
    }
    return USHRT_MAX;
}

bool checkForSuffix(struct list_node* node){
    node->chord_suffix = getSuffixCode(&node->key_mask) < USHRT_MAX;
    return node->chord_suffix;
}

void matrix_scan_user(void){

    while (depressBuffer[depressTail] > 0){
        unregister_code16(depressBuffer[depressTail]);
        depressBuffer[depressTail++] = 0;
        if(depressTail == KEYBUFFER){
            depressTail = 0;
        }
    }
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
//   debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

struct list_node* add_node(uint64_t* key_mask, uint16_t* key_code){

    storedCodes = storedCodes | (*key_mask);

    while(buffer[bufferHead].key_mask > 0){
        if (++bufferHead == KEYBUFFER){
            bufferHead = 0u;
        }
    }

    buffer[bufferHead].next = NULL;
    buffer[bufferHead].prev = NULL;

    buffer[bufferHead].depressed = false;
    buffer[bufferHead].sent = false;

    buffer[bufferHead].key_mask = (*key_mask);
    buffer[bufferHead].key_code = (*key_code);

    buffer[bufferHead].chord_id = USHRT_MAX;
    buffer[bufferHead].chord_suffix = false;

    if (head == NULL || tail == NULL){
        head = buffer + bufferHead;
        tail = buffer + bufferHead;
    } else {
        tail->next = buffer + bufferHead;
        buffer[bufferHead].prev = tail;
        tail = buffer + bufferHead;
    }

    return buffer + bufferHead;
}

void remove_node(struct list_node* node){

    storedCodes = storedCodes & (~(node->key_mask));

    if(node->next != NULL){
        node->next->prev = node->prev;
    }

    if(node->prev != NULL){
        node->prev->next = node->next;
    }

    if(node == head){
        head = node->next;
    }

    if(node == tail){
        tail = node->prev;
    }

    node->next = NULL;
    node->prev = NULL;

    node->depressed = false;
    node->sent = false;

    node->key_mask = 0llu;
    node->key_code = 0llu;

    node->chord_id = USHRT_MAX;
    node->chord_suffix = false;
}

struct list_node* get_depressed(uint64_t* check_key_mask){
    for(uint16_t i = 0; i < KEYBUFFER; i++){
        if (buffer[i].key_mask == (*check_key_mask)){
            return buffer + i;
        }
    }
    return NULL;
}

bool recordPresent(uint64_t key_mask) {
    return (storedCodes & key_mask) > 0;
}
