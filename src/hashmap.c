#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.h"

#define INITIAL_SIZE 16
#define LOAD_FACTOR 0.75

// djb2 hash function
unsigned int hash(const char* key, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}

HashMap newHashMap() {
    HashMap table;
    table.buckets = (Node**)calloc(INITIAL_SIZE, sizeof(Node*));
    table.size = INITIAL_SIZE;
    table.count = 0;
    table.load_factor = LOAD_FACTOR;
    return table;
}

void resizeHashMap(HashMap* table) {
    int newSize = table->size * 2;
    Node** newBuckets = (Node**)calloc(newSize, sizeof(Node*));

    for (int i = 0; i < table->size; i++) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;
            unsigned int index = hash(current->key, newSize);
            current->next = newBuckets[index];
            newBuckets[index] = current;
            current = next;
        }
    }

    free(table->buckets);
    table->buckets = newBuckets;
    table->size = newSize;
}

void insert(HashMap* table, const char* key) {
    float current_load = (float)(table->count + 1) / table->size;
    if (current_load > table->load_factor) {
        resizeHashMap(table);
    }

    unsigned int index = hash(key, table->size);
    Node* current = table->buckets[index];

    // Check for duplicates
    while (current) {
        if (strcmp(current->key, key) == 0) {
            fprintf(stderr, "Error: Duplicate key %s\n", key);
            exit(1);
        }
        current = current->next;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    table->count++;
}

int variableLookup(HashMap *table, const char *key) {
    unsigned int index = hash(key, table->size);
    Node *current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return 1; // Found
        }
        current = current->next;
    }
    return 0; // Not found
}

void freeHashMap(HashMap *table) {
    for (int i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->buckets);
    table->size = 0;
    table->count = 0;
}
