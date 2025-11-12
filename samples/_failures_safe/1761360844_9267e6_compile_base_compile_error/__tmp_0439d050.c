//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t size;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

void init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return;
    
    collection->size = 0;
    collection->capacity = initial_capacity;
}

void add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || collection->size >= MAX_ENTRIES) return;
    
    if (collection->size >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->size] = *entry;
    collection->size++;
}

void count_duplicate_entries(LogCollection *collection) {
    if (!collection || collection->size < 2) return;
    
    for (size_t i = 0; i < collection->size - 1; i++) {
        if (collection->entries[i].count == 0) continue;
        
        for (size_t j = i + 1; j < collection->size; j++) {
            if (collection->entries[j].count == 0) continue;
            
            if (strcmp(collection->entries[i].timestamp, collection->entries[j].timestamp) == 0 &&
                strcmp(collection->entries[i].level, collection->entries[j].level) == 0 &&
                strcmp(collection->entries[i].message, collection->entries[j].message) == 0) {
                collection->entries[i].count++;
                collection->entries[j].count = 0;
            }
        }
    }
}

void print_log_summary(const LogCollection *collection) {
    if (!collection) return;
    
    printf("Log Analysis Summary:\n");
    printf("=====================\n");
    
    uint32_t total_entries = 0;
    uint32_t unique_entries = 0;
    
    for (size_t i = 0; i < collection->size; i++) {
        if (collection->entries[i].count > 0) {
            unique_entries++;
            total_entries += collection->entries[i].count;
            printf("Entry %zu: [%s] %s - %s (count: %u)\n", 
                   i + 1, collection->entries[i].timestamp, 
                   collection->entries[i].level, collection->entries[i].message,
                   collection->entries[i].count);
        }
    }
    
    printf("\nTotal entries: %u\n", total_entries);
    printf("Unique entries: %u\n", unique_entries);
}

void cleanup_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->size = 0;
        collection->capacity = 0;
    }
}

int main() {
    char input_line[MAX_LINE_LENGTH];
    LogCollection collection;
    
    init_log_collection(&collection, 10);
    if (!collection.entries) {
        printf("Error: Failed to initialize log collection\n");
        return 1;
    }
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(input_line, sizeof(input_line), stdin)) {
        if (strlen(input_line) > 0 && input_line[strlen(input_line) - 1] == '\n') {
            input_line[strlen(input_line) - 1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (