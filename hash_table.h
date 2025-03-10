//
// Created by Kok on 3/9/25.
//

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdlib.h>

#include "linked_list.h"

typedef struct {
    char *key;
    void *value;
    size_t value_size;
} hash_table_item_t;

typedef struct {
    hash_table_item_t **items;
    linked_list_t **overflow_buckets;
    int size;
    int count;
} hash_table_t;

hash_table_t *hash_table_create(int size);
hash_table_item_t *hash_table_item_create(char *key, void *value, size_t value_size);

void *hash_table_search(hash_table_t *table, char *key);
int hash_table_insert(hash_table_t *table, hash_table_item_t *item);
int hash_table_delete(hash_table_t *table, char *key);

linked_list_t **hash_table_overflow_buckets_create(int table_size);
void hash_table_overflow_buckets_free(const hash_table_t *table);

void hash_table_free_item(hash_table_item_t *item);
void hash_table_free(hash_table_t *table);

#endif //HASH_TABLE_H
