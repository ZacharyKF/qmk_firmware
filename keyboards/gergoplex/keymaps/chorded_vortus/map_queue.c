#include "map_queue.h"

map_queue_t map = {
    0,
    0,
    {0},
    {0}
};

void set_entry(map_queue_t* map, uint64_t* key_in_binary, uint16_t* key_value){
    for(short i = 0; i < MAX_MAP_QUEUE_SIZE; i++){
        if (map->key_in_binary[i] == 0){
            map->key_in_binary[i] = *key_in_binary;
            map->key_value[i] = *key_value;
        }
    }
}

uint16_t get_and_remove_entry(map_queue_t* map, uint64_t* key_in_binary){
    for(short i = 0; i < MAX_MAP_QUEUE_SIZE; i++){
        if (map->key_in_binary[i] == *key_in_binary){
            map->key_in_binary[i] = 0;
            uint16_t tmp = map->key_value[i];
            map->key_value[i] = 0;
            return tmp;
        }
    }
    return -1;
}
