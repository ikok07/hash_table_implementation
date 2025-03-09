//
// Created by Kok on 3/9/25.
//

#include <stdlib.h>

#include "linked_list.h"

linked_list_t *list_allocate() {
    linked_list_t *list = malloc(sizeof(linked_list_t));
    return list;
}

linked_list_item_t *list_read(linked_list_t *list, int item_id) {
    if (list == NULL) return NULL;

    linked_list_t *curr_node = list;

    while (curr_node != NULL) {
        if (curr_node->item != NULL && curr_node->item->id == item_id) return curr_node->item;
        curr_node = curr_node->next;
    }

    return NULL;
}

linked_list_t *list_insert(linked_list_t *list, linked_list_item_t *item) {
    if (list == NULL) return NULL;

    linked_list_t *new_node = list_allocate();
    if (new_node == NULL) return NULL;

    new_node->item = item;
    new_node->next = list;

    return new_node;
}

linked_list_t *list_remove(linked_list_t *list, int item_id) {
    if (list == NULL) return NULL;

    linked_list_t *prev_node = NULL;
    linked_list_t *curr_node = list;

    while (curr_node != NULL) {
        if (curr_node->item && curr_node->item->id == item_id) {
            if (prev_node == NULL) list = curr_node->next;
            else prev_node->next = curr_node->next;
            list_node_free(curr_node);
            return list;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    return list;
}

void list_node_free(linked_list_t *node) {
    if (node == NULL) return;

    if (node->item != NULL && node->item->should_free) {
        if (node->item->value != NULL) free(node->item->value);
        free(node->item);
    }

    free(node);
}
