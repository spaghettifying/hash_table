#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

// key value pairs
typedef struct {
    char *key;
    char *value;
} ht_item;

// hash table
typedef struct {
    int size;
    int count;
    ht_item **items; // array of pointers to items
} ht_hash_table;

#endif //HASH_TABLE_HASH_TABLE_H
