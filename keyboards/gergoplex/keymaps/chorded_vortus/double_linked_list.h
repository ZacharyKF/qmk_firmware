#pragma once
#include <stdint.h>

typedef struct {
    // Previous and next node in the list
    list_node* prev;
    list_node* next;

    // Binary mask for the key, and the code it wanted to send on press
    uint64_t key_mask;
    uint16_t key_code;

    // Chord that this key is associated with
    unsigned short chord_id;
} list_node;

list_node* head = NULL;

// Add a keypress to the end of our list
void add(uint64_t* key_mask, uint16_t* key_code);

// Remove a keymask, wherever it is in our list
void remove(uint64_t* key_mask);

// Get the next node
list_node* get_next(list_node* node);
