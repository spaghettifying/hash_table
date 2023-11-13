#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "hash_table.h"

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

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); // initialise array of items with NULL bytes
    return ht;
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

int main() {
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
    return 0;
}