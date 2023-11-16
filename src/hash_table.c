#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/hash_table.h"
#include "../include/prime.h"

/* Globals */
// HT_DELETED_ITEM is used to mark a bucket containing a deleted item
static ht_item HT_DELETED_ITEM = {NULL, NULL};

// HT_PRIMEs are parameters in the hashing algorithm
static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

/* Static functions */

/// Creates a new item at key 'k'
/// \param k key to create item at
/// \param v value to assign to key
/// \return new item with key 'k' and value 'v'
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k); // copy k and v in new chunk of memory
    i->value = strdup(v);
    return i;
}

/// Deletes given item
/// \param i item to delete
static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

/// Returns the hash of 's', an int between 0 and 'm'
/// \param s string to hash
/// \param a
/// \param m upper limit of hash
/// \return hash of 's'
static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int string_len = strlen(s);
    for (int i = 0; i < string_len; ++i) {
        hash += (long) pow(a, string_len - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}


static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/// Creates a new empty hash table with given size index
/// \param size_index size index for new table
/// \return newly created hash table
static ht_hash_table* ht_new_sized(const int size_index) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    if (!ht) {
        fprintf(stderr, "ht_new_sized ht malloc failed");
        exit(1);
    }
    ht->size_index = size_index;

    const int base_size = 50 << ht->size_index;
    ht->size = next_prime(base_size);

    ht->count = 0;
    ht->items = calloc((size_t) ht->size, sizeof(ht_item*));
    if (!ht->items) {
        fprintf(stderr, "ht_new_sized ht->items calloc failed");
        exit(1);
    }
    return ht;
}

/// Resizes given hash table in given direction
/// \param ht hash table to resize
/// \param direction direction to resize in
static void ht_resize(ht_hash_table* ht, const int direction) {
    const int new_size_index = ht->size_index + direction;
    if (new_size_index < 0) {
        return; // don't resize down the smallest hash table
    }
    ht_hash_table *new_ht = ht_new_sized(new_size_index);
    for (int i = 0; i < ht->size; ++i) {
        ht_item *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    // pass new_ht and ht's properties
    ht->size_index = new_ht->size_index;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item **tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(ht);
}

/* API functions */

/// Creates a new hash table
/// \return newly created hash table
ht_hash_table* ht_new() {
    return ht_new_sized(0);
}

/// Deletes hash table
/// \param ht hash table to delete
void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; ++i) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

/// Searches hash table for value assigned to 'key'
/// \param ht hash table
/// \param key key to search for
/// \return value if found, NULL if not
char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0 ) { // check whether item's key matches key searching for
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL; // indicates not found
}

/// Insert 'value' into hash table at location 'key'
/// \param ht hash table
/// \param key key to insert value at
/// \param value value to insert at key
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    // Resize if load > 0.7
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize(ht, 1);
    }
    ht_item* item = ht_new_item(key, value);

    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) { // iterate through indexes until empty bucket, jump over deleted nodes
        if (cur_item != &HT_DELETED_ITEM && strcmp(cur_item->key, key) == 0) {
            ht_del_item(cur_item); // allows to update key's value
            ht->items[index] = item;
            return;
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item; // insert into empty bucket
    ht->count++; // increment hash table's count attribute to indicate a new item has been added
}

/// Deletes key's item from hash table. Does nothing if 'key' doesn't exist.
/// \param ht hash table
/// \param key key to delete value
void ht_delete(ht_hash_table* ht, const char* key) { // cant simply delete as could be part of collision chain
    // Resize if load < 0.1
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize(ht, -1);
    }

    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM; // mark as deleted, replacing with pointer to global
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--; // decrement count attribute
}
