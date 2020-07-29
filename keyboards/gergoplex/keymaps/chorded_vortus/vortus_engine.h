#ifdef QMK_KEYBOARD_H
    #include QMK_KEYBOARD_H
#else
    #include "../../gergoplex.h"
    #include "../../config.h"
#endif

#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define TESTING

struct list_node {
    // Previous and next node in the list
    struct list_node* prev;
    struct list_node* next;

    // Marks if the the key has been sent
    bool depressed;
    bool sent;

    // Binary mask for the key, and the code it wanted to send on press
    uint64_t key_mask;
    uint16_t key_code;

    // Chord that this key is associated with and indication of if it's a suffix
    uint16_t chord_id;
    bool chord_suffix;
};

#ifdef TESTING
    void printChord(uint64_t* chord);
    short count_bits(uint64_t value);
    void printShort(uint16_t* chord);
    void printNodes(void);
#endif

uint16_t getChordCode(uint64_t* chord_mask);
uint16_t getSuffixCode(uint64_t* chord_mask);
bool checkForSuffix(struct list_node* node);

// Add a keypress to the end of our list
struct list_node* add_node(uint64_t* key_mask, uint16_t* key_code);

// Remove a keymask, wherever it is in our list
void remove_node(struct list_node* node);

// Get the next node
struct list_node* get_next_node(struct list_node* node);

// Marks the node as depressed, used to finish a combo
struct list_node* get_depressed(uint64_t* check_key_mask);

// Tells you if the record is alreay present
bool recordPresent(uint64_t key_mask);
