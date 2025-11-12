//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
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
    if (!collection || !entry || !collection->entries) return;
    
    if (collection->size >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->size < collection->capacity) {
        collection->entries[collection->size] = *entry;
        collection->size++;
    }
}

void free_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->size = 0;
        collection->capacity = 0;
    }
}

int compare_entries(const void *a, const void *b) {
    const LogEntry *entry_a = (const LogEntry *)a;
    const LogEntry *entry_b = (const LogEntry *)b;
    
    int level_cmp = strcmp(entry_a->level, entry_b->level);
    if (level_cmp != 0) return level_cmp;
    
    return strcmp(entry_a->message, entry_b->message);
}

void process_log_file(const char *filename, LogCollection *collection) {
    if (!filename || !collection) return;
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LEN];
    LogEntry stack_entry;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (parse_log_line(line, &stack_entry)) {
            int found = 0;
            for (size_t i = 0; i < collection->size; i++) {
                if (strcmp(collection->entries[i].timestamp, stack_entry.timestamp) == 0 &&
                    strcmp(collection->entries[i].level, stack_entry.level) == 0 &&
                    strcmp(collection->entries[i].message, stack_entry.message) == 0) {
                    collection->entries[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                add_log_entry(collection, &stack_entry);
            }
        }
    }
    
    fclose(file);
}

void print_summary(const LogCollection *collection) {
    if (!collection || !collection->entries) return;
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < collection->size; i++) {
        const LogEntry *entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count += entry->count;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count += entry->count;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count += entry->count;
        }
    }
    
    printf("Total unique log patterns: %zu\n", collection->size);
    printf("ERROR entries: %u\n", error_count);
    printf