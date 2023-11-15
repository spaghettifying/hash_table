#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

// key value pairs
typedef struct {
    char *key;
    char *value;
} ht_item;

// hash table
typedef struct {
    int base_size;
    int size;
    int count;
    ht_item **items; // array of pointers to items
} ht_hash_table;

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);

#endif //HASH_TABLE_HASH_TABLE_H
