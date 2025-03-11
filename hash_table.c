//
// Created by Kok on 3/9/25.
//

#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "linked_list.h"

hash_table_item_t *hash_table_item_create(char *key, void *value, size_t value_size) {
    hash_table_item_t *item = malloc(sizeof(hash_table_item_t));
    item->key = malloc(strlen(key) + 1);
    if (item->key == NULL) {
        free(item);
        return NULL;
    }

    item->value = malloc(value_size);
    if (item->value == NULL) {
        free(item->key);
        free(item);
        return NULL;
    }

    item->value_size = value_size;
    strcpy(item->key, key);
    memcpy(item->value, value, value_size);

    return item;
}

hash_table_t *hash_table_create(int size) {
    hash_table_t *table = malloc(sizeof(hash_table_t));
    table->size = size;
    table->count = 0;
    table->items = malloc(sizeof(hash_table_item_t*) * table->size);

    for (int i = 0; i < table->size; i++) table->items[i] = NULL;

    table->overflow_buckets = hash_table_overflow_buckets_create(table->size);

    return table;
}

// FNV-1a hash function
long hash_table_create_hash(const char *key) {
    const uint32_t FNV_PRIME = 16777619;
    const uint32_t FNV_OFFSET_BASIS = 2166136261;

    long hash = FNV_OFFSET_BASIS;

    for (const unsigned char *p = (const unsigned char *)key; *p; p++) {
        hash ^= *p;
        hash *= FNV_PRIME;
    }

    return hash;
}

int handle_collision(const hash_table_t *table, hash_table_item_t *item, const int index) {
    linked_list_item_t *list_item = malloc(sizeof(linked_list_item_t));

    list_item->id = malloc(strlen(item->key) + 1);
    if (list_item->id == NULL) {
        free(list_item);
        return -1;
    }

    list_item->value = malloc(item->value_size);
    if (list_item->value == NULL) {
        free(list_item->id);
        free(list_item);
        return -1;
    }

    list_item->value_size = item->value_size;
    list_item->should_free = true;

    strcpy(list_item->id, item->key);
    memcpy(list_item->value, item->value, item->value_size);

    if (table->overflow_buckets[index] == NULL) {
        linked_list_t *node = list_allocate();
        node->item = list_item;
        node->next = NULL;
        table->overflow_buckets[index] = node;
    } else {
        table->overflow_buckets[index] = list_insert(table->overflow_buckets[index], list_item);
    }

    return 0;
}

void *hash_table_search(hash_table_t *table, char *key) {
    if (table == NULL) {
        perror("Table is NULL!");
        return NULL;
    }

    const int index = (int)(hash_table_create_hash(key) % table->size);

    if (table->items[index] != NULL) {
        if (strcmp(table->items[index]->key, key) == 0) {
            return table->items[index]->value;
        }
        if (table->overflow_buckets[index] != NULL) {
            linked_list_t *curr_node = table->overflow_buckets[index];
            list_read(curr_node, key);
        }
    }
    return NULL;
}

int hash_table_insert(hash_table_t *table, hash_table_item_t *item) {
    if (table == NULL || item->key == NULL || item->value == NULL) return -1;

    const int index = (int)(hash_table_create_hash(item->key) % table->size);

    if (table->items[index] == NULL) {
        if (table->count >= table->size) {
            perror("Hash table is full!");
            return -1;
        }
        table->items[index] = item;
        table->count++;
    } else {
        if (strcmp(item->key, table->items[index]->key) == 0) {
            void *temp = realloc(table->items[index]->value, item->value_size);
            if (temp == NULL) {
                perror("Failed to resize already available item in the hash table!");
                return -1;
            }
            table->items[index]->value = temp;

            memcpy(table->items[index]->value, item->value, item->value_size);
            table->items[index]->value_size = item->value_size;
        } else {
            return handle_collision(table, item, index);
        }
    }

    return 0;
}

int hash_table_delete(hash_table_t *table, char *key) {
    if (table == NULL) {
        perror("Table is NULL!");
        return -1;
    }

    const int index = (int)(hash_table_create_hash(key) % table->size);

    hash_table_item_t *item = table->items[index];
    if (item == NULL) return - 1;

    linked_list_t *head = table->overflow_buckets[index];

    if (head == NULL && strcmp(table->items[index]->key, key) == 0) {
        hash_table_free_item(item);
        table->items[index] = NULL;
        table->count--;
        return 0;
    }

    if (head != NULL) {
        if (strcmp(table->items[index]->key, key) == 0) {
            hash_table_free_item(item);
            linked_list_t *node = head;
            head = head->next;
            node->next = NULL;
            table->items[index] = hash_table_item_create(node->item->id, node->item->value, node->item->value_size);
            list_node_free(node);
            table->overflow_buckets[index] = head;
            table->count--;
            return 0;
        }

        linked_list_t *new_head = list_remove(head, key);
        if (new_head != head) {
            table->overflow_buckets[index] = new_head;
            table->count--;
            return 0;
        }
    }

    return -1;
}

linked_list_t **hash_table_overflow_buckets_create(const int table_size) {
    linked_list_t **buckets = malloc(sizeof(linked_list_t*) * table_size);
    for (int i = 0; i < table_size; i++) {
        buckets[i] = NULL;
    }

    return buckets;
}

void hash_table_overflow_buckets_free(const hash_table_t *table) {
    if (table == NULL || table->overflow_buckets == NULL) return;

    for (int i = 0; i < table->size; i++) {
        if (table->overflow_buckets[i] != NULL) list_node_free(table->overflow_buckets[i]);
    }

    free(table->overflow_buckets);
}

void hash_table_free_item(hash_table_item_t *item) {
    if (item->key != NULL) free(item->key);
    if (item->value != NULL) free(item->value);
    free(item);
}

void hash_table_free(hash_table_t *table) {
    if (table == NULL || table->items == NULL) return;

    for (int i = 0; i < table->count; i++) {
        if (table->items[i] != NULL) hash_table_free_item(table->items[i]);
    }
    hash_table_overflow_buckets_free(table);
    free(table->items);
    free(table);
}
