#include "double_linked_list.h"
#include <limits.h>

void add(uint64_t* key_mask, uint16_t* key_code){
    list_node* new_node = malloc(sizeof(list_node));
    new_node->key_mask = *key_mask;
    new_node->key_code = *key_code;
    new_node->chord_id = USHRT_MAX;
    if (head == NULL){
        head = new_node;
    } else {
        list_node* prev = head;
        while(prev->next != NULL){
            prev = head->next;
        }
        prev->next = new_node;
        new_node->prev = prev;
    }
}

void remove(uint64_t* key_mask){
    if (head != NULL){
        list_node* remove = head;
        while(remove != NULL && remove->key_mask != *key_mask){
            remove = remove->next;
        }
        if (remove->key_mask == *key_mask){
            if(remove->next != NULL){
                remove->next->prev = remove->prev;
            }
            if(remove->prev != NULL){
                remove->prev->next = remove->next;
            }
            if(remove == head){
                if (remove->next){
                    head = remove->next;
                } else {
                    head = NULL;
                }
            }
            free(remove);
        }
    }
}

list_node* get_next(list_node* node){
    if (node == NULL){
        return head;
    } else {
        return node->next;
    }
}
