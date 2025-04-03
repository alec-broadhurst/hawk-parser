#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct Node {
    char* key;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;
    int size;        // Current number of buckets
    int count;       // Number of elements
    float load_factor; // When to resize
} HashMap;

HashMap newHashMap();
void insert(HashMap* table, const char* key);
int variableLookup(HashMap* table, const char* key);
void freeHashMap(HashMap* table);
void resizeHashMap(HashMap* table);

#endif
