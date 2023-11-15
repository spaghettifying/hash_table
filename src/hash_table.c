#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "hash_table.h"
#include "prime.h"

/* Globals */
// HT_DELETED_ITEM is used to mark a bucket containing a deleted item
static ht_item HT_DELETED_ITEM = {NULL, NULL};

// HT_PRIMEs are parameters in the hashing algorithm
static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

/* Static functions */
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k); // copy k and v in new chunk of memory
    i->value = strdup(v);
    return i;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

// return an even dist of bucket indexes for an average set of inputs
static int ht_hash(const char* s, const int a, const int m) {
    int hash = 0;
    const int string_len = strlen(s);
    for (int i = 0; i < string_len; ++i) {
        hash += (long)pow(a, string_len - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    if (!ht) {
        fprintf(stderr, "ht_new_sized ht malloc failed");
        exit(1);
    }
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = calloc((size_t) ht->size, sizeof(ht_item*));
    if (!ht->items) {
        fprintf(stderr, "ht_new_sized ht->items calloc failed");
        exit(1);
    }
    return ht;
}

static void ht_resize(ht_hash_table* ht, const int base_size) {

}

/* API functions */
ht_hash_table* ht_new() {
    return ht_new_sized(0);
}

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

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
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

void ht_delete(ht_hash_table* ht, const char* key) { // cant simply delete as could be part of collision chain
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

int main() {
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
    return 0;
}
