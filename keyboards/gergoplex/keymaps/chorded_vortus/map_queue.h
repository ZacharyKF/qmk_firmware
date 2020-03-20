#pragma once
#include <stdint.h>

#define MAX_MAP_QUEUE_SIZE 10

typedef struct {
    short head;
    short tail;
    uint64_t key_in_binary[MAX_MAP_QUEUE_SIZE];
    uint16_t key_value[MAX_MAP_QUEUE_SIZE];
} map_queue_t;

map_queue_t map;

void set_entry(map_queue_t* map, uint64_t* key_in_binary, uint16_t* key_value);

uint16_t get_and_remove_entry(map_queue_t* map, uint64_t* key_in_binary);
