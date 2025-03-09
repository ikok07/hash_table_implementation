//
// Created by Kok on 3/9/25.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct {
    int id;
    void *value;
    bool should_free;
} linked_list_item_t;

typedef struct linked_list {
    linked_list_item_t *item;
    struct linked_list *next;
} linked_list_t;

linked_list_t *list_allocate();
linked_list_item_t *list_read(linked_list_t *list, int item_id);
linked_list_t *list_insert(linked_list_t *list, linked_list_item_t *item);
linked_list_t *list_remove(linked_list_t *list, int item_id);

void list_node_free(linked_list_t *list);

#endif //LINKED_LIST_H
