#include <stdio.h>
#include <string.h>

#include "hash_table.h"

#define CAPACITY 50000

void print_search(hash_table_t *table, char *key)
{
    void *val;

    if ((val = hash_table_search(table, key)) == NULL)
    {
        printf("Key: %s does not exist\n", key);
        return;
    }
    printf("Key: %s, Value: %s\n", key, (char*)val);
}

int main()
{
    hash_table_t *ht = hash_table_create(CAPACITY);
    hash_table_item_t *it1 = hash_table_item_create("1", "First address", strlen("First address") + 1);
    hash_table_item_t *it2 = hash_table_item_create("2", "Second address", strlen("Second address") + 1);
    hash_table_item_t *it3 = hash_table_item_create("2", "Third address", strlen("Third address") + 1);

    if (hash_table_insert(ht, it1) != 0) {
        perror("Failed to insert item!");
        return -1;
    }
    if (hash_table_insert(ht, it2) != 0) {
        perror("Failed to insert item!");
        return -1;
    }

    // Overwrite the second item
    if (hash_table_insert(ht, it3) != 0) {
        perror("Failed to insert item!");
        return -1;
    }

    print_search(ht, "1");
    print_search(ht, "2");
    print_search(ht, "3");

    if (hash_table_delete(ht, "1") != 0) {
        perror("Failed to delete item!");
        return -1;
    }

    print_search(ht, "1");
    print_search(ht, "2");

    hash_table_free(ht);
    return 0;
}
